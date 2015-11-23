#include "client_for_gs.h"

#include "console.h"

#include "webserver.h"
#include "Socket.h"


void HttpHandle(webserver::http_request* r) {
	ASSERT(r);
	r->PrintDebugString();

	Socket s = *(r->s_);

	std::string body;

	if(r->path_ == "/") {
		body  = "hello wh O(��_��)O~";
	}
	else if (r->path_ == "/gmcommond") {
		//body    = "AdwoPaied!";
		//bool ret = CPayHandle::AdwoPaied(r);
		//if (!ret){
		//	body = "failed";
		//}
		//ASSERT(ret);

		CConsole::ProcessCommond(r->params_["commond"], body);
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


int main(){

	////�����û���ʱ��
	//CClientForAS::GetInstance()->Init();
	//CClientForAS::GetInstance()->Start();
	//CClientForAS::GetInstance()->Join();
	////gs��������as��½�ɹ�֮��

	CClientForGS* client = CClientForGS::GetInstance();
	client->Init("127.0.0.1", 54324);
	client->Start();

	CConsole* console = new CConsole;
	console->Start();

	webserver(8086, HttpHandle);

	console->Join();

	client->Join();

	return 0;
}


//#include <stdio.h>
//#include <string.h>
////#include <unistd.h>//windows��û��
//#include "uv.h"
//
//#include "libuv_helper.h"
//
//uv_loop_t *loop;
//uv_tty_t tty;
//
//void myread(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf) {
//	printf("read %d bytes\n", (int) nread);
//	assert(uv_read_stop(stream) == 0);
//	//if (nread > 0)      
//	//	uv_timer_start(&timer, mytimer, 1, 0);
//}
//
//
//int main() {
//	loop = uv_default_loop();
//
//	uv_tty_init(loop, &tty, 1, 0);
//	//UV_TTY_MODE_NORMAL ����ڸ߰汾�вŻ��У���
//	//uv_tty_set_mode(&tty, UV_TTY_MODE_NORMAL);//UV_TTY_MODE_NORMAL
//
//	uv_tty_set_mode(&tty, 0);// 1 rawģʽ  0 normalģʽ
//
//	if (uv_guess_handle(1) == UV_TTY) {
//		uv_write_t req;
//		uv_buf_t buf;
//		buf.base = "\033[41;37m";
//		buf.len = strlen(buf.base);
//		uv_write(&req, (uv_stream_t*) &tty, &buf, 1, NULL);
//	}
//
//	uv_write_t req;
//	uv_buf_t buf;
//	buf.base = "Hello TTY\n";
//	buf.len = strlen(buf.base);
//	uv_write(&req, (uv_stream_t*) &tty, &buf, 1, NULL);
//	//uv_tty_reset_mode();
//
//	uv_read_start((uv_stream_t*) &tty, alloc_cb, myread);
//
//	return uv_run(loop, UV_RUN_DEFAULT);
//}


//#include <assert.h>
//#include <uv.h>
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//
//#include "libuv_helper.h"
//
//uv_loop_t *loop;
//uv_tty_t tty;
//uv_timer_t timer;
//
//void myread(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf);
//
////void alloc(uv_handle_t *handle, size_t suggested, uv_buf_t *buf) {
////	buf->len = 1;
////	buf->base = malloc(1);
////}
//
//void mytimer(uv_timer_t *timer/*, int status*/) {
//	printf("going back for a read\n");
//	assert(uv_read_start((uv_stream_t*) &tty, alloc_cb, myread) == 0);
//}
//
////tty̫�����ˣ��� ��������ı����룡 ��ϵͳ����������Ͳ��ᣡ
//void myread(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf) {
//	//������ Ϊʲô���Ǳ�ʵ��Ҫ�������ֽڣ���
//	printf("read %d bytes content is ��%s��\n", (int) nread, buf->base);
//	assert(uv_read_stop(stream) == 0);
//	if (nread > 0)      
//		uv_timer_start(&timer, mytimer, 1, 0);
//}
//
//int main() {
//
//	char str[3];
//	printf("������һ������:\n");
//	scanf("%s",str);
//	printf("������ĺ�����:%s\n",str);
//
//	loop = uv_default_loop();
//
//	uv_tty_init(loop, &tty, 0, 1);
//	uv_timer_init(loop, &timer);
//
//	uv_tty_set_mode(&tty, 0);// 1 rawģʽ  0 normalģʽ
//	//uv_tty_set_mode(&tty, 1);// 1 rawģʽ  0 normalģʽ
//
//	uv_read_start((uv_stream_t*) &tty, alloc_cb, myread);
//	return uv_run(loop, UV_RUN_DEFAULT);
//}