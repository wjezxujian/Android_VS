#pragma once

/****************************************************************************
功 能：控件帮助类
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-04-20
最后修改者：董伟
最后修改日期：2015-04-20
****************************************************************************/

#include "include/tinaMacros.h"
#include "widget/base/Widget.h"

TINA_NS_BEGIN
class Image;
class RepeatImage;
class ScaleImage;
class ProgressImage;
class IntLabel;
class Label;
class TextField;
class Button;
class Box;
class HBox;
class VBox;
class TileBox;
class ScrollView;
class ListView;
class GridView;
class ClipView;
class ShapeRect;
class ShapeCircle;
class ProgressTimer;

class WidgetHelper
{
public:
	static Widget* findWidgetByID(Widget* parent, const std::string& id);
	static Image* findImageByID(Widget* parent, const std::string& id);
	static RepeatImage* findRepeatImageByID(Widget* parent, const std::string& id);
	static ScaleImage* findScaleImageByID(Widget* parent, const std::string& id);
	static ProgressImage* findProgressImageByID(Widget* parent, const std::string& id);
	static IntLabel* findIntLabelByID(Widget* parent, const std::string& id);
	static Label* findLabelByID(Widget* parent, const std::string& id);
	static TextField* findTextFieldByID(Widget* parent, const std::string& id);
	static Button* findButtonByID(Widget* parent, const std::string& id);
	static Box* findBoxByID(Widget* parent, const std::string& id);
	static HBox* findHBoxByID(Widget* parent, const std::string& id);
	static VBox* findVBoxByID(Widget* parent, const std::string& id);
	static TileBox* findTileBoxByID(Widget* parent, const std::string& id);
	static ScrollView* findScrollViewByID(Widget* parent, const std::string& id);
	static ListView* findListViewByID(Widget* parent, const std::string& id);
	static GridView* findGridViewByID(Widget* parent, const std::string& id);
	static ClipView* findClipViewByID(Widget* parent, const std::string& id);
	static ShapeRect* findShapeRectByID(Widget* parent, const std::string& id);
	static ShapeCircle* findShapeCircleByID(Widget* parent, const std::string& id);
	static ProgressTimer* findProgressTimerByID(Widget* parent, const std::string& id);
	template<class T>
	static T* findCustomWidgetByID(Widget* parent, const std::string& id)
	{
		auto _widget = findWidgetByID(parent, id);
		if (!_widget)
			return nullptr;
		return dynamic_cast<T*>(_widget);
	};
};
TINA_NS_END