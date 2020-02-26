#pragma once

#ifndef __SSA_VDR_STRINGCODING_H
#define __SSA_VDR_STRINGCODING_H

#include <iostream>
#include <string>
#include "export.h"

using namespace std;

namespace ssa
{
	class xmVDR_EXPORT StringCoding
	{
	public:
		StringCoding(void);
		~StringCoding(void);

		////////ʹ��C++11������ʵ��/////////////////////////////
		static std::string UTF8ToGBK(const std::string & utf8);
		static std::string GBKToUTF8(const std::string & gbk);
		////////////////////////////////////////////////////////

		static std::string  UTF_8ToGB2312(const char *pText, int pLen); //utf_8תΪgb2312
		static std::string  GB2312ToUTF_8(const char *pText, int pLen); //gb2312 תutf_8
		static std::string  UrlGB2312(const char * str);                                   //urlgb2312����
		static std::string  UrlUTF8(const char * str);                                     //urlutf8 ����
		static std::string  UrlUTF8Decode(std::string str);                          //urlutf8����
		static std::string  UrlGB2312Decode(std::string str);                        //urlgb2312����
		static std::string  FormatString(const char *lpcszFormat, ...);
		static std::wstring FormatWstring(const wchar_t *lpcwszFormat, ...);
	};
}

#endif