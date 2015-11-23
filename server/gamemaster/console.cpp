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
		printf("请输入命令:\n");
		//scanf("%s",str);
		scanf("%[^\n]", &str);
		getchar();

		//printf("您输入的命令是:%s\n",str);

		printf("确认执行【%s】吗?  是[y]/否[n]\n", str);
		char check[1024] = {0};
		scanf("%[^\n]", &check);
		getchar();

		if (stricmp(check, "y") != 0){
			printf("取消执行【%s】\n", str);
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
		printf("输入不是正确的指令格式 参数个数不对  need[%u] input[%u]！ 请检查是否有多余的空格？\n", num, args.size());
		return false;
	}

	return true;
}

bool checkArrArg(std::string &arg, bool check_empty = false){
	if (arg.empty() || arg.size() < 2 || arg.at(0) != '[' || arg.at(arg.size()-1) != ']'
		|| (check_empty && arg.size() <= 2) 
		){
		printf("输入不是正确的数组参数格式  arg: %s \n", arg.c_str());
		return false;
	}

	return true;
}

bool CConsole::ProcessCommond( std::vector<std::string> &args, std::string &ret_info)
{
	if (args.empty()){
		//printf("请输入正确的指令格式！\n");
		ret_info = "请输入正确的指令格式！\n";
		return false;
	}

	std::string& com_type = args[0];
	if ( com_type.at(0) != '@'){
		//printf("输入不是正确的指令格式 请以@开头！\n");
		ret_info = "输入不是正确的指令格式 请以@开头！\n";
		return false;
	}

	if (com_type == "@addelement"){
		if (!checkArgsNum(args, 4)){
			//printf("参数个数不对!\n");
			ret_info = "参数个数不对!\n";
			return false;
		}

		gm_gs_add_item_req* req = new gm_gs_add_item_req;
		req->set_user_id(atoi(args[1].c_str()));
		req->set_item_id(atoi(args[2].c_str()));
		req->set_num(atoi(args[3].c_str()));

		//不能跨线程调用！ 会报不可写错误！
		//CClientForGS::GetInstance()->Send(e_msg_gm_gs_add_item_req, &req);

		CClientForGS::GetInstance()->SendAsync(e_msg_gm_gs_add_item_req, req);
	}
	else if (com_type == "@marquee"){
		if (!checkArgsNum(args, 3)){
			//printf("参数个数不对!\n");
			ret_info = "参数个数不对!\n";
			return false;
		}

		gm_gs_marquee_req::MARQUEE_TYPE etype = (gm_gs_marquee_req::MARQUEE_TYPE)atoi(args[1].c_str());
		if (!gm_gs_marquee_req::MARQUEE_TYPE_IsValid(etype)){
			//printf("跑马灯类型不正确！请重新输入命令：\n");
			ret_info = "跑马灯类型不正确！请重新输入命令：\n";
			return false;
		}

		gm_gs_marquee_req* req = new gm_gs_marquee_req;
		req->set_type(etype);
		//req->set_times(atoi(args[2].c_str()));
		
		//req->set_content(args[2]);//这样不是utf8！
		char* pbuf = ANSIToUTF8(args[2].c_str());
		req->set_content(pbuf);
		free(pbuf);

		CClientForGS::GetInstance()->SendAsync(e_msg_gm_gs_marquee_req, req);
	}
	else if (com_type == "@mail"){
		if (!checkArgsNum(args, 5) || !checkArrArg(args[2]) || !checkArrArg(args[4])){
			ret_info = "参数个数不对!\n";
			return false;
		}

		gm_gs_mail_req::MAIL_TYPE etype = (gm_gs_mail_req::MAIL_TYPE)atoi(args[1].c_str());
		if (!gm_gs_mail_req::MAIL_TYPE_IsValid(etype)){
			//printf("邮件类型不正确！请重新输入命令：\n");
			ret_info = "邮件类型不正确！请重新输入命令：\n";
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
				//printf("用户id参数不正确！请重新输入命令：\n");
				ret_info = "用户id参数不正确！请重新输入命令：\n";
				return false;
			}
		}

		//req->set_content(args[2]);//这样不是utf8！
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
				//printf("附件参数不正确！请重新输入命令：\n");
				ret_info = "附件参数不正确！请重新输入命令：\n";
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
		//printf("暂不支持[%s]指令！\n", com_type.c_str());
		ret_info = "暂不支持此指令！\n";
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
		ret_info = "命令格式不正确！\n";
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