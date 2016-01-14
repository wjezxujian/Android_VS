#include "ShapeCircle.h"
#include "util/ParamUtil.h"

USING_NS_CC;

TINA_NS_BEGIN

ShapeCircle* ShapeCircle::create()
{
	auto _ref = new ShapeCircle();
	if (_ref && _ref->init())
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

void ShapeCircle::parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values)
{
	Widget::parseAttr(keys, values);
	for (auto i = 0; i < keys->size(); ++i)
	{
		const auto& _key = keys->at(i);
		const auto& _value = values->at(i);
		if (_key == "bgcolor")
		{
			setBgColor(ParamUtil::readColor4B(_value));
		}
	}
}

void ShapeCircle::setBgColor(const cocos2d::Color4B& color)
{
	if (m_bgColor == color)return;
	m_bgColor = color;
	DRAW_RENDERER;
}

void ShapeCircle::setBgColor(const std::string& colorStr)
{
	setBgColor(ParamUtil::readColor4B(colorStr));
}

bool ShapeCircle::init()
{
	if (!Widget::init())
	{
		return false;
	}
	setType(WidgetType::SHAPE_CIRCLE);
	m_renderer = DrawNode::create();
	{
		m_renderer->ignoreAnchorPointForPosition(true);
		Node::addChild(m_renderer);
	}
	return true;
}

void ShapeCircle::drawRenderer(bool updateSize)
{
	if (m_parsing)return;
	float _radius = MIN(_contentSize.width, _contentSize.height) * 0.5f;
	m_renderer->clear();
	m_renderer->drawDot(Vec2(_radius, _radius), _radius, Color4F(m_bgColor));
	m_renderer->setScaleX(_contentSize.width / MIN(_contentSize.width, _contentSize.height));
	m_renderer->setScaleY(_contentSize.height / MIN(_contentSize.width, _contentSize.height));
}
TINA_NS_END