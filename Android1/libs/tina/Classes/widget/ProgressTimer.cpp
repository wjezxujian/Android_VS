#include "ProgressTimer.h"
#include "util/ParamUtil.h"
#include "util/TinaMgr.h"

USING_NS_CC;

TINA_NS_BEGIN

ProgressTimer::ProgressTimer()
: m_fromPercent(0)
, m_toPercent(0)
, m_duration(0)
, m_isFromPercent(false)
, m_isToPercent(false)
, m_isColor(false)
, m_isOpacity(false)
{

}

ProgressTimer* ProgressTimer::create()
{
	return create("");
}

ProgressTimer* ProgressTimer::create(const std::string& file)
{
	auto _ref = new ProgressTimer();
	if (_ref && _ref->init(file))
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

void ProgressTimer::parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values)
{
	Widget::parseAttr(keys, values);
	for (auto i = 0; i < keys->size(); ++i)
	{
		const auto& _key = keys->at(i);
		const auto& _value = values->at(i);
		if (_key == "source" || _key == "src")
		{
			setSource(_value);
		}
		else if (_key == "progresstype")
		{
			Type _processType = Type::BAR;
			if (_value == "radial")
				_processType = Type::RADIAL;
			else if (_value == "bar")
				_processType = Type::BAR;
			setProcessType(_processType);
		}
		else if (_key == "frompercent")
		{
			setFromPercentage(tina::ParamUtil::readFloat(_value));
		}
		else if (_key == "topercent" || _key == "percent")
		{
			setToPercentage(tina::ParamUtil::readFloat(_value));
		}
		else if (_key == "duration")
		{
			setPercentDuration(tina::ParamUtil::readFloat(_value));
		}
		else if (_key == "reverse")
		{
			setReverseDirection(tina::ParamUtil::readBool(_value));
		}
		else if (_key == "midpoint")
		{
			setMidpoint(tina::ParamUtil::readVec2(_value));
		}
		else if (_key == "barchangerate")
		{
			setBarChangeRate(tina::ParamUtil::readVec2(_value));
		}
		else if (_key == "color")
		{
			setColor(tina::ParamUtil::readColor3B(_value));
		}
		else if (_key == "opacity")
		{
			setOpacity((GLubyte)tina::ParamUtil::readUInt(_value));
		}

	}
}

void ProgressTimer::setSource(const std::string& file)
{
	if (file.empty())return;
	if (m_source == file)return;
	auto _fullPath = TINA_MGR->getImageFullPath(file);
	auto _sprite = Sprite::create(_fullPath);
	if (_sprite)this->setContentSize(_sprite->getContentSize());
	m_renderer->setSprite(_sprite);
}

void ProgressTimer::setProcessType(ProgressTimer::Type type)
{
	if (m_type == type)return;
	m_type = type;
	m_renderer->setType((cocos2d::ProgressTimer::Type)type);
}

void ProgressTimer::setFromPercentage(float percent)
{
	m_fromPercent = percent;
	m_isFromPercent = true;
	DRAW_RENDERER;
}
void ProgressTimer::setToPercentage(float percent)
{
	m_toPercent = percent;
	m_isToPercent = true;
	DRAW_RENDERER;
}
void ProgressTimer::setPercentDuration(float duration)
{
	m_duration  = duration;
	DRAW_RENDERER;
}

void ProgressTimer::setReverseDirection(bool value)
{
	m_renderer->setReverseDirection(value);
}

void ProgressTimer::setMidpoint(const cocos2d::Vec2& point)
{
	m_renderer->setMidpoint(point);
}

void ProgressTimer::setBarChangeRate(const cocos2d::Vec2& barChangeRate)
{
	m_renderer->setBarChangeRate(barChangeRate);
}

void ProgressTimer::setColor(const cocos2d::Color3B &color)
{
	m_color = color;
	m_isColor = true;
	DRAW_RENDERER;
}

void ProgressTimer::setOpacity(GLubyte opacity)
{
	m_opacity = opacity;
	m_isOpacity = true;
	DRAW_RENDERER;
}

void ProgressTimer::setPercentage(float percent, float duration)
{
	if (duration > 0)
	{
		ProgressTo* _to = ProgressTo::create(duration, percent);
		m_renderer->runAction(_to);	
	}
	else{
		m_renderer->setPercentage(percent);
	}
}

void ProgressTimer::setPercentFromTo(float fromPercent, float toPercent, float duration)
{
	ProgressFromTo* _to = ProgressFromTo::create(duration, fromPercent, toPercent);
	m_renderer->runAction(_to);
}

void ProgressTimer::updatePercentage()
{
	if (m_duration && m_isFromPercent && m_isToPercent)
	{
		ProgressFromTo* _to = ProgressFromTo::create(m_duration, m_fromPercent, m_toPercent);
		m_renderer->runAction(_to);
	}
	else if (m_duration && !m_isFromPercent && m_isToPercent){
		ProgressTo* _to = ProgressTo::create(m_duration, m_toPercent);
		m_renderer->runAction(_to);
	}
	else{
		m_renderer->setPercentage(m_toPercent);
	}

	m_fromPercent = 0;
	m_toPercent = 0;
	m_duration = 0;
	m_isFromPercent = false;
	m_isToPercent = false;
	
}

void ProgressTimer::updateSpriteAttr()
{
	if (m_renderer->getSprite())
	{
		if (m_isColor)
			m_renderer->setColor(m_color);

		if (m_isOpacity)
			m_renderer->setOpacity(m_opacity);


		m_isColor = false;
		m_isOpacity = false;
	}
}


bool ProgressTimer::init(const std::string& file)
{
	if (!Widget::init())
	{
		return false;
	}
	
	auto _fullPath = TINA_MGR->getImageFullPath(file);
	auto _sprite = Sprite::create(_fullPath);
	if (_sprite)this->Node::setContentSize(_sprite->getContentSize());
	m_renderer = cocos2d::ProgressTimer::create(_sprite);
	{
		m_renderer->ignoreAnchorPointForPosition(true);
		Node::addChild(m_renderer);
	}
	//setSource(file);
	setProcessType(Type::RADIAL);
	return true;
}

void ProgressTimer::drawRenderer(bool updateSize)
{
	if (m_parsing)return;
	updatePercentage();
	updateSpriteAttr();
}
TINA_NS_END