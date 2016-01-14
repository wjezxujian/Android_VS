#include "TiNode.h"
#include "widget/util/WidgetReader.h"
#include "widget/Button.h"

TINA_NS_BEGIN
TiNode* TiNode::create(const std::string& file)
{
	auto _ref = new TiNode();
	if (_ref && _ref->initWithTxml(file))
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

TiNode* TiNode::create()
{
	auto _ref = new TiNode();
	if (_ref && _ref->init())
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

TiNode::TiNode()
:m_uiReader(nullptr)
{}

TiNode::~TiNode()
{
	if (m_uiReader != nullptr)
		CC_SAFE_RELEASE(m_uiReader);
}

WidgetReader* TiNode::getUIReader()
{
	return m_uiReader;
}

bool TiNode::initWithTxml(const std::string& file)
{
	if (!cocos2d::Node::init())
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

bool TiNode::init()
{
	if (!cocos2d::Node::init())
	{
		return false;
	}
	setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	return true;
}

void TiNode::foreachWidgetChildren(Widget* parent)
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

		_buttonWidget->setTouchCallfunc(CC_CALLBACK_2(TiNode::handlerButton, this));
	}
}
TINA_NS_END