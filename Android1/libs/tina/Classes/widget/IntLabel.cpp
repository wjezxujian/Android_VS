#include "IntLabel.h"
#include "util/ParamUtil.h"
#include "util/StringUtil.h"

TINA_NS_BEGIN
IntLabel* IntLabel::create()
{
	auto _ref = new IntLabel();
	if (_ref && _ref->init())
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

IntLabel* IntLabel::create(int num, const std::string& fontName, int fontSize)
{
	auto _ref = new IntLabel();
	if (_ref && _ref->init(num, fontName, fontSize))
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

void IntLabel::parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values)
{
	Label::parseAttr(keys, values);
	for (auto i = 0; i < keys->size(); ++i)
	{
		const auto& _key = keys->at(i);
		const auto& _value = values->at(i);
		if (_key == "duration")
		{
			setDuration(ParamUtil::readFloat(_value));
		}
	}
}

void IntLabel::setNumber(int number)
{
	if (this->isScheduled(schedule_selector(IntLabel::exeSetNumber)))
	{
		this->unschedule(schedule_selector(IntLabel::exeSetNumber));
		std::ostringstream _str;
		_str << m_newNum;
		setString(_str.str());
	}
	m_dt = 0;
	m_dtMax = m_duration;
	m_originNum = atoi(m_renderer->getString().c_str());
	m_newNum = number;
	this->schedule(schedule_selector(IntLabel::exeSetNumber));
}

int IntLabel::getNumber()
{
	return atoi(m_renderer->getString().c_str());
}

bool IntLabel::init(int number, const std::string& fontName, int fontSize)
{
	if (!Label::init(STRING_FORMAT_INT(number), fontName, fontSize))
	{
		return false;
	}
	setType(WidgetType::INT_LABEL);
	return true;
}

bool IntLabel::init()
{
	if (!Label::init())
	{
		return false;
	}
	setType(WidgetType::INT_LABEL);
	return true;
}

void IntLabel::exeSetNumber(float dt)
{
	m_dt += dt;
	float _step = MIN(m_dt / m_dtMax, 1.f);
	int _num = (m_newNum - m_originNum) * _step + m_originNum;
	std::ostringstream _str;
	_str << _num;
	setString(_str.str());
	if (_step == 1.f)
	{
		this->unschedule(schedule_selector(IntLabel::exeSetNumber));
	}
}
TINA_NS_END