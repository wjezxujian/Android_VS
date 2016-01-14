#pragma once

/****************************************************************************
�� �ܣ�ʱ�乤����
============================================================================
Copyright (c) 2015 Tungway
�����ߣ���ΰ
�������ڣ�2015-07-20
����޸��ߣ���ΰ
����޸����ڣ�2015-07-20
****************************************************************************/

#include "include/tinaMacros.h"
#include <string>
#include <ctime>

TINA_NS_BEGIN
class TimeUtil
{
public:
	/**	��ȡ����ʱ�������������Уʱ��*/
	static uint64_t getLocalTimestamp();

	/**	��ȡ��ǰ��ʱ���(������Уʱ��)
	*/
	static uint64_t getTimestamp();

	/**	��ȡ��ǰXʱX��X���ʱ���(������Уʱ��)
	*	@param timeStr XʱX��X����ַ�����hh::mm::ss
	*/
	static uint64_t getTimestamp(const std::string& timeStr);

	/**	������ʱ���ʽ���ַ�����yyyy-mm-dd hh:mm:ss��ת��Ϊʱ���
	*	@param timeStr XʱX��X����ַ�����hh::mm::ss
	*/
	static uint64_t convertToTimeStamp(const std::string& dateStr);

	/**	��ȡ��ǰ������ʱ���룬��ָ�봫��(������Уʱ��)
	*	@param year �� [1900-2018]�� ����Ҫ��nullptr
	*	@param month �� [1-12]�� ����Ҫ��nullptr
	*	@param day �� [1-31]�� ����Ҫ��nullptr
	*	@param hour ʱ [0-23]�� ����Ҫ��nullptr
	*	@param minute �� [0-59]�� ����Ҫ��nullptr
	*	@param second �� [0-59]�� ����Ҫ��nullptr
	*/
	static void getTimeDetail(int* year, int* month, int* day, int* hour, int* minute, int* second);

	/**	��ȡ��ǰ��ʱ��ṹ(������Уʱ��)*/
	static std::tm getTime();

	/**	����ת��������Сʱ��������������*/
	static void convertSecondsToTime(int seconds, int* dstHour, int* dstMinute, int* dstSecond);

	/**	���������˵�ʱ���*/
	static void setTimeStamp(uint64_t timestamp);
};
TINA_NS_END