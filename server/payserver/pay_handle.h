#ifndef pay_handle_h__
#define pay_handle_h__

#include "webserver.h"

class CPayHandle{
public:
	static bool AdwoPaied(webserver::http_request* r);
};

#endif // pay_handle_h__

