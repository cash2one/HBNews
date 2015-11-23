#ifndef pb_helper_h__
#define pb_helper_h__

//格式化PB结构中的32位整数域到sql
#define SQL_APPEND_FIELD_U(object_ref, field_name) \
	do {\
	if (object_ref.has_##field_name##()){\
	used = sprintf_s(p, left, " `"#field_name"`= %u,", object_ref.##field_name##());\
	if (used == -1){\
	return -1;\
	}\
	p += used;\
	left -= used;\
	}\
	} while (false);\

//格式化PB结构中的字符串域到sql
#define SQL_APPEND_FIELD_S(object_ref, field_name) \
	do {\
	if (object_ref.has_##field_name##()){\
	used = sprintf_s(p, left, " `"#field_name"`= '%s',", object_ref.##field_name##().c_str());\
	if (used == -1){\
	return -1;\
	}\
	p += used;\
	left -= used;\
	}\
	} while (false);\

#endif // pb_helper_h__