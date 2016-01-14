#include "TimeZUtil.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include "StringUtil.h"
#include "cocos2d.h"

TINA_NS_BEGIN
//与服务端的时间差
static uint64_t m_beginTimeStamp = TimeUtil::getLocalTimestamp();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
static uint64_t m_beginTimePoint = GetTickCount();
#else
static std::chrono::steady_clock::time_point m_beginTimePoint = std::chrono::steady_clock::now();
#endif

uint64_t TimeUtil::getLocalTimestamp()
{
	return std::time(nullptr);
}

uint64_t TimeUtil::getTimestamp()
{
	uint64_t _seconds = 0;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	auto _endTimePoint = GetTickCount();
	_seconds = (_endTimePoint - m_beginTimePoint) /1000;
#else
	std::chrono::steady_clock::time_point _endTimePoint = std::chrono::steady_clock::now();
	_seconds = std::chrono::duration_cast<std::chrono::seconds>(_endTimePoint - m_beginTimePoint).count();
#endif

	
	return m_beginTimeStamp + _seconds;
}

uint64_t TimeUtil::getTimestamp(const std::string& timeStr)
{
	std::vector<std::string> _timeList;
	StringUtil::split(timeStr, ":", &_timeList);

	//获取当前时间
	std::time_t __time = getTimestamp();
	auto _localtime = std::localtime(&__time);
    auto __tm = std::localtime(&__time);
	tm _activityStructTime;
	{
		_activityStructTime.tm_year = _localtime->tm_year;
		_activityStructTime.tm_mon = _localtime->tm_mon;
		_activityStructTime.tm_mday = _localtime->tm_mday;
        int _tm_hour = atoi(_timeList[0].c_str());
        int _tm_min = atoi(_timeList[1].c_str());
        int _tm_sec = atoi(_timeList[2].c_str());
		_activityStructTime.tm_hour = _tm_hour;
		_activityStructTime.tm_min = _tm_min;
		_activityStructTime.tm_sec = _tm_sec;
        _activityStructTime.tm_isdst = 0;
	}
    uint64_t _ts = std::mktime(&_activityStructTime);
	return _ts;
}

uint64_t TimeUtil::convertToTimeStamp(const std::string& dateStr)
{
	uint64_t _timeStamp = 0;
	int _year = 0;
	int _month = 0;
	int _day = 0;
	int _hour = 0;
	int _minute = 0;
	int _second = 0;

	do
	{
		std::string _str = dateStr;
		//将多余的空格去掉，最后只留一个空格
		StringUtil::replace(&_str, "  ", " ");
		std::vector<std::string> _strVec1;
		StringUtil::split(_str, " ", &_strVec1);
		CC_BREAK_IF(_strVec1.empty());

		std::vector<int> _strVec2;
		StringUtil::split2i(_strVec1.front(), "-", &_strVec2);
		CC_BREAK_IF(_strVec2.empty());

		_year = _strVec2[0];
		if (_strVec2.size() > 1)
			_month = _strVec2[1];
		if (_strVec2.size() > 2)
			_day = _strVec2[2];

		if (_strVec1.size() == 2)
		{
			std::vector<int> _strVec3;
			StringUtil::split2i(_strVec1.back(), "-", &_strVec3);
			if (_strVec3.size() > 0)
				_hour = _strVec3[0];
			if (_strVec3.size() > 1)
				_minute = _strVec3[1];
			if (_strVec3.size() > 2)
				_second = _strVec3[2];
		}

		tm _structTime;
		{
			_structTime.tm_year = MAX((_year - 1900), 0);
			_structTime.tm_mon = MAX((_month - 1), 0);
			_structTime.tm_mday = _day;
			_structTime.tm_hour = _hour;
			_structTime.tm_min = _minute;
			_structTime.tm_sec = _second;
		}
		_timeStamp = std::mktime(&_structTime);
	} while (0);
	return _timeStamp;
}

void TimeUtil::getTimeDetail(int* year, int* month, int* day, int* hour, int* minute, int* second)
{
	std::time_t __time = getTimestamp();
	auto _localtime = std::localtime(&__time);

	if (year != nullptr)
		*year = _localtime->tm_year + 1900;
	if (month != nullptr)
		*month = _localtime->tm_mon + 1;
	if (day != nullptr)
		*day = _localtime->tm_mday;
	if (hour != nullptr)
		*hour = _localtime->tm_hour;
	if (minute != nullptr)
		*minute = _localtime->tm_min;
	if (second != nullptr)
		*second = _localtime->tm_sec;
}

std::tm TimeUtil::getTime()
{
	std::time_t _timestamp = getTimestamp();
	return *std::localtime(&_timestamp);
}

void TimeUtil::convertSecondsToTime(int seconds, int* dstHour, int* dstMinute, int* dstSecond)
{
	if (dstHour != nullptr)
		*dstHour = seconds / 3600;
	if (dstMinute != nullptr)
		*dstMinute = (seconds % 3600) / 60;
	if (dstSecond != nullptr)
		*dstSecond = (seconds % 3600) % 60;
}

void TimeUtil::setTimeStamp(uint64_t timestamp)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	m_beginTimePoint = GetTickCount();
#else
	m_beginTimePoint = std::chrono::steady_clock::now();
#endif

	m_beginTimeStamp = timestamp;
}
TINA_NS_END