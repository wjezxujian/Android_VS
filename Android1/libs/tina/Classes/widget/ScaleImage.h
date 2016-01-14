
#pragma once

/****************************************************************************
功 能：图片拉伸类
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-02-21
最后修改者：董伟
最后修改日期：2015-02-27
****************************************************************************/

#include "base/Widget.h"
#include "cocos-ext.h"

TINA_NS_BEGIN
class ScaleImage : public Widget
{
public:
	static ScaleImage* create();
	static ScaleImage* create(const std::string& file, const cocos2d::Size& size, const cocos2d::Rect& capInsets);
	static ScaleImage* create(const std::string& file, const cocos2d::Size& size);
	static ScaleImage* create(const std::string& file, const cocos2d::Rect& rect, const cocos2d::Rect& capInsets);
	static ScaleImage* create(const cocos2d::Rect& capInsets, const std::string& file);
	static ScaleImage* create(const std::string& file, const cocos2d::Rect& rect);

public:
	virtual void parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values) override;

public:
	void setSource(const std::string& src, const cocos2d::Rect& capInset);
	void setSource(const std::string& src);
	void setSourceWithFormat(const cocos2d::Rect& capInset, const char* format, ...);
	void setSourceWithFormat(const char* format, ...);

	cocos2d::ui::Scale9Sprite* getRenderer(){ return m_renderer; };

	const cocos2d::Size& getRenderContentSize();

protected:
	virtual bool init(const std::string& file, const cocos2d::Rect& rect, const cocos2d::Rect& capInsets);
	virtual void drawRenderer(bool updateSize = true) override;

protected:
	cocos2d::ui::Scale9Sprite*	m_renderer = nullptr;
	bool								m_needUpdateRendererSize = false;
	bool								m_useAbsPath = false;
	std::string							m_source;
};
TINA_NS_END