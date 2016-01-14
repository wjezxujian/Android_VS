#pragma once

/****************************************************************************
�� �ܣ���ѧ������
============================================================================
Copyright (c) 2015 Tungway
�����ߣ���ΰ
�������ڣ�2015-04-16
����޸��ߣ���ΰ
����޸����ڣ�2015-04-16
****************************************************************************/

#include "include/tinaMacros.h"
#include <string>

TINA_NS_BEGIN
class MathUtil
{
public:
	/**	����һ�������ڵ������
	*	����������� >= min �� <= max
	*	@param min ��Сֵ
	*	@param max ���ֵ
	*/
	static int random(int min, int max);

	/**	�Ƚ������汾�ţ��á�.�����������磺0.4.3343 > 0.3.24
	*	ver1 >= ver2 �򷵻�true�����򷵻�false
	*	@param ver1 �汾��1
	*	@param ver2 �汾��2
	*/
	static bool compareVersion(const std::string& ver1, const std::string& ver2);

	/** �Ƚ�����������
	*/
	static bool cmpFloat(float n1, float n2, float range = 0.000001);
};
TINA_NS_END