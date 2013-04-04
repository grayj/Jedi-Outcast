// tr_font.c
// 
//
#include "tr_local.h"
//#include "../qcommon/qcommon.h"	

#include "../qcommon/sstring.h"	// stl string class won't compile in here (MS shite), so use Gil's.
#include "tr_local.h"
#include "tr_font.h"


#pragma warning (push, 3)	//go back down to 3 for the stl include
#include <vector>
#include <map>
//#include <list>
//#include <string>
#pragma warning (pop)

using namespace std;


inline int Round(float value)
{
	return((int)floorf(value + 0.5f));
}

int							fontIndex;	// entry 0 is reserved index for missing/invalid, else ++ with each new font registered
vector<CFontInfo *>			fontArray;
typedef map<sstring_t, int>	fontIndexMap_t;
							fontIndexMap_t fontIndexMap;
//paletteRGBA_c				lastcolour;

// =============================== some korean stuff =======================================

#define KSC5601_HANGUL_HIBYTE_START		0xB0	// range is...
#define KSC5601_HANGUL_HIBYTE_STOP		0xC8	// ... inclusive
#define KSC5601_HANGUL_LOBYTE_LOBOUND	0xA0	// range is...
#define KSC5601_HANGUL_LOBYTE_HIBOUND	0xFF	// ...bounding (ie only valid in between these points, but NULLs in charsets for these codes)
#define KSC5601_HANGUL_CODES_PER_ROW	96		// 2 more than the number of glyphs

//extern qboolean Language_IsKorean( void );
qboolean Language_IsKorean( void )
{
	return qfalse;	// multiplayer does not support asian languages
}

static inline bool Korean_ValidKSC5601Hangul( byte _iHi, byte _iLo )
{
	return (_iHi >=KSC5601_HANGUL_HIBYTE_START		&&
			_iHi <=KSC5601_HANGUL_HIBYTE_STOP		&&
			_iLo > KSC5601_HANGUL_LOBYTE_LOBOUND	&&
			_iLo < KSC5601_HANGUL_LOBYTE_HIBOUND
			);
}

static inline bool Korean_ValidKSC5601Hangul( unsigned int uiCode )
{
	return Korean_ValidKSC5601Hangul( uiCode >> 8, uiCode & 0xFF );
}


// takes a KSC5601 double-byte hangul code and collapses down to a 0..n glyph index...
// Assumes rows are 96 wide (glyph slots), not 94 wide (actual glyphs), so I can ignore boundary markers
//
// (invalid hangul codes will return 0)
//
static int Korean_CollapseKSC5601HangulCode(unsigned int uiCode)
{
	if (Korean_ValidKSC5601Hangul( uiCode ))
	{
		uiCode -= (KSC5601_HANGUL_HIBYTE_START * 256) + KSC5601_HANGUL_LOBYTE_LOBOUND;	// sneaky maths on both bytes, reduce to 0x0000 onwards
		uiCode  = ((uiCode >> 8) * KSC5601_HANGUL_CODES_PER_ROW) + (uiCode & 0xFF);
		return uiCode;
	}
	return 0;
}

// ======================== some taiwanese stuff ==============================

// (all ranges inclusive for Big5)...
//
#define BIG5_HIBYTE_START0		0xA1	// (misc chars + level 1 hanzi)
#define BIG5_HIBYTE_STOP0		0xC6	// 
#define BIG5_HIBYTE_START1		0xC9	// (level 2 hanzi)
#define BIG5_HIBYTE_STOP1		0xF9	// 
#define BIG5_LOBYTE_LOBOUND0	0x40	// 
#define BIG5_LOBYTE_HIBOUND0	0x7E	// 
#define BIG5_LOBYTE_LOBOUND1	0xA1	// 
#define BIG5_LOBYTE_HIBOUND1	0xFE	// 
#define BIG5_CODES_PER_ROW		160		// 3 more than the number of glyphs

//extern qboolean Language_IsTaiwanese( void );
qboolean Language_IsTaiwanese( void )
{
	return qfalse;	// multiplayer does not support asian languages
}

static bool Taiwanese_ValidBig5Code( unsigned int uiCode )
{
	if (	(uiCode >= ((BIG5_HIBYTE_START0<<8)|BIG5_LOBYTE_LOBOUND0) && uiCode <= ((BIG5_HIBYTE_STOP0<<8)|BIG5_LOBYTE_HIBOUND0))
		||	(uiCode >= ((BIG5_HIBYTE_START1<<8)|BIG5_LOBYTE_LOBOUND0) && uiCode <= ((BIG5_HIBYTE_STOP1<<8)|0xD5))	// no meaningful equate for this, it's just end-of-glyphs for highest row
		)
	{
		const byte _iLo = uiCode & 0xFF;

		if ( (_iLo >= BIG5_LOBYTE_LOBOUND0 && _iLo <= BIG5_LOBYTE_HIBOUND0) ||
			 (_iLo >= BIG5_LOBYTE_LOBOUND1 && _iLo <= BIG5_LOBYTE_HIBOUND1)
			)
		{
			return true;
		}
	}

	return false;
}


// takes a BIG5 double-byte code (including level 2 hanzi) and collapses down to a 0..n glyph index...
// Assumes rows are 160 wide (glyph slots), not 157 wide (actual glyphs), so I can ignore boundary markers
//
// (invalid big5 codes will return 0)
//
static int Taiwanese_CollapseBig5Code( unsigned int uiCode )
{
	if (Taiwanese_ValidBig5Code( uiCode ))
	{			
		uiCode -= (BIG5_HIBYTE_START0 * 256) + BIG5_LOBYTE_LOBOUND0;	// sneaky maths on both bytes, reduce to 0x0000 onwards
		if ( (uiCode & 0xFF) >= (BIG5_LOBYTE_LOBOUND1-1)-BIG5_LOBYTE_LOBOUND0)
		{
			uiCode -= ((BIG5_LOBYTE_LOBOUND1-1) - (BIG5_LOBYTE_HIBOUND0+1)) -1;
		}
		uiCode = ((uiCode >> 8) * BIG5_CODES_PER_ROW) + (uiCode & 0xFF);
		return uiCode;
	}
	return 0;
}


// ============================================================================


// takes char *, returns integer char at that point, and advances char * on by enough bytes to move
//	past the letter (either western 1 byte or Asian multi-byte)...
//
unsigned int AnyLanguage_ReadCharFromString( const char **ppsText )
{	
	const byte *psString = (const byte *) *ppsText;	// avoid sign-promote bug
	unsigned int uiLetter;

	// at some stage I can put other MBCS languages here I guess, but for now...
	//
	if ( (Language_IsKorean()	&& Korean_ValidKSC5601Hangul( psString[0], psString[1] )) ||
		 (Language_IsTaiwanese()&& Taiwanese_ValidBig5Code ((psString[0] * 256) + psString[1]))
		 )
	{
		uiLetter = (psString[0] * 256) + psString[1];
		*ppsText += 2;
	}
	else
	{
		uiLetter = psString[0];	
		*ppsText += 1;	// NOT ++
	}

	return uiLetter;
}

// ======================================================================

CFontInfo::CFontInfo(const char *fontName)
{
	int			len, i;
	void		*buff;
	dfontdat_t	*fontdat;

	len = ri.FS_ReadFile(fontName, NULL);
	if (len == sizeof(dfontdat_t))
	{
		ri.FS_ReadFile(fontName, &buff);
		fontdat = (dfontdat_t *)buff;

		for(i = 0; i < GLYPH_COUNT; i++)
		{
			mGlyphs[i] = fontdat->mGlyphs[i];
		}
		mPointSize = fontdat->mPointSize;
		mHeight = fontdat->mHeight;
		mAscender = fontdat->mAscender;
		mDescender = fontdat->mDescender;
		mAsianHack = fontdat->mKoreanHack;
		mbRoundCalcs = !!strstr(fontName,"ergo");

		ri.FS_FreeFile(buff);
	}
	else
	{
		mHeight = 0;
		mShader = 0;
	}

	Q_strncpyz(m_sFontName, fontName, sizeof(m_sFontName));
	COM_StripExtension( m_sFontName, m_sFontName );	// so we get better error printing if failed to load shader (ie lose ".fontdat")
	mShader = RE_RegisterShaderNoMip(m_sFontName);

	FlagNoAsianGlyphs();
	UpdateAsianIfNeeded(true);

	// finished...
	fontArray.resize(fontIndex + 1);
	fontArray[fontIndex++] = this;
}

//extern int Language_GetIntegerValue(void);
static int Language_GetIntegerValue(void)
{
	return 0;	// irrlevant in this codebase since multiplayer doesn't support asian chars
}

void CFontInfo::UpdateAsianIfNeeded( bool bForceReEval /* = false */ )
{
	// if asian language, then provide an alternative glyph set and fill in relevant fields...
	//
	if (mHeight)	// western charset exists in first place?
	{
		qboolean bKorean	= Language_IsKorean();
		qboolean bTaiwanese	= Language_IsTaiwanese();

		if (bKorean || bTaiwanese)
		{
			const int iThisLanguage = Language_GetIntegerValue();

			int iCappedHeight = mHeight < 16 ? 16: mHeight;	// arbitrary limit on small char sizes because Asian chars don't squash well

			if (m_iAsianLanguageLoaded != iThisLanguage || !AsianGlyphsAvailable() || bForceReEval)
			{
				m_iAsianLanguageLoaded  = iThisLanguage;

				int iGlyphTPs = 0;
				const char *psLang = NULL;

				if (bKorean)
				{
					iGlyphTPs		= GLYPH_MAX_KOREAN_SHADERS;
					psLang			= "kor";
					m_iAsianGlyphsAcross = 32;							// hardwired for now (only one glyph set), may change later
				}
				else 
				if (bTaiwanese)
				{
					iGlyphTPs		= GLYPH_MAX_TAIWANESE_SHADERS;
					psLang			= "tai";
					m_iAsianGlyphsAcross = 64;							// hardwired for now (only one glyph set), may change later
				}


				// textures need loading...
				//
				if (m_sFontName[0])
				{
					// Use this sometime if we need to do logic to load alternate-height glyphs to better fit other fonts.
					// (but for now, we just use the one glyph set)
					//
				}
				
				for (int i = 0; i < iGlyphTPs; i++)
				{
					// (Note!!  assumption for S,T calculations: all Asian glyph textures pages are square except for last one)
					//
					char sTemp[MAX_QPATH];
					Com_sprintf(sTemp,sizeof(sTemp), "fonts/%s_%d_1024_%d", psLang, 1024/m_iAsianGlyphsAcross, i);
					//
					// returning 0 here will automatically inhibit Asian glyph calculations at runtime...
					//
					m_hAsianShaders[i] = RE_RegisterShaderNoMip( sTemp );
				}
			
				// for now I'm hardwiring these, but if we ever have more than one glyph set per language then they'll be changed...
				//
				m_iAsianPagesLoaded = iGlyphTPs;	// not necessarily true, but will be safe, and show up obvious if something missing
				m_bAsianLastPageHalfHeight = true;

				bForceReEval = true;
			}

			if (bForceReEval)
			{			
				// now init the Asian member glyph fields to make them come out the same size as the western ones
				//	that they serve as an alternative for...
				//
				m_AsianGlyph.width			= iCappedHeight;	// square Asian chars same size as height of western set
				m_AsianGlyph.height			= iCappedHeight;	// ""
				m_AsianGlyph.horizAdvance	= iCappedHeight + (bTaiwanese?3:-1);	// Asian chars contain a small amount of space in the glyph
				m_AsianGlyph.horizOffset	= 0;				// ""
				// .. or you can use the mKoreanHack value (which is the same number as the calc below)
				m_AsianGlyph.baseline		= mAscender + ((iCappedHeight - mHeight) >> 1);
			}
		}
		else
		{
			// not using Asian...
			//
			FlagNoAsianGlyphs();
		}
	}
	else
	{			
		// no western glyphs available, so don't attempt to match asian...
		//
		FlagNoAsianGlyphs();
	}
}

// needed to add *piShader param because of multiple TPs, 
//	if not passed in, then I also skip S,T calculations for re-usable static korean glyphinfo struct...
//
const glyphInfo_t *CFontInfo::GetLetter(const unsigned int uiLetter, int *piShader /* = NULL */)
{ 	
	if ( AsianGlyphsAvailable() )
	{
		int iCollapsedAsianCode = 0;

		qboolean bTaiHack = qfalse;
		if ( Language_IsKorean() )
		{
			iCollapsedAsianCode = Korean_CollapseKSC5601HangulCode( uiLetter );
		}
		else
		if ( Language_IsTaiwanese() )
		{
			iCollapsedAsianCode = Taiwanese_CollapseBig5Code( uiLetter );
			bTaiHack = qtrue;
		}

		if (iCollapsedAsianCode)
		{
			if (piShader)
			{
				// (Note!!  assumption for S,T calculations: all asian glyph textures pages are square except for last one
				//			which may or may not be half height)
				//				
				int iTexturePageIndex = iCollapsedAsianCode / (m_iAsianGlyphsAcross * m_iAsianGlyphsAcross);

				if (iTexturePageIndex > m_iAsianPagesLoaded)
				{
					assert(0);				// should never happen
					iTexturePageIndex = 0;
				}

				iCollapsedAsianCode -= iTexturePageIndex *  (m_iAsianGlyphsAcross * m_iAsianGlyphsAcross);

				const int iColumn	= iCollapsedAsianCode % m_iAsianGlyphsAcross;
				const int iRow		= iCollapsedAsianCode / m_iAsianGlyphsAcross;				
				const bool bHalfT	= (iTexturePageIndex == (m_iAsianPagesLoaded - 1) && m_bAsianLastPageHalfHeight);
				const int iAsianGlyphsDown = (bHalfT) ? m_iAsianGlyphsAcross / 2 : m_iAsianGlyphsAcross;

				if (!bTaiHack)
				{
					// standard...
					//
					m_AsianGlyph.s  = (float)( iColumn    ) / (float)m_iAsianGlyphsAcross;
					m_AsianGlyph.t  = (float)( iRow       ) / (float)  iAsianGlyphsDown;
				}
				else
				{
					// special hack for Taiwanese stuff to avoid GL-bleed...
					//
					m_AsianGlyph.s  = (float)(((1024 / m_iAsianGlyphsAcross) * ( iColumn    ))+1) / 1024.0f;
					m_AsianGlyph.t  = (float)(((1024 / iAsianGlyphsDown    ) * ( iRow       ))+1) / 1024.0f;
				}
				m_AsianGlyph.s2 = (float)( iColumn + 1) / (float)m_iAsianGlyphsAcross;				
				m_AsianGlyph.t2 = (float)( iRow + 1   ) / (float)  iAsianGlyphsDown;

				*piShader = m_hAsianShaders[ iTexturePageIndex ];
			}
			return &m_AsianGlyph;
		}
	}

	if (piShader)
	{
		*piShader = GetShader();
	}

	return(mGlyphs + (uiLetter & 0xff)); 
}

const int CFontInfo::GetAsianCode(ulong uiLetter) const
{
	int iCollapsedAsianCode = 0;

	if (AsianGlyphsAvailable())
	{
		if ( Language_IsKorean() )
		{
			iCollapsedAsianCode = Korean_CollapseKSC5601HangulCode( uiLetter );
		}
		else
		if ( Language_IsTaiwanese() )
		{
			iCollapsedAsianCode = Taiwanese_CollapseBig5Code( uiLetter );
		}
	}

	return iCollapsedAsianCode;
}

const int CFontInfo::GetLetterWidth(unsigned int uiLetter) const
{
	if ( GetAsianCode(uiLetter) )
	{
		return m_AsianGlyph.width;
	}

	uiLetter &= 0xff;
	if(mGlyphs[uiLetter].width)
	{
		return(mGlyphs[uiLetter].width);
	}
	return(mGlyphs['.'].width);
}

const int CFontInfo::GetLetterHorizAdvance(unsigned int uiLetter) const
{
	if ( GetAsianCode(uiLetter) )
	{
		return m_AsianGlyph.horizAdvance;
	}

	uiLetter &= 0xff;
	if(mGlyphs[uiLetter].horizAdvance)
	{
		return(mGlyphs[uiLetter].horizAdvance);
	}
	return(mGlyphs['.'].horizAdvance);
}

CFontInfo *GetFont(int index)
{
	index &= SET_MASK;
	if((index >= 1) && (index < fontIndex))
	{
		CFontInfo *pFont = fontArray[index];

		if (pFont)
		{
			pFont->UpdateAsianIfNeeded();
		}

		return pFont;
	}
	return(NULL);
}


int RE_Font_StrLenPixels(const char *psText, const int iFontHandle, const float fScale)
{			
	int			x = 0, i = 0, r = 0;
	CFontInfo	*curfont;
	char		parseText[2048];

	//It gets confused about ^blah here too and reports an inaccurate length as a result
	while (psText[i] && r < 2048)
	{
		if (psText[i] == '^')
		{
			if ( (i < 1 || psText[i-1] != '^') &&
				(!psText[i+1] || psText[i+1] != '^') )
			{ //If char before or after ^ is ^ then it prints ^ instead of accepting a colorcode
				i += 2;
			}
		}

		parseText[r] = psText[i];
		r++;
		i++;
	}
	parseText[r] = 0;
	
	const char *constParseText = parseText;

	curfont = GetFont(iFontHandle);
	if(!curfont)
	{
		return(0);
	}
	while(*constParseText)
	{
		int a = curfont->GetLetterHorizAdvance( AnyLanguage_ReadCharFromString( &constParseText ));

		x += Round ( a * fScale );
	}

	return(x);
}

// not really a font function, but keeps naming consistant...
//
int RE_Font_StrLenChars(const char *psText)
{			
	// logic for this function's letter counting must be kept same in this function and RE_Font_DrawString()
	//
	int iCharCount = 0;

	while ( *psText )
	{
		// in other words, colour codes and CR/LF don't count as chars, all else does...
		//
		switch (AnyLanguage_ReadCharFromString( &psText ))
		{
			case '^':					psText++;	break;	// colour code (note next-char skip)
			case 10:								break;	// linefeed
			case 13:								break;	// return 
			default:	iCharCount++;				break;
		}
	}
	
	return iCharCount;
}

int RE_Font_HeightPixels(const int iFontHandle, const float fScale)
{			
	CFontInfo	*curfont;

	curfont = GetFont(iFontHandle);
	if(curfont)
	{
		return(Round(curfont->GetPointSize() * fScale));
	}
	return(0);
}

// iCharLimit is -1 for "all of string", else MBCS char count...
//
void RE_Font_DrawString(int ox, int oy, const char *psText, const float *rgba, const int iFontHandle, int iCharLimit, const float fScale)
{		
	int					x, y, colour, offset;
	const glyphInfo_t	*pLetter;
	qhandle_t			hShader;
	qboolean			qbThisCharCountsAsLetter;	// logic for this bool must be kept same in this function and RE_Font_StrLenChars()

	if(iFontHandle & STYLE_BLINK)
	{
		if((ri.Milliseconds() >> 7) & 1)
		{
			return;
		}
	}

/*	if (Language_IsTaiwanese())
	{
		psText = "Wp:¶}·F§a ¿p·G´µ¡A§Æ±æ§A¹³¥L­Ì»¡ªº¤@¼Ë¦æ¡C";
	}
	else
	if (Language_IsKorean())
	{
		psText = "Wp:¼îÅ¸ÀÓÀÌ´Ù ¸Ö¸°. ±×µéÀÌ ¸»ÇÑ´ë·Î ³×°¡ ÀßÇÒÁö ±â´ëÇÏ°Ú´Ù.";
	}
*/
	CFontInfo *curfont = GetFont(iFontHandle);
	if(!curfont)
	{
		return;
	}
	
	// Draw a dropshadow if required
	if(iFontHandle & STYLE_DROPSHADOW)
	{
		int i = 0, r = 0;
		char dropShadowText[1024];
		static const vec4_t v4DKGREY2 = {0.15f, 0.15f, 0.15f, 1};

		offset = Round(curfont->GetPointSize() * fScale * 0.075f);

		//^blah stuff confuses shadows, so parse it out first
		while (psText[i] && r < 1024)
		{
			if (psText[i] == '^')
			{
				if ( (i < 1 || psText[i-1] != '^') &&
					(!psText[i+1] || psText[i+1] != '^') )
				{ //If char before or after ^ is ^ then it prints ^ instead of accepting a colorcode
					i += 2;
				}
			}

			dropShadowText[r] = psText[i];
			r++;
			i++;
		}
		dropShadowText[r] = 0;
		
		RE_Font_DrawString(ox + offset, oy + offset, dropShadowText, v4DKGREY2, iFontHandle & SET_MASK, iCharLimit, fScale);
	}
	
	RE_SetColor( rgba );

	x = ox;
	oy += Round((curfont->GetHeight() - (curfont->GetDescender() >> 1)) * fScale);
	
	while(*psText)
	{
		qbThisCharCountsAsLetter = qfalse;

		unsigned int uiLetter = AnyLanguage_ReadCharFromString(&psText);	// 'psText' ptr has been advanced now
		switch( uiLetter )
		{
		case '^':
			{
			colour = ColorIndex(*psText++);
			RE_SetColor( g_color_table[colour] );
			}
			break;
		case 10:						//linefeed
			x = ox;
			oy += Round(curfont->GetPointSize() * fScale);
			break;
		case 13:						// Return
			break;
		case 32:						// Space
			qbThisCharCountsAsLetter = qtrue;
			pLetter = curfont->GetLetter(' ');
			x += Round(pLetter->horizAdvance * fScale);
			break;

		default:
			qbThisCharCountsAsLetter = qtrue;
			pLetter = curfont->GetLetter( uiLetter, &hShader );			// Description of pLetter
			if(!pLetter->width)
			{
				pLetter = curfont->GetLetter('.');
			}

			// for some reason in JK2MP we DO need these Round() calls, but in SP they cause it to go wrong... ???
			//
			// this 'mbRoundCalcs' stuff is crap, but the only way to make the font code work. Sigh...
			//
			y = oy - (curfont->mbRoundCalcs ? Round(pLetter->baseline * fScale) : pLetter->baseline * fScale);

			RE_StretchPic ( x + Round(pLetter->horizOffset * fScale), // float x
							y,								// float y
							curfont->mbRoundCalcs ? Round(pLetter->width * fScale) : pLetter->width * fScale,	// float w
							curfont->mbRoundCalcs ? Round(pLetter->height * fScale) : pLetter->height * fScale, // float h
							pLetter->s,						// float s1
							pLetter->t,						// float t1
							pLetter->s2,					// float s2
							pLetter->t2,					// float t2
							//lastcolour.c, 
							hShader							// qhandle_t hShader
							);

			x += Round(pLetter->horizAdvance * fScale);
			break;
		}		

		if (qbThisCharCountsAsLetter && iCharLimit != -1)
		{
			if (!--iCharLimit)
				break;
		}
	}
	//let it remember the old color //RE_SetColor(NULL);;
}

int RE_RegisterFont(const char *psName) 
{
	fontIndexMap_t::iterator it = fontIndexMap.find(psName);
	if (it != fontIndexMap.end() )
	{
		int iIndex = (*it).second;

		CFontInfo *pFont = GetFont(iIndex);
		if (pFont)
		{
			pFont->UpdateAsianIfNeeded();
		}
		else
		{
			return 0;	// iIndex will be 0 anyway, but this makes it clear
		}

		return iIndex;
	}

	// not registered, so...
	//
	{
		char		temp[MAX_QPATH];
		Com_sprintf(temp, MAX_QPATH, "fonts/%s.fontdat", psName);
		CFontInfo *pFont = new CFontInfo(temp);
		if (pFont->GetPointSize() > 0)
		{
			fontIndexMap[psName] = fontIndex - 1;
			return fontIndex - 1;
		}
		else
		{
			fontIndexMap[psName] = 0;	// missing/invalid
		}
	}

	return(0);
}


void R_InitFonts(void)
{
	fontIndex = 1;	// entry 0 is reserved for "missing/invalid"
}

void R_ShutdownFonts(void)
{
	for(int i = 1; i < fontIndex; i++)	// entry 0 is reserved for "missing/invalid"
	{
		delete fontArray[i];
	}
	fontIndexMap.clear();
	fontArray.clear();
	fontIndex = 1;	// entry 0 is reserved for "missing/invalid"
}

