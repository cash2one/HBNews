#ifndef rand_h__
#define rand_h__

#pragma once;

#include <stdlib.h>
#include <time.h>
#include <set>

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
			//return min + rand()%(max- min + 1);
			return min + (UINT32)(((double(rand()))/(RAND_MAX))*(max- min));
		}
		else if (max == min)
		{
			return min;
		}
		else{
			//return max + rand()%(min-max);
			return GetRandom(max, min);
		}
	}

	//从[min, max]中随机选取num个不重复的 如果不够就有多少返回多少 exlude为需要排除的
	void GetRandom(UINT32 min, UINT32 max, UINT32 exlude, UINT32 num, std::set<UINT32> &randoms){
		//最常见的情况写在if第一个
		if (max > min)
		{
			UINT32 pool_len = max - min + 1;
			UINT32* samples = new UINT32[pool_len];
			//memset(samples, 0, (max-min+1)* sizeof UINT32);
			for (UINT32 i = 0; i < pool_len ; ++i){
				samples[i] = min + i;
			}

			if (exlude >= min && exlude <= max){
				std::swap(samples[exlude-min], samples[pool_len-1]);//交换到尾部
				--pool_len;
			}

			for (UINT32 i = 0 ; i < num  && pool_len > 0; ++i){
				UINT32 chosenIdx = GetRandom(0, pool_len-1);
				randoms.insert(samples[chosenIdx]);
				std::swap(samples[chosenIdx], samples[pool_len-1]);//交换到尾部
				--pool_len;
			}
		}
		else if (max == min)
		{
			if (min != exlude){
				randoms.insert(min);
			}
		}
		else{
			//return max + rand()%(min-max);
			return GetRandom(max, min, exlude, num, randoms);
		}
	}

	//void test(){

	//	for (int i = 0 ; i = 1000 ; ++i)
	//	{
	//		UINT32 race = CRandom::GetInstance()->GetRandom(0, MAXPROBITY_BURST);
	//		//printf("%u\n", race);
	//		if (race == max)
	//		{
	//			int kk = 0;
	//		}
	//	}

	//}

private:
	CRandom(){
		srand((unsigned int)(time(NULL)));//初始化随机种子！
	}

};

#endif // rand_h__