#include "WidgetReader.h"
#include "cocos2d.h"
#include "TXMLMgr.h"
#include "widget/Box.h"
#include "widget/HBox.h"
#include "widget/VBox.h"
#include "widget/TileBox.h"
#include "widget/Image.h"
#include "widget/ScaleImage.h"
#include "widget/ProgressImage.h"
#include "widget/Label.h"
#include "widget/IntLabel.h"
#include "widget/TextField.h"
#include "widget/Button.h"
#include "widget/ImageButton.h"
#include "widget/ShapeRect.h"
#include "widget/ShapeCircle.h"
#include "widget/ClipView.h"
#include "widget/ScrollView.h"
#include "widget/ListView.h"
#include "widget/GridView.h"
#include "widget/RepeatImage.h"
#include "widget/ProgressTimer.h"
#include "util/StringUtil.h"
#include "util/TinaMgr.h"
#include "ParamUtil.h"
#include "WidgetHelper.h"

USING_NS_CC;

TINA_NS_BEGIN

WidgetReader* WidgetReader::create()
{
	return create("");
}

WidgetReader* WidgetReader::create(const std::string& file)
{
	auto _ref = new WidgetReader();
	if (_ref && _ref->init(file))
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

bool WidgetReader::loadTXML(const std::string& file, bool fromCache)
{
	//cocos2d::log("WidgetReader::loadTXML %s", file.c_str());
	if (m_useAbsPath)
		m_txmlFile = file;
	else
		m_txmlFile = TINA_MGR->getTxmlFullPath(file);
	bool _fromCache = fromCache;

#if defined(COCOS2D_DEBUG) || COCOS2D_DEBUG == 1
	//_fromCache = false;
#endif
	
	auto _data = TXMLMgr::getInstance()->loadTXML(m_txmlFile, _fromCache);
	do
	{
		CC_BREAK_IF(!_data);
		auto _start = clock();
		m_rootWidget = readChildNode(_data->getXml()->first_node(), nullptr);
		//CCLOG("parse widget file: %s cost: %d", file.c_str(), clock() - _start);
		CC_BREAK_IF(!m_rootWidget);
		return true;
	} while (0);
	return false;
}

Widget* WidgetReader::getRootWidget()
{
	return m_rootWidget;
}

Widget* WidgetReader::findWidgetByID(const std::string& id)
{
	if (!m_rootWidget)return nullptr;
	return WidgetHelper::findWidgetByID(m_rootWidget, id);
}

Widget* WidgetReader::findWidgetByID(const std::string& id, Widget* parent)
{
	if (!parent)return nullptr;
	return WidgetHelper::findWidgetByID(parent, id);
}

Image* WidgetReader::findImageByID(const std::string& id)
{ 
	return dynamic_cast<Image*>(findWidgetByID(id)); 
}

RepeatImage* WidgetReader::findRepeatImageByID(const std::string& id)
{
	return dynamic_cast<RepeatImage*>(findWidgetByID(id));
}

ProgressImage* WidgetReader::findProgressImageByID(const std::string& id)
{
	return dynamic_cast<ProgressImage*>(findWidgetByID(id));
}

IntLabel* WidgetReader::findIntLabelByID(const std::string& id)
{
	return dynamic_cast<IntLabel*>(findWidgetByID(id));
}

Label* WidgetReader::findLabelByID(const std::string& id)
{ 
	return dynamic_cast<Label*>(findWidgetByID(id)); 
}

TextField* WidgetReader::findTextFieldByID(const std::string& id)
{ 
	return dynamic_cast<TextField*>(findWidgetByID(id)); 
}
ScaleImage* WidgetReader::findScaleImageByID(const std::string& id)
{ 
	return dynamic_cast<ScaleImage*>(findWidgetByID(id)); 
}

Button* WidgetReader::findButtonByID(const std::string& id)
{ 
	return dynamic_cast<Button*>(findWidgetByID(id)); 
}

ImageButton* WidgetReader::findImageButtonByID(const std::string& id)
{
	return dynamic_cast<ImageButton*>(findWidgetByID(id));
}

Box* WidgetReader::findBoxByID(const std::string& id)
{ 
	return dynamic_cast<Box*>(findWidgetByID(id)); 
}

HBox* WidgetReader::findHBoxByID(const std::string& id)
{ 
	return dynamic_cast<HBox*>(findWidgetByID(id)); 
}

VBox* WidgetReader::findVBoxByID(const std::string& id)
{ 
	return dynamic_cast<VBox*>(findWidgetByID(id)); 
}

TileBox* WidgetReader::findTileBoxByID(const std::string& id)
{
	return dynamic_cast<TileBox*>(findWidgetByID(id));
}

ScrollView* WidgetReader::findScrollViewByID(const std::string& id)
{ 
	return dynamic_cast<ScrollView*>(findWidgetByID(id)); 
}

ListView* WidgetReader::findListViewByID(const std::string& id)
{ 
	return dynamic_cast<ListView*>(findWidgetByID(id)); 
}

GridView* WidgetReader::findGridViewByID(const std::string& id)
{ 
	return dynamic_cast<GridView*>(findWidgetByID(id)); 
}

ClipView* WidgetReader::findClipViewByID(const std::string& id)
{
	return dynamic_cast<ClipView*>(findWidgetByID(id));
}

ShapeRect* WidgetReader::findShapeRectByID(const std::string& id)
{ 
	return dynamic_cast<ShapeRect*>(findWidgetByID(id)); 
}

ShapeCircle* WidgetReader::findShapeCircleByID(const std::string& id)
{ 
	return dynamic_cast<ShapeCircle*>(findWidgetByID(id)); 
}

ProgressTimer* WidgetReader::findProgressTimerByID(const std::string& id)
{
	return dynamic_cast<ProgressTimer*>(findWidgetByID(id));
}



bool WidgetReader::init(const std::string& file)
{
	m_rootWidget = nullptr;
	if (!file.empty())
	{
		loadTXML(file);
	}
	return true;
}

Widget* WidgetReader::readChildNode(rapidxml::xml_node<>* node, Widget* parent)
{
	std::string _tagName = node->name();
	StringUtil::toLower(&_tagName);
	StringUtil::eraseWhiteSpaces(&_tagName);
	Widget* _widget = createWidget(_tagName);
	//cocos2d::log("WidgetReader::readChildNode %s", _tagName.c_str());
	if (!_widget && _tagName != "t:include")return nullptr;

	std::vector<std::string> _keys;
	std::vector<std::string> _values;
	auto _attr = node->first_attribute();
	while (_attr)
	{
		std::string _key = _attr->name();
		StringUtil::toLower(&_key);
		StringUtil::eraseWhiteSpaces(&_key);

		std::string _value = _attr->value();

		if (!_key.empty() && !_value.empty())
		{
			_keys.push_back(_key);
			_values.push_back(_value);
		}
		_attr = _attr->next_attribute();
	}
	if (_tagName == "t:include")
	{
		auto _fileIt = std::find(_keys.begin(), _keys.end(), "file");
		if (_fileIt == _keys.end())
		{
			return nullptr;
		}
		std::string _includeSrc = *(_values.begin() + (_fileIt - _keys.begin()));

		bool _useCache = true;
		auto _cacheIt = std::find(_keys.begin(), _keys.end(), "cache");
		if (_cacheIt != _keys.end())
		{
			std::string _cacheStr = *(_values.begin() + (_cacheIt - _keys.begin()));
			_useCache = ParamUtil::readBool(_cacheStr);
		}

		auto _reader = WidgetReader::create();
		_reader->loadTXML(_includeSrc, _useCache);
		auto _rootWidget = _reader->getRootWidget();
		if (_rootWidget)
			_rootWidget->parse(&_keys, &_values);
		return _rootWidget;
	}
	_widget->parse(&_keys, &_values);
	
	auto _xmlChild = node->first_node();
	while (_xmlChild)
	{
		std::string _childTagName = _xmlChild->name();
		StringUtil::toLower(&_childTagName);
		StringUtil::eraseWhiteSpaces(&_childTagName);

		//属性节点
		if (_childTagName.substr(0, 2) == "a:")
		{
			std::vector<Widget*> _container;
			auto _attrChild = _xmlChild->first_node();
			while (_attrChild)
			{
				readAttrNode(_attrChild, &_container);
				_attrChild = _attrChild->next_sibling();
			}
			_widget->parse(_childTagName.substr(2, _childTagName.size() - 2), _container);
		}
		//子节点
		else
		{
			auto _widgetChild = readChildNode(_xmlChild, _widget);
			if (_widgetChild)
				_widget->addWidget(_widgetChild);
		}
		_xmlChild = _xmlChild->next_sibling();
	}

	return _widget;
}

void WidgetReader::readAttrNode(rapidxml::xml_node<>* node, std::vector<Widget*>* container)
{
	std::string _tagName = node->name();
	StringUtil::toLower(&_tagName);
	StringUtil::eraseWhiteSpaces(&_tagName);
	Widget* _widget = createWidget(_tagName);
	if (!_widget)return;

	std::vector<std::string> _keys;
	std::vector<std::string> _values;
	auto _attr = node->first_attribute();
	while (_attr)
	{
		std::string _key = _attr->name();
		StringUtil::toLower(&_key);
		StringUtil::eraseWhiteSpaces(&_key);

		std::string _value = _attr->value();

		if (!_key.empty() && !_value.empty())
		{
			_keys.push_back(_key);
			_values.push_back(_value);
		}
		_attr = _attr->next_attribute();
	}
	_widget->parse(&_keys, &_values);

	auto _xmlChild = node->first_node();
	while (_xmlChild)
	{
		auto _widgetChild = readChildNode(_xmlChild, _widget);
		if (_widgetChild)
			_widget->addWidget(_widgetChild);

		_xmlChild = _xmlChild->next_sibling();
	}
	container->push_back(_widget);
}

Widget* WidgetReader::createWidget(const std::string& tagName)
{
	Widget* _widget = nullptr;
	if (tagName == "t:box")
	{
		_widget = Box::create();
	}
	else if (tagName == "t:hbox")
	{
		_widget = HBox::create();
	}
	else if (tagName == "t:vbox")
	{
		_widget = VBox::create();
	}
	else if (tagName == "t:tilebox")
	{
		_widget = TileBox::create();
	}
	else if (tagName == "t:image")
	{
		_widget = Image::create();
	}
	else if (tagName == "t:scaleimage")
	{
		_widget = ScaleImage::create();
	}
	else if (tagName == "t:repeatimage")
	{
		_widget = RepeatImage::create();
	}
	else if (tagName == "t:progressimage")
	{
		_widget = ProgressImage::create();
	}
	else if (tagName == "t:label")
	{
		_widget = Label::create();
	}
	else if (tagName == "t:intlabel")
	{
		_widget = IntLabel::create();
	}
	else if (tagName == "t:textfield")
	{
		_widget = TextField::create();
	}
	else if (tagName == "t:button")
	{
		_widget = Button::create();
	}
	else if (tagName == "t:imagebutton")
	{
		_widget = ImageButton::create();
	}
	else if (tagName == "t:shaperect")
	{
		_widget = ShapeRect::create();
	}
	else if (tagName == "t:shapecircle")
	{
		_widget = ShapeCircle::create();
	}
	else if (tagName == "t:shapecircle")
	{
		_widget = ShapeCircle::create();
	}
	else if (tagName == "t:clipview")
	{
		_widget = ClipView::create();
	}
	else if (tagName == "t:scrollview")
	{
		_widget = ScrollView::create();
	}
	else if (tagName == "t:listview")
	{
		_widget = ListView::create();
	}
	else if (tagName == "t:gridview")
	{
		_widget = GridView::create();
	}
	else if (tagName == "t:progresstimer")
	{
		_widget = ProgressTimer::create();
	}
	else
	{
		auto _customWidgetCreator = TINA_MGR->getConfig()->getCustomWidgetCreator();
		if (_customWidgetCreator != nullptr)
			_widget = _customWidgetCreator->createWidget(tagName);
	}
	if (_widget)
		_widget->ignoreAnchorPointForPosition(true);
	return _widget;
}
TINA_NS_END