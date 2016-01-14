#pragma once

/****************************************************************************
功 能：滚动组件
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-03-07
最后修改者：董伟
最后修改日期：2015-03-07
****************************************************************************/

#include "base/Widget.h"

TINA_NS_BEGIN
class BaseBox;
class ScrollView : public Widget
{
public:
	enum class Location
	{
		TOP,
		BOTTOM,
		LEFT,
		RIGHT
	};

	typedef std::function<void(ScrollView*)> didScrollCallfunc;
	typedef std::function<void(ScrollView*)> endScrollCallfunc;

public:
	static ScrollView* create();
	static ScrollView* create(const cocos2d::Size& size);
	static ScrollView* create(float width, float height);

	virtual void parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values) override;
	virtual void setSorption(const cocos2d::Vec2& sorption);
	virtual void setContainerSize(const cocos2d::Size& size);
	virtual void scrollTo(const cocos2d::Vec2& distance, bool withAnimate = true, float speed = -1);
	virtual void scrollTo(const Location& location, bool withAnimate = true, float speed = -1);
	virtual cocos2d::Vec2 getContentOffset();
	virtual void setScrollEnabled(bool enabled){ m_scrollEnabled = enabled; };
	virtual bool getScrollEnabled(){ return m_scrollEnabled; };
	virtual void setScrollDirection(ScrollDirection direction){ m_scrollDirection = direction; };
	virtual ScrollDirection getScrollDirection(){ return m_scrollDirection; };
	virtual void setBounceEnabled(bool enabled){ m_bounce = enabled; };
	virtual bool getBounceEnabled(){ return m_bounce; };

	virtual void setDidScrollCallfunc(const didScrollCallfunc& func){ m_didScrollCallfunc = func; };
	virtual void setEndScrollCallfunc(const endScrollCallfunc& func){ m_endScrollCallfunc = func; };

	bool isBegin();
	bool isEnd();

protected:
	virtual bool init(const cocos2d::Size& viewSize, BaseBox* container = nullptr);
	virtual void updateSizeFromParent(const cocos2d::Size& virtualSize) override;
	virtual void updateSize(const cocos2d::Size& contentSize) override;
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unusedEvent) override;
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unusedEvent) override;
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unusedEvent) override;
	void updateContainerPos(const cocos2d::Vec2& newPos);
	cocos2d::Rect getBoundingBox();
	bool isNeedRebound();
	void calcScrollRect();
	bool checkCanScroll();
	void rebound();
	void inertia(float dt);
	bool needSorption();
	void sorption();
	cocos2d::Vec2 getSorptionPosition(const cocos2d::Vec2& pos);
	virtual void update(float dt) override;
	void handlerScrollEnd();
	void handlerContainerLayout(BaseBox* box);
	virtual void didScroll(){};
	virtual void endScroll(){};

public:
	virtual void addWidget(Widget* child) override;
	virtual void addWidget(Widget* child, const std::string &id) override;
	virtual void addWidget(Widget* child, int localZOrder) override;
	virtual void addWidget(Widget* child, int localZOrder, const std::string &id) override;
	virtual void addWidgetAt(Widget* child, uint16_t index) override;
	virtual void removeWidget(Widget* child, bool cleanup = true) override;
	virtual void removeAllWidget() override;
	virtual const cocos2d::Vector<Widget*>& getWidgetChildren() override;

protected:
	virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;
	void beforeDraw();
	void onBeforeDraw();
	void afterDraw();
	void onAfterDraw();
	cocos2d::Rect getViewRect();

protected:
	BaseBox*			m_container;	
	cocos2d::Vec2		m_containerLocation;	//非opengl的坐标信息（以左上角为原点）
	bool				m_scrollEnabled;
	ScrollDirection		m_scrollDirection;
	ScrollDirection		m_finalScrollDirection;
	float				m_detectiveDirectionPast;
	bool				m_bounce;

	bool				m_isInertia;
	cocos2d::Vec2		m_inertiaForce;

	cocos2d::Rect		m_scrollRect;
	cocos2d::Vec2		m_sorption;
	float				m_inertiaDuration;
	float				m_inertialOriginSpeed;
	bool				m_isScrolling;
	cocos2d::Vec2		m_scrollOriginPos;
	cocos2d::Vec2		m_scrollTo;

	cocos2d::Vec2		m_beganPos;
	cocos2d::Vec2		m_movedPos;
	cocos2d::Vec2		m_endedPos;
	//是否正在手动拖动
	bool				m_isDrag;
	//手动拖动经历的时间（秒）
	float				m_dragDuration;
	cocos2d::Vec2		m_dragDirection;
	bool				m_needCalScrollRect = false;
	didScrollCallfunc	m_didScrollCallfunc = nullptr;
	endScrollCallfunc	m_endScrollCallfunc = nullptr;

protected:
	bool				m_clippingToBounds;
	bool				m_scissorRestored;
	cocos2d::Rect		m_parentScissorRect;
	cocos2d::CustomCommand	m_beforeDrawCommand;
	cocos2d::CustomCommand	m_afterDrawCommand;
};
TINA_NS_END