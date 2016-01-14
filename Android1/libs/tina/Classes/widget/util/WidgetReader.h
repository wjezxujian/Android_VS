#pragma once

/****************************************************************************
功 能：控件读取器
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-02-27
最后修改者：董伟
最后修改日期：2015-02-27
****************************************************************************/

#include "include/tinaMacros.h"
#include "widget/base/Widget.h"
#include "base/CCRef.h"
#include "rapidxml.h"

TINA_NS_BEGIN
class Image;
class RepeatImage;
class ProgressImage;
class IntLabel;
class Label;
class TextField;
class ScaleImage;
class Button;
class ImageButton;
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
class CustomWidgetCreator
{
public:
	virtual Widget* createWidget(const std::string& tagName) = 0;
};

class WidgetReader : public cocos2d::Ref
{
public:
	static WidgetReader* create();
	static WidgetReader* create(const std::string& file);
	bool loadTXML(const std::string& file, bool fromCache = true);
	Widget* getRootWidget();
	Widget* findWidgetByID(const std::string& id);
	Widget* findWidgetByID(const std::string& id, Widget* parent);

public:
	Image* findImageByID(const std::string& id);
	RepeatImage* findRepeatImageByID(const std::string& id);
	ProgressImage* findProgressImageByID(const std::string& id);
	IntLabel* findIntLabelByID(const std::string& id);
	Label* findLabelByID(const std::string& id);
	TextField* findTextFieldByID(const std::string& id);
	ScaleImage* findScaleImageByID(const std::string& id);
	Button* findButtonByID(const std::string& id);
	ImageButton* findImageButtonByID(const std::string& id);
	Box* findBoxByID(const std::string& id);
	HBox* findHBoxByID(const std::string& id);
	VBox* findVBoxByID(const std::string& id);
	TileBox* findTileBoxByID(const std::string& id);
	ScrollView* findScrollViewByID(const std::string& id);
	ListView* findListViewByID(const std::string& id);
	GridView* findGridViewByID(const std::string& id);
	ClipView* findClipViewByID(const std::string& id);
	ShapeRect* findShapeRectByID(const std::string& id);
	ShapeCircle* findShapeCircleByID(const std::string& id);
	ProgressTimer* findProgressTimerByID(const std::string& id);
	template<class T>
	T* findCustomWidgetByID(const std::string& id)
	{
		auto _widget = findWidgetByID(id);
		if (!_widget)
			return nullptr;
		return dynamic_cast<T*>(_widget);
	};


private:
	bool init(const std::string& file);
	Widget* readChildNode(rapidxml::xml_node<>* node, Widget* parent);
	void readAttrNode(rapidxml::xml_node<>* node, std::vector<Widget*>* container);
	Widget* createWidget(const std::string& tagName);

private:
	std::string		m_txmlFile;
	Widget*			m_rootWidget = nullptr;
	bool			m_useAbsPath = false;
};
TINA_NS_END