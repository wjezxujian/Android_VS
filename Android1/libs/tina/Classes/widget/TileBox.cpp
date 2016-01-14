#include "TileBox.h"
#include "util/ParamUtil.h"
#include "util/Layout.h"

USING_NS_CC;

TINA_NS_BEGIN
TileBox* TileBox::create()
{
	auto _ref = new TileBox();
	if (_ref && _ref->init())
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

void TileBox::parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values)
{
	BaseBox::parseAttr(keys, values);
	for (auto i = 0; i < keys->size(); ++i)
	{
		const auto& _key = keys->at(i);
		const auto& _value = values->at(i);
		if (_key == "hgap")
		{
			setHGap(ParamUtil::readUInt(_value));
		}
		else if (_key == "vgap")
		{
			setVGap(ParamUtil::readUInt(_value));
		}
	}
}

bool TileBox::init()
{
	if (!BaseBox::init(LayoutType::TILE))
	{
		return false;
	}
	setType(WidgetType::TILEBOX);
	return true;
}
TINA_NS_END