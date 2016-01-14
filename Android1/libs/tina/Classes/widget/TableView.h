#pragma once

/****************************************************************************
功 能：TableView组件
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-08-28
最后修改者：董伟
最后修改日期：2015-08-28
****************************************************************************/

#include "ScrollView.h"
#include "GridCell.h"

TINA_NS_BEGIN
class TableView : public ScrollView
{
public:
	enum class TouchType
	{
		CLICK,
		LONG_PRESS
	};
	typedef std::map<int16_t, GridCell*> CachedCellList;
	typedef std::function<GridCell*(TableView*, GridCell*, uint32_t)> CellForIndexCallfunc;
	typedef std::function<uint32_t(TableView*)> NumberOfCellCallfunc;
	typedef std::function<void(TableView*, GridCell*, uint32_t, TouchType)> CellTouchCallfunc;

public:
	static TableView* create();
	virtual void parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values) override;
	virtual void setScrollDirection(ScrollDirection direction) override;
	void setHSpacing(uint16_t);
	uint16_t getHSpacing(){ return m_hSpacing; };
	void setVSpacing(uint16_t);
	uint16_t getVSpacing(){ return m_vSpacing; };
	void setPaddingTop(uint16_t);
	uint16_t getPaddingTop(){ return m_paddingTop; };
	void setPaddingRight(uint16_t);
	uint16_t getPaddingRight(){ return m_paddingRight; };
	void setPaddingBottom(uint16_t);
	uint16_t getPaddingBottom(){ return m_paddingBottom; };
	void setPaddingLeft(uint16_t);
	uint16_t getPaddingLeft(){ return m_paddingLeft; };
	void setPadding(uint16_t);
	void setCellSize(const cocos2d::Size& size);
	const cocos2d::Size& getCellSize(){ return m_cellSize; };
	void setPageMode(bool pageMode);
	virtual void setColumnCnt(uint16_t cnt);
	virtual void setRowCnt(uint16_t cnt);
	virtual void setCellForIndexCallfunc(const CellForIndexCallfunc& func){ m_cellForIndexCallfunc = func; };
	virtual void setNumberOfCellCallfunc(const NumberOfCellCallfunc& func){ m_numberOfCellCallfunc = func; };
	virtual void setCellTouchCallfunc(const CellTouchCallfunc& func){ m_cellTouchCallfunc = func; };
	void load();
	void fresh();
	void previous(uint32_t itemNum = 1, bool withAnimate = true, float speed = -1);
	void next(uint32_t itemNum = 1, bool withAnimate = true, float speed = -1);
	void pageTo(uint32_t pageIndex, bool withAnimate = true, float speed = -1);
	GridCell* getCellByIndex(uint32_t index);
	void setSorptionMode(bool sorption);
	bool isBegin();
	bool isEnd();
	CachedCellList* getCachedCells();
	uint32_t getPageCount();
	uint32_t getPageIndex();


protected:
	virtual bool init();
	void getShowCells(const cocos2d::Rect& area, std::vector<uint16_t>* dst);
	/*	获取某个Cell的显示区域
		@param idx 序号，0 ~ m_totalCnt-1
		@return Rect此Cell的显示区域
	*/
	cocos2d::Rect getCellRect(uint32_t idx);
	void getCellRect(uint32_t idx, cocos2d::Rect* dstRect);
	cocos2d::Size calcContainerSize();
	GridCell* getAvailableCachedCell(const std::vector<uint16_t>& showCells, uint16_t idx, uint16_t* originIdx = nullptr);
	int getCellIndex(const cocos2d::Vec2& position);
	void arrangeCells(bool reload = false);
	CachedCellList::iterator getCellNotIn(int first, int last);
	float getCellTotalWidth(){ return m_cellSize.width + m_hSpacing * 2; };
	float getCellTotalHeight(){ return m_cellSize.height + m_vSpacing * 2; };
	virtual void onLongPress() override;
	virtual void didScroll() override;
	virtual void endScroll() override;
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unusedEvent) override;
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unusedEvent) override;
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unusedEvent) override;
	virtual void drawRenderer(bool updateSize = true) override;

protected:
	CellForIndexCallfunc	m_cellForIndexCallfunc;
	NumberOfCellCallfunc	m_numberOfCellCallfunc;
	CellTouchCallfunc		m_cellTouchCallfunc;

	uint16_t				m_columnCnt = 0;
	uint16_t				m_rowCnt = 0;
	uint16_t				m_paddingTop = 0;
	uint16_t				m_paddingRight = 0;
	uint16_t				m_paddingBottom = 0;
	uint16_t				m_paddingLeft = 0;
	uint16_t				m_hSpacing = 0;
	uint16_t				m_vSpacing = 0;
	cocos2d::Size			m_cellSize;
	/*	PageMode主要影响Cell的排列形式
		假设排列方式为：横向，columnCnt(3),rowCnt(2)
		*m_pageMode = false :
		1...3...5...7...9...
		2...4...6...8...10..
		*m_pageMode = true :
		1...2...3...7...8...9...
		4...5...6...10..
		纵向排列时则不受此值影响
	*/
	bool					m_pageMode = false;

	uint32_t				m_totalCnt = 0;
	CachedCellList			m_cachedCellList;
	//正在显示的cell列表（序号：0 ~ m_totalCnt-1）
	std::vector<uint16_t>	m_showCells;
	int						m_startFocusCellIndex;
	bool					m_loading = false;

};
TINA_NS_END