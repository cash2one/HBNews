#ifndef __FIGHT_H__
#define __FIGHT_H__

#pragma once;

#include <stdlib.h>
#include <time.h>  

#include "def.h"

class CRandom{

public:
	//todo 单例工具
	inline static CRandom* GetInstance(){
		static CRandom singleton;
		return &singleton;
	}

	UINT32 GetRandom(UINT32 min, UINT32 max){
		//最常见的情况写在if第一个
		if (max > min)
		{
			return min + rand()%(max- min + 1);	
		}
		else if (max == min)
		{
			return min;
		}
		else{
			return max + rand()%(min-max);	
		}
	}

private:
	CRandom(){
		srand((unsigned int)(time(NULL)));//初始化随机种子！
	}

};

#endif