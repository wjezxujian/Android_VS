#include "WidgetHelper.h"
#include "widget/Box.h"
#include "widget/HBox.h"
#include "widget/VBox.h"
#include "widget/TileBox.h"
#include "widget/Image.h"
#include "widget/ScaleImage.h"
#include "widget/RepeatImage.h"
#include "widget/ProgressImage.h"
#include "widget/Label.h"
#include "widget/IntLabel.h"
#include "widget/TextField.h"
#include "widget/Button.h"
#include "widget/ShapeRect.h"
#include "widget/ShapeCircle.h"
#include "widget/ClipView.h"
#include "widget/ScrollView.h"
#include "widget/ListView.h"
#include "widget/GridView.h"
#include "widget/ProgressTimer.h"

TINA_NS_BEGIN
Widget* WidgetHelper::findWidgetByID(Widget* parent, const std::string& id)
{
	if (parent->getId() == id)return parent;
	if (!parent->getWidgetChildren().empty())
	{
		for (auto child : parent->getWidgetChildren())
		{
			Widget* _widget = findWidgetByID(child, id);
			if (_widget)return _widget;
		}
	}
	return nullptr;
}

Image* WidgetHelper::findImageByID(Widget* parent, const std::string& id)
{
	return dynamic_cast<Image*>(findWidgetByID(parent, id));
}

RepeatImage* WidgetHelper::findRepeatImageByID(Widget* parent, const std::string& id)
{
	return dynamic_cast<RepeatImage*>(findWidgetByID(parent, id));
}

ScaleImage* WidgetHelper::findScaleImageByID(Widget* parent, const std::string& id)
{
	return dynamic_cast<ScaleImage*>(findWidgetByID(parent, id));
}

ProgressImage* WidgetHelper::findProgressImageByID(Widget* parent, const std::string& id)
{
	return dynamic_cast<ProgressImage*>(findWidgetByID(parent, id));
}

IntLabel* WidgetHelper::findIntLabelByID(Widget* parent, const std::string& id)
{
	return dynamic_cast<IntLabel*>(findWidgetByID(parent, id));
}

Label* WidgetHelper::findLabelByID(Widget* parent, const std::string& id)
{
	return dynamic_cast<Label*>(findWidgetByID(parent, id));
}

TextField* WidgetHelper::findTextFieldByID(Widget* parent, const std::string& id)
{
	return dynamic_cast<TextField*>(findWidgetByID(parent, id));
}

Button* WidgetHelper::findButtonByID(Widget* parent, const std::string& id)
{
	return dynamic_cast<Button*>(findWidgetByID(parent, id));
}

Box* WidgetHelper::findBoxByID(Widget* parent, const std::string& id)
{
	return dynamic_cast<Box*>(findWidgetByID(parent, id));
}

HBox* WidgetHelper::findHBoxByID(Widget* parent, const std::string& id)
{
	return dynamic_cast<HBox*>(findWidgetByID(parent, id));
}

VBox* WidgetHelper::findVBoxByID(Widget* parent, const std::string& id)
{
	return dynamic_cast<VBox*>(findWidgetByID(parent, id));
}

TileBox* WidgetHelper::findTileBoxByID(Widget* parent, const std::string& id)
{
	return dynamic_cast<TileBox*>(findWidgetByID(parent, id));
}

ScrollView* WidgetHelper::findScrollViewByID(Widget* parent, const std::string& id)
{
	return dynamic_cast<ScrollView*>(findWidgetByID(parent, id));
}

ListView* WidgetHelper::findListViewByID(Widget* parent, const std::string& id)
{
	return dynamic_cast<ListView*>(findWidgetByID(parent, id));
}

GridView* WidgetHelper::findGridViewByID(Widget* parent, const std::string& id)
{
	return dynamic_cast<GridView*>(findWidgetByID(parent, id));
}

ClipView* WidgetHelper::findClipViewByID(Widget* parent, const std::string& id)
{
	return dynamic_cast<ClipView*>(findWidgetByID(parent, id));
}

ShapeRect* WidgetHelper::findShapeRectByID(Widget* parent, const std::string& id)
{
	return dynamic_cast<ShapeRect*>(findWidgetByID(parent, id));
}

ShapeCircle* WidgetHelper::findShapeCircleByID(Widget* parent, const std::string& id)
{
	return dynamic_cast<ShapeCircle*>(findWidgetByID(parent, id));
}

ProgressTimer* WidgetHelper::findProgressTimerByID(Widget* parent, const std::string& id)
{
	return dynamic_cast<ProgressTimer*>(findWidgetByID(parent, id));
}


TINA_NS_END