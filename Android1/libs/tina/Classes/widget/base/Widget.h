#pragma once

/****************************************************************************
功 能：Widget基类
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-02-17
最后修改者：董伟
最后修改日期：2015-03-03
****************************************************************************/

#include "include/tinaMacros.h"
#include "cocos2d.h"
#include <string>
#include <vector>

TINA_NS_BEGIN
class WidgetReader;
class LayoutUtil;
class Layout;
class ScrollView;
class Widget : public cocos2d::Node
{
public:
	//触摸事件回调函数
	typedef std::function<void(Widget*, TouchType)> TouchCallfunc;

public:
	/**
	*	设置左边距与父对象内框的距离
	*	注意：此值不一定等于setX()或者setPositionX()
	*/
	virtual void setLeft(float left);
	virtual float getLeft(){ return m_left; };

	/**
	*	设置右边距与父对象内框的距离
	*/
	virtual void setRight(float right);
	virtual float getRight(){ return m_right; };

	/**
	*	设置上边距与父对象内框的距离
	*/
	virtual void setTop(float top);
	virtual float getTop(){ return m_top; };

	/**
	*	设置下边距与父对象内框的距离
	*	注意：此值不一定等于setY()或者setPositionY()
	*/
	virtual void setBottom(float bottom);
	virtual float getBottom(){ return m_bottom; };

	virtual void setPositionX(float x) override;
	virtual void setPositionY(float y) override;

	/**
	*	设置绝对宽度
	*/
	virtual void setWidth(float width);
	virtual void ignoreWidth();
	virtual inline float getWidth(){ return _contentSize.width; };

	/**	设置百分比宽度（相对于父对象的百分比），值为0~100 */
	virtual void setPercentWidth(float percentWidth);
	virtual float getPercentWidth(){ return m_percentWidth; };

	/**
	*	设置绝对高度
	*/
	virtual void setHeight(float height);
	virtual void ignoreHeight();
	virtual float getHeight(){ return _contentSize.height; };

	/**	设置百分比高度（相对于父对象的百分比），值为0~100 */
	virtual void setPercentHeight(float percentHeight);
	virtual float getPercentHeight(){ return m_percentHeight; }
	
	/**	设置x绝对坐标，等同setPositionX()*/
	virtual void setX(float x);
	virtual inline float getX(){ return m_x; };

	/**	设置x的百分比坐标，为父对象的宽的百分比*/
	virtual void setPercentX(float percentX);
	virtual float getPercentX(){ return m_percentX; };

	/**	设置y绝对坐标，等同setPositionY()*/
	virtual void setY(float y);
	virtual float getY(){ return m_y; };
	/**	设置y的百分比坐标，为父对象的高的百分比*/
	virtual void setPercentY(float percentY);
	virtual float getPercentY(){ return m_percentY; };

	virtual void setPercentPosition(float percentX, float percentY);

	/**
	*	设置渲染节点z坐标，等同setLocalZOrder()
	*/
	virtual void setZ(int z);
	virtual inline int getZ(){ return getLocalZOrder(); };

	/**	设置锚点（重写此方法）*/
	virtual void setAnchorPoint(const cocos2d::Vec2& anchorPoint) override;
	virtual void ignoreAnchorPointForPosition(bool ignore) override;
	virtual void setPosition(const cocos2d::Vec2 &position) override;
	virtual void setPosition(float x, float y) override;
	virtual void setContentSize(const cocos2d::Size& contentSize) override;
	virtual void setVisible(bool enabled) override;

	/**	
	*	设置外边距，分别是上、右、下、左
	*	注意：当父对象为流布局容器时此值方有效果
	*/
	virtual void setMargin(uint16_t top, uint16_t right, uint16_t bottom, uint16_t left);
	/**	
	*	设置上外边距
	*	注意：当父对象为流布局容器时此值方有效果
	*/
	virtual void setMarginTop(uint16_t margin);
	/**
	*	设置右外边距
	*	注意：当父对象为流布局容器时此值方有效果
	*/
	virtual void setMarginRight(uint16_t margin);
	/**
	*	设置下外边距
	*	注意：当父对象为流布局容器时此值方有效果
	*/
	virtual void setMarginBottom(uint16_t margin);
	/**
	*	设置左外边距
	*	注意：当父对象为流布局容器时此值方有效果
	*/
	virtual void setMarginLeft(uint16_t margin);
	virtual uint16_t getMarginTop(){ return m_margin.at(0); };
	virtual uint16_t getMarginRight(){ return m_margin.at(1); };
	virtual uint16_t getMarginBottom(){ return m_margin.at(2); };
	virtual uint16_t getMarginLeft(){ return m_margin.at(3); };


	/**
	*	设置唯一标识符
	*/
	virtual void setId(const std::string& id){ m_id = id; };
	virtual const std::string& getId() const{ return m_id; };

	/**	设置是否禁用*/
	virtual void setEnabled(bool enabled);
	void setDarkMode(bool enabled);
	void setBlur();
	void setFocus();
	bool isEnabled(){ return m_enabled; };
	bool isFocus(){ return m_focus; };
	void setTouchEnabled(bool enabled);
	bool isTouchEnabled(){ return m_touchEnabled; };
	void setSwallowTouch(bool enabled);
	
	/**	获取控件类型*/
	WidgetType getType(){ return m_type; };

public:
	virtual void addWidget(Widget* child);
	virtual void addWidget(Widget* child, const std::string &id);
	virtual void addWidget(Widget* child, int localZOrder);
	virtual void addWidget(Widget* child, int localZOrder, const std::string &id);
	virtual void addWidgetAt(Widget* child, uint16_t index);
	virtual void removeWidget(Widget* child, bool cleanup = true);
	virtual void removeAllWidget();
	virtual const cocos2d::Vector<Widget*>& getWidgetChildren() { return m_widgetChildren; };
	Widget* getWidgetParent();
	virtual void removeFromParent() override;
	virtual void removeFromParentAndCleanup(bool cleanup) override;

public:
	/*	设置触摸回调方法
	*	请先设置setTouchEnabled()
	*/
	void setTouchCallfunc(const TouchCallfunc& func);
	/*	设置是否自动设置Touch回调
	*/
	void setAutoTouchCallfunc(bool autoCallfunc);
	bool isAutoTouchCallfunc(){ return m_autoTouchCallfunc; }
	/*	是否开启对长按的支持*/
	void enableLongPress(bool enabled);
	/*	设置长按触发的时间（默认为1秒）*/
	void setLongPressInterval(float interval);
	/*	设置触摸向外延伸的半径
	*	为了解决触摸区域太小不能触摸到的问题
	*	@param radius 向外延伸半径
	*/
	void setTouchExtend(int radius);
	void setTouchExtend(const cocos2d::Vec2& size);
	/*	获取当前的触摸信息*/
	cocos2d::Touch* getTouch();


protected:
	friend class WidgetReader;
	virtual void parse(std::vector<std::string>* keys, std::vector<std::string>* values) final;
	virtual void parse(const std::string& key, const std::vector<Widget*>& nodeParams) final;
	virtual void parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values);
	virtual void parseAttr(const std::string& key, const std::vector<Widget*>& nodeParams){};

protected:
	friend class BaseBox;
	virtual float left2PositionX(float left);
	virtual float right2PositionX(float right, float parentWidth = -1);
	virtual float bottom2PositionY(float bottom);
	virtual float top2PositionY(float top, float parentHeight = -1);
	bool isParentLinearLayout();
	virtual void callParentRelayout() final;
	virtual void updateRelativeLayout();

public:
	//被父容器调用，更新百分比尺寸
	virtual void updateSizeFromParent(const cocos2d::Size& virtualSize);

protected:
	//自身调用，当设置绝对尺寸时
	virtual void updateSize(const cocos2d::Size& contentSize);
	//自身调用，内容撑开尺寸
	virtual void updateSizeFromContent(const cocos2d::Size& contentSize);
	//被子节点调用
	virtual void relayoutByChildrenChanges(){};
	//被父节点调用
	virtual void updateRelativePositionFromParent(const cocos2d::Size& parentSize);
	virtual void drawRenderer(bool updateSize = true){};

protected:
	virtual bool init();
	
	const cocos2d::Size& getParentSize();
	bool isAncestorsVisible(cocos2d::Node* node);
	bool isAncestorsEnabled(cocos2d::Node* node);
	bool hasAncestorsSVParent(cocos2d::Node* node, ScrollView** dstParent);
	friend class LayoutUtil;
	friend class Layout;
	cocos2d::Vec2 getRealAnchor();
	void setType(WidgetType type){ m_type = type; };
	virtual void setWidgetParent(Widget* parent){ m_widgetParent = parent; };
	virtual void onExit() override;

protected:
	/*	判断一个点是否与本对象相撞
		@param vec 基于舞台坐标系的点
	*/
	bool hitTest(const cocos2d::Vec2& vec);
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);
	virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);
	virtual void onFocus();
	virtual void onBlur();
	virtual void onClick();
	virtual void onDisabled();
	virtual void onLongPress();
	void detectiveLongPress(float);

protected:
	WidgetType				m_type = WidgetType::WIDGET;
	std::string				m_id;
	bool					m_enabled = true;
	bool					m_touchEnabled = false;
	bool					m_focus = false;
	cocos2d::Size			m_givenSize = cocos2d::Size(-1, -1);
	cocos2d::Size			m_rendererSize = cocos2d::Size(-1, -1);
	/////////////////////////
	float					m_x = 0;
	float					m_y = 0;
	float					m_percentX = NaN;
	float					m_percentY = NaN;
	float					m_left = NaN;
	float					m_right = NaN;
	float					m_top = NaN;
	float					m_bottom = NaN;
	float					m_percentWidth = NaN;
	float					m_percentHeight = NaN;
	/////////////////////////
	std::vector<uint16_t>	m_margin = std::vector<uint16_t>(4);
	bool					m_darkMode = true;
	bool					m_parsing = false;
	Widget*					m_widgetParent = nullptr;
	cocos2d::Vector<Widget*>	m_widgetChildren;
	cocos2d::EventListenerTouchOneByOne* m_touchListener = nullptr;
	cocos2d::Touch*				m_touch = nullptr;
	TouchCallfunc				m_touchCallfunc = nullptr;
	bool					m_longPressEnabled = false;
	float					m_longPressInterval = 0;
	bool					m_executedLongPress = false;
	cocos2d::Vec2			m_touchExtend = { -1, -1 };
	bool					m_autoTouchCallfunc = false;
};
TINA_NS_END