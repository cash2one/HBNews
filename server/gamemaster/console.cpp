#include <stdio.h>

#include "def.h"
#include "console.h"
#include "str.h"

//#include "gs_gm_proto.pb.h"
#include "client_for_gs.h"
bool CConsole::Start()
{
	int r = uv_thread_create(&m_thread, run, NULL);
	ASSERT( r == 0);
	return r == 0;
}

bool CConsole::Join()
{
	int r = uv_thread_join(&m_thread);
	ASSERT( r == 0);
	return r == 0;
}

void CConsole::run( void* arg )
{
	while (true){
		char str[1024] = {0};
		printf("����������:\n");
		//scanf("%s",str);
		scanf("%[^\n]", &str);
		getchar();

		//printf("�������������:%s\n",str);

		printf("ȷ��ִ�С�%s����?  ��[y]/��[n]\n", str);
		char check[1024] = {0};
		scanf("%[^\n]", &check);
		getchar();

		if (stricmp(check, "y") != 0){
			printf("ȡ��ִ�С�%s��\n", str);
			continue;
		}

		/*
		std::vector<std::string> v;
		split_string(str, v, " ");
		if (v.empty()){
			continue;
		}

		std::vector<std::string> v_args; 
		for (std::vector<std::string>::iterator it = v.begin() ; it != v.end() ; ++it){
			std::string& arg = *it;
			trim(arg);
			if (!arg.empty()){
				v_args.push_back(arg);
			}
		}

		std::string ret_info;
		bool r = ProcessCommond(v_args, ret_info);
		if (!r){
			printf("%s", ret_info.c_str());
		}
		*/

		std::string ret_info;
		bool r = ProcessCommond(std::string(str), ret_info);
		if (!r){
			printf("%s", ret_info.c_str());
		}
	}
}

bool checkArgsNum(std::vector<std::string> &args, UINT32 num){
	if (args.size() != num){
		printf("���벻����ȷ��ָ���ʽ ������������  need[%u] input[%u]�� �����Ƿ��ж���Ŀո�\n", num, args.size());
		return false;
	}

	return true;
}

bool checkArrArg(std::string &arg, bool check_empty = false){
	if (arg.empty() || arg.size() < 2 || arg.at(0) != '[' || arg.at(arg.size()-1) != ']'
		|| (check_empty && arg.size() <= 2) 
		){
		printf("���벻����ȷ�����������ʽ  arg: %s \n", arg.c_str());
		return false;
	}

	return true;
}

bool CConsole::ProcessCommond( std::vector<std::string> &args, std::string &ret_info)
{
	if (args.empty()){
		//printf("��������ȷ��ָ���ʽ��\n");
		ret_info = "��������ȷ��ָ���ʽ��\n";
		return false;
	}

	std::string& com_type = args[0];
	if ( com_type.at(0) != '@'){
		//printf("���벻����ȷ��ָ���ʽ ����@��ͷ��\n");
		ret_info = "���벻����ȷ��ָ���ʽ ����@��ͷ��\n";
		return false;
	}

	if (com_type == "@addelement"){
		if (!checkArgsNum(args, 4)){
			//printf("������������!\n");
			ret_info = "������������!\n";
			return false;
		}

		gm_gs_add_item_req* req = new gm_gs_add_item_req;
		req->set_user_id(atoi(args[1].c_str()));
		req->set_item_id(atoi(args[2].c_str()));
		req->set_num(atoi(args[3].c_str()));

		//���ܿ��̵߳��ã� �ᱨ����д����
		//CClientForGS::GetInstance()->Send(e_msg_gm_gs_add_item_req, &req);

		CClientForGS::GetInstance()->SendAsync(e_msg_gm_gs_add_item_req, req);
	}
	else if (com_type == "@marquee"){
		if (!checkArgsNum(args, 3)){
			//printf("������������!\n");
			ret_info = "������������!\n";
			return false;
		}

		gm_gs_marquee_req::MARQUEE_TYPE etype = (gm_gs_marquee_req::MARQUEE_TYPE)atoi(args[1].c_str());
		if (!gm_gs_marquee_req::MARQUEE_TYPE_IsValid(etype)){
			//printf("��������Ͳ���ȷ���������������\n");
			ret_info = "��������Ͳ���ȷ���������������\n";
			return false;
		}

		gm_gs_marquee_req* req = new gm_gs_marquee_req;
		req->set_type(etype);
		//req->set_times(atoi(args[2].c_str()));
		
		//req->set_content(args[2]);//��������utf8��
		char* pbuf = ANSIToUTF8(args[2].c_str());
		req->set_content(pbuf);
		free(pbuf);

		CClientForGS::GetInstance()->SendAsync(e_msg_gm_gs_marquee_req, req);
	}
	else if (com_type == "@mail"){
		if (!checkArgsNum(args, 5) || !checkArrArg(args[2]) || !checkArrArg(args[4])){
			ret_info = "������������!\n";
			return false;
		}

		gm_gs_mail_req::MAIL_TYPE etype = (gm_gs_mail_req::MAIL_TYPE)atoi(args[1].c_str());
		if (!gm_gs_mail_req::MAIL_TYPE_IsValid(etype)){
			//printf("�ʼ����Ͳ���ȷ���������������\n");
			ret_info = "�ʼ����Ͳ���ȷ���������������\n";
			return false;
		}

		gm_gs_mail_req* req = new gm_gs_mail_req;
		req->set_type(etype);

		std::string s = args[2].substr(1, args[2].size()-2);
		std::vector<int> users_id;
		split_string_2i(s, users_id, ",");
		for (std::vector<int>::iterator it = users_id.begin(); it != users_id.end() ; ++it){
			if (*it){
				req->add_users(*it);
			}
			else{
				//printf("�û�id��������ȷ���������������\n");
				ret_info = "�û�id��������ȷ���������������\n";
				return false;
			}
		}

		//req->set_content(args[2]);//��������utf8��
		char* pbuf = ANSIToUTF8(args[3].c_str());
		req->set_content(pbuf);
		free(pbuf);

		s = args[4].substr(1, args[4].size()-2);
		std::vector<std::string> eles;
		split_string(s, eles, ",");
		for (std::vector<std::string>::iterator it = eles.begin(); it != eles.end() ; ++it){

			std::string& ele_s = *it;
			std::vector<int> ele_info;
			split_string_2i(ele_s, ele_info, "-");
			ASSERT(ele_info.size() == 3);
			if (ele_info.size() != 3){
				//printf("������������ȷ���������������\n");
				ret_info = "������������ȷ���������������\n";
				return false;
			}

			game_element* ele =  req->add_elements();
			ele->set_type(ele_info[0]);
			ele->set_id(ele_info[1]);
			ele->set_num(ele_info[2]);
		}

		CClientForGS::GetInstance()->SendAsync(e_msg_gm_gs_mail_req, req);

	}
	else{
		//printf("�ݲ�֧��[%s]ָ�\n", com_type.c_str());
		ret_info = "�ݲ�֧�ִ�ָ�\n";
		return false;
	}

	ret_info = "ok";
	return true;
}

bool CConsole::ProcessCommond( std::string &str_commond, std::string &ret_info )
{
	std::vector<std::string> v;
	split_string(str_commond, v, " ");
	if (v.empty()){
		//continue;
		ret_info = "�����ʽ����ȷ��\n";
		return false;
	}

	std::vector<std::string> v_args; 
	for (std::vector<std::string>::iterator it = v.begin() ; it != v.end() ; ++it){
		std::string& arg = *it;
		trim(arg);
		if (!arg.empty()){
			v_args.push_back(arg);
		}
	}

	return ProcessCommond(v_args, ret_info);
}