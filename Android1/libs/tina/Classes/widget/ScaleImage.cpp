#include "ScaleImage.h"
#include "util/ParamUtil.h"
#include "util/TinaMgr.h"
#include "util/StringUtil.h"

USING_NS_CC;

TINA_NS_BEGIN

ScaleImage* ScaleImage::create()
{
	return create("", Rect::ZERO, Rect::ZERO);
}

ScaleImage* ScaleImage::create(const std::string& file, const cocos2d::Size& size, const cocos2d::Rect& capInsets)
{
	auto _ref = create(capInsets, file);
	if (_ref)
		_ref->setContentSize(size);
	return _ref;
}

ScaleImage* ScaleImage::create(const std::string& file, const cocos2d::Size& size)
{
	auto _ref = create(file, Rect::ZERO, Rect::ZERO);
	if (_ref)
		_ref->setContentSize(size);
	return _ref;
}

ScaleImage* ScaleImage::create(const std::string& file, const cocos2d::Rect& rect, const cocos2d::Rect& capInsets)
{
	auto _ref = new ScaleImage();
	if (_ref && _ref->init(file, rect, capInsets))
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

ScaleImage* ScaleImage::create(const cocos2d::Rect& capInsets, const std::string& file)
{
	return create(file, Rect::ZERO, capInsets);
}

ScaleImage* ScaleImage::create(const std::string& file, const cocos2d::Rect& rect)
{
	return create(file, rect, Rect::ZERO);
}

void ScaleImage::parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values)
{
	Widget::parseAttr(keys, values);
	for (auto i = 0; i < keys->size(); ++i)
	{
		const auto& _key = keys->at(i);
		const auto& _value = values->at(i);
		if (_key == "source" || _key == "src")
		{
			auto _it = std::find(keys->begin(), keys->end(), "capinsets");
			auto _capInsets = Rect::ZERO;
			if (_it != keys->end())
			{
				_capInsets = ParamUtil::readRect(*(values->begin() + (_it - keys->begin())));
			}
			setSource(_value, _capInsets);
		}
	}
}

void ScaleImage::setSource(const std::string& src, const cocos2d::Rect& capInset)
{
	if (m_source == src)return;
	m_source = src;
	std::string _fullPath = m_source;
	if (!m_useAbsPath)
		_fullPath = TINA_MGR->getImageFullPath(_fullPath);
	m_renderer->updateWithSprite(Sprite::create(_fullPath), Rect::ZERO, false, capInset);
	DRAW_RENDERER;
}

void ScaleImage::setSource(const std::string& src)
{
	setSource(src, Rect::ZERO);
}

void ScaleImage::setSourceWithFormat(const cocos2d::Rect& capInset, const char* format, ...)
{
	std::string _file;
	va_list _ap;
	va_start(_ap, format);
	_file = StringUtil::formatVa(format, _ap);
	va_end(_ap);
	setSource(_file, capInset);
}

void ScaleImage::setSourceWithFormat(const char* format, ...)
{
	std::string _file;
	va_list _ap;
	va_start(_ap, format);
	_file = StringUtil::formatVa(format, _ap);
	va_end(_ap);
	setSource(_file, Rect::ZERO);
}

const cocos2d::Size& ScaleImage::getRenderContentSize()
{
	return m_renderer->getContentSize();
}

bool ScaleImage::init(const std::string& file, const cocos2d::Rect& rect, const cocos2d::Rect& capInsets)
{
	if (!Widget::init())
	{
		return false;
	}
    m_source = file;
    std::string _fullPath = m_source;
    if (!m_useAbsPath)
        _fullPath = TINA_MGR->getImageFullPath(_fullPath);

	setType(WidgetType::SCALE_IMAGE);
	if (file.empty())
		m_renderer = ui::Scale9Sprite::create();
	else
	{
		if (!rect.equals(Rect::ZERO) && !capInsets.equals(Rect::ZERO))
			m_renderer = ui::Scale9Sprite::create(_fullPath, rect, capInsets);
		else if (!rect.equals(Rect::ZERO) && capInsets.equals(Rect::ZERO))
			m_renderer = ui::Scale9Sprite::create(_fullPath, rect);
		else if (rect.equals(Rect::ZERO) && !capInsets.equals(Rect::ZERO))
			m_renderer = ui::Scale9Sprite::create(capInsets, _fullPath);
		else
			m_renderer = ui::Scale9Sprite::create(_fullPath);
	}
	{
		m_renderer->ignoreAnchorPointForPosition(true);
		m_renderer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		Node::addChild(m_renderer);
	}
	return true;
}

void ScaleImage::drawRenderer(bool updateSize)
{
	if (m_parsing)return;
	Size _rendererSize = m_renderer->getContentSize();
	Size _size;
	_size.width = m_givenSize.width < 0 ? _rendererSize.width : m_givenSize.width;
	_size.height = m_givenSize.height < 0 ? _rendererSize.height : m_givenSize.height;
	m_renderer->setContentSize(_size);
	if (updateSize)
		updateSizeFromContent(_size);
}
TINA_NS_END