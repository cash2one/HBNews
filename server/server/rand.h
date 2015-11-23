#ifndef __FIGHT_H__
#define __FIGHT_H__

#pragma once;

#include <stdlib.h>
#include <time.h>  

#include "def.h"

class CRandom{

public:
	//todo ��������
	inline static CRandom* GetInstance(){
		static CRandom singleton;
		return &singleton;
	}

	UINT32 GetRandom(UINT32 min, UINT32 max){
		//��������д��if��һ��
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
		srand((unsigned int)(time(NULL)));//��ʼ��������ӣ�
	}

};

#endif