/*****************************************************************************
*   for : ��������
data : 20150312
author : baomin
*
******************************************************************************/
#ifndef __CONFIG_H__
#define __CONFIG_H__

#pragma once;

#define MAXLEN_MAPPOINTNAME 255		//�ݵ���������ֽ���
#define MAXNUM_JOINEDMAPPOINT 10	//������Ӿݵ���

#define MAXNUM_HEROSKILL 10 //Ӣ�����ʮ������

#define MAXNUM_HEROINTEAM	5	//һ����������Ӣ����
#define MAXNUM_TEAM		1		//һ������������趨�Ķ����� �����ø�������չ������

#define MAXNUM_TASKENTERCONDITION	5 //һ���������Ľ�����������

#define MAXPROBITY_BURST	10000	//���ı���

enum EErrorCode{
	EErrorCode_Success = 0,
	EErrorCode_UnKown_Error = 1,
	EErrorCode_TASKCONFIG_ERROR = 2//������������
	
};

enum EArea{
	EArea_Huaxia	= 1,	//����
	EArea_Dahe		= 2,	//���
	EArea_Europe	= 3,	//ŷ����
	EArea_MAX		= 4
};

enum ENatureProperty{
	ENatureProperty_Huo		=	1,	//��
	ENatureProperty_Feng	=	2,	//��
	ENatureProperty_Shui	=	3,	//ˮ
	ENatureProperty_Guang	=	4,	//��
	ENatureProperty_An		=	5,	//��
	ENatureProperty_MAX		=	6
};

//�ܶ�Ӣ�����Է���������Ҫ
enum EProperty{
	EProperty_Health = 1, //����
	EProperty_Damage = 2, //�˺�
	//EProperty_AttackSpeed = 3, //����
	EProperty_MAX
};

enum EOccupation{
	EOccupation_Warrior		=	1,	//��ս սʿ
	EOccupation_Archer		=	2,	//���� ��� ��Զ������
	EOccupation_Mage		=	3,	//��ʦ
	EOccupation_Auxiliary	=	4,	//����
	EOccupation_MAX			=	5
};


enum ETASKTYPE{
	ETASKTYPE_MAIN = 1, //���߾���
	ETASKTYPE_BRANCH = 2, //֧��
	ETASKTYPE_ACTIVITY = 3//�
};

enum EUSERSUBTASKSTATUS{
	EUSERSUBTASKSTATUS_NOT_DONE = 0 ,//δͨ��
	EUSERSUBTASKSTATUS_DONE = 1//1��ͨ��
};

enum EUSERTASKSTATUS{
	EUSERTASKSTATUS_NOT_DONE = 0, 
	EUSERTASKSTATUS_DONE = 1
};

#endif