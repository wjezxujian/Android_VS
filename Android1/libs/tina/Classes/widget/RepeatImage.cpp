#include "RepeatImage.h"
#include "util/ParamUtil.h"
#include "util/LayoutUtil.h"
#include "util/TinaMgr.h"

USING_NS_CC;

TINA_NS_BEGIN
RepeatImage* RepeatImage::create()
{
	return create("");
}

RepeatImage* RepeatImage::create(const std::string& file)
{
	auto _ref = new RepeatImage();
	if (_ref && _ref->init(file))
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

void RepeatImage::parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values)
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
		else if (_key == "texturerect" || _key == "rect")
		{
			setTextureRect(ParamUtil::readRect(_value));
		}
		else if (_key == "repeat" || _key == "repeatcnt")
		{
			setRepeatCnt(ParamUtil::readUInt(_value));
		}
		else if (_key == "direction")
		{
			setDirection(ParamUtil::readScrollDirection(_value));
		}
		else if (_key == "gap")
		{
			setGap(ParamUtil::readInt(_value));
		}
	}
}

void RepeatImage::setSource(const std::string& file)
{
	if (file.empty())return;

	std::string _fullPath = file;
	if (!m_useAbsPath && !file.empty())
		_fullPath = TINA_MGR->getImageFullPath(file);

	if (!m_renderer)
	{
		m_renderer = SpriteBatchNode::create(_fullPath);
		m_renderer->ignoreAnchorPointForPosition(true);
		Node::addChild(m_renderer);
	}
	else
	{
		m_renderer->removeAllChildren();
		m_renderer->setTexture(TextureCache::getInstance()->addImage(_fullPath));
	}
	DRAW_RENDERER;
}

void RepeatImage::setTextureRect(const cocos2d::Rect& rect)
{
	if (m_textureRect.equals(rect))return;
	m_textureRect = rect;
	DRAW_RENDERER;
}

void RepeatImage::setRepeatCnt(uint16_t cnt)
{
	if (cnt == m_repeatCnt)return;
	m_repeatCnt = cnt;
	DRAW_RENDERER;
}

void RepeatImage::setDirection(ScrollDirection direction)
{
	CCASSERT(direction == ScrollDirection::HORIZONTAL || direction == ScrollDirection::VERTICAL, "");
	if (direction == ScrollDirection::HORIZONTAL)
	{
		m_layoutParam.vAlign = VAlign::CENTER;
		m_layoutParam.hAlign = HAlign::LEFT;
		m_layoutParam.type = LayoutType::HORIZONTAL;
	}
	else if (direction == ScrollDirection::VERTICAL)
	{
		m_layoutParam.vAlign = VAlign::BOTTOM;
		m_layoutParam.hAlign = HAlign::CENTER;
		m_layoutParam.type = LayoutType::VERTICAL;
	}
	DRAW_RENDERER;
}

ScrollDirection RepeatImage::getDirection()
{
	if (m_layoutParam.type == LayoutType::HORIZONTAL)
	{
		return ScrollDirection::HORIZONTAL;
	}
	else
	{
		return ScrollDirection::VERTICAL;
	}
}

void RepeatImage::setGap(int gap)
{
	m_layoutParam.hGap = gap;
	m_layoutParam.vGap = gap;
	DRAW_RENDERER;
}

bool RepeatImage::init(const std::string& file)
{
	if (!Widget::init())
	{
		return false;
	}
	setType(WidgetType::REPEAT_IMAGE);
	setCascadeColorEnabled(true);
	m_renderer = nullptr;
	m_layoutParam.type = LayoutType::HORIZONTAL;
	m_layoutParam.hAlign = HAlign::LEFT;
	m_layoutParam.vAlign = VAlign::CENTER;
	setSource(file);
	return true;
}

void RepeatImage::drawRenderer(bool updateSize)
{
	if (m_parsing)return;
	m_renderer->removeAllChildren();
	m_renderer->setCascadeColorEnabled(true);
	m_renderer->setCascadeOpacityEnabled(true);
	for (uint16_t i = 0; i < m_repeatCnt; ++i)
	{
		auto _sprite = Sprite::createWithTexture(m_renderer->getTexture());
		_sprite->setCascadeColorEnabled(true);
		_sprite->setCascadeOpacityEnabled(true);
		m_renderer->addChild(_sprite);
	}
	Size _size;
	if (m_repeatCnt > 0)
		_size = LayoutUtil::layout(m_layoutParam, { -1, -1 }, m_renderer->getChildren());
	if (updateSize)
		updateSizeFromContent(_size);
}
TINA_NS_END