#pragma once

/****************************************************************************
功 能：绝对布局类
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-03-02
最后修改者：董伟
最后修改日期：2015-03-02
****************************************************************************/

#include "base/Widget.h"

TINA_NS_BEGIN
class TextField : public Widget, public cocos2d::TextFieldDelegate, public cocos2d::IMEDelegate
{
public:
	static TextField* create();
	virtual void parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values) override;

public:
    static void detachIME();
	virtual void setPlaceHolderColor(const cocos2d::Color4B& color);
	virtual void setTextColor(const cocos2d::Color4B& textColor);
	virtual void setFontSize(float size);
	virtual void setFontName(const std::string& fontName);
	virtual void setString(const std::string& text);
	virtual const std::string& getString() const;
	virtual void setPlaceHolder(const std::string& text);
	virtual void setBorderWidth(float width);
	virtual void setBorderColor(const cocos2d::Color4B& color);
	virtual void setBgColor(const cocos2d::Color4B& color);
	virtual void displayAsPassword(bool enabled);
	virtual void setMaxNum(uint16_t num);

	void setCallfuncDeleteBackward(const std::function<bool(const char*, size_t)>& cf);

protected:
	virtual bool init();
	void createCursor();
	virtual void keyboardWillShow(cocos2d::IMEKeyboardNotificationInfo& info) override;
	virtual void keyboardWillHide(cocos2d::IMEKeyboardNotificationInfo& info) override;
	virtual bool attachWithIME() override;
	virtual bool detachWithIME() override;
	virtual bool onTextFieldInsertText(cocos2d::TextFieldTTF* sender, const char* text, size_t nLen) override;
	virtual bool onTextFieldDeleteBackward(cocos2d::TextFieldTTF* sender, const char* delText, size_t nLen) override;
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) override;
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) override;
	void drawBorder();
	void adjustWithKeyboardShow();
	void adjustWithKeyboardHide();
	void adjustCursor();
    virtual void onExit() override;
	virtual void drawRenderer(bool updateSize = true) override;
	virtual void visit(cocos2d::Renderer* renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;
	
protected:
	cocos2d::DrawNode*		m_bg;
	cocos2d::TextFieldTTF*	m_renderer;
	cocos2d::ClippingNode*	m_clipping;
	cocos2d::LayerColor*	m_stenceil;
	float					m_borderWidth = 1;
	cocos2d::Color4B		m_borderColor = cocos2d::Color4B::BLACK;
	cocos2d::Color4B		m_bgColor = cocos2d::Color4B(0, 0, 0, 0);
	bool					m_inputing = false;
	cocos2d::Sprite*		m_cursor;
	bool					m_needAdjustCursor = false;
	float					m_adjustCursorDt = 0;
	uint16_t				m_maxNum = 0;

	std::function<bool(const char*, size_t)> m_callfuncDelete;//删除回调
};
TINA_NS_END