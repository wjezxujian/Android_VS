#include "Image.h"
#include "util/ParamUtil.h"
#include "util/TinaMgr.h"
#include "util/StringUtil.h"

USING_NS_CC;

TINA_NS_BEGIN

Image* Image::create()
{
	return create("");
}

Image* Image::create(const std::string& file)
{
	auto _ref = new Image();
	if (_ref && _ref->init(file))
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

Image* Image::createWithFormat(const char* format, ...)
{
	std::string _file;
	va_list _ap;
	va_start(_ap, format);
	_file = StringUtil::formatVa(format, _ap);
	va_end(_ap);
	return create(_file);
}

cocos2d::Sprite* Image::createSprite(const std::string& file)
{
	auto _sprite = Sprite::create(TINA_MGR->getImageFullPath(file));
	return _sprite;
}

void Image::parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values)
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
			auto _rect = ParamUtil::readRect(_value);
			m_renderer->setTextureRect(_rect);
		}
		else if (_key == "repeattype")
		{
			RepeatType _repeatType = RepeatType::SCALE;
			if (_value == "pattern")
				_repeatType = RepeatType::PATTERN;
			setRepeatType(_repeatType);
		}
		else if (_key == "filter")
		{
			FilterType _filter = FilterType::NONE;
			if (_value == "gray")
			{
				_filter = FilterType::GRAY;
				
			}
			else if (_value == "blur")
			{
				_filter = FilterType::BLUR;
			}
			setFilter(_filter);
		}
		else if (_key == "graypercent")
		{
			setGrayPercent(tina::ParamUtil::readFloat(_value));
		}
		else if (_key == "blurradius")
		{
			setBlurRadius(tina::ParamUtil::readFloat(_value));
		}
		else if (_key == "blursamplenum")
		{
			setBlurSampleNum(tina::ParamUtil::readFloat(_value));
		}
	}
}

void Image::setSource(const std::string& file)
{
	if (file.empty())return;
	if (m_source == file)return;
	m_source = file;
	std::string _fullPath = m_source;
	if (!m_useAbsPath)
		_fullPath = TINA_MGR->getImageFullPath(_fullPath);
	auto _lastTextureFormat = Texture2D::getDefaultAlphaPixelFormat();
	Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGBA8888);
	//cocos2d::log("Image::setSource: %s", _fullPath.c_str());
	m_renderer->setTexture(_fullPath);
	Texture2D::setDefaultAlphaPixelFormat(_lastTextureFormat);
	DRAW_RENDERER;
}

void Image::setSourceWithFormat(const char* format, ...)
{
	std::string _file;
	va_list _ap;
	va_start(_ap, format);
	_file = StringUtil::formatVa(format, _ap);
	va_end(_ap);
	setSource(_file);
}

void Image::setResSource(const std::string& file)
{
	if (file.empty())return;
	if (m_source == file)return;
	std::string _fullPath = file;
	if (!m_useAbsPath)
		_fullPath = TINA_MGR->getResFullPath(_fullPath);
	m_renderer->setTexture(_fullPath);
	DRAW_RENDERER;
}

void Image::setResSourceWithFormat(const char* format, ...)
{
	std::string _file;
	va_list _ap;
	va_start(_ap, format);
	_file = StringUtil::formatVa(format, _ap);
	va_end(_ap);
	setResSource(_file);
}

void Image::setTextureRect(const cocos2d::Rect& rect)
{
	m_renderer->setTextureRect(rect);
	DRAW_RENDERER;
}

void Image::setRepeatType(RepeatType type)
{
	if (m_repeatType == type)return;
	m_repeatType = type;
	DRAW_RENDERER;
}

void Image::setColor(const cocos2d::Color3B& color)
{
	if (m_renderer)
		m_renderer->setColor(color);
}

cocos2d::Texture2D* Image::getTexture()
{
	if (!m_renderer)return nullptr;
	return m_renderer->getTexture();
}

void Image::setFilter(FilterType type)
{
	if (m_filter == type)return;
	m_filter = type;
	DRAW_RENDERER;
}

void Image::setGrayPercent(float grayPercent)
{
	if (m_grayPercent == grayPercent)return;
	m_grayPercent = grayPercent; 
	DRAW_RENDERER;
}

void Image::setBlurRadius(float blurRadius)
{ 
	if (m_blurRadius == blurRadius)return;
	m_blurRadius = blurRadius;
	DRAW_RENDERER;
}

void Image::setBlurSampleNum(float blurSampleNum)
{ 
	if (m_blurSampleNum == blurSampleNum)return;
	m_blurSampleNum = blurSampleNum;
	DRAW_RENDERER;
}

void Image::setRadialBlurStrength(int strength)
{
	if (strength == m_uRadialBlurStrength)return;
	m_uRadialBlurStrength = MIN(MAX(0, strength), 100);
	DRAW_RENDERER;
}

bool Image::init(const std::string& file)
{
	if (!Widget::init())
	{
		return false;
	}
	setType(WidgetType::IMAGE);
	m_renderer = Sprite::create();
	{
		m_renderer->setCascadeColorEnabled(true);
		m_renderer->setCascadeOpacityEnabled(true);
		m_renderer->ignoreAnchorPointForPosition(true);
		m_renderer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		Node::addChild(m_renderer);
	}
	setSource(file);
	return true;
}

void Image::drawRenderer(bool updateSize)
{
	if (m_parsing)return;
	Size _rendererSize = m_renderer->getContentSize();
	Size _size;
	_size.width = m_givenSize.width < 0 ? _rendererSize.width : m_givenSize.width;
	_size.height = m_givenSize.height < 0 ? _rendererSize.height : m_givenSize.height;

	Rect _textureRect = m_renderer->getTextureRect();
	if (m_repeatType == RepeatType::SCALE)
	{
		m_renderer->setScaleX(_size.width / _textureRect.size.width);
		m_renderer->setScaleY(_size.height / _textureRect.size.height);
	}
	else
	{
		_textureRect.size.width = _size.width;
		_textureRect.size.height = _size.height;
		auto _texture = m_renderer->getTexture();
		m_renderer->setTextureRect(_textureRect);
		if (((_texture->getPixelsWide() % 8) == 0) && ((_texture->getPixelsHigh() % 8) == 0))
		{
			m_renderer->getTexture()->setTexParameters({ GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT });
		}
	}

	switch (m_filter)
	{
	case tina::Image::FilterType::NONE:
		initNormalGLprogram();
		break;
	case tina::Image::FilterType::GRAY:
		initGrayGLprogram();
		break;
	case tina::Image::FilterType::BLUR:
		initBlurGLprogram();
		break;
	case tina::Image::FilterType::RADIAL_BLUR:
		initRadialBlurGLprogram();
		break;
	default:
		break;
	}

	if (updateSize)
		updateSizeFromContent(_size);
}

void Image::initGrayGLprogram()
{
	if (!m_grayGLProgramState)
	{
		auto _grayVshFile = TINA_MGR->getShaderFullPath("gray.vsh");
		auto _blurFshFile = TINA_MGR->getShaderFullPath("gray.fsh");
		auto _program = GLProgram::createWithFilenames(_grayVshFile.c_str(), _blurFshFile.c_str());
		m_grayGLProgramState = GLProgramState::getOrCreateWithGLProgram(_program);
	}
	m_renderer->setGLProgramState(m_grayGLProgramState);
	m_grayGLProgramState->setUniformFloat("u_grayPercent", m_grayPercent);
}

void Image::initBlurGLprogram()
{
	if (!m_blurGLProgramState)
	{
		GLchar * fragSource = (GLchar*)String::createWithContentsOfFile(
			FileUtils::getInstance()->fullPathForFilename(TINA_MGR->getShaderFullPath("blur.fsh")).c_str())->getCString();
		auto _program = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource);
		m_blurGLProgramState = GLProgramState::getOrCreateWithGLProgram(_program);
	}
	m_renderer->setGLProgramState(m_blurGLProgramState);
	auto _size = m_renderer->getTexture()->getContentSizeInPixels();
	m_blurGLProgramState->setUniformVec2("resolution", _size);
	m_blurGLProgramState->setUniformFloat("blurRadius", m_blurRadius);
	m_blurGLProgramState->setUniformFloat("sampleNum", m_blurSampleNum);
}

void Image::initNormalGLprogram()
{
	m_renderer->setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
}

void Image::initRadialBlurGLprogram()
{
	if (!m_userProgramState)
	{
		auto _vshFile = TINA_MGR->getShaderFullPath("radialblur.vsh");
		auto _fshFile = TINA_MGR->getShaderFullPath("radialblur.fsh");
		auto _program = GLProgram::createWithFilenames(_vshFile.c_str(), _fshFile.c_str());
		m_userProgramState = GLProgramState::getOrCreateWithGLProgram(_program);
	}
	m_renderer->setGLProgramState(m_userProgramState);
	m_userProgramState->setUniformInt("u_strength", m_uRadialBlurStrength);
}

TINA_NS_END