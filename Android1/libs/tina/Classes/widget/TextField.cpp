#include "TextField.h"
#include "Label.h"
#include "util/ParamUtil.h"

USING_NS_CC;

TINA_NS_BEGIN

#define DEFAULT_WIDTH (100)
#define DEFAULT_HEIGHT (20)
#define PADDING (2)
#define PADDING_LEFT (15)

static cocos2d::IMEKeyboardNotificationInfo m_keyboardInfo;
static cocos2d::Vector<TextField*> m_inputingList;

static int _calcCharCount(const char * text)
{
	int n = 0;
	char ch = 0;
	while ((ch = *text))
	{
		CC_BREAK_IF(!ch);

		if (0x80 != (0xC0 & ch))
		{
			++n;
		}
		++text;
	}
	return n;
}

TextField* TextField::create()
{
	auto _ref = new TextField();
	if (_ref && _ref->init())
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

void TextField::parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values)
{
	Widget::parseAttr(keys, values);
	for (auto i = 0; i < keys->size(); ++i)
	{
		const auto& _key = keys->at(i);
		const auto& _value = values->at(i);
		if (_key == "placeholdercolor")
		{
			setPlaceHolderColor(ParamUtil::readColor4B(_value));
		}
		else if (_key == "placeholder")
		{
			setPlaceHolder(_value);
		}
		else if (_key == "fontcolor" || _key == "textcolor")
		{
			setTextColor(ParamUtil::readColor4B(_value));
		}
		else if (_key == "fontsize")
		{
			setFontSize(ParamUtil::readFloat(_value));
		}
		else if (_key == "fontname" || _key == "font")
		{
			setFontName(_value);
		}
		else if (_key == "text")
		{
			setString(_value);
		}
		else if (_key == "bordercolor")
		{
			setBorderColor(ParamUtil::readColor4B(_value));
		}
		else if (_key == "borderwidth" || _key == "border")
		{
			setBorderWidth(ParamUtil::readFloat(_value));
		}
		else if (_key == "bgcolor")
		{
			setBgColor(ParamUtil::readColor4B(_value));
		}
		else if (_key == "displayaspassword" || _key == "passwordmode")
		{
			displayAsPassword(ParamUtil::readBool(_value));
		}
		else if (_key == "maxnum")
		{
			setMaxNum(ParamUtil::readInt(_value));
		}
	}
}

void TextField::detachIME()
{
    if(m_inputingList.empty())
    {
        return;
    }
    m_inputingList.front()->detachWithIME();
}

void TextField::setPlaceHolderColor(const cocos2d::Color4B& color)
{
	m_renderer->setColorSpaceHolder(color);
}

void TextField::setTextColor(const cocos2d::Color4B& textColor)
{
	m_renderer->setTextColor(textColor);
}

void TextField::setFontSize(float size)
{
	m_renderer->setSystemFontSize(size);
}

void TextField::setFontName(const std::string& fontName)
{
	m_renderer->setSystemFontName(fontName);
}

void TextField::setString(const std::string& text)
{
	m_renderer->setString(text);
	adjustCursor();
}

const std::string& TextField::getString() const
{
	return m_renderer->getString();
}

void TextField::setPlaceHolder(const std::string& text)
{
	m_renderer->setPlaceHolder(text);
}

void TextField::setBorderWidth(float width)
{
	m_borderWidth = width;
	DRAW_RENDERER;
}

void TextField::setBorderColor(const cocos2d::Color4B& color)
{
	m_borderColor = color;
	DRAW_RENDERER;
}

void TextField::setBgColor(const cocos2d::Color4B& color)
{
	m_bgColor = color;
	DRAW_RENDERER;
}

void TextField::displayAsPassword(bool enabled)
{
	m_renderer->setSecureTextEntry(enabled);
}

void TextField::setMaxNum(uint16_t num)
{
	m_maxNum = num;
}

void TextField::setCallfuncDeleteBackward(const std::function<bool(const char*, size_t)>& cf)
{
	m_callfuncDelete = cf;
}

bool TextField::init()
{
	if (!Widget::init())
	{
		return false;
	}

	m_callfuncDelete = nullptr;

	m_bg = DrawNode::create();
	{
		m_bg->ignoreAnchorPointForPosition(true);
		Node::addChild(m_bg);
	}

	m_stenceil = LayerColor::create(Color4B::BLACK);
	{
		m_stenceil->setContentSize(Size(DEFAULT_WIDTH - PADDING, DEFAULT_HEIGHT));
		m_stenceil->ignoreAnchorPointForPosition(true);
	}

	m_clipping = ClippingNode::create();
	{
		m_clipping->ignoreAnchorPointForPosition(true);
		m_clipping->setStencil(m_stenceil);
		Node::addChild(m_clipping);
	}

	m_renderer = TextFieldTTF::textFieldWithPlaceHolder("", "arial", 12);
	{
		m_renderer->setTextColor(Color4B::BLACK);
		m_renderer->setHorizontalAlignment(TextHAlignment::LEFT);
		m_renderer->setVerticalAlignment(TextVAlignment::CENTER);
		m_renderer->setAnchorPoint({0, 0.5f});
		m_renderer->setPositionX(PADDING);
		m_renderer->setDelegate(this);
		m_clipping->addChild(m_renderer);
	}

	m_cursor = nullptr;

	setTouchEnabled(true);

	updateSizeFromContent(Size(DEFAULT_WIDTH, DEFAULT_HEIGHT));
	return true;
}

void TextField::createCursor()
{
	if (m_cursor)
	{
		m_cursor->stopAllActions();
		auto _texture = m_cursor->getTexture();
		m_cursor->removeFromParent();
		CC_SAFE_DELETE(_texture);
	}

	int16_t _height = MAX(_contentSize.height - 4 - m_borderWidth * 2, m_renderer->getSystemFontSize());
	int16_t _width = 4;

	int* _pixel = new int[_height * _width];
	for (int i = 0; i < _height * _width; ++i)
	{
		*(_pixel + i) = 0xffffffff;
	}
	Texture2D* _texture = new Texture2D();
	_texture->initWithData(_pixel, _height * _width, Texture2D::PixelFormat::RGB888, _width, _height, Size(_width, _height));
	m_cursor = Sprite::createWithTexture(_texture);
	m_cursor->setColor(Color3B(66, 107, 242));
	m_cursor->setVisible(true);
	m_cursor->runAction(RepeatForever::create((ActionInterval *)Sequence::createWithTwoActions(FadeOut::create(0.55f), FadeIn::create(0.55f))));
	Node::addChild(m_cursor);
	adjustCursor();
	CC_SAFE_DELETE(_pixel);
}

void TextField::keyboardWillShow(cocos2d::IMEKeyboardNotificationInfo& info)
{
    if(!m_inputing)return;
    m_keyboardInfo = info;
    adjustWithKeyboardShow();
}

void TextField::keyboardWillHide(cocos2d::IMEKeyboardNotificationInfo& info)
{
    if(!m_inputing)return;
    detachWithIME();
}

bool TextField::attachWithIME()
{
    if(!m_inputingList.contains(this))
    {
        m_inputingList.pushBack(this);
    }
    m_inputing = true;
    m_renderer->attachWithIME();
	createCursor();
	return true;
}

bool TextField::detachWithIME()
{
    m_inputingList.eraseObject(this);
    if(m_inputingList.size() == 0)
    {
        adjustWithKeyboardHide();
        m_renderer->detachWithIME();
    }
	m_inputing = false;
    if(m_cursor)
    {
        m_cursor->setVisible(false);
        m_cursor->stopAllActions();
    }
	return true;
}

bool TextField::onTextFieldInsertText(cocos2d::TextFieldTTF* sender, const char* text, size_t nLen)
{
	if (m_maxNum > 0)
	{
		int _textLen = _calcCharCount(text);
		if (m_renderer->getStringLength() + _textLen > m_maxNum)
		{
			return true;
		}
	}
	
	m_needAdjustCursor = true;
	m_adjustCursorDt = 0;
	return false;
}

bool TextField::onTextFieldDeleteBackward(cocos2d::TextFieldTTF* sender, const char* delText, size_t nLen)
{
	m_needAdjustCursor = true;
	m_adjustCursorDt = 0;

	if (m_callfuncDelete != nullptr)
	{
		return m_callfuncDelete(delText, nLen);
	}

	return false;
}

bool TextField::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unusedEvent)
{
	do
	{
		CC_BREAK_IF(!isVisible());
		CC_BREAK_IF(!isEnabled());
		CC_BREAK_IF(!isAncestorsVisible(this));
		CC_BREAK_IF(!isAncestorsEnabled(this));
		return true;
	} while (0);
	return false;
}

void TextField::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unusedEvent)
{
	if (hitTest(touch->getLocation()))
	{
		attachWithIME();
	}
	else
	{
		if (m_inputing)
			detachWithIME();
	}
}

void TextField::drawBorder()
{
	m_bg->clear();
	Vec2 _points[4];
	_points[0] = {m_borderWidth, m_borderWidth};
	_points[1] = { _contentSize.width - m_borderWidth, m_borderWidth };
	_points[2] = { _contentSize.width - m_borderWidth, _contentSize.height - m_borderWidth };
	_points[3] = { m_borderWidth, _contentSize.height - m_borderWidth };
	m_bg->drawPolygon(_points, 4, Color4F(m_bgColor), m_borderWidth * 0.5f, Color4F(m_borderColor));
}

void TextField::adjustWithKeyboardShow()
{
    auto _runingScene = _director->getRunningScene();
    if(!_runingScene)return;
    auto _keyboardHeight = MIN(m_keyboardInfo.end.size.width, m_keyboardInfo.end.size.height);
    auto _selfPos = this->convertToWorldSpace(Vec2::ZERO);
    float _adjustKeyboardY = MAX(0, _keyboardHeight - _selfPos.y + _runingScene->getPositionY());
    _runingScene->setPositionY(_adjustKeyboardY);
}

void TextField::adjustWithKeyboardHide()
{
    auto _runingScene = _director->getRunningScene();
    if(!_runingScene)return;
    _runingScene->setPositionY(0);
}

void TextField::adjustCursor()
{
	if (!m_inputing)return;
	float _rendererWidth = (m_renderer->getCharCount() > 0) ? m_renderer->getContentSize().width : 0;
	float _rendererX = MIN(_contentSize.width - PADDING_LEFT - _rendererWidth, PADDING);
	m_renderer->setPositionX(_rendererX);
	float _x = _rendererWidth + m_renderer->getPositionX() + m_clipping->getPositionX();
	_x += 3;
	m_cursor->setPosition(Vec2(_x, _contentSize.height / 2));
}

void TextField::onExit()
{
    Widget::onExit();
    m_inputingList.eraseObject(this);
}

void TextField::drawRenderer(bool updateSize)
{
	drawBorder();
	m_stenceil->setContentSize(Size(_contentSize.width - PADDING - m_borderWidth * 2, _contentSize.height));
	m_clipping->setPositionX(m_borderWidth);
	m_renderer->setHeight(_contentSize.height);
	m_renderer->setPositionY(_contentSize.height * 0.5f);
}

void TextField::visit(cocos2d::Renderer* renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags)
{
	if (m_needAdjustCursor)
	{
		m_needAdjustCursor = false;
		adjustCursor();
	}
	Widget::visit(renderer, parentTransform, parentFlags);
}

TINA_NS_END