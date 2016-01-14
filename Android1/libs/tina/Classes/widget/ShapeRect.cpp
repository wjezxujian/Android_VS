#include "ShapeRect.h"
#include "util/ParamUtil.h"

USING_NS_CC;

TINA_NS_BEGIN

ShapeRect* ShapeRect::create()
{
	auto _ref = new ShapeRect();
	if (_ref && _ref->init())
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

void ShapeRect::parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values)
{
	Widget::parseAttr(keys, values);
	for (auto i = 0; i < keys->size(); ++i)
	{
		const auto& _key = keys->at(i);
		const auto& _value = values->at(i);
		if (_key == "borderwidth" || _key == "border")
		{
			setBorderWidth(ParamUtil::readFloat(_value));
		}
		else if (_key == "bordercolor")
		{
			setBorderColor(ParamUtil::readColor4B(_value));
		}
		else if (_key == "bgcolor")
		{
			setBgColor(ParamUtil::readColor4B(_value));
		}
		else if (_key == "opacity")
		{
			setOpacity(ParamUtil::readInt(_value));
		}
	}
}

void ShapeRect::setBorderWidth(float width)
{
	m_borderWidth = width;
	DRAW_RENDERER;
}

void ShapeRect::setBorderColor(const cocos2d::Color4B& color)
{
	if (m_borderColor == color)return;
	m_borderColor = color;
	DRAW_RENDERER;
}

void ShapeRect::setBorderColor(const std::string& colorStr)
{
	setBorderColor(ParamUtil::readColor4B(colorStr));
}

void ShapeRect::setBgColor(const cocos2d::Color4B& color)
{
	if (m_bgColor == color)return;
	m_bgColor = color;
	DRAW_RENDERER;
}

void ShapeRect::setBgColor(const std::string& colorStr)
{
	setBgColor(ParamUtil::readColor4B(colorStr));
}

void ShapeRect::setOpacity(GLubyte opacity)
{
	if (m_isOpacity && m_opacity == opacity) return;
	m_opacity = opacity;
	m_isOpacity = true;
	DRAW_RENDERER;
}

bool ShapeRect::init()
{
	if (!Widget::init())
	{
		return false;
	}
	setType(WidgetType::SHAPE_RECT);
	m_renderer = DrawNode::create();
	{
		m_renderer->ignoreAnchorPointForPosition(true);
		Node::addChild(m_renderer);
	}
	return true;
}

void ShapeRect::drawRenderer(bool updateSize)
{
	if (m_parsing)return;

	if (m_isOpacity) m_bgColor.a = m_opacity;

	Vec2 _points[4];
	_points[0] = Vec2::ZERO;
	_points[1] = { _contentSize.width, 0 };
	_points[2] = { _contentSize.width, _contentSize.height };
	_points[3] = { 0, _contentSize.height };
	m_renderer->clear();
	m_renderer->drawPolygon(_points, 4, Color4F(m_bgColor), m_borderWidth, Color4F(m_borderColor));
}
TINA_NS_END