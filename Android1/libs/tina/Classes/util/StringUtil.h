#pragma once

/****************************************************************************
功 能：字符串工具类
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-02-28
最后修改者：唐为民
最后修改日期：2015-08-27
****************************************************************************/

#include "include/tinaMacros.h"
#include <string>
#include <vector>

#define STR_FORMAT_INT(__int__) tina::StringUtil::format("%d", __int__).c_str() //将int转化为C字符串
#define STR_FORMAT_FLOAT(__float__) tina::StringUtil::format("%f", __float__).c_str() //将float转化为C字符串
#define STR_FORMAT(__format__, ...) tina::StringUtil::format(__format__, ##__VA_ARGS__).c_str() //格式化C字符串

#define STRING_FORMAT_INT(__int__) tina::StringUtil::format("%d", __int__) //将int转化为String字符串
#define STRING_FORMAT_FLOAT(__float__) tina::StringUtil::format("%f", __float__) //将float转化为String字符串
#define STRING_FORMAT(__format__, ...) tina::StringUtil::format(__format__, ##__VA_ARGS__) //格式化String字符串

TINA_NS_BEGIN
class StringUtil
{
public:
	/**	去除字符串首尾多余空格*/
	static void trim(std::string* str);

	/**	去除字符串内所有的空格（慎用）*/
	static void eraseWhiteSpaces(std::string* str);

	/**	字母转小写*/
	static void toLower(std::string* str);

	/**	字母转大写*/
	static void toUpper(std::string* str);

	/**	字符串分割
	*	@param srcStr 原字符串
	*	@param splitFlag 分隔符（支持多字符）
	*	@param container 分割容器（将原字符串分割成多条子字符串存储于此）
	*/
	static void split(const std::string& srcStr, const std::string& splitFlag, std::vector<std::string>* container);
	
	/**	字符串分割
	*	@param srcStr 原字符串
	*	@param splitFlag 分隔符（支持多字符）
	*	@param container 分割容器
	*/
	static void split2f(const std::string& srcStr, const std::string& splitFlag, std::vector<float>* container);
	
	/**	字符串分割
	*	@param srcStr 原字符串
	*	@param splitFlag 分隔符（支持多字符）
	*	@param container 分割容器
	*/
	static void split2i(const std::string& srcStr, const std::string& splitFlag, std::vector<int>* container);

	/**	字符串分割
	*	@param srcStr 原字符串
	*	@param splitFlag 分隔符（支持多字符）
	*	@param container 分割容器
	*/
	static void split2l(const std::string& srcStr, const std::string& splitFlag, std::vector<int64_t>* container);

	/**	gb2312字符串转utf-8字符串
	*	注意：此方法只能运行在win32环境上
	*/
	static void gb2312Str2utf8Str(const std::string& gb2312Str, std::string* utf8Str);

	/**	gb2312字符串转utf-8字符串
	*	注意：此方法只能运行在win32环境上
	*/
	static std::string gb2312Str2utf8Str(const std::string& gb2312Str);

	/** 判断一个字符串是否为UTF-8编码
	*/
	static bool isUtf8(const char* srcString);

	/**	字符串替换
	*	@param srcStr 源字符串
	*	@param repStr 欲被替换的字符串
	*	@param dstStr 替换后的字符串
	*/
	static void replace(std::string* srcStr, const std::string& repStr, const std::string& dstStr);

	/**	截取UTF-8（汉字算一个字符）字符串
	*	@param str utf-8字符串
	*	@param start 起始位置（从0开始）
	*	@param length 截取长度
	*/
	static std::string getSubStringOfUTF8String(const std::string& str, std::string::size_type start, std::string::size_type length);

	/**	获取单词（文字）长度，一个汉字算2个长度*/
	static int getCharacterLength(const std::string& str);

	/**	格式化字符串*/
	static std::string format(const char* format, ...);
	static std::string formatVa(const char* format, va_list va);
};
TINA_NS_END