#pragma once

/****************************************************************************
功 能：通用按钮类
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-03-02
最后修改者：唐为民
最后修改日期：2015-08-21
****************************************************************************/

#include "base/Widget.h"

TINA_NS_BEGIN
class Button : public Widget
{
public:
	static Button* create();
	static Button* create(Widget* upState, Widget* downState = nullptr, Widget* disabledState = nullptr);
	virtual void parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values) override;
	virtual void parseAttr(const std::string& key, const std::vector<Widget*>& nodeParams) override;
	virtual void setEnabled(bool enabled) override;
	virtual void setUpState(Widget* node);
	virtual void setDownState(Widget* node);
	virtual void setDisabledState(Widget* node);
	virtual void setScaleMode(bool enabled);
	virtual void setScaleModeFactor(float factor);
	virtual void setScale(float scale) override;
	void setForceSwallowTouch(bool swallow);
	/**	设置按钮点击CD时间
	*	@param interval CD时间（秒），若为0则取消按钮CD
	*/
	void setCD(float interval);

public:
	Widget* getUpState(){ return m_upState; };
	Widget* getDownState(){ return m_downState; };
	Widget* getDisabledState(){ return m_disabledState; };


protected:
	virtual bool init();
	virtual bool initWithStates(Widget* upState, Widget* downState = nullptr, Widget* disabledState = nullptr);
	void detectiveCding(float t);
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) override;
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) override;
	virtual void onFocus() override;
	virtual void onBlur() override;
	virtual void onClick() override;
	virtual void onDisabled() override;
	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void drawRenderer(bool updateSize = true) override;

	
protected:
	Widget*				m_upState = nullptr;
	Widget*				m_downState = nullptr;
	Widget*				m_disabledState = nullptr;
	bool				m_scaleMode = false;
	float				m_scaleModeFactor;
	float				m_originScale = 1;
	bool				m_needCD;
	bool				m_isCDing;
	float				m_cdInterval;
	bool				m_hasSVParent = false;
	bool				m_effectualClick = true;
	bool				m_forceSwallow = false;
};
TINA_NS_END