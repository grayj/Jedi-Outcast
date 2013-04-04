#include "q_shared.h"
#include "bg_public.h"
#include "bg_local.h"

int PM_irand_timesync(int val1, int val2)
{
	int i;

	i = (val1-1) + (Q_random( &pm->cmd.serverTime )*(val2 - val1)) + 1;
	if (i < val1)
	{
		i = val1;
	}
	if (i > val2)
	{
		i = val2;
	}

	return i;
}

void BG_ForcePowerDrain( playerState_t *ps, forcePowers_t forcePower, int overrideAmt )
{
	//take away the power
	int	drain = overrideAmt;

	/*
	if (ps->powerups[PW_FORCE_BOON])
	{
		return;
	}
	*/

	if ( !drain )
	{
		drain = forcePowerNeeded[ps->fd.forcePowerLevel[forcePower]][forcePower];
	}
	if ( !drain )
	{
		return;
	}

	if (forcePower == FP_LEVITATION)
	{ //special case
		int jumpDrain = 0;

		if (ps->velocity[2] > 250)
		{
			jumpDrain = 20;
		}
		else if (ps->velocity[2] > 200)
		{
			jumpDrain = 16;
		}
		else if (ps->velocity[2] > 150)
		{
			jumpDrain = 12;
		}
		else if (ps->velocity[2] > 100)
		{
			jumpDrain = 8;
		}
		else if (ps->velocity[2] > 50)
		{
			jumpDrain = 6;
		}
		else if (ps->velocity[2] > 0)
		{
			jumpDrain = 4;
		}

		if (jumpDrain)
		{
			jumpDrain /= ps->fd.forcePowerLevel[FP_LEVITATION];
		}

		ps->fd.forcePower -= jumpDrain;
		if ( ps->fd.forcePower < 0 )
		{
			ps->fd.forcePower = 0;
		}

		return;
	}

	ps->fd.forcePower -= drain;
	if ( ps->fd.forcePower < 0 )
	{
		ps->fd.forcePower = 0;
	}
}

// Silly, but I'm replacing these macros so they are shorter!
#define AFLAG_IDLE	(SETANIM_FLAG_NORMAL)
#define AFLAG_ACTIVE (/*SETANIM_FLAG_OVERRIDE | */SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS)
#define AFLAG_WAIT (SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS)
#define AFLAG_FINISH (SETANIM_FLAG_HOLD)

saberMoveData_t	saberMoveData[LS_MOVE_MAX] = {//							NB:randomized
	// name			anim				startQ	endQ	setanimflag		blend,	blocking	chain_idle		chain_attack	trailLen
	{"None",		BOTH_STAND1,		Q_R,	Q_R,	AFLAG_IDLE,		350,	BLK_NO,		LS_NONE,		LS_NONE,		0	},	// LS_NONE		= 0,

	// General movements with saber
	{"Ready",		BOTH_STAND2,		Q_R,	Q_R,	AFLAG_IDLE,		350,	BLK_WIDE,	LS_READY,		LS_S_R2L,		0	},	// LS_READY,
	{"Draw",		BOTH_STAND1TO2,		Q_R,	Q_R,	AFLAG_FINISH,	350,	BLK_NO,		LS_READY,		LS_S_R2L,		0	},	// LS_DRAW,
	{"Putaway",		BOTH_STAND2TO1,		Q_R,	Q_R,	AFLAG_FINISH,	350,	BLK_NO,		LS_READY,		LS_S_R2L,		0	},	// LS_PUTAWAY,

	// Attacks
	//UL2LR
	{"TL2BR Att",	BOTH_A1_TL_BR,		Q_TL,	Q_BR,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_R_TL2BR,		LS_R_TL2BR,		200	},	// LS_A_TL2BR
	//SLASH LEFT
	{"L2R Att",		BOTH_A1__L__R,		Q_L,	Q_R,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_R_L2R,		LS_R_L2R,		200 },	// LS_A_L2R
	//LL2UR
	{"BL2TR Att",	BOTH_A1_BL_TR,		Q_BL,	Q_TR,	AFLAG_ACTIVE,	50,		BLK_TIGHT,	LS_R_BL2TR,		LS_R_BL2TR,		200	},	// LS_A_BL2TR
	//LR2UL
	{"BR2TL Att",	BOTH_A1_BR_TL,		Q_BR,	Q_TL,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_R_BR2TL,		LS_R_BR2TL,		200	},	// LS_A_BR2TL
	//SLASH RIGHT
	{"R2L Att",		BOTH_A1__R__L,		Q_R,	Q_L,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_R_R2L,		LS_R_R2L,		200 },// LS_A_R2L
	//UR2LL
	{"TR2BL Att",	BOTH_A1_TR_BL,		Q_TR,	Q_BL,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_R_TR2BL,		LS_R_TR2BL,		200	},	// LS_A_TR2BL
	//SLASH DOWN
	{"T2B Att",		BOTH_A1_T__B_,		Q_T,	Q_B,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_R_T2B,		LS_R_T2B,		200	},	// LS_A_T2B
	//special attacks
	{"Back Stab",	BOTH_A2_STABBACK1,	Q_R,	Q_R,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_READY,		LS_READY,		200	},	// LS_A_BACKSTAB
	{"Back Att",	BOTH_ATTACK_BACK,	Q_R,	Q_R,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_READY,		LS_READY,		200	},	// LS_A_BACK
	{"CR Back Att",	BOTH_CROUCHATTACKBACK1,Q_R,	Q_R,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_READY,		LS_READY,		200	},	// LS_A_BACK_CR
	{"Lunge Att",	BOTH_LUNGE2_B__T_,	Q_B,	Q_T,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_READY,		LS_READY,		200	},	// LS_A_LUNGE
	{"Jump Att",	BOTH_FORCELEAP2_T__B_,Q_T,	Q_B,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_READY,		LS_READY,		200	},	// LS_A_JUMP_T__B_
	{"Flip Stab",	BOTH_JUMPFLIPSTABDOWN,Q_R,	Q_T,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_READY,		LS_T1_T___R,	200	},	// LS_A_FLIP_STAB
	{"Flip Slash",	BOTH_JUMPFLIPSLASHDOWN1,Q_L,Q_R,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_READY,		LS_T1__R_T_,	200	},	// LS_A_FLIP_SLASH

	//starts
	{"TL2BR St",	BOTH_S1_S1_TL,		Q_R,	Q_TL,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_A_TL2BR,		LS_A_TL2BR,		200	},	// LS_S_TL2BR
	{"L2R St",		BOTH_S1_S1__L,		Q_R,	Q_L,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_A_L2R,		LS_A_L2R,		200	},	// LS_S_L2R
	{"BL2TR St",	BOTH_S1_S1_BL,		Q_R,	Q_BL,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_A_BL2TR,		LS_A_BL2TR,		200	},	// LS_S_BL2TR
	{"BR2TL St",	BOTH_S1_S1_BR,		Q_R,	Q_BR,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_A_BR2TL,		LS_A_BR2TL,		200	},	// LS_S_BR2TL
	{"R2L St",		BOTH_S1_S1__R,		Q_R,	Q_R,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_A_R2L,		LS_A_R2L,		200	},	// LS_S_R2L
	{"TR2BL St",	BOTH_S1_S1_TR,		Q_R,	Q_TR,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_A_TR2BL,		LS_A_TR2BL,		200	},	// LS_S_TR2BL
	{"T2B St",		BOTH_S1_S1_T_,		Q_R,	Q_T,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_A_T2B,		LS_A_T2B,		200	},	// LS_S_T2B
	
	//returns
	{"TL2BR Ret",	BOTH_R1_BR_S1,		Q_BR,	Q_R,	AFLAG_FINISH,	100,	BLK_TIGHT,	LS_READY,		LS_READY,		200	},	// LS_R_TL2BR
	{"L2R Ret",		BOTH_R1__R_S1,		Q_R,	Q_R,	AFLAG_FINISH,	100,	BLK_TIGHT,	LS_READY,		LS_READY,		200	},	// LS_R_L2R
	{"BL2TR Ret",	BOTH_R1_TR_S1,		Q_TR,	Q_R,	AFLAG_FINISH,	100,	BLK_TIGHT,	LS_READY,		LS_READY,		200	},	// LS_R_BL2TR
	{"BR2TL Ret",	BOTH_R1_TL_S1,		Q_TL,	Q_R,	AFLAG_FINISH,	100,	BLK_TIGHT,	LS_READY,		LS_READY,		200	},	// LS_R_BR2TL
	{"R2L Ret",		BOTH_R1__L_S1,		Q_L,	Q_R,	AFLAG_FINISH,	100,	BLK_TIGHT,	LS_READY,		LS_READY,		200	},	// LS_R_R2L
	{"TR2BL Ret",	BOTH_R1_BL_S1,		Q_BL,	Q_R,	AFLAG_FINISH,	100,	BLK_TIGHT,	LS_READY,		LS_READY,		200	},	// LS_R_TR2BL
	{"T2B Ret",		BOTH_R1_B__S1,		Q_B,	Q_R,	AFLAG_FINISH,	100,	BLK_TIGHT,	LS_READY,		LS_READY,		200	},	// LS_R_T2B

	//Transitions
	{"BR2R Trans",	BOTH_T1_BR__R,		Q_BR,	Q_R,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_L2R,		LS_A_R2L,		150	},	//# Fast arc bottom right to right
	{"BR2TR Trans",	BOTH_T1_BR_TR,		Q_BR,	Q_TR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_A_TR2BL,		150	},	//# Fast arc bottom right to top right		(use: BOTH_T1_TR_BR)
	{"BR2T Trans",	BOTH_T1_BR_T_,		Q_BR,	Q_T,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_A_T2B,		150	},	//# Fast arc bottom right to top			(use: BOTH_T1_T__BR)
	{"BR2TL Trans",	BOTH_T1_BR_TL,		Q_BR,	Q_TL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BR2TL,		LS_A_TL2BR,		150	},	//# Fast weak spin bottom right to top left
	{"BR2L Trans",	BOTH_T1_BR__L,		Q_BR,	Q_L,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_R2L,		LS_A_L2R,		150	},	//# Fast weak spin bottom right to left
	{"BR2BL Trans",	BOTH_T1_BR_BL,		Q_BR,	Q_BL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TR2BL,		LS_A_BL2TR,		150	},	//# Fast weak spin bottom right to bottom left
	{"R2BR Trans",	BOTH_T1__R_BR,		Q_R,	Q_BR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TL2BR,		LS_A_BR2TL,		150	},	//# Fast arc right to bottom right			(use: BOTH_T1_BR__R)
	{"R2TR Trans",	BOTH_T1__R_TR,		Q_R,	Q_TR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_A_TR2BL,		150	},	//# Fast arc right to top right
	{"R2T Trans",	BOTH_T1__R_T_,		Q_R,	Q_T,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_A_T2B,		150	},	//# Fast ar right to top				(use: BOTH_T1_T___R)
	{"R2TL Trans",	BOTH_T1__R_TL,		Q_R,	Q_TL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BR2TL,		LS_A_TL2BR,		150	},	//# Fast arc right to top left
	{"R2L Trans",	BOTH_T1__R__L,		Q_R,	Q_L,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_R2L,		LS_A_L2R,		150	},	//# Fast weak spin right to left
	{"R2BL Trans",	BOTH_T1__R_BL,		Q_R,	Q_BL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TR2BL,		LS_A_BL2TR,		150	},	//# Fast weak spin right to bottom left
	{"TR2BR Trans",	BOTH_T1_TR_BR,		Q_TR,	Q_BR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TL2BR,		LS_A_BR2TL,		150	},	//# Fast arc top right to bottom right
	{"TR2R Trans",	BOTH_T1_TR__R,		Q_TR,	Q_R,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_L2R,		LS_A_R2L,		150	},	//# Fast arc top right to right			(use: BOTH_T1__R_TR)
	{"TR2T Trans",	BOTH_T1_TR_T_,		Q_TR,	Q_T,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_A_T2B,		150	},	//# Fast arc top right to top				(use: BOTH_T1_T__TR)
	{"TR2TL Trans",	BOTH_T1_TR_TL,		Q_TR,	Q_TL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BR2TL,		LS_A_TL2BR,		150	},	//# Fast arc top right to top left
	{"TR2L Trans",	BOTH_T1_TR__L,		Q_TR,	Q_L,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_R2L,		LS_A_L2R,		150	},	//# Fast arc top right to left
	{"TR2BL Trans",	BOTH_T1_TR_BL,		Q_TR,	Q_BL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TR2BL,		LS_A_BL2TR,		150	},	//# Fast weak spin top right to bottom left
	{"T2BR Trans",	BOTH_T1_T__BR,		Q_T,	Q_BR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TL2BR,		LS_A_BR2TL,		150	},	//# Fast arc top to bottom right
	{"T2R Trans",	BOTH_T1_T___R,		Q_T,	Q_R,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_L2R,		LS_A_R2L,		150	},	//# Fast arc top to right
	{"T2TR Trans",	BOTH_T1_T__TR,		Q_T,	Q_TR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_A_TR2BL,		150	},	//# Fast arc top to top right
	{"T2TL Trans",	BOTH_T1_T__TL,		Q_T,	Q_TL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BR2TL,		LS_A_TL2BR,		150	},	//# Fast arc top to top left
	{"T2L Trans",	BOTH_T1_T___L,		Q_T,	Q_L,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_R2L,		LS_A_L2R,		150	},	//# Fast arc top to left
	{"T2BL Trans",	BOTH_T1_T__BL,		Q_T,	Q_BL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TR2BL,		LS_A_BL2TR,		150	},	//# Fast arc top to bottom left
	{"TL2BR Trans",	BOTH_T1_TL_BR,		Q_TL,	Q_BR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TL2BR,		LS_A_BR2TL,		150	},	//# Fast weak spin top left to bottom right
	{"TL2R Trans",	BOTH_T1_TL__R,		Q_TL,	Q_R,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_L2R,		LS_A_R2L,		150	},	//# Fast arc top left to right			(use: BOTH_T1__R_TL)
	{"TL2TR Trans",	BOTH_T1_TL_TR,		Q_TL,	Q_TR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_A_TR2BL,		150	},	//# Fast arc top left to top right			(use: BOTH_T1_TR_TL)
	{"TL2T Trans",	BOTH_T1_TL_T_,		Q_TL,	Q_T,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_A_T2B,		150	},	//# Fast arc top left to top				(use: BOTH_T1_T__TL)
	{"TL2L Trans",	BOTH_T1_TL__L,		Q_TL,	Q_L,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_R2L,		LS_A_L2R,		150	},	//# Fast arc top left to left				(use: BOTH_T1__L_TL)
	{"TL2BL Trans",	BOTH_T1_TL_BL,		Q_TL,	Q_BL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TR2BL,		LS_A_BL2TR,		150	},	//# Fast arc top left to bottom left
	{"L2BR Trans",	BOTH_T1__L_BR,		Q_L,	Q_BR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TL2BR,		LS_A_BR2TL,		150	},	//# Fast weak spin left to bottom right
	{"L2R Trans",	BOTH_T1__L__R,		Q_L,	Q_R,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_L2R,		LS_A_R2L,		150	},	//# Fast weak spin left to right
	{"L2TR Trans",	BOTH_T1__L_TR,		Q_L,	Q_TR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_A_TR2BL,		150	},	//# Fast arc left to top right			(use: BOTH_T1_TR__L)
	{"L2T Trans",	BOTH_T1__L_T_,		Q_L,	Q_T,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_A_T2B,		150	},	//# Fast arc left to top				(use: BOTH_T1_T___L)
	{"L2TL Trans",	BOTH_T1__L_TL,		Q_L,	Q_TL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BR2TL,		LS_A_TL2BR,		150	},	//# Fast arc left to top left
	{"L2BL Trans",	BOTH_T1__L_BL,		Q_L,	Q_BL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TR2BL,		LS_A_BL2TR,		150	},	//# Fast arc left to bottom left			(use: BOTH_T1_BL__L)
	{"BL2BR Trans",	BOTH_T1_BL_BR,		Q_BL,	Q_BR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TL2BR,		LS_A_BR2TL,		150	},	//# Fast weak spin bottom left to bottom right
	{"BL2R Trans",	BOTH_T1_BL__R,		Q_BL,	Q_R,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_L2R,		LS_A_R2L,		150	},	//# Fast weak spin bottom left to right
	{"BL2TR Trans",	BOTH_T1_BL_TR,		Q_BL,	Q_TR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_A_TR2BL,		150	},	//# Fast weak spin bottom left to top right
	{"BL2T Trans",	BOTH_T1_BL_T_,		Q_BL,	Q_T,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_A_T2B,		150	},	//# Fast arc bottom left to top			(use: BOTH_T1_T__BL)
	{"BL2TL Trans",	BOTH_T1_BL_TL,		Q_BL,	Q_TL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BR2TL,		LS_A_TL2BR,		150	},	//# Fast arc bottom left to top left		(use: BOTH_T1_TL_BL)
	{"BL2L Trans",	BOTH_T1_BL__L,		Q_BL,	Q_L,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_R2L,		LS_A_L2R,		150	},	//# Fast arc bottom left to left

	//Bounces
	{"Bounce BR",	BOTH_B1_BR___,		Q_BR,	Q_BR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TL2BR,		LS_T1_BR_TR,	150	},	
	{"Bounce R",	BOTH_B1__R___,		Q_R,	Q_R,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_L2R,		LS_T1__R__L,	150	},	
	{"Bounce TR",	BOTH_B1_TR___,		Q_TR,	Q_TR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_T1_TR_TL,	150	},	
	{"Bounce T",	BOTH_B1_T____,		Q_T,	Q_T,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_T1_T__BL,	150	},	
	{"Bounce TL",	BOTH_B1_TL___,		Q_TL,	Q_TL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BR2TL,		LS_T1_TL_TR,	150	},	
	{"Bounce L",	BOTH_B1__L___,		Q_L,	Q_L,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_R2L,		LS_T1__L__R,	150	},	
	{"Bounce BL",	BOTH_B1_BL___,		Q_BL,	Q_BL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TR2BL,		LS_T1_BL_TR,	150	},	

	//Deflected attacks (like bounces, but slide off enemy saber, not straight back)
	{"Deflect BR",	BOTH_D1_BR___,		Q_BR,	Q_BR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TL2BR,		LS_T1_BR_TR,	150	},	
	{"Deflect R",	BOTH_D1__R___,		Q_R,	Q_R,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_L2R,		LS_T1__R__L,	150	},	
	{"Deflect TR",	BOTH_D1_TR___,		Q_TR,	Q_TR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_T1_TR_TL,	150	},	
	{"Deflect T",	BOTH_B1_T____,		Q_T,	Q_T,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_T1_T__BL,	150	},	
	{"Deflect TL",	BOTH_D1_TL___,		Q_TL,	Q_TL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BR2TL,		LS_T1_TL_TR,	150	},	
	{"Deflect L",	BOTH_D1__L___,		Q_L,	Q_L,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_R2L,		LS_T1__L__R,	150	},	
	{"Deflect BL",	BOTH_D1_BL___,		Q_BL,	Q_BL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TR2BL,		LS_T1_BL_TR,	150	},	
	{"Deflect B",	BOTH_D1_B____,		Q_B,	Q_B,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_T1_T__BL,	150	},	

	//Reflected attacks
	{"Reflected BR",BOTH_V1_BR_S1,		Q_BR,	Q_BR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_READY,		LS_READY,	150	},//	LS_V1_BR
	{"Reflected R",	BOTH_V1__R_S1,		Q_R,	Q_R,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_READY,		LS_READY,	150	},//	LS_V1__R
	{"Reflected TR",BOTH_V1_TR_S1,		Q_TR,	Q_TR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_READY,		LS_READY,	150	},//	LS_V1_TR
	{"Reflected T",	BOTH_V1_T__S1,		Q_T,	Q_T,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_READY,		LS_READY,	150	},//	LS_V1_T_
	{"Reflected TL",BOTH_V1_TL_S1,		Q_TL,	Q_TL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_READY,		LS_READY,	150	},//	LS_V1_TL
	{"Reflected L",	BOTH_V1__L_S1,		Q_L,	Q_L,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_READY,		LS_READY,	150	},//	LS_V1__L
	{"Reflected BL",BOTH_V1_BL_S1,		Q_BL,	Q_BL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_READY,		LS_READY,	150	},//	LS_V1_BL
	{"Reflected B",	BOTH_V1_B__S1,		Q_B,	Q_B,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_READY,		LS_READY,	150	},//	LS_V1_B_

	// Broken parries
	{"BParry Top",	BOTH_H1_S1_T_,		Q_T,	Q_B,	AFLAG_ACTIVE,	50,		BLK_NO,	LS_READY,		LS_READY,		150	},	// LS_PARRY_UP,
	{"BParry UR",	BOTH_H1_S1_TR,		Q_TR,	Q_BL,	AFLAG_ACTIVE,	50,		BLK_NO,	LS_READY,		LS_READY,		150	},	// LS_PARRY_UR,
	{"BParry UL",	BOTH_H1_S1_TL,		Q_TL,	Q_BR,	AFLAG_ACTIVE,	50,		BLK_NO,	LS_READY,		LS_READY,		150	},	// LS_PARRY_UL,
	{"BParry LR",	BOTH_H1_S1_BL,		Q_BL,	Q_TR,	AFLAG_ACTIVE,	50,		BLK_NO,	LS_READY,		LS_READY,		150	},	// LS_PARRY_LR,
	{"BParry Bot",	BOTH_H1_S1_B_,		Q_B,	Q_T,	AFLAG_ACTIVE,	50,		BLK_NO,	LS_READY,		LS_READY,		150	},	// LS_PARRY_LL
	{"BParry LL",	BOTH_H1_S1_BR,		Q_BR,	Q_TL,	AFLAG_ACTIVE,	50,		BLK_NO,	LS_READY,		LS_READY,		150	},	// LS_PARRY_LL

	// Knockaways
	{"Knock Top",	BOTH_K1_S1_T_,		Q_R,	Q_T,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_BL2TR,		LS_T1_T__BR,		150	},	// LS_PARRY_UP,
	{"Knock UR",	BOTH_K1_S1_TR,		Q_R,	Q_TR,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_BL2TR,		LS_T1_TR__R,		150	},	// LS_PARRY_UR,
	{"Knock UL",	BOTH_K1_S1_TL,		Q_R,	Q_TL,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_BR2TL,		LS_T1_TL__L,		150	},	// LS_PARRY_UL,
	{"Knock LR",	BOTH_K1_S1_BL,		Q_R,	Q_BL,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_TL2BR,		LS_T1_BL_TL,		150	},	// LS_PARRY_LR,
	{"Knock LL",	BOTH_K1_S1_BR,		Q_R,	Q_BR,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_TR2BL,		LS_T1_BR_TR,		150	},	// LS_PARRY_LL

	// Parry
	{"Parry Top",	BOTH_P1_S1_T_,		Q_R,	Q_T,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_BL2TR,		LS_A_T2B,		150	},	// LS_PARRY_UP,
	{"Parry UR",	BOTH_P1_S1_TR,		Q_R,	Q_TL,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_BL2TR,		LS_A_TR2BL,		150	},	// LS_PARRY_UR,
	{"Parry UL",	BOTH_P1_S1_TL,		Q_R,	Q_TR,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_BR2TL,		LS_A_TL2BR,		150	},	// LS_PARRY_UL,
	{"Parry LR",	BOTH_P1_S1_BL,		Q_R,	Q_BR,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_TL2BR,		LS_A_BR2TL,		150	},	// LS_PARRY_LR,
	{"Parry LL",	BOTH_P1_S1_BR,		Q_R,	Q_BL,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_TR2BL,		LS_A_BL2TR,		150	},	// LS_PARRY_LL

	// Reflecting a missile
	{"Reflect Top",	BOTH_P1_S1_T_,		Q_R,	Q_T,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_BL2TR,		LS_A_T2B,		300	},	// LS_PARRY_UP,
	{"Reflect UR",	BOTH_P1_S1_TL,		Q_R,	Q_TR,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_BR2TL,		LS_A_TL2BR,		300	},	// LS_PARRY_UR,
	{"Reflect UL",	BOTH_P1_S1_TR,		Q_R,	Q_TL,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_BL2TR,		LS_A_TR2BL,		300	},	// LS_PARRY_UL,
	{"Reflect LR",	BOTH_P1_S1_BR,		Q_R,	Q_BL,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_TR2BL,		LS_A_BL2TR,		300	},	// LS_PARRY_LR
	{"Reflect LL",	BOTH_P1_S1_BL,		Q_R,	Q_BR,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_TL2BR,		LS_A_BR2TL,		300	},	// LS_PARRY_LL,
};

int transitionMove[Q_NUM_QUADS][Q_NUM_QUADS] = 
{
	LS_NONE,	//Can't transition to same pos!
	LS_T1_BR__R,//40
	LS_T1_BR_TR,
	LS_T1_BR_T_,
	LS_T1_BR_TL,
	LS_T1_BR__L,
	LS_T1_BR_BL,
	LS_NONE,	//No transitions to bottom, and no anims start there, so shouldn't need any
	LS_T1__R_BR,//46
	LS_NONE,	//Can't transition to same pos!
	LS_T1__R_TR,
	LS_T1__R_T_,
	LS_T1__R_TL,
	LS_T1__R__L,
	LS_T1__R_BL,
	LS_NONE,	//No transitions to bottom, and no anims start there, so shouldn't need any
	LS_T1_TR_BR,//52
	LS_T1_TR__R,
	LS_NONE,	//Can't transition to same pos!
	LS_T1_TR_T_,
	LS_T1_TR_TL,
	LS_T1_TR__L,
	LS_T1_TR_BL,
	LS_NONE,	//No transitions to bottom, and no anims start there, so shouldn't need any
	LS_T1_T__BR,//58
	LS_T1_T___R,
	LS_T1_T__TR,
	LS_NONE,	//Can't transition to same pos!
	LS_T1_T__TL,
	LS_T1_T___L,
	LS_T1_T__BL,
	LS_NONE,	//No transitions to bottom, and no anims start there, so shouldn't need any
	LS_T1_TL_BR,//64
	LS_T1_TL__R,
	LS_T1_TL_TR,
	LS_T1_TL_T_,
	LS_NONE,	//Can't transition to same pos!
	LS_T1_TL__L,
	LS_T1_TL_BL,
	LS_NONE,	//No transitions to bottom, and no anims start there, so shouldn't need any
	LS_T1__L_BR,//70
	LS_T1__L__R,
	LS_T1__L_TR,
	LS_T1__L_T_,
	LS_T1__L_TL,
	LS_NONE,	//Can't transition to same pos!
	LS_T1__L_BL,
	LS_NONE,	//No transitions to bottom, and no anims start there, so shouldn't need any
	LS_T1_BL_BR,//76
	LS_T1_BL__R,
	LS_T1_BL_TR,
	LS_T1_BL_T_,
	LS_T1_BL_TL,
	LS_T1_BL__L,
	LS_NONE,	//Can't transition to same pos!
	LS_NONE,	//No transitions to bottom, and no anims start there, so shouldn't need any
	LS_T1_BL_BR,//NOTE: there are no transitions from bottom, so re-use the bottom right transitions
	LS_T1_BR__R,
	LS_T1_BR_TR,
	LS_T1_BR_T_,
	LS_T1_BR_TL,
	LS_T1_BR__L,
	LS_T1_BR_BL,
	LS_NONE		//No transitions to bottom, and no anims start there, so shouldn't need any
};

saberMoveName_t PM_AttackMoveForQuad( int quad )
{
	switch ( quad )
	{
	case Q_B:
	case Q_BR:
		return LS_A_BR2TL;
		break;
	case Q_R:
		return LS_A_R2L;
		break;
	case Q_TR:
		return LS_A_TR2BL;
		break;
	case Q_T:
		return LS_A_T2B;
		break;
	case Q_TL:
		return LS_A_TL2BR;
		break;
	case Q_L:
		return LS_A_L2R;
		break;
	case Q_BL:
		return LS_A_BL2TR;
		break;
	}
	return LS_NONE;
}

int PM_SaberAnimTransitionAnim( int curmove, int newmove )
{
	//FIXME: take FP_SABERATTACK into account here somehow?
	int retmove = newmove;
	if ( curmove == LS_READY )
	{//just standing there
		switch ( newmove )
		{
		case LS_A_TL2BR:
		case LS_A_L2R:
		case LS_A_BL2TR:
		case LS_A_BR2TL:
		case LS_A_R2L:
		case LS_A_TR2BL:
		case LS_A_T2B:
			//transition is the start
			retmove = LS_S_TL2BR + (newmove-LS_A_TL2BR);
			break;
		}
	}
	else
	{
		switch ( newmove )
		{
		//transitioning to ready pose
		case LS_READY:
			switch ( curmove )
			{
			//transitioning from an attack
			case LS_A_TL2BR:
			case LS_A_L2R:
			case LS_A_BL2TR:
			case LS_A_BR2TL:
			case LS_A_R2L:
			case LS_A_TR2BL:
			case LS_A_T2B:
				//transition is the return
				retmove = LS_R_TL2BR + (newmove-LS_A_TL2BR);
				break;
			}
			break;
		//transitioning to an attack
		case LS_A_TL2BR:
		case LS_A_L2R:
		case LS_A_BL2TR:
		case LS_A_BR2TL:
		case LS_A_R2L:
		case LS_A_TR2BL:
		case LS_A_T2B:
			/*if ( newmove == curmove )
			{//FIXME: need a spin or something or go to next level, but for now, just play the return
				retmove = LS_R_TL2BR + (newmove-LS_A_TL2BR);
			}
			else */if ( saberMoveData[curmove].endQuad == saberMoveData[newmove].startQuad )
			{//new move starts from same quadrant
				retmove = newmove;
			}
			else
			{
				switch ( curmove )
				{
				//transitioning from an attack
				case LS_A_TL2BR:
				case LS_A_L2R:
				case LS_A_BL2TR:
				case LS_A_BR2TL:
				case LS_A_R2L:
				case LS_A_TR2BL:
				case LS_A_T2B:
					retmove = transitionMove[saberMoveData[curmove].endQuad][saberMoveData[newmove].startQuad];
					break;
				//transitioning from a return
				case LS_R_TL2BR:
				case LS_R_L2R:
				case LS_R_BL2TR:
				case LS_R_BR2TL:
				case LS_R_R2L:
				case LS_R_TR2BL:
				case LS_R_T2B:
				//transitioning from a bounce
				/*
				case LS_BOUNCE_UL2LL:
				case LS_BOUNCE_LL2UL:
				case LS_BOUNCE_L2LL:
				case LS_BOUNCE_L2UL:
				case LS_BOUNCE_UR2LR:
				case LS_BOUNCE_LR2UR:
				case LS_BOUNCE_R2LR:
				case LS_BOUNCE_R2UR:
				case LS_BOUNCE_TOP:
				case LS_OVER_UR2UL:
				case LS_OVER_UL2UR:
				case LS_BOUNCE_UR:
				case LS_BOUNCE_UL:
				case LS_BOUNCE_LR:
				case LS_BOUNCE_LL:
				*/
				//transitioning from a parry/deflection
				case LS_PARRY_UP:
				case LS_REFLECT_UP:
				case LS_PARRY_UR:
				case LS_REFLECT_UR:
				case LS_PARRY_UL:
				case LS_REFLECT_UL:
				case LS_PARRY_LR:
				case LS_REFLECT_LR:
				case LS_PARRY_LL:
				case LS_REFLECT_LL:
					retmove = transitionMove[saberMoveData[curmove].endQuad][saberMoveData[newmove].startQuad];
					break;
				//NB: transitioning from transitions is fine
				}
			}
			break;
		//transitioning to any other anim is not supported
		}
	}

	if ( retmove == LS_NONE )
	{
		return newmove;
	}

	return retmove;
}


int PM_SaberMoveQuadrantForMovement( usercmd_t *ucmd )
{
	if ( ucmd->rightmove > 0 )
	{//moving right
		if ( ucmd->forwardmove > 0 )
		{//forward right = TL2BR slash
			return Q_TL;
		}
		else if ( ucmd->forwardmove < 0 )
		{//backward right = BL2TR uppercut
			return Q_BL;
		}
		else
		{//just right is a left slice
			return Q_L;
		}
	}
	else if ( ucmd->rightmove < 0 )
	{//moving left
		if ( ucmd->forwardmove > 0 )
		{//forward left = TR2BL slash
			return Q_TR;
		}
		else if ( ucmd->forwardmove < 0 )
		{//backward left = BR2TL uppercut
			return Q_BR;
		}
		else
		{//just left is a right slice
			return Q_R;
		}
	}
	else
	{//not moving left or right
		if ( ucmd->forwardmove > 0 )
		{//forward= T2B slash
			return Q_T;
		}
		else if ( ucmd->forwardmove < 0 )
		{//backward= T2B slash	//or B2T uppercut?
			return Q_T;
		}
		else //if ( curmove == LS_READY )//???
		{//Not moving at all
			return Q_R;
		}
	}
	//return Q_R;//????
}

//===================================================================
qboolean PM_SaberInBounce( int move )
{
	if ( move >= LS_B1_BR && move <= LS_B1_BL )
	{
		return qtrue;
	}
	if ( move >= LS_D1_BR && move <= LS_D1_BL )
	{
		return qtrue;
	}
	return qfalse;
}

qboolean PM_SaberInTransition( int move )
{
	if ( move >= LS_T1_BR__R && move <= LS_T1_BL__L )
	{
		return qtrue;
	}
	return qfalse;
}

qboolean PM_SaberKataDone( void )
{
	if ( (pm->ps->fd.saberAnimLevel >= FORCE_LEVEL_3 && pm->ps->saberAttackChainCount > PM_irand_timesync( 0, 1 )) ||
		( pm->ps->fd.saberAnimLevel == FORCE_LEVEL_2 && pm->ps->saberAttackChainCount > PM_irand_timesync( 2, 5 ) ) )
	{
		return qtrue;
	}
	return qfalse;
}

void PM_SetAnimFrame( playerState_t *gent, int frame, qboolean torso, qboolean legs )
{
	gent->saberLockFrame = frame;
}

void PM_SaberLockBreak( playerState_t *genemy, qboolean victory )
{
	int	winAnim = BOTH_STAND1, loseAnim = BOTH_STAND1;
	qboolean punishLoser = qfalse;

	switch ( (pm->ps->torsoAnim&~ANIM_TOGGLEBIT) )
	{
	case BOTH_BF2LOCK:
		pm->ps->saberMove = LS_A_T2B;
		winAnim = BOTH_A3_T__B_;
		if ( !victory )
		{//no-one won
			genemy->saberMove = LS_A_T2B;
			loseAnim = winAnim;
		}
		else
		{
			//loseAnim = BOTH_KNOCKDOWN4;
			punishLoser = qtrue;
		}
		break;
	case BOTH_BF1LOCK:
		pm->ps->saberMove = LS_K1_T_;
		winAnim = BOTH_K1_S1_T_;
		if ( !victory )
		{//no-one won
			genemy->saberMove = LS_K1_T_;
			loseAnim = winAnim;
		}
		else
		{
			//loseAnim = BOTH_BF1BREAK;
			punishLoser = qtrue;
		}
		break;
	case BOTH_CWCIRCLELOCK:
		winAnim = BOTH_CWCIRCLEBREAK;
		if ( !victory )
		{//no-one won
			loseAnim = winAnim;
		}
		else
		{
			genemy->saberMove = /*genemy->saberBounceMove =*/ LS_H1_BL;
			genemy->saberBlocked = BLOCKED_PARRY_BROKEN;
			//loseAnim = BOTH_H1_S1_BR;
			punishLoser = qtrue;
		}
		break;
	case BOTH_CCWCIRCLELOCK:
		winAnim = BOTH_CCWCIRCLEBREAK;
		if ( !victory )
		{//no-one won
			loseAnim = winAnim;
		}
		else
		{
			genemy->saberMove = /*genemy->saberBounceMove =*/ LS_H1_BR;
			genemy->saberBlocked = BLOCKED_PARRY_BROKEN;
			//loseAnim = BOTH_H1_S1_BL;
			punishLoser = qtrue;
		}
		break;
	}
	PM_SetAnim( SETANIM_BOTH, winAnim, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD, -1 );

	if (punishLoser)
	{
		vec3_t oppDir;

		int strength = 8;

		VectorSubtract(genemy->origin, pm->ps->origin, oppDir);
		VectorNormalize(oppDir);

		genemy->forceHandExtend = HANDEXTEND_KNOCKDOWN;
		genemy->forceHandExtendTime = pm->cmd.serverTime + 1100;
		genemy->forceDodgeAnim = 0; //this toggles between 1 and 0, when it's 1 we should play the get up anim

		genemy->otherKiller = pm->ps->clientNum;
		genemy->otherKillerTime = pm->cmd.serverTime + 5000;
		genemy->otherKillerDebounceTime = pm->cmd.serverTime + 100;

		genemy->velocity[0] = oppDir[0]*(strength*40);
		genemy->velocity[1] = oppDir[1]*(strength*40);
		genemy->velocity[2] = 100;
	}
	else
	{
		vec3_t oppDir;

		int strength = 4;

		VectorSubtract(genemy->origin, pm->ps->origin, oppDir);
		VectorNormalize(oppDir);
		genemy->velocity[0] = oppDir[0]*(strength*40);
		genemy->velocity[1] = oppDir[1]*(strength*40);
		genemy->velocity[2] = 150;

		VectorSubtract(pm->ps->origin, genemy->origin, oppDir);
		VectorNormalize(oppDir);
		pm->ps->velocity[0] = oppDir[0]*(strength*40);
		pm->ps->velocity[1] = oppDir[1]*(strength*40);
		pm->ps->velocity[2] = 150;

		genemy->forceHandExtend = HANDEXTEND_WEAPONREADY;
	}

	pm->ps->weaponTime = 0;//pm->ps->torsoTimer;
	//The enemy unfortunately has no valid torso animation time at this point, so just use ours
	genemy->weaponTime = 0;

	pm->ps->saberLockTime = genemy->saberLockTime = 0;
	pm->ps->saberLockFrame = genemy->saberLockFrame = 0;
	pm->ps->saberLockEnemy = genemy->saberLockEnemy = 0;

	pm->ps->forceHandExtend = HANDEXTEND_WEAPONREADY;

	PM_AddEvent( EV_JUMP );
	if ( !victory )
	{//no-one won
		BG_AddPredictableEventToPlayerstate(EV_JUMP, 0, genemy);
	}
	else
	{
		if ( Q_irand( 0, 1 ) )
		{
			BG_AddPredictableEventToPlayerstate(EV_JUMP, PM_irand_timesync( 0, 75 ), genemy);
		}
	}
}

extern qboolean ValidAnimFileIndex ( int index );
void PM_SaberLocked( void )
{
	int	remaining = 0;
	/*
	if ( pm->ps->weaponTime )
	{//can't attack yet
		return;
	}
	*/
	playerState_t *genemy = pm->bgClients[pm->ps->saberLockEnemy];
	if ( !genemy )
	{
		return;
	}
	if ( ( (pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_BF2LOCK ||
			(pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_BF1LOCK ||
			(pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_CWCIRCLELOCK ||
			(pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_CCWCIRCLELOCK )
		&& ( (genemy->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_BF2LOCK ||
			(genemy->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_BF1LOCK ||
			(genemy->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_CWCIRCLELOCK ||
			(genemy->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_CCWCIRCLELOCK )
		)
	{
		float dist = 0;

		pm->ps->torsoTimer = 0;
		pm->ps->weaponTime = 0;
		genemy->torsoTimer = 0;
		genemy->weaponTime = 0;

		dist = DistanceSquared(pm->ps->origin,genemy->origin);
		if ( dist < 64 || dist > 6400 )//( dist < 128 || dist > 2304 )
		{//between 8 and 80 from each other//was 16 and 48
			PM_SaberLockBreak( genemy, qfalse );
			return;
		}
		if ( (pm->cmd.buttons & BUTTON_ATTACK) || pm->ps->saberLockAdvance )
		{//holding attack
			if (pm->ps->saberLockAdvance)
			{//tapping
				animation_t *anim;
				float		currentFrame;
				int			curFrame;
				int			strength = 1;

				pm->ps->saberLockAdvance = qfalse;

				anim = &pm->animations[pm->ps->torsoAnim&~ANIM_TOGGLEBIT];
	
				currentFrame = pm->ps->saberLockFrame;

				strength = pm->ps->fd.forcePowerLevel[FP_SABERATTACK]+1;

				if ( (pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_CCWCIRCLELOCK ||
					(pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_BF2LOCK )
				{
					curFrame = floor( currentFrame )-strength;
					//drop my frame one
					if ( curFrame <= anim->firstFrame )
					{//I won!  Break out
						PM_SaberLockBreak( genemy, qtrue );
						return;
					}
					else
					{
						PM_SetAnimFrame( pm->ps, curFrame, qtrue, qtrue );
						remaining = curFrame-anim->firstFrame;
					}
				}
				else
				{
					curFrame = ceil( currentFrame )+strength;
					//advance my frame one
					if ( curFrame >= anim->firstFrame+anim->numFrames )
					{//I won!  Break out
						PM_SaberLockBreak( genemy, qtrue );
						return;
					}
					else
					{
						PM_SetAnimFrame( pm->ps, curFrame, qtrue, qtrue );
						remaining = anim->firstFrame+anim->numFrames-curFrame;
					}
				}
				if ( !Q_irand( 0, 2 ) )
				{
					PM_AddEvent( EV_JUMP );
				}
			}
			else
			{
				return;
			}
			if( 1/*ValidAnimFileIndex( genemy->client->clientInfo.animFileIndex )*/ )
			{
				animation_t *anim;
				anim = &pm->animations[(genemy->torsoAnim&~ANIM_TOGGLEBIT)];

				if ( (genemy->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_CWCIRCLELOCK ||
					(genemy->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_BF1LOCK )
				{
					if ( !Q_irand( 0, 2 ) )
					{
						BG_AddPredictableEventToPlayerstate(EV_PAIN, floor((float)80/100*100.0f), genemy);
					}
					PM_SetAnimFrame( genemy, anim->firstFrame+remaining, qtrue, qtrue );
				}
				else
				{
					PM_SetAnimFrame( genemy, anim->firstFrame+anim->numFrames-remaining, qtrue, qtrue );
				}
			}
		}
	}
	else
	{//something broke us out of it
		PM_SaberLockBreak( genemy, qfalse );
	}
}

qboolean PM_SaberInBrokenParry( int move )
{
	if ( move >= LS_H1_T_ && move <= LS_H1_BL )
	{
		return qtrue;
	}
	return qfalse;
}


int PM_BrokenParryForParry( int move )
{
	//FIXME: need actual anims for this
	//FIXME: need to know which side of the saber was hit!  For now, we presume the saber gets knocked away from the center
	switch ( move )
	{
	case LS_PARRY_UP:
		return LS_H1_T_;
		break;
	case LS_PARRY_UR:
		return LS_H1_TR;
		break;
	case LS_PARRY_UL:
		return LS_H1_TL;
		break;
	case LS_PARRY_LR:
		return LS_H1_BL;
		break;
	case LS_PARRY_LL:
		return LS_H1_BR;
		break;
	case LS_READY:
		return LS_H1_B_;//???
		break;
	}
	return LS_NONE;
}

#define BACK_STAB_DISTANCE 128//64

qboolean PM_CanBackstab(void)
{
	trace_t tr;
	vec3_t flatAng;
	vec3_t fwd, back;
	vec3_t trmins = {-15, -15, -8};
	vec3_t trmaxs = {15, 15, 8};

	VectorCopy(pm->ps->viewangles, flatAng);
	flatAng[PITCH] = 0;

	AngleVectors(flatAng, fwd, 0, 0);

	back[0] = pm->ps->origin[0] - fwd[0]*BACK_STAB_DISTANCE;
	back[1] = pm->ps->origin[1] - fwd[1]*BACK_STAB_DISTANCE;
	back[2] = pm->ps->origin[2] - fwd[2]*BACK_STAB_DISTANCE;

	pm->trace(&tr, pm->ps->origin, trmins, trmaxs, back, pm->ps->clientNum, MASK_PLAYERSOLID);

	if (tr.fraction != 1.0 && tr.entityNum >= 0 && tr.entityNum < MAX_CLIENTS)
	{ //We don't have real entity access here so we can't do an indepth check. But if it's a client and it's behind us, I guess that's reason enough to stab backward
		return qtrue;
	}

	return qfalse;
}

saberMoveName_t PM_SaberFlipOverAttackMove(trace_t *tr)
{
	//FIXME: check above for room enough to jump!
	vec3_t fwdAngles, jumpFwd;
	float zDiff = 0;
	playerState_t *psData;

	VectorCopy( pm->ps->viewangles, fwdAngles );
	fwdAngles[PITCH] = fwdAngles[ROLL] = 0;
	AngleVectors( fwdAngles, jumpFwd, NULL, NULL );
	VectorScale( jumpFwd, /*100*/50, pm->ps->velocity );
	pm->ps->velocity[2] = 400;

	psData = pm->bgClients[tr->entityNum];

	pm->ps->velocity[2] *= 1;//(pm->gent->enemy->maxs[2]-pm->gent->enemy->mins[2])/64.0f;

	//go higher for enemies higher than you, lower for those lower than you
	if (psData)
	{
		zDiff = psData->origin[2] - pm->ps->origin[2];
	}
	else
	{
		zDiff = 0;
	}
	pm->ps->velocity[2] += (zDiff)*1.5f;

	//clamp to decent-looking values
	if ( zDiff <= 0 && pm->ps->velocity[2] < 200 )
	{//if we're on same level, don't let me jump so low, I clip into the ground
		pm->ps->velocity[2] = 200;
	}
	else if ( pm->ps->velocity[2] < 100 )
	{
		pm->ps->velocity[2] = 100;
	}
	else if ( pm->ps->velocity[2] > 400 )
	{
		pm->ps->velocity[2] = 400;
	}

	pm->ps->fd.forceJumpZStart = pm->ps->origin[2];//so we don't take damage if we land at same height

	PM_AddEvent( EV_JUMP );
	pm->ps->fd.forceJumpSound = 1;
	pm->cmd.upmove = 0;

	if ( Q_irand( 0, 1 ) )
	{
		return LS_A_FLIP_STAB;
	}
	else
	{
		return LS_A_FLIP_SLASH;
	}
}

#define FLIPHACK_DISTANCE 200

qboolean PM_SomeoneInFront(trace_t *tr)
{ //Also a very simplified version of the sp counterpart
	vec3_t flatAng;
	vec3_t fwd, back;
	vec3_t trmins = {-15, -15, -8};
	vec3_t trmaxs = {15, 15, 8};

	VectorCopy(pm->ps->viewangles, flatAng);
	flatAng[PITCH] = 0;

	AngleVectors(flatAng, fwd, 0, 0);

	back[0] = pm->ps->origin[0] + fwd[0]*FLIPHACK_DISTANCE;
	back[1] = pm->ps->origin[1] + fwd[1]*FLIPHACK_DISTANCE;
	back[2] = pm->ps->origin[2] + fwd[2]*FLIPHACK_DISTANCE;

	pm->trace(tr, pm->ps->origin, trmins, trmaxs, back, pm->ps->clientNum, MASK_PLAYERSOLID);

	if (tr->fraction != 1.0 && tr->entityNum >= 0 && tr->entityNum < MAX_CLIENTS)
	{
		return qtrue;
	}

	return qfalse;
}

saberMoveName_t PM_SaberLungeAttackMove( void )
{
	vec3_t fwdAngles, jumpFwd;

	VectorCopy( pm->ps->viewangles, fwdAngles );
	fwdAngles[PITCH] = fwdAngles[ROLL] = 0;
	//do the lunge
	AngleVectors( fwdAngles, jumpFwd, NULL, NULL );
	VectorScale( jumpFwd, 150, pm->ps->velocity );
	//pm->ps->velocity[2] = 50;
	PM_AddEvent( EV_JUMP );

	return LS_A_LUNGE;
}

saberMoveName_t PM_SaberJumpAttackMove( void )
{
	vec3_t fwdAngles, jumpFwd;

	VectorCopy( pm->ps->viewangles, fwdAngles );
	fwdAngles[PITCH] = fwdAngles[ROLL] = 0;
	AngleVectors( fwdAngles, jumpFwd, NULL, NULL );
	VectorScale( jumpFwd, /*200*/300, pm->ps->velocity );
	pm->ps->velocity[2] = 280;//180;
	pm->ps->fd.forceJumpZStart = pm->ps->origin[2];//so we don't take damage if we land at same height

	PM_AddEvent( EV_JUMP );
	pm->ps->fd.forceJumpSound = 1;
	pm->cmd.upmove = 0;

	return LS_A_JUMP_T__B_;
}

float PM_GroundDistance(void)
{
	trace_t tr;
	vec3_t down;

	VectorCopy(pm->ps->origin, down);

	down[2] -= 4096;

	pm->trace(&tr, pm->ps->origin, pm->mins, pm->maxs, down, pm->ps->clientNum, MASK_SOLID);

	VectorSubtract(pm->ps->origin, tr.endpos, down);

	return VectorLength(down);
}

saberMoveName_t PM_SaberAttackForMovement(saberMoveName_t curmove)
{
	saberMoveName_t newmove = -1;

	if ( pm->cmd.rightmove > 0 )
	{//moving right
		if ( pm->cmd.forwardmove > 0 )
		{//forward right = TL2BR slash
			newmove = LS_A_TL2BR;
		}
		else if ( pm->cmd.forwardmove < 0 )
		{//backward right = BL2TR uppercut
			newmove = LS_A_BL2TR;
		}
		else
		{//just right is a left slice
			newmove = LS_A_L2R;
		}
	}
	else if ( pm->cmd.rightmove < 0 )
	{//moving left
		if ( pm->cmd.forwardmove > 0 )
		{//forward left = TR2BL slash
			newmove = LS_A_TR2BL;
		}
		else if ( pm->cmd.forwardmove < 0 )
		{//backward left = BR2TL uppercut
			newmove = LS_A_BR2TL;
		}
		else
		{//just left is a right slice
			newmove = LS_A_R2L;
		}
	}
	else
	{//not moving left or right
		if ( pm->cmd.forwardmove > 0 )
		{//forward= T2B slash
			if (pm->ps->fd.saberAnimLevel == FORCE_LEVEL_2 &&
				/*pm->ps->groundEntityNum != ENTITYNUM_NONE &&*/
				pm->ps->velocity[2] > 100 &&
				PM_GroundDistance() < 32 &&
				!BG_InSpecialJump(pm->ps->legsAnim))
			{ //FLIP AND DOWNWARD ATTACK
				trace_t tr;

				if (PM_SomeoneInFront(&tr))
				{
					newmove = PM_SaberFlipOverAttackMove(&tr);
				}
			}
			else if (pm->ps->fd.saberAnimLevel == FORCE_LEVEL_1 &&
				(pm->ps->pm_flags & PMF_DUCKED) &&
				pm->ps->weaponTime <= 0)
			{ //LUNGE (weak)
				newmove = PM_SaberLungeAttackMove();
			}
			else
			{
				newmove = LS_A_T2B;
			}
		}
		else if ( pm->cmd.forwardmove < 0 )
		{//backward= T2B slash//B2T uppercut?
			if (PM_CanBackstab())
			{ //BACKSTAB (attack varies by level)
				if (pm->ps->fd.saberAnimLevel >= FORCE_LEVEL_2)
				{//medium and higher attacks
					if ( (pm->ps->pm_flags&PMF_DUCKED) || pm->cmd.upmove < 0 )
					{
						newmove = LS_A_BACK_CR;
					}
					else
					{
						newmove = LS_A_BACK;
					}
				}
				else
				{ //weak attack
					newmove = LS_A_BACKSTAB;
				}
			}
			else
			{
				newmove = LS_A_T2B;
			}
		}
		else if ( PM_SaberInBounce( curmove ) )
		{//bounces should go to their default attack if you don't specify a direction but are attacking
			if ( PM_SaberKataDone() )
			{
				newmove = saberMoveData[curmove].chain_idle;
			}
			else
			{
				newmove = saberMoveData[curmove].chain_attack;
			}
		}
		else if ( curmove == LS_READY )
		{//Not moving at all, shouldn't have gotten here...?
			//for now, just pick a random attack
			//newmove = Q_irand( LS_A_TL2BR, LS_A_T2B );
			//rww - If we don't seed with a "common" value, the client and server will get mismatched
			//prediction values. Under laggy conditions this will cause the appearance of rapid swing
			//sequence changes.
			newmove = PM_irand_timesync(LS_A_TL2BR, LS_A_T2B);
		}
	}

	return newmove;
}
/*
=================
PM_WeaponLightsaber

Consults a chart to choose what to do with the lightsaber.
While this is a little different than the Quake 3 code, there is no clean way of using the Q3 code for this kind of thing.
=================
*/
// Ultimate goal is to set the sabermove to the proper next location
// Note that if the resultant animation is NONE, then the animation is essentially "idle", and is set in WP_TorsoAnim
void PM_WeaponLightsaber(void)
{
	int			addTime,amount;
	qboolean	delayed_fire = qfalse;
	int			anim=-1, curmove, newmove=LS_NONE;

	qboolean saberInAir = qtrue;
	qboolean checkOnlyWeap = qfalse;

 	if ( pm->ps->saberLockTime > pm->cmd.serverTime )
	{
		pm->ps->saberMove = LS_NONE;
		PM_SaberLocked();
		return;
	}
	else
	{
		if ( ( (pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_BF2LOCK ||
				(pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_BF1LOCK ||
				(pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_CWCIRCLELOCK ||
				(pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_CCWCIRCLELOCK ||
				pm->ps->saberLockFrame )
			)
		{
			if (pm->ps->saberLockEnemy < ENTITYNUM_NONE &&
				pm->ps->saberLockEnemy >= 0)
			{
				playerState_t *en;

				en = pm->bgClients[pm->ps->saberLockEnemy];

				if (en)
				{
					PM_SaberLockBreak(en, qfalse);
					return;
				}
			}

			if ( ( (pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_BF2LOCK ||
					(pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_BF1LOCK ||
					(pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_CWCIRCLELOCK ||
					(pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_CCWCIRCLELOCK ||
					pm->ps->saberLockFrame )
				)
			{
				pm->ps->torsoTimer = 0;
				PM_SetAnim(SETANIM_TORSO,BOTH_STAND1,SETANIM_FLAG_OVERRIDE, 100);
				pm->ps->saberLockFrame = 0;
			}
		}
	}

	if (pm->ps->saberHolstered)
	{
		if (pm->ps->saberMove != LS_READY)
		{
			PM_SetSaberMove( LS_READY );
		}

		if ((pm->ps->legsAnim & ~ANIM_TOGGLEBIT) != (pm->ps->torsoAnim & ~ANIM_TOGGLEBIT))
		{
			PM_SetAnim(SETANIM_TORSO,(pm->ps->legsAnim & ~ANIM_TOGGLEBIT),SETANIM_FLAG_OVERRIDE, 100);
		}

		if (BG_InSaberStandAnim(pm->ps->torsoAnim))
		{
			PM_SetAnim(SETANIM_TORSO,BOTH_STAND1,SETANIM_FLAG_OVERRIDE, 100);
		}

		if (pm->ps->weaponTime < 1 && ((pm->cmd.buttons & BUTTON_ALT_ATTACK) || (pm->cmd.buttons & BUTTON_ATTACK)))
		{
			if (pm->ps->duelTime < pm->cmd.serverTime)
			{
				pm->ps->saberHolstered = qfalse;
				PM_AddEvent(EV_SABER_UNHOLSTER);
			}
		}

		if ( pm->ps->weaponTime > 0 )
		{
			pm->ps->weaponTime -= pml.msec;
		}

		checkOnlyWeap = qtrue;
		goto weapChecks;
	}

	if ((pm->cmd.buttons & BUTTON_ALT_ATTACK) &&
		pm->ps->weaponTime < 1 &&
		pm->ps->saberCanThrow &&
		pm->ps->fd.forcePower >= forcePowerNeeded[pm->ps->fd.forcePowerLevel[FP_SABERTHROW]][FP_SABERTHROW] &&
		!BG_HasYsalamiri(pm->gametype, pm->ps) &&
		BG_CanUseFPNow(pm->gametype, pm->ps, pm->cmd.serverTime, FP_SABERTHROW)
		)
	{ //might as well just check for a saber throw right here
		//This will get set to false again once the saber makes it back to its owner game-side
		if (!pm->ps->saberInFlight)
		{
			pm->ps->fd.forcePower -= forcePowerNeeded[pm->ps->fd.forcePowerLevel[FP_SABERTHROW]][FP_SABERTHROW];
		}

		pm->ps->saberInFlight = qtrue;
	}
	
	if ( pm->ps->saberInFlight )
	{//guiding saber
		PM_SetAnim(SETANIM_TORSO, BOTH_SABERPULL, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD, 100);
		pm->ps->torsoTimer = 1;
		return;
	}

   // don't allow attack until all buttons are up
	if ( pm->ps->pm_flags & PMF_RESPAWNED ) {
		return;
	}

	// check for dead player
	if ( pm->ps->stats[STAT_HEALTH] <= 0 ) {
		return;
	}

	if (pm->ps->weaponstate == WEAPON_READY ||
		pm->ps->weaponstate == WEAPON_IDLE)
	{
		if (pm->ps->saberMove != LS_READY && pm->ps->weaponTime <= 0 && !pm->ps->saberBlocked)
		{
			PM_SetSaberMove( LS_READY );
		}
	}

	if( (pm->ps->torsoAnim & ~ANIM_TOGGLEBIT) == BOTH_RUN2 ||
		(pm->ps->torsoAnim & ~ANIM_TOGGLEBIT) == BOTH_RUN1 )
	{
		if ((pm->ps->torsoAnim & ~ANIM_TOGGLEBIT) != (pm->ps->legsAnim & ~ANIM_TOGGLEBIT))
		{
			PM_SetAnim(SETANIM_TORSO,(pm->ps->legsAnim & ~ANIM_TOGGLEBIT),SETANIM_FLAG_OVERRIDE, 100);
		}
	}

	// make weapon function
	if ( pm->ps->weaponTime > 0 ) {
		pm->ps->weaponTime -= pml.msec;

		if (pm->ps->saberBlocked && pm->ps->torsoAnim != saberMoveData[pm->ps->saberMove].animToUse)
		{ //rww - keep him in the blocking pose until he can attack again
			PM_SetAnim(SETANIM_TORSO,saberMoveData[pm->ps->saberMove].animToUse,saberMoveData[pm->ps->saberMove].animSetFlags|SETANIM_FLAG_HOLD, saberMoveData[pm->ps->saberMove].blendTime);
			return;
		}
	}
	else
	{
		pm->ps->weaponstate = WEAPON_READY;
	}

	// Now we react to a block action by the player's lightsaber.
	if ( pm->ps->saberBlocked )
	{
		int firstSet = 0;

		if (!pm->ps->weaponTime)
		{
			firstSet = 1;
		}

		switch ( pm->ps->saberBlocked )
		{
			case BLOCKED_PARRY_BROKEN:
				//whatever parry we were is in now broken, play the appropriate knocked-away anim
				{
					int nextMove;

					if ( PM_SaberInBrokenParry( pm->ps->saberMove ) )
					{//already have one...?
						nextMove = pm->ps->saberMove;
					}
					else
					{
						nextMove = PM_BrokenParryForParry( pm->ps->saberMove );
					}
					if ( nextMove != LS_NONE )
					{
						PM_SetSaberMove( nextMove );
						pm->ps->weaponTime = pm->ps->torsoTimer;
					}
					else
					{//Maybe in a knockaway?
					}
				}
				break;
			case BLOCKED_ATK_BOUNCE:
				// If there is absolutely no blocked move in the chart, don't even mess with the animation.
				// OR if we are already in a block or parry.
				if (pm->ps->saberMove >= LS_T1_BR__R/*LS_BOUNCE_TOP*/ )//|| saberMoveData[pm->ps->saberMove].bounceMove == LS_NONE )
				{//an actual bounce?  Other bounces before this are actually transitions?
					pm->ps->saberBlocked = BLOCKED_NONE;
				}
				else
				{
					int bounceMove;

					if ( pm->cmd.buttons & BUTTON_ATTACK )
					{//transition to a new attack
						int newQuad = PM_SaberMoveQuadrantForMovement( &pm->cmd );
						while ( newQuad == saberMoveData[pm->ps->saberMove].startQuad )
						{//player is still in same attack quad, don't repeat that attack because it looks bad, 
							//FIXME: try to pick one that might look cool?
							//newQuad = Q_irand( Q_BR, Q_BL );
							newQuad = PM_irand_timesync( Q_BR, Q_BL );
							//FIXME: sanity check, just in case?
						}//else player is switching up anyway, take the new attack dir
						bounceMove = transitionMove[saberMoveData[pm->ps->saberMove].startQuad][newQuad];
					}
					else
					{//return to ready
						if ( saberMoveData[pm->ps->saberMove].startQuad == Q_T )
						{
							bounceMove = LS_R_BL2TR;
						}
						else if ( saberMoveData[pm->ps->saberMove].startQuad < Q_T )
						{
							bounceMove = LS_R_TL2BR+saberMoveData[pm->ps->saberMove].startQuad-Q_BR;
						}
						else// if ( saberMoveData[pm->ps->saberMove].startQuad > Q_T )
						{
							bounceMove = LS_R_BR2TL+saberMoveData[pm->ps->saberMove].startQuad-Q_TL;
						}
					}
					PM_SetSaberMove( bounceMove );

					pm->ps->weaponTime = pm->ps->torsoTimer;//+saberMoveData[bounceMove].blendTime+SABER_BLOCK_DUR;

				}
				break;
			case BLOCKED_UPPER_RIGHT:
				PM_SetSaberMove( LS_PARRY_UR );
				break;
			case BLOCKED_UPPER_RIGHT_PROJ:
				PM_SetSaberMove( LS_REFLECT_UR );
				break;
			case BLOCKED_UPPER_LEFT:
				PM_SetSaberMove( LS_PARRY_UL );
				break;
			case BLOCKED_UPPER_LEFT_PROJ:
				PM_SetSaberMove( LS_REFLECT_UL );
				break;
			case BLOCKED_LOWER_RIGHT:
				PM_SetSaberMove( LS_PARRY_LR );
				break;
			case BLOCKED_LOWER_RIGHT_PROJ:
				PM_SetSaberMove( LS_REFLECT_LR );
				break;
			case BLOCKED_LOWER_LEFT:
				PM_SetSaberMove( LS_PARRY_LL );
				break;
			case BLOCKED_LOWER_LEFT_PROJ:
				PM_SetSaberMove( LS_REFLECT_LL);
				break;
			case BLOCKED_TOP:
				PM_SetSaberMove( LS_PARRY_UP );
				break;
			case BLOCKED_TOP_PROJ:
				PM_SetSaberMove( LS_REFLECT_UP );
				break;
			default:
				pm->ps->saberBlocked = BLOCKED_NONE;
				break;
		}

		if (pm->ps->saberBlocked != BLOCKED_ATK_BOUNCE && pm->ps->saberBlocked != BLOCKED_PARRY_BROKEN && pm->ps->weaponTime < 1)
		{
			pm->ps->torsoTimer = SABER_BLOCK_DUR;
			pm->ps->weaponTime = pm->ps->torsoTimer;
		}

		if (firstSet)
		{
			return;
		}

		// Charging is like a lead-up before attacking again.  This is an appropriate use, or we can create a new weaponstate for blocking
		pm->ps->weaponstate = WEAPON_READY;

		// Done with block, so stop these active weapon branches.
		return;
	}

weapChecks:
	// check for weapon change
	// can't change if weapon is firing, but can change again if lowering or raising
	if ( pm->ps->weaponTime <= 0 || pm->ps->weaponstate != WEAPON_FIRING ) {
		if ( pm->ps->weapon != pm->cmd.weapon ) {
			PM_BeginWeaponChange( pm->cmd.weapon );
		}
	}

	if ( pm->ps->weaponTime > 0 ) 
	{
		return;
	}

	// *********************************************************
	// WEAPON_DROPPING
	// *********************************************************

	// change weapon if time
	if ( pm->ps->weaponstate == WEAPON_DROPPING ) {
		PM_FinishWeaponChange();
		return;
	}

	// *********************************************************
	// WEAPON_RAISING
	// *********************************************************

	if ( pm->ps->weaponstate == WEAPON_RAISING ) 
	{//Just selected the weapon
		pm->ps->weaponstate = WEAPON_IDLE;
		if((pm->ps->legsAnim & ~ANIM_TOGGLEBIT) == BOTH_WALK1 )
		{
			PM_SetAnim(SETANIM_TORSO,BOTH_WALK1,SETANIM_FLAG_NORMAL, 100);
		}
		else if((pm->ps->legsAnim & ~ANIM_TOGGLEBIT) == BOTH_RUN2 )
		{
			PM_SetAnim(SETANIM_TORSO,BOTH_RUN2,SETANIM_FLAG_NORMAL, 100);
		}
		else if((pm->ps->legsAnim & ~ANIM_TOGGLEBIT) == BOTH_WALK2 )
		{
			PM_SetAnim(SETANIM_TORSO,BOTH_WALK2,SETANIM_FLAG_NORMAL, 100);
		}
		else
		{
			PM_SetAnim(SETANIM_TORSO,PM_GetSaberStance(),SETANIM_FLAG_NORMAL, 100);
		}

		if (pm->ps->weaponstate == WEAPON_RAISING)
		{
			return;
		}

	}

	if (checkOnlyWeap)
	{
		return;
	}

	// *********************************************************
	// Check for WEAPON ATTACK
	// *********************************************************

	// NOTENOTE This is simply a client-side struct.  Anything that is needed client and server should be moved to bg_weapon.

	if(!delayed_fire)
	{
		// Start with the current move, and cross index it with the current control states.
		if ( pm->ps->saberMove > LS_NONE && pm->ps->saberMove < LS_MOVE_MAX )
		{
			curmove = pm->ps->saberMove;
		}
		else
		{
			curmove = LS_READY;
		}
		// check for fire
		if ( !(pm->cmd.buttons & (BUTTON_ATTACK/*|BUTTON_ALT_ATTACK*/)) )
		{
			if (pm->ps->weaponTime != 0)
			{//Still firing
				pm->ps->weaponstate = WEAPON_FIRING;
			}
			else if ( pm->ps->weaponstate != WEAPON_READY )
			{
				pm->ps->weaponstate = WEAPON_IDLE;
			}
			//Check for finishing an anim if necc.
			if ( curmove >= LS_S_TL2BR && curmove <= LS_S_T2B )
			{//started a swing, must continue from here
				newmove = LS_A_TL2BR + (curmove-LS_S_TL2BR);
			}
			else if ( curmove >= LS_A_TL2BR && curmove <= LS_A_T2B )
			{//finished an attack, must continue from here
				newmove = LS_R_TL2BR + (curmove-LS_A_TL2BR);
			}
			else if ( PM_SaberInTransition( curmove ) )
			{//in a transition, must play sequential attack
				newmove = saberMoveData[curmove].chain_attack;
			}
			else if ( PM_SaberInBounce( curmove ) )
			{//in a bounce
				newmove = saberMoveData[curmove].chain_idle;//oops, not attacking, so don't chain
			}
			else
			{//FIXME: what about returning from a parry?
				PM_SetSaberMove( LS_READY );
				return;
			}
		}

		// ***************************************************
		// Pressing attack, so we must look up the proper attack move.

		saberInAir = qtrue;

		if ( pm->ps->weaponTime > 0 )
		{	// Last attack is not yet complete.
			pm->ps->weaponstate = WEAPON_FIRING;
			return;
		}
		else
		{
			int	both = qfalse;

			if ( curmove >= LS_PARRY_UP && curmove <= LS_REFLECT_LL )
			{//from a parry or deflection, can go directly into an attack (?)
				switch ( saberMoveData[curmove].endQuad )
				{
				case Q_T:
					newmove = LS_A_T2B;
					break;
				case Q_TR:
					newmove = LS_A_TL2BR;
					break;
				case Q_TL:
					newmove = LS_A_TR2BL;
					break;
				case Q_BR:
					newmove = LS_A_BR2TL;
					break;
				case Q_BL:
					newmove = LS_A_BL2TR;
					break;
				//shouldn't be a parry that ends at L, R or B
				}
			}

			if ( newmove != LS_NONE )
			{//have a valid, final LS_ move picked, so skip findingt he transition move and just get the anim
				anim = saberMoveData[newmove].animToUse;
			}

			//FIXME: diagonal dirs use the figure-eight attacks from ready pose?
			if ( anim == -1 )
			{
				//FIXME: take FP_SABER_OFFENSE into account here somehow?
				if ( curmove >= LS_T1_BR__R && curmove <= LS_T1_BL__L )
				{//in a transition, must play sequential attack
					newmove = saberMoveData[curmove].chain_attack;
				}
				else if ( curmove >= LS_S_TL2BR && curmove <= LS_S_T2B )
				{//started a swing, must continue from here
					newmove = LS_A_TL2BR + (curmove-LS_S_TL2BR);
				}
				else
				{
					if ( PM_SaberKataDone() )
					{//we came from a bounce and cannot chain to another attack because our kata is done
						newmove = saberMoveData[curmove].chain_idle;
					}
					else
					{
						saberMoveName_t checkMove = PM_SaberAttackForMovement(curmove);
						if (checkMove != -1)
						{
							newmove = checkMove;
						}
					}
				}
				/*
				if ( newmove == LS_NONE )
				{//FIXME: should we allow this?  Are there some anims that you should never be able to chain into an attack?
					//only curmove that might get in here is LS_NONE, LS_DRAW, LS_PUTAWAY and the LS_R_ returns... all of which are in Q_R
					newmove = PM_AttackMoveForQuad( saberMoveData[curmove].endQuad );
				}
				*/
				if ( newmove != LS_NONE )
				{
					//Now get the proper transition move
					newmove = PM_SaberAnimTransitionAnim( curmove, newmove );
					// NOTENOTE Had to remove this concept since there is no gent in pmove.
					/*
					if ( PM_HasAnimation( pm->gent, saberMoveData[newmove].animToUse ) )
					*/

					assert(	bgGlobalAnimations[saberMoveData[newmove].animToUse].firstFrame != 0 || 
							bgGlobalAnimations[saberMoveData[newmove].animToUse].numFrames != 0);

					if (1)
					{
						anim = saberMoveData[newmove].animToUse;
					}
				}
			}

			if (anim == -1)
			{//not side-stepping, pick neutral anim
				// Add randomness for prototype?
				newmove = saberMoveData[curmove].chain_attack;

				anim= saberMoveData[newmove].animToUse;

				if ( !pm->cmd.forwardmove && !pm->cmd.rightmove && pm->cmd.upmove >= 0 && pm->ps->groundEntityNum != ENTITYNUM_NONE )
				{//not moving at all, so set the anim on entire body
					both = qtrue;
				}
			
			}

			if ( anim == -1)
			{
				if((pm->ps->legsAnim & ~ANIM_TOGGLEBIT) == BOTH_WALK1 )
				{
					anim = BOTH_WALK1;
				}
				else if((pm->ps->legsAnim & ~ANIM_TOGGLEBIT) == BOTH_RUN2 )
				{
					anim = BOTH_RUN2;
				}
				else if((pm->ps->legsAnim & ~ANIM_TOGGLEBIT) == BOTH_WALK2 )
				{
					anim = BOTH_WALK2;
				}
				else
				{
					anim = PM_GetSaberStance();
				}
				newmove = LS_READY;
			}

			if ( !pm->ps->saberActive )
			{//turn on the saber if it's not on
				pm->ps->saberActive = qtrue;
			}

			PM_SetSaberMove( newmove );

			if ( both )
			{
				PM_SetAnim(SETANIM_LEGS,anim,SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD, 100);
			}

			//don't fire again until anim is done
			pm->ps->weaponTime = pm->ps->torsoTimer;
		}
	}

	// *********************************************************
	// WEAPON_FIRING
	// *********************************************************

	pm->ps->weaponstate = WEAPON_FIRING;

	amount = weaponData[pm->ps->weapon].energyPerShot;

	addTime = pm->ps->weaponTime;

	pm->ps->saberAttackSequence = pm->ps->torsoAnim;
	if ( !addTime )
	{
		addTime = weaponData[pm->ps->weapon].fireTime;
	}
	pm->ps->weaponTime = addTime;
}

void PM_SetSaberMove(short newMove)
{
	unsigned int setflags = saberMoveData[newMove].animSetFlags;
	int	anim = saberMoveData[newMove].animToUse;
	int parts = SETANIM_TORSO;
	
	if ( newMove == LS_READY )
	{//finished with a kata, reset attack counter
		pm->ps->saberAttackChainCount = 0;
	}
	else if ( BG_SaberInAttack( newMove ) )
	{//continuing with a kata, increment attack counter
		pm->ps->saberAttackChainCount++;
	}

	if (pm->ps->saberAttackChainCount > 16)
	{ //for the sake of being able to send the value over the net within a reasonable bit count
		pm->ps->saberAttackChainCount = 16;
	}

	if ( pm->ps->fd.saberAnimLevel > FORCE_LEVEL_1 &&
		 !BG_SaberInIdle( newMove ) && !PM_SaberInParry( newMove ) && !PM_SaberInReflect( newMove ) && !BG_SaberInSpecial(newMove))
	{//readies, parries and reflections have only 1 level 
		//increment the anim to the next level of saber anims
		if ( !PM_SaberInTransition( newMove ) )
		{//FIXME: only have level 1 transitions for now
			anim += (pm->ps->fd.saberAnimLevel-FORCE_LEVEL_1) * SABER_ANIM_GROUP_SIZE;
		}
	}

	// If the move does the same animation as the last one, we need to force a restart...
	if ( saberMoveData[pm->ps->saberMove].animToUse == anim && newMove > LS_PUTAWAY)
	{
		setflags |= SETANIM_FLAG_RESTART;
	}

	//saber torso anims should always be highest priority
	setflags |= SETANIM_FLAG_OVERRIDE;

	if ( BG_InSaberStandAnim(anim) || anim == BOTH_STAND1 )
	{
		anim = (pm->ps->legsAnim & ~ANIM_TOGGLEBIT);

		if ((anim >= BOTH_STAND1 && anim <= BOTH_STAND4TOATTACK2) ||
			(anim >= TORSO_DROPWEAP1 && anim <= TORSO_WEAPONIDLE12))
		{ //If standing then use the special saber stand anim
			anim = PM_GetSaberStance();
		}

		if (pm->ps->pm_flags & PMF_DUCKED)
		{ //Playing torso walk anims while crouched makes you look like a monkey
			anim = PM_GetSaberStance();
		}

		if (anim == BOTH_WALKBACK1 || anim == BOTH_WALKBACK2)
		{ //normal stance when walking backward so saber doesn't look like it's cutting through leg
			anim = PM_GetSaberStance();
		}

		parts = SETANIM_TORSO;
	}

	if ( newMove == LS_A_LUNGE 
		|| newMove == LS_A_JUMP_T__B_ 
		|| newMove == LS_A_BACKSTAB
		|| newMove == LS_A_BACK
		|| newMove == LS_A_BACK_CR
		|| newMove == LS_A_FLIP_STAB
		|| newMove == LS_A_FLIP_SLASH )
	{
		parts = SETANIM_BOTH;
	}
	else if ( BG_SpinningSaberAnim( anim ) )
	{//spins must be played on entire body
		parts = SETANIM_BOTH;
	}
	PM_SetAnim(parts, anim, setflags|SETANIM_FLAG_HOLD, saberMoveData[newMove].blendTime);

	if ( (pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == anim )
	{//successfully changed anims
	//special check for *starting* a saber swing
		//playing at attack
		if ( BG_SaberInAttack( newMove ) || BG_SaberInSpecialAttack( anim ) )
		{
			if ( pm->ps->saberMove != newMove )
			{//wasn't playing that attack before
				PM_AddEvent(EV_SABER_ATTACK);
			}
		}

		pm->ps->saberMove = newMove;
		pm->ps->saberBlocking = saberMoveData[newMove].blocking;

		pm->ps->torsoAnim = anim;//saberMoveData[newMove].animToUse;

		if (pm->ps->weaponTime <= 0)
		{
			pm->ps->saberBlocked = BLOCKED_NONE;
		}
	}
}

