#include "ScrollView.h"
#include "util/ParamUtil.h"
#include "util/LayoutUtil.h"
#include "Box.h"

USING_NS_CC;

TINA_NS_BEGIN

#define DETECTIVE_DIRECTION_DURATION (.5f)
#define DETECTIVE_INERTIA_DURATION (0.016f)
#define DETECTIVE_INERTIA_SPEED (20.f)
#define INERTIA_ACCELERATION_SPEED (-1000.f)
#define REBOUND_SPEED (500.f)
#define ANIMATE_SCROLL_SPEED (1500.f)
#define SCROLL_ACTION_TAG (999901)
#define MAX_DRAG_SPEED (1000.f)
#define DEFAULT_WIDTH (100)
#define DEFAULT_HEIGHT (100)

ScrollView* ScrollView::create()
{
	auto _ref = new ScrollView();
	if (_ref && _ref->init(Size(DEFAULT_WIDTH, DEFAULT_HEIGHT)))
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

ScrollView* ScrollView::create(const cocos2d::Size& size)
{
	ScrollView* pRet = new (std::nothrow) ScrollView();
	if (pRet && pRet->init(size))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

ScrollView* ScrollView::create(float width, float height)
{
	return create(Size(width, height));
}

void ScrollView::parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values)
{
	Widget::parseAttr(keys, values);
	for (auto i = 0; i < keys->size(); ++i)
	{
		const auto& _key = keys->at(i);
		const auto& _value = values->at(i);
		if (_key == "sorption")
		{
			setSorption(ParamUtil::readVec2(_value));
		}
		else if (_key == "direction" || _key == "scrolldirection")
		{
			setScrollDirection(ParamUtil::readScrollDirection(_value));
		}
		else if (_key == "scrollenabled" || _key == "scrollenable" || _key == "scroll")
		{
			setScrollEnabled(ParamUtil::readBool(_value));
		}
		else if (_key == "bounceenabled" || _key == "bounceenable" || _key == "bounce")
		{
			setBounceEnabled(ParamUtil::readBool(_value));
		}
	}
}

void ScrollView::setSorption(const Vec2& sorption)
{
	m_sorption.x = fabsf(sorption.x);
	m_sorption.y = fabsf(sorption.y);
}

void ScrollView::setContainerSize(const cocos2d::Size& size)
{
	m_container->setContentSize(size);
	calcScrollRect();
}

void ScrollView::scrollTo(const cocos2d::Vec2& distance, bool withAnimate, float speed)
{
	//
	if (m_container->getActionByTag(SCROLL_ACTION_TAG))
	{
		auto _pos = m_scrollOriginPos + m_scrollTo;
		_pos.x = MAX(MIN(_pos.x, m_scrollRect.getMaxX()), m_scrollRect.getMinX());
		_pos.y = MAX(MIN(_pos.y, m_scrollRect.getMaxY()), m_scrollRect.getMinY());
		m_container->setPosition(_pos);
	}
	m_container->stopAllActions();

	m_scrollTo = distance;
	m_scrollOriginPos = m_container->getPosition();
	auto _distance = distance;
	_distance.x = MAX(MIN(m_scrollRect.getMaxX() - m_container->getPositionX(), _distance.x), m_scrollRect.getMinX() - m_container->getPositionX());
	_distance.y = MAX(MIN(m_scrollRect.getMaxY() - m_container->getPositionY(), _distance.y), m_scrollRect.getMinY() - m_container->getPositionY());
	
	Vec2 _targetPos = m_scrollTo + m_scrollOriginPos;
	{
		_targetPos.x = (int)_targetPos.x;
		_targetPos.y = (int)_targetPos.y;
		_targetPos.x = MAX(MIN(m_scrollRect.getMaxX(), _targetPos.x), m_scrollRect.getMinX());
		_targetPos.y = MAX(MIN(m_scrollRect.getMaxY(), _targetPos.y), m_scrollRect.getMinY());
	}

	if (!withAnimate)
	{
		m_container->setPosition(m_container->getPosition() + _distance);
		handlerScrollEnd();
		m_scrollTo = Vec2::ZERO;
		m_scrollOriginPos = m_container->getPosition();
	}
	else
	{
		float _speed = speed <= 0 ? ANIMATE_SCROLL_SPEED : speed;
		float _duration = _distance.getDistance(Vec2::ZERO) / _speed;
		auto _action = Sequence::createWithTwoActions(
			EaseSineIn::create(MoveTo::create(_duration, _targetPos)),
			CallFunc::create([=]()->void{
			handlerScrollEnd();
			m_scrollTo = Vec2::ZERO;
			m_scrollOriginPos = m_container->getPosition();
		})
			);
		_action->setTag(SCROLL_ACTION_TAG);
		m_container->runAction(_action);
		m_isScrolling = true;
	}
}

void ScrollView::scrollTo(const Location& location, bool withAnimate, float speed)
{
	Vec2 _distance;
	switch (location)
	{
	case Location::TOP:
		_distance.y = m_scrollRect.getMinY() - m_container->getPositionY();
		break;
	case Location::BOTTOM:
		_distance.y = m_scrollRect.getMaxY() - m_container->getPositionY();
		break;
	case Location::LEFT:
		_distance.x = m_scrollRect.getMaxX() - m_container->getPositionX();
		break;
	case Location::RIGHT:
		_distance.x = m_scrollRect.getMinX() - m_container->getPositionX();
		break;
	default:
		break;
	}
	scrollTo(_distance, withAnimate, speed);
}

Vec2 ScrollView::getContentOffset()
{
	Vec2 _offset;
	if (m_scrollRect.getMinX() != 0)
	{
		_offset.x = m_container->getPositionX() / m_scrollRect.getMinX();
	}
	else
	{
		_offset.x = 0;
	}

	if (m_scrollRect.getMinY() != 0)
	{
		_offset.y = 1.f - m_container->getPositionY() / m_scrollRect.getMinY();
	}
	else
	{
		_offset.y = 0;
	}
	return _offset;
}

//=============================================================================

bool ScrollView::init(const Size& viewSize, BaseBox* container)
{
	if (!Widget::init())
	{
		return false;
	}
	setType(WidgetType::SCROLLVIEW);
	m_container = nullptr;
	m_containerLocation = Vec2::ZERO;
	Node::setContentSize(viewSize);
	m_scrollEnabled = true;
	m_bounce = true;
	m_scrollDirection = ScrollDirection::BOTH;
	m_finalScrollDirection = ScrollDirection::BOTH;
	m_detectiveDirectionPast = 0;
	m_isInertia = false;
	m_inertiaDuration = 0;
	m_inertialOriginSpeed = 0;

	m_clippingToBounds = true;
	m_sorption = Vec2::ZERO;
	m_isScrolling = false;

	m_beganPos = Vec2::ZERO;
	m_movedPos = Vec2::ZERO;
	m_endedPos = Vec2::ZERO;
	m_isDrag = false;
	m_dragDuration = 0;

	m_container = container;
	if (!m_container)
		m_container = BaseBox::create();
	m_container->ignoreAnchorPointForPosition(true);
	Node::addChild(m_container, m_container->getLocalZOrder(), m_container->getTag());
	m_container->setPosition(0, _contentSize.height - m_container->getHeight());
	m_container->setLayoutCallfunc(CC_CALLBACK_1(ScrollView::handlerContainerLayout, this));
	calcScrollRect();

	setTouchEnabled(true);
	this->scheduleUpdate();

	return true;
}

void ScrollView::updateSizeFromParent(const cocos2d::Size& virtualSize)
{
	Widget::updateSizeFromParent(virtualSize);
	handlerContainerLayout(m_container);
}

void ScrollView::updateSize(const cocos2d::Size& contentSize)
{
	Widget::updateSize(contentSize);
	handlerContainerLayout(m_container);
}

bool ScrollView::onTouchBegan(Touch *touch, Event *unusedEvent)
{
	bool _canContinue = Widget::onTouchBegan(touch, unusedEvent);
	if (!_canContinue)return false;
	
	m_beganPos = touch->getLocation();
	//取消回弹
	m_container->stopAllActions();
	//取消惯性
	m_isInertia = false;
	//开始触摸
	m_isDrag = true;
	m_dragDuration = 0;

	if (m_scrollDirection != ScrollDirection::AUTO)
	{
		m_finalScrollDirection = m_scrollDirection;
	}
	return true;
}

void ScrollView::onTouchMoved(Touch *touch, Event *unusedEvent)
{
	Widget::onTouchMoved(touch, unusedEvent);
	m_movedPos = touch->getLocation();
	if (!checkCanScroll())return;
	m_isScrolling = true;
	auto _delta = touch->getDelta();
	if (!m_scrollRect.containsPoint(m_container->getPosition()))
	{
		_delta = _delta * 0.2f;
	}
	auto _newPos = m_container->getPosition() + _delta;
	updateContainerPos(_newPos);
}

void ScrollView::onTouchEnded(Touch *touch, Event *unusedEvent)
{
	Widget::onTouchEnded(touch, unusedEvent);
	m_isDrag = false;
	m_endedPos = touch->getLocation();
	if (!checkCanScroll())return;
	auto _deltaPos = m_endedPos - m_beganPos;
	auto _distance = 0;

	//判断滚动方向
	if (m_scrollDirection == ScrollDirection::HORIZONTAL)
	{
		_distance = m_endedPos.x - m_beganPos.x;
		m_dragDirection = (_distance > 0) ? Vec2(1, 0) : Vec2(-1, 0);
	}
	else if (m_scrollDirection == ScrollDirection::VERTICAL)
	{
		_distance = m_endedPos.y - m_beganPos.y;
		m_dragDirection = (_distance > 0) ? Vec2(0, 1) : Vec2(0, -1);
	}
	else if (m_scrollDirection == ScrollDirection::BOTH)
	{
		_distance = _deltaPos.length();
		m_dragDirection = _deltaPos.getNormalized();
	}
	m_inertialOriginSpeed = MIN(fabsf(_distance / m_dragDuration), MAX_DRAG_SPEED);

	if (isNeedRebound())
	{
		rebound();
	}
	else
	{
		//是否能产生惯性
		if (m_inertialOriginSpeed > 500)
		{
			//如果使用了吸附模式
			if (m_sorption != Vec2::ZERO && m_scrollDirection == ScrollDirection::HORIZONTAL && fabsf(m_dragDirection.x) > fabsf(m_dragDirection.y))
			{
				int _inertiaNum = m_dragDirection.x;
				auto _sorption = m_sorption * _inertiaNum + getSorptionPosition(m_container->getPosition()) - m_container->getPosition();
				scrollTo(_sorption);
			}
			else if (m_sorption != Vec2::ZERO && m_scrollDirection == ScrollDirection::VERTICAL && fabsf(m_dragDirection.y) > fabsf(m_dragDirection.x))
			{
				int _inertiaNum = m_dragDirection.y;
				auto _sorption = m_sorption * _inertiaNum + getSorptionPosition(m_container->getPosition()) - m_container->getPosition();
				scrollTo(_sorption);
			}
			else
				m_isInertia = true;
		}
		else
		{
			if (needSorption())
				sorption();
			else
				handlerScrollEnd();
		}
	}
}

void ScrollView::updateContainerPos(const cocos2d::Point& newPos)
{
	auto _newPos = newPos;
	//如果没有橡皮筋效果
	if (!m_bounce)
	{
		_newPos.x = MIN(MAX(_newPos.x, m_scrollRect.getMinX()), m_scrollRect.getMaxX());
		_newPos.y = MIN(MAX(_newPos.y, m_scrollRect.getMinY()), m_scrollRect.getMaxY());
	}

	switch (m_finalScrollDirection)
	{
	case ScrollDirection::HORIZONTAL:
		m_container->setPositionX(_newPos.x);
		break;
	case ScrollDirection::VERTICAL:
		m_container->setPositionY(_newPos.y);
		break;
	case ScrollDirection::BOTH:
		m_container->setPosition(_newPos);
		break;
	default:
		m_container->setPosition(_newPos);
		break;
	}
}

Rect ScrollView::getBoundingBox()
{
	Rect _rect = { 0, 0, getContentSize().width, getContentSize().height };
	return _rect;
}

bool ScrollView::isNeedRebound()
{
	return !m_scrollRect.containsPoint(m_container->getPosition());
}

void ScrollView::calcScrollRect()
{
	const auto& _containerSize = m_container->getContentSize();
	m_scrollRect.origin.x = MIN(_contentSize.width - _containerSize.width, 0);
	m_scrollRect.size.width = MAX(_containerSize.width - _contentSize.width, 0);
	if (_containerSize.height <= _contentSize.height)
	{
		m_scrollRect.origin.y = _contentSize.height - _containerSize.height;
		m_scrollRect.size.height = 0;
	}
	else
	{
		m_scrollRect.origin.y = _contentSize.height - _containerSize.height;
		m_scrollRect.size.height = fabsf(_contentSize.height - _containerSize.height);
	}
}

bool ScrollView::checkCanScroll()
{
	do
	{
		CC_BREAK_IF(!m_scrollEnabled);
		if (m_scrollDirection == ScrollDirection::HORIZONTAL &&
			_contentSize.width >= m_container->getContentSize().width)
		{
			break;
		}
		else if (m_scrollDirection == ScrollDirection::VERTICAL &&
			_contentSize.height >= m_container->getContentSize().height)
		{
			break;
		}
		else if (m_scrollDirection == ScrollDirection::BOTH &&
			_contentSize.width >= m_container->getContentSize().width &&
			_contentSize.height >= m_container->getContentSize().height)
		{
			break;
		}
		return true;
	} while (0);
	return false;
}

void ScrollView::rebound()
{
	Point _targetPos = m_container->getPosition();
	_targetPos.x = MIN(MAX(_targetPos.x, m_scrollRect.getMinX()), m_scrollRect.getMaxX());
	_targetPos.y = MIN(MAX(_targetPos.y, m_scrollRect.getMinY()), m_scrollRect.getMaxY());
	float _duration = MIN(m_container->getPosition().getDistance(_targetPos) / REBOUND_SPEED, 0.2f);
	auto _action = Sequence::createWithTwoActions(
		EaseBackOut::create(MoveTo::create(_duration, _targetPos)),
		CallFunc::create(CC_CALLBACK_0(ScrollView::handlerScrollEnd, this))
		);
	m_container->stopAllActions();
	m_container->runAction(_action);
	m_isScrolling = true;
}

void ScrollView::inertia(float dt)
{
	float _lastInertiaDuration = m_inertiaDuration;
	m_inertiaDuration += dt;

	float _speed = m_inertialOriginSpeed + INERTIA_ACCELERATION_SPEED * m_inertiaDuration;
	if (isNeedRebound())
	{
		_speed += INERTIA_ACCELERATION_SPEED * m_inertiaDuration * 2.f;
	}
	if (_speed <= 0)
	{
		m_inertiaDuration = 0;
		m_isInertia = false;
		//如果已经超出拖动区域
		if (isNeedRebound())
		{
			//需要反弹
			rebound();
		}
		else
		{
			if (needSorption())
			{
				sorption();
			}
			else
			{
				handlerScrollEnd();
			}
		}
		return;
	}

	float _timeParam = _lastInertiaDuration * 2 + dt;
	float _offset = (m_inertialOriginSpeed + INERTIA_ACCELERATION_SPEED * _timeParam * 0.5f) * dt;
	if (isNeedRebound())
		_offset *= (1.f - m_inertiaDuration) * 0.5f;
	float _offsetX = _offset * m_dragDirection.x;
	float _offsetY = _offset * m_dragDirection.y;
	auto _newPos = Vec2(_offsetX, _offsetY) + m_container->getPosition();
	updateContainerPos(_newPos);
}

bool ScrollView::needSorption()
{
	if (m_sorption.x > 0 || m_sorption.y > 0)
	{
		return true;
	}
	return false;
}

void ScrollView::sorption()
{
	if (m_sorption == Vec2::ZERO)return;
	Vec2 _sorptionPos = getSorptionPosition(m_container->getPosition());
	scrollTo(_sorptionPos - m_container->getPosition());
}

Vec2 ScrollView::getSorptionPosition(const Vec2& pos)
{
	if (m_sorption == Vec2::ZERO)return pos;
	Vec2 _sorptionPos = pos;
	int _x = _sorptionPos.x;
	int _y = _sorptionPos.y;
	if (m_sorption.x > 0)
	{
		int _mod = _x % (int)m_sorption.x;
		if (_mod != 0)
		{
			int _symbol = std::abs(_mod) / _mod;
			if (std::abs(_mod) >= m_sorption.x * 0.5f)
			{
				_sorptionPos.x += (m_sorption.x - std::abs(_mod)) * _symbol;
			}
			else
			{
				_sorptionPos.x += std::abs(_mod) * -_symbol;
			}
		}
	}
	if (m_sorption.y > 0)
	{
		int _mod = _y % (int)m_sorption.y;
		if (_mod != 0)
		{
			int _symbol = std::abs(_mod) / _mod;
			if (std::abs(_mod) >= m_sorption.y * 0.5f)
			{
				_sorptionPos.y += (m_sorption.y - std::abs(_mod)) * _symbol;
			}
			else
			{
				_sorptionPos.y += std::abs(_mod) * -_symbol;
			}
		}
	}
	return _sorptionPos;
}

void ScrollView::update(float dt)
{
	if (m_isScrolling)
	{
		const auto& _containerSize = m_container->getContentSize();
		m_containerLocation.x = m_container->getPositionX();
		m_containerLocation.y = _contentSize.height - (_containerSize.height + m_container->getPositionY());
	}

	//执行惯性
	if (m_isInertia)
	{
		inertia(dt);
	}

	if (m_isScrolling)
	{
		didScroll();
		if (m_didScrollCallfunc)
			m_didScrollCallfunc(this);
	}

	if (m_isDrag)
	{
		m_dragDuration += dt;
	}
}

void ScrollView::handlerScrollEnd()
{
	const auto& _containerSize = m_container->getContentSize();
	m_containerLocation.x = m_container->getPositionX();
	m_containerLocation.y = _contentSize.height - (_containerSize.height + m_container->getPositionY());
	m_isScrolling = false;
	endScroll();
	if (m_endScrollCallfunc)
	{
		m_endScrollCallfunc(this);
	}
}

void ScrollView::handlerContainerLayout(BaseBox* box)
{
	calcScrollRect();
	const auto& _containerSize = m_container->getContentSize();
	Vec2 _containerPos = { m_containerLocation.x, _contentSize.height - _containerSize.height - m_containerLocation.y };
	_containerPos.x = MIN(m_scrollRect.getMaxX(), MAX(m_scrollRect.getMinX(), _containerPos.x));
	_containerPos.y = MIN(m_scrollRect.getMaxY(), MAX(m_scrollRect.getMinY(), _containerPos.y));
	m_container->setPosition(_containerPos);
	m_containerLocation.x = m_container->getPositionX();
	m_containerLocation.y = _contentSize.height - (_containerSize.height + m_container->getPositionY());
}

//override Node addChild functions==========================================

void ScrollView::addWidget(Widget* child)
{
	m_container->addWidget(child);
}

void ScrollView::addWidget(Widget* child, const std::string &id)
{
	m_container->addWidget(child, id);
}

void ScrollView::addWidget(Widget* child, int localZOrder)
{
	m_container->addWidget(child, localZOrder);
}

void ScrollView::addWidget(Widget* child, int localZOrder, const std::string &id)
{
	m_container->addWidget(child, localZOrder, id);
}

void ScrollView::addWidgetAt(Widget* child, uint16_t index)
{
	m_container->addWidgetAt(child, index);
}

void ScrollView::removeWidget(Widget* child, bool cleanup)
{
	m_container->removeWidget(child, cleanup);
}

void ScrollView::removeAllWidget()
{
	m_container->removeAllWidget();
}

const cocos2d::Vector<Widget*>& ScrollView::getWidgetChildren()
{
	return m_container->getWidgetChildren();
}

//==========================================================================

//Clipping functions========================================================

void ScrollView::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
	// quick return if not visible
	if (!isVisible())
	{
		return;
	}

	uint32_t flags = processParentFlags(parentTransform, parentFlags);

	// IMPORTANT:
	// To ease the migration to v3.0, we still support the Mat4 stack,
	// but it is deprecated and your code should not rely on it
	Director* director = Director::getInstance();
	CCASSERT(nullptr != director, "Director is null when seting matrix stack");
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);

	this->beforeDraw();

	if (!_children.empty())
	{
		int i = 0;

		// draw children zOrder < 0
		for (; i < _children.size(); i++)
		{
			Node *child = _children.at(i);
			if (child->getLocalZOrder() < 0)
			{
				child->visit(renderer, _modelViewTransform, flags);
			}
			else
			{
				break;
			}
		}

		// this draw
		this->draw(renderer, _modelViewTransform, flags);

		// draw children zOrder >= 0
		for (; i < _children.size(); i++)
		{
			Node *child = _children.at(i);
			child->visit(renderer, _modelViewTransform, flags);
		}

	}
	else
	{
		this->draw(renderer, _modelViewTransform, flags);
	}

	this->afterDraw();

	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void ScrollView::beforeDraw()
{
	m_beforeDrawCommand.init(_globalZOrder);
	m_beforeDrawCommand.func = CC_CALLBACK_0(ScrollView::onBeforeDraw, this);
	Director::getInstance()->getRenderer()->addCommand(&m_beforeDrawCommand);
}

void ScrollView::onBeforeDraw()
{
	if (m_clippingToBounds)
	{
		m_scissorRestored = false;
		Rect frame = getViewRect();
		auto glview = Director::getInstance()->getOpenGLView();

		if (glview->isScissorEnabled()) {
			m_scissorRestored = true;
			m_parentScissorRect = glview->getScissorRect();
			//set the intersection of _parentScissorRect and frame as the new scissor rect
			if (frame.intersectsRect(m_parentScissorRect)) {
				float x = MAX(frame.origin.x, m_parentScissorRect.origin.x);
				float y = MAX(frame.origin.y, m_parentScissorRect.origin.y);
				float xx = MIN(frame.origin.x + frame.size.width, m_parentScissorRect.origin.x + m_parentScissorRect.size.width);
				float yy = MIN(frame.origin.y + frame.size.height, m_parentScissorRect.origin.y + m_parentScissorRect.size.height);
				glview->setScissorInPoints(x, y, xx - x, yy - y);
			}
		}
		else {
			glEnable(GL_SCISSOR_TEST);
			glview->setScissorInPoints(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
		}
	}
}

void ScrollView::afterDraw()
{
	m_afterDrawCommand.init(_globalZOrder);
	m_afterDrawCommand.func = CC_CALLBACK_0(ScrollView::onAfterDraw, this);
	Director::getInstance()->getRenderer()->addCommand(&m_afterDrawCommand);
}

void ScrollView::onAfterDraw()
{
	if (m_clippingToBounds)
	{
		if (m_scissorRestored) {//restore the parent's scissor rect
			auto glview = Director::getInstance()->getOpenGLView();

			glview->setScissorInPoints(m_parentScissorRect.origin.x, m_parentScissorRect.origin.y, m_parentScissorRect.size.width, m_parentScissorRect.size.height);
		}
		else {
			glDisable(GL_SCISSOR_TEST);
		}
	}
}

Rect ScrollView::getViewRect()
{
	Point screenPos = this->convertToWorldSpace(Point::ZERO);

	float scaleX = this->getScaleX();
	float scaleY = this->getScaleY();

	for (Node *p = _parent; p != NULL; p = p->getParent()) {
		scaleX *= p->getScaleX();
		scaleY *= p->getScaleY();
	}

	// Support negative scaling. Not doing so causes intersectsRect calls
	// (eg: to check if the touch was within the bounds) to return false.
	// Note, Node::getScale will assert if X and Y scales are different.
	if (scaleX < 0.f) {
		screenPos.x += _contentSize.width*scaleX;
		scaleX = -scaleX;
	}
	if (scaleY < 0.f) {
		screenPos.y += _contentSize.height*scaleY;
		scaleY = -scaleY;
	}

	return Rect(screenPos.x, screenPos.y, _contentSize.width*scaleX, _contentSize.height*scaleY);
}

bool ScrollView::isBegin()
{
	do
	{
		auto _offset = ScrollView::getContentOffset();
		if (m_scrollDirection == ScrollDirection::HORIZONTAL)
			return _offset.x <= 0;
		else if (m_scrollDirection == ScrollDirection::VERTICAL)
			return _offset.y <= 0;
	} while (0);
	return true;
}

bool ScrollView::isEnd()
{
	do
	{
		auto _offset = ScrollView::getContentOffset();
		if (m_scrollDirection == ScrollDirection::HORIZONTAL)
			return _offset.x >= 1;
		else if (m_scrollDirection == ScrollDirection::VERTICAL)
			return _offset.y >= 1;
	} while (0);
	return true;
}

TINA_NS_END