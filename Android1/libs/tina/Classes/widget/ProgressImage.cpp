#include "ProgressImage.h"
#include "util/ParamUtil.h"
#include "util/TinaMgr.h"

USING_NS_CC;

TINA_NS_BEGIN

ProgressImage* ProgressImage::create()
{
	return create("", Rect::ZERO, Rect::ZERO);
}

ProgressImage* ProgressImage::create(const std::string& file, const cocos2d::Size& size, const cocos2d::Rect& capInsets)
{
	auto _ref = create(capInsets, file);
	if (_ref)
		_ref->setContentSize(size);
	return _ref;
}

ProgressImage* ProgressImage::create(const std::string& file, const cocos2d::Size& size)
{
	auto _ref = create(file, Rect::ZERO, Rect::ZERO);
	if (_ref)
		_ref->setContentSize(size);
	return _ref;
}

ProgressImage* ProgressImage::create(const std::string& file, const cocos2d::Rect& rect, const cocos2d::Rect& capInsets)
{
	auto _ref = new ProgressImage();
	if (_ref && _ref->init(file, rect, capInsets))
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

ProgressImage* ProgressImage::create(const cocos2d::Rect& capInsets, const std::string& file)
{
	return create(file, Rect::ZERO, capInsets);
}

ProgressImage* ProgressImage::create(const std::string& file, const cocos2d::Rect& rect)
{
	return create(file, rect, Rect::ZERO);
}

void ProgressImage::parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values)
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
		else if (_key == "percent")
		{
			setPercent(ParamUtil::readFloat(_value));
		}
	}
}

void ProgressImage::setSource(const std::string& src, const cocos2d::Rect& capInset)
{
	if (m_source == src)return;
	m_source = src;
	std::string _fullPath = m_source;
	if (!m_useAbsPath)
		_fullPath = TINA_MGR->getImageFullPath(_fullPath);
	m_img->updateWithSprite(Sprite::create(_fullPath), Rect::ZERO, false, capInset);
	DRAW_RENDERER;
}

void ProgressImage::setPercent(float percent)
{
	if (m_percent == percent)return;
	m_percent = MIN(MAX(percent, 0), 100.f);
	DRAW_RENDERER;
}

bool ProgressImage::init(const std::string& file, const cocos2d::Rect& rect, const cocos2d::Rect& capInsets)
{
	if (!Widget::init())
	{
		return false;
	}
    m_source = file;
    std::string _fullPath = m_source;
    if (!m_useAbsPath)
        _fullPath = TINA_MGR->getImageFullPath(_fullPath);
	setType(WidgetType::PROGRESS_IMAGE);
	m_renderer = ClippingNode::create();
	{
		m_stencil = DrawNode::create();
		m_stencil->ignoreAnchorPointForPosition(true);
		m_renderer->setStencil(m_stencil);

		m_renderer->ignoreAnchorPointForPosition(true);
		//m_renderer->setAlphaThreshold(0.5);
		Node::addChild(m_renderer);
	}

	if (file.empty())
		m_img = ui::Scale9Sprite::create();
	else
	{
		if (!rect.equals(Rect::ZERO) && !capInsets.equals(Rect::ZERO))
			m_img = ui::Scale9Sprite::create(_fullPath, rect, capInsets);
		else if (!rect.equals(Rect::ZERO) && capInsets.equals(Rect::ZERO))
			m_img = ui::Scale9Sprite::create(_fullPath, rect);
		else if (rect.equals(Rect::ZERO) && !capInsets.equals(Rect::ZERO))
			m_img = ui::Scale9Sprite::create(capInsets, _fullPath);
		else
			m_img = ui::Scale9Sprite::create(_fullPath);
	}
	{
		m_img->ignoreAnchorPointForPosition(true);
		m_renderer->addChild(m_img);
	}

	m_useAbsPath = false;
	m_percent = 100.f;

	return true;
}

void ProgressImage::drawRenderer(bool updateSize)
{
	if (m_parsing)return;
	Size _rendererSize = m_img->getContentSize();
	Size _size;
	_size.width = m_givenSize.width < 0 ? _rendererSize.width : m_givenSize.width;
	_size.height = m_givenSize.height < 0 ? _rendererSize.height : m_givenSize.height;
	m_img->setContentSize(_size);
	m_renderer->setContentSize(_size);
	m_stencil->clear();
	m_stencil->drawSolidRect(Vec2::ZERO, Vec2(_size.width * (m_percent / 100.f), _size.height), Color4F::WHITE);
	if (updateSize)
		updateSizeFromContent(_size);
}

TINA_NS_END