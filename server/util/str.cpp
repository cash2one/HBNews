#include <stdio.h>
#include <algorithm>

//#include <stringstream>
#include <strstream>
#include <functional>
#include <cctype>

#include "str.h"
#include "def.h"

char* w2c(char *pcstr,const wchar_t *pwstr, size_t len)
{
	int nlength= wcslen(pwstr);
	//获取转换后的长度
	int nbytes = WideCharToMultiByte( 0, 0, pwstr, nlength, NULL,0,NULL, NULL ); 

	ASSERT(len >= nbytes);//强制

	if(nbytes > len)   nbytes=len;
	// 通过以上得到的结果，转换unicode 字符为ascii 字符
	WideCharToMultiByte( 0,0, pwstr, nlength,   pcstr, nbytes, NULL,   NULL );
	return pcstr ;
}

int w2c_needfree(const wchar_t *pwstr, char* &buf)
{
	int nlength= wcslen(pwstr);
	//获取转换后的长度
	int nbytes = WideCharToMultiByte( 0, 0, pwstr, nlength, NULL, 0, NULL, NULL); 

	//ASSERT(len >= nbytes);//强制

	buf = new char[nbytes];

	//if(nbytes > len)   nbytes=len;
	// 通过以上得到的结果，转换unicode 字符为ascii 字符
	WideCharToMultiByte( 0, 0, pwstr, nlength, buf, nbytes, NULL, NULL );
	//return buf;
	return nbytes;
}


wchar_t * ANSIToUnicode( const char* str )
{
	int    textlen ;
	wchar_t * result;
	textlen = MultiByteToWideChar( CP_ACP, 0, str,-1,    NULL,0 );  
	result = (wchar_t *)malloc((textlen+1)*sizeof(wchar_t));  
	memset(result,0,(textlen+1)*sizeof(wchar_t));  
	MultiByteToWideChar(CP_ACP, 0,str,-1,(LPWSTR)result,textlen );  
	return    result;  
}

char * UnicodeToANSI( const wchar_t *str )
{
	char * result;
	int textlen;
	// wide char to multi char
	textlen = WideCharToMultiByte( CP_ACP,    0,    str,    -1,    NULL, 0, NULL, NULL );
	result =(char *)malloc((textlen+1)*sizeof(char));
	memset( result, 0, sizeof(char) * ( textlen + 1 ) );
	WideCharToMultiByte( CP_ACP, 0, str, -1, result, textlen, NULL, NULL );
	return result;
}


wchar_t * UTF8ToUnicode( const char* str )
{
	int    textlen ;
	wchar_t * result;
	textlen = MultiByteToWideChar( CP_UTF8, 0, str,-1,    NULL,0 );  
	result = (wchar_t *)malloc((textlen+1)*sizeof(wchar_t));  
	memset(result,0,(textlen+1)*sizeof(wchar_t));  
	MultiByteToWideChar(CP_UTF8, 0,str,-1,(LPWSTR)result,textlen );  
	return    result;  
}

char * UnicodeToUTF8( const wchar_t *str )
{
	char * result;
	int textlen;
	// wide char to multi char
	textlen = WideCharToMultiByte( CP_UTF8,    0,    str,    -1,    NULL, 0, NULL, NULL );
	result =(char *)malloc((textlen+1)*sizeof(char));
	memset(result, 0, sizeof(char) * ( textlen + 1 ) );
	WideCharToMultiByte( CP_UTF8, 0, str, -1, result, textlen, NULL, NULL );
	return result;
}

char* UTF8ToANSI(const char* str){
	wchar_t * unicodes = UTF8ToUnicode(str);
	char* ansi = UnicodeToANSI(unicodes);
	free(unicodes);
	return ansi;
}

char* ANSIToUTF8(const char* str){
	wchar_t* unicodes  = ANSIToUnicode(str);
	char* utf8 = UnicodeToUTF8(unicodes);
	free(unicodes);
	return utf8;
}

void split_string(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
	std::string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while(std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2-pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if(pos1 != s.length()){
		v.push_back(s.substr(pos1));
	}
}

void split_string_2i(const std::string& s, std::vector<int>& v, const std::string& c)
{
	std::string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while(std::string::npos != pos2)
	{
		v.push_back(atoi(s.substr(pos1, pos2-pos1).c_str()));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}

	if(pos1 != s.length()){
		v.push_back(atoi(s.substr(pos1).c_str()));
	}
}

bool split_string_to_int_array(const std::string& str, const std::string& c, unsigned int* int_arr, unsigned int &arr_len, const unsigned int max_arr_len, const bool need_sort /*= false*/ )
{
	std::vector<int> v;
	split_string_2i(str, v, c);
	if (v.size() > max_arr_len){
		return false;
	}

	arr_len = v.size();

	if (need_sort){
		std::sort(v.begin(), v.end());
	}

	UINT32 i = 0;
	for (std::vector<int>::iterator it = v.begin() ; it != v.end() ; ++it){
		//ASSERT(*it) >= 0;//一定是正数！！
		if (*it < 0){
			return false;
		}
		int_arr[i++] = *it;
	}

	return true;
}

void trim(::std::string & s)
{
	/* //会崩在assert这里
	//#if defined (_DEBUG)
	//extern "C" int __cdecl _chvalidator(
	//int c,
	//int mask
	//)
	//{
	//_ASSERTE((unsigned)(c + 1) <= 256);
	//return _chvalidator_l(NULL, c, mask);
	//}

	//left
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	//right
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	*/

	std::string::size_type pos = s.find_last_not_of(' ');
	if(pos != std::string::npos) {
		s.erase(pos + 1);
		pos = s.find_first_not_of(' ');
		if(pos != std::string::npos) s.erase(0, pos);
	}
	else s.erase(s.begin(), s.end());
}
