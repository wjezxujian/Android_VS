#include "Button.h"
#include "util/ParamUtil.h"

USING_NS_CC;

TINA_NS_BEGIN

#define SCROLL_BUTTON_DIVIATION (10.f)
#define COMPATIBALE_RADIUS (10.f)
#define LONG_PRESS_INTERVAL (1.f)
#define SCALE_MODE_FACTOR (0.95f)
#define SCALE_ACTION_TAG 999901
#define SCALE_ACTION_INTERVAL 0.06f
#define CLICK_MAX_MOVE_DISTANCE (20)

Button* Button::create()
{
	auto _ref = new Button();
	if (_ref && _ref->init())
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

Button* Button::create(Widget* upState, Widget* downState, Widget* disabledState)
{
	auto _ref = new Button();
	if (_ref && _ref->initWithStates(upState, downState, disabledState))
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

void Button::parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values)
{
	Widget::parseAttr(keys, values);
	for (auto i = 0; i < keys->size(); ++i)
	{
		const auto& _key = keys->at(i);
		const auto& _value = values->at(i);
		if (_key == "scalemode")
		{
			setScaleMode(ParamUtil::readBool(_value));
		}
		else if (_key == "scalemodefactor")
		{
			setScaleModeFactor(ParamUtil::readFloat(_value));
		}
		else if (_key == "enablelongpress" || _key == "longpressenabled" || _key == "longpressenable")
		{
			enableLongPress(ParamUtil::readBool(_value));
		}
		else if (_key == "longpressinterval")
		{
			setLongPressInterval(ParamUtil::readFloat(_value));
		}
		else if (_key == "cd" || _key == "touchcd")
		{
			setCD(ParamUtil::readFloat(_value));
		}
		else if (_key == "originscale")
		{
			setScale(ParamUtil::readFloat(_value));
		}
		else if (_key == "forceswallowtouch" || _key == "forceswallow")
		{
			setForceSwallowTouch(ParamUtil::readBool(_value));
		}
	}
}

void Button::parseAttr(const std::string& key, const std::vector<Widget*>& nodeParams)
{
	if (key == "up")
	{
		setUpState(nodeParams.front());
	}
	else if (key == "down")
	{
		setDownState(nodeParams.front());
	}
	else if (key == "disabled")
	{
		setDisabledState(nodeParams.front());
	}
}

void Button::setEnabled(bool enabled)
{
	if (m_enabled == enabled)return;
	m_enabled = enabled;
	if (m_enabled)
	{
		onBlur();
	}
	else
		onDisabled();
}

void Button::setUpState(Widget* node)
{
	CCASSERT(node != nullptr, "DO NOT BE NULL");
	if (m_upState)
	{
		m_upState->Node::removeFromParent();
	}
	m_upState = node;
	Node::addChild(m_upState);
	node->setPosition(Vec2::ZERO);
	node->ignoreAnchorPointForPosition(true);
	DRAW_RENDERER;
}

void Button::setDownState(Widget* node)
{
	CCASSERT(node != nullptr, "DO NOT BE NULL");
	if (m_downState)
	{
		m_downState->Node::removeFromParent();
	}
	m_downState = node;
	Node::addChild(m_downState);
	node->setPosition(Vec2::ZERO);
	node->ignoreAnchorPointForPosition(true);
	node->setVisible(false);
}

void Button::setDisabledState(Widget* node)
{
	CCASSERT(node != nullptr, "DO NOT BE NULL");
	if (m_disabledState)
	{
		m_disabledState->Node::removeFromParent();
	}
	m_disabledState = node;
	Node::addChild(m_disabledState);
	node->setPosition(Vec2::ZERO);
	node->ignoreAnchorPointForPosition(true);
	node->setVisible(false);
}

void Button::setScaleMode(bool enabled)
{
	if (m_scaleMode == enabled)return;
	m_scaleMode = enabled;
}

void Button::setScaleModeFactor(float factor)
{
	m_scaleModeFactor = factor;
}

void Button::setScale(float scale)
{
	Widget::setScale(scale);
	m_originScale = scale;
}

void Button::setForceSwallowTouch(bool swallow)
{
	if (swallow == m_forceSwallow)return;
	m_forceSwallow = swallow;
}

void Button::setCD(float interval)
{
	m_needCD = (interval > 0);
	m_cdInterval = interval;
	unschedule(schedule_selector(Button::detectiveCding));
}

/////

bool Button::init()
{
	return initWithStates(nullptr);
}

bool Button::initWithStates(Widget* upState, Widget* downState, Widget* disabledState)
{
	if (!Widget::init())
	{
		return false;
	}
	setType(WidgetType::BUTTON);
	m_enabled = true;
	m_needCD = false;
	m_isCDing = false;
	m_cdInterval = 0;
	m_longPressEnabled = false;
	m_longPressInterval = LONG_PRESS_INTERVAL;
	m_scaleModeFactor = SCALE_MODE_FACTOR;
	m_originScale = 1.f;
	m_upState = upState;
	if (m_upState)
	{
		Node::addChild(m_upState);
		m_upState->setPosition(Vec2::ZERO);
		m_upState->ignoreAnchorPointForPosition(true);
	}

	m_downState = downState;
	if (m_downState)
	{
		Node::addChild(m_downState);
		m_downState->setPosition(Vec2::ZERO);
		m_downState->ignoreAnchorPointForPosition(true);
		m_downState->setVisible(false);
	}

	m_disabledState = disabledState;
	if (m_disabledState)
	{
		Node::addChild(m_disabledState);
		m_disabledState->setPosition(Vec2::ZERO);
		m_disabledState->ignoreAnchorPointForPosition(true);
		m_disabledState->setVisible(false);
	}
	setTouchEnabled(true);
	setSwallowTouch(true);
	if (m_upState)
	{
		drawRenderer(true);
	}
	return true;
}

void Button::detectiveCding(float t)
{
	m_isCDing = false;
}

bool Button::onTouchBegan(Touch *touch, cocos2d::Event *unusedEvent)
{
	m_effectualClick = true;
	if (m_isCDing)return false;
	return Widget::onTouchBegan(touch, unusedEvent);
}

void Button::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unusedEvent)
{
	Widget::onTouchMoved(touch, unusedEvent);
	if (m_hasSVParent && (touch->getDelta().getLength() > CLICK_MAX_MOVE_DISTANCE ||
		touch->getLocation().getDistance(touch->getStartLocation()) > CLICK_MAX_MOVE_DISTANCE)
		)
	{
		m_effectualClick = false;
	}
}

void Button::onFocus()
{
	if (m_downState)
	{
		m_downState->setVisible(true);
		if (m_upState)
			m_upState->setVisible(false);
	}
	if (m_scaleMode)
	{
		if (!_director->isPaused())
		{
			this->stopActionByTag(SCALE_ACTION_TAG);
			auto _action = ScaleTo::create(SCALE_ACTION_INTERVAL, m_scaleModeFactor * m_originScale);
			_action->setTag(SCALE_ACTION_TAG);
			this->runAction(_action);
		}
		else
		{
			setScale(m_scaleModeFactor * m_originScale);
		}
	}
}

void Button::onBlur()
{
	if (m_downState)
		m_downState->setVisible(false);
	if (m_disabledState)
		m_disabledState->setVisible(false);
	if (m_upState)
		m_upState->setVisible(true);
	if (m_darkMode)
		m_upState->setColor(Color3B::WHITE);

	if (m_scaleMode)
	{
		if (!_director->isPaused())
		{
			this->stopActionByTag(SCALE_ACTION_TAG);
			auto _action = ScaleTo::create(SCALE_ACTION_INTERVAL, m_originScale);
			_action->setTag(SCALE_ACTION_TAG);
			this->runAction(_action);
		}
		else
		{
			setScale(m_originScale);
		}
	}
}

void Button::onClick()
{
	if (m_effectualClick)
	{
		Widget::onClick();
		if (m_needCD && m_cdInterval > 0)
		{
			m_isCDing = true;
			scheduleOnce(schedule_selector(Button::detectiveCding), m_cdInterval);
		}
	}
}

void Button::onDisabled()
{
	if (!m_upState)return;

	if (m_downState)
		m_downState->setVisible(false);

	if (m_disabledState)
	{
		if (m_upState)
			m_upState->setVisible(false);
		m_disabledState->setVisible(true);
	}
	else
	{
		if (m_darkMode)
			m_upState->setColor(Color3B(100, 100, 100));
	}
}

void Button::onEnter()
{
	Widget::onEnter();
	if (!m_enabled)
		onDisabled();
	m_hasSVParent = hasAncestorsSVParent(this, nullptr);
	setSwallowTouch(m_forceSwallow || (!m_hasSVParent && !m_forceSwallow));
}

void Button::onExit()
{
	Widget::onExit();
	unschedule(schedule_selector(Button::detectiveCding));
}

void Button::drawRenderer(bool updateSize)
{
	Size _size;
	if (m_upState)
		_size = m_upState->getContentSize();
	if (m_givenSize.width >= 0)
		_size.width = m_givenSize.width;
	if (m_givenSize.height >= 0)
		_size.height = m_givenSize.height;

	if (m_givenSize.width >= 0 || m_givenSize.width >= 0)
	{
		if (m_upState)
			m_upState->updateSizeFromParent(_size);
		if (m_downState)
			m_downState->updateSizeFromParent(_size);
		if (m_disabledState)
			m_disabledState->updateSizeFromParent(_size);
	}

	if (updateSize)
		updateSizeFromContent(_size);
}
TINA_NS_END