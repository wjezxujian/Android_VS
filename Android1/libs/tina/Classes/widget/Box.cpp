#include "Box.h"
#include "util/Layout.h"


USING_NS_CC;

TINA_NS_BEGIN
Box* Box::create()
{
	auto _ref = new Box();
	if (_ref && _ref->init())
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

void Box::parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values)
{
	BaseBox::parseAttr(keys, values);
}

bool Box::init()
{
	if (!BaseBox::init(LayoutType::ABSOLUTE))
	{
		return false;
	}
	setType(WidgetType::BOX);
	return true;
}
TINA_NS_END