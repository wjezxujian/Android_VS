#include "VBox.h"
#include "util/ParamUtil.h"
#include "util/Layout.h"

USING_NS_CC;

TINA_NS_BEGIN
VBox* VBox::create()
{
	auto _ref = new VBox();
	if (_ref && _ref->init())
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

void VBox::parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values)
{
	BaseBox::parseAttr(keys, values);
}

bool VBox::init()
{
	if (!BaseBox::init(LayoutType::VERTICAL))
	{
		return false;
	}
	setType(WidgetType::VBOX);
	return true;
}
TINA_NS_END