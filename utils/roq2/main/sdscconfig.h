/**

 **	$Header: /sdsc/dev/vis/misc/libsdsc/v3.0/libsdsc/src/include/RCS/sdscconfig.h,v 1.14 1995/06/29 00:12:30 bduggan Exp $

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

 **	sdscconfig.h.h	-  SDSC machine characteristics include file

 **

 **  PROJECT

 **	All SDSC software

 **

 **  PUBLIC CONTENTS

 **	__SDSCCONFIGH__	-  file inclusion flag

 **

 **	The following flags are defined for the machine and C compiler:

 **		VOID		'void' type is available

 **		MBF		most-significant byte first

 **		LBF		most-significant byte first

 **

 **	The following constants are defined for the machine and C compiler:

 **		VENDOR_NAME	computer vendor name (like SUN, SGI, etc)

 **		MACHINE_NAME	machine name (like Sun4, 4D, etc)

 **		CPU_NAME	cpu name (like SPARC, MIPS R2000, etc)

 **		OS_NAME		operating system name (like SunOS, IRIX, etc)

 **

 **		CHAR_SIZE	bits in a char

 **		SHORT_SIZE	bits in a short

 **		INT_SIZE	bits in an int

 **		LONG_SIZE	bits in a long

 **

 **  PRIVATE CONTENTS

 **

 **  HISTORY

 **	$Log: sdscconfig.h,v $

 **	Revision 1.14  1995/06/29  00:12:30  bduggan

 **	Added MSWIND

 **

 **	Revision 1.13  1994/10/25  11:18:02  allans

 **	added defn. for paragon

 **

 **	Revision 1.12  94/10/03  16:41:50  nadeau

 **	Added SGI and MIPS cases.

 **	Updated copyright message.

 **	

 **	Revision 1.11  93/05/17  18:53:16  lrw

 **	Added support for DEC alpha systems.

 **	

 **	Revision 1.10  92/09/02  15:23:08  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.9  92/09/02  13:43:38  nadeau

 **	Added support for HP9000.

 **	

 **	Revision 1.8  92/06/29  17:59:39  todd

 **	Added definitions for nCUBE architecture.

 **	

 **	Revision 1.7  91/09/17  19:25:32  nadeau

 **	Updated header comments about how to get define names for

 **	a UNIX system.  Added IBM RS6000 support.

 **	

 **	Revision 1.6  91/09/06  10:12:00  nadeau

 **	Added Apple Mac (AUX) definitions

 **	

 **	Revision 1.5  91/09/02  12:58:25  nadeau

 **	Added NeXT definitions

 **	

 **	Revision 1.4  91/06/06  18:54:13  todd

 **	Added nCUBE 2 definitions

 **	

 **	Revision 1.3  91/05/23  14:37:13  nadeau

 **	Check in so Todd can add nCUBE entry.

 **	

 **	Revision 1.2  91/01/15  13:53:51  todd

 **	Added Alliant FX2800 defines

 **	

 **	Revision 1.1  90/06/22  12:16:23  nadeau

 **	Initial revision

 **	

 **/



#ifndef __SDSCCONFIGH__

#define __SDSCCONFIGH__



/*

 *  Explanation:

 *	C differs slightly from system to system.  The C preprocessor (/lib/cpp)

 *	for virtually all implementations will define a unique name for the

 *	machine & OS, allowing one to #ifdef in special code for it when needed.

 *	This file uses this feature to recognize a set of machines and attempt

 *	to intelligently define some standard flags that may be #ifdef-ed with

 *	later on.

 *

 *	If, by the end of this file, all of these flags have NOT been defined

 *	by one of the sets of #if's, a syntax error is generated to insure that

 *	the porter gets here to fix the oversight.

 *

 *	The following is a known list of /lib/cpp names for various machines and

 *	OS's.  The list is derived from an article in comp.lang.c, July 24th

 *	1987, by Bill Jones of Develcon Electronics.  The list has since been

 *	heavily augmented by further net postings and local experience.

 *

 *	Name	      Description and availability

 *	----------------------------------------------------------------------

 *		cpu names

 *	hp9000s200    HP 9000 s200

 *	hp9000s300    HP 9000 s300

 *	hp9000s400    HP 9000 s400

 *	hp9000s500    HP 9000 s500

 *	hp9000s700    HP 9000 s700

 *	hp9000s800    HP 9000 s800

 *	i860          Intel i860

 *	i80186        Intel 80186 compiled by Lattice C compiler

 *	m68000        Motorola M68000 family: CCI

 *	m68k          Motorola M68000 family: Motorola SysV

 *	mc68000       Motorola M68000 family: Sun, Fortune, others

 *	mc68020       Motorola M68020:  Sun 2, ...

 *	mc68030       Motorola M68030:  Sun 3, Mac, ...

 *	mips	      MIPS RISC chip (Silicon Graphics 4D, Ardent Titan, etc)

 *	__mips	      MIPS RISC chip (Silicon Graphics 4D, Ardent Titan, etc)

 *	sparc         Sun SPARC RISC processor

 *	u370          AT&T Unix on IBM 370

 *	u3b           AT&T Unix on 3B series

 *	u3b2          AT&T Unix on 3B-2

 *	u3b20         AT&T Unix on 3B-20

 *	u3b5          AT&T Unix on 3B-5

 *	vax           DEC VAX computers

 *	vax11c        DEC (VMS/Ultrix) VAX-11 C

 *	z80           Zilog Z80: Vandata C

 *	z8000         Zilog Z8000

 *

 *		machine/vendor names

 *	accel	      Celerity accel

 *	alliant       Alliant Computer Systems Corporation

 *	apollo        Apollo workstations

 *	ardent	      Ardent Computer Coropration (old name for Stardent)

 *	cray          Cray X-MP, Y-MP, or Cray-2

 *	dana	      Dana (old name for Ardent)

 *	DATAGENERAL   Data General hardware

 *	datageneral   Data General hardware

 *	ibm           IBM and Amdahl mainframes

 *	_IBMR2        IBM RS6000, AIX

 *	ibm032	      IBM RT, Berkeley Unix

 *	interdata     Interdata 8/32 (AT&T only?)

 *	hp9000s200    Hewlett-Packard 9000 HP/UX

 *	hp9000s500    Hewlett-Packard 9000 HP/UX

 *	kl10          DEC-20 KL10 processor: U of Utah pcc

 *	n16	      Encore Multimax with an NS16032 ?

 *	NCUBE         nCUBE Corpration parallel computer

 *	NeXT          NeXT station

 *	ns16000       Encore Multimax with an NS16032 ?

 *	ns32000       Encore Multimax with an NS32032 ?

 *	orion         Orion supermicro

 *	pdp11         DEC PDP-11: Unix V7, Decus C

 *	sel           Gould Concept 32

 *	selport       Gould Concept 32

 *	sgi	      Silicon Graphics Inc.

 *	__sgi	      Silicon Graphics Inc.

 *	stellar       Stellar Computers (old name for Stardent)

 *	sun           Sun Microsystems workstations

 *	TM_DPS6       Honeywell DPS 6: Waterloo C

 *	TM_L66        Honeywell Level 66: Waterloo C

 *	TS_GCOS       Honeywell GCOS 8: Waterloo C

 *	TS_MOD400     Honeywell GCOS 6 model 400: Waterloo C

 *	titan	      Ardent Titan

 *	univac        Univac/1100 Unix

 *

 *		operating systems

 *	_AIX          IBM AIX

 *	AOSVS         Data General AOS/VS operating system

 *	_AUX_SOURCE   Apple Macintosh AUX

 *	aosvs         Data General AOS/VS operating system

 *	bsd4_2        Berkeley 4.2 Unix: Ultrix

 *	DGUX          Data General Unix

 *	dgux          Data General Unix

 *	gcos          Honeywell 6000 GCOS system (AT&T only?)

 *	hpux	      HP variant of Unix

 *	mert          AT&T MERT variant of Unix

 *	os            IBM OS/360, /370 (AT&T only?)

 *	rsx           DEC PDP-11 RSX: Decus C

 *	RT            AT&T UNIX/RT

 *	SVR3	      MIPS (and SGI) System V, Release 3 UNIX

 *	SYSTEM_FIVE   Ultrix SysV emulation

 *	tops20        DEC-20 TOPS-20: U of Utah pcc

 *	TS            AT&T UNIX/TS for IBM

 *	tss           AT&T C under IBM TSS

 *	ultrix        DEC's Ultrix variant

 *	unix          any Unix

 *	uts           Amdahl UTS variant

 *	vms           DEC VMS operating system: VAX-11 C

 *

 *		programming languages/flags

 *	decus         Decus C (PDP-11 RSX, RSTS, RT-11)

 *	GFLOAT        Ultrix compiling 8-byte floating point

 *	ghs	      Green Hills C compiler

 *	lint          being processed by lint

 *	nomacarg      cpp without macro arguments: Decus C

 *	PWB           Programmer's workbench variant

 *	RES           AT&T Research Unix

 *	LANGUAGE_C    C language in use:  Ardent Titan

 *

 *

 *	These special defined names come from a variety of sources, depending

 *	upon the OS being used.

 *

 *		1.  On "standard" UNIX systems, /lib/cpp has been modified to

 *		    add a few extra names into its hash table.  These extra

 *		    names can be found by typing:

 *

 *			strings -3 /lib/cpp > tmp

 *

 *		    The file 'tmp' should then be scanned to look for the

 *		    standard preprocessor keywords, like 'ifdef' and 'endif'.

 *		    Immediately preceding or following this list are usually

 *		    two or three of these extra names.

 *

 *		2.  On some UNIX variants, a "clean" /lib/cpp is used that

 *		    has not been modified to define extra names.  Instead,

 *		    the C compiler invokes /lib/cpp with -D options to define

 *		    extra names.  These extra names can be found by typing:

 *

 *			strings -3 /bin/cc > tmp

 *

 *		    'tmp' should be scanned to look for strings of the

 *		    form -Dblah.  'blah' will be an extra name.

 *

 *		3.  On IBM's AIX systems, the C compiler checks a configuration

 * 		    file to get a list of -D options to pass on to /lib/cpp.

 *		    This configuration data may be found in /etc/xlc.cfg.

 *

 *		4.  For non-UNIX systems, you'll have to consult your local

 *		    guru or the system's C manuals.

 */







/*

 *  Configurations:

 *	The following #ifdef's set up the configuration flags for each system

 *	variant for which we have had local experience.

 */



#define IEEE_FLOAT	0

#define VAX_FLOAT	1

#define VAXG_FLOAT	2

#define CRAYMP_FLOAT	3





#ifdef vax

#define LBF

#define VENDOR_NAME	"Digital Equipment Corp. (DEC)"

#define CPU_NAME	"VAX"

#define MACHINE_NAME	"VAX"

#define CHAR_SIZE	8

#define SHORT_SIZE	16

#define INT_SIZE	32

#define LONG_SIZE	32

#define FLOAT_TYPE	VAX_FLOAT



#ifdef vms

#define VOID

#define OS_NAME		"VMS"

#else



#ifdef ultrix

#define VOID

#define OS_NAME		"ULTRIX"		/* BSD + SysV		*/

#endif /* ultrix */

#endif /* vms */



#endif /* vax */





#ifdef mips

#ifdef ultrix

#define VOID

#define LBF

#define VENDOR_NAME	"Digital Equipment Corp. (DEC)"

#define CPU_NAME	"MIPS"

#define MACHINE_NAME	"DecStation"

#define OS_NAME		"ULTRIX"		/* BSD + SysV		*/

#define CHAR_SIZE	8

#define SHORT_SIZE	16

#define INT_SIZE	32

#define LONG_SIZE	32

#define FLOAT_TYPE	IEEE_FLOAT

#endif /* ultrix */

#endif /* mips */





#ifdef __alpha

#ifdef __osf__

#define VOID

#define LBF

#define VENDOR_NAME	"Digital Equipment Corp. (DEC)"

#define CPU_NAME	"ALPHA"

#define MACHINE_NAME	"DecAlpha"

#define OS_NAME		"OSF"		/* BSD + SysV		*/

#define CHAR_SIZE	8

#define SHORT_SIZE	16

#define INT_SIZE	32

#define LONG_SIZE	64

#define FLOAT_TYPE	IEEE_FLOAT

#endif /* __osf__ */

#endif /* __alpha */





#ifdef sun

#define VOID

#define VENDOR_NAME	"Sun Microsystems Inc."

#define OS_NAME		"SunOS"			/* BSD + SysV		*/



#ifdef mc68020

#define MBF

#define MACHINE_NAME	"Sun 2"

#define CPU_NAME	"Motorola 68020"

#define CHAR_SIZE	8

#define SHORT_SIZE	16

#define INT_SIZE	32

#define LONG_SIZE	32

#define FLOAT_TYPE	IEEE_FLOAT

#endif /* mc68020 */



#ifdef mc68030

#define MBF

#define MACHINE_NAME	"Sun 3"

#define CPU_NAME	"Motorola 68030"

#define CHAR_SIZE	8

#define SHORT_SIZE	16

#define INT_SIZE	32

#define LONG_SIZE	32

#define FLOAT_TYPE	IEEE_FLOAT

#endif /* mc68030 */



#ifdef sparc

#define MBF

#define MACHINE_NAME	"Sun 4"

#define CPU_NAME	"SPARC"

#define CHAR_SIZE	8

#define SHORT_SIZE	16

#define INT_SIZE	32

#define LONG_SIZE	32

#define FLOAT_TYPE	IEEE_FLOAT

#define VOID

#endif /* sparc */



#endif /* sun */





#if sgi | __sgi

#define VENDOR_NAME	"Silicon Graphics Inc. (SGI)"



#if mips | __mips

#define VOID

#define MBF

#define CPU_NAME	"MIPS"

#define MACHINE_NAME	"Iris 4D"

#define OS_NAME		"IRIX"			/* SysV + BSD		*/

#define CHAR_SIZE	8

#define SHORT_SIZE	16

#define INT_SIZE	32

#define LONG_SIZE	32

#define FLOAT_TYPE	IEEE_FLOAT

#endif /* mips */



#endif /* sgi */





#ifdef ardent

#define VENDOR_NAME	"Ardent Computer Corp."



#ifdef mips

#define VOID

#define MBF

#define CPU_NAME	"MIPS"

#define MACHINE_NAME	"TITAN"

#define OS_NAME		"UNIX"			/* SysV			*/

#define CHAR_SIZE	8

#define SHORT_SIZE	16

#define INT_SIZE	32

#define LONG_SIZE	32

#define FLOAT_TYPE	IEEE_FLOAT

#endif /* mips */



#endif /* ardent */



#ifdef stellar

#define VENDOR_NAME	"Stardent Computer Corp."

#define CPU_NAME	"STELLAR"

#define MACHINE_NAME	"STARDENT GS1000"

#define OS_NAME		"STELLIX"		/* SysV			*/

#define CHAR_SIZE	8

#define VOID

#define MBF

#define SHORT_SIZE	16

#define INT_SIZE	32

#define LONG_SIZE	32

#define FLOAT_TYPE	IEEE_FLOAT

#endif /* stellar */





#ifdef accel

#define VOID

#define MBF

#define VENDOR_NAME	"Celerity"

#define CPU_NAME	"Accel"

#define MACHINE_NAME	"Accel"

#define OS_NAME		"UNIX"			/* BSD			*/

#define CHAR_SIZE	8

#define SHORT_SIZE	16

#define INT_SIZE	32

#define LONG_SIZE	32

#define FLOAT_TYPE	IEEE_FLOAT

#endif /* accel */





#ifdef cray

#define VOID

#define MBF

#define VENDOR_NAME	"CRAY Research Inc"

#define CPU_NAME	"Cray"

#define MACHINE_NAME	"X/Y-MP/C90"

#define OS_NAME		"UNICOS"		/* SysV			*/

#define CHAR_SIZE	8

#define SHORT_SIZE	24

#define INT_SIZE	64

#define LONG_SIZE	64

#define FLOAT_TYPE	CRAYMP_FLOAT

#endif /* cray */





#ifdef alliant

#define VOID

#define VENDOR_NAME	"Alliant Computer Systems Corporation"

#define OS_NAME		"Concentrix"		/* BSD + SysV		*/



#ifdef mc68000

#define MBF

#define MACHINE_NAME	"FX80"

#define CPU_NAME	"Motorola 68000"

#define CHAR_SIZE	8

#define SHORT_SIZE	16

#define INT_SIZE	32

#define LONG_SIZE	32

#define FLOAT_TYPE	IEEE_FLOAT

#endif /* mc68000 */



#ifdef i860

#define LBF

#define MACHINE_NAME	"FX2800"

#define CPU_NAME	"Intel i860"

#define CHAR_SIZE	8

#define SHORT_SIZE	16

#define INT_SIZE	32

#define LONG_SIZE	32

#define FLOAT_TYPE	IEEE_FLOAT

#endif /* i860 */



#endif /* alliant */



#ifdef paragon

#define LBF

#define MACHINE_NAME    "paragon"

#define CPU_NAME        "Paragon i860"

#define CHAR_SIZE       8

#define SHORT_SIZE      16

#define INT_SIZE        32

#define LONG_SIZE       32

#define FLOAT_TYPE      IEEE_FLOAT

#endif /* paragon */

 





/* ncube */

#ifdef ncube

#define VOID

#define LBF

#define VENDOR_NAME	"nCUBE Corporation"

#define CPU_NAME	"ncube"

#define MACHINE_NAME	"nCUBE 2"

#define OS_NAME		"HYDRA-NX"		/* SysV			*/

#define CHAR_SIZE	8

#define SHORT_SIZE	16

#define INT_SIZE	32

#define LONG_SIZE	32

#define FLOAT_TYPE	IEEE_FLOAT

#endif /* ncube */



/* ncube duplicate entry because of compiler inconsistency */

#ifdef NCUBE

#define VOID

#define LBF

#define VENDOR_NAME	"nCUBE Corporation"

#define CPU_NAME	"ncube"

#define MACHINE_NAME	"nCUBE 2"

#define OS_NAME		"HYDRA-NX"		/* SysV			*/

#define CHAR_SIZE	8

#define SHORT_SIZE	16

#define INT_SIZE	32

#define LONG_SIZE	32

#define FLOAT_TYPE	IEEE_FLOAT

#endif /* ncube */



/* Win32 */

#ifdef WIN32

#define VOID

#define LBF

#define VENDOR_NAME	"Microsoft"

#define CPU_NAME	"Intel"

#define MACHINE_NAME	"Tomba"

#define OS_NAME		"Windows95"			/* BSD			*/

#define CHAR_SIZE	8

#define SHORT_SIZE	16

#define INT_SIZE	32

#define LONG_SIZE	32

#define FLOAT_TYPE	IEEE_FLOAT

#endif /* Win32 */



/* NeXT */

#ifdef NeXT

#define VOID

#define MBF

#define VENDOR_NAME	"NeXT Computer Inc"

#define CPU_NAME	"Motorola 68000"

#define MACHINE_NAME	"NeXT"

#define OS_NAME		"MACH"			/* BSD			*/

#define CHAR_SIZE	8

#define SHORT_SIZE	16

#define INT_SIZE	32

#define LONG_SIZE	32

#define FLOAT_TYPE	IEEE_FLOAT

#endif /* NeXT */



/* Apple Mac AUX */

#ifdef _AUX_SOURCE

#define VOID

#define MBF

#define VENDOR_NAME	"Apple Computer Corp"

#define CPU_NAME	"Motorola 68000"

#define MACHINE_NAME	"Apple Macintosh"

#define OS_NAME		"AUX"			/* SysV			*/

#define CHAR_SIZE	8

#define SHORT_SIZE	16

#define INT_SIZE	32

#define LONG_SIZE	32

#define FLOAT_TYPE	IEEE_FLOAT

#endif /* _AUX_SOURCE */



/* IBM RS6000 AIX */

#ifdef _IBMR2

#ifdef _AIX

#define VOID

#define MBF

#define VENDOR_NAME	"International Business Machines Corporation"

#define CPU_NAME	"RISC System/6000"

#define MACHINE_NAME	"RS 6000"

#define OS_NAME		"AIX"			/* SysV			*/

#define CHAR_SIZE	8

#define SHORT_SIZE	16

#define INT_SIZE	32

#define LONG_SIZE	32

#define FLOAT_TYPE	IEEE_FLOAT

#endif /* _AIX */

#endif /* _IBMR2 */



/* HP 9000 HP-UX */

#ifdef hpux

#define VENDOR_NAME	"Hewlett-Packard (HP)"

#define OS_NAME		"HP-UX"			/* SysV			*/



#if hp9000s800 | hp9000s700 | hp9000s300

#define VOID

#define MBF

#define CPU_NAME	"HP9000"

#define MACHINE_NAME	"HP9000 S700/800"

#define CHAR_SIZE	8

#define SHORT_SIZE	16

#define INT_SIZE	32

#define LONG_SIZE	32

#define FLOAT_TYPE	IEEE_FLOAT

#endif /* hp9000 */



#endif /* hpux */



#ifdef MSWIND

#define VOID

#define LBF

#define CHAR_SIZE 	8

#define SHORT_SIZE 	16

#define INT_SIZE 	32

#define LONG_SIZE 	32

#define FLOAT_TYPE 	IEEE_FLOAT

#endif







/*

 *  Crash and Burn:

 *	If neither MBF nor LBF has been defined, or FLOAT_TYPE hasn't been

 *	defined, generate a syntax error to bring the programmer here...

 */



#ifndef MBF

#ifndef LBF

<<<<--+=+ Syntax Error +=+-->>>>	/* Read message below!!!	*/

#endif

#endif

#ifndef FLOAT_TYPE

<<<<--+=+ Syntax Error +=+-->>>>	/* Read message below!!!	*/

#endif



/*

 *  Programmer!  This syntax error was generated because the machine, OS or

 *  C compiler combination on which this code is being compiled is currently

 *  unknown.  This has prevented the setting of several key flags used by

 *  this source to intelligently #ifdef in and out special code.  Please

 *  return to the top of this file and read the pertinent comments to learn

 *  about these flags and how to set them up for your site.  Please do not

 *  simply default them to something you think is right.  Check and make sure!

 *  Bugs arising from faulty defaulting can be very hard to find.

 *

 *  When you have a good set of flags, please forward them to us for inclusion

 *  in a future release of this software.  Thanks!

 */





#endif /* __SDSCCONFIGH__ */

