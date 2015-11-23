#include "webserver.h"
#include "Socket.h"

#include "game_config.h"
#include "db_mgr.h"
#include "async_command_module.h"
#include "server_for_gs.h"
#include "pay_handle.h"

//void Get(webserver::http_request* r) {
//  Socket s = *(r->s_);
//
//  std::string title;
//  std::string body;
//  std::string bgcolor="#ffffff";
//  std::string links =
//      "<p><a href='/red'>red</a> "
//      "<br><a href='/blue'>blue</a> "
//      "<br><a href='/form'>form</a> "
//      "<br><a href='/auth'>authentication example</a> [use <b>adp</b> as username and <b>gmbh</b> as password"
//      "<br><a href='/header'>show some HTTP header details</a> "
//      ;
//
//  if(r->path_ == "/") {
//    title = "Web Server Example";
//    body  = "<h1>Web Server Example</h1>"
//            "I wonder what you're going to click"  + links;
//  }
//  else if (r->path_ == "/red") {
//    bgcolor = "#ff4444";
//    title   = "You chose red";
//    body    = "<h1>Red</h1>" + links;
//  }
//  else if (r->path_ == "/blue") {
//    bgcolor = "#4444ff";
//    title   = "You chose blue";
//    body    = "<h1>Blue</h1>" + links;
//  }
//  else if (r->path_ == "/form") {
//    title   = "Fill a form";
//
//    body    = "<h1>Fill a form</h1>";
//    body   += "<form action='/form'>"
//              "<table>"
//              "<tr><td>Field 1</td><td><input name=field_1></td></tr>"
//              "<tr><td>Field 2</td><td><input name=field_2></td></tr>"
//              "<tr><td>Field 3</td><td><input name=field_3></td></tr>"
//              "</table>"
//              "<input type=submit></form>";
//
//
//    for (std::map<std::string, std::string>::const_iterator i = r->params_.begin();
//         i != r->params_.end();
//         i++) {
//
//      body += "<br>" + i->first + " = " + i->second;
//    }
//
//
//    body += "<hr>" + links;
//
//  }
//  else if (r->path_ == "/auth") {
//    if (r->authentication_given_) {
//      if (r->username_ == "adp" && r->password_ == "gmbh") {
//         body = "<h1>Successfully authenticated</h1>" + links;
//      }
//      else {
//         body = "<h1>Wrong username or password</h1>" + links;
//         r->auth_realm_ = "Private Stuff";
//      }
//    }
//    else {
//      r->auth_realm_ = "Private Stuff";
//    }
//  }
//  else if (r->path_ == "/header") {
//    title   = "some HTTP header details";
//    body    = std::string ("<table>")                                   +
//              "<tr><td>Accept:</td><td>"          + r->accept_          + "</td></tr>" +
//              "<tr><td>Accept-Encoding:</td><td>" + r->accept_encoding_ + "</td></tr>" +
//              "<tr><td>Accept-Language:</td><td>" + r->accept_language_ + "</td></tr>" +
//              "<tr><td>User-Agent:</td><td>"      + r->user_agent_      + "</td></tr>" +
//              "</table>"                                                +
//              links;
//  }
//  else {
//    r->status_ = "404 Not Found";
//    title      = "Wrong URL";
//    body       = "<h1>Wrong URL</h1>";
//    body      += "Path is : &gt;" + r->path_ + "&lt;"; 
//  }
//
//  r->answer_  = "<html><head><title>";
//  r->answer_ += title;
//  r->answer_ += "</title></head><body bgcolor='" + bgcolor + "'>";
//  r->answer_ += body;
//  r->answer_ += "</body></html>";
//}

void HttpHandle(webserver::http_request* r) {
	ASSERT(r);
	r->PrintDebugString();
	
	Socket s = *(r->s_);
	
	std::string body;

	if(r->path_ == "/") {
		body  = "hello wh O(∩_∩)O~";
	}
	else if (r->path_ == "/AdwoPaied") {
		body    = "AdwoPaied!";
		bool ret = CPayHandle::AdwoPaied(r);
		if (!ret){
			body = "failed";
		}
		ASSERT(ret);
	}
	else if (r->path_ == "/ReceiveRemoteNtf") {
		body    = "ReceiveRemoteNtf";
	}
	else {
		r->status_ = "404 Not Found";
		body       = "<h1>Wrong URL</h1>";
		body      += "Path is : &gt;" + r->path_ + "&lt;"; 
	}

	r->answer_ += body;
}

int main() {

	//初始化配置
	bool r = GameConfig::GetInstance()->Init();
	ASSERT(r);

	//启动db线程
	const DBConfig& db_config = GameConfig::GetInstance()->GetDBConfig();
	r = CDBMgr::GetInstance()->Init(db_config.m_host, db_config.m_port, db_config.m_user, db_config.m_passwd, db_config.m_db);
	ASSERT(r);

	r = CAsyncCommandModule::GetInstance()->Init(CAsyncCommandModule::COMMOND_SINGLE);
	ASSERT(r);
	//启动gs链接线程
	//const ClientForGSConfig& gs_config = GameConfig::GetInstance()->GetClientForGSConfig();
	//CClientForGS::GetInstance()->Init(gs_config.m_ip, gs_config.m_port);

	r = CServerForGS::GetInstance()->Init();
	ASSERT(r);

	r = CServerForGS::GetInstance()->Start();
	ASSERT(r);

	webserver(8085, HttpHandle);

	//todo 关闭所有的gs客户端链接
	CServerForGS::GetInstance()->Join();//等待gs线程结束
	CAsyncCommandModule::GetInstance()->Join();//等待数据库线程结束

	//struct st{
	//	unsigned a : 1;
	//	unsigned b : 2;
	//	unsigned c : 5;
	//};

	//st d = {0};
	//d.a = 5;
	//d.b = 100;
}
