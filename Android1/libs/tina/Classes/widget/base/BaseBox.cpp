#include "BaseBox.h"
#include "widget/util/ParamUtil.h"
#include "widget/util/Layout.h"
#include "util/TinaMgr.h"

USING_NS_CC;

TINA_NS_BEGIN

BaseBox* BaseBox::create()
{
	auto _ref = new BaseBox();
	if (_ref && _ref->init(LayoutType::ABSOLUTE))
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

void BaseBox::parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values)
{
	Widget::parseAttr(keys, values);
	for (auto i = 0; i < keys->size(); ++i)
	{
		const auto& _key = keys->at(i);
		const auto& _value = values->at(i);
		if (_key == "gap")
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

//////////////////////////////////////////////////////////
void BaseBox::setVAlign(VAlign align)
{
	if (m_layout->getVAlign() == align)return;
	m_layout->setVAlign(align);
	DRAW_RENDERER;
}

VAlign BaseBox::getVAlign()
{
	return m_layout->getVAlign();
}

void BaseBox::setHAlign(HAlign align)
{
	if (m_layout->getHAlign() == align)return;
	m_layout->setHAlign(align);
	DRAW_RENDERER;
}

HAlign BaseBox::getHAlign()
{
	return m_layout->getHAlign();
}

void BaseBox::setGap(uint16_t gap)
{
	if (m_layout->getHGap() == gap)return;
	m_layout->setHGap(gap);
	m_layout->setVGap(gap);
	DRAW_RENDERER;
}

uint16_t BaseBox::getGap()
{
	return m_layout->getHGap();
}

void BaseBox::setHGap(uint16_t gap)
{
	if (m_layout->getHGap() == gap)return;
	m_layout->setHGap(gap);
	DRAW_RENDERER;
}

uint16_t BaseBox::getHGap()
{
	return m_layout->getHGap();
}

void BaseBox::setVGap(uint16_t gap)
{
	if (m_layout->getVGap() == gap)return;
	m_layout->setVGap(gap);
	DRAW_RENDERER;
}

uint16_t BaseBox::getVGap()
{
	return m_layout->getVGap();
}

void BaseBox::setPaddingLeft(uint16_t paddingLeft)
{
	if (m_layout->getPaddingLeft() == paddingLeft)return;
	m_layout->setPaddingLeft(paddingLeft);
	DRAW_RENDERER;
}

uint16_t BaseBox::getPaddingLeft()
{
	return m_layout->getPaddingLeft();
}

void BaseBox::setPaddingRight(uint16_t paddingRight)
{
	if (m_layout->getPaddingRight() == paddingRight)return;
	m_layout->setPaddingRight(paddingRight);
	DRAW_RENDERER;
}

uint16_t BaseBox::getPaddingRight()
{
	return m_layout->getPaddingRight();
}

void BaseBox::setPaddingTop(uint16_t paddingTop)
{
	if (m_layout->getPaddingTop() == paddingTop)return;
	m_layout->setPaddingTop(paddingTop);
	DRAW_RENDERER;
}

uint16_t BaseBox::getPaddingTop()
{
	return m_layout->getPaddingTop();
}

void BaseBox::setPaddingBottom(uint16_t paddingBottom)
{
	if (m_layout->getPaddingBottom() == paddingBottom)return;
	m_layout->setPaddingBottom(paddingBottom);
	DRAW_RENDERER;
}

uint16_t BaseBox::getPaddingBottom()
{
	return m_layout->getPaddingBottom();
}
//////////////////////////////////////////////////////////

BaseBox::~BaseBox()
{
	CC_SAFE_DELETE(m_layout);
}

bool BaseBox::init(LayoutType type)
{
	if (!Widget::init())
	{
		return false;
	}
	m_layout = new Layout;
	m_layout->setType(type);
	setType(WidgetType::BASEBOX);
	return true;
}

void BaseBox::setLayoutType(LayoutType type)
{
	if (m_layout->getType() == type)return;
	m_layout->setType(type);
	DRAW_RENDERER;
}

void BaseBox::relayoutByChildrenChanges()
{
	DRAW_RENDERER;
}

void BaseBox::drawRenderer(bool updateSize)
{
	if (!TinaMgr::getInstance()->getConfig()->getLayoutSwitch())
	{
		return;
	}
	m_layout->calcVirtualSize(m_widgetChildren, m_givenSize);
	for (auto child : m_widgetChildren)
	{
		Size _virtualSize = m_layout->getVirtualSize();
		if (m_layout->getType() == LayoutType::HORIZONTAL && child->getPercentWidth() != NaN && m_layout->getPercentSize().width > 100)
			_virtualSize.width *= 100.f / m_layout->getPercentSize().width;
		if (m_layout->getType() == LayoutType::VERTICAL && child->getPercentHeight() != NaN && m_layout->getPercentSize().height > 100)
			_virtualSize.height *= 100.f / m_layout->getPercentSize().height;

		child->updateSizeFromParent(_virtualSize);

		if (m_layout->getType() == LayoutType::ABSOLUTE)
			child->updateRelativePositionFromParent(m_layout->getVirtualSize());
	}

	auto _containerSize = m_layout->doLayout(m_widgetChildren, m_givenSize);

	if (updateSize)
		updateSizeFromContent(_containerSize);

	if (m_callfunc)
		m_callfunc(this);
}

TINA_NS_END