#include "Action.h"
#include "widget/Image.h"

USING_NS_CC;

TINA_NS_BEGIN
SizeTo* SizeTo::create(float duration, float dstWidth, float dstHeight)
{
	return create(duration, Size(dstWidth, dstHeight));
}

SizeTo* SizeTo::create(float duration, const cocos2d::Size& dstSize)
{
	auto _ref = new SizeTo();
	if (_ref && _ref->initWithDuration(duration, dstSize))
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

SizeTo* SizeTo::clone() const
{
	// no copy constructor
	auto a = new (std::nothrow) SizeTo();
	a->initWithDuration(_duration, m_dstSize);
	a->autorelease();
	return a;
}

SizeTo* SizeTo::reverse() const
{
	return nullptr;
}

void SizeTo::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	m_startSize = _target->getContentSize();
	m_diffSize = m_dstSize - m_startSize;
}

void SizeTo::update(float time)
{
	_target->setContentSize(Size(m_startSize.width + m_diffSize.width * time, m_startSize.height + m_diffSize.height * time));
}

bool SizeTo::initWithDuration(float duration, const cocos2d::Size& dstSize)
{
	if (!ActionInterval::initWithDuration(duration))
	{
		return false;
	}
	m_dstSize = dstSize;
	return true;
}

/////

RadialBlurTo* RadialBlurTo::create(float duration, int dstStrength)
{
	auto _ref = new RadialBlurTo();
	if (_ref && _ref->initWithDuration(duration, dstStrength))
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

RadialBlurTo* RadialBlurTo::clone() const
{
	// no copy constructor
	auto a = new (std::nothrow) RadialBlurTo();
	a->initWithDuration(_duration, m_dstStrength);
	a->autorelease();
	return a;
}

RadialBlurTo* RadialBlurTo::reverse() const
{
	return nullptr;
}

void RadialBlurTo::startWithTarget(Node *target)
{
	auto __target = dynamic_cast<tina::Image*>(target);
	if (__target == nullptr)return;
	ActionInterval::startWithTarget(__target);
	m_startStrength = __target->getRadialBlurStrength();
	m_diffStrength = m_dstStrength - m_startStrength;
}

void RadialBlurTo::update(float time)
{
	auto __target = dynamic_cast<tina::Image*>(_target);
	if (__target == nullptr)return;
	__target->setRadialBlurStrength(m_startStrength + m_diffStrength * time);
}

bool RadialBlurTo::initWithDuration(float duration, int dstStrength)
{
	if (!ActionInterval::initWithDuration(duration))
	{
		return false;
	}
	m_dstStrength = dstStrength;
	return true;
}
TINA_NS_END