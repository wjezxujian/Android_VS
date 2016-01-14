#include "ListView.h"
#include "util/ParamUtil.h"
#include "base/BaseBox.h"

USING_NS_CC;

TINA_NS_BEGIN
#define DEFAULT_WIDTH (100)
#define DEFAULT_HEIGHT (100)

ListView* ListView::create()
{
	auto _ref = new ListView();
	if (_ref && _ref->init(Size(DEFAULT_WIDTH, DEFAULT_HEIGHT)))
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

void ListView::parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values)
{
	Widget::parseAttr(keys, values);
	for (auto i = 0; i < keys->size(); ++i)
	{
		const auto& _key = keys->at(i);
		const auto& _value = values->at(i);
		if (_key == "layouttype" || _key == "layout")
		{
			setLayoutType(ParamUtil::readLayoutType(_value));
		}
		else if (_key == "gap")
		{
			setGap(ParamUtil::readUInt(_value));
		}
		else if (_key == "gap")
		{
			setGap(ParamUtil::readUInt(_value));
		}
		else if (_key == "valign")
		{
			setVAlign(ParamUtil::readValign(_value));
		}
		else if (_key == "halign" || _key == "align")
		{
			setHAlign(ParamUtil::readHalign(_value));
		}
		else if (_key == "paddingleft")
		{
			setPaddingLeft(ParamUtil::readUInt(_value));
		}
		else if (_key == "paddingright")
		{
			setPaddingRight(ParamUtil::readUInt(_value));
		}
		else if (_key == "paddingtop")
		{
			setPaddingTop(ParamUtil::readUInt(_value));
		}
		else if (_key == "paddingbottom")
		{
			setPaddingBottom(ParamUtil::readUInt(_value));
		}
	}
}

void ListView::setLayoutType(LayoutType type)
{
	if (m_layoutType == type)return;
	m_layoutType = type;
	m_container->setLayoutType(type);
	if (type == LayoutType::HORIZONTAL)
	{
		m_container->ignoreWidth();
		m_container->setHeight(_contentSize.height);
		setScrollDirection(ScrollDirection::HORIZONTAL);
	}
	else if (type == LayoutType::VERTICAL || type == LayoutType::TILE)
	{
		m_container->ignoreHeight();
		m_container->setWidth(_contentSize.width);
		setScrollDirection(ScrollDirection::VERTICAL);
	}
	else
	{
		m_container->ignoreWidth();
		m_container->ignoreHeight();
		setScrollDirection(ScrollDirection::BOTH);
	}
}

LayoutType ListView::getLayoutType()
{
	return m_layoutType;
}

void ListView::setVAlign(VAlign align)
{
	m_container->setVAlign(align);
}

VAlign ListView::getVAlign()
{
	return m_container->getVAlign();
}

void ListView::setHAlign(HAlign align)
{
	m_container->setHAlign(align);
}

HAlign ListView::getHAlign()
{
	return m_container->getHAlign();
}

void ListView::setGap(uint16_t gap)
{
	m_container->setGap(gap);
}

uint16_t ListView::getGap()
{
	return m_container->getHGap();
}

void ListView::setHGap(uint16_t gap)
{
	m_container->setHGap(gap);
}

uint16_t ListView::getHGap()
{
	return m_container->getHGap();
}

void ListView::setVGap(uint16_t gap)
{
	m_container->setVGap(gap);
}

uint16_t ListView::getVGap()
{
	return m_container->getVGap();
}

void ListView::setPaddingLeft(uint16_t paddingLeft)
{
	m_container->setPaddingLeft(paddingLeft);
}

uint16_t ListView::getPaddingLeft()
{
	return m_container->getPaddingLeft();
}

void ListView::setPaddingRight(uint16_t paddingRight)
{
	m_container->setPaddingRight(paddingRight);
}

uint16_t ListView::getPaddingRight()
{
	return m_container->getPaddingRight();
}

void ListView::setPaddingTop(uint16_t paddingTop)
{
	m_container->setPaddingTop(paddingTop);
}

uint16_t ListView::getPaddingTop()
{
	return m_container->getPaddingTop();
}

void ListView::setPaddingBottom(uint16_t paddingBottom)
{
	m_container->setPaddingBottom(paddingBottom);
}

uint16_t ListView::getPaddingBottom()
{
	return m_container->getPaddingBottom();
}

bool ListView::init(const cocos2d::Size& viewSize)
{
	if (!ScrollView::init(viewSize))
	{
		return false;
	}
	setType(WidgetType::LISTVIEW);
	setLayoutType(LayoutType::VERTICAL);
	return true;
}

void ListView::updateSizeFromParent(const cocos2d::Size& virtualSize)
{
	ScrollView::updateSizeFromParent(virtualSize);
	if (m_layoutType == LayoutType::VERTICAL || m_layoutType == LayoutType::TILE)
	{
		m_container->setWidth(_contentSize.width);
	}
	else if (m_layoutType == LayoutType::HORIZONTAL)
	{
		m_container->setHeight(_contentSize.height);
	}
}

void ListView::updateSize(const cocos2d::Size& contentSize)
{
	ScrollView::updateSize(contentSize);
	if (m_layoutType == LayoutType::VERTICAL || m_layoutType == LayoutType::TILE)
	{
		m_container->setWidth(_contentSize.width);
	}
	else if (m_layoutType == LayoutType::HORIZONTAL)
	{
		m_container->setHeight(_contentSize.height);
	}
}
TINA_NS_END