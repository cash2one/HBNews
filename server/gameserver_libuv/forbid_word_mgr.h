#pragma once
#ifndef forbid_word_mgr_h__
#define forbid_word_mgr_h__
#include <string>
#include <vector>
class CForbidWordMgr
{
public:
	static CForbidWordMgr* GetInstance(){
		static CForbidWordMgr singleton;
		return &singleton;
	}

	bool Init();

	bool IsForbidWord(const std::string& word);
private:
	CForbidWordMgr(void){};
	std::vector<std::string> m_vecForbidWord;
};

#endif