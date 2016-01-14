#include "HBox.h"
#include "util/ParamUtil.h"

USING_NS_CC;

TINA_NS_BEGIN

HBox* HBox::create()
{
	auto _ref = new HBox();
	if (_ref && _ref->init())
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

void HBox::parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values)
{
	BaseBox::parseAttr(keys, values);
}

bool HBox::init()
{
	if (!BaseBox::init(LayoutType::HORIZONTAL))
	{
		return false;
	}
	setType(WidgetType::HBOX);
	return true;
}
TINA_NS_END