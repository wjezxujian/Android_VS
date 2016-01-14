#include "ClipView.h"
#include "util/ParamUtil.h"

USING_NS_CC;

TINA_NS_BEGIN
ClipView* ClipView::create()
{
	auto _ref = new ClipView();
	if (_ref && _ref->init())
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

void ClipView::parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values)
{
	Widget::parseAttr(keys, values);
	for (auto i = 0; i < keys->size(); ++i)
	{
		const auto& _key = keys->at(i);
		const auto& _value = values->at(i);
		if (_key == "alphathreshold")
		{
			setAlphaThreshold(ParamUtil::readFloat(_value));
		}
		else if (_key == "inverted")
		{
			setInverted(ParamUtil::readBool(_value));
		}
	}
}

void ClipView::parseAttr(const std::string& key, const std::vector<Widget*>& nodeParams)
{
	if (key == "stencil")
	{
		setStencil(nodeParams.front());
	}
}

void ClipView::setStencil(Widget* stencilNode)
{
	Widget* _oldStencil = dynamic_cast<Widget*>(m_renderer->getStencil());
	stencilNode->ignoreAnchorPointForPosition(true);
	stencilNode->setPosition(Vec2::ZERO);
	m_renderer->setStencil(stencilNode);
	DRAW_RENDERER;
}

void ClipView::setAlphaThreshold(float alpha)
{
	m_renderer->setAlphaThreshold(alpha);
}

void ClipView::setInverted(bool inverted)
{
	m_renderer->setInverted(inverted);
}

void ClipView::addWidget(Widget* child)
{
	m_renderer->addChild(child);
	m_widgetChildren.pushBack(child);
}

void ClipView::addWidget(Widget* child, const std::string &id)
{
	m_renderer->addChild(child);
	child->setId(id);
	m_widgetChildren.pushBack(child);
}

void ClipView::addWidget(Widget* child, int localZOrder)
{
	m_renderer->addChild(child, localZOrder);
	m_widgetChildren.pushBack(child);
}

void ClipView::addWidget(Widget* child, int localZOrder, const std::string &id)
{
	m_renderer->addChild(child, localZOrder);
	child->setId(id);
	m_widgetChildren.pushBack(child);
}

void ClipView::removeWidget(Widget* child, bool cleanup)
{
	m_renderer->removeChild(child, cleanup);
	m_widgetChildren.eraseObject(child, true);
}

void ClipView::removeAllWidget()
{
	m_renderer->removeAllChildren();
	m_widgetChildren.clear();
}

bool ClipView::init()
{
	if (!Widget::init())
	{
		return false;
	}

	setType(WidgetType::CLIPVIEW);
	m_renderer = ClippingNode::create();
	{
        m_renderer->setCascadeColorEnabled(true);
        m_renderer->setCascadeOpacityEnabled(true);
		m_renderer->ignoreAnchorPointForPosition(true);
		//m_renderer->setAlphaThreshold(0.5);
		Node::addChild(m_renderer);
	}
	return true;
}

TINA_NS_END