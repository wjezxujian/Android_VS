#pragma once

/****************************************************************************
功 能：数学工具类
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-04-16
最后修改者：董伟
最后修改日期：2015-04-16
****************************************************************************/

#include "include/tinaMacros.h"
#include <string>

TINA_NS_BEGIN
class MathUtil
{
public:
	/**	产生一个区间内的随机数
	*	产生的随机数 >= min 且 <= max
	*	@param min 最小值
	*	@param max 最大值
	*/
	static int random(int min, int max);

	/**	比较两个版本号（用“.”隔开），如：0.4.3343 > 0.3.24
	*	ver1 >= ver2 则返回true，否则返回false
	*	@param ver1 版本号1
	*	@param ver2 版本号2
	*/
	static bool compareVersion(const std::string& ver1, const std::string& ver2);

	/** 比较两个浮点数
	*/
	static bool cmpFloat(float n1, float n2, float range = 0.000001);
};
TINA_NS_END