#pragma once

/****************************************************************************
功 能：字符串转属性值
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-02-28
最后修改者：董伟
最后修改日期：2015-05-08
****************************************************************************/

#include "include/tinaMacros.h"
#include "cocos2d.h"

TINA_NS_BEGIN
class ParamUtil
{
public:
	/** 读取浮点数
	*	特殊变量：winwidth = 屏幕宽度， 支持一阶基本运算：*乘，/除，+加，-减，如:winwidth*2，winwith-10，winwidth+6，winwidth/3
	*	特殊变量：winheight = 屏幕高度，支持一阶基本运算
	*/
	static float readFloat(const std::string& str);

	/**	读取有符号整型	*/
	static int readInt(const std::string& str);

	/**	读取无符号整型	*/
	static uint32_t readUInt(const std::string& str);

	/**	读取百分比值	
	*	如：将"50%"读取为50
	*/
	static float readPercent(const std::string& str);

	/**	读取布朗值
	*	如：将"true","yes","1"读取为true，其它都将读取为false
	*/
	static bool readBool(const std::string& str);

	/**	读取用","隔开的4个值读取为cocos2d::Rect
	*	如：将"1,2,3,4"读取为cocos2d::Rect(1,2,3,4)
	*/
	static cocos2d::Rect readRect(const std::string& str);

	/**	读取用","隔开的2个值读取为cocos2d::Size
	*	如：将"1,2"读取为cocos2d::Size(1,2)
	*/
	static cocos2d::Size readSize(const std::string& str);

	/**	读取用","隔开的2个值读取为cocos2d::Vec2
	*	如：将"1,2"读取为cocos2d::Vec2(1,2)
	*/
	static cocos2d::Vec2 readVec2(const std::string& str);

	/**	读取RGB颜色值
	*	如：将"#ff00ff"读取为：cocos2d::Color3B(255, 0, 255)
	*	支持如下常用颜色单词：white,yellow,blue,green,red,magenta,black,orange,gray
	*/
	static cocos2d::Color3B readColor3B(const std::string& str);

	/**	读取RGBA颜色值
	*	如：将"#ff00ffff"读取为：cocos2d::Color4B(255, 0, 255, 255)
	*	支持如下常用颜色单词：white,yellow,blue,green,red,magenta,black,orange,gray
	*/
	static cocos2d::Color4B readColor4B(const std::string& str);

	/**	将字符串转换为VAlign枚举
	*	"top" => VAlign::TOP
	*	"center"或"middle" => VAlign::CENTER
	*	"bottom" => VAlign::BOTTOM
	*/
	static VAlign readValign(const std::string& str);

	/**	将字符串转换为HAlign枚举
	*	"left" => HAlign::LEFT
	*	"center"或"middle" => HAlign::CENTER
	*	"right" => HAlign::RIGHT
	*/
	static HAlign readHalign(const std::string& str);

	/**	将字符串转换为cocos2d::TextHAlignment枚举
	*	"left" => cocos2d::TextHAlignment::LEFT
	*	"center"或"middle" => cocos2d::TextHAlignment::CENTER
	*	"right" => cocos2d::TextHAlignment::RIGHT
	*/
	static cocos2d::TextHAlignment readTextHalign(const std::string& str);

	/**	将字符串转换为cocos2d::TextVAlignment枚举
	*	"top" => cocos2d::TextVAlignment::TOP
	*	"center"或"middle" => cocos2d::TextVAlignment::CENTER
	*	"bottom" => cocos2d::TextVAlignment::BOTTOM
	*/
	static cocos2d::TextVAlignment readTextValign(const std::string& str);

	/**	将字符串转换为ScrollDirection枚举
	*	"horizontal" => ScrollDirection::HORIZONTAL
	*	"vertical" => ScrollDirection::VERTICAL
	*	"both" => ScrollDirection::BOTH
	*	"auto" => ScrollDirection::AUTO
	*/
	static ScrollDirection readScrollDirection(const std::string& str);

	/**	将字符串转换为LayoutType枚举
	*	"horizontal" => LayoutType::HORIZONTAL
	*	"vertical" => LayoutType::VERTICAL
	*	"tile" => LayoutType::TILE
	*	"absolute" => LayoutType::ABSOLUTE
	*/
	static LayoutType readLayoutType(const std::string& str);

	/**	将用逗号隔开的字符串转换为string数组
	*/
	static void readStringList(const std::string& str, std::vector<std::string>* container);

private:
	static float converStr2Float(const std::string& str);
};
TINA_NS_END