#include "time_helper.h"

//返回自纪元 Epoch（1970-01-01 00:00:00 UTC）起经过的时间，以秒为单位。如果 seconds 不为空，则返回值也存储在变量 seconds 中。
time_t Now(){
	time_t rawtime;
	time(&rawtime);  //结果是时间戳 //得到的utc 本时区的  对的！
	return rawtime;
}

//时间差 秒为单位   t1 - t2
UINT32 diffTime(time_t t1, time_t t2){
	ASSERT(t1 >= t2);
	return (UINT32)difftime(t1, t2);
}

//两个时间是否是同一天
bool isSameDay(time_t t1, time_t t2, int offset/* = 0*/){
	//因为gmtime结果指针共用的！所以这里用对象
	t1 = t1 - offset;
	t2 = t2 - offset;
	//tm tm1 = *(gmtime(&t1));//得到的utc 0时区的  比本地早八个小时！
	//tm tm2 = *(gmtime(&t2));//得到的utc 0时区的  比本地早八个小时！

	tm tm1, tm2;
	localtime_s(&tm1, &t1);//得到的utc 本时区的
	localtime_s(&tm2, &t2);//得到的utc 本时区的

	return tm1.tm_year == tm2.tm_year && tm1.tm_yday == tm2.tm_yday;
}

time_t GetTime(UINT32 iTotalSeconds)
{
	UINT32 iHour = iTotalSeconds/3600;
	UINT32 iMin = (iTotalSeconds - 3600 * iHour)/60;
	UINT32 iSec = iTotalSeconds - iHour*3600 - iMin*60;

	return GetTime(iHour, iMin, iSec);
}

time_t GetTime(UINT32 iHour, UINT32 iMin, UINT32 iSec)
{
	ASSERT(iHour >= 0 && iHour <= 23);
	ASSERT(iMin >= 0 && iMin <= 59);
	ASSERT(iSec >= 0 && iSec <= 59);

	time_t tNow;
	time(&tNow); 

	struct tm t_now; 
	localtime_s(&t_now, &tNow);
	t_now.tm_hour = iHour;
	t_now.tm_min = iMin;
	t_now.tm_sec = iSec;
	time_t tGet = mktime(&t_now);

	return tGet;
}

time_t GetTimeYestoday(UINT32 iTotalSeconds){
	time_t today = GetTime(iTotalSeconds);
	return today - 86400;//前一天的这个时间点
}

//获取秒的时间戳和余下的毫秒
//void GetTime(UINT32 &timestamp, UINT32 &milliseconds){
//	SYSTEMTIME systime;
//	GetLocalTime(&systime);
//
//	struct tm t_now;
//	t_now.tm_year = systime.wYear;
//	t_now.tm_mon = systime.wMonth;
//	t_now.tm_mday = systime.wDay;
//	t_now.tm_hour = systime.wHour;
//	t_now.tm_min = systime.wMinute;
//	t_now.tm_sec = systime.wSecond;
//
//	time_t tGet = mktime(&t_now);
//
//	return tGet;
//}

tm GetTM(time_t time)
{
	struct tm t;
	localtime_s(&t, &time);
	return t;
}

//从时间戳中 获取距离当天0点0分0秒的 秒数
UINT32 GetSecondsOfDay(time_t time){
	struct tm t;
	localtime_s(&t, &time);
	return t.tm_hour*3600 + t.tm_min*60 + t.tm_sec;
}

////是否在同一个区间 比如  多个刷新点 判断上一次的刷新时间 和 now 是否在同一个时间区间
////last_value 一定是某一个刷新点
////环形的
////如果不在同一个区间 则new_stamp为新的刷新点
//bool IsInSameInterval( UINT32* int_arr, UINT32 arr_len, UINT32 last_value, UINT32 cur_value, UINT32 &new_stamp )
//{
//	//不严谨 如果只有一个或者两个元素呢？  还要考虑是否同一天！
//	printf("IsInSameInterval 不严谨 如果只有一个或者两个元素呢？  还要考虑是否同一天！ \n");
//	return false;
//
//	ASSERT(int_arr && arr_len);
//	//首先找到上一个的区间， 然后判断now是否在这个区间里
//	UINT32 next_value = 0;
//	for(UINT32 i = 0 ; i < arr_len ; ++i){
//		if (int_arr[i] > last_value){
//			next_value = int_arr[i];
//			break;
//		}
//	}
//
//	bool isIn = false;
//	new_stamp = 0;
//	if (next_value){
//		isIn = cur_value > last_value && cur_value <= next_value;//cur_value 不能等于 last_value 因为 会导致重复刷新
//		new_stamp = next_value;
//	}
//	else{//环形的 上一次已经是最后的一次的 此时就要判断 cur_value是否比第一个元素大
//		isIn = cur_value >= int_arr[0];
//		new_stamp = int_arr[0];
//	}
//
//}

bool IsInSameInterval( UINT32* int_arr, UINT32 arr_valid_num, UINT32 last_value, UINT32 cur_value )
{
	ASSERT(cur_value >= last_value);
	//ASSERT(int_arr && arr_valid_num);//即使0个元素也没关系 那就是永远不用刷新   假如有一个元素 并且元素值是0 则表示在凌晨刷新

	//判断首尾
	if(arr_valid_num > 0 ){
		UINT32 timestamp_yestoday = GetTimeYestoday(int_arr[0]);
		if (last_value < timestamp_yestoday){//不写等于 因为等于的时候 也就是那次是刷新的
			return false;//不在一个区间 需要刷新
		}

		UINT32 timestamp_latest = GetTime(int_arr[arr_valid_num-1]);
		if (last_value >= timestamp_latest){
			return true;//在一个区间 不需要刷新
		}
	}

	//首先找到上一个的区间， 然后判断now是否在这个区间里
	UINT32 next_value = 0;
	for(UINT32 i = 0 ; i < arr_valid_num ; ++i){
		UINT32 timestamp = GetTime(int_arr[i]);
		
		//如果不在一个区间
		if ( (last_value < timestamp && cur_value >= timestamp)
			//|| (last_value < timestamp && cur_value >= timestamp
		){
			return false;//不在一个区间
		}
	}

	return true;//没有刷新时间数据 或者 不存在一个时间点分开两个值 就认为是在一个区间！
}

int GetDaysInMonth( int year, int month )
{
	int d;
	int day[]= {31,28,31,30,31,30,31,31,30,31,30,31};
	if (2==month)
	{
		d=(((0==year%4)&&(0!=year%100)||(0==year%400))?29:28);
	}
	else
	{
		d=day[month-1];

	}
	return d;
}

tm GetNowTM(time_t t)
{
	tm now = GetTM(t);
	now.tm_year = now.tm_year + 1900;
	now.tm_mon = now.tm_mon + 1;
	return now;
}
