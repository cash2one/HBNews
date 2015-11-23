#include "time_helper.h"

//�����Լ�Ԫ Epoch��1970-01-01 00:00:00 UTC���𾭹���ʱ�䣬����Ϊ��λ����� seconds ��Ϊ�գ��򷵻�ֵҲ�洢�ڱ��� seconds �С�
time_t Now(){
	time_t rawtime;
	time(&rawtime);  //�����ʱ��� //�õ���utc ��ʱ����  �Եģ�
	return rawtime;
}

//ʱ��� ��Ϊ��λ   t1 - t2
UINT32 diffTime(time_t t1, time_t t2){
	ASSERT(t1 >= t2);
	return (UINT32)difftime(t1, t2);
}

//����ʱ���Ƿ���ͬһ��
bool isSameDay(time_t t1, time_t t2, int offset/* = 0*/){
	//��Ϊgmtime���ָ�빲�õģ����������ö���
	t1 = t1 - offset;
	t2 = t2 - offset;
	//tm tm1 = *(gmtime(&t1));//�õ���utc 0ʱ����  �ȱ�����˸�Сʱ��
	//tm tm2 = *(gmtime(&t2));//�õ���utc 0ʱ����  �ȱ�����˸�Сʱ��

	tm tm1, tm2;
	localtime_s(&tm1, &t1);//�õ���utc ��ʱ����
	localtime_s(&tm2, &t2);//�õ���utc ��ʱ����

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
	return today - 86400;//ǰһ������ʱ���
}

//��ȡ���ʱ��������µĺ���
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

//��ʱ����� ��ȡ���뵱��0��0��0��� ����
UINT32 GetSecondsOfDay(time_t time){
	struct tm t;
	localtime_s(&t, &time);
	return t.tm_hour*3600 + t.tm_min*60 + t.tm_sec;
}

////�Ƿ���ͬһ������ ����  ���ˢ�µ� �ж���һ�ε�ˢ��ʱ�� �� now �Ƿ���ͬһ��ʱ������
////last_value һ����ĳһ��ˢ�µ�
////���ε�
////�������ͬһ������ ��new_stampΪ�µ�ˢ�µ�
//bool IsInSameInterval( UINT32* int_arr, UINT32 arr_len, UINT32 last_value, UINT32 cur_value, UINT32 &new_stamp )
//{
//	//���Ͻ� ���ֻ��һ����������Ԫ���أ�  ��Ҫ�����Ƿ�ͬһ�죡
//	printf("IsInSameInterval ���Ͻ� ���ֻ��һ����������Ԫ���أ�  ��Ҫ�����Ƿ�ͬһ�죡 \n");
//	return false;
//
//	ASSERT(int_arr && arr_len);
//	//�����ҵ���һ�������䣬 Ȼ���ж�now�Ƿ������������
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
//		isIn = cur_value > last_value && cur_value <= next_value;//cur_value ���ܵ��� last_value ��Ϊ �ᵼ���ظ�ˢ��
//		new_stamp = next_value;
//	}
//	else{//���ε� ��һ���Ѿ�������һ�ε� ��ʱ��Ҫ�ж� cur_value�Ƿ�ȵ�һ��Ԫ�ش�
//		isIn = cur_value >= int_arr[0];
//		new_stamp = int_arr[0];
//	}
//
//}

bool IsInSameInterval( UINT32* int_arr, UINT32 arr_valid_num, UINT32 last_value, UINT32 cur_value )
{
	ASSERT(cur_value >= last_value);
	//ASSERT(int_arr && arr_valid_num);//��ʹ0��Ԫ��Ҳû��ϵ �Ǿ�����Զ����ˢ��   ������һ��Ԫ�� ����Ԫ��ֵ��0 ���ʾ���賿ˢ��

	//�ж���β
	if(arr_valid_num > 0 ){
		UINT32 timestamp_yestoday = GetTimeYestoday(int_arr[0]);
		if (last_value < timestamp_yestoday){//��д���� ��Ϊ���ڵ�ʱ�� Ҳ�����Ǵ���ˢ�µ�
			return false;//����һ������ ��Ҫˢ��
		}

		UINT32 timestamp_latest = GetTime(int_arr[arr_valid_num-1]);
		if (last_value >= timestamp_latest){
			return true;//��һ������ ����Ҫˢ��
		}
	}

	//�����ҵ���һ�������䣬 Ȼ���ж�now�Ƿ������������
	UINT32 next_value = 0;
	for(UINT32 i = 0 ; i < arr_valid_num ; ++i){
		UINT32 timestamp = GetTime(int_arr[i]);
		
		//�������һ������
		if ( (last_value < timestamp && cur_value >= timestamp)
			//|| (last_value < timestamp && cur_value >= timestamp
		){
			return false;//����һ������
		}
	}

	return true;//û��ˢ��ʱ������ ���� ������һ��ʱ���ֿ�����ֵ ����Ϊ����һ�����䣡
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
