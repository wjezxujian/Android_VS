#pragma once

/****************************************************************************
功 能：tina定义通用类
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-02-17
最后修改者：董伟
最后修改日期：2015-02-17
****************************************************************************/

#include <cstdint>

#define TINA_NS_BEGIN	namespace tina{
#define TINA_NS_END		}

namespace tina{

#define NOT_A_NUMBER -987654321
	/**	未赋值数字*/
	static const float NaN = NOT_A_NUMBER;

#define PARSING_START m_parsing = true
#define PARSING_END m_parsing = false

#define DRAW_RENDERER \
	if (!m_parsing) \
	{ \
	drawRenderer(); \
	} \

	/**	控件类型*/
	enum class WidgetType
	{
		WIDGET,
		IMAGE,
		SCALE_IMAGE,
		REPEAT_IMAGE,
		PROGRESS_IMAGE,
		BUTTON,
		LABEL,
		INT_LABEL,
		TEXTFIELD,
		BOX,
		HBOX,
		VBOX,
		BASEBOX,
		TILEBOX,
		SHAPE_RECT,
		SHAPE_CIRCLE,
		CLIPVIEW,
		SCROLLVIEW,
		LISTVIEW,
		GRIDVIEW,

	};

	/**	布局水平对齐方式*/
	enum class HAlign
	{
		LEFT,
		CENTER,
		RIGHT
	};

	/**	布局垂直对齐方式*/
	enum class VAlign
	{
		TOP,
		CENTER,
		BOTTOM
	};

	enum class ScrollDirection
	{
		BOTH,
		HORIZONTAL,
		VERTICAL,
		AUTO
	};

	enum class LayoutType
	{
        UNDEFINED, //未定义
		//垂直布局
		VERTICAL,
		//水平布局
		HORIZONTAL,
		//格子布局
		TILE,
		//自由布局
		ABSOLUTE
	};

	typedef struct _LayoutParam
	{
		_LayoutParam()
		:hGap(0)
		, vGap(0)
		{}
		LayoutType	type;
		int	hGap;
		int	vGap;
		HAlign		hAlign;
		VAlign		vAlign;
	} LayoutParam;

	//触摸交互的类型
	enum class TouchType
	{
		CLICK = 1,//点击
		LONG_PRESS = 2,//长按
		DOWN, //按下（不限定是否在目标对象内，相当于touchBegan）
		PRESS,//按下（在目标对象内）
		RELEASE,//抬起
		MOVE, //移动
		MOVE_OUT //移出目标外
	};

	enum class LabelType {
		TTF,
		BMFONT,
		CHARMAP,
		STRING_TEXTURE,
		SYSTEM
	};
}
