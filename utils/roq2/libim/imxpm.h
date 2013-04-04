/**

 **	$Header: /sdsc/dev/vis/image/imtools/v3.0/libim/src/include/RCS/imxpm.h,v 1.2 1995/06/29 00:32:03 bduggan Exp $

 **	Copyright (c) 1989-1995  San Diego Supercomputer Center (SDSC)

 **		a division of General Atomics, San Diego, California, USA

 **

 **	Users and possessors of this source code are hereby granted a

 **	nonexclusive, royalty-free copyright and design patent license to

 **	use this code in individual software.  License is not granted for

 **	commercial resale, in whole or in part, without prior written

 **	permission from SDSC.  This source is provided "AS IS" without express

 **	or implied warranty of any kind.

 **

 **	For further information contact:

 **		E-Mail:		info@sds.sdsc.edu

 **

 **		Surface Mail:	Information Center

 **				San Diego Supercomputer Center

 **				P.O. Box 85608

 **				San Diego, CA  92138-5608

 **				(619) 534-5000

 **/



/**

 **  FILE

 **	imxpm.h		-  Include file for the xpm format

 **

 **  PROJECT

 **	libim		-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	This file contains a list of color names, and associated rgb values.

 **

 **  PUBLIC CONTENTS

 **			d =defined constant

 **			f =function

 **			m =defined macro

 **			t =typedef/struct/union

 **			v =variable

 **			? =other

 **

 **	imXpmColor	t	one color in the list

 **	imXpmColorList	v	list of colors

 **

 **	$Log: imxpm.h,v $

 * Revision 1.2  1995/06/29  00:32:03  bduggan

 * updated copyright

 *

 * Revision 1.1  1995/05/18  00:07:36  bduggan

 * Initial revision

 *

 **

 **/



#ifndef __IMXPM_H__

#define __IMXPM_H__





/*

 * TYPEDEF

 *	imXpmColor

 * DESCRIPTION

 *	This points to a single record entry in the list

 * of colors below.  The list may be traversed by doing

 * something like:

 *	imXpmColor ptr=imXpmColorList;

 *	while (ptr->name!=NULL)

 *		ptr++;

 */





typedef struct imXpmRecordStruct* imXpmColor;



struct imXpmRecordStruct

{

	char* name;

	unsigned char red;

	unsigned char green;

	unsigned char blue;

};





struct imXpmRecordStruct imXpmColorList[] =



/*

 * This list was taken from /usr/lib/X11/rgb*,

 * using the showrgb command on an sgi.

 */

{

{"None",0,0,0},    /* transparent.  We'll make it black. */

{"gainsboro",220,220,220},

{"honeydew",240,255,240},

{"mistyrose",255,228,225},

{"slategrey",112,128,144},

{"skyblue",135,206,235},

{"light steel blue",176,196,222},

{"lightcyan",224,255,255},

{"limegreen",50,205,50},

{"yellowgreen",154,205,50},

{"goldenrod",218,165,32},

{"peru",205,133,63},

{"light salmon",255,160,122},

{"medium violet red",199,21,133},

{"snow4",139,137,137},

{"bisque2",238,213,183},

{"bisque3",205,183,158},

{"azure1",240,255,255},

{"slateblue1",131,111,255},

{"slateblue4",71,60,139},

{"skyblue1",135,206,255},

{"lightskyblue3",141,182,205},

{"paleturquoise4",102,139,139},

{"cadetblue2",142,229,238},

{"aquamarine4",69,139,116},

{"springgreen2",0,238,118},

{"olivedrab1",192,255,62},

{"yellow2",238,238,0},

{"goldenrod3",205,155,29},

{"goldenrod4",139,105,20},

{"burlywood1",255,211,155},

{"orange3",205,133,0},

{"tomato3",205,79,57},

{"darkorchid2",178,58,238},

{"purple4",85,26,139},

{"mediumpurple2",159,121,238},

{"gray26",66,66,66},

{"gray33",84,84,84},

{"grey34",87,87,87},

{"gray37",94,94,94},

{"gray44",112,112,112},

{"gray63",161,161,161},

{"gray99",252,252,252},

{"sgi gray 0",0,0,0},

{"sgigray12",30,30,30},

{"sgi grey 24",61,61,61},

{"sgi gray 36",91,91,91},

{"sgigrey44",112,112,112},

{"sgigrey96",244,244,244},

{"sgichartreuse",113,198,113},

{"sgi bright gray",197,193,170},

{"deep sky blue",0,191,255},

{"darkgreen",0,100,0},

{"dark khaki",189,183,107},

{"wheat",245,222,179},

{"dark orange",255,140,0},

{"coral",255,127,80},

{"medium purple",147,112,219},

{"thistle",216,191,216},

{"navajowhite3",205,179,139},

{"lemonchiffon1",255,250,205},

{"honeydew1",240,255,240},

{"paleturquoise1",187,255,255},

{"goldenrod2",238,180,34},

{"burlywood2",238,197,145},

{"tan4",139,90,43},

{"chocolate2",238,118,33},

{"brown4",139,35,35},

{"maroon1",255,52,179},

{"maroon2",238,48,167},

{"darkorchid1",191,62,255},

{"mediumpurple1",171,130,255},

{"thistle1",255,225,255},

{"thistle2",238,210,238},

{"thistle3",205,181,205},

{"gray16",41,41,41},

{"gray23",59,59,59},

{"grey25",64,64,64},

{"gray27",69,69,69},

{"gray35",89,89,89},

{"gray41",105,105,105},

{"grey44",112,112,112},

{"gray45",115,115,115},

{"grey53",135,135,135},

{"gray74",189,189,189},

{"sgigrey4",10,10,10},

{"sgi gray 16",40,40,40},

{"sgigray28",71,71,71},

{"sgigrey28",71,71,71},

{"sgi grey 48",122,122,122},

{"sgi grey 84",214,214,214},

{"sgigrey88",224,224,224},

{"sgilightblue",125,158,192},

{"sgislateblue",113,113,198},

{"navy blue",0,0,128},

{"medium spring green",0,250,154},

{"olive drab",107,142,35},

{"darkgoldenrod",184,134,11},

{"rosybrown",188,143,143},

{"violetred",208,32,144},

{"slateblue3",105,89,205},

{"deepskyblue2",0,178,238},

{"deepskyblue3",0,154,205},

{"deepskyblue4",0,104,139},

{"cadetblue4",83,134,139},

{"indianred1",255,106,106},

{"wheat4",139,126,102},

{"firebrick1",255,48,48},

{"lightsalmon2",238,149,114},

{"darkorange1",255,127,0},

{"grey61",156,156,156},

{"gray65",166,166,166},

{"grey100",255,255,255},

{"sgigrey36",91,91,91},

{"sgigrey40",102,102,102},

{"sgi grey 88",224,224,224},

{"crimson",220,20,60},

{"floralwhite",255,250,240},

{"lemon chiffon",255,250,205},

{"light slate grey",119,136,153},

{"mediumblue",0,0,205},

{"dodger blue",30,144,255},

{"steelblue",70,130,180},

{"turquoise",64,224,208},

{"medium aquamarine",102,205,170},

{"sea green",46,139,87},

{"seagreen",46,139,87},

{"lawngreen",124,252,0},

{"green",0,255,0},

{"lightyellow",255,255,224},

{"dark goldenrod",184,134,11},

{"brown",165,42,42},

{"violet",238,130,238},

{"medium orchid",186,85,211},

{"navajowhite4",139,121,94},

{"ivory1",255,255,240},

{"honeydew4",131,139,131},

{"deepskyblue1",0,191,255},

{"darkolivegreen3",162,205,90},

{"lightgoldenrod3",205,190,112},

{"darkgoldenrod1",255,185,15},

{"burlywood4",139,115,85},

{"wheat1",255,231,186},

{"chocolate3",205,102,29},

{"chocolate4",139,69,19},

{"pink2",238,169,184},

{"orchid4",139,71,137},

{"gray0",0,0,0},

{"grey4",10,10,10},

{"grey5",13,13,13},

{"grey17",43,43,43},

{"gray29",74,74,74},

{"grey30",77,77,77},

{"grey46",117,117,117},

{"gray47",120,120,120},

{"grey47",120,120,120},

{"grey48",122,122,122},

{"grey55",140,140,140},

{"grey56",143,143,143},

{"grey57",145,145,145},

{"grey85",217,217,217},

{"grey88",224,224,224},

{"sgigrey0",0,0,0},

{"sgi gray 8",20,20,20},

{"sgigray24",61,61,61},

{"sgigrey64",163,163,163},

{"sgigray84",214,214,214},

{"sgi very light grey",214,214,214},

{"dim grey",105,105,105},

{"lightslategrey",119,136,153},

{"navyblue",0,0,128},

{"lightskyblue",135,206,250},

{"forestgreen",34,139,34},

{"rosy brown",188,143,143},

{"beige",245,245,220},

{"deeppink",255,20,147},

{"mediumvioletred",199,21,133},

{"dark orchid",153,50,204},

{"peachpuff3",205,175,149},

{"blue4",0,0,139},

{"dodgerblue4",16,78,139},

{"lightcyan4",122,139,139},

{"palegreen3",124,205,124},

{"indianred3",205,85,85},

{"indianred4",139,58,58},

{"salmon3",205,112,84},

{"salmon4",139,76,57},

{"lightsalmon4",139,87,66},

{"orangered2",238,64,0},

{"plum3",205,150,205},

{"gray8",20,20,20},

{"gray9",23,23,23},

{"gray11",28,28,28},

{"gray57",145,145,145},

{"grey77",196,196,196},

{"sgigray8",20,20,20},

{"sgigrey52",132,132,132},

{"sgi gray 64",163,163,163},

{"sgigray68",173,173,173},

{"sgigrey76",193,193,193},

{"floral white",255,250,240},

{"antique white",250,235,215},

{"aliceblue",240,248,255},

{"light gray",211,211,211},

{"dodgerblue",30,144,255},

{"mediumaquamarine",102,205,170},

{"dark green",0,100,0},

{"khaki",240,230,140},

{"lightgoldenrod",238,221,130},

{"hot pink",255,105,180},

{"light pink",255,182,193},

{"antiquewhite4",139,131,120},

{"steelblue2",92,172,238},

{"turquoise3",0,197,205},

{"darkslategray4",82,139,139},

{"aquamarine1",127,255,212},

{"darkseagreen2",180,238,180},

{"darkseagreen3",155,205,155},

{"seagreen1",84,255,159},

{"springgreen4",0,139,69},

{"lightyellow4",139,139,122},

{"gold4",139,117,0},

{"rosybrown2",238,180,180},

{"indianred2",238,99,99},

{"sienna2",238,121,66},

{"brown2",238,59,59},

{"deeppink4",139,10,80},

{"magenta1",255,0,255},

{"orchid1",255,131,250},

{"grey1",3,3,3},

{"grey19",48,48,48},

{"grey42",107,107,107},

{"grey51",130,130,130},

{"gray61",156,156,156},

{"gray85",217,217,217},

{"grey86",219,219,219},

{"grey87",222,222,222},

{"gray94",240,240,240},

{"grey94",240,240,240},

{"grey97",247,247,247},

{"gray100",255,255,255},

{"sgi grey 0",0,0,0},

{"sgi grey 16",40,40,40},

{"sgigray20",51,51,51},

{"sgigray48",122,122,122},

{"sgigray76",193,193,193},

{"sgigray80",204,204,204},

{"sgi gray 88",224,224,224},

{"sgigray88",224,224,224},

{"sgi dark grey",85,85,85},

{"sgibeet",142,56,142},

{"sgi teal",56,142,142},

{"sgimediumgray",132,132,132},

{"bisque",255,228,196},

{"mint cream",245,255,250},

{"slate grey",112,128,144},

{"lightslategray",119,136,153},

{"slateblue",106,90,205},

{"powderblue",176,224,230},

{"light sea green",32,178,170},

{"greenyellow",173,255,47},

{"darkkhaki",189,183,107},

{"sienna",160,82,45},

{"seashell2",238,229,222},

{"peachpuff2",238,203,173},

{"lemonchiffon2",238,233,191},

{"royalblue3",58,95,205},

{"lightsteelblue1",202,225,255},

{"lightsteelblue2",188,210,238},

{"palegreen2",144,238,144},

{"springgreen1",0,255,127},

{"chartreuse2",118,238,0},

{"red1",255,0,0},

{"deeppink2",238,18,137},

{"maroon4",139,28,98},

{"grey18",46,46,46},

{"gray25",64,64,64},

{"gray43",110,110,110},

{"gray59",150,150,150},

{"grey67",171,171,171},

{"grey71",181,181,181},

{"sgigray0",0,0,0},

{"sgigray4",10,10,10},

{"sgi very dark gray",40,40,40},

{"whitesmoke",245,245,245},

{"navajowhite",255,222,173},

{"alice blue",240,248,255},

{"lightgrey",211,211,211},

{"cornflowerblue",100,149,237},

{"lawn green",124,252,0},

{"olivedrab",107,142,35},

{"chocolate",210,105,30},

{"maroon",176,48,96},

{"magenta",255,0,255},

{"snow3",205,201,201},

{"antiquewhite1",255,239,219},

{"bisque1",255,228,196},

{"navajowhite1",255,222,173},

{"navajowhite2",238,207,161},

{"steelblue1",99,184,255},

{"lightskyblue2",164,211,238},

{"lightblue3",154,192,205},

{"turquoise2",0,229,238},

{"khaki4",139,134,78},

{"lightyellow3",205,205,180},

{"yellow1",255,255,0},

{"wheat2",238,216,174},

{"chocolate1",255,127,36},

{"brown1",255,64,64},

{"orange1",255,165,0},

{"orange2",238,154,0},

{"darkorange3",205,102,0},

{"lightpink4",139,95,101},

{"magenta3",205,0,205},

{"magenta4",139,0,139},

{"purple2",145,44,238},

{"purple3",125,38,205},

{"grey11",28,28,28},

{"grey15",38,38,38},

{"gray22",56,56,56},

{"gray34",87,87,87},

{"gray40",102,102,102},

{"grey41",105,105,105},

{"grey50",127,127,127},

{"gray58",148,148,148},

{"gray80",204,204,204},

{"grey80",204,204,204},

{"grey95",242,242,242},

{"grey96",245,245,245},

{"sgi grey 20",51,51,51},

{"sgigray44",112,112,112},

{"sgi grey 64",163,163,163},

{"sgigray92",234,234,234},

{"sgi grey 96",244,244,244},

{"sgidarkgrey",85,85,85},

{"sgi salmon",198,113,113},

{"lemonchiffon",255,250,205},

{"lavender blush",255,240,245},

{"midnightblue",25,25,112},

{"darkslateblue",72,61,139},

{"slate blue",106,90,205},

{"light cyan",224,255,255},

{"cadet blue",95,158,160},

{"mediumseagreen",60,179,113},

{"green yellow",173,255,47},

{"lime green",50,205,50},

{"tan",210,180,140},

{"darksalmon",233,150,122},

{"lightsalmon",255,160,122},

{"darkorange",255,140,0},

{"light coral",240,128,128},

{"orange red",255,69,0},

{"red",255,0,0},

{"deep pink",255,20,147},

{"pale violet red",219,112,147},

{"darkviolet",148,0,211},

{"snow2",238,233,233},

{"royalblue4",39,64,139},

{"dodgerblue3",24,116,205},

{"lightskyblue1",176,226,255},

{"khaki2",238,230,133},

{"lightgoldenrod4",139,129,76},

{"sienna4",139,71,38},

{"lightsalmon1",255,160,122},

{"darkorange4",139,69,0},

{"red3",205,0,0},

{"red4",139,0,0},

{"pink3",205,145,158},

{"magenta2",238,0,238},

{"mediumorchid2",209,95,238},

{"gray1",3,3,3},

{"grey7",18,18,18},

{"grey13",33,33,33},

{"gray31",79,79,79},

{"grey35",89,89,89},

{"gray51",130,130,130},

{"gray55",140,140,140},

{"grey68",173,173,173},

{"grey69",176,176,176},

{"grey73",186,186,186},

{"grey74",189,189,189},

{"grey75",191,191,191},

{"gray81",207,207,207},

{"gray90",229,229,229},

{"sgi gray 28",71,71,71},

{"sgi grey 40",102,102,102},

{"sgi beet",142,56,142},

{"sgimediumgrey",132,132,132},

{"indigo",75,0,130},

{"old lace",253,245,230},

{"peach puff",255,218,185},

{"dark slate gray",47,79,79},

{"grey",190,190,190},

{"lightgray",211,211,211},

{"navy",0,0,128},

{"light slate blue",132,112,255},

{"powder blue",176,224,230},

{"dark olive green",85,107,47},

{"lightgoldenrodyellow",250,250,210},

{"gold",255,215,0},

{"saddlebrown",139,69,19},

{"lightpink",255,182,193},

{"orchid",218,112,214},

{"bisque4",139,125,107},

{"cornsilk1",255,248,220},

{"honeydew2",224,238,224},

{"lightblue4",104,131,139},

{"olivedrab4",105,139,34},

{"khaki3",205,198,115},

{"sienna1",255,130,71},

{"brown3",205,51,51},

{"red2",238,0,0},

{"palevioletred3",205,104,137},

{"darkorchid3",154,50,205},

{"darkorchid4",104,34,139},

{"mediumpurple3",137,104,205},

{"mediumpurple4",93,71,139},

{"grey8",20,20,20},

{"grey10",26,26,26},

{"grey14",36,36,36},

{"gray20",51,51,51},

{"gray21",54,54,54},

{"grey26",66,66,66},

{"grey43",110,110,110},

{"grey52",133,133,133},

{"gray53",135,135,135},

{"grey72",184,184,184},

{"gray75",191,191,191},

{"grey82",209,209,209},

{"grey83",212,212,212},

{"grey84",214,214,214},

{"sgi gray 4",10,10,10},

{"sgi gray 12",30,30,30},

{"sgi grey 12",30,30,30},

{"sgigrey12",30,30,30},

{"sgi gray 20",51,51,51},

{"sgigrey24",61,61,61},

{"sgi grey 36",91,91,91},

{"sgi gray 56",142,142,142},

{"sgi grey 56",142,142,142},

{"sgi grey 92",234,234,234},

{"sgi gray 100",255,255,255},

{"blanched almond",255,235,205},

{"peachpuff",255,218,185},

{"moccasin",255,228,181},

{"mintcream",245,255,250},

{"lavenderblush",255,240,245},

{"light slate gray",119,136,153},

{"gray",190,190,190},

{"cornflower blue",100,149,237},

{"blue",0,0,255},

{"darkolivegreen",85,107,47},

{"palegreen",152,251,152},

{"chartreuse",127,255,0},

{"salmon",250,128,114},

{"lightcoral",240,128,128},

{"pink",255,192,203},

{"violet red",208,32,144},

{"lavenderblush1",255,240,245},

{"lavenderblush2",238,224,229},

{"lavenderblush3",205,193,197},

{"mistyrose1",255,228,225},

{"mistyrose4",139,125,123},

{"azure4",131,139,139},

{"skyblue2",126,192,238},

{"lightcyan3",180,205,205},

{"aquamarine2",118,238,198},

{"olivedrab2",179,238,58},

{"darkolivegreen1",202,255,112},

{"tan3",205,133,63},

{"tomato1",255,99,71},

{"maroon3",205,41,144},

{"orchid3",205,105,201},

{"grey3",8,8,8},

{"gray15",38,38,38},

{"gray19",48,48,48},

{"gray30",77,77,77},

{"grey37",94,94,94},

{"grey38",97,97,97},

{"grey39",99,99,99},

{"gray50",127,127,127},

{"gray54",138,138,138},

{"grey62",158,158,158},

{"gray66",168,168,168},

{"grey81",207,207,207},

{"gray83",212,212,212},

{"gray92",235,235,235},

{"sgigrey16",40,40,40},

{"sgi grey 28",71,71,71},

{"sgigray40",102,102,102},

{"sgigrey56",142,142,142},

{"sgi gray 72",183,183,183},

{"indigo2",33,136,104},

{"antiquewhite",250,235,215},

{"midnight blue",25,25,112},

{"sky blue",135,206,235},

{"light sky blue",135,206,250},

{"steel blue",70,130,180},

{"darkseagreen",143,188,143},

{"yellow green",154,205,50},

{"palegoldenrod",238,232,170},

{"orangered",255,69,0},

{"lightsteelblue4",110,123,139},

{"darkslategray1",151,255,255},

{"darkslategray2",141,238,238},

{"darkslategray3",121,205,205},

{"chartreuse4",69,139,0},

{"olivedrab3",154,205,50},

{"darkolivegreen4",110,139,61},

{"coral1",255,114,86},

{"coral4",139,62,47},

{"lightpink2",238,162,173},

{"palevioletred4",139,71,93},

{"orchid2",238,122,233},

{"plum1",255,187,255},

{"thistle4",139,123,139},

{"grey9",23,23,23},

{"grey28",71,71,71},

{"grey29",74,74,74},

{"grey36",92,92,92},

{"gray77",196,196,196},

{"gray78",199,199,199},

{"gray79",201,201,201},

{"sgigrey60",153,153,153},

{"dark slate blue",72,61,139},

{"royalblue",65,105,225},

{"medium turquoise",72,209,204},

{"cyan",0,255,255},

{"cadetblue",95,158,160},

{"springgreen",0,255,127},

{"indian red",205,92,92},

{"hotpink",255,105,180},

{"mediumorchid",186,85,211},

{"antiquewhite3",205,192,176},

{"lemonchiffon4",139,137,112},

{"mistyrose2",238,213,210},

{"blue2",0,0,238},

{"steelblue3",79,148,205},

{"skyblue3",108,166,205},

{"lightcyan2",209,238,238},

{"turquoise4",0,134,139},

{"darkseagreen1",193,255,193},

{"yellow3",205,205,0},

{"gold1",255,215,0},

{"gold3",205,173,0},

{"tan2",238,154,73},

{"firebrick2",238,44,44},

{"salmon1",255,140,105},

{"orange4",139,90,0},

{"plum4",139,102,139},

{"gray6",15,15,15},

{"grey6",15,15,15},

{"gray14",36,36,36},

{"gray18",46,46,46},

{"gray39",99,99,99},

{"gray56",143,143,143},

{"grey90",229,229,229},

{"sgi gray 40",102,102,102},

{"sgigray64",163,163,163},

{"sgi gray 80",204,204,204},

{"sgigrey80",204,204,204},

{"sgi slate blue",113,113,198},

{"sgiverydarkgray",40,40,40},

{"sgibrightgray",197,193,170},

{"ivory",255,255,240},

{"dark salmon",233,150,122},

{"dark violet",148,0,211},

{"lightblue1",191,239,255},

{"lightblue2",178,223,238},

{"turquoise1",0,245,255},

{"lightyellow2",238,238,209},

{"yellow4",139,139,0},

{"coral3",205,91,69},

{"deeppink3",205,16,118},

{"hotpink1",255,110,180},

{"lightpink1",255,174,185},

{"mediumorchid4",122,55,139},

{"gray2",5,5,5},

{"gray4",10,10,10},

{"grey20",51,51,51},

{"grey24",61,61,61},

{"grey31",79,79,79},

{"grey49",125,125,125},

{"grey58",148,148,148},

{"grey63",161,161,161},

{"gray67",171,171,171},

{"gray73",186,186,186},

{"grey89",227,227,227},

{"sgigray56",142,142,142},

{"sgi gray 76",193,193,193},

{"sgigray96",244,244,244},

{"sgi medium gray",132,132,132},

{"ghostwhite",248,248,255},

{"dimgrey",105,105,105},

{"slategray",112,128,144},

{"lightslateblue",132,112,255},

{"royal blue",65,105,225},

{"pale green",152,251,152},

{"forest green",34,139,34},

{"seashell3",205,197,191},

{"peachpuff4",139,119,101},

{"cornsilk2",238,232,205},

{"cornsilk4",139,136,120},

{"ivory3",205,205,193},

{"honeydew3",193,205,193},

{"azure2",224,238,238},

{"slategray2",185,211,238},

{"slategray3",159,182,205},

{"slategray4",108,123,139},

{"cadetblue3",122,197,205},

{"darkseagreen4",105,139,105},

{"palegreen4",84,139,84},

{"springgreen3",0,205,102},

{"khaki1",255,246,143},

{"lightgoldenrod2",238,220,130},

{"goldenrod1",255,193,37},

{"firebrick3",205,38,38},

{"firebrick4",139,26,26},

{"orangered3",205,55,0},

{"mediumorchid3",180,82,205},

{"purple1",155,48,255},

{"gray3",8,8,8},

{"grey12",31,31,31},

{"grey22",56,56,56},

{"grey32",82,82,82},

{"gray49",125,125,125},

{"gray60",153,153,153},

{"grey60",153,153,153},

{"gray64",163,163,163},

{"grey70",179,179,179},

{"gray71",181,181,181},

{"gray89",227,227,227},

{"gray98",250,250,250},

{"sgi gray 32",81,81,81},

{"sgigray60",153,153,153},

{"sgi gray 68",173,173,173},

{"sgi grey 68",173,173,173},

{"sgigrey84",214,214,214},

{"sgiteal",56,142,142},

{"sgi light grey",170,170,170},

{"sgiverylightgrey",214,214,214},

{"navajo white",255,222,173},

{"white",255,255,255},

{"black",0,0,0},

{"paleturquoise2",174,238,238},

{"cadetblue1",152,245,255},

{"cyan3",0,205,205},

{"aquamarine3",102,205,170},

{"green2",0,238,0},

{"lightyellow1",255,255,224},

{"burlywood3",205,170,125},

{"wheat3",205,186,150},

{"tomato2",238,92,66},

{"tomato4",139,54,38},

{"lightpink3",205,140,149},

{"palevioletred1",255,130,171},

{"palevioletred2",238,121,159},

{"grey23",59,59,59},

{"gray52",133,133,133},

{"gray62",158,158,158},

{"gray72",184,184,184},

{"sgi gray 24",61,61,61},

{"sgi grey 76",193,193,193},

{"sgi light blue",125,158,192},

{"sgi light gray",170,170,170},

{"sgilightgray",170,170,170},

{"sgiverylightgray",214,214,214},

{"sgi medium grey",132,132,132},

{"sgiverydarkgrey",40,40,40},

{"sgibrightgrey",197,193,170},

{"deepskyblue",0,191,255},

{"saddle brown",139,69,19},

{"firebrick",178,34,34},

{"snow1",255,250,250},

{"azure3",193,205,205},

{"slateblue2",122,103,238},

{"darkgoldenrod2",238,173,14},

{"darkgoldenrod3",205,149,12},

{"darkgoldenrod4",139,101,8},

{"tan1",255,165,79},

{"violetred1",255,62,150},

{"violetred2",238,58,140},

{"violetred3",205,50,120},

{"gray10",26,26,26},

{"gray13",33,33,33},

{"gray32",82,82,82},

{"gray36",92,92,92},

{"grey76",194,194,194},

{"grey79",201,201,201},

{"gray84",214,214,214},

{"gray93",237,237,237},

{"sgigray36",91,91,91},

{"sgi gray 44",112,112,112},

{"sgi grey 44",112,112,112},

{"sgigrey48",122,122,122},

{"sgi grey 72",183,183,183},

{"sgi grey 80",204,204,204},

{"sgi dark gray",85,85,85},

{"sgi olive drab",142,142,56},

{"sgi bright grey",197,193,170},

{"oldlace",253,245,230},

{"linen",250,240,230},

{"lavender",230,230,250},

{"misty rose",255,228,225},

{"darkslategrey",47,79,79},

{"dark turquoise",0,206,209},

{"darkturquoise",0,206,209},

{"light goldenrod yellow",250,250,210},

{"indianred",205,92,92},

{"lavenderblush4",139,131,134},

{"seagreen2",78,238,148},

{"rosybrown4",139,105,105},

{"lightsalmon3",205,129,98},

{"deeppink1",255,20,147},

{"gray12",31,31,31},

{"grey21",54,54,54},

{"gray38",97,97,97},

{"grey45",115,115,115},

{"grey54",138,138,138},

{"gray70",179,179,179},

{"grey78",199,199,199},

{"gray86",219,219,219},

{"gray87",222,222,222},

{"gray88",224,224,224},

{"gray95",242,242,242},

{"gray96",245,245,245},

{"gray97",247,247,247},

{"sgigrey100",255,255,255},

{"sgidarkgray",85,85,85},

{"snow",255,250,250},

{"ghost white",248,248,255},

{"white smoke",245,245,245},

{"mediumslateblue",123,104,238},

{"medium blue",0,0,205},

{"spring green",0,255,127},

{"light yellow",255,255,224},

{"light goldenrod",238,221,130},

{"blueviolet",138,43,226},

{"seashell4",139,134,130},

{"peachpuff1",255,218,185},

{"blue1",0,0,255},

{"steelblue4",54,100,139},

{"lightskyblue4",96,123,139},

{"paleturquoise3",150,205,205},

{"cyan1",0,255,255},

{"cyan2",0,238,238},

{"palegreen1",154,255,154},

{"lightgoldenrod1",255,236,139},

{"rosybrown3",205,155,155},

{"sienna3",205,104,57},

{"gray5",13,13,13},

{"grey40",102,102,102},

{"gray82",209,209,209},

{"gray91",232,232,232},

{"sgi grey 32",81,81,81},

{"sgi gray 84",214,214,214},

{"sgigrey92",234,234,234},

{"sgigray100",255,255,255},

{"sgisalmon",198,113,113},

{"sgi very light gray",214,214,214},

{"blanchedalmond",255,235,205},

{"cornsilk",255,248,220},

{"darkslategray",47,79,79},

{"lightseagreen",32,178,170},

{"plum",221,160,221},

{"lemonchiffon3",205,201,165},

{"ivory2",238,238,224},

{"blue3",0,0,205},

{"cyan4",0,139,139},

{"rosybrown1",255,193,193},

{"salmon2",238,130,98},

{"darkorange2",238,118,0},

{"pink1",255,181,197},

{"grey2",5,5,5},

{"gray7",18,18,18},

{"grey16",41,41,41},

{"gray17",43,43,43},

{"gray48",122,122,122},

{"grey91",232,232,232},

{"grey92",235,235,235},

{"grey93",237,237,237},

{"sgigrey20",51,51,51},

{"sgigray32",81,81,81},

{"sgi grey 52",132,132,132},

{"sgi gray 60",153,153,153},

{"sgigrey72",183,183,183},

{"sgiolivedrab",142,142,56},

{"papaya whip",255,239,213},

{"papayawhip",255,239,213},

{"azure",240,255,255},

{"dimgray",105,105,105},

{"lightsteelblue",176,196,222},

{"aquamarine",127,255,212},

{"mediumspringgreen",0,250,154},

{"pale goldenrod",238,232,170},

{"sandy brown",244,164,96},

{"tomato",255,99,71},

{"mistyrose3",205,183,181},

{"royalblue1",72,118,255},

{"royalblue2",67,110,238},

{"skyblue4",74,112,139},

{"lightcyan1",224,255,255},

{"chartreuse3",102,205,0},

{"darkolivegreen2",188,238,104},

{"hotpink2",238,106,167},

{"hotpink3",205,96,144},

{"hotpink4",139,58,98},

{"violetred4",139,34,82},

{"grey0",0,0,0},

{"grey27",69,69,69},

{"grey33",84,84,84},

{"grey64",163,163,163},

{"grey65",166,166,166},

{"grey66",168,168,168},

{"gray68",173,173,173},

{"gray69",176,176,176},

{"gray76",194,194,194},

{"sgigrey32",81,81,81},

{"sgi gray 48",122,122,122},

{"sgigray52",132,132,132},

{"sgi grey 100",255,255,255},

{"sgi very dark grey",40,40,40},

{"seashell",255,245,238},

{"dark slate grey",47,79,79},

{"dim gray",105,105,105},

{"light grey",211,211,211},

{"medium slate blue",123,104,238},

{"lightblue",173,216,230},

{"pale turquoise",175,238,238},

{"paleturquoise",175,238,238},

{"medium sea green",60,179,113},

{"yellow",255,255,0},

{"burlywood",222,184,135},

{"sandybrown",244,164,96},

{"orange",255,165,0},

{"purple",160,32,240},

{"antiquewhite2",238,223,204},

{"ivory4",139,139,131},

{"seagreen3",67,205,128},

{"seagreen4",46,139,87},

{"green1",0,255,0},

{"green4",0,139,0},

{"chartreuse1",127,255,0},

{"gold2",238,201,0},

{"coral2",238,106,80},

{"orangered1",255,69,0},

{"pink4",139,99,108},

{"plum2",238,174,238},

{"grey99",252,252,252},

{"sgigrey8",20,20,20},

{"sgi gray 52",132,132,132},

{"sgi grey 60",153,153,153},

{"sgi gray 96",244,244,244},

{"sgi chartreuse",113,198,113},

{"sgilightgrey",170,170,170},

{"slate gray",112,128,144},

{"light blue",173,216,230},

{"mediumturquoise",72,209,204},

{"dark sea green",143,188,143},

{"palevioletred",219,112,147},

{"darkorchid",153,50,204},

{"blue violet",138,43,226},

{"mediumpurple",147,112,219},

{"seashell1",255,245,238},

{"cornsilk3",205,200,177},

{"dodgerblue1",30,144,255},

{"dodgerblue2",28,134,238},

{"slategray1",198,226,255},

{"lightsteelblue3",162,181,205},

{"green3",0,205,0},

{"orangered4",139,37,0},

{"mediumorchid1",224,102,255},

{"gray24",61,61,61},

{"gray28",71,71,71},

{"gray42",107,107,107},

{"gray46",117,117,117},

{"grey59",150,150,150},

{"grey98",250,250,250},

{"sgi grey 4",10,10,10},

{"sgi grey 8",20,20,20},

{"sgigray16",40,40,40},

{"sgigrey68",173,173,173},

{"sgigray72",183,183,183},

{"sgi gray 92",234,234,234},

{ NULL, 0, 0, 0}

};





#endif  /* __IMXPM_H__ */





