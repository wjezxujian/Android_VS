#pragma once

/****************************************************************************
功 能：通用图片按钮类
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-03-02
最后修改者：董伟
最后修改日期：2015-09-14
****************************************************************************/

#include "Button.h"

TINA_NS_BEGIN
class ImageButton : public Button
{
public:
	static ImageButton* create();
	static ImageButton* create(const std::string& srcUp, const std::string& srcDown = "", const std::string& srcDisabled = "");
	virtual void parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values) override;
	void setSrc(const std::string& srcUp);
	void setSrcDown(const std::string& srcDown);
	void setSrcDisabled(const std::string& srcDisabled);

protected:
	virtual bool init();
	virtual bool initWithSrc(const std::string& srcUp, const std::string& srcDown = "", const std::string& srcDisabled = "");
	virtual void onFocus() override;
	virtual void onBlur() override;
	virtual void onDisabled() override;
	virtual void drawRenderer(bool updateSize = true) override;

protected:
	std::string		m_srcUp;
	std::string		m_srcDown;
	std::string		m_srcDisabled;
};
TINA_NS_END