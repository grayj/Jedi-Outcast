// Filename:-	zclip.h
//

#ifndef ZCLIP_H
#define ZCLIP_H

// I don't like doing macros without braces and with whitespace, but the compiler moans if I do these differently,
//	and since they're only for use within glColor3f() calls anyway then this is ok...  (that's my excuse anyway)
//
#define ZCLIP_COLOUR		1.0, 0.0, 1.0
#define ZCLIP_COLOUR_DIM	0.8, 0.0, 0.8


class CZClip
{
public:
	CZClip();
	~CZClip();

	int		GetTop(void);
	int		GetBottom(void);
	void	SetTop(int iNewZ);
	void	SetBottom(int iNewZ);
	void	Reset(void);
	bool	IsEnabled(void);
	bool	Enable(bool bOnOff);
	void	Paint(void);

protected:
	void	Legalise(void);

	bool	m_bEnabled;
	int		m_iZClipTop;
	int		m_iZClipBottom;
};


#endif	// #ifndef ZCLIP_H


///////////// eof ///////////////


