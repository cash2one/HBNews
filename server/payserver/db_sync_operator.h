#ifndef db_sync_operator_h__
#define db_sync_operator_h__
#pragma once

#include <map>
#include <string>
#include "def.h"

#include "order.h"

namespace db_sync_operator{

	//¸üÐÂ¶©µ¥×´Ì¬
	int order_notify(const COrder* pOrder);
	int order_process(const std::string &order_id, const COrder::ESTATUS eStatus);
}


#endif // db_sync_operator_h__