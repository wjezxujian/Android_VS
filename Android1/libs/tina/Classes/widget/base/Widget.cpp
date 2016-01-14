#include "Widget.h"
#include "widget/util/ParamUtil.h"
#include "widget/ScrollView.h"

USING_NS_CC;

TINA_NS_BEGIN
void Widget::setLeft(float left)
{
	if (m_left == left)return;
	if (isParentLinearLayout())return;
	m_left = left;
	m_right = NaN;
	m_percentX = NaN;
	Node::setPosition(left2PositionX(m_left), _position.y);
	if (!m_parsing && m_widgetParent)
	{
		callParentRelayout();
	}
}

void Widget::setRight(float right)
{
	if (m_right == right)return;
	if (isParentLinearLayout())return;
	m_right = right;
	m_left = NaN;
	m_percentX = NaN;
	if (!m_parsing && m_widgetParent)
	{
		Node::setPosition(right2PositionX(m_right), _position.y);
	} 
}

void Widget::setTop(float top)
{
	if (m_top == top)return;
	if (isParentLinearLayout())return;
	m_top = top;
	m_bottom = NaN;
	m_percentY = NaN;
	if (!m_parsing && m_widgetParent)
	{
		Node::setPosition(_position.x, top2PositionY(m_top));
	}
}

void Widget::setBottom(float bottom)
{
	if (m_bottom == bottom)return;
	if (isParentLinearLayout())return;
	m_bottom = bottom;
	m_top = NaN;
	m_percentY = NaN;
	Node::setPosition(_position.x, bottom2PositionY(m_bottom));
	if (!m_parsing && m_widgetParent)
	{
		callParentRelayout();
	}
}

void Widget::setPosition(const cocos2d::Vec2 &position)
{
	setPosition(position.x, position.y);
}

void Widget::setPosition(float x, float y)
{
	if (isParentLinearLayout())return;
	m_x = x;
	m_y = y;
	m_percentX = NaN;
	m_percentY = NaN;
	m_right = NaN;
	m_top = NaN;
	m_left = NaN;
	m_bottom = NaN;
	Node::setPosition(x, y);
	if (!m_parsing && m_widgetParent)
	{
		callParentRelayout();
	}
}

void Widget::setPositionX(float x)
{
	setX(x);
}

void Widget::setX(float x)
{
	if (x == m_x)return;
	if (isParentLinearLayout())return;
	m_x = x;
	m_percentX = NaN;
	m_left = NaN;
	m_right = NaN;
	Node::setPosition(m_x, _position.y);
	if (!m_parsing && m_widgetParent)
	{
		callParentRelayout();
	}
}

void Widget::setPercentX(float percentX)
{
	if (m_percentX == percentX)return;
	if (isParentLinearLayout())return;
	m_percentX = percentX;
	m_x = NaN;
	m_left = NaN;
	m_right = NaN;
	if (!m_parsing && m_widgetParent)
	{
		Node::setPosition(m_widgetParent->getWidth() * MIN(1.f, m_percentX /100.f), _position.y);
	}
}

void Widget::setPositionY(float y)
{
	setY(y);
}

void Widget::setY(float y)
{
	if (y == m_y)return;
	if (isParentLinearLayout())return;
	m_y = y;
	Node::setPosition(_position.x, m_y);
	m_percentY = NaN;
	m_bottom = NaN;
	m_top = NaN;
	if (!m_parsing && m_widgetParent)
	{
		callParentRelayout();
	}
}

void Widget::setPercentY(float percentY)
{
	if (m_percentY == percentY)return;
	if (isParentLinearLayout())return;
	m_percentY = percentY;
	m_y = NaN;
	m_top = NaN;
	m_bottom = NaN;
	if (!m_parsing && m_widgetParent)
	{
		Node::setPosition(m_widgetParent->getHeight() * MIN(1.f, m_percentY / 100.f), _position.y);
	}
}

void Widget::setPercentPosition(float percentX, float percentY)
{
	setPercentX(percentX);
	setPercentY(percentY);
}

void Widget::setZ(int z)
{
	Node::setLocalZOrder(z);
}

void Widget::setWidth(float width)
{
	if (_contentSize.width == width)return;
	m_givenSize.width = width;
	m_percentWidth = NaN;
	updateSize({ width, _contentSize.height });
}

void Widget::ignoreWidth()
{
	m_percentWidth = NaN;
	_contentSize.width = 0;
	m_givenSize.width = -1;
	if (!m_parsing)
		drawRenderer(false);
}

void Widget::setPercentWidth(float percentWidth)
{
	if (m_percentWidth == percentWidth)return;
	m_percentWidth = percentWidth;
	m_givenSize.width = 0;
	if (!m_parsing && m_widgetParent)
	{
		callParentRelayout();
	}
}

void Widget::setHeight(float height)
{
	if (_contentSize.height == height)return;
	m_givenSize.height = height;
	m_percentHeight = NaN;
	updateSize({ _contentSize.width, height });
}

void Widget::ignoreHeight()
{
	m_percentHeight = NaN;
	_contentSize.height = 0;
	m_givenSize.height = -1;
	if (!m_parsing)
		drawRenderer(false);
}

void Widget::setPercentHeight(float percentHeight)
{
	if (m_percentHeight == percentHeight)return;
	m_percentHeight = percentHeight;
	m_givenSize.height = 0;
	if (!m_parsing && m_widgetParent)
	{
		callParentRelayout();
	}
}

void Widget::setAnchorPoint(const Vec2& anchorPoint)
{
	Node::setAnchorPoint(anchorPoint);
	if (!m_parsing && m_widgetParent)
	{
		updateRelativeLayout();
		callParentRelayout();
	}
}

void Widget::ignoreAnchorPointForPosition(bool ignore)
{
	Node::ignoreAnchorPointForPosition(ignore);
	if (!m_parsing && m_widgetParent)
	{
		updateRelativeLayout();
		callParentRelayout();
	}
}

void Widget::setContentSize(const Size& contentSize)
{
	m_givenSize = contentSize;
	m_percentWidth = NaN;
	m_percentHeight = NaN;
	updateSize(contentSize);
}

void Widget::setVisible(bool enabled)
{
	if (isVisible() == enabled)return;
	Node::setVisible(enabled);
	if (!m_parsing && m_widgetParent)
	{
		callParentRelayout();
	}
}

void Widget::setMargin(uint16_t top, uint16_t right, uint16_t bottom, uint16_t left)
{
	m_margin = { top, right, bottom, left };
	if (!m_parsing && m_widgetParent && isParentLinearLayout())
	{
		callParentRelayout();
	}
}

void Widget::setMarginTop(uint16_t margin)
{
	m_margin[0] = margin;
	if (!m_parsing && m_widgetParent && isParentLinearLayout())
	{
		callParentRelayout();
	}
}

void Widget::setMarginRight(uint16_t margin)
{
	m_margin[1] = margin;
	if (!m_parsing && m_widgetParent && isParentLinearLayout())
	{
		callParentRelayout();
	}
}

void Widget::setMarginBottom(uint16_t margin)
{
	m_margin[2] = margin;
	if (!m_parsing && m_widgetParent && isParentLinearLayout())
	{
		callParentRelayout();
	}
}

void Widget::setMarginLeft(uint16_t margin)
{
	m_margin[3] = margin;
	if (!m_parsing && m_widgetParent && isParentLinearLayout())
	{
		callParentRelayout();
	}
}

void Widget::setEnabled(bool enabled)
{
	if (m_enabled == enabled)return;
	m_enabled = enabled;
	if (m_enabled)
		onBlur();
	else
		onDisabled();
}

void Widget::setDarkMode(bool enabled)
{
	if (m_darkMode == enabled)return;
	m_darkMode = enabled;
	if (m_enabled)
		onBlur();
	else
		onDisabled();
}

void Widget::setBlur()
{
	if (!m_focus)return;
	m_focus = false;
	onBlur();
}

void Widget::setFocus()
{
	if (m_focus)return;
	m_focus = true;
	onFocus();
}

void Widget::setTouchEnabled(bool enabled)
{
	if (enabled == m_touchEnabled)
	{
		return;
	}
	m_touchEnabled = enabled;
	if (m_touchEnabled)
	{
		m_touchListener = EventListenerTouchOneByOne::create();
		CC_SAFE_RETAIN(m_touchListener);
		m_touchListener->setSwallowTouches(false);
		m_touchListener->onTouchBegan = CC_CALLBACK_2(Widget::onTouchBegan, this);
		m_touchListener->onTouchMoved = CC_CALLBACK_2(Widget::onTouchMoved, this);
		m_touchListener->onTouchEnded = CC_CALLBACK_2(Widget::onTouchEnded, this);
		m_touchListener->onTouchCancelled = CC_CALLBACK_2(Widget::onTouchCancelled, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(m_touchListener, this);
	}
	else
	{
		_eventDispatcher->removeEventListener(m_touchListener);
		CC_SAFE_RELEASE_NULL(m_touchListener);
	}
}

void Widget::setSwallowTouch(bool enabled)
{
	if (m_touchListener)
		m_touchListener->setSwallowTouches(enabled);
}

//======================================

void Widget::addWidget(Widget* child)
{
	Node::addChild(child);
	m_widgetChildren.pushBack(child);
	child->setWidgetParent(this);
	DRAW_RENDERER;
}

void Widget::addWidget(Widget* child, const std::string &id)
{
	Node::addChild(child);
	child->setId(id);
	m_widgetChildren.pushBack(child);
	child->setWidgetParent(this);
	DRAW_RENDERER;
}

void Widget::addWidget(Widget* child, int localZOrder)
{
	Node::addChild(child, localZOrder);
	m_widgetChildren.pushBack(child);
	child->setWidgetParent(this);
	DRAW_RENDERER;
}

void Widget::addWidget(Widget* child, int localZOrder, const std::string &id)
{
	Node::addChild(child, localZOrder);
	child->setId(id);
	m_widgetChildren.pushBack(child);
	child->setWidgetParent(this);
	DRAW_RENDERER;
}

void Widget::addWidgetAt(Widget* child, uint16_t index)
{
	Node::addChild(child);
	m_widgetChildren.insert(index, child);
	child->setWidgetParent(this);
	DRAW_RENDERER;
}

void Widget::removeWidget(Widget* child, bool cleanup)
{
	if (child->getParent())
		child->getParent()->removeChild(child, cleanup);
	child->setWidgetParent(nullptr);
	m_widgetChildren.eraseObject(child);
	DRAW_RENDERER;
}

void Widget::removeAllWidget()
{
	while (!m_widgetChildren.empty())
	{
		removeWidget(m_widgetChildren.front());
	}
	DRAW_RENDERER;
}

Widget* Widget::getWidgetParent()
{
	return m_widgetParent;
}

void Widget::removeFromParent()
{
	removeFromParentAndCleanup(true);
}

void Widget::removeFromParentAndCleanup(bool cleanup)
{
	if (m_widgetParent)
	{
		m_widgetParent->removeWidget(this, cleanup);
	}
	else if (_parent)
	{
		_parent->removeChild(this, cleanup);
	}
}

//////////////////////////////////////////

void Widget::setTouchCallfunc(const TouchCallfunc& func)
{
	m_touchCallfunc = func;
}

void Widget::setAutoTouchCallfunc(bool autoCallfunc)
{
	m_autoTouchCallfunc = autoCallfunc;
}

void Widget::enableLongPress(bool enabled)
{
	m_longPressEnabled = enabled;
	if (!m_longPressEnabled)
		unschedule(schedule_selector(Widget::detectiveLongPress));
}

void Widget::setLongPressInterval(float interval)
{
	m_longPressInterval = interval;
}

void Widget::setTouchExtend(int radius)
{ 
	m_touchExtend.x = radius;
	m_touchExtend.y = radius;
}

void Widget::setTouchExtend(const Vec2& size)
{
	m_touchExtend.x = size.x;
	m_touchExtend.y = size.y;
}

cocos2d::Touch* Widget::getTouch()
{
	return m_touch;
}

//////////////////////////////////////////

void Widget::updateSizeFromParent(const cocos2d::Size& virtualSize)
{
	if (m_percentWidth == NaN && m_percentHeight == NaN)return;
	auto __contentSize = _contentSize;
	if (m_percentWidth != NaN)
	{
		__contentSize.width = virtualSize.width * MIN(1.f, m_percentWidth / 100.f);
		m_givenSize.width = __contentSize.width;
	}
	if (m_percentHeight != NaN)
	{
		__contentSize.height = virtualSize.height * MIN(1.f, m_percentHeight / 100.f);
		m_givenSize.height = __contentSize.height;
	}
	if (__contentSize.equals(_contentSize))return;
	Node::setContentSize(__contentSize);
	updateRelativeLayout();
	if (!m_parsing)
		drawRenderer(false);
}

void Widget::updateSizeFromContent(const cocos2d::Size& contentSize)
{
	if (m_givenSize.width >= 0 && m_givenSize.height >= 0)return;
	Size _size = m_givenSize;
	if (m_givenSize.width < 0)
		_size.width = contentSize.width;
	if (m_givenSize.height < 0)
		_size.height = contentSize.height;

	if (_size.equals(_contentSize))return;
	Node::setContentSize(contentSize);
	if ((m_percentWidth == NaN || m_percentHeight == NaN) && !m_parsing && m_widgetParent)
	{
		callParentRelayout();
	}
	updateRelativeLayout();
}

void Widget::updateSize(const cocos2d::Size& contentSize)
{
	if (contentSize.equals(_contentSize))return;
	Node::setContentSize(contentSize);
	if ((m_percentWidth == NaN || m_percentHeight == NaN) && !m_parsing && m_widgetParent)
	{
		callParentRelayout();
	}
	updateRelativeLayout();
	if (!m_parsing)
		drawRenderer(false);
}

void Widget::updateRelativePositionFromParent(const cocos2d::Size& virtualSize)
{
	if (isParentLinearLayout())return;
	if (m_left != NaN)
		Node::setPosition(left2PositionX(m_left), _position.y);
	if (m_right != NaN && m_widgetParent)
		Node::setPosition(right2PositionX(m_right, virtualSize.width), _position.y);
	if (m_top != NaN && m_widgetParent)
		Node::setPosition(_position.x, top2PositionY(m_top, virtualSize.height));
	if (m_bottom != NaN)
		Node::setPosition(_position.x, bottom2PositionY(m_bottom));
	if (m_percentX != NaN && m_widgetParent)
		Node::setPosition(virtualSize.width * MIN(1.f, m_percentX / 100.f), _position.y);
	if (m_percentY != NaN && m_widgetParent)
		Node::setPosition(_position.x, virtualSize.height * MIN(1.f, m_percentY / 100.f));
}

////////////////////////////////////////////

bool Widget::init()
{
	if (!Node::init())
	{
		return false;
	}
	Node::setCascadeColorEnabled(true);
	Node::setCascadeOpacityEnabled(true);
	Node::setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	return true;
}

///////////////////////////////////////////////////////////////////////

void Widget::parse(std::vector<std::string>* keys, std::vector<std::string>* values)
{
	PARSING_START;
	parseAttr(keys, values);
	PARSING_END;
	drawRenderer();
}

void Widget::parse(const std::string& key, const std::vector<Widget*>& nodeParams)
{
	PARSING_START;
	parseAttr(key, nodeParams);
	PARSING_END;
	drawRenderer();
}

void Widget::parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values)
{
	for (auto i = 0; i < keys->size(); ++i)
	{
		const auto& _key = keys->at(i);
		const auto& _value = values->at(i);
		if (_key == "width")
		{
			//如果是百分比
			if (_value.back() == '%')
			{
				setPercentWidth(ParamUtil::readPercent(_value));
			}
			else
			{
				setWidth(ParamUtil::readFloat(_value));
			}
		}
		else if (_key == "height")
		{
			//如果是百分比
			if (_value.back() == '%')
			{
				setPercentHeight(ParamUtil::readPercent(_value));
			}
			else
			{
				setHeight(ParamUtil::readFloat(_value));
			}
		}
		else if (_key == "size" || _key == "contentsize")
		{
			setContentSize(ParamUtil::readSize(_value));
		}
		else if (_key == "x")
		{
			//如果是百分比
			if (_value.back() == '%')
			{
				setPercentX(ParamUtil::readPercent(_value));
			}
			else
			{
				setX(ParamUtil::readFloat(_value));
			}
		}
		else if (_key == "y")
		{
			//如果是百分比
			if (_value.back() == '%')
			{
				setPercentY(ParamUtil::readPercent(_value));
			}
			else
			{
				setY(ParamUtil::readFloat(_value));
			}
		}
		else if (_key == "z")
		{
			setZ(ParamUtil::readInt(_value));
		}
		else if (_key == "postion" || _key == "pos")
		{
			auto _position = ParamUtil::readVec2(_value);
			setPosition(_position.x, _position.y);
		}
		else if (_key == "left")
		{
			setLeft(ParamUtil::readFloat(_value));
		}
		else if (_key == "right")
		{
			setRight(ParamUtil::readFloat(_value));
		}
		else if (_key == "top")
		{
			setTop(ParamUtil::readFloat(_value));
		}
		else if (_key == "bottom")
		{
			setBottom(ParamUtil::readFloat(_value));
		}
		else if (_key == "marginleft")
		{
			setMarginLeft(ParamUtil::readUInt(_value));
		}
		else if (_key == "marginright")
		{
			setMarginRight(ParamUtil::readUInt(_value));
		}
		else if (_key == "margintop")
		{
			setMarginTop(ParamUtil::readUInt(_value));
		}
		else if (_key == "marginbottom")
		{
			setMarginBottom(ParamUtil::readUInt(_value));
		}
		else if (_key == "id")
		{
			setId(_value);
		}
		else if (_key == "enabled" || _key == "enable")
		{
			setEnabled(ParamUtil::readBool(_value));
		}
		else if (_key == "touchenabled" || _key == "touchenable")
		{
			setTouchEnabled(ParamUtil::readBool(_value));
		}
		else if (_key == "visible")
		{
			Node::setVisible(ParamUtil::readBool(_value));
		}
		else if (_key == "color")
		{
			setColor(ParamUtil::readColor3B(_value));
		}
		else if (_key == "alpha")
		{
			GLubyte _opacity = MAX(MIN(ParamUtil::readFloat(_value) * 255, 255), 0);
			Node::setOpacity(_opacity);
		}
		else if (_key == "opacity")
		{
			Node::setOpacity(ParamUtil::readUInt(_value));
		}
		else if (_key == "rotation")
		{
			Node::setRotation(ParamUtil::readFloat(_value));
		}
		else if (_key == "rotationx")
		{
			Node::setRotationX(ParamUtil::readFloat(_value));
		}
		else if (_key == "rotationy")
		{
			Node::setRotationY(ParamUtil::readFloat(_value));
		}
		else if (_key == "scalex")
		{
			Node::setScaleX(ParamUtil::readFloat(_value));
		}
		else if (_key == "scaley")
		{
			Node::setScaleY(ParamUtil::readFloat(_value));
		}
		else if (_key == "scale")
		{
			setScale(ParamUtil::readFloat(_value));
		}
		else if (_key == "ignoreanchor")
		{
			Node::ignoreAnchorPointForPosition(ParamUtil::readBool(_value));
		}
		else if (_key == "anchor")
		{
			Node::setAnchorPoint(ParamUtil::readVec2(_value));
		}
		else if (_key == "enablelongpress")
		{
			enableLongPress(ParamUtil::readBool(_value));
		}
		else if (_key == "longpressinterval")
		{
			setLongPressInterval(ParamUtil::readFloat(_value));
		}
		else if (_key == "swallowtouch")
		{
			setSwallowTouch(ParamUtil::readBool(_value));
		}
		else if (_key == "touchextend")
		{
			setTouchExtend(ParamUtil::readUInt(_value));
		}
		else if (_key == "touchextend2")
		{
			setTouchExtend(ParamUtil::readVec2(_value));
		}
		else if (_key == "autotouchcallfunc")
		{
			setAutoTouchCallfunc(ParamUtil::readBool(_value));
		}
		else if (_key == "darkmode")
		{
			setDarkMode(ParamUtil::readBool(_value));
		}
	}
}

///////////////////////////////////////////////////////////////////////

float Widget::left2PositionX(float left)
{
	float _x = getRealAnchor().x * _contentSize.width;
	if (left != NaN)
		_x += left;
	return _x;
}

float Widget::right2PositionX(float right, float parentWidth)
{
	float _parentWidth = 0;
	if (m_widgetParent)
		_parentWidth = m_widgetParent->getWidth();
	if (parentWidth >= 0)
		_parentWidth = parentWidth;

	return _parentWidth - right - _contentSize.width + getRealAnchor().x * _contentSize.width;
}

float Widget::bottom2PositionY(float bottom)
{
	float _y = getRealAnchor().y * _contentSize.height;
	if (bottom != NaN)
		_y += bottom;
	return _y;
}

float Widget::top2PositionY(float top, float parentHeight)
{
	float _parentHeight = 0;
	if (m_widgetParent)
		_parentHeight =m_widgetParent->getHeight();
	if (parentHeight >= 0)
		_parentHeight = parentHeight;
	return _parentHeight - top - _contentSize.height + getRealAnchor().y * _contentSize.height;
}

bool Widget::isParentLinearLayout()
{
	do
	{
		CC_BREAK_IF(!m_widgetParent);
		auto _parentType = m_widgetParent->getType();
		if (_parentType == WidgetType::HBOX || _parentType == WidgetType::VBOX || _parentType == WidgetType::TILEBOX)
		{
			return true;
		}
	} while (0);
	return false;
}

void Widget::callParentRelayout()
{
	if (!m_widgetParent)return;
	m_widgetParent->relayoutByChildrenChanges();
}

void Widget::updateRelativeLayout()
{
	if (isParentLinearLayout())return;

	if (m_left != NaN)
		Node::setPosition(left2PositionX(m_left), _position.y);
	if (m_right != NaN && m_widgetParent)
		Node::setPosition(right2PositionX(m_right), _position.y);
	if (m_top != NaN && m_widgetParent)
		Node::setPosition(_position.x, top2PositionY(m_top));
	if (m_bottom != NaN)
		Node::setPosition(_position.x, bottom2PositionY(m_bottom));
	if (m_percentX != NaN && m_widgetParent)
		Node::setPosition(m_widgetParent->getWidth() * MIN(1.f, m_percentX / 100.f), _position.y);
	if (m_percentY != NaN && m_widgetParent)
		Node::setPosition(_position.x, m_widgetParent->getHeight() * MIN(1.f, m_percentY / 100.f));
}

///////////////////////////////////////////////////////////////////////

const cocos2d::Size& Widget::getParentSize()
{
	auto _parent = getParent();
	if (_parent)
		return _parent->getContentSize();
	else
		return Director::getInstance()->getWinSize();
}

bool Widget::isAncestorsVisible(Node* node)
{
	do
	{
		CC_BREAK_IF(!node);
		auto _parent = node->getParent();
		if (_parent && !_parent->isVisible())
			return false;
		return isAncestorsVisible(_parent);
	} while (0);
	return true;
}

bool Widget::isAncestorsEnabled(Node* node)
{
	do
	{
		CC_BREAK_IF(!node);
		auto _widgetParent = dynamic_cast<Widget*>(node->getParent());
		CC_BREAK_IF(!_widgetParent);
		if(!_widgetParent->isEnabled())
			return false;
		return isAncestorsEnabled(_widgetParent);
	} while (0);
	return true;
}

bool Widget::hasAncestorsSVParent(cocos2d::Node* node, ScrollView** dstParent)
{
	do
	{
		auto _parent = node->getParent();
		CC_BREAK_IF(!_parent);
		auto _widgetParent = dynamic_cast<Widget*>(_parent);
		if (_widgetParent)
		{
			WidgetType _type = _widgetParent->getType();
			if ((_type != WidgetType::SCROLLVIEW) &&
				(_type != WidgetType::LISTVIEW) &&
				(_type != WidgetType::GRIDVIEW))
			{
				return hasAncestorsSVParent(_widgetParent, dstParent);
			}
			if (dstParent != nullptr)
				*dstParent = (ScrollView*)_parent;
			return true;
		}
		else
		{
			return hasAncestorsSVParent(_parent, dstParent);
		}
	} while (0);
	return false;
}

cocos2d::Vec2 Widget::getRealAnchor()
{
	if (isIgnoreAnchorPointForPosition())
		return Vec2::ZERO;
	else
		return getAnchorPoint();
}

void Widget::onExit()
{
	setTouchEnabled(false);
	unschedule(schedule_selector(Widget::detectiveLongPress));
	Node::onExit();
}

bool Widget::hitTest(const cocos2d::Vec2& vec)
{
	//判断是否有顶级ScrollView父对象
	ScrollView* _svParent = nullptr;
	hasAncestorsSVParent(this, &_svParent);
	if (_svParent != nullptr)
	{
		Rect _area;
		_area.size = _svParent->getContentSize();
		if (!_area.containsPoint(_svParent->convertToNodeSpace(vec)))
			return false;
	}

	Vec2 _nsp = convertToNodeSpace(vec);
	Rect _area;
	{
		_area.size = _contentSize;
		if (m_touchExtend.x > 0 && m_touchExtend.y > 0)
		{
			_area = Rect(-m_touchExtend.x, -m_touchExtend.y, _contentSize.width + m_touchExtend.x * 2, _contentSize.height + m_touchExtend.y * 2);
		}
		else if (m_touchExtend.x > 0 )
		{
			_area.origin.x = -m_touchExtend.x; 
			_area.size.width = _contentSize.width + m_touchExtend.x * 2;
		}
		else if (m_touchExtend.y > 0)
		{
			_area.origin.y = -m_touchExtend.y;
			_area.size.height = _contentSize.height + m_touchExtend.y * 2;
		}
	}
	return _area.containsPoint(_nsp);
}

bool Widget::onTouchBegan(Touch *touch, cocos2d::Event *unusedEvent)
{
	do
	{
		m_touch = touch;
		if (m_touchCallfunc)
		{
			m_touchCallfunc(this, TouchType::DOWN);
		}
		CC_BREAK_IF(!isVisible());
		CC_BREAK_IF(!isEnabled());
		CC_BREAK_IF(!isAncestorsVisible(this));
		CC_BREAK_IF(!isAncestorsEnabled(this));
		CC_BREAK_IF(!hitTest(touch->getLocation()));
		if (m_longPressEnabled && m_longPressInterval > 0)
			scheduleOnce(schedule_selector(Widget::detectiveLongPress), m_longPressInterval);
		setFocus();
		if (m_touchCallfunc)
		{
			m_touchCallfunc(this, TouchType::PRESS);
		}
		return true;
	} while (0);
	m_touch = nullptr;
	return false;
}

void Widget::onTouchMoved(Touch *touch, cocos2d::Event *unusedEvent)
{
	bool _hitTest = hitTest(touch->getLocation());
	if (_hitTest)
		setFocus();
	else
	{
		unschedule(schedule_selector(Widget::detectiveLongPress));
		setBlur();
	}
		
	if (m_touchCallfunc)
	{
		m_touchCallfunc(this, TouchType::MOVE);
	}
}

void Widget::onTouchEnded(Touch *touch, cocos2d::Event *unusedEvent)
{
	this->retain();
	unschedule(schedule_selector(Widget::detectiveLongPress));
	bool _hitTest = hitTest(touch->getLocation());
	setBlur();
	
	if (m_touchCallfunc)
		m_touchCallfunc(this, TouchType::RELEASE);

	//如果触发过长按，则点击不会再触发
	if (_hitTest && !m_executedLongPress)
		onClick();

	m_executedLongPress = false;
	m_touch = nullptr;
	this->release();
}

void Widget::onTouchCancelled(Touch *touch, cocos2d::Event *unusedEvent)
{
	onTouchEnded(touch, unusedEvent);
}

void Widget::onFocus()
{
	//do nothing
}

void Widget::onBlur()
{
	Node::setColor(Color3B::WHITE);
}

void Widget::onClick()
{
	if (m_touchCallfunc)
		m_touchCallfunc(this, TouchType::CLICK);
}

void Widget::onDisabled()
{
	if (m_darkMode)
	{
		Node::setColor(Color3B(100, 100, 100));
	}
}

void Widget::onLongPress()
{

}

void Widget::detectiveLongPress(float)
{
	onLongPress();
	m_executedLongPress = true;
	if (m_touchCallfunc)
		m_touchCallfunc(this, TouchType::LONG_PRESS);
}
TINA_NS_END