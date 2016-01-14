#pragma once

/****************************************************************************
功 能：时间工具类
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-07-20
最后修改者：董伟
最后修改日期：2015-07-20
****************************************************************************/

#include "include/tinaMacros.h"
#include <string>
#include <ctime>

TINA_NS_BEGIN
class TimeUtil
{
public:
	/**	获取本地时间戳（不与服务端校时）*/
	static uint64_t getLocalTimestamp();

	/**	获取当前的时间戳(与服务端校时过)
	*/
	static uint64_t getTimestamp();

	/**	获取当前X时X分X秒的时间戳(与服务端校时过)
	*	@param timeStr X时X分X秒的字符串：hh::mm::ss
	*/
	static uint64_t getTimestamp(const std::string& timeStr);

	/**	将日期时间格式的字符串（yyyy-mm-dd hh:mm:ss）转化为时间戳
	*	@param timeStr X时X分X秒的字符串：hh::mm::ss
	*/
	static uint64_t convertToTimeStamp(const std::string& dateStr);

	/**	获取当前年月日时分秒，用指针传出(与服务端校时过)
	*	@param year 年 [1900-2018]， 不需要则传nullptr
	*	@param month 月 [1-12]， 不需要则传nullptr
	*	@param day 日 [1-31]， 不需要则传nullptr
	*	@param hour 时 [0-23]， 不需要则传nullptr
	*	@param minute 分 [0-59]， 不需要则传nullptr
	*	@param second 秒 [0-59]， 不需要则传nullptr
	*/
	static void getTimeDetail(int* year, int* month, int* day, int* hour, int* minute, int* second);

	/**	获取当前的时间结构(与服务端校时过)*/
	static std::tm getTime();

	/**	将秒转换成整数小时、整数分钟与秒*/
	static void convertSecondsToTime(int seconds, int* dstHour, int* dstMinute, int* dstSecond);

	/**	设置与服务端的时间差*/
	static void setTimeStamp(uint64_t timestamp);
};
TINA_NS_END