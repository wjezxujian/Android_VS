#include "StringUtil.h"
#include "cocos2d.h"
#include <stdint.h>
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <Windows.h>
#endif

TINA_NS_BEGIN
void StringUtil::trim(std::string* str)
{
	if (str == nullptr || str->empty())return;
	//去除字符串首部的空白字符串
	while (!str->empty() && str->front() == ' ')
	{
		str->erase(0, 1);
	}

	//去除字符串尾部的空白字符串
	while (!str->empty() && str->back() == ' ')
	{
		str->erase(str->size()-1, 1);
	}
}

void StringUtil::eraseWhiteSpaces(std::string* str)
{
	if (str == nullptr || str->empty())return;
	int _idx = str->find(' ');
	while (_idx > 0)
	{
		str->erase(_idx, 1);
		_idx = str->find(' ');
	}
}

void StringUtil::toLower(std::string* str)
{
	if (str == nullptr || str->empty())return;
	for (int i = 0; i < str->size(); ++i)
	{
		char _chr = str->at(i);
		char _newChr = tolower(_chr);
		(*str)[i] = _newChr;
	}
}

void StringUtil::toUpper(std::string* str)
{
	if (str == nullptr || str->empty())return;
	for (int i = 0; i < str->size(); ++i)
	{
		char _chr = str->at(i);
		char _newChr = toupper(_chr);
		(*str)[i] = _newChr;
	}
}

void StringUtil::split(const std::string& srcStr, const std::string& splitFlag, std::vector<std::string>* container)
{
	if (srcStr.empty() || splitFlag.empty() || !container)return;

	const char* _result = nullptr;
	//指针地址
	uint16_t _pos = 0;
	while (true)
	{
		_result = strstr(srcStr.c_str() + _pos, splitFlag.c_str());
		if (!_result)
		{
			container->push_back(srcStr.c_str() + _pos);
			break;
		}

		char _buff[2048];
		memset(_buff, 0, sizeof(_buff));
		memcpy(_buff, srcStr.c_str() + _pos, _result - (srcStr.c_str() + _pos));
		container->push_back(_buff);
		_pos = _result - srcStr.c_str() + splitFlag.size();
	}
}

void StringUtil::split2f(const std::string& srcStr, const std::string& splitFlag, std::vector<float>* container)
{
	std::vector<std::string> _tmpContainer;
	split(srcStr, splitFlag, &_tmpContainer);
	for (const auto& str : _tmpContainer)
	{
		container->push_back(atof(str.c_str()));
	}
}

void StringUtil::split2i(const std::string& srcStr, const std::string& splitFlag, std::vector<int>* container)
{
	std::vector<std::string> _tmpContainer;
	split(srcStr, splitFlag, &_tmpContainer);
	for (const auto& str : _tmpContainer)
	{
		container->push_back(atoi(str.c_str()));
	}
}

void StringUtil::split2l(const std::string& srcStr, const std::string& splitFlag, std::vector<int64_t>* container)
{
	std::vector<std::string> _tmpContainer;
	split(srcStr, splitFlag, &_tmpContainer);
	for (const auto& str : _tmpContainer)
	{
		container->push_back(atol(str.c_str()));
	}
}

void StringUtil::gb2312Str2utf8Str(const std::string& gb2312Str, std::string* utf8Str)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	int _len = MultiByteToWideChar(CP_ACP, 0, gb2312Str.c_str(), -1, NULL, 0);
	wchar_t* _wstr = new wchar_t[_len + 1];
	memset(_wstr, 0, _len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312Str.c_str(), -1, _wstr, _len);
	_len = WideCharToMultiByte(CP_UTF8, 0, _wstr, -1, NULL, 0, NULL, NULL);
	char* _str = new char[_len + 1];
	memset(_str, 0, _len + 1);
	WideCharToMultiByte(CP_UTF8, 0, _wstr, -1, _str, _len, NULL, NULL);
	if (_wstr) delete[] _wstr;
	utf8Str->append(_str);
	delete[] _str;
#else
	utf8Str->append(gb2312Str);
#endif
}

std::string StringUtil::gb2312Str2utf8Str(const std::string& gb2312Str)
{
	std::string _utf8Str;
	gb2312Str2utf8Str(gb2312Str, &_utf8Str);
	return _utf8Str;
}

bool StringUtil::isUtf8(const char* pBuffer)
{
	bool IsUTF8 = true;
	int size = strlen(pBuffer);
	unsigned char* start = (unsigned char*)pBuffer;
	unsigned char* end = (unsigned char*)pBuffer + size;
	while (start < end)
	{
		if (*start < 0x80) // (10000000): 值小于0x80的为ASCII字符
		{
			start++;
		}
		else if (*start < (0xC0)) // (11000000): 值介于0x80与0xC0之间的为无效UTF-8字符
		{
			IsUTF8 = false;
			break;
		}
		else if (*start < (0xE0)) // (11100000): 此范围内为2字节UTF-8字符
		{
			if (start >= end - 1)
				break;
			if ((start[1] & (0xC0)) != 0x80)
			{
				IsUTF8 = false;
				break;
			}
			start += 2;
		}
		else if (*start < (0xF0)) // (11110000): 此范围内为3字节UTF-8字符
		{
			if (start >= end - 2)
				break;
			if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80)
			{
				IsUTF8 = false;
				break;
			}
			start += 3;
		}
		else
		{
			IsUTF8 = false;
			break;
		}
	}
	return IsUTF8;
}

void StringUtil::replace(std::string* srcStr, const std::string& repStr, const std::string& dstStr)
{
	while (true)
	{
		auto _pos = srcStr->find(repStr.c_str(), 0);
		CC_BREAK_IF(_pos == std::string::npos);
		srcStr->replace(_pos, repStr.size(), dstStr.c_str());
	}
}

std::string StringUtil::getSubStringOfUTF8String(const std::string& str, std::string::size_type start, std::string::size_type length)
{
	if (length == 0)
	{
		return "";
	}
	std::string::size_type c, i, ix, q, min = std::string::npos, max = std::string::npos;
	for (q = 0, i = 0, ix = str.length(); i < ix; i++, q++)
	{
		if (q == start)
		{
			min = i;
		}
		if (q <= start + length || length == std::string::npos)
		{
			max = i;
		}

		c = (unsigned char)str[i];

		if (c <= 127) i += 0;
		else if ((c & 0xE0) == 0xC0) i += 1;
		else if ((c & 0xF0) == 0xE0) i += 2;
		else if ((c & 0xF8) == 0xF0) i += 3;
		else return "";//invalid utf8
	}
	if (q <= start + length || length == std::string::npos)
	{
		max = i;
	}
	if (min == std::string::npos || max == std::string::npos)
	{
		return "";
	}
	return str.substr(min, max);
}

int StringUtil::getCharacterLength(const std::string& str)
{
	int _length = 0;
	int _idx = 0;
	int _charLen = strlen(str.c_str());

	while (_idx < str.size())
	{
		char _chr = str.at(_idx);
		if ((_chr & 0xC0) != 0x80)
		{
			if ((_chr & 0xC0) == 0xC0)
				_length = _length + 2;
			else
				_length++;
		}
		++_idx;
	}
	return _length;
}

std::string StringUtil::format(const char* format, ...)
{
	std::string _str;
	{
		va_list _ap;
		va_start(_ap, format);
		const int size = 0x800;
		char buff[size + 1] = { 0 };
		vsnprintf(buff, size, format, _ap);
		va_end(_ap);
		_str.append(buff);
	}
	return _str;
}

std::string StringUtil::formatVa(const char* format, va_list va)
{
	std::string _str;
	{
		const int size = 0x800;
		char buff[size + 1] = { 0 };
		vsnprintf(buff, size, format, va);
		_str.append(buff);
	}
	return _str;
}
TINA_NS_END