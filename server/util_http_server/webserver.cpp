/*
   WebServer.cpp

   Copyright (C) 2003-2004 Ren?Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   Ren?Nyffenegger rene.nyffenegger@adp-gmbh.ch

   Thanks to Tom Lynn who pointed out an error in this source code.
*/

#include <ctime>
#include <process.h>
#include <iostream>
#include <string>
#include <map>
#include <sstream>
//<process.h>


#include "webserver.h"
#include "socket.h"
#include "UrlHelper.h"
#include "base64.h"

webserver::request_func webserver::request_func_=0;

unsigned webserver::Request(void* ptr_s) {

	static const std::string authorization   = "Authorization: Basic ";
	static const std::string accept          = "Accept: "             ;
	static const std::string accept_language = "Accept-Language: "    ;
	static const std::string accept_encoding = "Accept-Encoding: "    ;
	static const std::string user_agent      = "User-Agent: "         ;

  Socket s = *(reinterpret_cast<Socket*>(ptr_s));
  
  std::string line = s.ReceiveLine();
  if (line.empty()) {
    return 1;
  }

  http_request req;
  std::string path;
  std::map<std::string, std::string> params;

  if (line.find("GET") == 0) {
    req.method_="GET";
	size_t posStartPath = line.find_first_not_of(" ",3);
	SplitGetReq(line.substr(posStartPath), path, params);

	while(1) {
		line=s.ReceiveLine();

		if (line.empty()) break;

		unsigned int pos_cr_lf = line.find_first_of("\x0a\x0d");
		if (pos_cr_lf == 0) break;

		line = line.substr(0,pos_cr_lf);

		if (line.substr(0, authorization.size()) == authorization) {
			req.authentication_given_ = true;
			std::string encoded = line.substr(authorization.size());
			std::string decoded = base64_decode(encoded);

			unsigned int pos_colon = decoded.find(":");

			req.username_ = decoded.substr(0, pos_colon);
			req.password_ = decoded.substr(pos_colon+1 );
		}
		else if (line.substr(0, accept.size()) == accept) {
			req.accept_ = line.substr(accept.size());
		}
		else if (line.substr(0, accept_language.size()) == accept_language) {
			req.accept_language_ = line.substr(accept_language.size());
		}
		else if (line.substr(0, accept_encoding.size()) == accept_encoding) {
			req.accept_encoding_ = line.substr(accept_encoding.size());
		}
		else if (line.substr(0, user_agent.size()) == user_agent) {
			req.user_agent_ = line.substr(user_agent.size());
		}
	}

  }
  else if (line.find("POST") == 0) {
    req.method_="POST";

	size_t space_1 = line.find(" ");
	size_t space_2 = line.find(" ", space_1 + 1);
	if (space_1 == std::string::npos || space_2 == std::string::npos){
		printf("！！！POST解析path出错！！！\n");
	}
	path = line.substr(space_1+1, space_2-space_1-1);

	//看看究竟是什么数据
	std::string bytes = s.ReceiveBytes();

	printf("原始数据:【%s】", bytes.c_str());

	char* field_start_flag = "Content-Disposition: form-data; name=\"";
	char* field_end_flag = "\"";
	unsigned int field_start_flag_len = strlen(field_start_flag);
	int field_start_pos;//名字的起点
	int field_end_pos;//名字的重点
	int last_field_start_pos = 0;
	char* line_flag = "\x0d\x0a\x0d\x0a";
	unsigned int line_flag_len = strlen(line_flag);
	do {
		field_start_pos = bytes.find(field_start_flag, last_field_start_pos);
		if (field_start_pos != std::string::npos){

			field_start_pos = field_start_pos + field_start_flag_len;
			last_field_start_pos = field_start_pos;

			field_end_pos = bytes.find(field_end_flag, last_field_start_pos);

			std::string field_name = bytes.substr(field_start_pos, field_end_pos-field_start_pos);
			//printf("filed_name:%s\n", field_name.c_str());

			int field_value_start_pos = bytes.find(line_flag, field_end_pos);
			if (field_value_start_pos == std::string::npos){
				printf("！！！from表单解析出错！！！\n");
				//return -1;
			}
			field_value_start_pos += line_flag_len;

			int field_value_end_pos = bytes.find("\x0d\x0a", field_value_start_pos);
			if (field_value_end_pos == std::string::npos){
				printf("！！！from表单解析出错！！！\n");
				//return -1;
			}

			std::string value = bytes.substr(field_value_start_pos, field_value_end_pos-field_value_start_pos);
			std::pair<std::map<std::string, std::string>::iterator, bool>  ret = params.insert(std::make_pair<std::string, std::string>(field_name, value));
			if (ret.second == false){
				printf("！！！from表单数据有问题 有重复字段！！！\n");
				//return -1;
			}
		}
	} while (field_start_pos != std::string::npos);
	
  }

  req.status_ = "202 OK";
  req.s_      = &s;
  req.path_   = path;
  req.params_ = params;

  request_func_(&req);

  std::stringstream str_str;
  str_str << req.answer_.size();

  time_t ltime;
  time(&ltime);
  tm* gmt= gmtime(&ltime);

  static std::string const serverName = "RenesWebserver (Windows)";

  char* asctime_remove_nl = asctime(gmt);
  asctime_remove_nl[24] = 0;

  s.SendBytes("HTTP/1.1 ");

  if (! req.auth_realm_.empty() ) {
    s.SendLine("401 Unauthorized");
    s.SendBytes("WWW-Authenticate: Basic Realm=\"");
    s.SendBytes(req.auth_realm_);
    s.SendLine("\"");
  }
  else {
    s.SendLine(req.status_);
  }
  s.SendLine(std::string("Date: ") + asctime_remove_nl + " GMT");
  s.SendLine(std::string("Server: ") +serverName);
  s.SendLine("Connection: close");
  s.SendLine("Content-Type: text/html; charset=ISO-8859-1");
  s.SendLine("Content-Length: " + str_str.str());
  s.SendLine("");
  s.SendLine(req.answer_);

  s.Close();
  

  return 0;
}

webserver::webserver(unsigned int port_to_listen, request_func r) {
  SocketServer in(port_to_listen,5);

  request_func_ = r;

  while (1) {
    Socket* ptr_s=in.Accept();

    unsigned ret;
    _beginthreadex(0,0,Request,(void*) ptr_s,0,&ret);
  }
}
