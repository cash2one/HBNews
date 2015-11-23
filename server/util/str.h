#ifndef str_h__
#define str_h__

#include <vector>
#include <string>

char* w2c(const wchar_t *pwstr, char *pcstr, size_t len);

//需要手动释放缓冲区
int w2c_needfree(const wchar_t *pwstr, char* &buf);

unsigned __int32 w2i(const wchar_t *pwstr);



void split_string(const std::string& s, std::vector<std::string>& v, const std::string& c);
void split_string_2i(const std::string& s, std::vector<int>& v, const std::string& c);
bool split_string_to_int_array(const std::string& str, const std::string& c, unsigned int* int_arr, unsigned int &arr_len, const unsigned int max_arr_len, const bool need_sort = false );
wchar_t * ANSIToUnicode( const char* str );
char * UnicodeToANSI( const wchar_t *str );
wchar_t * UTF8ToUnicode( const char* str );
char * UnicodeToUTF8( const wchar_t *str );
char* UTF8ToANSI(const char* str);
char* ANSIToUTF8(const char* str);
void trim(::std::string & s);


//chrome里面的int string实现
//template <typename STR, typename INT, typename UINT, bool NEG>
//struct IntToStringT {
//
//	template <typename INT2, typename UINT2, bool NEG2>
//	struct ToUnsignedT { };
//
//	template <typename INT2, typename UINT2>
//	struct ToUnsignedT<INT2, UINT2, false> {
//		static UINT2 ToUnsigned(INT2 value) {
//			return static_cast<UINT2>(value);
//		}
//	};
//
//	template <typename INT2, typename UINT2>
//	struct ToUnsignedT<INT2, UINT2, true> {
//		static UINT2 ToUnsigned(INT2 value) {
//			return static_cast<UINT2>(value < 0 ? -value : value);
//		}
//	};
//
//	static  STR IntToString(INT value) {
//		const int kOutputBufSize = 3 * sizeof(INT) + 1;
//		STR outbuf(kOutputBufSize, 0);
//
//		bool is_neg = value < 0;
//		UINT res = ToUnsignedT<INT, UINT, NEG>::ToUnsigned(value);
//
//		for (typename STR::iterator it = outbuf.end();;) {
//			--it;
//			DCHECK(it != outbuf.begin());
//			*it = static_cast<typename STR::value_type>((res % 10) + '0');
//			res /= 10;
//			if (res == 0) {
//				if (is_neg) {
//					--it;
//					DCHECK(it != outbuf.begin());
//					*it = static_cast<typename STR::value_type>('-');
//				}
//				return STR(it, outbuf.end());
//			}
//		}
//	}
//};
//
//inline std::string IntToString(int value) {
//	return IntToStringT<std::string, int, unsigned int, true>::
//		IntToString(value);
//}
//inline std::wstring IntToWString(int value) {
//	return IntToStringT<std::wstring, int, unsigned int, true>::
//		IntToString(value);
//}
//inline std::string UintToString(unsigned int value) {
//	return IntToStringT<std::string, unsigned int, unsigned int, false>::
//		IntToString(value);
//}
//inline std::wstring UintToWString(unsigned int value) {
//	return IntToStringT<std::wstring, unsigned int, unsigned int, false>::
//		IntToString(value);
//}
//inline std::string Int64ToString(int64 value) {
//	return IntToStringT<std::string, int64, uint64, true>::
//		IntToString(value);
//}
//inline std::wstring Int64ToWString(int64 value) {
//	return IntToStringT<std::wstring, int64, uint64, true>::
//		IntToString(value);
//}
//inline std::string Uint64ToString(uint64 value) {
//	return IntToStringT<std::string, uint64, uint64, false>::
//		IntToString(value);
//}
//inline std::wstring Uint64ToWString(uint64 value) {
//	return IntToStringT<std::wstring, uint64, uint64, false>::
//		IntToString(value);
//}

#endif // str_h__
