#include "TiLayer.h"
#include "widget/util/WidgetReader.h"
#include "widget/Button.h"

TINA_NS_BEGIN
TiLayer* TiLayer::create(const std::string& file)
{
	auto _ref = new TiLayer();
	if (_ref && _ref->initWithTxml(file))
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

TiLayer* TiLayer::create()
{
	auto _ref = new TiLayer();
	if (_ref && _ref->init())
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

TiLayer::TiLayer()
:m_uiReader(nullptr)
{}

TiLayer::~TiLayer()
{
	if (m_uiReader != nullptr)
		CC_SAFE_RELEASE(m_uiReader);
}

WidgetReader* TiLayer::getUIReader()
{
	return m_uiReader;
}

bool TiLayer::initWithTxml(const std::string& file)
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	m_uiReader = tina::WidgetReader::create(file);
	m_uiReader->retain();
	auto _rootWidget = m_uiReader->getRootWidget();
	{
		addChild(_rootWidget);
		if (!_rootWidget->isIgnoreAnchorPointForPosition())
		{
			_rootWidget->setPosition(_rootWidget->getAnchorPointInPoints());
		}
	}
	setContentSize(_rootWidget->getContentSize());

	if (m_uiReader != nullptr && m_uiReader->getRootWidget())
	{
		foreachWidgetChildren(m_uiReader->getRootWidget());
	}

	return true;
}

bool TiLayer::init()
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}
	setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	return true;
}

void TiLayer::foreachWidgetChildren(Widget* parent)
{
	if (parent == nullptr)
		return;

	for (auto widget : parent->getWidgetChildren())
	{
		if (!widget->getWidgetChildren().empty())
		{
			foreachWidgetChildren(widget);
			continue;
		}

		if (!widget->isAutoTouchCallfunc())continue;

		auto _buttonWidget = dynamic_cast<tina::Button*>(widget);
		if (!_buttonWidget)continue;

		_buttonWidget->setTouchCallfunc(CC_CALLBACK_2(TiLayer::handlerButton, this));
	}
}

TINA_NS_END