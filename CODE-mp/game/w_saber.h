#define	FORCE_POWER_MAX	100
#define MAX_GRIP_DISTANCE 256
#define MAX_TRICK_DISTANCE 512
#define FORCE_JUMP_CHARGE_TIME 6400//3000.0f
#define GRIP_DRAIN_AMOUNT 30
#define FORCE_LIGHTNING_RADIUS 300
#define MAX_DRAIN_DISTANCE 512

typedef enum
{
	FJ_FORWARD,
	FJ_BACKWARD,
	FJ_RIGHT,
	FJ_LEFT,
	FJ_UP
};

extern vmCvar_t g_MaxHolocronCarry;
extern int forcePowerNeeded[NUM_FORCE_POWER_LEVELS][NUM_FORCE_POWERS];
extern float forceJumpHeight[NUM_FORCE_POWER_LEVELS];
extern float forceJumpStrength[NUM_FORCE_POWER_LEVELS];

