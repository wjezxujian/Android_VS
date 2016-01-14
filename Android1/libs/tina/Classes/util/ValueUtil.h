#pragma once

/****************************************************************************
功 能：cocos2d::Value操作工具类
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-08-18
最后修改者：董伟
最后修改日期：2015-08-18
****************************************************************************/

#include "include/tinaMacros.h"
#include "base/CCValue.h"

TINA_NS_BEGIN

class ValueUtil
{
public:

	/* 读取ValueMap中的某个整数
	* 参数 1：源数据
	* 参数 x：各级字段名
	* 返回值：目标值
	*/
	static int readInt(cocos2d::Value* value, ...);
	static int readInt(cocos2d::ValueMap* value, ...);

	/* 读取ValueMap中的某个浮点数
	* 参数 1：源数据
	* 参数 x：各级字段名
	* 返回值：目标值
	*/
	static float readFloat(cocos2d::Value* value, ...);
	static float readFloat(cocos2d::ValueMap* value, ...);

	/* 读取ValueMap中的某个字符串
	* 参数 1：源数据
	* 参数 x：各级字段名
	* 返回值：目标值
	*/
	static const char* readCString(cocos2d::Value* value, ...);
	static const char* readCString(cocos2d::ValueMap* value, ...);

	/* 读取ValueMap中的某个字符串
	* 参数 1：源数据
	* 参数 x：各级字段名
	* 返回值：目标值
	*/
	static std::string readString(cocos2d::Value* value, ...);
	static std::string readString(cocos2d::ValueMap* value, ...);

	/* 读取ValueMap中的某个字典数据
	* 参数 1：源数据
	* 参数 x：各级字段名
	* 返回值：目标值
	*/
	static cocos2d::ValueMap* readDict(cocos2d::Value* value, ...);
	static cocos2d::ValueMap* readDict(cocos2d::ValueMap* value, ...);

	/* 读取ValueMap中的某个数组数据
	* 参数 1：源数据
	* 参数 x：各级字段名
	* 返回值：目标值
	*/
	static cocos2d::ValueVector* readArray(cocos2d::Value* value, ...);
	static cocos2d::ValueVector* readArray(cocos2d::ValueMap* value, ...);

	/* 读取ValueMap中的某个数据
	* 参数 1：源数据
	* 参数 2：各级字段名
	* 返回值：目标值
	*/
	static cocos2d::Value* readValue(const cocos2d::Value* value, va_list keys);
	static cocos2d::Value* readValue(const cocos2d::ValueMap* value, va_list keys);

};

TINA_NS_END
