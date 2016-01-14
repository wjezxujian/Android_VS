#pragma once

/****************************************************************************
功 能：图片类
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-02-18
最后修改者：董伟
最后修改日期：2015-02-27
****************************************************************************/

#include "base/Widget.h"

TINA_NS_BEGIN
class Image : public Widget
{
public:
	enum class RepeatType
	{
		SCALE,
		PATTERN
	};

	enum class FilterType
	{
		NONE,
		GRAY,
		BLUR,
		RADIAL_BLUR //径向模糊
	};

public:
	static Image* create();
	static Image* create(const std::string& file);
	static Image* createWithFormat(const char* format, ...);
	static cocos2d::Sprite* createSprite(const std::string& file);

	virtual void parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values) override;

public:
	void setSource(const std::string& file);
	void setSourceWithFormat(const char* format, ...);
	void setResSource(const std::string& file);
	void setResSourceWithFormat(const char* format, ...);
	virtual void setTextureRect(const cocos2d::Rect& rect);
	virtual void setRepeatType(RepeatType type);
	virtual void setColor(const cocos2d::Color3B& color) override;
	cocos2d::Texture2D* getTexture();

	void setFilter(FilterType type);
	void setGrayPercent(float grayPercent);
	void setBlurRadius(float blurRadius);
	void setBlurSampleNum(float blurSampleNum);
	/**	设置径向模糊的强度
	*	@param strength 强度值 0-100
	*/
	void setRadialBlurStrength(int strength);
	/**	获取径向模糊的强度*/
	int getRadialBlurStrength(){ return m_uRadialBlurStrength; };

	cocos2d::Sprite* getRenderer(){ return m_renderer; };

protected:
	virtual bool init(const std::string& file);
	virtual void drawRenderer(bool updateSize = true) override;
	void initGrayGLprogram();
	void initBlurGLprogram();
	void initNormalGLprogram();
	void initRadialBlurGLprogram();

protected:
	cocos2d::Sprite*			m_renderer = nullptr;
	RepeatType					m_repeatType = RepeatType::SCALE;
	bool						m_useAbsPath = false;
	std::string					m_source;

	FilterType					m_filter = FilterType::NONE;
	cocos2d::GLProgramState*	m_grayGLProgramState = nullptr;
	cocos2d::GLProgramState*	m_blurGLProgramState = nullptr;
	cocos2d::GLProgramState*	m_userProgramState = nullptr;
	float						m_grayPercent = 100.0f;
	float						m_blurRadius = 25.0f;
	float						m_blurSampleNum = 11.0f;
	int							m_uRadialBlurStrength = 0;

};
TINA_NS_END