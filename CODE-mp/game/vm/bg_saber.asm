export PM_irand_timesync
code
proc PM_irand_timesync 8 4
file "../bg_saber.c"
line 6
;1:#include "q_shared.h"
;2:#include "bg_public.h"
;3:#include "bg_local.h"
;4:
;5:int PM_irand_timesync(int val1, int val2)
;6:{
line 9
;7:	int i;
;8:
;9:	i = (val1-1) + (Q_random( &pm->cmd.serverTime )*(val2 - val1)) + 1;
ADDRGP4 pm
INDIRP4
CNSTI4 4
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 Q_random
CALLF4
ASGNF4
ADDRLP4 0
ADDRFP4 0
INDIRI4
CNSTI4 1
SUBI4
CVIF4 4
ADDRLP4 4
INDIRF4
ADDRFP4 4
INDIRI4
ADDRFP4 0
INDIRI4
SUBI4
CVIF4 4
MULF4
ADDF4
CNSTF4 1065353216
ADDF4
CVFI4 4
ASGNI4
line 10
;10:	if (i < val1)
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRI4
GEI4 $66
line 11
;11:	{
line 12
;12:		i = val1;
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
line 13
;13:	}
LABELV $66
line 14
;14:	if (i > val2)
ADDRLP4 0
INDIRI4
ADDRFP4 4
INDIRI4
LEI4 $68
line 15
;15:	{
line 16
;16:		i = val2;
ADDRLP4 0
ADDRFP4 4
INDIRI4
ASGNI4
line 17
;17:	}
LABELV $68
line 19
;18:
;19:	return i;
ADDRLP4 0
INDIRI4
RETI4
LABELV $65
endproc PM_irand_timesync 8 4
export BG_ForcePowerDrain
proc BG_ForcePowerDrain 12 0
line 23
;20:}
;21:
;22:void BG_ForcePowerDrain( playerState_t *ps, forcePowers_t forcePower, int overrideAmt )
;23:{
line 25
;24:	//take away the power
;25:	int	drain = overrideAmt;
ADDRLP4 0
ADDRFP4 8
INDIRI4
ASGNI4
line 34
;26:
;27:	/*
;28:	if (ps->powerups[PW_FORCE_BOON])
;29:	{
;30:		return;
;31:	}
;32:	*/
;33:
;34:	if ( !drain )
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $71
line 35
;35:	{
line 36
;36:		drain = forcePowerNeeded[ps->fd.forcePowerLevel[forcePower]][forcePower];
ADDRLP4 4
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 0
ADDRLP4 4
INDIRI4
CNSTI4 72
ADDRLP4 4
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 932
ADDP4
ADDP4
INDIRI4
MULI4
ADDRGP4 forcePowerNeeded
ADDP4
ADDP4
INDIRI4
ASGNI4
line 37
;37:	}
LABELV $71
line 38
;38:	if ( !drain )
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $73
line 39
;39:	{
line 40
;40:		return;
ADDRGP4 $70
JUMPV
LABELV $73
line 43
;41:	}
;42:
;43:	if (forcePower == FP_LEVITATION)
ADDRFP4 4
INDIRI4
CNSTI4 1
NEI4 $75
line 44
;44:	{ //special case
line 45
;45:		int jumpDrain = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 47
;46:
;47:		if (ps->velocity[2] > 250)
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
CNSTF4 1132068864
LEF4 $77
line 48
;48:		{
line 49
;49:			jumpDrain = 20;
ADDRLP4 4
CNSTI4 20
ASGNI4
line 50
;50:		}
ADDRGP4 $78
JUMPV
LABELV $77
line 51
;51:		else if (ps->velocity[2] > 200)
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
CNSTF4 1128792064
LEF4 $79
line 52
;52:		{
line 53
;53:			jumpDrain = 16;
ADDRLP4 4
CNSTI4 16
ASGNI4
line 54
;54:		}
ADDRGP4 $80
JUMPV
LABELV $79
line 55
;55:		else if (ps->velocity[2] > 150)
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
CNSTF4 1125515264
LEF4 $81
line 56
;56:		{
line 57
;57:			jumpDrain = 12;
ADDRLP4 4
CNSTI4 12
ASGNI4
line 58
;58:		}
ADDRGP4 $82
JUMPV
LABELV $81
line 59
;59:		else if (ps->velocity[2] > 100)
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
CNSTF4 1120403456
LEF4 $83
line 60
;60:		{
line 61
;61:			jumpDrain = 8;
ADDRLP4 4
CNSTI4 8
ASGNI4
line 62
;62:		}
ADDRGP4 $84
JUMPV
LABELV $83
line 63
;63:		else if (ps->velocity[2] > 50)
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
CNSTF4 1112014848
LEF4 $85
line 64
;64:		{
line 65
;65:			jumpDrain = 6;
ADDRLP4 4
CNSTI4 6
ASGNI4
line 66
;66:		}
ADDRGP4 $86
JUMPV
LABELV $85
line 67
;67:		else if (ps->velocity[2] > 0)
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
CNSTF4 0
LEF4 $87
line 68
;68:		{
line 69
;69:			jumpDrain = 4;
ADDRLP4 4
CNSTI4 4
ASGNI4
line 70
;70:		}
LABELV $87
LABELV $86
LABELV $84
LABELV $82
LABELV $80
LABELV $78
line 72
;71:
;72:		if (jumpDrain)
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $89
line 73
;73:		{
line 74
;74:			jumpDrain /= ps->fd.forcePowerLevel[FP_LEVITATION];
ADDRLP4 4
ADDRLP4 4
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 936
ADDP4
INDIRI4
DIVI4
ASGNI4
line 75
;75:		}
LABELV $89
line 77
;76:
;77:		ps->fd.forcePower -= jumpDrain;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 920
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
SUBI4
ASGNI4
line 78
;78:		if ( ps->fd.forcePower < 0 )
ADDRFP4 0
INDIRP4
CNSTI4 920
ADDP4
INDIRI4
CNSTI4 0
GEI4 $70
line 79
;79:		{
line 80
;80:			ps->fd.forcePower = 0;
ADDRFP4 0
INDIRP4
CNSTI4 920
ADDP4
CNSTI4 0
ASGNI4
line 81
;81:		}
line 83
;82:
;83:		return;
ADDRGP4 $70
JUMPV
LABELV $75
line 86
;84:	}
;85:
;86:	ps->fd.forcePower -= drain;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 920
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
ADDRLP4 0
INDIRI4
SUBI4
ASGNI4
line 87
;87:	if ( ps->fd.forcePower < 0 )
ADDRFP4 0
INDIRP4
CNSTI4 920
ADDP4
INDIRI4
CNSTI4 0
GEI4 $93
line 88
;88:	{
line 89
;89:		ps->fd.forcePower = 0;
ADDRFP4 0
INDIRP4
CNSTI4 920
ADDP4
CNSTI4 0
ASGNI4
line 90
;90:	}
LABELV $93
line 91
;91:}
LABELV $70
endproc BG_ForcePowerDrain 12 0
data
export saberMoveData
align 4
LABELV saberMoveData
address $95
byte 4 550
byte 4 1
byte 4 1
byte 4 0
byte 4 350
byte 4 0
byte 4 0
byte 4 0
byte 4 0
address $96
byte 4 553
byte 4 1
byte 4 1
byte 4 0
byte 4 350
byte 4 2
byte 4 1
byte 4 22
byte 4 0
address $97
byte 4 566
byte 4 1
byte 4 1
byte 4 2
byte 4 350
byte 4 0
byte 4 1
byte 4 22
byte 4 0
address $98
byte 4 567
byte 4 1
byte 4 1
byte 4 2
byte 4 350
byte 4 0
byte 4 1
byte 4 22
byte 4 0
address $99
byte 4 129
byte 4 4
byte 4 0
byte 4 10
byte 4 100
byte 4 1
byte 4 25
byte 4 25
byte 4 200
address $100
byte 4 127
byte 4 5
byte 4 1
byte 4 10
byte 4 100
byte 4 1
byte 4 26
byte 4 26
byte 4 200
address $101
byte 4 131
byte 4 6
byte 4 2
byte 4 10
byte 4 50
byte 4 1
byte 4 27
byte 4 27
byte 4 200
address $102
byte 4 130
byte 4 0
byte 4 4
byte 4 10
byte 4 100
byte 4 1
byte 4 28
byte 4 28
byte 4 200
address $103
byte 4 128
byte 4 1
byte 4 5
byte 4 10
byte 4 100
byte 4 1
byte 4 29
byte 4 29
byte 4 200
address $104
byte 4 132
byte 4 2
byte 4 6
byte 4 10
byte 4 100
byte 4 1
byte 4 30
byte 4 30
byte 4 200
address $105
byte 4 126
byte 4 3
byte 4 7
byte 4 10
byte 4 100
byte 4 1
byte 4 31
byte 4 31
byte 4 200
address $106
byte 4 803
byte 4 1
byte 4 1
byte 4 10
byte 4 100
byte 4 1
byte 4 1
byte 4 1
byte 4 200
address $107
byte 4 804
byte 4 1
byte 4 1
byte 4 10
byte 4 100
byte 4 1
byte 4 1
byte 4 1
byte 4 200
address $108
byte 4 809
byte 4 1
byte 4 1
byte 4 10
byte 4 100
byte 4 1
byte 4 1
byte 4 1
byte 4 200
address $109
byte 4 808
byte 4 7
byte 4 3
byte 4 10
byte 4 100
byte 4 1
byte 4 1
byte 4 1
byte 4 200
address $110
byte 4 807
byte 4 3
byte 4 7
byte 4 10
byte 4 100
byte 4 1
byte 4 1
byte 4 1
byte 4 200
address $111
byte 4 806
byte 4 1
byte 4 3
byte 4 10
byte 4 100
byte 4 1
byte 4 1
byte 4 51
byte 4 200
address $112
byte 4 805
byte 4 5
byte 4 1
byte 4 10
byte 4 100
byte 4 1
byte 4 1
byte 4 40
byte 4 200
address $113
byte 4 178
byte 4 1
byte 4 4
byte 4 10
byte 4 100
byte 4 1
byte 4 4
byte 4 4
byte 4 200
address $114
byte 4 176
byte 4 1
byte 4 5
byte 4 10
byte 4 100
byte 4 1
byte 4 5
byte 4 5
byte 4 200
address $115
byte 4 180
byte 4 1
byte 4 6
byte 4 10
byte 4 100
byte 4 1
byte 4 6
byte 4 6
byte 4 200
address $116
byte 4 179
byte 4 1
byte 4 0
byte 4 10
byte 4 100
byte 4 1
byte 4 7
byte 4 7
byte 4 200
address $117
byte 4 177
byte 4 1
byte 4 1
byte 4 10
byte 4 100
byte 4 1
byte 4 8
byte 4 8
byte 4 200
address $118
byte 4 181
byte 4 1
byte 4 2
byte 4 10
byte 4 100
byte 4 1
byte 4 9
byte 4 9
byte 4 200
address $119
byte 4 175
byte 4 1
byte 4 3
byte 4 10
byte 4 100
byte 4 1
byte 4 10
byte 4 10
byte 4 200
address $120
byte 4 186
byte 4 0
byte 4 1
byte 4 2
byte 4 100
byte 4 1
byte 4 1
byte 4 1
byte 4 200
address $121
byte 4 184
byte 4 1
byte 4 1
byte 4 2
byte 4 100
byte 4 1
byte 4 1
byte 4 1
byte 4 200
address $122
byte 4 188
byte 4 2
byte 4 1
byte 4 2
byte 4 100
byte 4 1
byte 4 1
byte 4 1
byte 4 200
address $123
byte 4 185
byte 4 4
byte 4 1
byte 4 2
byte 4 100
byte 4 1
byte 4 1
byte 4 1
byte 4 200
address $124
byte 4 183
byte 4 5
byte 4 1
byte 4 2
byte 4 100
byte 4 1
byte 4 1
byte 4 1
byte 4 200
address $125
byte 4 187
byte 4 6
byte 4 1
byte 4 2
byte 4 100
byte 4 1
byte 4 1
byte 4 1
byte 4 200
address $126
byte 4 182
byte 4 7
byte 4 1
byte 4 2
byte 4 100
byte 4 1
byte 4 1
byte 4 1
byte 4 200
address $127
byte 4 133
byte 4 0
byte 4 1
byte 4 10
byte 4 100
byte 4 0
byte 4 26
byte 4 8
byte 4 150
address $128
byte 4 160
byte 4 0
byte 4 2
byte 4 10
byte 4 100
byte 4 0
byte 4 27
byte 4 9
byte 4 150
address $129
byte 4 161
byte 4 0
byte 4 3
byte 4 10
byte 4 100
byte 4 0
byte 4 27
byte 4 10
byte 4 150
address $130
byte 4 134
byte 4 0
byte 4 4
byte 4 10
byte 4 100
byte 4 0
byte 4 28
byte 4 4
byte 4 150
address $131
byte 4 135
byte 4 0
byte 4 5
byte 4 10
byte 4 100
byte 4 0
byte 4 29
byte 4 5
byte 4 150
address $132
byte 4 136
byte 4 0
byte 4 6
byte 4 10
byte 4 100
byte 4 0
byte 4 30
byte 4 6
byte 4 150
address $133
byte 4 162
byte 4 1
byte 4 0
byte 4 10
byte 4 100
byte 4 0
byte 4 25
byte 4 7
byte 4 150
address $134
byte 4 137
byte 4 1
byte 4 2
byte 4 10
byte 4 100
byte 4 0
byte 4 27
byte 4 9
byte 4 150
address $135
byte 4 163
byte 4 1
byte 4 3
byte 4 10
byte 4 100
byte 4 0
byte 4 27
byte 4 10
byte 4 150
address $136
byte 4 138
byte 4 1
byte 4 4
byte 4 10
byte 4 100
byte 4 0
byte 4 28
byte 4 4
byte 4 150
address $137
byte 4 139
byte 4 1
byte 4 5
byte 4 10
byte 4 100
byte 4 0
byte 4 29
byte 4 5
byte 4 150
address $138
byte 4 140
byte 4 1
byte 4 6
byte 4 10
byte 4 100
byte 4 0
byte 4 30
byte 4 6
byte 4 150
address $139
byte 4 141
byte 4 2
byte 4 0
byte 4 10
byte 4 100
byte 4 0
byte 4 25
byte 4 7
byte 4 150
address $140
byte 4 164
byte 4 2
byte 4 1
byte 4 10
byte 4 100
byte 4 0
byte 4 26
byte 4 8
byte 4 150
address $141
byte 4 165
byte 4 2
byte 4 3
byte 4 10
byte 4 100
byte 4 0
byte 4 27
byte 4 10
byte 4 150
address $142
byte 4 142
byte 4 2
byte 4 4
byte 4 10
byte 4 100
byte 4 0
byte 4 28
byte 4 4
byte 4 150
address $143
byte 4 143
byte 4 2
byte 4 5
byte 4 10
byte 4 100
byte 4 0
byte 4 29
byte 4 5
byte 4 150
address $144
byte 4 144
byte 4 2
byte 4 6
byte 4 10
byte 4 100
byte 4 0
byte 4 30
byte 4 6
byte 4 150
address $145
byte 4 145
byte 4 3
byte 4 0
byte 4 10
byte 4 100
byte 4 0
byte 4 25
byte 4 7
byte 4 150
address $146
byte 4 146
byte 4 3
byte 4 1
byte 4 10
byte 4 100
byte 4 0
byte 4 26
byte 4 8
byte 4 150
address $147
byte 4 147
byte 4 3
byte 4 2
byte 4 10
byte 4 100
byte 4 0
byte 4 27
byte 4 9
byte 4 150
address $148
byte 4 148
byte 4 3
byte 4 4
byte 4 10
byte 4 100
byte 4 0
byte 4 28
byte 4 4
byte 4 150
address $149
byte 4 149
byte 4 3
byte 4 5
byte 4 10
byte 4 100
byte 4 0
byte 4 29
byte 4 5
byte 4 150
address $150
byte 4 150
byte 4 3
byte 4 6
byte 4 10
byte 4 100
byte 4 0
byte 4 30
byte 4 6
byte 4 150
address $151
byte 4 151
byte 4 4
byte 4 0
byte 4 10
byte 4 100
byte 4 0
byte 4 25
byte 4 7
byte 4 150
address $152
byte 4 166
byte 4 4
byte 4 1
byte 4 10
byte 4 100
byte 4 0
byte 4 26
byte 4 8
byte 4 150
address $153
byte 4 167
byte 4 4
byte 4 2
byte 4 10
byte 4 100
byte 4 0
byte 4 27
byte 4 9
byte 4 150
address $154
byte 4 168
byte 4 4
byte 4 3
byte 4 10
byte 4 100
byte 4 0
byte 4 27
byte 4 10
byte 4 150
address $155
byte 4 169
byte 4 4
byte 4 5
byte 4 10
byte 4 100
byte 4 0
byte 4 29
byte 4 5
byte 4 150
address $156
byte 4 152
byte 4 4
byte 4 6
byte 4 10
byte 4 100
byte 4 0
byte 4 30
byte 4 6
byte 4 150
address $157
byte 4 153
byte 4 5
byte 4 0
byte 4 10
byte 4 100
byte 4 0
byte 4 25
byte 4 7
byte 4 150
address $158
byte 4 154
byte 4 5
byte 4 1
byte 4 10
byte 4 100
byte 4 0
byte 4 26
byte 4 8
byte 4 150
address $159
byte 4 170
byte 4 5
byte 4 2
byte 4 10
byte 4 100
byte 4 0
byte 4 27
byte 4 9
byte 4 150
address $160
byte 4 171
byte 4 5
byte 4 3
byte 4 10
byte 4 100
byte 4 0
byte 4 27
byte 4 10
byte 4 150
address $161
byte 4 155
byte 4 5
byte 4 4
byte 4 10
byte 4 100
byte 4 0
byte 4 28
byte 4 4
byte 4 150
address $162
byte 4 172
byte 4 5
byte 4 6
byte 4 10
byte 4 100
byte 4 0
byte 4 30
byte 4 6
byte 4 150
address $163
byte 4 156
byte 4 6
byte 4 0
byte 4 10
byte 4 100
byte 4 0
byte 4 25
byte 4 7
byte 4 150
address $164
byte 4 157
byte 4 6
byte 4 1
byte 4 10
byte 4 100
byte 4 0
byte 4 26
byte 4 8
byte 4 150
address $165
byte 4 158
byte 4 6
byte 4 2
byte 4 10
byte 4 100
byte 4 0
byte 4 27
byte 4 9
byte 4 150
address $166
byte 4 173
byte 4 6
byte 4 3
byte 4 10
byte 4 100
byte 4 0
byte 4 27
byte 4 10
byte 4 150
address $167
byte 4 174
byte 4 6
byte 4 4
byte 4 10
byte 4 100
byte 4 0
byte 4 28
byte 4 4
byte 4 150
address $168
byte 4 159
byte 4 6
byte 4 5
byte 4 10
byte 4 100
byte 4 0
byte 4 29
byte 4 5
byte 4 150
address $169
byte 4 189
byte 4 0
byte 4 0
byte 4 10
byte 4 100
byte 4 0
byte 4 25
byte 4 33
byte 4 150
address $170
byte 4 190
byte 4 1
byte 4 1
byte 4 10
byte 4 100
byte 4 0
byte 4 26
byte 4 42
byte 4 150
address $171
byte 4 191
byte 4 2
byte 4 2
byte 4 10
byte 4 100
byte 4 0
byte 4 27
byte 4 47
byte 4 150
address $172
byte 4 192
byte 4 3
byte 4 3
byte 4 10
byte 4 100
byte 4 0
byte 4 27
byte 4 55
byte 4 150
address $173
byte 4 193
byte 4 4
byte 4 4
byte 4 10
byte 4 100
byte 4 0
byte 4 28
byte 4 58
byte 4 150
address $174
byte 4 194
byte 4 5
byte 4 5
byte 4 10
byte 4 100
byte 4 0
byte 4 29
byte 4 63
byte 4 150
address $175
byte 4 195
byte 4 6
byte 4 6
byte 4 10
byte 4 100
byte 4 0
byte 4 30
byte 4 70
byte 4 150
address $176
byte 4 196
byte 4 0
byte 4 0
byte 4 10
byte 4 100
byte 4 0
byte 4 25
byte 4 33
byte 4 150
address $177
byte 4 197
byte 4 1
byte 4 1
byte 4 10
byte 4 100
byte 4 0
byte 4 26
byte 4 42
byte 4 150
address $178
byte 4 198
byte 4 2
byte 4 2
byte 4 10
byte 4 100
byte 4 0
byte 4 27
byte 4 47
byte 4 150
address $179
byte 4 192
byte 4 3
byte 4 3
byte 4 10
byte 4 100
byte 4 0
byte 4 27
byte 4 55
byte 4 150
address $180
byte 4 199
byte 4 4
byte 4 4
byte 4 10
byte 4 100
byte 4 0
byte 4 28
byte 4 58
byte 4 150
address $181
byte 4 200
byte 4 5
byte 4 5
byte 4 10
byte 4 100
byte 4 0
byte 4 29
byte 4 63
byte 4 150
address $182
byte 4 201
byte 4 6
byte 4 6
byte 4 10
byte 4 100
byte 4 0
byte 4 30
byte 4 70
byte 4 150
address $183
byte 4 202
byte 4 7
byte 4 7
byte 4 10
byte 4 100
byte 4 0
byte 4 27
byte 4 55
byte 4 150
address $184
byte 4 522
byte 4 0
byte 4 0
byte 4 10
byte 4 100
byte 4 0
byte 4 1
byte 4 1
byte 4 150
address $185
byte 4 523
byte 4 1
byte 4 1
byte 4 10
byte 4 100
byte 4 0
byte 4 1
byte 4 1
byte 4 150
address $186
byte 4 524
byte 4 2
byte 4 2
byte 4 10
byte 4 100
byte 4 0
byte 4 1
byte 4 1
byte 4 150
address $187
byte 4 525
byte 4 3
byte 4 3
byte 4 10
byte 4 100
byte 4 0
byte 4 1
byte 4 1
byte 4 150
address $188
byte 4 526
byte 4 4
byte 4 4
byte 4 10
byte 4 100
byte 4 0
byte 4 1
byte 4 1
byte 4 150
address $189
byte 4 527
byte 4 5
byte 4 5
byte 4 10
byte 4 100
byte 4 0
byte 4 1
byte 4 1
byte 4 150
address $190
byte 4 528
byte 4 6
byte 4 6
byte 4 10
byte 4 100
byte 4 0
byte 4 1
byte 4 1
byte 4 150
address $191
byte 4 529
byte 4 7
byte 4 7
byte 4 10
byte 4 100
byte 4 0
byte 4 1
byte 4 1
byte 4 150
address $192
byte 4 530
byte 4 3
byte 4 7
byte 4 10
byte 4 50
byte 4 0
byte 4 1
byte 4 1
byte 4 150
address $193
byte 4 531
byte 4 2
byte 4 6
byte 4 10
byte 4 50
byte 4 0
byte 4 1
byte 4 1
byte 4 150
address $194
byte 4 532
byte 4 4
byte 4 0
byte 4 10
byte 4 50
byte 4 0
byte 4 1
byte 4 1
byte 4 150
address $195
byte 4 533
byte 4 6
byte 4 2
byte 4 10
byte 4 50
byte 4 0
byte 4 1
byte 4 1
byte 4 150
address $196
byte 4 534
byte 4 7
byte 4 3
byte 4 10
byte 4 50
byte 4 0
byte 4 1
byte 4 1
byte 4 150
address $197
byte 4 535
byte 4 0
byte 4 4
byte 4 10
byte 4 50
byte 4 0
byte 4 1
byte 4 1
byte 4 150
address $198
byte 4 516
byte 4 1
byte 4 3
byte 4 10
byte 4 50
byte 4 2
byte 4 27
byte 4 50
byte 4 150
address $199
byte 4 517
byte 4 1
byte 4 2
byte 4 10
byte 4 50
byte 4 2
byte 4 27
byte 4 45
byte 4 150
address $200
byte 4 518
byte 4 1
byte 4 4
byte 4 10
byte 4 50
byte 4 2
byte 4 28
byte 4 60
byte 4 150
address $201
byte 4 519
byte 4 1
byte 4 6
byte 4 10
byte 4 50
byte 4 2
byte 4 25
byte 4 72
byte 4 150
address $202
byte 4 521
byte 4 1
byte 4 0
byte 4 10
byte 4 50
byte 4 2
byte 4 30
byte 4 33
byte 4 150
address $203
byte 4 511
byte 4 1
byte 4 3
byte 4 10
byte 4 50
byte 4 2
byte 4 27
byte 4 10
byte 4 150
address $204
byte 4 512
byte 4 1
byte 4 4
byte 4 10
byte 4 50
byte 4 2
byte 4 27
byte 4 9
byte 4 150
address $205
byte 4 513
byte 4 1
byte 4 2
byte 4 10
byte 4 50
byte 4 2
byte 4 28
byte 4 4
byte 4 150
address $206
byte 4 514
byte 4 1
byte 4 0
byte 4 10
byte 4 50
byte 4 2
byte 4 25
byte 4 7
byte 4 150
address $207
byte 4 515
byte 4 1
byte 4 6
byte 4 10
byte 4 50
byte 4 2
byte 4 30
byte 4 6
byte 4 150
address $208
byte 4 511
byte 4 1
byte 4 3
byte 4 10
byte 4 50
byte 4 2
byte 4 27
byte 4 10
byte 4 300
address $209
byte 4 513
byte 4 1
byte 4 2
byte 4 10
byte 4 50
byte 4 2
byte 4 28
byte 4 4
byte 4 300
address $210
byte 4 512
byte 4 1
byte 4 4
byte 4 10
byte 4 50
byte 4 2
byte 4 27
byte 4 9
byte 4 300
address $211
byte 4 515
byte 4 1
byte 4 6
byte 4 10
byte 4 50
byte 4 2
byte 4 30
byte 4 6
byte 4 300
address $212
byte 4 514
byte 4 1
byte 4 0
byte 4 10
byte 4 50
byte 4 2
byte 4 25
byte 4 7
byte 4 300
export transitionMove
align 4
LABELV transitionMove
byte 4 0
byte 4 32
byte 4 33
byte 4 34
byte 4 35
byte 4 36
byte 4 37
byte 4 0
byte 4 38
byte 4 0
byte 4 39
byte 4 40
byte 4 41
byte 4 42
byte 4 43
byte 4 0
byte 4 44
byte 4 45
byte 4 0
byte 4 46
byte 4 47
byte 4 48
byte 4 49
byte 4 0
byte 4 50
byte 4 51
byte 4 52
byte 4 0
byte 4 53
byte 4 54
byte 4 55
byte 4 0
byte 4 56
byte 4 57
byte 4 58
byte 4 59
byte 4 0
byte 4 60
byte 4 61
byte 4 0
byte 4 62
byte 4 63
byte 4 64
byte 4 65
byte 4 66
byte 4 0
byte 4 67
byte 4 0
byte 4 68
byte 4 69
byte 4 70
byte 4 71
byte 4 72
byte 4 73
byte 4 0
byte 4 0
byte 4 68
byte 4 32
byte 4 33
byte 4 34
byte 4 35
byte 4 36
byte 4 37
byte 4 0
export PM_AttackMoveForQuad
code
proc PM_AttackMoveForQuad 4 0
line 322
;92:
;93:// Silly, but I'm replacing these macros so they are shorter!
;94:#define AFLAG_IDLE	(SETANIM_FLAG_NORMAL)
;95:#define AFLAG_ACTIVE (/*SETANIM_FLAG_OVERRIDE | */SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS)
;96:#define AFLAG_WAIT (SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS)
;97:#define AFLAG_FINISH (SETANIM_FLAG_HOLD)
;98:
;99:saberMoveData_t	saberMoveData[LS_MOVE_MAX] = {//							NB:randomized
;100:	// name			anim				startQ	endQ	setanimflag		blend,	blocking	chain_idle		chain_attack	trailLen
;101:	{"None",		BOTH_STAND1,		Q_R,	Q_R,	AFLAG_IDLE,		350,	BLK_NO,		LS_NONE,		LS_NONE,		0	},	// LS_NONE		= 0,
;102:
;103:	// General movements with saber
;104:	{"Ready",		BOTH_STAND2,		Q_R,	Q_R,	AFLAG_IDLE,		350,	BLK_WIDE,	LS_READY,		LS_S_R2L,		0	},	// LS_READY,
;105:	{"Draw",		BOTH_STAND1TO2,		Q_R,	Q_R,	AFLAG_FINISH,	350,	BLK_NO,		LS_READY,		LS_S_R2L,		0	},	// LS_DRAW,
;106:	{"Putaway",		BOTH_STAND2TO1,		Q_R,	Q_R,	AFLAG_FINISH,	350,	BLK_NO,		LS_READY,		LS_S_R2L,		0	},	// LS_PUTAWAY,
;107:
;108:	// Attacks
;109:	//UL2LR
;110:	{"TL2BR Att",	BOTH_A1_TL_BR,		Q_TL,	Q_BR,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_R_TL2BR,		LS_R_TL2BR,		200	},	// LS_A_TL2BR
;111:	//SLASH LEFT
;112:	{"L2R Att",		BOTH_A1__L__R,		Q_L,	Q_R,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_R_L2R,		LS_R_L2R,		200 },	// LS_A_L2R
;113:	//LL2UR
;114:	{"BL2TR Att",	BOTH_A1_BL_TR,		Q_BL,	Q_TR,	AFLAG_ACTIVE,	50,		BLK_TIGHT,	LS_R_BL2TR,		LS_R_BL2TR,		200	},	// LS_A_BL2TR
;115:	//LR2UL
;116:	{"BR2TL Att",	BOTH_A1_BR_TL,		Q_BR,	Q_TL,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_R_BR2TL,		LS_R_BR2TL,		200	},	// LS_A_BR2TL
;117:	//SLASH RIGHT
;118:	{"R2L Att",		BOTH_A1__R__L,		Q_R,	Q_L,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_R_R2L,		LS_R_R2L,		200 },// LS_A_R2L
;119:	//UR2LL
;120:	{"TR2BL Att",	BOTH_A1_TR_BL,		Q_TR,	Q_BL,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_R_TR2BL,		LS_R_TR2BL,		200	},	// LS_A_TR2BL
;121:	//SLASH DOWN
;122:	{"T2B Att",		BOTH_A1_T__B_,		Q_T,	Q_B,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_R_T2B,		LS_R_T2B,		200	},	// LS_A_T2B
;123:	//special attacks
;124:	{"Back Stab",	BOTH_A2_STABBACK1,	Q_R,	Q_R,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_READY,		LS_READY,		200	},	// LS_A_BACKSTAB
;125:	{"Back Att",	BOTH_ATTACK_BACK,	Q_R,	Q_R,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_READY,		LS_READY,		200	},	// LS_A_BACK
;126:	{"CR Back Att",	BOTH_CROUCHATTACKBACK1,Q_R,	Q_R,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_READY,		LS_READY,		200	},	// LS_A_BACK_CR
;127:	{"Lunge Att",	BOTH_LUNGE2_B__T_,	Q_B,	Q_T,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_READY,		LS_READY,		200	},	// LS_A_LUNGE
;128:	{"Jump Att",	BOTH_FORCELEAP2_T__B_,Q_T,	Q_B,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_READY,		LS_READY,		200	},	// LS_A_JUMP_T__B_
;129:	{"Flip Stab",	BOTH_JUMPFLIPSTABDOWN,Q_R,	Q_T,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_READY,		LS_T1_T___R,	200	},	// LS_A_FLIP_STAB
;130:	{"Flip Slash",	BOTH_JUMPFLIPSLASHDOWN1,Q_L,Q_R,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_READY,		LS_T1__R_T_,	200	},	// LS_A_FLIP_SLASH
;131:
;132:	//starts
;133:	{"TL2BR St",	BOTH_S1_S1_TL,		Q_R,	Q_TL,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_A_TL2BR,		LS_A_TL2BR,		200	},	// LS_S_TL2BR
;134:	{"L2R St",		BOTH_S1_S1__L,		Q_R,	Q_L,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_A_L2R,		LS_A_L2R,		200	},	// LS_S_L2R
;135:	{"BL2TR St",	BOTH_S1_S1_BL,		Q_R,	Q_BL,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_A_BL2TR,		LS_A_BL2TR,		200	},	// LS_S_BL2TR
;136:	{"BR2TL St",	BOTH_S1_S1_BR,		Q_R,	Q_BR,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_A_BR2TL,		LS_A_BR2TL,		200	},	// LS_S_BR2TL
;137:	{"R2L St",		BOTH_S1_S1__R,		Q_R,	Q_R,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_A_R2L,		LS_A_R2L,		200	},	// LS_S_R2L
;138:	{"TR2BL St",	BOTH_S1_S1_TR,		Q_R,	Q_TR,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_A_TR2BL,		LS_A_TR2BL,		200	},	// LS_S_TR2BL
;139:	{"T2B St",		BOTH_S1_S1_T_,		Q_R,	Q_T,	AFLAG_ACTIVE,	100,	BLK_TIGHT,	LS_A_T2B,		LS_A_T2B,		200	},	// LS_S_T2B
;140:	
;141:	//returns
;142:	{"TL2BR Ret",	BOTH_R1_BR_S1,		Q_BR,	Q_R,	AFLAG_FINISH,	100,	BLK_TIGHT,	LS_READY,		LS_READY,		200	},	// LS_R_TL2BR
;143:	{"L2R Ret",		BOTH_R1__R_S1,		Q_R,	Q_R,	AFLAG_FINISH,	100,	BLK_TIGHT,	LS_READY,		LS_READY,		200	},	// LS_R_L2R
;144:	{"BL2TR Ret",	BOTH_R1_TR_S1,		Q_TR,	Q_R,	AFLAG_FINISH,	100,	BLK_TIGHT,	LS_READY,		LS_READY,		200	},	// LS_R_BL2TR
;145:	{"BR2TL Ret",	BOTH_R1_TL_S1,		Q_TL,	Q_R,	AFLAG_FINISH,	100,	BLK_TIGHT,	LS_READY,		LS_READY,		200	},	// LS_R_BR2TL
;146:	{"R2L Ret",		BOTH_R1__L_S1,		Q_L,	Q_R,	AFLAG_FINISH,	100,	BLK_TIGHT,	LS_READY,		LS_READY,		200	},	// LS_R_R2L
;147:	{"TR2BL Ret",	BOTH_R1_BL_S1,		Q_BL,	Q_R,	AFLAG_FINISH,	100,	BLK_TIGHT,	LS_READY,		LS_READY,		200	},	// LS_R_TR2BL
;148:	{"T2B Ret",		BOTH_R1_B__S1,		Q_B,	Q_R,	AFLAG_FINISH,	100,	BLK_TIGHT,	LS_READY,		LS_READY,		200	},	// LS_R_T2B
;149:
;150:	//Transitions
;151:	{"BR2R Trans",	BOTH_T1_BR__R,		Q_BR,	Q_R,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_L2R,		LS_A_R2L,		150	},	//# Fast arc bottom right to right
;152:	{"BR2TR Trans",	BOTH_T1_BR_TR,		Q_BR,	Q_TR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_A_TR2BL,		150	},	//# Fast arc bottom right to top right		(use: BOTH_T1_TR_BR)
;153:	{"BR2T Trans",	BOTH_T1_BR_T_,		Q_BR,	Q_T,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_A_T2B,		150	},	//# Fast arc bottom right to top			(use: BOTH_T1_T__BR)
;154:	{"BR2TL Trans",	BOTH_T1_BR_TL,		Q_BR,	Q_TL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BR2TL,		LS_A_TL2BR,		150	},	//# Fast weak spin bottom right to top left
;155:	{"BR2L Trans",	BOTH_T1_BR__L,		Q_BR,	Q_L,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_R2L,		LS_A_L2R,		150	},	//# Fast weak spin bottom right to left
;156:	{"BR2BL Trans",	BOTH_T1_BR_BL,		Q_BR,	Q_BL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TR2BL,		LS_A_BL2TR,		150	},	//# Fast weak spin bottom right to bottom left
;157:	{"R2BR Trans",	BOTH_T1__R_BR,		Q_R,	Q_BR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TL2BR,		LS_A_BR2TL,		150	},	//# Fast arc right to bottom right			(use: BOTH_T1_BR__R)
;158:	{"R2TR Trans",	BOTH_T1__R_TR,		Q_R,	Q_TR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_A_TR2BL,		150	},	//# Fast arc right to top right
;159:	{"R2T Trans",	BOTH_T1__R_T_,		Q_R,	Q_T,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_A_T2B,		150	},	//# Fast ar right to top				(use: BOTH_T1_T___R)
;160:	{"R2TL Trans",	BOTH_T1__R_TL,		Q_R,	Q_TL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BR2TL,		LS_A_TL2BR,		150	},	//# Fast arc right to top left
;161:	{"R2L Trans",	BOTH_T1__R__L,		Q_R,	Q_L,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_R2L,		LS_A_L2R,		150	},	//# Fast weak spin right to left
;162:	{"R2BL Trans",	BOTH_T1__R_BL,		Q_R,	Q_BL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TR2BL,		LS_A_BL2TR,		150	},	//# Fast weak spin right to bottom left
;163:	{"TR2BR Trans",	BOTH_T1_TR_BR,		Q_TR,	Q_BR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TL2BR,		LS_A_BR2TL,		150	},	//# Fast arc top right to bottom right
;164:	{"TR2R Trans",	BOTH_T1_TR__R,		Q_TR,	Q_R,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_L2R,		LS_A_R2L,		150	},	//# Fast arc top right to right			(use: BOTH_T1__R_TR)
;165:	{"TR2T Trans",	BOTH_T1_TR_T_,		Q_TR,	Q_T,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_A_T2B,		150	},	//# Fast arc top right to top				(use: BOTH_T1_T__TR)
;166:	{"TR2TL Trans",	BOTH_T1_TR_TL,		Q_TR,	Q_TL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BR2TL,		LS_A_TL2BR,		150	},	//# Fast arc top right to top left
;167:	{"TR2L Trans",	BOTH_T1_TR__L,		Q_TR,	Q_L,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_R2L,		LS_A_L2R,		150	},	//# Fast arc top right to left
;168:	{"TR2BL Trans",	BOTH_T1_TR_BL,		Q_TR,	Q_BL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TR2BL,		LS_A_BL2TR,		150	},	//# Fast weak spin top right to bottom left
;169:	{"T2BR Trans",	BOTH_T1_T__BR,		Q_T,	Q_BR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TL2BR,		LS_A_BR2TL,		150	},	//# Fast arc top to bottom right
;170:	{"T2R Trans",	BOTH_T1_T___R,		Q_T,	Q_R,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_L2R,		LS_A_R2L,		150	},	//# Fast arc top to right
;171:	{"T2TR Trans",	BOTH_T1_T__TR,		Q_T,	Q_TR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_A_TR2BL,		150	},	//# Fast arc top to top right
;172:	{"T2TL Trans",	BOTH_T1_T__TL,		Q_T,	Q_TL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BR2TL,		LS_A_TL2BR,		150	},	//# Fast arc top to top left
;173:	{"T2L Trans",	BOTH_T1_T___L,		Q_T,	Q_L,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_R2L,		LS_A_L2R,		150	},	//# Fast arc top to left
;174:	{"T2BL Trans",	BOTH_T1_T__BL,		Q_T,	Q_BL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TR2BL,		LS_A_BL2TR,		150	},	//# Fast arc top to bottom left
;175:	{"TL2BR Trans",	BOTH_T1_TL_BR,		Q_TL,	Q_BR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TL2BR,		LS_A_BR2TL,		150	},	//# Fast weak spin top left to bottom right
;176:	{"TL2R Trans",	BOTH_T1_TL__R,		Q_TL,	Q_R,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_L2R,		LS_A_R2L,		150	},	//# Fast arc top left to right			(use: BOTH_T1__R_TL)
;177:	{"TL2TR Trans",	BOTH_T1_TL_TR,		Q_TL,	Q_TR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_A_TR2BL,		150	},	//# Fast arc top left to top right			(use: BOTH_T1_TR_TL)
;178:	{"TL2T Trans",	BOTH_T1_TL_T_,		Q_TL,	Q_T,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_A_T2B,		150	},	//# Fast arc top left to top				(use: BOTH_T1_T__TL)
;179:	{"TL2L Trans",	BOTH_T1_TL__L,		Q_TL,	Q_L,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_R2L,		LS_A_L2R,		150	},	//# Fast arc top left to left				(use: BOTH_T1__L_TL)
;180:	{"TL2BL Trans",	BOTH_T1_TL_BL,		Q_TL,	Q_BL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TR2BL,		LS_A_BL2TR,		150	},	//# Fast arc top left to bottom left
;181:	{"L2BR Trans",	BOTH_T1__L_BR,		Q_L,	Q_BR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TL2BR,		LS_A_BR2TL,		150	},	//# Fast weak spin left to bottom right
;182:	{"L2R Trans",	BOTH_T1__L__R,		Q_L,	Q_R,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_L2R,		LS_A_R2L,		150	},	//# Fast weak spin left to right
;183:	{"L2TR Trans",	BOTH_T1__L_TR,		Q_L,	Q_TR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_A_TR2BL,		150	},	//# Fast arc left to top right			(use: BOTH_T1_TR__L)
;184:	{"L2T Trans",	BOTH_T1__L_T_,		Q_L,	Q_T,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_A_T2B,		150	},	//# Fast arc left to top				(use: BOTH_T1_T___L)
;185:	{"L2TL Trans",	BOTH_T1__L_TL,		Q_L,	Q_TL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BR2TL,		LS_A_TL2BR,		150	},	//# Fast arc left to top left
;186:	{"L2BL Trans",	BOTH_T1__L_BL,		Q_L,	Q_BL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TR2BL,		LS_A_BL2TR,		150	},	//# Fast arc left to bottom left			(use: BOTH_T1_BL__L)
;187:	{"BL2BR Trans",	BOTH_T1_BL_BR,		Q_BL,	Q_BR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TL2BR,		LS_A_BR2TL,		150	},	//# Fast weak spin bottom left to bottom right
;188:	{"BL2R Trans",	BOTH_T1_BL__R,		Q_BL,	Q_R,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_L2R,		LS_A_R2L,		150	},	//# Fast weak spin bottom left to right
;189:	{"BL2TR Trans",	BOTH_T1_BL_TR,		Q_BL,	Q_TR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_A_TR2BL,		150	},	//# Fast weak spin bottom left to top right
;190:	{"BL2T Trans",	BOTH_T1_BL_T_,		Q_BL,	Q_T,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_A_T2B,		150	},	//# Fast arc bottom left to top			(use: BOTH_T1_T__BL)
;191:	{"BL2TL Trans",	BOTH_T1_BL_TL,		Q_BL,	Q_TL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BR2TL,		LS_A_TL2BR,		150	},	//# Fast arc bottom left to top left		(use: BOTH_T1_TL_BL)
;192:	{"BL2L Trans",	BOTH_T1_BL__L,		Q_BL,	Q_L,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_R2L,		LS_A_L2R,		150	},	//# Fast arc bottom left to left
;193:
;194:	//Bounces
;195:	{"Bounce BR",	BOTH_B1_BR___,		Q_BR,	Q_BR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TL2BR,		LS_T1_BR_TR,	150	},	
;196:	{"Bounce R",	BOTH_B1__R___,		Q_R,	Q_R,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_L2R,		LS_T1__R__L,	150	},	
;197:	{"Bounce TR",	BOTH_B1_TR___,		Q_TR,	Q_TR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_T1_TR_TL,	150	},	
;198:	{"Bounce T",	BOTH_B1_T____,		Q_T,	Q_T,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_T1_T__BL,	150	},	
;199:	{"Bounce TL",	BOTH_B1_TL___,		Q_TL,	Q_TL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BR2TL,		LS_T1_TL_TR,	150	},	
;200:	{"Bounce L",	BOTH_B1__L___,		Q_L,	Q_L,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_R2L,		LS_T1__L__R,	150	},	
;201:	{"Bounce BL",	BOTH_B1_BL___,		Q_BL,	Q_BL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TR2BL,		LS_T1_BL_TR,	150	},	
;202:
;203:	//Deflected attacks (like bounces, but slide off enemy saber, not straight back)
;204:	{"Deflect BR",	BOTH_D1_BR___,		Q_BR,	Q_BR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TL2BR,		LS_T1_BR_TR,	150	},	
;205:	{"Deflect R",	BOTH_D1__R___,		Q_R,	Q_R,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_L2R,		LS_T1__R__L,	150	},	
;206:	{"Deflect TR",	BOTH_D1_TR___,		Q_TR,	Q_TR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_T1_TR_TL,	150	},	
;207:	{"Deflect T",	BOTH_B1_T____,		Q_T,	Q_T,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_T1_T__BL,	150	},	
;208:	{"Deflect TL",	BOTH_D1_TL___,		Q_TL,	Q_TL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BR2TL,		LS_T1_TL_TR,	150	},	
;209:	{"Deflect L",	BOTH_D1__L___,		Q_L,	Q_L,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_R2L,		LS_T1__L__R,	150	},	
;210:	{"Deflect BL",	BOTH_D1_BL___,		Q_BL,	Q_BL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_TR2BL,		LS_T1_BL_TR,	150	},	
;211:	{"Deflect B",	BOTH_D1_B____,		Q_B,	Q_B,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_R_BL2TR,		LS_T1_T__BL,	150	},	
;212:
;213:	//Reflected attacks
;214:	{"Reflected BR",BOTH_V1_BR_S1,		Q_BR,	Q_BR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_READY,		LS_READY,	150	},//	LS_V1_BR
;215:	{"Reflected R",	BOTH_V1__R_S1,		Q_R,	Q_R,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_READY,		LS_READY,	150	},//	LS_V1__R
;216:	{"Reflected TR",BOTH_V1_TR_S1,		Q_TR,	Q_TR,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_READY,		LS_READY,	150	},//	LS_V1_TR
;217:	{"Reflected T",	BOTH_V1_T__S1,		Q_T,	Q_T,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_READY,		LS_READY,	150	},//	LS_V1_T_
;218:	{"Reflected TL",BOTH_V1_TL_S1,		Q_TL,	Q_TL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_READY,		LS_READY,	150	},//	LS_V1_TL
;219:	{"Reflected L",	BOTH_V1__L_S1,		Q_L,	Q_L,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_READY,		LS_READY,	150	},//	LS_V1__L
;220:	{"Reflected BL",BOTH_V1_BL_S1,		Q_BL,	Q_BL,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_READY,		LS_READY,	150	},//	LS_V1_BL
;221:	{"Reflected B",	BOTH_V1_B__S1,		Q_B,	Q_B,	AFLAG_ACTIVE,	100,	BLK_NO,	LS_READY,		LS_READY,	150	},//	LS_V1_B_
;222:
;223:	// Broken parries
;224:	{"BParry Top",	BOTH_H1_S1_T_,		Q_T,	Q_B,	AFLAG_ACTIVE,	50,		BLK_NO,	LS_READY,		LS_READY,		150	},	// LS_PARRY_UP,
;225:	{"BParry UR",	BOTH_H1_S1_TR,		Q_TR,	Q_BL,	AFLAG_ACTIVE,	50,		BLK_NO,	LS_READY,		LS_READY,		150	},	// LS_PARRY_UR,
;226:	{"BParry UL",	BOTH_H1_S1_TL,		Q_TL,	Q_BR,	AFLAG_ACTIVE,	50,		BLK_NO,	LS_READY,		LS_READY,		150	},	// LS_PARRY_UL,
;227:	{"BParry LR",	BOTH_H1_S1_BL,		Q_BL,	Q_TR,	AFLAG_ACTIVE,	50,		BLK_NO,	LS_READY,		LS_READY,		150	},	// LS_PARRY_LR,
;228:	{"BParry Bot",	BOTH_H1_S1_B_,		Q_B,	Q_T,	AFLAG_ACTIVE,	50,		BLK_NO,	LS_READY,		LS_READY,		150	},	// LS_PARRY_LL
;229:	{"BParry LL",	BOTH_H1_S1_BR,		Q_BR,	Q_TL,	AFLAG_ACTIVE,	50,		BLK_NO,	LS_READY,		LS_READY,		150	},	// LS_PARRY_LL
;230:
;231:	// Knockaways
;232:	{"Knock Top",	BOTH_K1_S1_T_,		Q_R,	Q_T,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_BL2TR,		LS_T1_T__BR,		150	},	// LS_PARRY_UP,
;233:	{"Knock UR",	BOTH_K1_S1_TR,		Q_R,	Q_TR,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_BL2TR,		LS_T1_TR__R,		150	},	// LS_PARRY_UR,
;234:	{"Knock UL",	BOTH_K1_S1_TL,		Q_R,	Q_TL,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_BR2TL,		LS_T1_TL__L,		150	},	// LS_PARRY_UL,
;235:	{"Knock LR",	BOTH_K1_S1_BL,		Q_R,	Q_BL,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_TL2BR,		LS_T1_BL_TL,		150	},	// LS_PARRY_LR,
;236:	{"Knock LL",	BOTH_K1_S1_BR,		Q_R,	Q_BR,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_TR2BL,		LS_T1_BR_TR,		150	},	// LS_PARRY_LL
;237:
;238:	// Parry
;239:	{"Parry Top",	BOTH_P1_S1_T_,		Q_R,	Q_T,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_BL2TR,		LS_A_T2B,		150	},	// LS_PARRY_UP,
;240:	{"Parry UR",	BOTH_P1_S1_TR,		Q_R,	Q_TL,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_BL2TR,		LS_A_TR2BL,		150	},	// LS_PARRY_UR,
;241:	{"Parry UL",	BOTH_P1_S1_TL,		Q_R,	Q_TR,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_BR2TL,		LS_A_TL2BR,		150	},	// LS_PARRY_UL,
;242:	{"Parry LR",	BOTH_P1_S1_BL,		Q_R,	Q_BR,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_TL2BR,		LS_A_BR2TL,		150	},	// LS_PARRY_LR,
;243:	{"Parry LL",	BOTH_P1_S1_BR,		Q_R,	Q_BL,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_TR2BL,		LS_A_BL2TR,		150	},	// LS_PARRY_LL
;244:
;245:	// Reflecting a missile
;246:	{"Reflect Top",	BOTH_P1_S1_T_,		Q_R,	Q_T,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_BL2TR,		LS_A_T2B,		300	},	// LS_PARRY_UP,
;247:	{"Reflect UR",	BOTH_P1_S1_TL,		Q_R,	Q_TR,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_BR2TL,		LS_A_TL2BR,		300	},	// LS_PARRY_UR,
;248:	{"Reflect UL",	BOTH_P1_S1_TR,		Q_R,	Q_TL,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_BL2TR,		LS_A_TR2BL,		300	},	// LS_PARRY_UL,
;249:	{"Reflect LR",	BOTH_P1_S1_BR,		Q_R,	Q_BL,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_TR2BL,		LS_A_BL2TR,		300	},	// LS_PARRY_LR
;250:	{"Reflect LL",	BOTH_P1_S1_BL,		Q_R,	Q_BR,	AFLAG_ACTIVE,	50,		BLK_WIDE,	LS_R_TL2BR,		LS_A_BR2TL,		300	},	// LS_PARRY_LL,
;251:};
;252:
;253:int transitionMove[Q_NUM_QUADS][Q_NUM_QUADS] = 
;254:{
;255:	LS_NONE,	//Can't transition to same pos!
;256:	LS_T1_BR__R,//40
;257:	LS_T1_BR_TR,
;258:	LS_T1_BR_T_,
;259:	LS_T1_BR_TL,
;260:	LS_T1_BR__L,
;261:	LS_T1_BR_BL,
;262:	LS_NONE,	//No transitions to bottom, and no anims start there, so shouldn't need any
;263:	LS_T1__R_BR,//46
;264:	LS_NONE,	//Can't transition to same pos!
;265:	LS_T1__R_TR,
;266:	LS_T1__R_T_,
;267:	LS_T1__R_TL,
;268:	LS_T1__R__L,
;269:	LS_T1__R_BL,
;270:	LS_NONE,	//No transitions to bottom, and no anims start there, so shouldn't need any
;271:	LS_T1_TR_BR,//52
;272:	LS_T1_TR__R,
;273:	LS_NONE,	//Can't transition to same pos!
;274:	LS_T1_TR_T_,
;275:	LS_T1_TR_TL,
;276:	LS_T1_TR__L,
;277:	LS_T1_TR_BL,
;278:	LS_NONE,	//No transitions to bottom, and no anims start there, so shouldn't need any
;279:	LS_T1_T__BR,//58
;280:	LS_T1_T___R,
;281:	LS_T1_T__TR,
;282:	LS_NONE,	//Can't transition to same pos!
;283:	LS_T1_T__TL,
;284:	LS_T1_T___L,
;285:	LS_T1_T__BL,
;286:	LS_NONE,	//No transitions to bottom, and no anims start there, so shouldn't need any
;287:	LS_T1_TL_BR,//64
;288:	LS_T1_TL__R,
;289:	LS_T1_TL_TR,
;290:	LS_T1_TL_T_,
;291:	LS_NONE,	//Can't transition to same pos!
;292:	LS_T1_TL__L,
;293:	LS_T1_TL_BL,
;294:	LS_NONE,	//No transitions to bottom, and no anims start there, so shouldn't need any
;295:	LS_T1__L_BR,//70
;296:	LS_T1__L__R,
;297:	LS_T1__L_TR,
;298:	LS_T1__L_T_,
;299:	LS_T1__L_TL,
;300:	LS_NONE,	//Can't transition to same pos!
;301:	LS_T1__L_BL,
;302:	LS_NONE,	//No transitions to bottom, and no anims start there, so shouldn't need any
;303:	LS_T1_BL_BR,//76
;304:	LS_T1_BL__R,
;305:	LS_T1_BL_TR,
;306:	LS_T1_BL_T_,
;307:	LS_T1_BL_TL,
;308:	LS_T1_BL__L,
;309:	LS_NONE,	//Can't transition to same pos!
;310:	LS_NONE,	//No transitions to bottom, and no anims start there, so shouldn't need any
;311:	LS_T1_BL_BR,//NOTE: there are no transitions from bottom, so re-use the bottom right transitions
;312:	LS_T1_BR__R,
;313:	LS_T1_BR_TR,
;314:	LS_T1_BR_T_,
;315:	LS_T1_BR_TL,
;316:	LS_T1_BR__L,
;317:	LS_T1_BR_BL,
;318:	LS_NONE		//No transitions to bottom, and no anims start there, so shouldn't need any
;319:};
;320:
;321:saberMoveName_t PM_AttackMoveForQuad( int quad )
;322:{
line 323
;323:	switch ( quad )
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $214
ADDRLP4 0
INDIRI4
CNSTI4 7
GTI4 $214
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $223
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $223
address $216
address $217
address $218
address $219
address $220
address $221
address $222
address $216
code
line 324
;324:	{
LABELV $216
line 327
;325:	case Q_B:
;326:	case Q_BR:
;327:		return LS_A_BR2TL;
CNSTI4 7
RETI4
ADDRGP4 $213
JUMPV
line 328
;328:		break;
LABELV $217
line 330
;329:	case Q_R:
;330:		return LS_A_R2L;
CNSTI4 8
RETI4
ADDRGP4 $213
JUMPV
line 331
;331:		break;
LABELV $218
line 333
;332:	case Q_TR:
;333:		return LS_A_TR2BL;
CNSTI4 9
RETI4
ADDRGP4 $213
JUMPV
line 334
;334:		break;
LABELV $219
line 336
;335:	case Q_T:
;336:		return LS_A_T2B;
CNSTI4 10
RETI4
ADDRGP4 $213
JUMPV
line 337
;337:		break;
LABELV $220
line 339
;338:	case Q_TL:
;339:		return LS_A_TL2BR;
CNSTI4 4
RETI4
ADDRGP4 $213
JUMPV
line 340
;340:		break;
LABELV $221
line 342
;341:	case Q_L:
;342:		return LS_A_L2R;
CNSTI4 5
RETI4
ADDRGP4 $213
JUMPV
line 343
;343:		break;
LABELV $222
line 345
;344:	case Q_BL:
;345:		return LS_A_BL2TR;
CNSTI4 6
RETI4
ADDRGP4 $213
JUMPV
line 346
;346:		break;
LABELV $214
line 348
;347:	}
;348:	return LS_NONE;
CNSTI4 0
RETI4
LABELV $213
endproc PM_AttackMoveForQuad 4 0
export PM_SaberAnimTransitionAnim
proc PM_SaberAnimTransitionAnim 32 0
line 352
;349:}
;350:
;351:int PM_SaberAnimTransitionAnim( int curmove, int newmove )
;352:{
line 354
;353:	//FIXME: take FP_SABERATTACK into account here somehow?
;354:	int retmove = newmove;
ADDRLP4 0
ADDRFP4 4
INDIRI4
ASGNI4
line 355
;355:	if ( curmove == LS_READY )
ADDRFP4 0
INDIRI4
CNSTI4 1
NEI4 $225
line 356
;356:	{//just standing there
line 357
;357:		switch ( newmove )
ADDRLP4 4
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 4
LTI4 $226
ADDRLP4 4
INDIRI4
CNSTI4 10
GTI4 $226
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $230-16
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $230
address $229
address $229
address $229
address $229
address $229
address $229
address $229
code
line 358
;358:		{
LABELV $229
line 367
;359:		case LS_A_TL2BR:
;360:		case LS_A_L2R:
;361:		case LS_A_BL2TR:
;362:		case LS_A_BR2TL:
;363:		case LS_A_R2L:
;364:		case LS_A_TR2BL:
;365:		case LS_A_T2B:
;366:			//transition is the start
;367:			retmove = LS_S_TL2BR + (newmove-LS_A_TL2BR);
ADDRLP4 0
ADDRFP4 4
INDIRI4
CNSTI4 4
SUBI4
CNSTI4 18
ADDI4
ASGNI4
line 368
;368:			break;
line 370
;369:		}
;370:	}
ADDRGP4 $226
JUMPV
LABELV $225
line 372
;371:	else
;372:	{
line 373
;373:		switch ( newmove )
ADDRLP4 4
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 1
LTI4 $232
ADDRLP4 4
INDIRI4
CNSTI4 10
GTI4 $232
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $258-4
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $258
address $234
address $232
address $232
address $240
address $240
address $240
address $240
address $240
address $240
address $240
code
line 374
;374:		{
LABELV $234
line 377
;375:		//transitioning to ready pose
;376:		case LS_READY:
;377:			switch ( curmove )
ADDRLP4 8
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 4
LTI4 $233
ADDRLP4 8
INDIRI4
CNSTI4 10
GTI4 $233
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $238-16
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $238
address $237
address $237
address $237
address $237
address $237
address $237
address $237
code
line 378
;378:			{
LABELV $237
line 388
;379:			//transitioning from an attack
;380:			case LS_A_TL2BR:
;381:			case LS_A_L2R:
;382:			case LS_A_BL2TR:
;383:			case LS_A_BR2TL:
;384:			case LS_A_R2L:
;385:			case LS_A_TR2BL:
;386:			case LS_A_T2B:
;387:				//transition is the return
;388:				retmove = LS_R_TL2BR + (newmove-LS_A_TL2BR);
ADDRLP4 0
ADDRFP4 4
INDIRI4
CNSTI4 4
SUBI4
CNSTI4 25
ADDI4
ASGNI4
line 389
;389:				break;
line 391
;390:			}
;391:			break;
ADDRGP4 $233
JUMPV
LABELV $240
line 400
;392:		//transitioning to an attack
;393:		case LS_A_TL2BR:
;394:		case LS_A_L2R:
;395:		case LS_A_BL2TR:
;396:		case LS_A_BR2TL:
;397:		case LS_A_R2L:
;398:		case LS_A_TR2BL:
;399:		case LS_A_T2B:
;400:			/*if ( newmove == curmove )
ADDRLP4 12
CNSTI4 40
ASGNI4
ADDRLP4 12
INDIRI4
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 saberMoveData+12
ADDP4
INDIRI4
ADDRLP4 12
INDIRI4
ADDRFP4 4
INDIRI4
MULI4
ADDRGP4 saberMoveData+8
ADDP4
INDIRI4
NEI4 $241
line 405
;401:			{//FIXME: need a spin or something or go to next level, but for now, just play the return
;402:				retmove = LS_R_TL2BR + (newmove-LS_A_TL2BR);
;403:			}
;404:			else */if ( saberMoveData[curmove].endQuad == saberMoveData[newmove].startQuad )
;405:			{//new move starts from same quadrant
line 406
;406:				retmove = newmove;
ADDRLP4 0
ADDRFP4 4
INDIRI4
ASGNI4
line 407
;407:			}
ADDRGP4 $233
JUMPV
LABELV $241
line 409
;408:			else
;409:			{
line 410
;410:				switch ( curmove )
ADDRLP4 16
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 4
LTI4 $233
ADDRLP4 16
INDIRI4
CNSTI4 31
GTI4 $253
ADDRLP4 16
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $254-16
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $254
address $247
address $247
address $247
address $247
address $247
address $247
address $247
address $233
address $233
address $233
address $233
address $233
address $233
address $233
address $233
address $233
address $233
address $233
address $233
address $233
address $233
address $250
address $250
address $250
address $250
address $250
address $250
address $250
code
LABELV $253
ADDRLP4 20
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 108
LTI4 $233
ADDRLP4 20
INDIRI4
CNSTI4 117
GTI4 $233
ADDRLP4 20
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $256-432
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $256
address $250
address $250
address $250
address $250
address $250
address $250
address $250
address $250
address $250
address $250
code
line 411
;411:				{
LABELV $247
line 420
;412:				//transitioning from an attack
;413:				case LS_A_TL2BR:
;414:				case LS_A_L2R:
;415:				case LS_A_BL2TR:
;416:				case LS_A_BR2TL:
;417:				case LS_A_R2L:
;418:				case LS_A_TR2BL:
;419:				case LS_A_T2B:
;420:					retmove = transitionMove[saberMoveData[curmove].endQuad][saberMoveData[newmove].startQuad];
ADDRLP4 24
CNSTI4 40
ASGNI4
ADDRLP4 0
ADDRLP4 24
INDIRI4
ADDRFP4 4
INDIRI4
MULI4
ADDRGP4 saberMoveData+8
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 24
INDIRI4
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 saberMoveData+12
ADDP4
INDIRI4
CNSTI4 5
LSHI4
ADDRGP4 transitionMove
ADDP4
ADDP4
INDIRI4
ASGNI4
line 421
;421:					break;
ADDRGP4 $233
JUMPV
LABELV $250
line 459
;422:				//transitioning from a return
;423:				case LS_R_TL2BR:
;424:				case LS_R_L2R:
;425:				case LS_R_BL2TR:
;426:				case LS_R_BR2TL:
;427:				case LS_R_R2L:
;428:				case LS_R_TR2BL:
;429:				case LS_R_T2B:
;430:				//transitioning from a bounce
;431:				/*
;432:				case LS_BOUNCE_UL2LL:
;433:				case LS_BOUNCE_LL2UL:
;434:				case LS_BOUNCE_L2LL:
;435:				case LS_BOUNCE_L2UL:
;436:				case LS_BOUNCE_UR2LR:
;437:				case LS_BOUNCE_LR2UR:
;438:				case LS_BOUNCE_R2LR:
;439:				case LS_BOUNCE_R2UR:
;440:				case LS_BOUNCE_TOP:
;441:				case LS_OVER_UR2UL:
;442:				case LS_OVER_UL2UR:
;443:				case LS_BOUNCE_UR:
;444:				case LS_BOUNCE_UL:
;445:				case LS_BOUNCE_LR:
;446:				case LS_BOUNCE_LL:
;447:				*/
;448:				//transitioning from a parry/deflection
;449:				case LS_PARRY_UP:
;450:				case LS_REFLECT_UP:
;451:				case LS_PARRY_UR:
;452:				case LS_REFLECT_UR:
;453:				case LS_PARRY_UL:
;454:				case LS_REFLECT_UL:
;455:				case LS_PARRY_LR:
;456:				case LS_REFLECT_LR:
;457:				case LS_PARRY_LL:
;458:				case LS_REFLECT_LL:
;459:					retmove = transitionMove[saberMoveData[curmove].endQuad][saberMoveData[newmove].startQuad];
ADDRLP4 28
CNSTI4 40
ASGNI4
ADDRLP4 0
ADDRLP4 28
INDIRI4
ADDRFP4 4
INDIRI4
MULI4
ADDRGP4 saberMoveData+8
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 28
INDIRI4
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 saberMoveData+12
ADDP4
INDIRI4
CNSTI4 5
LSHI4
ADDRGP4 transitionMove
ADDP4
ADDP4
INDIRI4
ASGNI4
line 460
;460:					break;
line 463
;461:				//NB: transitioning from transitions is fine
;462:				}
;463:			}
line 464
;464:			break;
LABELV $232
LABELV $233
line 467
;465:		//transitioning to any other anim is not supported
;466:		}
;467:	}
LABELV $226
line 469
;468:
;469:	if ( retmove == LS_NONE )
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $260
line 470
;470:	{
line 471
;471:		return newmove;
ADDRFP4 4
INDIRI4
RETI4
ADDRGP4 $224
JUMPV
LABELV $260
line 474
;472:	}
;473:
;474:	return retmove;
ADDRLP4 0
INDIRI4
RETI4
LABELV $224
endproc PM_SaberAnimTransitionAnim 32 0
export PM_SaberMoveQuadrantForMovement
proc PM_SaberMoveQuadrantForMovement 0 0
line 479
;475:}
;476:
;477:
;478:int PM_SaberMoveQuadrantForMovement( usercmd_t *ucmd )
;479:{
line 480
;480:	if ( ucmd->rightmove > 0 )
ADDRFP4 0
INDIRP4
CNSTI4 25
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
LEI4 $263
line 481
;481:	{//moving right
line 482
;482:		if ( ucmd->forwardmove > 0 )
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
LEI4 $265
line 483
;483:		{//forward right = TL2BR slash
line 484
;484:			return Q_TL;
CNSTI4 4
RETI4
ADDRGP4 $262
JUMPV
LABELV $265
line 486
;485:		}
;486:		else if ( ucmd->forwardmove < 0 )
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
GEI4 $267
line 487
;487:		{//backward right = BL2TR uppercut
line 488
;488:			return Q_BL;
CNSTI4 6
RETI4
ADDRGP4 $262
JUMPV
LABELV $267
line 491
;489:		}
;490:		else
;491:		{//just right is a left slice
line 492
;492:			return Q_L;
CNSTI4 5
RETI4
ADDRGP4 $262
JUMPV
LABELV $263
line 495
;493:		}
;494:	}
;495:	else if ( ucmd->rightmove < 0 )
ADDRFP4 0
INDIRP4
CNSTI4 25
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
GEI4 $269
line 496
;496:	{//moving left
line 497
;497:		if ( ucmd->forwardmove > 0 )
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
LEI4 $271
line 498
;498:		{//forward left = TR2BL slash
line 499
;499:			return Q_TR;
CNSTI4 2
RETI4
ADDRGP4 $262
JUMPV
LABELV $271
line 501
;500:		}
;501:		else if ( ucmd->forwardmove < 0 )
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
GEI4 $273
line 502
;502:		{//backward left = BR2TL uppercut
line 503
;503:			return Q_BR;
CNSTI4 0
RETI4
ADDRGP4 $262
JUMPV
LABELV $273
line 506
;504:		}
;505:		else
;506:		{//just left is a right slice
line 507
;507:			return Q_R;
CNSTI4 1
RETI4
ADDRGP4 $262
JUMPV
LABELV $269
line 511
;508:		}
;509:	}
;510:	else
;511:	{//not moving left or right
line 512
;512:		if ( ucmd->forwardmove > 0 )
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
LEI4 $275
line 513
;513:		{//forward= T2B slash
line 514
;514:			return Q_T;
CNSTI4 3
RETI4
ADDRGP4 $262
JUMPV
LABELV $275
line 516
;515:		}
;516:		else if ( ucmd->forwardmove < 0 )
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
GEI4 $277
line 517
;517:		{//backward= T2B slash	//or B2T uppercut?
line 518
;518:			return Q_T;
CNSTI4 3
RETI4
ADDRGP4 $262
JUMPV
LABELV $277
line 521
;519:		}
;520:		else //if ( curmove == LS_READY )//???
;521:		{//Not moving at all
line 522
;522:			return Q_R;
CNSTI4 1
RETI4
LABELV $262
endproc PM_SaberMoveQuadrantForMovement 0 0
export PM_SaberInBounce
proc PM_SaberInBounce 8 0
line 530
;523:		}
;524:	}
;525:	//return Q_R;//????
;526:}
;527:
;528://===================================================================
;529:qboolean PM_SaberInBounce( int move )
;530:{
line 531
;531:	if ( move >= LS_B1_BR && move <= LS_B1_BL )
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 74
LTI4 $280
ADDRLP4 0
INDIRI4
CNSTI4 80
GTI4 $280
line 532
;532:	{
line 533
;533:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $279
JUMPV
LABELV $280
line 535
;534:	}
;535:	if ( move >= LS_D1_BR && move <= LS_D1_BL )
ADDRLP4 4
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 81
LTI4 $282
ADDRLP4 4
INDIRI4
CNSTI4 87
GTI4 $282
line 536
;536:	{
line 537
;537:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $279
JUMPV
LABELV $282
line 539
;538:	}
;539:	return qfalse;
CNSTI4 0
RETI4
LABELV $279
endproc PM_SaberInBounce 8 0
export PM_SaberInTransition
proc PM_SaberInTransition 4 0
line 543
;540:}
;541:
;542:qboolean PM_SaberInTransition( int move )
;543:{
line 544
;544:	if ( move >= LS_T1_BR__R && move <= LS_T1_BL__L )
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 32
LTI4 $285
ADDRLP4 0
INDIRI4
CNSTI4 73
GTI4 $285
line 545
;545:	{
line 546
;546:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $284
JUMPV
LABELV $285
line 548
;547:	}
;548:	return qfalse;
CNSTI4 0
RETI4
LABELV $284
endproc PM_SaberInTransition 4 0
export PM_SaberKataDone
proc PM_SaberKataDone 20 8
line 552
;549:}
;550:
;551:qboolean PM_SaberKataDone( void )
;552:{
line 553
;553:	if ( (pm->ps->fd.saberAnimLevel >= FORCE_LEVEL_3 && pm->ps->saberAttackChainCount > PM_irand_timesync( 0, 1 )) ||
ADDRLP4 0
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 1216
ADDP4
INDIRI4
CNSTI4 3
LTI4 $291
CNSTI4 0
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 4
ADDRGP4 PM_irand_timesync
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 1296
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
GTI4 $290
LABELV $291
ADDRLP4 8
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 12
CNSTI4 2
ASGNI4
ADDRLP4 8
INDIRP4
CNSTI4 1216
ADDP4
INDIRI4
ADDRLP4 12
INDIRI4
NEI4 $288
ADDRLP4 12
INDIRI4
ARGI4
CNSTI4 5
ARGI4
ADDRLP4 16
ADDRGP4 PM_irand_timesync
CALLI4
ASGNI4
ADDRLP4 8
INDIRP4
CNSTI4 1296
ADDP4
INDIRI4
ADDRLP4 16
INDIRI4
LEI4 $288
LABELV $290
line 555
;554:		( pm->ps->fd.saberAnimLevel == FORCE_LEVEL_2 && pm->ps->saberAttackChainCount > PM_irand_timesync( 2, 5 ) ) )
;555:	{
line 556
;556:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $287
JUMPV
LABELV $288
line 558
;557:	}
;558:	return qfalse;
CNSTI4 0
RETI4
LABELV $287
endproc PM_SaberKataDone 20 8
export PM_SetAnimFrame
proc PM_SetAnimFrame 0 0
line 562
;559:}
;560:
;561:void PM_SetAnimFrame( playerState_t *gent, int frame, qboolean torso, qboolean legs )
;562:{
line 563
;563:	gent->saberLockFrame = frame;
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
ADDRFP4 4
INDIRI4
ASGNI4
line 564
;564:}
LABELV $292
endproc PM_SetAnimFrame 0 0
export PM_SaberLockBreak
proc PM_SaberLockBreak 88 16
line 567
;565:
;566:void PM_SaberLockBreak( playerState_t *genemy, qboolean victory )
;567:{
line 568
;568:	int	winAnim = BOTH_STAND1, loseAnim = BOTH_STAND1;
ADDRLP4 0
CNSTI4 550
ASGNI4
ADDRLP4 8
CNSTI4 550
ASGNI4
line 569
;569:	qboolean punishLoser = qfalse;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 571
;570:
;571:	switch ( (pm->ps->torsoAnim&~ANIM_TOGGLEBIT) )
ADDRLP4 12
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 100
ADDP4
INDIRI4
CNSTI4 -2049
BANDI4
ASGNI4
ADDRLP4 20
CNSTI4 538
ASGNI4
ADDRLP4 12
INDIRI4
ADDRLP4 20
INDIRI4
EQI4 $297
ADDRLP4 12
INDIRI4
CNSTI4 541
EQI4 $300
ADDRLP4 12
INDIRI4
ADDRLP4 20
INDIRI4
LTI4 $294
LABELV $309
ADDRLP4 12
INDIRI4
CNSTI4 548
EQI4 $303
ADDRLP4 12
INDIRI4
CNSTI4 549
EQI4 $306
ADDRGP4 $294
JUMPV
line 572
;572:	{
LABELV $297
line 574
;573:	case BOTH_BF2LOCK:
;574:		pm->ps->saberMove = LS_A_T2B;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 512
ADDP4
CNSTI4 10
ASGNI4
line 575
;575:		winAnim = BOTH_A3_T__B_;
ADDRLP4 0
CNSTI4 280
ASGNI4
line 576
;576:		if ( !victory )
ADDRFP4 4
INDIRI4
CNSTI4 0
NEI4 $298
line 577
;577:		{//no-one won
line 578
;578:			genemy->saberMove = LS_A_T2B;
ADDRFP4 0
INDIRP4
CNSTI4 512
ADDP4
CNSTI4 10
ASGNI4
line 579
;579:			loseAnim = winAnim;
ADDRLP4 8
ADDRLP4 0
INDIRI4
ASGNI4
line 580
;580:		}
ADDRGP4 $295
JUMPV
LABELV $298
line 582
;581:		else
;582:		{
line 584
;583:			//loseAnim = BOTH_KNOCKDOWN4;
;584:			punishLoser = qtrue;
ADDRLP4 4
CNSTI4 1
ASGNI4
line 585
;585:		}
line 586
;586:		break;
ADDRGP4 $295
JUMPV
LABELV $300
line 588
;587:	case BOTH_BF1LOCK:
;588:		pm->ps->saberMove = LS_K1_T_;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 512
ADDP4
CNSTI4 103
ASGNI4
line 589
;589:		winAnim = BOTH_K1_S1_T_;
ADDRLP4 0
CNSTI4 516
ASGNI4
line 590
;590:		if ( !victory )
ADDRFP4 4
INDIRI4
CNSTI4 0
NEI4 $301
line 591
;591:		{//no-one won
line 592
;592:			genemy->saberMove = LS_K1_T_;
ADDRFP4 0
INDIRP4
CNSTI4 512
ADDP4
CNSTI4 103
ASGNI4
line 593
;593:			loseAnim = winAnim;
ADDRLP4 8
ADDRLP4 0
INDIRI4
ASGNI4
line 594
;594:		}
ADDRGP4 $295
JUMPV
LABELV $301
line 596
;595:		else
;596:		{
line 598
;597:			//loseAnim = BOTH_BF1BREAK;
;598:			punishLoser = qtrue;
ADDRLP4 4
CNSTI4 1
ASGNI4
line 599
;599:		}
line 600
;600:		break;
ADDRGP4 $295
JUMPV
LABELV $303
line 602
;601:	case BOTH_CWCIRCLELOCK:
;602:		winAnim = BOTH_CWCIRCLEBREAK;
ADDRLP4 0
CNSTI4 546
ASGNI4
line 603
;603:		if ( !victory )
ADDRFP4 4
INDIRI4
CNSTI4 0
NEI4 $304
line 604
;604:		{//no-one won
line 605
;605:			loseAnim = winAnim;
ADDRLP4 8
ADDRLP4 0
INDIRI4
ASGNI4
line 606
;606:		}
ADDRGP4 $295
JUMPV
LABELV $304
line 608
;607:		else
;608:		{
line 609
;609:			genemy->saberMove = /*genemy->saberBounceMove =*/ LS_H1_BL;
ADDRFP4 0
INDIRP4
CNSTI4 512
ADDP4
CNSTI4 102
ASGNI4
line 610
;610:			genemy->saberBlocked = BLOCKED_PARRY_BROKEN;
ADDRFP4 0
INDIRP4
CNSTI4 520
ADDP4
CNSTI4 1
ASGNI4
line 612
;611:			//loseAnim = BOTH_H1_S1_BR;
;612:			punishLoser = qtrue;
ADDRLP4 4
CNSTI4 1
ASGNI4
line 613
;613:		}
line 614
;614:		break;
ADDRGP4 $295
JUMPV
LABELV $306
line 616
;615:	case BOTH_CCWCIRCLELOCK:
;616:		winAnim = BOTH_CCWCIRCLEBREAK;
ADDRLP4 0
CNSTI4 547
ASGNI4
line 617
;617:		if ( !victory )
ADDRFP4 4
INDIRI4
CNSTI4 0
NEI4 $307
line 618
;618:		{//no-one won
line 619
;619:			loseAnim = winAnim;
ADDRLP4 8
ADDRLP4 0
INDIRI4
ASGNI4
line 620
;620:		}
ADDRGP4 $295
JUMPV
LABELV $307
line 622
;621:		else
;622:		{
line 623
;623:			genemy->saberMove = /*genemy->saberBounceMove =*/ LS_H1_BR;
ADDRFP4 0
INDIRP4
CNSTI4 512
ADDP4
CNSTI4 100
ASGNI4
line 624
;624:			genemy->saberBlocked = BLOCKED_PARRY_BROKEN;
ADDRFP4 0
INDIRP4
CNSTI4 520
ADDP4
CNSTI4 1
ASGNI4
line 626
;625:			//loseAnim = BOTH_H1_S1_BL;
;626:			punishLoser = qtrue;
ADDRLP4 4
CNSTI4 1
ASGNI4
line 627
;627:		}
line 628
;628:		break;
LABELV $294
LABELV $295
line 630
;629:	}
;630:	PM_SetAnim( SETANIM_BOTH, winAnim, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD, -1 );
ADDRLP4 28
CNSTI4 3
ASGNI4
ADDRLP4 28
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 28
INDIRI4
ARGI4
CNSTI4 -1
ARGI4
ADDRGP4 PM_SetAnim
CALLV
pop
line 632
;631:
;632:	if (punishLoser)
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $310
line 633
;633:	{
line 636
;634:		vec3_t oppDir;
;635:
;636:		int strength = 8;
ADDRLP4 44
CNSTI4 8
ASGNI4
line 638
;637:
;638:		VectorSubtract(genemy->origin, pm->ps->origin, oppDir);
ADDRLP4 48
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 52
CNSTI4 20
ASGNI4
ADDRLP4 56
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 32
ADDRLP4 48
INDIRP4
ADDRLP4 52
INDIRI4
ADDP4
INDIRF4
ADDRLP4 56
INDIRP4
INDIRP4
ADDRLP4 52
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 60
CNSTI4 24
ASGNI4
ADDRLP4 32+4
ADDRLP4 48
INDIRP4
ADDRLP4 60
INDIRI4
ADDP4
INDIRF4
ADDRLP4 56
INDIRP4
INDIRP4
ADDRLP4 60
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 64
CNSTI4 28
ASGNI4
ADDRLP4 32+8
ADDRFP4 0
INDIRP4
ADDRLP4 64
INDIRI4
ADDP4
INDIRF4
ADDRGP4 pm
INDIRP4
INDIRP4
ADDRLP4 64
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
line 639
;639:		VectorNormalize(oppDir);
ADDRLP4 32
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 641
;640:
;641:		genemy->forceHandExtend = HANDEXTEND_KNOCKDOWN;
ADDRFP4 0
INDIRP4
CNSTI4 1236
ADDP4
CNSTI4 8
ASGNI4
line 642
;642:		genemy->forceHandExtendTime = pm->cmd.serverTime + 1100;
ADDRFP4 0
INDIRP4
CNSTI4 1240
ADDP4
ADDRGP4 pm
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 1100
ADDI4
ASGNI4
line 643
;643:		genemy->forceDodgeAnim = 0; //this toggles between 1 and 0, when it's 1 we should play the get up anim
ADDRFP4 0
INDIRP4
CNSTI4 1248
ADDP4
CNSTI4 0
ASGNI4
line 645
;644:
;645:		genemy->otherKiller = pm->ps->clientNum;
ADDRFP4 0
INDIRP4
CNSTI4 748
ADDP4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
ASGNI4
line 646
;646:		genemy->otherKillerTime = pm->cmd.serverTime + 5000;
ADDRFP4 0
INDIRP4
CNSTI4 752
ADDP4
ADDRGP4 pm
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 5000
ADDI4
ASGNI4
line 647
;647:		genemy->otherKillerDebounceTime = pm->cmd.serverTime + 100;
ADDRFP4 0
INDIRP4
CNSTI4 756
ADDP4
ADDRGP4 pm
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 649
;648:
;649:		genemy->velocity[0] = oppDir[0]*(strength*40);
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
ADDRLP4 32
INDIRF4
CNSTI4 40
ADDRLP4 44
INDIRI4
MULI4
CVIF4 4
MULF4
ASGNF4
line 650
;650:		genemy->velocity[1] = oppDir[1]*(strength*40);
ADDRFP4 0
INDIRP4
CNSTI4 36
ADDP4
ADDRLP4 32+4
INDIRF4
CNSTI4 40
ADDRLP4 44
INDIRI4
MULI4
CVIF4 4
MULF4
ASGNF4
line 651
;651:		genemy->velocity[2] = 100;
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
CNSTF4 1120403456
ASGNF4
line 652
;652:	}
ADDRGP4 $311
JUMPV
LABELV $310
line 654
;653:	else
;654:	{
line 657
;655:		vec3_t oppDir;
;656:
;657:		int strength = 4;
ADDRLP4 44
CNSTI4 4
ASGNI4
line 659
;658:
;659:		VectorSubtract(genemy->origin, pm->ps->origin, oppDir);
ADDRLP4 48
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 52
CNSTI4 20
ASGNI4
ADDRLP4 56
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 32
ADDRLP4 48
INDIRP4
ADDRLP4 52
INDIRI4
ADDP4
INDIRF4
ADDRLP4 56
INDIRP4
INDIRP4
ADDRLP4 52
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 60
CNSTI4 24
ASGNI4
ADDRLP4 32+4
ADDRLP4 48
INDIRP4
ADDRLP4 60
INDIRI4
ADDP4
INDIRF4
ADDRLP4 56
INDIRP4
INDIRP4
ADDRLP4 60
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 64
CNSTI4 28
ASGNI4
ADDRLP4 32+8
ADDRFP4 0
INDIRP4
ADDRLP4 64
INDIRI4
ADDP4
INDIRF4
ADDRGP4 pm
INDIRP4
INDIRP4
ADDRLP4 64
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
line 660
;660:		VectorNormalize(oppDir);
ADDRLP4 32
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 661
;661:		genemy->velocity[0] = oppDir[0]*(strength*40);
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
ADDRLP4 32
INDIRF4
CNSTI4 40
ADDRLP4 44
INDIRI4
MULI4
CVIF4 4
MULF4
ASGNF4
line 662
;662:		genemy->velocity[1] = oppDir[1]*(strength*40);
ADDRFP4 0
INDIRP4
CNSTI4 36
ADDP4
ADDRLP4 32+4
INDIRF4
CNSTI4 40
ADDRLP4 44
INDIRI4
MULI4
CVIF4 4
MULF4
ASGNF4
line 663
;663:		genemy->velocity[2] = 150;
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
CNSTF4 1125515264
ASGNF4
line 665
;664:
;665:		VectorSubtract(pm->ps->origin, genemy->origin, oppDir);
ADDRLP4 68
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 72
CNSTI4 20
ASGNI4
ADDRLP4 76
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 32
ADDRLP4 68
INDIRP4
INDIRP4
ADDRLP4 72
INDIRI4
ADDP4
INDIRF4
ADDRLP4 76
INDIRP4
ADDRLP4 72
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 80
CNSTI4 24
ASGNI4
ADDRLP4 32+4
ADDRLP4 68
INDIRP4
INDIRP4
ADDRLP4 80
INDIRI4
ADDP4
INDIRF4
ADDRLP4 76
INDIRP4
ADDRLP4 80
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 84
CNSTI4 28
ASGNI4
ADDRLP4 32+8
ADDRGP4 pm
INDIRP4
INDIRP4
ADDRLP4 84
INDIRI4
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
ADDRLP4 84
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
line 666
;666:		VectorNormalize(oppDir);
ADDRLP4 32
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 667
;667:		pm->ps->velocity[0] = oppDir[0]*(strength*40);
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ADDRLP4 32
INDIRF4
CNSTI4 40
ADDRLP4 44
INDIRI4
MULI4
CVIF4 4
MULF4
ASGNF4
line 668
;668:		pm->ps->velocity[1] = oppDir[1]*(strength*40);
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 36
ADDP4
ADDRLP4 32+4
INDIRF4
CNSTI4 40
ADDRLP4 44
INDIRI4
MULI4
CVIF4 4
MULF4
ASGNF4
line 669
;669:		pm->ps->velocity[2] = 150;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
CNSTF4 1125515264
ASGNF4
line 671
;670:
;671:		genemy->forceHandExtend = HANDEXTEND_WEAPONREADY;
ADDRFP4 0
INDIRP4
CNSTI4 1236
ADDP4
CNSTI4 6
ASGNI4
line 672
;672:	}
LABELV $311
line 674
;673:
;674:	pm->ps->weaponTime = 0;//pm->ps->torsoTimer;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 44
ADDP4
CNSTI4 0
ASGNI4
line 676
;675:	//The enemy unfortunately has no valid torso animation time at this point, so just use ours
;676:	genemy->weaponTime = 0;
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
CNSTI4 0
ASGNI4
line 678
;677:
;678:	pm->ps->saberLockTime = genemy->saberLockTime = 0;
ADDRLP4 32
CNSTI4 524
ASGNI4
ADDRLP4 36
CNSTI4 0
ASGNI4
ADDRFP4 0
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
ADDRLP4 36
INDIRI4
ASGNI4
ADDRGP4 pm
INDIRP4
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
ADDRLP4 36
INDIRI4
ASGNI4
line 679
;679:	pm->ps->saberLockFrame = genemy->saberLockFrame = 0;
ADDRLP4 40
CNSTI4 532
ASGNI4
ADDRLP4 44
CNSTI4 0
ASGNI4
ADDRFP4 0
INDIRP4
ADDRLP4 40
INDIRI4
ADDP4
ADDRLP4 44
INDIRI4
ASGNI4
ADDRGP4 pm
INDIRP4
INDIRP4
ADDRLP4 40
INDIRI4
ADDP4
ADDRLP4 44
INDIRI4
ASGNI4
line 680
;680:	pm->ps->saberLockEnemy = genemy->saberLockEnemy = 0;
ADDRLP4 48
CNSTI4 528
ASGNI4
ADDRLP4 52
CNSTI4 0
ASGNI4
ADDRFP4 0
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
ADDRLP4 52
INDIRI4
ASGNI4
ADDRGP4 pm
INDIRP4
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
ADDRLP4 52
INDIRI4
ASGNI4
line 682
;681:
;682:	pm->ps->forceHandExtend = HANDEXTEND_WEAPONREADY;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 1236
ADDP4
CNSTI4 6
ASGNI4
line 684
;683:
;684:	PM_AddEvent( EV_JUMP );
CNSTI4 14
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 685
;685:	if ( !victory )
ADDRFP4 4
INDIRI4
CNSTI4 0
NEI4 $321
line 686
;686:	{//no-one won
line 687
;687:		BG_AddPredictableEventToPlayerstate(EV_JUMP, 0, genemy);
CNSTI4 14
ARGI4
CNSTI4 0
ARGI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BG_AddPredictableEventToPlayerstate
CALLV
pop
line 688
;688:	}
ADDRGP4 $322
JUMPV
LABELV $321
line 690
;689:	else
;690:	{
line 691
;691:		if ( Q_irand( 0, 1 ) )
CNSTI4 0
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 56
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 56
INDIRI4
CNSTI4 0
EQI4 $323
line 692
;692:		{
line 693
;693:			BG_AddPredictableEventToPlayerstate(EV_JUMP, PM_irand_timesync( 0, 75 ), genemy);
CNSTI4 0
ARGI4
CNSTI4 75
ARGI4
ADDRLP4 60
ADDRGP4 PM_irand_timesync
CALLI4
ASGNI4
CNSTI4 14
ARGI4
ADDRLP4 60
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BG_AddPredictableEventToPlayerstate
CALLV
pop
line 694
;694:		}
LABELV $323
line 695
;695:	}
LABELV $322
line 696
;696:}
LABELV $293
endproc PM_SaberLockBreak 88 16
export PM_SaberLocked
proc PM_SaberLocked 84 16
line 700
;697:
;698:extern qboolean ValidAnimFileIndex ( int index );
;699:void PM_SaberLocked( void )
;700:{
line 701
;701:	int	remaining = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 708
;702:	/*
;703:	if ( pm->ps->weaponTime )
;704:	{//can't attack yet
;705:		return;
;706:	}
;707:	*/
;708:	playerState_t *genemy = pm->bgClients[pm->ps->saberLockEnemy];
ADDRLP4 8
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
INDIRP4
CNSTI4 528
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
INDIRP4
CNSTI4 248
ADDP4
ADDP4
INDIRP4
ASGNP4
line 709
;709:	if ( !genemy )
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $326
line 710
;710:	{
line 711
;711:		return;
ADDRGP4 $325
JUMPV
LABELV $326
line 713
;712:	}
;713:	if ( ( (pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_BF2LOCK ||
ADDRLP4 12
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 100
ADDP4
INDIRI4
CNSTI4 -2049
BANDI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 538
EQI4 $332
ADDRLP4 12
INDIRI4
CNSTI4 541
EQI4 $332
ADDRLP4 12
INDIRI4
CNSTI4 548
EQI4 $332
ADDRLP4 12
INDIRI4
CNSTI4 549
NEI4 $328
LABELV $332
ADDRLP4 16
ADDRLP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRI4
CNSTI4 -2049
BANDI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 538
EQI4 $335
ADDRLP4 16
INDIRI4
CNSTI4 541
EQI4 $335
ADDRLP4 16
INDIRI4
CNSTI4 548
EQI4 $335
ADDRLP4 16
INDIRI4
CNSTI4 549
NEI4 $328
LABELV $335
line 722
;714:			(pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_BF1LOCK ||
;715:			(pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_CWCIRCLELOCK ||
;716:			(pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_CCWCIRCLELOCK )
;717:		&& ( (genemy->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_BF2LOCK ||
;718:			(genemy->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_BF1LOCK ||
;719:			(genemy->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_CWCIRCLELOCK ||
;720:			(genemy->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_CCWCIRCLELOCK )
;721:		)
;722:	{
line 723
;723:		float dist = 0;
ADDRLP4 20
CNSTF4 0
ASGNF4
line 725
;724:
;725:		pm->ps->torsoTimer = 0;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 96
ADDP4
CNSTI4 0
ASGNI4
line 726
;726:		pm->ps->weaponTime = 0;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 44
ADDP4
CNSTI4 0
ASGNI4
line 727
;727:		genemy->torsoTimer = 0;
ADDRLP4 0
INDIRP4
CNSTI4 96
ADDP4
CNSTI4 0
ASGNI4
line 728
;728:		genemy->weaponTime = 0;
ADDRLP4 0
INDIRP4
CNSTI4 44
ADDP4
CNSTI4 0
ASGNI4
line 730
;729:
;730:		dist = DistanceSquared(pm->ps->origin,genemy->origin);
ADDRLP4 24
CNSTI4 20
ASGNI4
ADDRGP4 pm
INDIRP4
INDIRP4
ADDRLP4 24
INDIRI4
ADDP4
ARGP4
ADDRLP4 0
INDIRP4
ADDRLP4 24
INDIRI4
ADDP4
ARGP4
ADDRLP4 28
ADDRGP4 DistanceSquared
CALLF4
ASGNF4
ADDRLP4 20
ADDRLP4 28
INDIRF4
ASGNF4
line 731
;731:		if ( dist < 64 || dist > 6400 )//( dist < 128 || dist > 2304 )
ADDRLP4 32
ADDRLP4 20
INDIRF4
ASGNF4
ADDRLP4 32
INDIRF4
CNSTF4 1115684864
LTF4 $338
ADDRLP4 32
INDIRF4
CNSTF4 1170735104
LEF4 $336
LABELV $338
line 732
;732:		{//between 8 and 80 from each other//was 16 and 48
line 733
;733:			PM_SaberLockBreak( genemy, qfalse );
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 PM_SaberLockBreak
CALLV
pop
line 734
;734:			return;
ADDRGP4 $325
JUMPV
LABELV $336
line 736
;735:		}
;736:		if ( (pm->cmd.buttons & BUTTON_ATTACK) || pm->ps->saberLockAdvance )
ADDRLP4 36
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 40
CNSTI4 0
ASGNI4
ADDRLP4 36
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
CNSTI4 1
BANDI4
ADDRLP4 40
INDIRI4
NEI4 $341
ADDRLP4 36
INDIRP4
INDIRP4
CNSTI4 540
ADDP4
INDIRI4
ADDRLP4 40
INDIRI4
EQI4 $329
LABELV $341
line 737
;737:		{//holding attack
line 738
;738:			if (pm->ps->saberLockAdvance)
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 540
ADDP4
INDIRI4
CNSTI4 0
EQI4 $325
line 739
;739:			{//tapping
line 743
;740:				animation_t *anim;
;741:				float		currentFrame;
;742:				int			curFrame;
;743:				int			strength = 1;
ADDRLP4 44
CNSTI4 1
ASGNI4
line 745
;744:
;745:				pm->ps->saberLockAdvance = qfalse;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 540
ADDP4
CNSTI4 0
ASGNI4
line 747
;746:
;747:				anim = &pm->animations[pm->ps->torsoAnim&~ANIM_TOGGLEBIT];
ADDRLP4 60
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 48
CNSTI4 28
ADDRLP4 60
INDIRP4
INDIRP4
CNSTI4 100
ADDP4
INDIRI4
CNSTI4 -2049
BANDI4
MULI4
ADDRLP4 60
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
ADDP4
ASGNP4
line 749
;748:	
;749:				currentFrame = pm->ps->saberLockFrame;
ADDRLP4 56
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
CVIF4 4
ASGNF4
line 751
;750:
;751:				strength = pm->ps->fd.forcePowerLevel[FP_SABERATTACK]+1;
ADDRLP4 44
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 992
ADDP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 753
;752:
;753:				if ( (pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_CCWCIRCLELOCK ||
ADDRLP4 64
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 100
ADDP4
INDIRI4
CNSTI4 -2049
BANDI4
ASGNI4
ADDRLP4 64
INDIRI4
CNSTI4 549
EQI4 $346
ADDRLP4 64
INDIRI4
CNSTI4 538
NEI4 $344
LABELV $346
line 755
;754:					(pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_BF2LOCK )
;755:				{
line 756
;756:					curFrame = floor( currentFrame )-strength;
ADDRLP4 56
INDIRF4
ARGF4
ADDRLP4 68
ADDRGP4 floor
CALLF4
ASGNF4
ADDRLP4 52
ADDRLP4 68
INDIRF4
ADDRLP4 44
INDIRI4
CVIF4 4
SUBF4
CVFI4 4
ASGNI4
line 758
;757:					//drop my frame one
;758:					if ( curFrame <= anim->firstFrame )
ADDRLP4 52
INDIRI4
ADDRLP4 48
INDIRP4
INDIRI4
GTI4 $347
line 759
;759:					{//I won!  Break out
line 760
;760:						PM_SaberLockBreak( genemy, qtrue );
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 PM_SaberLockBreak
CALLV
pop
line 761
;761:						return;
ADDRGP4 $325
JUMPV
LABELV $347
line 764
;762:					}
;763:					else
;764:					{
line 765
;765:						PM_SetAnimFrame( pm->ps, curFrame, qtrue, qtrue );
ADDRGP4 pm
INDIRP4
INDIRP4
ARGP4
ADDRLP4 52
INDIRI4
ARGI4
ADDRLP4 72
CNSTI4 1
ASGNI4
ADDRLP4 72
INDIRI4
ARGI4
ADDRLP4 72
INDIRI4
ARGI4
ADDRGP4 PM_SetAnimFrame
CALLV
pop
line 766
;766:						remaining = curFrame-anim->firstFrame;
ADDRLP4 4
ADDRLP4 52
INDIRI4
ADDRLP4 48
INDIRP4
INDIRI4
SUBI4
ASGNI4
line 767
;767:					}
line 768
;768:				}
ADDRGP4 $345
JUMPV
LABELV $344
line 770
;769:				else
;770:				{
line 771
;771:					curFrame = ceil( currentFrame )+strength;
ADDRLP4 56
INDIRF4
ARGF4
ADDRLP4 68
ADDRGP4 ceil
CALLF4
ASGNF4
ADDRLP4 52
ADDRLP4 68
INDIRF4
ADDRLP4 44
INDIRI4
CVIF4 4
ADDF4
CVFI4 4
ASGNI4
line 773
;772:					//advance my frame one
;773:					if ( curFrame >= anim->firstFrame+anim->numFrames )
ADDRLP4 72
ADDRLP4 48
INDIRP4
ASGNP4
ADDRLP4 52
INDIRI4
ADDRLP4 72
INDIRP4
INDIRI4
ADDRLP4 72
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ADDI4
LTI4 $349
line 774
;774:					{//I won!  Break out
line 775
;775:						PM_SaberLockBreak( genemy, qtrue );
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 PM_SaberLockBreak
CALLV
pop
line 776
;776:						return;
ADDRGP4 $325
JUMPV
LABELV $349
line 779
;777:					}
;778:					else
;779:					{
line 780
;780:						PM_SetAnimFrame( pm->ps, curFrame, qtrue, qtrue );
ADDRGP4 pm
INDIRP4
INDIRP4
ARGP4
ADDRLP4 52
INDIRI4
ARGI4
ADDRLP4 76
CNSTI4 1
ASGNI4
ADDRLP4 76
INDIRI4
ARGI4
ADDRLP4 76
INDIRI4
ARGI4
ADDRGP4 PM_SetAnimFrame
CALLV
pop
line 781
;781:						remaining = anim->firstFrame+anim->numFrames-curFrame;
ADDRLP4 80
ADDRLP4 48
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 80
INDIRP4
INDIRI4
ADDRLP4 80
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ADDI4
ADDRLP4 52
INDIRI4
SUBI4
ASGNI4
line 782
;782:					}
line 783
;783:				}
LABELV $345
line 784
;784:				if ( !Q_irand( 0, 2 ) )
CNSTI4 0
ARGI4
CNSTI4 2
ARGI4
ADDRLP4 68
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 68
INDIRI4
CNSTI4 0
NEI4 $343
line 785
;785:				{
line 786
;786:					PM_AddEvent( EV_JUMP );
CNSTI4 14
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 787
;787:				}
line 788
;788:			}
line 790
;789:			else
;790:			{
line 791
;791:				return;
LABELV $343
line 793
;792:			}
;793:			if( 1/*ValidAnimFileIndex( genemy->client->clientInfo.animFileIndex )*/ )
line 794
;794:			{
line 796
;795:				animation_t *anim;
;796:				anim = &pm->animations[(genemy->torsoAnim&~ANIM_TOGGLEBIT)];
ADDRLP4 44
CNSTI4 28
ADDRLP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRI4
CNSTI4 -2049
BANDI4
MULI4
ADDRGP4 pm
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
ADDP4
ASGNP4
line 798
;797:
;798:				if ( (genemy->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_CWCIRCLELOCK ||
ADDRLP4 48
ADDRLP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRI4
CNSTI4 -2049
BANDI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 548
EQI4 $357
ADDRLP4 48
INDIRI4
CNSTI4 541
NEI4 $355
LABELV $357
line 800
;799:					(genemy->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_BF1LOCK )
;800:				{
line 801
;801:					if ( !Q_irand( 0, 2 ) )
CNSTI4 0
ARGI4
CNSTI4 2
ARGI4
ADDRLP4 52
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 52
INDIRI4
CNSTI4 0
NEI4 $358
line 802
;802:					{
line 803
;803:						BG_AddPredictableEventToPlayerstate(EV_PAIN, floor((float)80/100*100.0f), genemy);
CNSTF4 1117782016
ARGF4
ADDRLP4 56
ADDRGP4 floor
CALLF4
ASGNF4
CNSTI4 77
ARGI4
ADDRLP4 56
INDIRF4
CVFI4 4
ARGI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 BG_AddPredictableEventToPlayerstate
CALLV
pop
line 804
;804:					}
LABELV $358
line 805
;805:					PM_SetAnimFrame( genemy, anim->firstFrame+remaining, qtrue, qtrue );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 44
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ARGI4
ADDRLP4 56
CNSTI4 1
ASGNI4
ADDRLP4 56
INDIRI4
ARGI4
ADDRLP4 56
INDIRI4
ARGI4
ADDRGP4 PM_SetAnimFrame
CALLV
pop
line 806
;806:				}
ADDRGP4 $329
JUMPV
LABELV $355
line 808
;807:				else
;808:				{
line 809
;809:					PM_SetAnimFrame( genemy, anim->firstFrame+anim->numFrames-remaining, qtrue, qtrue );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 52
ADDRLP4 44
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
INDIRI4
ADDRLP4 52
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ADDI4
ADDRLP4 4
INDIRI4
SUBI4
ARGI4
ADDRLP4 56
CNSTI4 1
ASGNI4
ADDRLP4 56
INDIRI4
ARGI4
ADDRLP4 56
INDIRI4
ARGI4
ADDRGP4 PM_SetAnimFrame
CALLV
pop
line 810
;810:				}
line 811
;811:			}
line 812
;812:		}
line 813
;813:	}
ADDRGP4 $329
JUMPV
LABELV $328
line 815
;814:	else
;815:	{//something broke us out of it
line 816
;816:		PM_SaberLockBreak( genemy, qfalse );
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 PM_SaberLockBreak
CALLV
pop
line 817
;817:	}
LABELV $329
line 818
;818:}
LABELV $325
endproc PM_SaberLocked 84 16
export PM_SaberInBrokenParry
proc PM_SaberInBrokenParry 4 0
line 821
;819:
;820:qboolean PM_SaberInBrokenParry( int move )
;821:{
line 822
;822:	if ( move >= LS_H1_T_ && move <= LS_H1_BL )
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 97
LTI4 $361
ADDRLP4 0
INDIRI4
CNSTI4 102
GTI4 $361
line 823
;823:	{
line 824
;824:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $360
JUMPV
LABELV $361
line 826
;825:	}
;826:	return qfalse;
CNSTI4 0
RETI4
LABELV $360
endproc PM_SaberInBrokenParry 4 0
export PM_BrokenParryForParry
proc PM_BrokenParryForParry 12 0
line 831
;827:}
;828:
;829:
;830:int PM_BrokenParryForParry( int move )
;831:{
line 834
;832:	//FIXME: need actual anims for this
;833:	//FIXME: need to know which side of the saber was hit!  For now, we presume the saber gets knocked away from the center
;834:	switch ( move )
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 4
CNSTI4 1
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
EQI4 $371
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $364
LABELV $372
ADDRLP4 8
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 108
LTI4 $364
ADDRLP4 8
INDIRI4
CNSTI4 112
GTI4 $364
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $373-432
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $373
address $366
address $367
address $368
address $369
address $370
code
line 835
;835:	{
LABELV $366
line 837
;836:	case LS_PARRY_UP:
;837:		return LS_H1_T_;
CNSTI4 97
RETI4
ADDRGP4 $363
JUMPV
line 838
;838:		break;
LABELV $367
line 840
;839:	case LS_PARRY_UR:
;840:		return LS_H1_TR;
CNSTI4 98
RETI4
ADDRGP4 $363
JUMPV
line 841
;841:		break;
LABELV $368
line 843
;842:	case LS_PARRY_UL:
;843:		return LS_H1_TL;
CNSTI4 99
RETI4
ADDRGP4 $363
JUMPV
line 844
;844:		break;
LABELV $369
line 846
;845:	case LS_PARRY_LR:
;846:		return LS_H1_BL;
CNSTI4 102
RETI4
ADDRGP4 $363
JUMPV
line 847
;847:		break;
LABELV $370
line 849
;848:	case LS_PARRY_LL:
;849:		return LS_H1_BR;
CNSTI4 100
RETI4
ADDRGP4 $363
JUMPV
line 850
;850:		break;
LABELV $371
line 852
;851:	case LS_READY:
;852:		return LS_H1_B_;//???
CNSTI4 101
RETI4
ADDRGP4 $363
JUMPV
line 853
;853:		break;
LABELV $364
line 855
;854:	}
;855:	return LS_NONE;
CNSTI4 0
RETI4
LABELV $363
endproc PM_BrokenParryForParry 12 0
lit
align 4
LABELV $376
byte 4 3245342720
byte 4 3245342720
byte 4 3238002688
align 4
LABELV $377
byte 4 1097859072
byte 4 1097859072
byte 4 1090519040
export PM_CanBackstab
code
proc PM_CanBackstab 1152 28
line 861
;856:}
;857:
;858:#define BACK_STAB_DISTANCE 128//64
;859:
;860:qboolean PM_CanBackstab(void)
;861:{
line 865
;862:	trace_t tr;
;863:	vec3_t flatAng;
;864:	vec3_t fwd, back;
;865:	vec3_t trmins = {-15, -15, -8};
ADDRLP4 1116
ADDRGP4 $376
INDIRB
ASGNB 12
line 866
;866:	vec3_t trmaxs = {15, 15, 8};
ADDRLP4 1128
ADDRGP4 $377
INDIRB
ASGNB 12
line 868
;867:
;868:	VectorCopy(pm->ps->viewangles, flatAng);
ADDRLP4 1104
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 156
ADDP4
INDIRB
ASGNB 12
line 869
;869:	flatAng[PITCH] = 0;
ADDRLP4 1104
CNSTF4 0
ASGNF4
line 871
;870:
;871:	AngleVectors(flatAng, fwd, 0, 0);
ADDRLP4 1104
ARGP4
ADDRLP4 1080
ARGP4
ADDRLP4 1140
CNSTP4 0
ASGNP4
ADDRLP4 1140
INDIRP4
ARGP4
ADDRLP4 1140
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 873
;872:
;873:	back[0] = pm->ps->origin[0] - fwd[0]*BACK_STAB_DISTANCE;
ADDRLP4 1092
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
CNSTF4 1124073472
ADDRLP4 1080
INDIRF4
MULF4
SUBF4
ASGNF4
line 874
;874:	back[1] = pm->ps->origin[1] - fwd[1]*BACK_STAB_DISTANCE;
ADDRLP4 1092+4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
CNSTF4 1124073472
ADDRLP4 1080+4
INDIRF4
MULF4
SUBF4
ASGNF4
line 875
;875:	back[2] = pm->ps->origin[2] - fwd[2]*BACK_STAB_DISTANCE;
ADDRLP4 1092+8
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
CNSTF4 1124073472
ADDRLP4 1080+8
INDIRF4
MULF4
SUBF4
ASGNF4
line 877
;876:
;877:	pm->trace(&tr, pm->ps->origin, trmins, trmaxs, back, pm->ps->clientNum, MASK_PLAYERSOLID);
ADDRLP4 0
ARGP4
ADDRLP4 1144
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 1148
ADDRLP4 1144
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 1148
INDIRP4
CNSTI4 20
ADDP4
ARGP4
ADDRLP4 1116
ARGP4
ADDRLP4 1128
ARGP4
ADDRLP4 1092
ARGP4
ADDRLP4 1148
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
ARGI4
CNSTI4 273
ARGI4
ADDRLP4 1144
INDIRP4
CNSTI4 240
ADDP4
INDIRP4
CALLV
pop
line 879
;878:
;879:	if (tr.fraction != 1.0 && tr.entityNum >= 0 && tr.entityNum < MAX_CLIENTS)
ADDRLP4 0+8
INDIRF4
CNSTF4 1065353216
EQF4 $382
ADDRLP4 0+52
INDIRI4
CNSTI4 0
LTI4 $382
ADDRLP4 0+52
INDIRI4
CNSTI4 32
GEI4 $382
line 880
;880:	{ //We don't have real entity access here so we can't do an indepth check. But if it's a client and it's behind us, I guess that's reason enough to stab backward
line 881
;881:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $375
JUMPV
LABELV $382
line 884
;882:	}
;883:
;884:	return qfalse;
CNSTI4 0
RETI4
LABELV $375
endproc PM_CanBackstab 1152 28
export PM_SaberFlipOverAttackMove
proc PM_SaberFlipOverAttackMove 56 16
line 888
;885:}
;886:
;887:saberMoveName_t PM_SaberFlipOverAttackMove(trace_t *tr)
;888:{
line 891
;889:	//FIXME: check above for room enough to jump!
;890:	vec3_t fwdAngles, jumpFwd;
;891:	float zDiff = 0;
ADDRLP4 24
CNSTF4 0
ASGNF4
line 894
;892:	playerState_t *psData;
;893:
;894:	VectorCopy( pm->ps->viewangles, fwdAngles );
ADDRLP4 0
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 156
ADDP4
INDIRB
ASGNB 12
line 895
;895:	fwdAngles[PITCH] = fwdAngles[ROLL] = 0;
ADDRLP4 32
CNSTF4 0
ASGNF4
ADDRLP4 0+8
ADDRLP4 32
INDIRF4
ASGNF4
ADDRLP4 0
ADDRLP4 32
INDIRF4
ASGNF4
line 896
;896:	AngleVectors( fwdAngles, jumpFwd, NULL, NULL );
ADDRLP4 0
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 36
CNSTP4 0
ASGNP4
ADDRLP4 36
INDIRP4
ARGP4
ADDRLP4 36
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 897
;897:	VectorScale( jumpFwd, /*100*/50, pm->ps->velocity );
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
CNSTF4 1112014848
ADDRLP4 12
INDIRF4
MULF4
ASGNF4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 36
ADDP4
CNSTF4 1112014848
ADDRLP4 12+4
INDIRF4
MULF4
ASGNF4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
CNSTF4 1112014848
ADDRLP4 12+8
INDIRF4
MULF4
ASGNF4
line 898
;898:	pm->ps->velocity[2] = 400;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
CNSTF4 1137180672
ASGNF4
line 900
;899:
;900:	psData = pm->bgClients[tr->entityNum];
ADDRLP4 28
ADDRFP4 0
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 pm
INDIRP4
CNSTI4 248
ADDP4
ADDP4
INDIRP4
ASGNP4
line 902
;901:
;902:	pm->ps->velocity[2] *= 1;//(pm->gent->enemy->maxs[2]-pm->gent->enemy->mins[2])/64.0f;
ADDRLP4 40
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
ASGNP4
ADDRLP4 40
INDIRP4
CNSTF4 1065353216
ADDRLP4 40
INDIRP4
INDIRF4
MULF4
ASGNF4
line 905
;903:
;904:	//go higher for enemies higher than you, lower for those lower than you
;905:	if (psData)
ADDRLP4 28
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $391
line 906
;906:	{
line 907
;907:		zDiff = psData->origin[2] - pm->ps->origin[2];
ADDRLP4 44
CNSTI4 28
ASGNI4
ADDRLP4 24
ADDRLP4 28
INDIRP4
ADDRLP4 44
INDIRI4
ADDP4
INDIRF4
ADDRGP4 pm
INDIRP4
INDIRP4
ADDRLP4 44
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
line 908
;908:	}
ADDRGP4 $392
JUMPV
LABELV $391
line 910
;909:	else
;910:	{
line 911
;911:		zDiff = 0;
ADDRLP4 24
CNSTF4 0
ASGNF4
line 912
;912:	}
LABELV $392
line 913
;913:	pm->ps->velocity[2] += (zDiff)*1.5f;
ADDRLP4 44
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
ASGNP4
ADDRLP4 44
INDIRP4
ADDRLP4 44
INDIRP4
INDIRF4
CNSTF4 1069547520
ADDRLP4 24
INDIRF4
MULF4
ADDF4
ASGNF4
line 916
;914:
;915:	//clamp to decent-looking values
;916:	if ( zDiff <= 0 && pm->ps->velocity[2] < 200 )
ADDRLP4 24
INDIRF4
CNSTF4 0
GTF4 $393
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
CNSTF4 1128792064
GEF4 $393
line 917
;917:	{//if we're on same level, don't let me jump so low, I clip into the ground
line 918
;918:		pm->ps->velocity[2] = 200;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
CNSTF4 1128792064
ASGNF4
line 919
;919:	}
ADDRGP4 $394
JUMPV
LABELV $393
line 920
;920:	else if ( pm->ps->velocity[2] < 100 )
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
CNSTF4 1120403456
GEF4 $395
line 921
;921:	{
line 922
;922:		pm->ps->velocity[2] = 100;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
CNSTF4 1120403456
ASGNF4
line 923
;923:	}
ADDRGP4 $396
JUMPV
LABELV $395
line 924
;924:	else if ( pm->ps->velocity[2] > 400 )
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
CNSTF4 1137180672
LEF4 $397
line 925
;925:	{
line 926
;926:		pm->ps->velocity[2] = 400;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
CNSTF4 1137180672
ASGNF4
line 927
;927:	}
LABELV $397
LABELV $396
LABELV $394
line 929
;928:
;929:	pm->ps->fd.forceJumpZStart = pm->ps->origin[2];//so we don't take damage if we land at same height
ADDRLP4 48
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 48
INDIRP4
CNSTI4 1080
ADDP4
ADDRLP4 48
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ASGNF4
line 931
;930:
;931:	PM_AddEvent( EV_JUMP );
CNSTI4 14
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 932
;932:	pm->ps->fd.forceJumpSound = 1;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 1088
ADDP4
CNSTI4 1
ASGNI4
line 933
;933:	pm->cmd.upmove = 0;
ADDRGP4 pm
INDIRP4
CNSTI4 30
ADDP4
CNSTI1 0
ASGNI1
line 935
;934:
;935:	if ( Q_irand( 0, 1 ) )
CNSTI4 0
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 52
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 52
INDIRI4
CNSTI4 0
EQI4 $399
line 936
;936:	{
line 937
;937:		return LS_A_FLIP_STAB;
CNSTI4 16
RETI4
ADDRGP4 $387
JUMPV
LABELV $399
line 940
;938:	}
;939:	else
;940:	{
line 941
;941:		return LS_A_FLIP_SLASH;
CNSTI4 17
RETI4
LABELV $387
endproc PM_SaberFlipOverAttackMove 56 16
lit
align 4
LABELV $402
byte 4 3245342720
byte 4 3245342720
byte 4 3238002688
align 4
LABELV $403
byte 4 1097859072
byte 4 1097859072
byte 4 1090519040
export PM_SomeoneInFront
code
proc PM_SomeoneInFront 80 28
line 948
;942:	}
;943:}
;944:
;945:#define FLIPHACK_DISTANCE 200
;946:
;947:qboolean PM_SomeoneInFront(trace_t *tr)
;948:{ //Also a very simplified version of the sp counterpart
line 951
;949:	vec3_t flatAng;
;950:	vec3_t fwd, back;
;951:	vec3_t trmins = {-15, -15, -8};
ADDRLP4 36
ADDRGP4 $402
INDIRB
ASGNB 12
line 952
;952:	vec3_t trmaxs = {15, 15, 8};
ADDRLP4 48
ADDRGP4 $403
INDIRB
ASGNB 12
line 954
;953:
;954:	VectorCopy(pm->ps->viewangles, flatAng);
ADDRLP4 24
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 156
ADDP4
INDIRB
ASGNB 12
line 955
;955:	flatAng[PITCH] = 0;
ADDRLP4 24
CNSTF4 0
ASGNF4
line 957
;956:
;957:	AngleVectors(flatAng, fwd, 0, 0);
ADDRLP4 24
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 60
CNSTP4 0
ASGNP4
ADDRLP4 60
INDIRP4
ARGP4
ADDRLP4 60
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 959
;958:
;959:	back[0] = pm->ps->origin[0] + fwd[0]*FLIPHACK_DISTANCE;
ADDRLP4 12
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
CNSTF4 1128792064
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
line 960
;960:	back[1] = pm->ps->origin[1] + fwd[1]*FLIPHACK_DISTANCE;
ADDRLP4 12+4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
CNSTF4 1128792064
ADDRLP4 0+4
INDIRF4
MULF4
ADDF4
ASGNF4
line 961
;961:	back[2] = pm->ps->origin[2] + fwd[2]*FLIPHACK_DISTANCE;
ADDRLP4 12+8
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
CNSTF4 1128792064
ADDRLP4 0+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 963
;962:
;963:	pm->trace(tr, pm->ps->origin, trmins, trmaxs, back, pm->ps->clientNum, MASK_PLAYERSOLID);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 64
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 68
ADDRLP4 64
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 68
INDIRP4
CNSTI4 20
ADDP4
ARGP4
ADDRLP4 36
ARGP4
ADDRLP4 48
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 68
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
ARGI4
CNSTI4 273
ARGI4
ADDRLP4 64
INDIRP4
CNSTI4 240
ADDP4
INDIRP4
CALLV
pop
line 965
;964:
;965:	if (tr->fraction != 1.0 && tr->entityNum >= 0 && tr->entityNum < MAX_CLIENTS)
ADDRLP4 72
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 72
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
CNSTF4 1065353216
EQF4 $408
ADDRLP4 76
ADDRLP4 72
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
ASGNI4
ADDRLP4 76
INDIRI4
CNSTI4 0
LTI4 $408
ADDRLP4 76
INDIRI4
CNSTI4 32
GEI4 $408
line 966
;966:	{
line 967
;967:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $401
JUMPV
LABELV $408
line 970
;968:	}
;969:
;970:	return qfalse;
CNSTI4 0
RETI4
LABELV $401
endproc PM_SomeoneInFront 80 28
export PM_SaberLungeAttackMove
proc PM_SaberLungeAttackMove 32 16
line 974
;971:}
;972:
;973:saberMoveName_t PM_SaberLungeAttackMove( void )
;974:{
line 977
;975:	vec3_t fwdAngles, jumpFwd;
;976:
;977:	VectorCopy( pm->ps->viewangles, fwdAngles );
ADDRLP4 0
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 156
ADDP4
INDIRB
ASGNB 12
line 978
;978:	fwdAngles[PITCH] = fwdAngles[ROLL] = 0;
ADDRLP4 24
CNSTF4 0
ASGNF4
ADDRLP4 0+8
ADDRLP4 24
INDIRF4
ASGNF4
ADDRLP4 0
ADDRLP4 24
INDIRF4
ASGNF4
line 980
;979:	//do the lunge
;980:	AngleVectors( fwdAngles, jumpFwd, NULL, NULL );
ADDRLP4 0
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 28
CNSTP4 0
ASGNP4
ADDRLP4 28
INDIRP4
ARGP4
ADDRLP4 28
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 981
;981:	VectorScale( jumpFwd, 150, pm->ps->velocity );
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
CNSTF4 1125515264
ADDRLP4 12
INDIRF4
MULF4
ASGNF4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 36
ADDP4
CNSTF4 1125515264
ADDRLP4 12+4
INDIRF4
MULF4
ASGNF4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
CNSTF4 1125515264
ADDRLP4 12+8
INDIRF4
MULF4
ASGNF4
line 983
;982:	//pm->ps->velocity[2] = 50;
;983:	PM_AddEvent( EV_JUMP );
CNSTI4 14
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 985
;984:
;985:	return LS_A_LUNGE;
CNSTI4 14
RETI4
LABELV $410
endproc PM_SaberLungeAttackMove 32 16
export PM_SaberJumpAttackMove
proc PM_SaberJumpAttackMove 36 16
line 989
;986:}
;987:
;988:saberMoveName_t PM_SaberJumpAttackMove( void )
;989:{
line 992
;990:	vec3_t fwdAngles, jumpFwd;
;991:
;992:	VectorCopy( pm->ps->viewangles, fwdAngles );
ADDRLP4 0
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 156
ADDP4
INDIRB
ASGNB 12
line 993
;993:	fwdAngles[PITCH] = fwdAngles[ROLL] = 0;
ADDRLP4 24
CNSTF4 0
ASGNF4
ADDRLP4 0+8
ADDRLP4 24
INDIRF4
ASGNF4
ADDRLP4 0
ADDRLP4 24
INDIRF4
ASGNF4
line 994
;994:	AngleVectors( fwdAngles, jumpFwd, NULL, NULL );
ADDRLP4 0
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 28
CNSTP4 0
ASGNP4
ADDRLP4 28
INDIRP4
ARGP4
ADDRLP4 28
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 995
;995:	VectorScale( jumpFwd, /*200*/300, pm->ps->velocity );
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
CNSTF4 1133903872
ADDRLP4 12
INDIRF4
MULF4
ASGNF4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 36
ADDP4
CNSTF4 1133903872
ADDRLP4 12+4
INDIRF4
MULF4
ASGNF4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
CNSTF4 1133903872
ADDRLP4 12+8
INDIRF4
MULF4
ASGNF4
line 996
;996:	pm->ps->velocity[2] = 280;//180;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
CNSTF4 1133248512
ASGNF4
line 997
;997:	pm->ps->fd.forceJumpZStart = pm->ps->origin[2];//so we don't take damage if we land at same height
ADDRLP4 32
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CNSTI4 1080
ADDP4
ADDRLP4 32
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ASGNF4
line 999
;998:
;999:	PM_AddEvent( EV_JUMP );
CNSTI4 14
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 1000
;1000:	pm->ps->fd.forceJumpSound = 1;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 1088
ADDP4
CNSTI4 1
ASGNI4
line 1001
;1001:	pm->cmd.upmove = 0;
ADDRGP4 pm
INDIRP4
CNSTI4 30
ADDP4
CNSTI1 0
ASGNI1
line 1003
;1002:
;1003:	return LS_A_JUMP_T__B_;
CNSTI4 15
RETI4
LABELV $414
endproc PM_SaberJumpAttackMove 36 16
export PM_GroundDistance
proc PM_GroundDistance 1108 28
line 1007
;1004:}
;1005:
;1006:float PM_GroundDistance(void)
;1007:{
line 1011
;1008:	trace_t tr;
;1009:	vec3_t down;
;1010:
;1011:	VectorCopy(pm->ps->origin, down);
ADDRLP4 0
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 1013
;1012:
;1013:	down[2] -= 4096;
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CNSTF4 1166016512
SUBF4
ASGNF4
line 1015
;1014:
;1015:	pm->trace(&tr, pm->ps->origin, pm->mins, pm->maxs, down, pm->ps->clientNum, MASK_SOLID);
ADDRLP4 12
ARGP4
ADDRLP4 1092
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 1096
ADDRLP4 1092
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 1096
INDIRP4
CNSTI4 20
ADDP4
ARGP4
ADDRLP4 1092
INDIRP4
CNSTI4 188
ADDP4
ARGP4
ADDRLP4 1092
INDIRP4
CNSTI4 200
ADDP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 1096
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 1092
INDIRP4
CNSTI4 240
ADDP4
INDIRP4
CALLV
pop
line 1017
;1016:
;1017:	VectorSubtract(pm->ps->origin, tr.endpos, down);
ADDRLP4 1100
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 1100
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 12+12
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 1100
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 12+12+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+8
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 12+12+8
INDIRF4
SUBF4
ASGNF4
line 1019
;1018:
;1019:	return VectorLength(down);
ADDRLP4 0
ARGP4
ADDRLP4 1104
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 1104
INDIRF4
RETF4
LABELV $418
endproc PM_GroundDistance 1108 28
export PM_SaberAttackForMovement
proc PM_SaberAttackForMovement 1104 8
line 1023
;1020:}
;1021:
;1022:saberMoveName_t PM_SaberAttackForMovement(saberMoveName_t curmove)
;1023:{
line 1024
;1024:	saberMoveName_t newmove = -1;
ADDRLP4 0
CNSTI4 -1
ASGNI4
line 1026
;1025:
;1026:	if ( pm->cmd.rightmove > 0 )
ADDRGP4 pm
INDIRP4
CNSTI4 29
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
LEI4 $428
line 1027
;1027:	{//moving right
line 1028
;1028:		if ( pm->cmd.forwardmove > 0 )
ADDRGP4 pm
INDIRP4
CNSTI4 28
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
LEI4 $430
line 1029
;1029:		{//forward right = TL2BR slash
line 1030
;1030:			newmove = LS_A_TL2BR;
ADDRLP4 0
CNSTI4 4
ASGNI4
line 1031
;1031:		}
ADDRGP4 $429
JUMPV
LABELV $430
line 1032
;1032:		else if ( pm->cmd.forwardmove < 0 )
ADDRGP4 pm
INDIRP4
CNSTI4 28
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
GEI4 $432
line 1033
;1033:		{//backward right = BL2TR uppercut
line 1034
;1034:			newmove = LS_A_BL2TR;
ADDRLP4 0
CNSTI4 6
ASGNI4
line 1035
;1035:		}
ADDRGP4 $429
JUMPV
LABELV $432
line 1037
;1036:		else
;1037:		{//just right is a left slice
line 1038
;1038:			newmove = LS_A_L2R;
ADDRLP4 0
CNSTI4 5
ASGNI4
line 1039
;1039:		}
line 1040
;1040:	}
ADDRGP4 $429
JUMPV
LABELV $428
line 1041
;1041:	else if ( pm->cmd.rightmove < 0 )
ADDRGP4 pm
INDIRP4
CNSTI4 29
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
GEI4 $434
line 1042
;1042:	{//moving left
line 1043
;1043:		if ( pm->cmd.forwardmove > 0 )
ADDRGP4 pm
INDIRP4
CNSTI4 28
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
LEI4 $436
line 1044
;1044:		{//forward left = TR2BL slash
line 1045
;1045:			newmove = LS_A_TR2BL;
ADDRLP4 0
CNSTI4 9
ASGNI4
line 1046
;1046:		}
ADDRGP4 $435
JUMPV
LABELV $436
line 1047
;1047:		else if ( pm->cmd.forwardmove < 0 )
ADDRGP4 pm
INDIRP4
CNSTI4 28
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
GEI4 $438
line 1048
;1048:		{//backward left = BR2TL uppercut
line 1049
;1049:			newmove = LS_A_BR2TL;
ADDRLP4 0
CNSTI4 7
ASGNI4
line 1050
;1050:		}
ADDRGP4 $435
JUMPV
LABELV $438
line 1052
;1051:		else
;1052:		{//just left is a right slice
line 1053
;1053:			newmove = LS_A_R2L;
ADDRLP4 0
CNSTI4 8
ASGNI4
line 1054
;1054:		}
line 1055
;1055:	}
ADDRGP4 $435
JUMPV
LABELV $434
line 1057
;1056:	else
;1057:	{//not moving left or right
line 1058
;1058:		if ( pm->cmd.forwardmove > 0 )
ADDRGP4 pm
INDIRP4
CNSTI4 28
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
LEI4 $440
line 1059
;1059:		{//forward= T2B slash
line 1060
;1060:			if (pm->ps->fd.saberAnimLevel == FORCE_LEVEL_2 &&
ADDRLP4 4
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 1216
ADDP4
INDIRI4
CNSTI4 2
NEI4 $442
ADDRLP4 4
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
CNSTF4 1120403456
LEF4 $442
ADDRLP4 8
ADDRGP4 PM_GroundDistance
CALLF4
ASGNF4
ADDRLP4 8
INDIRF4
CNSTF4 1107296256
GEF4 $442
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 92
ADDP4
INDIRI4
ARGI4
ADDRLP4 12
ADDRGP4 BG_InSpecialJump
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $442
line 1065
;1061:				/*pm->ps->groundEntityNum != ENTITYNUM_NONE &&*/
;1062:				pm->ps->velocity[2] > 100 &&
;1063:				PM_GroundDistance() < 32 &&
;1064:				!BG_InSpecialJump(pm->ps->legsAnim))
;1065:			{ //FLIP AND DOWNWARD ATTACK
line 1068
;1066:				trace_t tr;
;1067:
;1068:				if (PM_SomeoneInFront(&tr))
ADDRLP4 16
ARGP4
ADDRLP4 1096
ADDRGP4 PM_SomeoneInFront
CALLI4
ASGNI4
ADDRLP4 1096
INDIRI4
CNSTI4 0
EQI4 $441
line 1069
;1069:				{
line 1070
;1070:					newmove = PM_SaberFlipOverAttackMove(&tr);
ADDRLP4 16
ARGP4
ADDRLP4 1100
ADDRGP4 PM_SaberFlipOverAttackMove
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 1100
INDIRI4
ASGNI4
line 1071
;1071:				}
line 1072
;1072:			}
ADDRGP4 $441
JUMPV
LABELV $442
line 1073
;1073:			else if (pm->ps->fd.saberAnimLevel == FORCE_LEVEL_1 &&
ADDRLP4 16
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 20
CNSTI4 1
ASGNI4
ADDRLP4 16
INDIRP4
CNSTI4 1216
ADDP4
INDIRI4
ADDRLP4 20
INDIRI4
NEI4 $446
ADDRLP4 24
CNSTI4 0
ASGNI4
ADDRLP4 16
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 20
INDIRI4
BANDI4
ADDRLP4 24
INDIRI4
EQI4 $446
ADDRLP4 16
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
ADDRLP4 24
INDIRI4
GTI4 $446
line 1076
;1074:				(pm->ps->pm_flags & PMF_DUCKED) &&
;1075:				pm->ps->weaponTime <= 0)
;1076:			{ //LUNGE (weak)
line 1077
;1077:				newmove = PM_SaberLungeAttackMove();
ADDRLP4 28
ADDRGP4 PM_SaberLungeAttackMove
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 28
INDIRI4
ASGNI4
line 1078
;1078:			}
ADDRGP4 $441
JUMPV
LABELV $446
line 1080
;1079:			else
;1080:			{
line 1081
;1081:				newmove = LS_A_T2B;
ADDRLP4 0
CNSTI4 10
ASGNI4
line 1082
;1082:			}
line 1083
;1083:		}
ADDRGP4 $441
JUMPV
LABELV $440
line 1084
;1084:		else if ( pm->cmd.forwardmove < 0 )
ADDRGP4 pm
INDIRP4
CNSTI4 28
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
GEI4 $448
line 1085
;1085:		{//backward= T2B slash//B2T uppercut?
line 1086
;1086:			if (PM_CanBackstab())
ADDRLP4 4
ADDRGP4 PM_CanBackstab
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $450
line 1087
;1087:			{ //BACKSTAB (attack varies by level)
line 1088
;1088:				if (pm->ps->fd.saberAnimLevel >= FORCE_LEVEL_2)
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 1216
ADDP4
INDIRI4
CNSTI4 2
LTI4 $452
line 1089
;1089:				{//medium and higher attacks
line 1090
;1090:					if ( (pm->ps->pm_flags&PMF_DUCKED) || pm->cmd.upmove < 0 )
ADDRLP4 8
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 12
CNSTI4 0
ASGNI4
ADDRLP4 8
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 1
BANDI4
ADDRLP4 12
INDIRI4
NEI4 $456
ADDRLP4 8
INDIRP4
CNSTI4 30
ADDP4
INDIRI1
CVII4 1
ADDRLP4 12
INDIRI4
GEI4 $454
LABELV $456
line 1091
;1091:					{
line 1092
;1092:						newmove = LS_A_BACK_CR;
ADDRLP4 0
CNSTI4 13
ASGNI4
line 1093
;1093:					}
ADDRGP4 $449
JUMPV
LABELV $454
line 1095
;1094:					else
;1095:					{
line 1096
;1096:						newmove = LS_A_BACK;
ADDRLP4 0
CNSTI4 12
ASGNI4
line 1097
;1097:					}
line 1098
;1098:				}
ADDRGP4 $449
JUMPV
LABELV $452
line 1100
;1099:				else
;1100:				{ //weak attack
line 1101
;1101:					newmove = LS_A_BACKSTAB;
ADDRLP4 0
CNSTI4 11
ASGNI4
line 1102
;1102:				}
line 1103
;1103:			}
ADDRGP4 $449
JUMPV
LABELV $450
line 1105
;1104:			else
;1105:			{
line 1106
;1106:				newmove = LS_A_T2B;
ADDRLP4 0
CNSTI4 10
ASGNI4
line 1107
;1107:			}
line 1108
;1108:		}
ADDRGP4 $449
JUMPV
LABELV $448
line 1109
;1109:		else if ( PM_SaberInBounce( curmove ) )
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 PM_SaberInBounce
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $457
line 1110
;1110:		{//bounces should go to their default attack if you don't specify a direction but are attacking
line 1111
;1111:			if ( PM_SaberKataDone() )
ADDRLP4 8
ADDRGP4 PM_SaberKataDone
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $459
line 1112
;1112:			{
line 1113
;1113:				newmove = saberMoveData[curmove].chain_idle;
ADDRLP4 0
CNSTI4 40
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 saberMoveData+28
ADDP4
INDIRI4
ASGNI4
line 1114
;1114:			}
ADDRGP4 $458
JUMPV
LABELV $459
line 1116
;1115:			else
;1116:			{
line 1117
;1117:				newmove = saberMoveData[curmove].chain_attack;
ADDRLP4 0
CNSTI4 40
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 saberMoveData+32
ADDP4
INDIRI4
ASGNI4
line 1118
;1118:			}
line 1119
;1119:		}
ADDRGP4 $458
JUMPV
LABELV $457
line 1120
;1120:		else if ( curmove == LS_READY )
ADDRFP4 0
INDIRI4
CNSTI4 1
NEI4 $463
line 1121
;1121:		{//Not moving at all, shouldn't have gotten here...?
line 1127
;1122:			//for now, just pick a random attack
;1123:			//newmove = Q_irand( LS_A_TL2BR, LS_A_T2B );
;1124:			//rww - If we don't seed with a "common" value, the client and server will get mismatched
;1125:			//prediction values. Under laggy conditions this will cause the appearance of rapid swing
;1126:			//sequence changes.
;1127:			newmove = PM_irand_timesync(LS_A_TL2BR, LS_A_T2B);
CNSTI4 4
ARGI4
CNSTI4 10
ARGI4
ADDRLP4 8
ADDRGP4 PM_irand_timesync
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 8
INDIRI4
ASGNI4
line 1128
;1128:		}
LABELV $463
LABELV $458
LABELV $449
LABELV $441
line 1129
;1129:	}
LABELV $435
LABELV $429
line 1131
;1130:
;1131:	return newmove;
ADDRLP4 0
INDIRI4
RETI4
LABELV $427
endproc PM_SaberAttackForMovement 1104 8
export PM_WeaponLightsaber
proc PM_WeaponLightsaber 104 16
line 1144
;1132:}
;1133:/*
;1134:=================
;1135:PM_WeaponLightsaber
;1136:
;1137:Consults a chart to choose what to do with the lightsaber.
;1138:While this is a little different than the Quake 3 code, there is no clean way of using the Q3 code for this kind of thing.
;1139:=================
;1140:*/
;1141:// Ultimate goal is to set the sabermove to the proper next location
;1142:// Note that if the resultant animation is NONE, then the animation is essentially "idle", and is set in WP_TorsoAnim
;1143:void PM_WeaponLightsaber(void)
;1144:{
line 1146
;1145:	int			addTime,amount;
;1146:	qboolean	delayed_fire = qfalse;
ADDRLP4 20
CNSTI4 0
ASGNI4
line 1147
;1147:	int			anim=-1, curmove, newmove=LS_NONE;
ADDRLP4 16
CNSTI4 -1
ASGNI4
ADDRLP4 8
CNSTI4 0
ASGNI4
line 1149
;1148:
;1149:	qboolean saberInAir = qtrue;
ADDRLP4 24
CNSTI4 1
ASGNI4
line 1150
;1150:	qboolean checkOnlyWeap = qfalse;
ADDRLP4 12
CNSTI4 0
ASGNI4
line 1152
;1151:
;1152: 	if ( pm->ps->saberLockTime > pm->cmd.serverTime )
ADDRLP4 32
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
INDIRP4
CNSTI4 524
ADDP4
INDIRI4
ADDRLP4 32
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
LEI4 $466
line 1153
;1153:	{
line 1154
;1154:		pm->ps->saberMove = LS_NONE;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 512
ADDP4
CNSTI4 0
ASGNI4
line 1155
;1155:		PM_SaberLocked();
ADDRGP4 PM_SaberLocked
CALLV
pop
line 1156
;1156:		return;
ADDRGP4 $465
JUMPV
LABELV $466
line 1159
;1157:	}
;1158:	else
;1159:	{
line 1160
;1160:		if ( ( (pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_BF2LOCK ||
ADDRLP4 36
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 40
ADDRLP4 36
INDIRP4
CNSTI4 100
ADDP4
INDIRI4
CNSTI4 -2049
BANDI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 538
EQI4 $473
ADDRLP4 40
INDIRI4
CNSTI4 541
EQI4 $473
ADDRLP4 40
INDIRI4
CNSTI4 548
EQI4 $473
ADDRLP4 40
INDIRI4
CNSTI4 549
EQI4 $473
ADDRLP4 36
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
CNSTI4 0
EQI4 $468
LABELV $473
line 1166
;1161:				(pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_BF1LOCK ||
;1162:				(pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_CWCIRCLELOCK ||
;1163:				(pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_CCWCIRCLELOCK ||
;1164:				pm->ps->saberLockFrame )
;1165:			)
;1166:		{
line 1167
;1167:			if (pm->ps->saberLockEnemy < ENTITYNUM_NONE &&
ADDRLP4 44
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 528
ADDP4
INDIRI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 1023
GEI4 $474
ADDRLP4 44
INDIRI4
CNSTI4 0
LTI4 $474
line 1169
;1168:				pm->ps->saberLockEnemy >= 0)
;1169:			{
line 1172
;1170:				playerState_t *en;
;1171:
;1172:				en = pm->bgClients[pm->ps->saberLockEnemy];
ADDRLP4 52
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 48
ADDRLP4 52
INDIRP4
INDIRP4
CNSTI4 528
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 52
INDIRP4
CNSTI4 248
ADDP4
ADDP4
INDIRP4
ASGNP4
line 1174
;1173:
;1174:				if (en)
ADDRLP4 48
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $476
line 1175
;1175:				{
line 1176
;1176:					PM_SaberLockBreak(en, qfalse);
ADDRLP4 48
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 PM_SaberLockBreak
CALLV
pop
line 1177
;1177:					return;
ADDRGP4 $465
JUMPV
LABELV $476
line 1179
;1178:				}
;1179:			}
LABELV $474
line 1181
;1180:
;1181:			if ( ( (pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_BF2LOCK ||
ADDRLP4 48
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 52
ADDRLP4 48
INDIRP4
CNSTI4 100
ADDP4
INDIRI4
CNSTI4 -2049
BANDI4
ASGNI4
ADDRLP4 52
INDIRI4
CNSTI4 538
EQI4 $483
ADDRLP4 52
INDIRI4
CNSTI4 541
EQI4 $483
ADDRLP4 52
INDIRI4
CNSTI4 548
EQI4 $483
ADDRLP4 52
INDIRI4
CNSTI4 549
EQI4 $483
ADDRLP4 48
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
CNSTI4 0
EQI4 $478
LABELV $483
line 1187
;1182:					(pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_BF1LOCK ||
;1183:					(pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_CWCIRCLELOCK ||
;1184:					(pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == BOTH_CCWCIRCLELOCK ||
;1185:					pm->ps->saberLockFrame )
;1186:				)
;1187:			{
line 1188
;1188:				pm->ps->torsoTimer = 0;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 96
ADDP4
CNSTI4 0
ASGNI4
line 1189
;1189:				PM_SetAnim(SETANIM_TORSO,BOTH_STAND1,SETANIM_FLAG_OVERRIDE, 100);
ADDRLP4 56
CNSTI4 1
ASGNI4
ADDRLP4 56
INDIRI4
ARGI4
CNSTI4 550
ARGI4
ADDRLP4 56
INDIRI4
ARGI4
CNSTI4 100
ARGI4
ADDRGP4 PM_SetAnim
CALLV
pop
line 1190
;1190:				pm->ps->saberLockFrame = 0;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 532
ADDP4
CNSTI4 0
ASGNI4
line 1191
;1191:			}
LABELV $478
line 1192
;1192:		}
LABELV $468
line 1193
;1193:	}
line 1195
;1194:
;1195:	if (pm->ps->saberHolstered)
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 1300
ADDP4
INDIRI4
CNSTI4 0
EQI4 $484
line 1196
;1196:	{
line 1197
;1197:		if (pm->ps->saberMove != LS_READY)
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 512
ADDP4
INDIRI4
CNSTI4 1
EQI4 $486
line 1198
;1198:		{
line 1199
;1199:			PM_SetSaberMove( LS_READY );
CNSTI4 1
ARGI4
ADDRGP4 PM_SetSaberMove
CALLV
pop
line 1200
;1200:		}
LABELV $486
line 1202
;1201:
;1202:		if ((pm->ps->legsAnim & ~ANIM_TOGGLEBIT) != (pm->ps->torsoAnim & ~ANIM_TOGGLEBIT))
ADDRLP4 36
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 40
CNSTI4 -2049
ASGNI4
ADDRLP4 36
INDIRP4
CNSTI4 92
ADDP4
INDIRI4
ADDRLP4 40
INDIRI4
BANDI4
ADDRLP4 36
INDIRP4
CNSTI4 100
ADDP4
INDIRI4
ADDRLP4 40
INDIRI4
BANDI4
EQI4 $488
line 1203
;1203:		{
line 1204
;1204:			PM_SetAnim(SETANIM_TORSO,(pm->ps->legsAnim & ~ANIM_TOGGLEBIT),SETANIM_FLAG_OVERRIDE, 100);
ADDRLP4 44
CNSTI4 1
ASGNI4
ADDRLP4 44
INDIRI4
ARGI4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 92
ADDP4
INDIRI4
CNSTI4 -2049
BANDI4
ARGI4
ADDRLP4 44
INDIRI4
ARGI4
CNSTI4 100
ARGI4
ADDRGP4 PM_SetAnim
CALLV
pop
line 1205
;1205:		}
LABELV $488
line 1207
;1206:
;1207:		if (BG_InSaberStandAnim(pm->ps->torsoAnim))
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 100
ADDP4
INDIRI4
ARGI4
ADDRLP4 44
ADDRGP4 BG_InSaberStandAnim
CALLI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 0
EQI4 $490
line 1208
;1208:		{
line 1209
;1209:			PM_SetAnim(SETANIM_TORSO,BOTH_STAND1,SETANIM_FLAG_OVERRIDE, 100);
ADDRLP4 48
CNSTI4 1
ASGNI4
ADDRLP4 48
INDIRI4
ARGI4
CNSTI4 550
ARGI4
ADDRLP4 48
INDIRI4
ARGI4
CNSTI4 100
ARGI4
ADDRGP4 PM_SetAnim
CALLV
pop
line 1210
;1210:		}
LABELV $490
line 1212
;1211:
;1212:		if (pm->ps->weaponTime < 1 && ((pm->cmd.buttons & BUTTON_ALT_ATTACK) || (pm->cmd.buttons & BUTTON_ATTACK)))
ADDRLP4 48
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 52
CNSTI4 1
ASGNI4
ADDRLP4 48
INDIRP4
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
ADDRLP4 52
INDIRI4
GEI4 $492
ADDRLP4 56
ADDRLP4 48
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ASGNI4
ADDRLP4 60
CNSTI4 0
ASGNI4
ADDRLP4 56
INDIRI4
CNSTI4 128
BANDI4
ADDRLP4 60
INDIRI4
NEI4 $494
ADDRLP4 56
INDIRI4
ADDRLP4 52
INDIRI4
BANDI4
ADDRLP4 60
INDIRI4
EQI4 $492
LABELV $494
line 1213
;1213:		{
line 1214
;1214:			if (pm->ps->duelTime < pm->cmd.serverTime)
ADDRLP4 64
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 64
INDIRP4
INDIRP4
CNSTI4 1288
ADDP4
INDIRI4
ADDRLP4 64
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
GEI4 $495
line 1215
;1215:			{
line 1216
;1216:				pm->ps->saberHolstered = qfalse;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 1300
ADDP4
CNSTI4 0
ASGNI4
line 1217
;1217:				PM_AddEvent(EV_SABER_UNHOLSTER);
CNSTI4 29
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 1218
;1218:			}
LABELV $495
line 1219
;1219:		}
LABELV $492
line 1221
;1220:
;1221:		if ( pm->ps->weaponTime > 0 )
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
CNSTI4 0
LEI4 $497
line 1222
;1222:		{
line 1223
;1223:			pm->ps->weaponTime -= pml.msec;
ADDRLP4 64
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 44
ADDP4
ASGNP4
ADDRLP4 64
INDIRP4
ADDRLP4 64
INDIRP4
INDIRI4
ADDRGP4 pml+40
INDIRI4
SUBI4
ASGNI4
line 1224
;1224:		}
LABELV $497
line 1226
;1225:
;1226:		checkOnlyWeap = qtrue;
ADDRLP4 12
CNSTI4 1
ASGNI4
line 1227
;1227:		goto weapChecks;
ADDRGP4 $500
JUMPV
LABELV $484
line 1230
;1228:	}
;1229:
;1230:	if ((pm->cmd.buttons & BUTTON_ALT_ATTACK) &&
ADDRLP4 36
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 40
CNSTI4 0
ASGNI4
ADDRLP4 36
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
CNSTI4 128
BANDI4
ADDRLP4 40
INDIRI4
EQI4 $501
ADDRLP4 44
ADDRLP4 36
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 44
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
CNSTI4 1
GEI4 $501
ADDRLP4 44
INDIRP4
CNSTI4 560
ADDP4
INDIRI4
ADDRLP4 40
INDIRI4
EQI4 $501
ADDRLP4 44
INDIRP4
CNSTI4 920
ADDP4
INDIRI4
CNSTI4 72
ADDRLP4 44
INDIRP4
CNSTI4 1000
ADDP4
INDIRI4
MULI4
ADDRGP4 forcePowerNeeded+68
ADDP4
INDIRI4
LTI4 $501
ADDRLP4 36
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
ARGI4
ADDRLP4 44
INDIRP4
ARGP4
ADDRLP4 48
ADDRGP4 BG_HasYsalamiri
CALLI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 0
NEI4 $501
ADDRLP4 52
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
ARGI4
ADDRLP4 52
INDIRP4
INDIRP4
ARGP4
ADDRLP4 52
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ARGI4
CNSTI4 17
ARGI4
ADDRLP4 56
ADDRGP4 BG_CanUseFPNow
CALLI4
ASGNI4
ADDRLP4 56
INDIRI4
CNSTI4 0
EQI4 $501
line 1237
;1231:		pm->ps->weaponTime < 1 &&
;1232:		pm->ps->saberCanThrow &&
;1233:		pm->ps->fd.forcePower >= forcePowerNeeded[pm->ps->fd.forcePowerLevel[FP_SABERTHROW]][FP_SABERTHROW] &&
;1234:		!BG_HasYsalamiri(pm->gametype, pm->ps) &&
;1235:		BG_CanUseFPNow(pm->gametype, pm->ps, pm->cmd.serverTime, FP_SABERTHROW)
;1236:		)
;1237:	{ //might as well just check for a saber throw right here
line 1239
;1238:		//This will get set to false again once the saber makes it back to its owner game-side
;1239:		if (!pm->ps->saberInFlight)
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 504
ADDP4
INDIRI4
CNSTI4 0
NEI4 $504
line 1240
;1240:		{
line 1241
;1241:			pm->ps->fd.forcePower -= forcePowerNeeded[pm->ps->fd.forcePowerLevel[FP_SABERTHROW]][FP_SABERTHROW];
ADDRLP4 60
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 64
ADDRLP4 60
INDIRP4
CNSTI4 920
ADDP4
ASGNP4
ADDRLP4 64
INDIRP4
ADDRLP4 64
INDIRP4
INDIRI4
CNSTI4 72
ADDRLP4 60
INDIRP4
CNSTI4 1000
ADDP4
INDIRI4
MULI4
ADDRGP4 forcePowerNeeded+68
ADDP4
INDIRI4
SUBI4
ASGNI4
line 1242
;1242:		}
LABELV $504
line 1244
;1243:
;1244:		pm->ps->saberInFlight = qtrue;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 504
ADDP4
CNSTI4 1
ASGNI4
line 1245
;1245:	}
LABELV $501
line 1247
;1246:	
;1247:	if ( pm->ps->saberInFlight )
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 504
ADDP4
INDIRI4
CNSTI4 0
EQI4 $507
line 1248
;1248:	{//guiding saber
line 1249
;1249:		PM_SetAnim(SETANIM_TORSO, BOTH_SABERPULL, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD, 100);
CNSTI4 1
ARGI4
CNSTI4 946
ARGI4
CNSTI4 3
ARGI4
CNSTI4 100
ARGI4
ADDRGP4 PM_SetAnim
CALLV
pop
line 1250
;1250:		pm->ps->torsoTimer = 1;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 96
ADDP4
CNSTI4 1
ASGNI4
line 1251
;1251:		return;
ADDRGP4 $465
JUMPV
LABELV $507
line 1255
;1252:	}
;1253:
;1254:   // don't allow attack until all buttons are up
;1255:	if ( pm->ps->pm_flags & PMF_RESPAWNED ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 512
BANDI4
CNSTI4 0
EQI4 $509
line 1256
;1256:		return;
ADDRGP4 $465
JUMPV
LABELV $509
line 1260
;1257:	}
;1258:
;1259:	// check for dead player
;1260:	if ( pm->ps->stats[STAT_HEALTH] <= 0 ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CNSTI4 0
GTI4 $511
line 1261
;1261:		return;
ADDRGP4 $465
JUMPV
LABELV $511
line 1264
;1262:	}
;1263:
;1264:	if (pm->ps->weaponstate == WEAPON_READY ||
ADDRLP4 60
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 152
ADDP4
INDIRI4
ASGNI4
ADDRLP4 60
INDIRI4
CNSTI4 0
EQI4 $515
ADDRLP4 60
INDIRI4
CNSTI4 6
NEI4 $513
LABELV $515
line 1266
;1265:		pm->ps->weaponstate == WEAPON_IDLE)
;1266:	{
line 1267
;1267:		if (pm->ps->saberMove != LS_READY && pm->ps->weaponTime <= 0 && !pm->ps->saberBlocked)
ADDRLP4 64
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 64
INDIRP4
CNSTI4 512
ADDP4
INDIRI4
CNSTI4 1
EQI4 $516
ADDRLP4 68
CNSTI4 0
ASGNI4
ADDRLP4 64
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
ADDRLP4 68
INDIRI4
GTI4 $516
ADDRLP4 64
INDIRP4
CNSTI4 520
ADDP4
INDIRI4
ADDRLP4 68
INDIRI4
NEI4 $516
line 1268
;1268:		{
line 1269
;1269:			PM_SetSaberMove( LS_READY );
CNSTI4 1
ARGI4
ADDRGP4 PM_SetSaberMove
CALLV
pop
line 1270
;1270:		}
LABELV $516
line 1271
;1271:	}
LABELV $513
line 1273
;1272:
;1273:	if( (pm->ps->torsoAnim & ~ANIM_TOGGLEBIT) == BOTH_RUN2 ||
ADDRLP4 64
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 100
ADDP4
INDIRI4
CNSTI4 -2049
BANDI4
ASGNI4
ADDRLP4 64
INDIRI4
CNSTI4 734
EQI4 $520
ADDRLP4 64
INDIRI4
CNSTI4 731
NEI4 $518
LABELV $520
line 1275
;1274:		(pm->ps->torsoAnim & ~ANIM_TOGGLEBIT) == BOTH_RUN1 )
;1275:	{
line 1276
;1276:		if ((pm->ps->torsoAnim & ~ANIM_TOGGLEBIT) != (pm->ps->legsAnim & ~ANIM_TOGGLEBIT))
ADDRLP4 68
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 72
CNSTI4 -2049
ASGNI4
ADDRLP4 68
INDIRP4
CNSTI4 100
ADDP4
INDIRI4
ADDRLP4 72
INDIRI4
BANDI4
ADDRLP4 68
INDIRP4
CNSTI4 92
ADDP4
INDIRI4
ADDRLP4 72
INDIRI4
BANDI4
EQI4 $521
line 1277
;1277:		{
line 1278
;1278:			PM_SetAnim(SETANIM_TORSO,(pm->ps->legsAnim & ~ANIM_TOGGLEBIT),SETANIM_FLAG_OVERRIDE, 100);
ADDRLP4 76
CNSTI4 1
ASGNI4
ADDRLP4 76
INDIRI4
ARGI4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 92
ADDP4
INDIRI4
CNSTI4 -2049
BANDI4
ARGI4
ADDRLP4 76
INDIRI4
ARGI4
CNSTI4 100
ARGI4
ADDRGP4 PM_SetAnim
CALLV
pop
line 1279
;1279:		}
LABELV $521
line 1280
;1280:	}
LABELV $518
line 1283
;1281:
;1282:	// make weapon function
;1283:	if ( pm->ps->weaponTime > 0 ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
CNSTI4 0
LEI4 $523
line 1284
;1284:		pm->ps->weaponTime -= pml.msec;
ADDRLP4 68
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 44
ADDP4
ASGNP4
ADDRLP4 68
INDIRP4
ADDRLP4 68
INDIRP4
INDIRI4
ADDRGP4 pml+40
INDIRI4
SUBI4
ASGNI4
line 1286
;1285:
;1286:		if (pm->ps->saberBlocked && pm->ps->torsoAnim != saberMoveData[pm->ps->saberMove].animToUse)
ADDRLP4 72
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 72
INDIRP4
CNSTI4 520
ADDP4
INDIRI4
CNSTI4 0
EQI4 $524
ADDRLP4 72
INDIRP4
CNSTI4 100
ADDP4
INDIRI4
CNSTI4 40
ADDRLP4 72
INDIRP4
CNSTI4 512
ADDP4
INDIRI4
MULI4
ADDRGP4 saberMoveData+4
ADDP4
INDIRI4
EQI4 $524
line 1287
;1287:		{ //rww - keep him in the blocking pose until he can attack again
line 1288
;1288:			PM_SetAnim(SETANIM_TORSO,saberMoveData[pm->ps->saberMove].animToUse,saberMoveData[pm->ps->saberMove].animSetFlags|SETANIM_FLAG_HOLD, saberMoveData[pm->ps->saberMove].blendTime);
CNSTI4 1
ARGI4
ADDRLP4 76
CNSTI4 40
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 512
ADDP4
INDIRI4
MULI4
ASGNI4
ADDRLP4 76
INDIRI4
ADDRGP4 saberMoveData+4
ADDP4
INDIRI4
ARGI4
ADDRLP4 76
INDIRI4
ADDRGP4 saberMoveData+16
ADDP4
INDIRU4
CNSTU4 2
BORU4
CVUI4 4
ARGI4
ADDRLP4 76
INDIRI4
ADDRGP4 saberMoveData+20
ADDP4
INDIRI4
ARGI4
ADDRGP4 PM_SetAnim
CALLV
pop
line 1289
;1289:			return;
ADDRGP4 $465
JUMPV
line 1291
;1290:		}
;1291:	}
LABELV $523
line 1293
;1292:	else
;1293:	{
line 1294
;1294:		pm->ps->weaponstate = WEAPON_READY;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 152
ADDP4
CNSTI4 0
ASGNI4
line 1295
;1295:	}
LABELV $524
line 1298
;1296:
;1297:	// Now we react to a block action by the player's lightsaber.
;1298:	if ( pm->ps->saberBlocked )
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 520
ADDP4
INDIRI4
CNSTI4 0
EQI4 $532
line 1299
;1299:	{
line 1300
;1300:		int firstSet = 0;
ADDRLP4 68
CNSTI4 0
ASGNI4
line 1302
;1301:
;1302:		if (!pm->ps->weaponTime)
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
CNSTI4 0
NEI4 $534
line 1303
;1303:		{
line 1304
;1304:			firstSet = 1;
ADDRLP4 68
CNSTI4 1
ASGNI4
line 1305
;1305:		}
LABELV $534
line 1307
;1306:
;1307:		switch ( pm->ps->saberBlocked )
ADDRLP4 72
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 520
ADDP4
INDIRI4
ASGNI4
ADDRLP4 72
INDIRI4
CNSTI4 1
LTI4 $536
ADDRLP4 72
INDIRI4
CNSTI4 12
GTI4 $536
ADDRLP4 72
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $572-4
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $572
address $539
address $544
address $562
address $564
address $566
address $568
address $570
address $563
address $565
address $567
address $569
address $571
code
line 1308
;1308:		{
LABELV $539
line 1311
;1309:			case BLOCKED_PARRY_BROKEN:
;1310:				//whatever parry we were is in now broken, play the appropriate knocked-away anim
;1311:				{
line 1314
;1312:					int nextMove;
;1313:
;1314:					if ( PM_SaberInBrokenParry( pm->ps->saberMove ) )
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 512
ADDP4
INDIRI4
ARGI4
ADDRLP4 84
ADDRGP4 PM_SaberInBrokenParry
CALLI4
ASGNI4
ADDRLP4 84
INDIRI4
CNSTI4 0
EQI4 $540
line 1315
;1315:					{//already have one...?
line 1316
;1316:						nextMove = pm->ps->saberMove;
ADDRLP4 80
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 512
ADDP4
INDIRI4
ASGNI4
line 1317
;1317:					}
ADDRGP4 $541
JUMPV
LABELV $540
line 1319
;1318:					else
;1319:					{
line 1320
;1320:						nextMove = PM_BrokenParryForParry( pm->ps->saberMove );
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 512
ADDP4
INDIRI4
ARGI4
ADDRLP4 88
ADDRGP4 PM_BrokenParryForParry
CALLI4
ASGNI4
ADDRLP4 80
ADDRLP4 88
INDIRI4
ASGNI4
line 1321
;1321:					}
LABELV $541
line 1322
;1322:					if ( nextMove != LS_NONE )
ADDRLP4 80
INDIRI4
CNSTI4 0
EQI4 $537
line 1323
;1323:					{
line 1324
;1324:						PM_SetSaberMove( nextMove );
ADDRLP4 80
INDIRI4
CVII2 4
CVII4 2
ARGI4
ADDRGP4 PM_SetSaberMove
CALLV
pop
line 1325
;1325:						pm->ps->weaponTime = pm->ps->torsoTimer;
ADDRLP4 88
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 88
INDIRP4
CNSTI4 44
ADDP4
ADDRLP4 88
INDIRP4
CNSTI4 96
ADDP4
INDIRI4
ASGNI4
line 1326
;1326:					}
line 1328
;1327:					else
;1328:					{//Maybe in a knockaway?
line 1329
;1329:					}
line 1330
;1330:				}
line 1331
;1331:				break;
ADDRGP4 $537
JUMPV
LABELV $544
line 1335
;1332:			case BLOCKED_ATK_BOUNCE:
;1333:				// If there is absolutely no blocked move in the chart, don't even mess with the animation.
;1334:				// OR if we are already in a block or parry.
;1335:				if (pm->ps->saberMove >= LS_T1_BR__R/*LS_BOUNCE_TOP*/ )//|| saberMoveData[pm->ps->saberMove].bounceMove == LS_NONE )
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 512
ADDP4
INDIRI4
CNSTI4 32
LTI4 $545
line 1336
;1336:				{//an actual bounce?  Other bounces before this are actually transitions?
line 1337
;1337:					pm->ps->saberBlocked = BLOCKED_NONE;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 520
ADDP4
CNSTI4 0
ASGNI4
line 1338
;1338:				}
ADDRGP4 $537
JUMPV
LABELV $545
line 1340
;1339:				else
;1340:				{
line 1343
;1341:					int bounceMove;
;1342:
;1343:					if ( pm->cmd.buttons & BUTTON_ATTACK )
ADDRGP4 pm
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $547
line 1344
;1344:					{//transition to a new attack
line 1345
;1345:						int newQuad = PM_SaberMoveQuadrantForMovement( &pm->cmd );
ADDRGP4 pm
INDIRP4
CNSTI4 4
ADDP4
ARGP4
ADDRLP4 88
ADDRGP4 PM_SaberMoveQuadrantForMovement
CALLI4
ASGNI4
ADDRLP4 84
ADDRLP4 88
INDIRI4
ASGNI4
ADDRGP4 $550
JUMPV
LABELV $549
line 1347
;1346:						while ( newQuad == saberMoveData[pm->ps->saberMove].startQuad )
;1347:						{//player is still in same attack quad, don't repeat that attack because it looks bad, 
line 1350
;1348:							//FIXME: try to pick one that might look cool?
;1349:							//newQuad = Q_irand( Q_BR, Q_BL );
;1350:							newQuad = PM_irand_timesync( Q_BR, Q_BL );
CNSTI4 0
ARGI4
CNSTI4 6
ARGI4
ADDRLP4 92
ADDRGP4 PM_irand_timesync
CALLI4
ASGNI4
ADDRLP4 84
ADDRLP4 92
INDIRI4
ASGNI4
line 1352
;1351:							//FIXME: sanity check, just in case?
;1352:						}//else player is switching up anyway, take the new attack dir
LABELV $550
line 1346
ADDRLP4 84
INDIRI4
CNSTI4 40
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 512
ADDP4
INDIRI4
MULI4
ADDRGP4 saberMoveData+8
ADDP4
INDIRI4
EQI4 $549
line 1353
;1353:						bounceMove = transitionMove[saberMoveData[pm->ps->saberMove].startQuad][newQuad];
ADDRLP4 80
ADDRLP4 84
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 40
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 512
ADDP4
INDIRI4
MULI4
ADDRGP4 saberMoveData+8
ADDP4
INDIRI4
CNSTI4 5
LSHI4
ADDRGP4 transitionMove
ADDP4
ADDP4
INDIRI4
ASGNI4
line 1354
;1354:					}
ADDRGP4 $548
JUMPV
LABELV $547
line 1356
;1355:					else
;1356:					{//return to ready
line 1357
;1357:						if ( saberMoveData[pm->ps->saberMove].startQuad == Q_T )
CNSTI4 40
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 512
ADDP4
INDIRI4
MULI4
ADDRGP4 saberMoveData+8
ADDP4
INDIRI4
CNSTI4 3
NEI4 $554
line 1358
;1358:						{
line 1359
;1359:							bounceMove = LS_R_BL2TR;
ADDRLP4 80
CNSTI4 27
ASGNI4
line 1360
;1360:						}
ADDRGP4 $555
JUMPV
LABELV $554
line 1361
;1361:						else if ( saberMoveData[pm->ps->saberMove].startQuad < Q_T )
CNSTI4 40
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 512
ADDP4
INDIRI4
MULI4
ADDRGP4 saberMoveData+8
ADDP4
INDIRI4
CNSTI4 3
GEI4 $557
line 1362
;1362:						{
line 1363
;1363:							bounceMove = LS_R_TL2BR+saberMoveData[pm->ps->saberMove].startQuad-Q_BR;
ADDRLP4 80
CNSTI4 40
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 512
ADDP4
INDIRI4
MULI4
ADDRGP4 saberMoveData+8
ADDP4
INDIRI4
CNSTI4 25
ADDI4
ASGNI4
line 1364
;1364:						}
ADDRGP4 $558
JUMPV
LABELV $557
line 1366
;1365:						else// if ( saberMoveData[pm->ps->saberMove].startQuad > Q_T )
;1366:						{
line 1367
;1367:							bounceMove = LS_R_BR2TL+saberMoveData[pm->ps->saberMove].startQuad-Q_TL;
ADDRLP4 80
CNSTI4 40
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 512
ADDP4
INDIRI4
MULI4
ADDRGP4 saberMoveData+8
ADDP4
INDIRI4
CNSTI4 28
ADDI4
CNSTI4 4
SUBI4
ASGNI4
line 1368
;1368:						}
LABELV $558
LABELV $555
line 1369
;1369:					}
LABELV $548
line 1370
;1370:					PM_SetSaberMove( bounceMove );
ADDRLP4 80
INDIRI4
CVII2 4
CVII4 2
ARGI4
ADDRGP4 PM_SetSaberMove
CALLV
pop
line 1372
;1371:
;1372:					pm->ps->weaponTime = pm->ps->torsoTimer;//+saberMoveData[bounceMove].blendTime+SABER_BLOCK_DUR;
ADDRLP4 84
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 84
INDIRP4
CNSTI4 44
ADDP4
ADDRLP4 84
INDIRP4
CNSTI4 96
ADDP4
INDIRI4
ASGNI4
line 1374
;1373:
;1374:				}
line 1375
;1375:				break;
ADDRGP4 $537
JUMPV
LABELV $562
line 1377
;1376:			case BLOCKED_UPPER_RIGHT:
;1377:				PM_SetSaberMove( LS_PARRY_UR );
CNSTI4 109
ARGI4
ADDRGP4 PM_SetSaberMove
CALLV
pop
line 1378
;1378:				break;
ADDRGP4 $537
JUMPV
LABELV $563
line 1380
;1379:			case BLOCKED_UPPER_RIGHT_PROJ:
;1380:				PM_SetSaberMove( LS_REFLECT_UR );
CNSTI4 114
ARGI4
ADDRGP4 PM_SetSaberMove
CALLV
pop
line 1381
;1381:				break;
ADDRGP4 $537
JUMPV
LABELV $564
line 1383
;1382:			case BLOCKED_UPPER_LEFT:
;1383:				PM_SetSaberMove( LS_PARRY_UL );
CNSTI4 110
ARGI4
ADDRGP4 PM_SetSaberMove
CALLV
pop
line 1384
;1384:				break;
ADDRGP4 $537
JUMPV
LABELV $565
line 1386
;1385:			case BLOCKED_UPPER_LEFT_PROJ:
;1386:				PM_SetSaberMove( LS_REFLECT_UL );
CNSTI4 115
ARGI4
ADDRGP4 PM_SetSaberMove
CALLV
pop
line 1387
;1387:				break;
ADDRGP4 $537
JUMPV
LABELV $566
line 1389
;1388:			case BLOCKED_LOWER_RIGHT:
;1389:				PM_SetSaberMove( LS_PARRY_LR );
CNSTI4 111
ARGI4
ADDRGP4 PM_SetSaberMove
CALLV
pop
line 1390
;1390:				break;
ADDRGP4 $537
JUMPV
LABELV $567
line 1392
;1391:			case BLOCKED_LOWER_RIGHT_PROJ:
;1392:				PM_SetSaberMove( LS_REFLECT_LR );
CNSTI4 116
ARGI4
ADDRGP4 PM_SetSaberMove
CALLV
pop
line 1393
;1393:				break;
ADDRGP4 $537
JUMPV
LABELV $568
line 1395
;1394:			case BLOCKED_LOWER_LEFT:
;1395:				PM_SetSaberMove( LS_PARRY_LL );
CNSTI4 112
ARGI4
ADDRGP4 PM_SetSaberMove
CALLV
pop
line 1396
;1396:				break;
ADDRGP4 $537
JUMPV
LABELV $569
line 1398
;1397:			case BLOCKED_LOWER_LEFT_PROJ:
;1398:				PM_SetSaberMove( LS_REFLECT_LL);
CNSTI4 117
ARGI4
ADDRGP4 PM_SetSaberMove
CALLV
pop
line 1399
;1399:				break;
ADDRGP4 $537
JUMPV
LABELV $570
line 1401
;1400:			case BLOCKED_TOP:
;1401:				PM_SetSaberMove( LS_PARRY_UP );
CNSTI4 108
ARGI4
ADDRGP4 PM_SetSaberMove
CALLV
pop
line 1402
;1402:				break;
ADDRGP4 $537
JUMPV
LABELV $571
line 1404
;1403:			case BLOCKED_TOP_PROJ:
;1404:				PM_SetSaberMove( LS_REFLECT_UP );
CNSTI4 113
ARGI4
ADDRGP4 PM_SetSaberMove
CALLV
pop
line 1405
;1405:				break;
ADDRGP4 $537
JUMPV
LABELV $536
line 1407
;1406:			default:
;1407:				pm->ps->saberBlocked = BLOCKED_NONE;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 520
ADDP4
CNSTI4 0
ASGNI4
line 1408
;1408:				break;
LABELV $537
line 1411
;1409:		}
;1410:
;1411:		if (pm->ps->saberBlocked != BLOCKED_ATK_BOUNCE && pm->ps->saberBlocked != BLOCKED_PARRY_BROKEN && pm->ps->weaponTime < 1)
ADDRLP4 80
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 84
ADDRLP4 80
INDIRP4
CNSTI4 520
ADDP4
INDIRI4
ASGNI4
ADDRLP4 84
INDIRI4
CNSTI4 2
EQI4 $574
ADDRLP4 88
CNSTI4 1
ASGNI4
ADDRLP4 84
INDIRI4
ADDRLP4 88
INDIRI4
EQI4 $574
ADDRLP4 80
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
ADDRLP4 88
INDIRI4
GEI4 $574
line 1412
;1412:		{
line 1413
;1413:			pm->ps->torsoTimer = SABER_BLOCK_DUR;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 96
ADDP4
CNSTI4 150
ASGNI4
line 1414
;1414:			pm->ps->weaponTime = pm->ps->torsoTimer;
ADDRLP4 92
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 92
INDIRP4
CNSTI4 44
ADDP4
ADDRLP4 92
INDIRP4
CNSTI4 96
ADDP4
INDIRI4
ASGNI4
line 1415
;1415:		}
LABELV $574
line 1417
;1416:
;1417:		if (firstSet)
ADDRLP4 68
INDIRI4
CNSTI4 0
EQI4 $576
line 1418
;1418:		{
line 1419
;1419:			return;
ADDRGP4 $465
JUMPV
LABELV $576
line 1423
;1420:		}
;1421:
;1422:		// Charging is like a lead-up before attacking again.  This is an appropriate use, or we can create a new weaponstate for blocking
;1423:		pm->ps->weaponstate = WEAPON_READY;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 152
ADDP4
CNSTI4 0
ASGNI4
line 1426
;1424:
;1425:		// Done with block, so stop these active weapon branches.
;1426:		return;
ADDRGP4 $465
JUMPV
LABELV $532
LABELV $500
line 1432
;1427:	}
;1428:
;1429:weapChecks:
;1430:	// check for weapon change
;1431:	// can't change if weapon is firing, but can change again if lowering or raising
;1432:	if ( pm->ps->weaponTime <= 0 || pm->ps->weaponstate != WEAPON_FIRING ) {
ADDRLP4 68
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 68
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
CNSTI4 0
LEI4 $580
ADDRLP4 68
INDIRP4
CNSTI4 152
ADDP4
INDIRI4
CNSTI4 3
EQI4 $578
LABELV $580
line 1433
;1433:		if ( pm->ps->weapon != pm->cmd.weapon ) {
ADDRLP4 72
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 72
INDIRP4
INDIRP4
CNSTI4 148
ADDP4
INDIRI4
ADDRLP4 72
INDIRP4
CNSTI4 24
ADDP4
INDIRU1
CVUI4 1
EQI4 $581
line 1434
;1434:			PM_BeginWeaponChange( pm->cmd.weapon );
ADDRGP4 pm
INDIRP4
CNSTI4 24
ADDP4
INDIRU1
CVUI4 1
ARGI4
ADDRGP4 PM_BeginWeaponChange
CALLV
pop
line 1435
;1435:		}
LABELV $581
line 1436
;1436:	}
LABELV $578
line 1438
;1437:
;1438:	if ( pm->ps->weaponTime > 0 ) 
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
CNSTI4 0
LEI4 $583
line 1439
;1439:	{
line 1440
;1440:		return;
ADDRGP4 $465
JUMPV
LABELV $583
line 1448
;1441:	}
;1442:
;1443:	// *********************************************************
;1444:	// WEAPON_DROPPING
;1445:	// *********************************************************
;1446:
;1447:	// change weapon if time
;1448:	if ( pm->ps->weaponstate == WEAPON_DROPPING ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 152
ADDP4
INDIRI4
CNSTI4 2
NEI4 $585
line 1449
;1449:		PM_FinishWeaponChange();
ADDRGP4 PM_FinishWeaponChange
CALLV
pop
line 1450
;1450:		return;
ADDRGP4 $465
JUMPV
LABELV $585
line 1457
;1451:	}
;1452:
;1453:	// *********************************************************
;1454:	// WEAPON_RAISING
;1455:	// *********************************************************
;1456:
;1457:	if ( pm->ps->weaponstate == WEAPON_RAISING ) 
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 152
ADDP4
INDIRI4
CNSTI4 1
NEI4 $587
line 1458
;1458:	{//Just selected the weapon
line 1459
;1459:		pm->ps->weaponstate = WEAPON_IDLE;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 152
ADDP4
CNSTI4 6
ASGNI4
line 1460
;1460:		if((pm->ps->legsAnim & ~ANIM_TOGGLEBIT) == BOTH_WALK1 )
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 92
ADDP4
INDIRI4
CNSTI4 -2049
BANDI4
CNSTI4 723
NEI4 $589
line 1461
;1461:		{
line 1462
;1462:			PM_SetAnim(SETANIM_TORSO,BOTH_WALK1,SETANIM_FLAG_NORMAL, 100);
CNSTI4 1
ARGI4
CNSTI4 723
ARGI4
CNSTI4 0
ARGI4
CNSTI4 100
ARGI4
ADDRGP4 PM_SetAnim
CALLV
pop
line 1463
;1463:		}
ADDRGP4 $590
JUMPV
LABELV $589
line 1464
;1464:		else if((pm->ps->legsAnim & ~ANIM_TOGGLEBIT) == BOTH_RUN2 )
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 92
ADDP4
INDIRI4
CNSTI4 -2049
BANDI4
CNSTI4 734
NEI4 $591
line 1465
;1465:		{
line 1466
;1466:			PM_SetAnim(SETANIM_TORSO,BOTH_RUN2,SETANIM_FLAG_NORMAL, 100);
CNSTI4 1
ARGI4
CNSTI4 734
ARGI4
CNSTI4 0
ARGI4
CNSTI4 100
ARGI4
ADDRGP4 PM_SetAnim
CALLV
pop
line 1467
;1467:		}
ADDRGP4 $592
JUMPV
LABELV $591
line 1468
;1468:		else if((pm->ps->legsAnim & ~ANIM_TOGGLEBIT) == BOTH_WALK2 )
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 92
ADDP4
INDIRI4
CNSTI4 -2049
BANDI4
CNSTI4 724
NEI4 $593
line 1469
;1469:		{
line 1470
;1470:			PM_SetAnim(SETANIM_TORSO,BOTH_WALK2,SETANIM_FLAG_NORMAL, 100);
CNSTI4 1
ARGI4
CNSTI4 724
ARGI4
CNSTI4 0
ARGI4
CNSTI4 100
ARGI4
ADDRGP4 PM_SetAnim
CALLV
pop
line 1471
;1471:		}
ADDRGP4 $594
JUMPV
LABELV $593
line 1473
;1472:		else
;1473:		{
line 1474
;1474:			PM_SetAnim(SETANIM_TORSO,PM_GetSaberStance(),SETANIM_FLAG_NORMAL, 100);
ADDRLP4 72
ADDRGP4 PM_GetSaberStance
CALLI4
ASGNI4
CNSTI4 1
ARGI4
ADDRLP4 72
INDIRI4
ARGI4
CNSTI4 0
ARGI4
CNSTI4 100
ARGI4
ADDRGP4 PM_SetAnim
CALLV
pop
line 1475
;1475:		}
LABELV $594
LABELV $592
LABELV $590
line 1477
;1476:
;1477:		if (pm->ps->weaponstate == WEAPON_RAISING)
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 152
ADDP4
INDIRI4
CNSTI4 1
NEI4 $595
line 1478
;1478:		{
line 1479
;1479:			return;
ADDRGP4 $465
JUMPV
LABELV $595
line 1482
;1480:		}
;1481:
;1482:	}
LABELV $587
line 1484
;1483:
;1484:	if (checkOnlyWeap)
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $597
line 1485
;1485:	{
line 1486
;1486:		return;
ADDRGP4 $465
JUMPV
LABELV $597
line 1495
;1487:	}
;1488:
;1489:	// *********************************************************
;1490:	// Check for WEAPON ATTACK
;1491:	// *********************************************************
;1492:
;1493:	// NOTENOTE This is simply a client-side struct.  Anything that is needed client and server should be moved to bg_weapon.
;1494:
;1495:	if(!delayed_fire)
ADDRLP4 20
INDIRI4
CNSTI4 0
NEI4 $599
line 1496
;1496:	{
line 1498
;1497:		// Start with the current move, and cross index it with the current control states.
;1498:		if ( pm->ps->saberMove > LS_NONE && pm->ps->saberMove < LS_MOVE_MAX )
ADDRLP4 72
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 512
ADDP4
INDIRI4
ASGNI4
ADDRLP4 72
INDIRI4
CNSTI4 0
LEI4 $601
ADDRLP4 72
INDIRI4
CNSTI4 118
GEI4 $601
line 1499
;1499:		{
line 1500
;1500:			curmove = pm->ps->saberMove;
ADDRLP4 4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 512
ADDP4
INDIRI4
ASGNI4
line 1501
;1501:		}
ADDRGP4 $602
JUMPV
LABELV $601
line 1503
;1502:		else
;1503:		{
line 1504
;1504:			curmove = LS_READY;
ADDRLP4 4
CNSTI4 1
ASGNI4
line 1505
;1505:		}
LABELV $602
line 1507
;1506:		// check for fire
;1507:		if ( !(pm->cmd.buttons & (BUTTON_ATTACK/*|BUTTON_ALT_ATTACK*/)) )
ADDRGP4 pm
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
NEI4 $603
line 1508
;1508:		{
line 1509
;1509:			if (pm->ps->weaponTime != 0)
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
CNSTI4 0
EQI4 $605
line 1510
;1510:			{//Still firing
line 1511
;1511:				pm->ps->weaponstate = WEAPON_FIRING;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 152
ADDP4
CNSTI4 3
ASGNI4
line 1512
;1512:			}
ADDRGP4 $606
JUMPV
LABELV $605
line 1513
;1513:			else if ( pm->ps->weaponstate != WEAPON_READY )
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 152
ADDP4
INDIRI4
CNSTI4 0
EQI4 $607
line 1514
;1514:			{
line 1515
;1515:				pm->ps->weaponstate = WEAPON_IDLE;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 152
ADDP4
CNSTI4 6
ASGNI4
line 1516
;1516:			}
LABELV $607
LABELV $606
line 1518
;1517:			//Check for finishing an anim if necc.
;1518:			if ( curmove >= LS_S_TL2BR && curmove <= LS_S_T2B )
ADDRLP4 76
ADDRLP4 4
INDIRI4
ASGNI4
ADDRLP4 76
INDIRI4
CNSTI4 18
LTI4 $609
ADDRLP4 76
INDIRI4
CNSTI4 24
GTI4 $609
line 1519
;1519:			{//started a swing, must continue from here
line 1520
;1520:				newmove = LS_A_TL2BR + (curmove-LS_S_TL2BR);
ADDRLP4 8
ADDRLP4 4
INDIRI4
CNSTI4 18
SUBI4
CNSTI4 4
ADDI4
ASGNI4
line 1521
;1521:			}
ADDRGP4 $610
JUMPV
LABELV $609
line 1522
;1522:			else if ( curmove >= LS_A_TL2BR && curmove <= LS_A_T2B )
ADDRLP4 80
ADDRLP4 4
INDIRI4
ASGNI4
ADDRLP4 80
INDIRI4
CNSTI4 4
LTI4 $611
ADDRLP4 80
INDIRI4
CNSTI4 10
GTI4 $611
line 1523
;1523:			{//finished an attack, must continue from here
line 1524
;1524:				newmove = LS_R_TL2BR + (curmove-LS_A_TL2BR);
ADDRLP4 8
ADDRLP4 4
INDIRI4
CNSTI4 4
SUBI4
CNSTI4 25
ADDI4
ASGNI4
line 1525
;1525:			}
ADDRGP4 $612
JUMPV
LABELV $611
line 1526
;1526:			else if ( PM_SaberInTransition( curmove ) )
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 84
ADDRGP4 PM_SaberInTransition
CALLI4
ASGNI4
ADDRLP4 84
INDIRI4
CNSTI4 0
EQI4 $613
line 1527
;1527:			{//in a transition, must play sequential attack
line 1528
;1528:				newmove = saberMoveData[curmove].chain_attack;
ADDRLP4 8
CNSTI4 40
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 saberMoveData+32
ADDP4
INDIRI4
ASGNI4
line 1529
;1529:			}
ADDRGP4 $614
JUMPV
LABELV $613
line 1530
;1530:			else if ( PM_SaberInBounce( curmove ) )
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 88
ADDRGP4 PM_SaberInBounce
CALLI4
ASGNI4
ADDRLP4 88
INDIRI4
CNSTI4 0
EQI4 $616
line 1531
;1531:			{//in a bounce
line 1532
;1532:				newmove = saberMoveData[curmove].chain_idle;//oops, not attacking, so don't chain
ADDRLP4 8
CNSTI4 40
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 saberMoveData+28
ADDP4
INDIRI4
ASGNI4
line 1533
;1533:			}
ADDRGP4 $617
JUMPV
LABELV $616
line 1535
;1534:			else
;1535:			{//FIXME: what about returning from a parry?
line 1536
;1536:				PM_SetSaberMove( LS_READY );
CNSTI4 1
ARGI4
ADDRGP4 PM_SetSaberMove
CALLV
pop
line 1537
;1537:				return;
ADDRGP4 $465
JUMPV
LABELV $617
LABELV $614
LABELV $612
LABELV $610
line 1539
;1538:			}
;1539:		}
LABELV $603
line 1544
;1540:
;1541:		// ***************************************************
;1542:		// Pressing attack, so we must look up the proper attack move.
;1543:
;1544:		saberInAir = qtrue;
ADDRLP4 24
CNSTI4 1
ASGNI4
line 1546
;1545:
;1546:		if ( pm->ps->weaponTime > 0 )
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
CNSTI4 0
LEI4 $619
line 1547
;1547:		{	// Last attack is not yet complete.
line 1548
;1548:			pm->ps->weaponstate = WEAPON_FIRING;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 152
ADDP4
CNSTI4 3
ASGNI4
line 1549
;1549:			return;
ADDRGP4 $465
JUMPV
LABELV $619
line 1552
;1550:		}
;1551:		else
;1552:		{
line 1553
;1553:			int	both = qfalse;
ADDRLP4 76
CNSTI4 0
ASGNI4
line 1555
;1554:
;1555:			if ( curmove >= LS_PARRY_UP && curmove <= LS_REFLECT_LL )
ADDRLP4 80
ADDRLP4 4
INDIRI4
ASGNI4
ADDRLP4 80
INDIRI4
CNSTI4 108
LTI4 $621
ADDRLP4 80
INDIRI4
CNSTI4 117
GTI4 $621
line 1556
;1556:			{//from a parry or deflection, can go directly into an attack (?)
line 1557
;1557:				switch ( saberMoveData[curmove].endQuad )
ADDRLP4 84
CNSTI4 40
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 saberMoveData+12
ADDP4
INDIRI4
ASGNI4
ADDRLP4 84
INDIRI4
CNSTI4 0
LTI4 $623
ADDRLP4 84
INDIRI4
CNSTI4 6
GTI4 $623
ADDRLP4 84
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $632
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $632
address $630
address $623
address $628
address $627
address $629
address $623
address $631
code
line 1558
;1558:				{
LABELV $627
line 1560
;1559:				case Q_T:
;1560:					newmove = LS_A_T2B;
ADDRLP4 8
CNSTI4 10
ASGNI4
line 1561
;1561:					break;
ADDRGP4 $624
JUMPV
LABELV $628
line 1563
;1562:				case Q_TR:
;1563:					newmove = LS_A_TL2BR;
ADDRLP4 8
CNSTI4 4
ASGNI4
line 1564
;1564:					break;
ADDRGP4 $624
JUMPV
LABELV $629
line 1566
;1565:				case Q_TL:
;1566:					newmove = LS_A_TR2BL;
ADDRLP4 8
CNSTI4 9
ASGNI4
line 1567
;1567:					break;
ADDRGP4 $624
JUMPV
LABELV $630
line 1569
;1568:				case Q_BR:
;1569:					newmove = LS_A_BR2TL;
ADDRLP4 8
CNSTI4 7
ASGNI4
line 1570
;1570:					break;
ADDRGP4 $624
JUMPV
LABELV $631
line 1572
;1571:				case Q_BL:
;1572:					newmove = LS_A_BL2TR;
ADDRLP4 8
CNSTI4 6
ASGNI4
line 1573
;1573:					break;
LABELV $623
LABELV $624
line 1576
;1574:				//shouldn't be a parry that ends at L, R or B
;1575:				}
;1576:			}
LABELV $621
line 1578
;1577:
;1578:			if ( newmove != LS_NONE )
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $633
line 1579
;1579:			{//have a valid, final LS_ move picked, so skip findingt he transition move and just get the anim
line 1580
;1580:				anim = saberMoveData[newmove].animToUse;
ADDRLP4 16
CNSTI4 40
ADDRLP4 8
INDIRI4
MULI4
ADDRGP4 saberMoveData+4
ADDP4
INDIRI4
ASGNI4
line 1581
;1581:			}
LABELV $633
line 1584
;1582:
;1583:			//FIXME: diagonal dirs use the figure-eight attacks from ready pose?
;1584:			if ( anim == -1 )
ADDRLP4 16
INDIRI4
CNSTI4 -1
NEI4 $636
line 1585
;1585:			{
line 1587
;1586:				//FIXME: take FP_SABER_OFFENSE into account here somehow?
;1587:				if ( curmove >= LS_T1_BR__R && curmove <= LS_T1_BL__L )
ADDRLP4 84
ADDRLP4 4
INDIRI4
ASGNI4
ADDRLP4 84
INDIRI4
CNSTI4 32
LTI4 $638
ADDRLP4 84
INDIRI4
CNSTI4 73
GTI4 $638
line 1588
;1588:				{//in a transition, must play sequential attack
line 1589
;1589:					newmove = saberMoveData[curmove].chain_attack;
ADDRLP4 8
CNSTI4 40
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 saberMoveData+32
ADDP4
INDIRI4
ASGNI4
line 1590
;1590:				}
ADDRGP4 $639
JUMPV
LABELV $638
line 1591
;1591:				else if ( curmove >= LS_S_TL2BR && curmove <= LS_S_T2B )
ADDRLP4 88
ADDRLP4 4
INDIRI4
ASGNI4
ADDRLP4 88
INDIRI4
CNSTI4 18
LTI4 $641
ADDRLP4 88
INDIRI4
CNSTI4 24
GTI4 $641
line 1592
;1592:				{//started a swing, must continue from here
line 1593
;1593:					newmove = LS_A_TL2BR + (curmove-LS_S_TL2BR);
ADDRLP4 8
ADDRLP4 4
INDIRI4
CNSTI4 18
SUBI4
CNSTI4 4
ADDI4
ASGNI4
line 1594
;1594:				}
ADDRGP4 $642
JUMPV
LABELV $641
line 1596
;1595:				else
;1596:				{
line 1597
;1597:					if ( PM_SaberKataDone() )
ADDRLP4 92
ADDRGP4 PM_SaberKataDone
CALLI4
ASGNI4
ADDRLP4 92
INDIRI4
CNSTI4 0
EQI4 $643
line 1598
;1598:					{//we came from a bounce and cannot chain to another attack because our kata is done
line 1599
;1599:						newmove = saberMoveData[curmove].chain_idle;
ADDRLP4 8
CNSTI4 40
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 saberMoveData+28
ADDP4
INDIRI4
ASGNI4
line 1600
;1600:					}
ADDRGP4 $644
JUMPV
LABELV $643
line 1602
;1601:					else
;1602:					{
line 1603
;1603:						saberMoveName_t checkMove = PM_SaberAttackForMovement(curmove);
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 100
ADDRGP4 PM_SaberAttackForMovement
CALLI4
ASGNI4
ADDRLP4 96
ADDRLP4 100
INDIRI4
ASGNI4
line 1604
;1604:						if (checkMove != -1)
ADDRLP4 96
INDIRI4
CNSTI4 -1
EQI4 $646
line 1605
;1605:						{
line 1606
;1606:							newmove = checkMove;
ADDRLP4 8
ADDRLP4 96
INDIRI4
ASGNI4
line 1607
;1607:						}
LABELV $646
line 1608
;1608:					}
LABELV $644
line 1609
;1609:				}
LABELV $642
LABELV $639
line 1617
;1610:				/*
;1611:				if ( newmove == LS_NONE )
;1612:				{//FIXME: should we allow this?  Are there some anims that you should never be able to chain into an attack?
;1613:					//only curmove that might get in here is LS_NONE, LS_DRAW, LS_PUTAWAY and the LS_R_ returns... all of which are in Q_R
;1614:					newmove = PM_AttackMoveForQuad( saberMoveData[curmove].endQuad );
;1615:				}
;1616:				*/
;1617:				if ( newmove != LS_NONE )
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $648
line 1618
;1618:				{
line 1620
;1619:					//Now get the proper transition move
;1620:					newmove = PM_SaberAnimTransitionAnim( curmove, newmove );
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 8
INDIRI4
ARGI4
ADDRLP4 92
ADDRGP4 PM_SaberAnimTransitionAnim
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 92
INDIRI4
ASGNI4
line 1626
;1621:					// NOTENOTE Had to remove this concept since there is no gent in pmove.
;1622:					/*
;1623:					if ( PM_HasAnimation( pm->gent, saberMoveData[newmove].animToUse ) )
;1624:					*/
;1625:
;1626:					assert(	bgGlobalAnimations[saberMoveData[newmove].animToUse].firstFrame != 0 || 
line 1629
;1627:							bgGlobalAnimations[saberMoveData[newmove].animToUse].numFrames != 0);
;1628:
;1629:					if (1)
line 1630
;1630:					{
line 1631
;1631:						anim = saberMoveData[newmove].animToUse;
ADDRLP4 16
CNSTI4 40
ADDRLP4 8
INDIRI4
MULI4
ADDRGP4 saberMoveData+4
ADDP4
INDIRI4
ASGNI4
line 1632
;1632:					}
LABELV $650
line 1633
;1633:				}
LABELV $648
line 1634
;1634:			}
LABELV $636
line 1636
;1635:
;1636:			if (anim == -1)
ADDRLP4 16
INDIRI4
CNSTI4 -1
NEI4 $653
line 1637
;1637:			{//not side-stepping, pick neutral anim
line 1639
;1638:				// Add randomness for prototype?
;1639:				newmove = saberMoveData[curmove].chain_attack;
ADDRLP4 8
CNSTI4 40
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 saberMoveData+32
ADDP4
INDIRI4
ASGNI4
line 1641
;1640:
;1641:				anim= saberMoveData[newmove].animToUse;
ADDRLP4 16
CNSTI4 40
ADDRLP4 8
INDIRI4
MULI4
ADDRGP4 saberMoveData+4
ADDP4
INDIRI4
ASGNI4
line 1643
;1642:
;1643:				if ( !pm->cmd.forwardmove && !pm->cmd.rightmove && pm->cmd.upmove >= 0 && pm->ps->groundEntityNum != ENTITYNUM_NONE )
ADDRLP4 84
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 88
CNSTI4 0
ASGNI4
ADDRLP4 84
INDIRP4
CNSTI4 28
ADDP4
INDIRI1
CVII4 1
ADDRLP4 88
INDIRI4
NEI4 $657
ADDRLP4 84
INDIRP4
CNSTI4 29
ADDP4
INDIRI1
CVII4 1
ADDRLP4 88
INDIRI4
NEI4 $657
ADDRLP4 84
INDIRP4
CNSTI4 30
ADDP4
INDIRI1
CVII4 1
ADDRLP4 88
INDIRI4
LTI4 $657
ADDRLP4 84
INDIRP4
INDIRP4
CNSTI4 84
ADDP4
INDIRI4
CNSTI4 1023
EQI4 $657
line 1644
;1644:				{//not moving at all, so set the anim on entire body
line 1645
;1645:					both = qtrue;
ADDRLP4 76
CNSTI4 1
ASGNI4
line 1646
;1646:				}
LABELV $657
line 1648
;1647:			
;1648:			}
LABELV $653
line 1650
;1649:
;1650:			if ( anim == -1)
ADDRLP4 16
INDIRI4
CNSTI4 -1
NEI4 $659
line 1651
;1651:			{
line 1652
;1652:				if((pm->ps->legsAnim & ~ANIM_TOGGLEBIT) == BOTH_WALK1 )
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 92
ADDP4
INDIRI4
CNSTI4 -2049
BANDI4
CNSTI4 723
NEI4 $661
line 1653
;1653:				{
line 1654
;1654:					anim = BOTH_WALK1;
ADDRLP4 16
CNSTI4 723
ASGNI4
line 1655
;1655:				}
ADDRGP4 $662
JUMPV
LABELV $661
line 1656
;1656:				else if((pm->ps->legsAnim & ~ANIM_TOGGLEBIT) == BOTH_RUN2 )
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 92
ADDP4
INDIRI4
CNSTI4 -2049
BANDI4
CNSTI4 734
NEI4 $663
line 1657
;1657:				{
line 1658
;1658:					anim = BOTH_RUN2;
ADDRLP4 16
CNSTI4 734
ASGNI4
line 1659
;1659:				}
ADDRGP4 $664
JUMPV
LABELV $663
line 1660
;1660:				else if((pm->ps->legsAnim & ~ANIM_TOGGLEBIT) == BOTH_WALK2 )
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 92
ADDP4
INDIRI4
CNSTI4 -2049
BANDI4
CNSTI4 724
NEI4 $665
line 1661
;1661:				{
line 1662
;1662:					anim = BOTH_WALK2;
ADDRLP4 16
CNSTI4 724
ASGNI4
line 1663
;1663:				}
ADDRGP4 $666
JUMPV
LABELV $665
line 1665
;1664:				else
;1665:				{
line 1666
;1666:					anim = PM_GetSaberStance();
ADDRLP4 84
ADDRGP4 PM_GetSaberStance
CALLI4
ASGNI4
ADDRLP4 16
ADDRLP4 84
INDIRI4
ASGNI4
line 1667
;1667:				}
LABELV $666
LABELV $664
LABELV $662
line 1668
;1668:				newmove = LS_READY;
ADDRLP4 8
CNSTI4 1
ASGNI4
line 1669
;1669:			}
LABELV $659
line 1671
;1670:
;1671:			if ( !pm->ps->saberActive )
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 508
ADDP4
INDIRI4
CNSTI4 0
NEI4 $667
line 1672
;1672:			{//turn on the saber if it's not on
line 1673
;1673:				pm->ps->saberActive = qtrue;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 1
ASGNI4
line 1674
;1674:			}
LABELV $667
line 1676
;1675:
;1676:			PM_SetSaberMove( newmove );
ADDRLP4 8
INDIRI4
CVII2 4
CVII4 2
ARGI4
ADDRGP4 PM_SetSaberMove
CALLV
pop
line 1678
;1677:
;1678:			if ( both )
ADDRLP4 76
INDIRI4
CNSTI4 0
EQI4 $669
line 1679
;1679:			{
line 1680
;1680:				PM_SetAnim(SETANIM_LEGS,anim,SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD, 100);
CNSTI4 2
ARGI4
ADDRLP4 16
INDIRI4
ARGI4
CNSTI4 3
ARGI4
CNSTI4 100
ARGI4
ADDRGP4 PM_SetAnim
CALLV
pop
line 1681
;1681:			}
LABELV $669
line 1684
;1682:
;1683:			//don't fire again until anim is done
;1684:			pm->ps->weaponTime = pm->ps->torsoTimer;
ADDRLP4 84
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 84
INDIRP4
CNSTI4 44
ADDP4
ADDRLP4 84
INDIRP4
CNSTI4 96
ADDP4
INDIRI4
ASGNI4
line 1685
;1685:		}
line 1686
;1686:	}
LABELV $599
line 1692
;1687:
;1688:	// *********************************************************
;1689:	// WEAPON_FIRING
;1690:	// *********************************************************
;1691:
;1692:	pm->ps->weaponstate = WEAPON_FIRING;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 152
ADDP4
CNSTI4 3
ASGNI4
line 1694
;1693:
;1694:	amount = weaponData[pm->ps->weapon].energyPerShot;
ADDRLP4 28
CNSTI4 56
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 148
ADDP4
INDIRI4
MULI4
ADDRGP4 weaponData+8
ADDP4
INDIRI4
ASGNI4
line 1696
;1695:
;1696:	addTime = pm->ps->weaponTime;
ADDRLP4 0
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
ASGNI4
line 1698
;1697:
;1698:	pm->ps->saberAttackSequence = pm->ps->torsoAnim;
ADDRLP4 72
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 72
INDIRP4
CNSTI4 732
ADDP4
ADDRLP4 72
INDIRP4
CNSTI4 100
ADDP4
INDIRI4
ASGNI4
line 1699
;1699:	if ( !addTime )
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $672
line 1700
;1700:	{
line 1701
;1701:		addTime = weaponData[pm->ps->weapon].fireTime;
ADDRLP4 0
CNSTI4 56
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 148
ADDP4
INDIRI4
MULI4
ADDRGP4 weaponData+12
ADDP4
INDIRI4
ASGNI4
line 1702
;1702:	}
LABELV $672
line 1703
;1703:	pm->ps->weaponTime = addTime;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 44
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 1704
;1704:}
LABELV $465
endproc PM_WeaponLightsaber 104 16
export PM_SetSaberMove
proc PM_SetSaberMove 52 16
ADDRFP4 0
ADDRFP4 0
INDIRI4
CVII2 4
ASGNI2
line 1707
;1705:
;1706:void PM_SetSaberMove(short newMove)
;1707:{
line 1708
;1708:	unsigned int setflags = saberMoveData[newMove].animSetFlags;
ADDRLP4 4
CNSTI4 40
ADDRFP4 0
INDIRI2
CVII4 2
MULI4
ADDRGP4 saberMoveData+16
ADDP4
INDIRU4
ASGNU4
line 1709
;1709:	int	anim = saberMoveData[newMove].animToUse;
ADDRLP4 0
CNSTI4 40
ADDRFP4 0
INDIRI2
CVII4 2
MULI4
ADDRGP4 saberMoveData+4
ADDP4
INDIRI4
ASGNI4
line 1710
;1710:	int parts = SETANIM_TORSO;
ADDRLP4 8
CNSTI4 1
ASGNI4
line 1712
;1711:	
;1712:	if ( newMove == LS_READY )
ADDRFP4 0
INDIRI2
CVII4 2
CNSTI4 1
NEI4 $678
line 1713
;1713:	{//finished with a kata, reset attack counter
line 1714
;1714:		pm->ps->saberAttackChainCount = 0;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 1296
ADDP4
CNSTI4 0
ASGNI4
line 1715
;1715:	}
ADDRGP4 $679
JUMPV
LABELV $678
line 1716
;1716:	else if ( BG_SaberInAttack( newMove ) )
ADDRFP4 0
INDIRI2
CVII4 2
ARGI4
ADDRLP4 12
ADDRGP4 BG_SaberInAttack
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $680
line 1717
;1717:	{//continuing with a kata, increment attack counter
line 1718
;1718:		pm->ps->saberAttackChainCount++;
ADDRLP4 16
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 1296
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1719
;1719:	}
LABELV $680
LABELV $679
line 1721
;1720:
;1721:	if (pm->ps->saberAttackChainCount > 16)
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 1296
ADDP4
INDIRI4
CNSTI4 16
LEI4 $682
line 1722
;1722:	{ //for the sake of being able to send the value over the net within a reasonable bit count
line 1723
;1723:		pm->ps->saberAttackChainCount = 16;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 1296
ADDP4
CNSTI4 16
ASGNI4
line 1724
;1724:	}
LABELV $682
line 1726
;1725:
;1726:	if ( pm->ps->fd.saberAnimLevel > FORCE_LEVEL_1 &&
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 1216
ADDP4
INDIRI4
CNSTI4 1
LEI4 $684
ADDRFP4 0
INDIRI2
CVII4 2
ARGI4
ADDRLP4 16
ADDRGP4 BG_SaberInIdle
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
NEI4 $684
ADDRFP4 0
INDIRI2
CVII4 2
ARGI4
ADDRLP4 20
ADDRGP4 PM_SaberInParry
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
NEI4 $684
ADDRFP4 0
INDIRI2
CVII4 2
ARGI4
ADDRLP4 24
ADDRGP4 PM_SaberInReflect
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
NEI4 $684
ADDRFP4 0
INDIRI2
CVII4 2
ARGI4
ADDRLP4 28
ADDRGP4 BG_SaberInSpecial
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
NEI4 $684
line 1728
;1727:		 !BG_SaberInIdle( newMove ) && !PM_SaberInParry( newMove ) && !PM_SaberInReflect( newMove ) && !BG_SaberInSpecial(newMove))
;1728:	{//readies, parries and reflections have only 1 level 
line 1730
;1729:		//increment the anim to the next level of saber anims
;1730:		if ( !PM_SaberInTransition( newMove ) )
ADDRFP4 0
INDIRI2
CVII4 2
ARGI4
ADDRLP4 32
ADDRGP4 PM_SaberInTransition
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
NEI4 $686
line 1731
;1731:		{//FIXME: only have level 1 transitions for now
line 1732
;1732:			anim += (pm->ps->fd.saberAnimLevel-FORCE_LEVEL_1) * SABER_ANIM_GROUP_SIZE;
ADDRLP4 36
CNSTI4 77
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 36
INDIRI4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 1216
ADDP4
INDIRI4
MULI4
ADDRLP4 36
INDIRI4
SUBI4
ADDI4
ASGNI4
line 1733
;1733:		}
LABELV $686
line 1734
;1734:	}
LABELV $684
line 1737
;1735:
;1736:	// If the move does the same animation as the last one, we need to force a restart...
;1737:	if ( saberMoveData[pm->ps->saberMove].animToUse == anim && newMove > LS_PUTAWAY)
CNSTI4 40
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 512
ADDP4
INDIRI4
MULI4
ADDRGP4 saberMoveData+4
ADDP4
INDIRI4
ADDRLP4 0
INDIRI4
NEI4 $688
ADDRFP4 0
INDIRI2
CVII4 2
CNSTI4 3
LEI4 $688
line 1738
;1738:	{
line 1739
;1739:		setflags |= SETANIM_FLAG_RESTART;
ADDRLP4 4
ADDRLP4 4
INDIRU4
CNSTU4 4
BORU4
ASGNU4
line 1740
;1740:	}
LABELV $688
line 1743
;1741:
;1742:	//saber torso anims should always be highest priority
;1743:	setflags |= SETANIM_FLAG_OVERRIDE;
ADDRLP4 4
ADDRLP4 4
INDIRU4
CNSTU4 1
BORU4
ASGNU4
line 1745
;1744:
;1745:	if ( BG_InSaberStandAnim(anim) || anim == BOTH_STAND1 )
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 32
ADDRGP4 BG_InSaberStandAnim
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
NEI4 $693
ADDRLP4 0
INDIRI4
CNSTI4 550
NEI4 $691
LABELV $693
line 1746
;1746:	{
line 1747
;1747:		anim = (pm->ps->legsAnim & ~ANIM_TOGGLEBIT);
ADDRLP4 0
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 92
ADDP4
INDIRI4
CNSTI4 -2049
BANDI4
ASGNI4
line 1749
;1748:
;1749:		if ((anim >= BOTH_STAND1 && anim <= BOTH_STAND4TOATTACK2) ||
ADDRLP4 0
INDIRI4
CNSTI4 550
LTI4 $697
ADDRLP4 0
INDIRI4
CNSTI4 571
LEI4 $696
LABELV $697
ADDRLP4 0
INDIRI4
CNSTI4 954
LTI4 $694
ADDRLP4 0
INDIRI4
CNSTI4 985
GTI4 $694
LABELV $696
line 1751
;1750:			(anim >= TORSO_DROPWEAP1 && anim <= TORSO_WEAPONIDLE12))
;1751:		{ //If standing then use the special saber stand anim
line 1752
;1752:			anim = PM_GetSaberStance();
ADDRLP4 44
ADDRGP4 PM_GetSaberStance
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 44
INDIRI4
ASGNI4
line 1753
;1753:		}
LABELV $694
line 1755
;1754:
;1755:		if (pm->ps->pm_flags & PMF_DUCKED)
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $698
line 1756
;1756:		{ //Playing torso walk anims while crouched makes you look like a monkey
line 1757
;1757:			anim = PM_GetSaberStance();
ADDRLP4 44
ADDRGP4 PM_GetSaberStance
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 44
INDIRI4
ASGNI4
line 1758
;1758:		}
LABELV $698
line 1760
;1759:
;1760:		if (anim == BOTH_WALKBACK1 || anim == BOTH_WALKBACK2)
ADDRLP4 0
INDIRI4
CNSTI4 748
EQI4 $702
ADDRLP4 0
INDIRI4
CNSTI4 749
NEI4 $700
LABELV $702
line 1761
;1761:		{ //normal stance when walking backward so saber doesn't look like it's cutting through leg
line 1762
;1762:			anim = PM_GetSaberStance();
ADDRLP4 48
ADDRGP4 PM_GetSaberStance
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 48
INDIRI4
ASGNI4
line 1763
;1763:		}
LABELV $700
line 1765
;1764:
;1765:		parts = SETANIM_TORSO;
ADDRLP4 8
CNSTI4 1
ASGNI4
line 1766
;1766:	}
LABELV $691
line 1768
;1767:
;1768:	if ( newMove == LS_A_LUNGE 
ADDRLP4 36
ADDRFP4 0
INDIRI2
CVII4 2
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 14
EQI4 $710
ADDRLP4 36
INDIRI4
CNSTI4 15
EQI4 $710
ADDRLP4 36
INDIRI4
CNSTI4 11
EQI4 $710
ADDRLP4 36
INDIRI4
CNSTI4 12
EQI4 $710
ADDRLP4 36
INDIRI4
CNSTI4 13
EQI4 $710
ADDRLP4 36
INDIRI4
CNSTI4 16
EQI4 $710
ADDRLP4 36
INDIRI4
CNSTI4 17
NEI4 $703
LABELV $710
line 1775
;1769:		|| newMove == LS_A_JUMP_T__B_ 
;1770:		|| newMove == LS_A_BACKSTAB
;1771:		|| newMove == LS_A_BACK
;1772:		|| newMove == LS_A_BACK_CR
;1773:		|| newMove == LS_A_FLIP_STAB
;1774:		|| newMove == LS_A_FLIP_SLASH )
;1775:	{
line 1776
;1776:		parts = SETANIM_BOTH;
ADDRLP4 8
CNSTI4 3
ASGNI4
line 1777
;1777:	}
ADDRGP4 $704
JUMPV
LABELV $703
line 1778
;1778:	else if ( BG_SpinningSaberAnim( anim ) )
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 40
ADDRGP4 BG_SpinningSaberAnim
CALLI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 0
EQI4 $711
line 1779
;1779:	{//spins must be played on entire body
line 1780
;1780:		parts = SETANIM_BOTH;
ADDRLP4 8
CNSTI4 3
ASGNI4
line 1781
;1781:	}
LABELV $711
LABELV $704
line 1782
;1782:	PM_SetAnim(parts, anim, setflags|SETANIM_FLAG_HOLD, saberMoveData[newMove].blendTime);
ADDRLP4 8
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
INDIRU4
CNSTU4 2
BORU4
CVUI4 4
ARGI4
CNSTI4 40
ADDRFP4 0
INDIRI2
CVII4 2
MULI4
ADDRGP4 saberMoveData+20
ADDP4
INDIRI4
ARGI4
ADDRGP4 PM_SetAnim
CALLV
pop
line 1784
;1783:
;1784:	if ( (pm->ps->torsoAnim&~ANIM_TOGGLEBIT) == anim )
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 100
ADDP4
INDIRI4
CNSTI4 -2049
BANDI4
ADDRLP4 0
INDIRI4
NEI4 $714
line 1785
;1785:	{//successfully changed anims
line 1788
;1786:	//special check for *starting* a saber swing
;1787:		//playing at attack
;1788:		if ( BG_SaberInAttack( newMove ) || BG_SaberInSpecialAttack( anim ) )
ADDRFP4 0
INDIRI2
CVII4 2
ARGI4
ADDRLP4 44
ADDRGP4 BG_SaberInAttack
CALLI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 0
NEI4 $718
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 48
ADDRGP4 BG_SaberInSpecialAttack
CALLI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 0
EQI4 $716
LABELV $718
line 1789
;1789:		{
line 1790
;1790:			if ( pm->ps->saberMove != newMove )
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 512
ADDP4
INDIRI4
ADDRFP4 0
INDIRI2
CVII4 2
EQI4 $719
line 1791
;1791:			{//wasn't playing that attack before
line 1792
;1792:				PM_AddEvent(EV_SABER_ATTACK);
CNSTI4 26
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 1793
;1793:			}
LABELV $719
line 1794
;1794:		}
LABELV $716
line 1796
;1795:
;1796:		pm->ps->saberMove = newMove;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 512
ADDP4
ADDRFP4 0
INDIRI2
CVII4 2
ASGNI4
line 1797
;1797:		pm->ps->saberBlocking = saberMoveData[newMove].blocking;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 516
ADDP4
CNSTI4 40
ADDRFP4 0
INDIRI2
CVII4 2
MULI4
ADDRGP4 saberMoveData+24
ADDP4
INDIRI4
ASGNI4
line 1799
;1798:
;1799:		pm->ps->torsoAnim = anim;//saberMoveData[newMove].animToUse;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 100
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 1801
;1800:
;1801:		if (pm->ps->weaponTime <= 0)
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
CNSTI4 0
GTI4 $722
line 1802
;1802:		{
line 1803
;1803:			pm->ps->saberBlocked = BLOCKED_NONE;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 520
ADDP4
CNSTI4 0
ASGNI4
line 1804
;1804:		}
LABELV $722
line 1805
;1805:	}
LABELV $714
line 1806
;1806:}
LABELV $675
endproc PM_SetSaberMove 52 16
import ValidAnimFileIndex
import PM_SetAnim
import PM_FinishWeaponChange
import PM_BeginWeaponChange
import PM_ForceLegsAnim
import PM_ContinueLegsAnim
import PM_StartTorsoAnim
import BG_CycleInven
import PM_StepSlideMove
import PM_SlideMove
import PM_AddEvent
import PM_AddTouchEnt
import PM_ClipVelocity
import PM_GetSaberStance
import PM_AnimLength
import PM_InRollComplete
import PM_InOnGroundAnim
import PM_SpinningAnim
import PM_LandingAnim
import PM_JumpingAnim
import PM_PainAnim
import PM_InKnockDown
import PM_InSaberAnim
import PM_SaberInStart
import PM_SaberInReflect
import PM_SaberInParry
import trap_FS_FCloseFile
import trap_FS_Write
import trap_FS_Read
import trap_FS_FOpenFile
import forcePowerNeeded
import c_pmove
import pm_flightfriction
import pm_waterfriction
import pm_friction
import pm_flyaccelerate
import pm_wateraccelerate
import pm_airaccelerate
import pm_accelerate
import pm_wadeScale
import pm_swimScale
import pm_duckScale
import pm_stopspeed
import pml
import forcePowerDarkLight
import WeaponAttackAnim
import WeaponReadyAnim
import BG_OutOfMemory
import BG_StringAlloc
import BG_TempFree
import BG_TempAlloc
import BG_AllocUnaligned
import BG_Alloc
import BG_CanUseFPNow
import BG_HasYsalamiri
import BG_GetItemIndexByTag
import BG_ParseAnimationFile
import BG_PlayerTouchesItem
import BG_G2PlayerAngles
import BG_PlayerStateToEntityStateExtraPolate
import BG_PlayerStateToEntityState
import BG_TouchJumpPad
import BG_AddPredictableEventToPlayerstate
import BG_EvaluateTrajectoryDelta
import BG_EvaluateTrajectory
import BG_SaberStartTransAnim
import BG_InDeathAnim
import BG_InRoll
import BG_SaberInSpecialAttack
import BG_SpinningSaberAnim
import BG_FlippingAnim
import BG_SaberInIdle
import BG_SaberInSpecial
import BG_SaberInAttack
import BG_DirectFlippingAnim
import BG_InSaberStandAnim
import BG_InSpecialJump
import BG_LegalizedForcePowers
import BG_CanItemBeGrabbed
import BG_FindItemForHoldable
import BG_FindItemForPowerup
import BG_FindItemForWeapon
import BG_FindItem
import vectoyaw
import bg_numItems
import bg_itemlist
import Pmove
import PM_UpdateViewAngles
import pm
import bgForcePowerCost
import forceMasteryPoints
import forceMasteryLevels
import bgGlobalAnimations
import BGPAFtextLoaded
import forcePowerSorted
import WP_MuzzlePoint
import ammoData
import weaponData
import GetStringForID
import GetIDForString
import Q_irand
import irand
import flrand
import Rand_Init
import Com_Printf
import Com_Error
import Info_NextPair
import Info_Validate
import Info_SetValueForKey_Big
import Info_SetValueForKey
import Info_RemoveKey_big
import Info_RemoveKey
import Info_ValueForKey
import va
import Q_CleanStr
import Q_PrintStrlen
import Q_strcat
import Q_strncpyz
import Q_strrchr
import Q_strupr
import Q_strlwr
import Q_stricmpn
import Q_strncmp
import Q_stricmp
import Q_isalpha
import Q_isupper
import Q_islower
import Q_isprint
import Com_sprintf
import Parse3DMatrix
import Parse2DMatrix
import Parse1DMatrix
import SkipRestOfLine
import SkipBracedSection
import COM_MatchToken
import COM_ParseWarning
import COM_ParseError
import COM_Compress
import COM_ParseExt
import COM_Parse
import SkipWhitespace
import COM_GetCurrentParseLine
import COM_BeginParseSession
import COM_DefaultExtension
import COM_StripExtension
import COM_SkipPath
import Com_Clamp
import PerpendicularVector
import AngleVectors
import MatrixMultiply
import MakeNormalVectors
import RotateAroundDirection
import RotatePointAroundVector
import ProjectPointOnPlane
import PlaneFromPoints
import AngleDelta
import AngleNormalize180
import AngleNormalize360
import AnglesSubtract
import AngleSubtract
import LerpAngle
import AngleMod
import BoxOnPlaneSide
import SetPlaneSignbits
import AxisCopy
import AxisClear
import AnglesToAxis
import vectoangles
import Q_crandom
import Q_random
import Q_rand
import Q_acos
import Q_log2
import VectorRotate
import Vector4Scale
import VectorNormalize2
import VectorNormalize
import CrossProduct
import VectorInverse
import VectorNormalizeFast
import DistanceSquared
import Distance
import VectorLengthSquared
import VectorLength
import VectorCompare
import AddPointToBounds
import ClearBounds
import RadiusFromBounds
import NormalizeColor
import ColorBytes4
import ColorBytes3
import _VectorMA
import _VectorScale
import _VectorCopy
import _VectorAdd
import _VectorSubtract
import _DotProduct
import ByteToDir
import DirToByte
import powf
import ClampShort
import ClampChar
import Q_rsqrt
import Q_fabs
import axisDefault
import vec3_origin
import g_color_table
import colorDkBlue
import colorLtBlue
import colorDkGrey
import colorMdGrey
import colorLtGrey
import colorWhite
import colorCyan
import colorMagenta
import colorYellow
import colorBlue
import colorGreen
import colorRed
import colorBlack
import colorTable
import bytedirs
import Com_Memcpy
import Com_Memset
import Hunk_Alloc
import forceSpeedLevels
import FloatSwap
import LongSwap
import ShortSwap
import acos
import fabs
import abs
import tan
import atan2
import cos
import sin
import sqrt
import floor
import ceil
import memcpy
import memset
import memmove
import sscanf
import vsprintf
import _atoi
import atoi
import _atof
import atof
import toupper
import tolower
import strncpy
import strstr
import strchr
import strcmp
import strcpy
import strcat
import strlen
import rand
import srand
import qsort
lit
align 1
LABELV $212
char 1 82
char 1 101
char 1 102
char 1 108
char 1 101
char 1 99
char 1 116
char 1 32
char 1 76
char 1 76
char 1 0
align 1
LABELV $211
char 1 82
char 1 101
char 1 102
char 1 108
char 1 101
char 1 99
char 1 116
char 1 32
char 1 76
char 1 82
char 1 0
align 1
LABELV $210
char 1 82
char 1 101
char 1 102
char 1 108
char 1 101
char 1 99
char 1 116
char 1 32
char 1 85
char 1 76
char 1 0
align 1
LABELV $209
char 1 82
char 1 101
char 1 102
char 1 108
char 1 101
char 1 99
char 1 116
char 1 32
char 1 85
char 1 82
char 1 0
align 1
LABELV $208
char 1 82
char 1 101
char 1 102
char 1 108
char 1 101
char 1 99
char 1 116
char 1 32
char 1 84
char 1 111
char 1 112
char 1 0
align 1
LABELV $207
char 1 80
char 1 97
char 1 114
char 1 114
char 1 121
char 1 32
char 1 76
char 1 76
char 1 0
align 1
LABELV $206
char 1 80
char 1 97
char 1 114
char 1 114
char 1 121
char 1 32
char 1 76
char 1 82
char 1 0
align 1
LABELV $205
char 1 80
char 1 97
char 1 114
char 1 114
char 1 121
char 1 32
char 1 85
char 1 76
char 1 0
align 1
LABELV $204
char 1 80
char 1 97
char 1 114
char 1 114
char 1 121
char 1 32
char 1 85
char 1 82
char 1 0
align 1
LABELV $203
char 1 80
char 1 97
char 1 114
char 1 114
char 1 121
char 1 32
char 1 84
char 1 111
char 1 112
char 1 0
align 1
LABELV $202
char 1 75
char 1 110
char 1 111
char 1 99
char 1 107
char 1 32
char 1 76
char 1 76
char 1 0
align 1
LABELV $201
char 1 75
char 1 110
char 1 111
char 1 99
char 1 107
char 1 32
char 1 76
char 1 82
char 1 0
align 1
LABELV $200
char 1 75
char 1 110
char 1 111
char 1 99
char 1 107
char 1 32
char 1 85
char 1 76
char 1 0
align 1
LABELV $199
char 1 75
char 1 110
char 1 111
char 1 99
char 1 107
char 1 32
char 1 85
char 1 82
char 1 0
align 1
LABELV $198
char 1 75
char 1 110
char 1 111
char 1 99
char 1 107
char 1 32
char 1 84
char 1 111
char 1 112
char 1 0
align 1
LABELV $197
char 1 66
char 1 80
char 1 97
char 1 114
char 1 114
char 1 121
char 1 32
char 1 76
char 1 76
char 1 0
align 1
LABELV $196
char 1 66
char 1 80
char 1 97
char 1 114
char 1 114
char 1 121
char 1 32
char 1 66
char 1 111
char 1 116
char 1 0
align 1
LABELV $195
char 1 66
char 1 80
char 1 97
char 1 114
char 1 114
char 1 121
char 1 32
char 1 76
char 1 82
char 1 0
align 1
LABELV $194
char 1 66
char 1 80
char 1 97
char 1 114
char 1 114
char 1 121
char 1 32
char 1 85
char 1 76
char 1 0
align 1
LABELV $193
char 1 66
char 1 80
char 1 97
char 1 114
char 1 114
char 1 121
char 1 32
char 1 85
char 1 82
char 1 0
align 1
LABELV $192
char 1 66
char 1 80
char 1 97
char 1 114
char 1 114
char 1 121
char 1 32
char 1 84
char 1 111
char 1 112
char 1 0
align 1
LABELV $191
char 1 82
char 1 101
char 1 102
char 1 108
char 1 101
char 1 99
char 1 116
char 1 101
char 1 100
char 1 32
char 1 66
char 1 0
align 1
LABELV $190
char 1 82
char 1 101
char 1 102
char 1 108
char 1 101
char 1 99
char 1 116
char 1 101
char 1 100
char 1 32
char 1 66
char 1 76
char 1 0
align 1
LABELV $189
char 1 82
char 1 101
char 1 102
char 1 108
char 1 101
char 1 99
char 1 116
char 1 101
char 1 100
char 1 32
char 1 76
char 1 0
align 1
LABELV $188
char 1 82
char 1 101
char 1 102
char 1 108
char 1 101
char 1 99
char 1 116
char 1 101
char 1 100
char 1 32
char 1 84
char 1 76
char 1 0
align 1
LABELV $187
char 1 82
char 1 101
char 1 102
char 1 108
char 1 101
char 1 99
char 1 116
char 1 101
char 1 100
char 1 32
char 1 84
char 1 0
align 1
LABELV $186
char 1 82
char 1 101
char 1 102
char 1 108
char 1 101
char 1 99
char 1 116
char 1 101
char 1 100
char 1 32
char 1 84
char 1 82
char 1 0
align 1
LABELV $185
char 1 82
char 1 101
char 1 102
char 1 108
char 1 101
char 1 99
char 1 116
char 1 101
char 1 100
char 1 32
char 1 82
char 1 0
align 1
LABELV $184
char 1 82
char 1 101
char 1 102
char 1 108
char 1 101
char 1 99
char 1 116
char 1 101
char 1 100
char 1 32
char 1 66
char 1 82
char 1 0
align 1
LABELV $183
char 1 68
char 1 101
char 1 102
char 1 108
char 1 101
char 1 99
char 1 116
char 1 32
char 1 66
char 1 0
align 1
LABELV $182
char 1 68
char 1 101
char 1 102
char 1 108
char 1 101
char 1 99
char 1 116
char 1 32
char 1 66
char 1 76
char 1 0
align 1
LABELV $181
char 1 68
char 1 101
char 1 102
char 1 108
char 1 101
char 1 99
char 1 116
char 1 32
char 1 76
char 1 0
align 1
LABELV $180
char 1 68
char 1 101
char 1 102
char 1 108
char 1 101
char 1 99
char 1 116
char 1 32
char 1 84
char 1 76
char 1 0
align 1
LABELV $179
char 1 68
char 1 101
char 1 102
char 1 108
char 1 101
char 1 99
char 1 116
char 1 32
char 1 84
char 1 0
align 1
LABELV $178
char 1 68
char 1 101
char 1 102
char 1 108
char 1 101
char 1 99
char 1 116
char 1 32
char 1 84
char 1 82
char 1 0
align 1
LABELV $177
char 1 68
char 1 101
char 1 102
char 1 108
char 1 101
char 1 99
char 1 116
char 1 32
char 1 82
char 1 0
align 1
LABELV $176
char 1 68
char 1 101
char 1 102
char 1 108
char 1 101
char 1 99
char 1 116
char 1 32
char 1 66
char 1 82
char 1 0
align 1
LABELV $175
char 1 66
char 1 111
char 1 117
char 1 110
char 1 99
char 1 101
char 1 32
char 1 66
char 1 76
char 1 0
align 1
LABELV $174
char 1 66
char 1 111
char 1 117
char 1 110
char 1 99
char 1 101
char 1 32
char 1 76
char 1 0
align 1
LABELV $173
char 1 66
char 1 111
char 1 117
char 1 110
char 1 99
char 1 101
char 1 32
char 1 84
char 1 76
char 1 0
align 1
LABELV $172
char 1 66
char 1 111
char 1 117
char 1 110
char 1 99
char 1 101
char 1 32
char 1 84
char 1 0
align 1
LABELV $171
char 1 66
char 1 111
char 1 117
char 1 110
char 1 99
char 1 101
char 1 32
char 1 84
char 1 82
char 1 0
align 1
LABELV $170
char 1 66
char 1 111
char 1 117
char 1 110
char 1 99
char 1 101
char 1 32
char 1 82
char 1 0
align 1
LABELV $169
char 1 66
char 1 111
char 1 117
char 1 110
char 1 99
char 1 101
char 1 32
char 1 66
char 1 82
char 1 0
align 1
LABELV $168
char 1 66
char 1 76
char 1 50
char 1 76
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $167
char 1 66
char 1 76
char 1 50
char 1 84
char 1 76
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $166
char 1 66
char 1 76
char 1 50
char 1 84
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $165
char 1 66
char 1 76
char 1 50
char 1 84
char 1 82
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $164
char 1 66
char 1 76
char 1 50
char 1 82
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $163
char 1 66
char 1 76
char 1 50
char 1 66
char 1 82
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $162
char 1 76
char 1 50
char 1 66
char 1 76
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $161
char 1 76
char 1 50
char 1 84
char 1 76
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $160
char 1 76
char 1 50
char 1 84
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $159
char 1 76
char 1 50
char 1 84
char 1 82
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $158
char 1 76
char 1 50
char 1 82
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $157
char 1 76
char 1 50
char 1 66
char 1 82
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $156
char 1 84
char 1 76
char 1 50
char 1 66
char 1 76
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $155
char 1 84
char 1 76
char 1 50
char 1 76
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $154
char 1 84
char 1 76
char 1 50
char 1 84
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $153
char 1 84
char 1 76
char 1 50
char 1 84
char 1 82
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $152
char 1 84
char 1 76
char 1 50
char 1 82
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $151
char 1 84
char 1 76
char 1 50
char 1 66
char 1 82
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $150
char 1 84
char 1 50
char 1 66
char 1 76
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $149
char 1 84
char 1 50
char 1 76
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $148
char 1 84
char 1 50
char 1 84
char 1 76
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $147
char 1 84
char 1 50
char 1 84
char 1 82
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $146
char 1 84
char 1 50
char 1 82
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $145
char 1 84
char 1 50
char 1 66
char 1 82
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $144
char 1 84
char 1 82
char 1 50
char 1 66
char 1 76
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $143
char 1 84
char 1 82
char 1 50
char 1 76
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $142
char 1 84
char 1 82
char 1 50
char 1 84
char 1 76
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $141
char 1 84
char 1 82
char 1 50
char 1 84
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $140
char 1 84
char 1 82
char 1 50
char 1 82
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $139
char 1 84
char 1 82
char 1 50
char 1 66
char 1 82
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $138
char 1 82
char 1 50
char 1 66
char 1 76
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $137
char 1 82
char 1 50
char 1 76
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $136
char 1 82
char 1 50
char 1 84
char 1 76
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $135
char 1 82
char 1 50
char 1 84
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $134
char 1 82
char 1 50
char 1 84
char 1 82
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $133
char 1 82
char 1 50
char 1 66
char 1 82
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $132
char 1 66
char 1 82
char 1 50
char 1 66
char 1 76
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $131
char 1 66
char 1 82
char 1 50
char 1 76
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $130
char 1 66
char 1 82
char 1 50
char 1 84
char 1 76
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $129
char 1 66
char 1 82
char 1 50
char 1 84
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $128
char 1 66
char 1 82
char 1 50
char 1 84
char 1 82
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $127
char 1 66
char 1 82
char 1 50
char 1 82
char 1 32
char 1 84
char 1 114
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $126
char 1 84
char 1 50
char 1 66
char 1 32
char 1 82
char 1 101
char 1 116
char 1 0
align 1
LABELV $125
char 1 84
char 1 82
char 1 50
char 1 66
char 1 76
char 1 32
char 1 82
char 1 101
char 1 116
char 1 0
align 1
LABELV $124
char 1 82
char 1 50
char 1 76
char 1 32
char 1 82
char 1 101
char 1 116
char 1 0
align 1
LABELV $123
char 1 66
char 1 82
char 1 50
char 1 84
char 1 76
char 1 32
char 1 82
char 1 101
char 1 116
char 1 0
align 1
LABELV $122
char 1 66
char 1 76
char 1 50
char 1 84
char 1 82
char 1 32
char 1 82
char 1 101
char 1 116
char 1 0
align 1
LABELV $121
char 1 76
char 1 50
char 1 82
char 1 32
char 1 82
char 1 101
char 1 116
char 1 0
align 1
LABELV $120
char 1 84
char 1 76
char 1 50
char 1 66
char 1 82
char 1 32
char 1 82
char 1 101
char 1 116
char 1 0
align 1
LABELV $119
char 1 84
char 1 50
char 1 66
char 1 32
char 1 83
char 1 116
char 1 0
align 1
LABELV $118
char 1 84
char 1 82
char 1 50
char 1 66
char 1 76
char 1 32
char 1 83
char 1 116
char 1 0
align 1
LABELV $117
char 1 82
char 1 50
char 1 76
char 1 32
char 1 83
char 1 116
char 1 0
align 1
LABELV $116
char 1 66
char 1 82
char 1 50
char 1 84
char 1 76
char 1 32
char 1 83
char 1 116
char 1 0
align 1
LABELV $115
char 1 66
char 1 76
char 1 50
char 1 84
char 1 82
char 1 32
char 1 83
char 1 116
char 1 0
align 1
LABELV $114
char 1 76
char 1 50
char 1 82
char 1 32
char 1 83
char 1 116
char 1 0
align 1
LABELV $113
char 1 84
char 1 76
char 1 50
char 1 66
char 1 82
char 1 32
char 1 83
char 1 116
char 1 0
align 1
LABELV $112
char 1 70
char 1 108
char 1 105
char 1 112
char 1 32
char 1 83
char 1 108
char 1 97
char 1 115
char 1 104
char 1 0
align 1
LABELV $111
char 1 70
char 1 108
char 1 105
char 1 112
char 1 32
char 1 83
char 1 116
char 1 97
char 1 98
char 1 0
align 1
LABELV $110
char 1 74
char 1 117
char 1 109
char 1 112
char 1 32
char 1 65
char 1 116
char 1 116
char 1 0
align 1
LABELV $109
char 1 76
char 1 117
char 1 110
char 1 103
char 1 101
char 1 32
char 1 65
char 1 116
char 1 116
char 1 0
align 1
LABELV $108
char 1 67
char 1 82
char 1 32
char 1 66
char 1 97
char 1 99
char 1 107
char 1 32
char 1 65
char 1 116
char 1 116
char 1 0
align 1
LABELV $107
char 1 66
char 1 97
char 1 99
char 1 107
char 1 32
char 1 65
char 1 116
char 1 116
char 1 0
align 1
LABELV $106
char 1 66
char 1 97
char 1 99
char 1 107
char 1 32
char 1 83
char 1 116
char 1 97
char 1 98
char 1 0
align 1
LABELV $105
char 1 84
char 1 50
char 1 66
char 1 32
char 1 65
char 1 116
char 1 116
char 1 0
align 1
LABELV $104
char 1 84
char 1 82
char 1 50
char 1 66
char 1 76
char 1 32
char 1 65
char 1 116
char 1 116
char 1 0
align 1
LABELV $103
char 1 82
char 1 50
char 1 76
char 1 32
char 1 65
char 1 116
char 1 116
char 1 0
align 1
LABELV $102
char 1 66
char 1 82
char 1 50
char 1 84
char 1 76
char 1 32
char 1 65
char 1 116
char 1 116
char 1 0
align 1
LABELV $101
char 1 66
char 1 76
char 1 50
char 1 84
char 1 82
char 1 32
char 1 65
char 1 116
char 1 116
char 1 0
align 1
LABELV $100
char 1 76
char 1 50
char 1 82
char 1 32
char 1 65
char 1 116
char 1 116
char 1 0
align 1
LABELV $99
char 1 84
char 1 76
char 1 50
char 1 66
char 1 82
char 1 32
char 1 65
char 1 116
char 1 116
char 1 0
align 1
LABELV $98
char 1 80
char 1 117
char 1 116
char 1 97
char 1 119
char 1 97
char 1 121
char 1 0
align 1
LABELV $97
char 1 68
char 1 114
char 1 97
char 1 119
char 1 0
align 1
LABELV $96
char 1 82
char 1 101
char 1 97
char 1 100
char 1 121
char 1 0
align 1
LABELV $95
char 1 78
char 1 111
char 1 110
char 1 101
char 1 0
