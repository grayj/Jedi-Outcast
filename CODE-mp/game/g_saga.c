#include "g_local.h"
#include "bg_saga.h"

char		saga_info[MAX_SAGA_INFO_SIZE];
int			saga_valid;
int			saga_round_over;

int			imperial_goals_required;
int			imperial_goals_completed;
int			rebel_goals_required;
int			rebel_goals_completed;

int			rebel_attackers;
int			imperial_attackers;

void LogExit( const char *string );

int GetValueGroup(char *buf, char *group, char *outbuf);
int GetPairedValue(char *buf, char *key, char *outbuf);
//Using the bot parsing functions on saga ini's, at least for now.

void InitSagaMode(void)
{
	vmCvar_t		mapname;
	char			levelname[512];
	char			goalreq[64];
	char			objectives[MAX_SAGA_INFO_SIZE];
	int				len = 0;
	fileHandle_t	f;

	if (g_gametype.integer != GT_SAGA)
	{
		goto failure;
	}

	imperial_goals_completed = 0;
	rebel_goals_completed = 0;

	saga_round_over = 0;

	trap_Cvar_Register( &mapname, "mapname", "", CVAR_SERVERINFO | CVAR_ROM );

	Com_sprintf(levelname, sizeof(levelname), "maps/%s.saga\0", mapname.string);

	if (!levelname || !levelname[0])
	{
		goto failure;
	}

	len = trap_FS_FOpenFile(levelname, &f, FS_READ);

	if (!f || len >= MAX_SAGA_INFO_SIZE)
	{
		goto failure;
	}

	trap_FS_Read(saga_info, len, f);

	saga_valid = 1;

	if (GetValueGroup(saga_info, "Rebel", objectives))
	{
		if (GetPairedValue(objectives, "RequiredObjectives", goalreq))
		{
			rebel_goals_required = atoi(goalreq);
		}
		if (GetPairedValue(objectives, "attackers", goalreq))
		{
			rebel_attackers = atoi(goalreq);
		}
	}
	if (GetValueGroup(saga_info, "Imperial", objectives))
	{
		if (GetPairedValue(objectives, "RequiredObjectives", goalreq))
		{
			imperial_goals_required = atoi(goalreq);
		}
		if (GetPairedValue(objectives, "attackers", goalreq))
		{
			imperial_attackers = atoi(goalreq);
		}
	}

	return;

failure:
	saga_valid = 0;
}

void UseSagaTarget(gentity_t *other, gentity_t *en, char *target)
{ //actually use the player which triggered the object which triggered the saga objective to trigger the target
	gentity_t		*t;
	gentity_t		*ent;

	if ( !en || !en->client )
	{ //looks like we don't have access to a player, so just use the activating entity
		ent = other;
	}
	else
	{
		ent = en;
	}

	if (!en)
	{
		return;
	}

	if ( !target )
	{
		return;
	}

	t = NULL;
	while ( (t = G_Find (t, FOFS(targetname), target)) != NULL )
	{
		if ( t == ent )
		{
			G_Printf ("WARNING: Entity used itself.\n");
		}
		else
		{
			if ( t->use )
			{
				t->use (t, ent, ent);
			}
		}
		if ( !ent->inuse )
		{
			G_Printf("entity was removed while using targets\n");
			return;
		}
	}
}

void SagaBroadcast_OBJECTIVECOMPLETE(int team, int client, int objective)
{
	gentity_t *te;
	vec3_t nomatter;

	VectorClear(nomatter);

	te = G_TempEntity( nomatter, EV_SAGA_OBJECTIVECOMPLETE );
	te->r.svFlags |= SVF_BROADCAST;
	te->s.eventParm = team;
	te->s.weapon = client;
	te->s.trickedentindex = objective;
}

void SagaBroadcast_ROUNDOVER(int winningteam, int winningclient)
{
	gentity_t *te;
	vec3_t nomatter;

	VectorClear(nomatter);

	te = G_TempEntity( nomatter, EV_SAGA_ROUNDOVER );
	te->r.svFlags |= SVF_BROADCAST;
	te->s.eventParm = winningteam;
	te->s.weapon = winningclient;
}

void BroadcastObjectiveCompletion(int team, int objective, int final, int client)
{
	if (client != ENTITYNUM_NONE && g_entities[client].client && g_entities[client].client->sess.sessionTeam == team)
	{ //guy who completed this objective gets points, providing he's on the opposing team
		AddScore(&g_entities[client], g_entities[client].client->ps.origin, SAGA_POINTS_OBJECTIVECOMPLETED);
	}
	
	SagaBroadcast_OBJECTIVECOMPLETE(team, client, objective);
	//G_Printf("Broadcast goal completion team %i objective %i final %i\n", team, objective, final);
}

void AddSagaWinningTeamPoints(int team, int winner)
{
	int i = 0;
	gentity_t *ent;

	while (i < MAX_CLIENTS)
	{
		ent = &g_entities[i];

		if (ent && ent->client && ent->client->sess.sessionTeam == team)
		{
			if (i == winner)
			{
				AddScore(ent, ent->client->ps.origin, SAGA_POINTS_TEAMWONROUND+SAGA_POINTS_FINALOBJECTIVECOMPLETED);
			}
			else
			{
				AddScore(ent, ent->client->ps.origin, SAGA_POINTS_TEAMWONROUND);
			}
		}

		i++;
	}
}

void SagaRoundComplete(int winningteam, int winningclient)
{
	vec3_t nomatter;

	saga_round_over = 1;

	//G_Printf("Team %i won\n", winningteam);

	if (winningclient != ENTITYNUM_NONE && g_entities[winningclient].client &&
		g_entities[winningclient].client->sess.sessionTeam != winningteam)
	{ //this person just won the round for the other team..
		winningclient = ENTITYNUM_NONE;
	}

	VectorClear(nomatter);

	SagaBroadcast_ROUNDOVER(winningteam, winningclient);

	AddSagaWinningTeamPoints(winningteam, winningclient);

	if (winningteam == SAGATEAM_IMPERIAL)
	{
		LogExit( "The Imperials completed their final objective." );
	}
	else
	{
		LogExit( "The Rebels completed their final objective." );
	}
}

void SagaObjectiveCompleted(int team, int objective, int final, int client)
{
	int goals_completed, goals_required;

	if (saga_round_over)
	{
		return;
	}

	if (final != -1)
	{
		if (team == SAGATEAM_IMPERIAL)
		{
			imperial_goals_completed++;
		}
		else
		{
			rebel_goals_completed++;
		}
	}

	if (team == SAGATEAM_IMPERIAL)
	{
		goals_completed = imperial_goals_completed;
		goals_required = imperial_goals_required;
	}
	else
	{
		goals_completed = rebel_goals_completed;
		goals_required = rebel_goals_required;
	}

	if (final == 1 || goals_completed >= goals_required)
	{
		SagaRoundComplete(team, client);
	}
	else
	{
		BroadcastObjectiveCompletion(team, objective, final, client);
	}
}

void StripTabs(char *buf)
{
	int i = 0;
	int i_r = 0;

	while (buf[i])
	{
		if (buf[i] != 9)
		{
			buf[i_r] = buf[i];
			i_r++;
		}

		i++;
	}
	buf[i_r] = '\0';
}

void sagaTriggerUse(gentity_t *ent, gentity_t *other, gentity_t *activator)
{
	char			teamstr[64];
	char			objectivestr[64];
	char			objectives[MAX_SAGA_INFO_SIZE];
	char			desiredobjective[MAX_SAGA_INFO_SIZE];
	int				clUser = ENTITYNUM_NONE;
	int				final = 0;
	int				i = 0;

	if (!saga_valid)
	{
		return;
	}

	if (activator && activator->client)
	{ //activator will hopefully be the person who triggered this event
		clUser = activator->s.number;
	}

	if (ent->side == SAGATEAM_IMPERIAL)
	{
		Com_sprintf(teamstr, sizeof(teamstr), "Imperial");
	}
	else
	{
		Com_sprintf(teamstr, sizeof(teamstr), "Rebel");
	}

	if (GetValueGroup(saga_info, teamstr, objectives))
	{
		Com_sprintf(objectivestr, sizeof(objectivestr), "Objective%i", ent->objective);

		StripTabs(objectives); //tabs confuse the parsing function

		if (GetValueGroup(objectives, objectivestr, desiredobjective))
		{
			if (GetPairedValue(desiredobjective, "final", teamstr))
			{
				final = atoi(teamstr);
			}

			if (GetPairedValue(desiredobjective, "target", teamstr))
			{
				while (teamstr[i])
				{
					if (teamstr[i] == '\r' ||
						teamstr[i] == '\n')
					{
						teamstr[i] = '\0';
					}

					i++;
				}
				UseSagaTarget(other, activator, teamstr);
			}

			if (ent->target && ent->target[0])
			{ //use this too
				UseSagaTarget(other, activator, ent->target);
			}

			SagaObjectiveCompleted(ent->side, ent->objective, final, clUser);
		}
	}
}


/*QUAKED info_saga_objective (1 0 1) (-16 -16 -24) (16 16 32)
"objective" - specifies the objective to complete upon activation
"side" - set to 1 to specify an imperial goal, 2 to specify rebels
*/
void SP_info_saga_objective (gentity_t *ent)
{
	if (!saga_valid || g_gametype.integer != GT_SAGA)
	{
		G_FreeEntity(ent);
		return;
	}

	ent->use = sagaTriggerUse;
	G_SpawnInt( "objective", "0", &ent->objective);
	G_SpawnInt( "side", "0", &ent->side);

	if (!ent->objective || !ent->side)
	{ //j00 fux0red something up
		G_FreeEntity(ent);
		G_Printf("ERROR: info_saga_objective without an objective or side value\n");
		return;
	}
}

