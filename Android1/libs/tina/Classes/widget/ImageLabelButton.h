#pragma once

/****************************************************************************
功 能：通用文本+背景图片按钮类
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-03-02
最后修改者：董伟
最后修改日期：2015-09-14
****************************************************************************/

#include "Button.h"

TINA_NS_BEGIN
class Image;
class Label;
class ScaleImage;
class ImageLabelButton : public Button
{
public:
	struct TextCfg
	{
		cocos2d::Color4B	color;
		int					fontSize = 0;
		int					border = 0;
		cocos2d::Color4B	borderColor;
	};

public:
	static ImageLabelButton* create();
	virtual void parseAttr(const std::string& key, const std::vector<Widget*>& nodeParams) override;
	void setBgUp(const std::string& srcUp);
	void setBgDown(const std::string& srcDown);
	void setBgDisabled(const std::string& srcDisabled);
	void setIconUp(const std::string& iconUp);
	void setIconDown(const std::string& iconDown);
	void setIconDisabled(const std::string& iconUp);
	void setText(const std::string& text);
	void setFontName(const std::string& fontName);
	void setTextCfgUp(const TextCfg& cfg);
	void setTextCfgDown(const TextCfg& cfg);
	void setTextCfgDisabled(const TextCfg& cfg);
	void setGap(int gap);

protected:
	virtual bool init();
	virtual void onFocus() override;
	virtual void onBlur() override;
	virtual void onDisabled() override;
	virtual void drawRenderer(bool updateSize = true) override;

protected:
	std::string		m_text;
	std::string		m_fontName;
	TextCfg			m_textCfgUp;
	TextCfg			m_textCfgDown;
	TextCfg			m_textCfgDisabled;
	std::string		m_iconUp;
	std::string		m_iconDown;
	std::string		m_iconDisabled;
	std::string		m_bgUp;
	std::string		m_bgDown;
	std::string		m_bgDisabled;
	int				m_gap = 0;
	Image*			m_icon;
	Label*			m_label;
	ScaleImage*		m_bg;
};
TINA_NS_END