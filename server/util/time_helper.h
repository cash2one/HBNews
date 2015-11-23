#ifndef time_helper_h__
#define time_helper_h__
#include <time.h>
#include "def.h"

//返回自纪元 Epoch（1970-01-01 00:00:00 UTC）起经过的时间，以秒为单位。如果 seconds 不为空，则返回值也存储在变量 seconds 中。
time_t Now();

//时间差 秒为单位   t1 - t2
UINT32 diffTime(time_t t1, time_t t2);

//两个时间是否是同一天 offset为偏移  有偏移时 就不是自然日了
bool isSameDay(time_t t1, time_t t2, int offset = 0);

//获取某个时间(时-分-秒)距离Epoch（1970-01-01 00:00:00 UTC）的秒数
time_t GetTime(UINT32 iHour, UINT32 iMin, UINT32 iSec);

//根据距离今天0点0分0秒的iTotalSeconds秒数 得到时间戳
time_t GetTime(UINT32 iTotalSeconds);

//将time_t格式转换成tm结构体格式
tm GetTM(time_t time);

tm GetNowTM(time_t t);
////从时间戳中 获取距离当天0点0分0秒的 秒数
UINT32 GetSecondsOfDay(time_t time);


////是否在同一个区间 比如  多个刷新点 判断上一次的刷新时间 和 now 是否在同一个时间区间
////last_value 一定是某一个刷新点
////环形的
//bool IsInSameInterval(UINT32* int_arr, UINT32 arr_len, UINT32 last_value, UINT32 cur_value, UINT32 &new_stamp);


/************************************************************************/
/* 
	int_arr		一日中的时间点数组
	arr_valid_num		已经拥有的时间点数量
	last_value	上一次的记录时间点
	cur_value	当前时间
*/
/************************************************************************/
bool IsInSameInterval(UINT32* int_arr, UINT32 arr_valid_num, UINT32 last_value, UINT32 cur_value);

//获取某年某月的天数
int GetDaysInMonth(int year, int month );


class CTimeChecker{
public:
	CTimeChecker(){
		m_start = clock();
	}

	void PrintfDuration(){
		clock_t now = clock();
		printf( "[duration]:%lu ms\n", (now - m_start));
	}
	
private:
	clock_t m_start ;
};

#endif // time_helper_h__