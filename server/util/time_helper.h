#ifndef time_helper_h__
#define time_helper_h__
#include <time.h>
#include "def.h"

//�����Լ�Ԫ Epoch��1970-01-01 00:00:00 UTC���𾭹���ʱ�䣬����Ϊ��λ����� seconds ��Ϊ�գ��򷵻�ֵҲ�洢�ڱ��� seconds �С�
time_t Now();

//ʱ��� ��Ϊ��λ   t1 - t2
UINT32 diffTime(time_t t1, time_t t2);

//����ʱ���Ƿ���ͬһ�� offsetΪƫ��  ��ƫ��ʱ �Ͳ�����Ȼ����
bool isSameDay(time_t t1, time_t t2, int offset = 0);

//��ȡĳ��ʱ��(ʱ-��-��)����Epoch��1970-01-01 00:00:00 UTC��������
time_t GetTime(UINT32 iHour, UINT32 iMin, UINT32 iSec);

//���ݾ������0��0��0���iTotalSeconds���� �õ�ʱ���
time_t GetTime(UINT32 iTotalSeconds);

//��time_t��ʽת����tm�ṹ���ʽ
tm GetTM(time_t time);

tm GetNowTM(time_t t);
////��ʱ����� ��ȡ���뵱��0��0��0��� ����
UINT32 GetSecondsOfDay(time_t time);


////�Ƿ���ͬһ������ ����  ���ˢ�µ� �ж���һ�ε�ˢ��ʱ�� �� now �Ƿ���ͬһ��ʱ������
////last_value һ����ĳһ��ˢ�µ�
////���ε�
//bool IsInSameInterval(UINT32* int_arr, UINT32 arr_len, UINT32 last_value, UINT32 cur_value, UINT32 &new_stamp);


/************************************************************************/
/* 
	int_arr		һ���е�ʱ�������
	arr_valid_num		�Ѿ�ӵ�е�ʱ�������
	last_value	��һ�εļ�¼ʱ���
	cur_value	��ǰʱ��
*/
/************************************************************************/
bool IsInSameInterval(UINT32* int_arr, UINT32 arr_valid_num, UINT32 last_value, UINT32 cur_value);

//��ȡĳ��ĳ�µ�����
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