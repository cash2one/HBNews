#ifndef __USER_MGR_H__
#define __USER_MGR_H__

#pragma once

#include <map>
using namespace std;

#include "user.h"

class CUserMgr{
public:
	//todo µ¥Àý¹¤¾ß
	inline static CUserMgr* GetInstance(){
		static CUserMgr singleton;
		return &singleton;
	}

	bool Init(){

	}

	bool UnInit();

	CUser* GetUserByUserID(UINT32 iUserID){
		map<UINT32, CUser*>::iterator it = m_mapUserID2User.find(iUserID);
		if (it != m_mapUserID2User.end())
		{
			return it->second;
		}
	
		return NULL;
	}

private:

	map<UINT32, CUser*> m_mapUserID2User;
};

#endif