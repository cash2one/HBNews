#include "user.h"

UINT32 CUserTeam::UpdateTeam( UINT8 iTeamIdx, UINT32 (&heros)[MAXNUM_HEROINTEAM] )
{
	if(iTeamIdx >= MAXNUM_TEAM){
		return 1;
	}

	memcpy(m_Teams[iTeamIdx], heros, MAXNUM_HEROINTEAM);
	return 0;
}