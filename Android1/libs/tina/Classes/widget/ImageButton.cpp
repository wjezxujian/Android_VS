#include "ImageButton.h"
#include "Image.h"

USING_NS_CC;

TINA_NS_BEGIN
ImageButton* ImageButton::create()
{
	auto _ref = new ImageButton();
	if (_ref && _ref->init())
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

ImageButton* ImageButton::create(const std::string& srcUp, const std::string& srcDown, const std::string& srcDisabled)
{
	auto _ref = new ImageButton();
	if (_ref && _ref->initWithSrc(srcUp, srcDown, srcDisabled))
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

void ImageButton::parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values)
{
	Button::parseAttr(keys, values);
	for (auto i = 0; i < keys->size(); ++i)
	{
		const auto& _key = keys->at(i);
		const auto& _value = values->at(i);
		if (_key == "src" || _key == "source" || _key == "upsrc" || _key == "upsource" || _key == "srcup")
		{
			setSrc(_value);
		}
		else if (_key == "srcdown" || _key == "downsrc" || _key == "downsource" || _key == "downsource")
		{
			setSrcDown(_value);
		}
		else if (_key == "srcdisabled" || _key == "disabledsrc" || _key == "disabledsource" || _key == "disablesource" || _key == "disabledsource")
		{
			setSrcDisabled(_value);
		}
	}
}

void ImageButton::setSrc(const std::string& srcUp)
{
	if (m_srcUp == srcUp)return;
	m_srcUp = srcUp;
	DRAW_RENDERER;
}

void ImageButton::setSrcDown(const std::string& srcDown)
{
	if (m_srcDown == srcDown)return;
	m_srcDown = srcDown;
}

void ImageButton::setSrcDisabled(const std::string& srcDisabled)
{
	if (m_srcDisabled == srcDisabled)return;
	m_srcDisabled = srcDisabled;
}

bool ImageButton::init()
{
	if (!Button::initWithStates(Image::create()))
	{
		return false;
	}
	setScaleMode(true);
	return true;
}

bool ImageButton::initWithSrc(const std::string& srcUp, const std::string& srcDown, const std::string& srcDisabled)
{
	m_srcUp = srcUp;
	m_srcDown = srcDown;
	m_srcDisabled = srcDisabled;
	auto _upState = Image::create(m_srcUp);
	{
		_upState->ignoreAnchorPointForPosition(true);
	}
	if (!Button::initWithStates(_upState))
	{
		return false;
	}
	setScaleMode(true);
	return true;
}

void ImageButton::onFocus()
{
	Button::onFocus();
	auto _upState = dynamic_cast<Image*>(m_upState);
	if (_upState != nullptr && !m_srcDown.empty())
		_upState->setSource(m_srcDown);
}

void ImageButton::onBlur()
{
	Button::onBlur();
	auto _upState = dynamic_cast<Image*>(m_upState);
	if (_upState != nullptr && !m_srcUp.empty())
		_upState->setSource(m_srcUp);
}

void ImageButton::onDisabled()
{
	Button::onDisabled();
	auto _upState = dynamic_cast<Image*>(m_upState);
	if (_upState != nullptr && !m_srcDisabled.empty())
		_upState->setSource(m_srcDisabled);
}

void ImageButton::drawRenderer(bool updateSize)
{
	Size _size;
	if (!m_enabled && !m_srcDisabled.empty())
		((Image*)m_upState)->setSource(m_srcDisabled);
	else
		((Image*)m_upState)->setSource(m_srcUp);

	if (m_upState)
		_size = m_upState->getContentSize();
	if (m_givenSize.width >= 0)
		_size.width = m_givenSize.width;
	if (m_givenSize.height >= 0)
		_size.height = m_givenSize.height;

	if (m_givenSize.width >= 0 || m_givenSize.width >= 0)
	{
		if (m_upState)
			m_upState->updateSizeFromParent(_size);
		if (m_downState)
			m_downState->updateSizeFromParent(_size);
		if (m_disabledState)
			m_disabledState->updateSizeFromParent(_size);
	}

	if (updateSize)
		updateSizeFromContent(_size);
}
TINA_NS_END