#include "ParamUtil.h"
#include "util/StringUtil.h"

USING_NS_CC;

TINA_NS_BEGIN
float ParamUtil::readFloat(const std::string& str)
{
	
	float _num;
	std::string _str = str;
	StringUtil::toLower(&_str);
	StringUtil::eraseWhiteSpaces(&_str);
	std::vector<std::string> _container;
	std::string _symbol = "";

	if ((int)_str.find('*') > -1)
	{
		_symbol = "*";
		StringUtil::split(_str, _symbol, &_container);
	}
	else if ((int)_str.find('/') > -1)
	{
		_symbol = "/";
		StringUtil::split(_str, _symbol, &_container);
	}
	else if ((int)_str.find('+') > -1)
	{
		_symbol = "+";
		StringUtil::split(_str, _symbol, &_container);
	}
	else if ((int)_str.find('-') > -1)
	{
		_symbol = "-";
		StringUtil::split(_str, _symbol, &_container);
	}

	if (_container.size() > 0)
	{
		float _num1 = converStr2Float(_container.front());
		float _num2 = converStr2Float(_container.back());
		if (_symbol == "*")
		{
			_num = _num1 * _num2;
		}
		else if(_symbol == "/")
		{
			_num = _num1 / _num2;
		}
		else if (_symbol == "+")
		{
			_num = _num1 + _num2;
		}
		else if (_symbol == "-")
		{
			_num = _num1 - _num2;
		}
	}
	else
	{
		_num = converStr2Float(_str);
	}

	return _num;
}

int ParamUtil::readInt(const std::string& str)
{
	std::string _str = str;
	StringUtil::eraseWhiteSpaces(&_str);
	return atoi(_str.c_str());
}

uint32_t ParamUtil::readUInt(const std::string& str)
{
	std::string _str = str;
	StringUtil::eraseWhiteSpaces(&_str);
	int _num = atoi(_str.c_str());
	return MAX(0, _num);
}

float ParamUtil::readPercent(const std::string& str)
{
	std::string _str = str;
	StringUtil::eraseWhiteSpaces(&_str);
	if (_str.back() == '%')
		_str = _str.substr(0, _str.size() - 1);
	return atof(_str.c_str());
}

bool ParamUtil::readBool(const std::string& str)
{
	std::string _str = str;
	StringUtil::toLower(&_str);
	StringUtil::eraseWhiteSpaces(&_str);
	if (str == "yes" || str == "true" || str == "1")
		return true;
	else
		return false;
}

Rect ParamUtil::readRect(const std::string& str)
{
	std::string _str = str;
	StringUtil::eraseWhiteSpaces(&_str);
	std::vector<std::string> _container;
	StringUtil::split(_str, ",", &_container);
	if (_container.size() >= 4)
	{
		return Rect(atof(_container.at(0).c_str()), atof(_container.at(1).c_str()), atof(_container.at(2).c_str()), atof(_container.at(3).c_str()));
	}
	return Rect::ZERO;
}

Size ParamUtil::readSize(const std::string& str)
{
	std::string _str = str;
	StringUtil::eraseWhiteSpaces(&_str);
	std::vector<std::string> _container;
	StringUtil::split(_str, ",", &_container);
	if (_container.size() >= 2)
	{
		return Size(atof(_container.at(0).c_str()), atof(_container.at(1).c_str()));
	}
	return Size::ZERO;
}

Vec2 ParamUtil::readVec2(const std::string& str)
{
	std::string _str = str;
	StringUtil::eraseWhiteSpaces(&_str);
	std::vector<std::string> _container;
	StringUtil::split(_str, ",", &_container);
	if (_container.size() >= 2)
	{
		return Vec2(atof(_container.at(0).c_str()), atof(_container.at(1).c_str()));
	}
	return Vec2::ZERO;
}

Color3B ParamUtil::readColor3B(const std::string& str)
{
	std::string _str = str;
	StringUtil::toLower(&_str);
	StringUtil::eraseWhiteSpaces(&_str);
	if (_str == "white")
	{
		return Color3B::WHITE;
	}
	else if (_str == "yellow")
	{
		return Color3B::YELLOW;
	}
	else if (_str == "blue")
	{
		return Color3B::BLUE;
	}
	else if (_str == "green")
	{
		return Color3B::GREEN;
	}
	else if (_str == "red")
	{
		return Color3B::RED;
	}
	else if (_str == "magenta")
	{
		return Color3B::MAGENTA;
	}
	else if (_str == "black")
	{
		return Color3B::BLACK;
	}
	else if (_str == "orange")
	{
		return Color3B::ORANGE;
	}
	else if (_str == "gray" || _str == "grey")
	{
		return Color3B::GRAY;
	}
	return Color3B(std::strtol(_str.substr(1, 2).c_str(), nullptr, 16), std::strtol(_str.substr(3, 2).c_str(), nullptr, 16), std::strtol(_str.substr(5, 2).c_str(), nullptr, 16));
}

Color4B ParamUtil::readColor4B(const std::string& str)
{
	std::string _str = str;
	StringUtil::toLower(&_str);
	StringUtil::eraseWhiteSpaces(&_str);
	if (_str == "white")
	{
		return Color4B::WHITE;
	}
	else if (_str == "yellow")
	{
		return Color4B::YELLOW;
	}
	else if (_str == "blue")
	{
		return Color4B::BLUE;
	}
	else if (_str == "green")
	{
		return Color4B::GREEN;
	}
	else if (_str == "red")
	{
		return Color4B::RED;
	}
	else if (_str == "magenta")
	{
		return Color4B::MAGENTA;
	}
	else if (_str == "black")
	{
		return Color4B::BLACK;
	}
	else if (_str == "orange")
	{
		return Color4B::ORANGE;
	}
	else if (_str == "gray" || _str == "grey")
	{
		return Color4B::GRAY;
	}
	if (_str.size() == 7)
	{
		return Color4B(std::strtol(_str.substr(1, 2).c_str(), nullptr, 16), std::strtol(_str.substr(3, 2).c_str(), nullptr, 16), std::strtol(_str.substr(5, 2).c_str(), nullptr, 16), 255);
	}
	else if (_str.size() == 9)
	{
		return Color4B(std::strtol(_str.substr(1, 2).c_str(), nullptr, 16), std::strtol(_str.substr(3, 2).c_str(), nullptr, 16), std::strtol(_str.substr(5, 2).c_str(), nullptr, 16), std::strtol(_str.substr(7, 2).c_str(), nullptr, 16));
	}
	return Color4B::BLACK;
}

VAlign ParamUtil::readValign(const std::string& str)
{
	std::string _str = str;
	StringUtil::toLower(&_str);
	StringUtil::eraseWhiteSpaces(&_str);
	VAlign _align = VAlign::BOTTOM;
	if (_str == "top")
	{
		_align = VAlign::TOP;
	}
	else if (_str == "middle" || _str == "center")
	{
		_align = VAlign::CENTER;
	}
	else if (_str == "bottom")
	{
		_align = VAlign::BOTTOM;
	}
	return _align;
}

HAlign ParamUtil::readHalign(const std::string& str)
{
	std::string _str = str;
	StringUtil::toLower(&_str);
	StringUtil::eraseWhiteSpaces(&_str);
	HAlign _align = HAlign::LEFT;
	if (_str == "left")
	{
		_align = HAlign::LEFT;
	}
	else if (_str == "middle" || _str == "center")
	{
		_align = HAlign::CENTER;
	}
	else if (_str == "right")
	{
		_align = HAlign::RIGHT;
	}
	return _align;
}

TextHAlignment ParamUtil::readTextHalign(const std::string& str)
{
	std::string _str = str;
	StringUtil::toLower(&_str);
	StringUtil::eraseWhiteSpaces(&_str);
	TextHAlignment _align = TextHAlignment::LEFT;
	if (_str == "left")
	{
		_align = TextHAlignment::LEFT;
	}
	else if (_str == "middle" || _str == "center")
	{
		_align = TextHAlignment::CENTER;
	}
	else if (_str == "right")
	{
		_align = TextHAlignment::RIGHT;
	}
	return _align;
}

TextVAlignment ParamUtil::readTextValign(const std::string& str)
{
	std::string _str = str;
	StringUtil::toLower(&_str);
	StringUtil::eraseWhiteSpaces(&_str);
	cocos2d::TextVAlignment _align = cocos2d::TextVAlignment::BOTTOM;
	if (_str == "top")
	{
		_align = TextVAlignment::TOP;
	}
	else if (_str == "middle" || _str == "center")
	{
		_align = TextVAlignment::CENTER;
	}
	else if (_str == "bottom")
	{
		_align = TextVAlignment::BOTTOM;
	}
	return _align;
}

ScrollDirection ParamUtil::readScrollDirection(const std::string& str)
{
	std::string _str = str;
	StringUtil::toLower(&_str);
	StringUtil::eraseWhiteSpaces(&_str);
	ScrollDirection _direction = ScrollDirection::BOTH;
	if (_str == "horizontal" || _str == "h")
	{
		_direction = ScrollDirection::HORIZONTAL;
	}
	else if (_str == "vertical" || _str == "v")
	{
		_direction = ScrollDirection::VERTICAL;
	}
	else if (_str == "auto")
	{
		_direction = ScrollDirection::AUTO;
	}
	else
	{
		_direction = ScrollDirection::BOTH;
	}
	return _direction;
}

LayoutType ParamUtil::readLayoutType(const std::string& str)
{
	std::string _str = str;
	StringUtil::toLower(&_str);
	StringUtil::eraseWhiteSpaces(&_str);
	LayoutType _layoutType = LayoutType::VERTICAL;
	if (_str == "horizontal" || _str == "h")
	{
		_layoutType = LayoutType::HORIZONTAL;
	}
	else if (_str == "vertical" || _str == "v")
	{
		_layoutType = LayoutType::VERTICAL;
	}
	else if (_str == "tile")
	{
		_layoutType = LayoutType::TILE;
	}
	else if (_str == "absolute")
	{
		_layoutType = LayoutType::ABSOLUTE;
	}
	return _layoutType;
}

void ParamUtil::readStringList(const std::string& str, std::vector<std::string>* container)
{
	std::string _str = str;
	StringUtil::trim(&_str);
	StringUtil::split(str, ",", container);
}

float ParamUtil::converStr2Float(const std::string& str)
{
	std::string _str = str;
	StringUtil::toLower(&_str);
	StringUtil::eraseWhiteSpaces(&_str);
	const auto& _winsize = Director::getInstance()->getWinSize();
	if (_str == "winwidth")
	{
		return _winsize.width;
	}
	else if (_str == "winheight")
	{
		return _winsize.height;
	}
	return atof(_str.c_str());
}
TINA_NS_END