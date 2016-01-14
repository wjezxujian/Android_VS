#include "TableView.h"
#include "util/ParamUtil.h"
#include "Box.h"


USING_NS_CC;

TINA_NS_BEGIN

#define CLICK_MAX_MOVE_DISTANCE (10)
#define LONG_PRESS_INTERVAL (1.f)

TableView* TableView::create()
{
	auto pRet = new TableView();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

void TableView::parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values)
{
	ScrollView::parseAttr(keys, values);
	for (auto i = 0; i < keys->size(); ++i)
	{
		const auto& _key = keys->at(i);
		const auto& _value = values->at(i);
		if (_key == "hspacing")
		{
			setHSpacing(ParamUtil::readFloat(_value));
		}
		else if (_key == "vspacing")
		{
			setVSpacing(ParamUtil::readFloat(_value));
		}
		else if (_key == "cellsize")
		{
			setCellSize(ParamUtil::readSize(_value));
		}
		else if (_key == "columncnt" || _key == "column")
		{
			setColumnCnt(ParamUtil::readUInt(_value));
		}
		else if (_key == "rowcnt" || _key == "row")
		{
			setRowCnt(ParamUtil::readUInt(_value));
		}
		else if (_key == "padding")
		{
			setPadding(ParamUtil::readUInt(_value));
		}
		else if (_key == "paddingtop")
		{
			setPaddingTop(ParamUtil::readUInt(_value));
		}
		else if (_key == "paddingright")
		{
			setPaddingRight(ParamUtil::readUInt(_value));
		}
		else if (_key == "paddingbottom")
		{
			setPaddingBottom(ParamUtil::readUInt(_value));
		}
		else if (_key == "paddingleft")
		{
			setPaddingLeft(ParamUtil::readUInt(_value));
		}
	}
}

void TableView::setScrollDirection(ScrollDirection direction)
{
	ScrollView::setScrollDirection(direction);
	DRAW_RENDERER;
}

void TableView::setHSpacing(uint16_t spacing)
{
	if (m_hSpacing == spacing)return;
	m_hSpacing = spacing;
	DRAW_RENDERER;
}

void TableView::setVSpacing(uint16_t spacing)
{
	if (m_vSpacing == spacing)return;
	m_vSpacing = spacing;
	DRAW_RENDERER;
}

void TableView::setPaddingTop(uint16_t padding)
{
	if (m_paddingTop == padding)return;
	m_paddingTop = padding;
	DRAW_RENDERER;
}

void TableView::setPaddingRight(uint16_t padding)
{
	if (m_paddingRight == padding)return;
	m_paddingRight = padding;
	DRAW_RENDERER;
}

void TableView::setPaddingBottom(uint16_t padding)
{
	if (m_paddingBottom == padding)return;
	m_paddingBottom = padding;
	DRAW_RENDERER;
}

void TableView::setPaddingLeft(uint16_t padding)
{
	if (m_paddingLeft == padding)return;
	m_paddingLeft = padding;
	DRAW_RENDERER;
}

void TableView::setPadding(uint16_t padding)
{
	m_paddingTop = padding;
	m_paddingRight = padding;
	m_paddingBottom = padding;
	m_paddingLeft = padding;
	DRAW_RENDERER;
}

void TableView::setCellSize(const Size& size)
{
	if (m_cellSize.equals(size))return;
	m_cellSize = size;
	DRAW_RENDERER;
}

void TableView::setPageMode(bool pageMode)
{
	if (m_pageMode == pageMode)return;
	m_pageMode = pageMode;
	DRAW_RENDERER;
}

void TableView::setColumnCnt(uint16_t cnt)
{
	if (m_columnCnt == cnt)return;
	m_columnCnt = cnt;
	DRAW_RENDERER;
}

void TableView::setRowCnt(uint16_t cnt)
{
	if (m_rowCnt == cnt)return;
	m_rowCnt = cnt;
	DRAW_RENDERER;
}

void TableView::load()
{
	if (!m_numberOfCellCallfunc || !m_cellForIndexCallfunc)return;
	m_loading = true;
	m_totalCnt = m_numberOfCellCallfunc(this);

	//计算_container的尺寸
	Size _containerSize = calcContainerSize();
	ScrollView::setContainerSize(_containerSize);

	if (m_scrollDirection == ScrollDirection::HORIZONTAL)
	{
		ScrollView::scrollTo(ScrollView::Location::LEFT, false);
	}
	else if (m_scrollDirection == ScrollDirection::VERTICAL)
	{
		ScrollView::scrollTo(ScrollView::Location::TOP, false);
	}

	arrangeCells(true);
	m_loading = false;
}

void TableView::fresh()
{
	auto _lastPos = m_container->getPosition();
	auto _lastContentsize = m_container->getContentSize();
	load();
	auto _newContentsize = m_container->getContentSize();
	Vec2 _newPos;
	if (m_scrollDirection == ScrollDirection::VERTICAL)
	{
		_newPos.y = _lastContentsize.height + _lastPos.y - _newContentsize.height;
	}
	else if (m_scrollDirection == ScrollDirection::HORIZONTAL)
	{
		_newPos.x = _lastPos.x;
	}
	_newPos.x = MIN(m_scrollRect.getMaxX(), MAX(_newPos.x, m_scrollRect.getMinX()));
	_newPos.y = MIN(m_scrollRect.getMaxY(), MAX(_newPos.y, m_scrollRect.getMinY()));
	m_container->setPosition(_newPos);
	arrangeCells();
}

void TableView::pageTo(uint32_t pageIndex, bool withAnimate, float speed)
{
	uint32_t _pageCnt = getPageCount();
	uint32_t _pageIdx = getPageIndex();
	if (pageIndex > _pageCnt - 1)
	{
		return;
	}

	int _pageDelta = pageIndex - _pageIdx;

	Vec2 _offset;
	if (m_scrollDirection == ScrollDirection::HORIZONTAL)
	{
		_offset.x = -_pageDelta * getContentSize().width;
	}
	else
	{
		_offset.y = _pageDelta * getContentSize().height;
	}
	ScrollView::scrollTo(_offset, withAnimate, speed);
}

void TableView::previous(uint32_t itemNum, bool withAnimate, float speed)
{
	Vec2 _offset;
	if (m_scrollDirection == ScrollDirection::HORIZONTAL)
		_offset.x = getCellTotalWidth() * itemNum;
	else
		_offset.y = -getCellTotalHeight() * itemNum;
	ScrollView::scrollTo(_offset, withAnimate, speed);
}

void TableView::next(uint32_t itemNum, bool withAnimate, float speed)
{
	Vec2 _offset;
	if (m_scrollDirection == ScrollDirection::HORIZONTAL)
		_offset.x = -getCellTotalWidth() * itemNum;
	else
		_offset.y = getCellTotalHeight() * itemNum;
	ScrollView::scrollTo(_offset, withAnimate, speed);
}

GridCell* TableView::getCellByIndex(uint32_t index)
{
	do
	{
		CC_BREAK_IF(index >= m_totalCnt);
		auto _it = m_cachedCellList.find(index);
		CC_BREAK_IF(_it == m_cachedCellList.end());
		return _it->second;
	} while (0);
	return nullptr;
}

void TableView::setSorptionMode(bool sorption)
{
	Vec2 _sorption;
	if (sorption)
	{
		if (m_scrollDirection == ScrollDirection::HORIZONTAL)
		{
			_sorption.x = getCellTotalWidth();
		}
		else if (m_scrollDirection == ScrollDirection::VERTICAL)
		{
			_sorption.y = getCellTotalHeight();
		}
	}
	ScrollView::setSorption(_sorption);
}

bool TableView::isBegin()
{
	do
	{
		CC_BREAK_IF(m_totalCnt <= m_columnCnt * m_rowCnt);
		return ScrollView::isBegin();
	} while (0);
	return true;
}

bool TableView::isEnd()
{
	do
	{
		CC_BREAK_IF(m_totalCnt <= m_columnCnt * m_rowCnt);
		return ScrollView::isEnd();
	} while (0);
	return true;
}

TableView::CachedCellList* TableView::getCachedCells()
{
	return &m_cachedCellList;
}

uint32_t TableView::getPageCount()
{
	return std::ceil(1.f * m_totalCnt / (m_rowCnt * m_columnCnt));
}

uint32_t TableView::getPageIndex()
{
	uint32_t _pageCnt = getPageCount();
	uint32_t _pageIdx = 0;
	auto _offset = getContentOffset();
	if (m_scrollDirection == ScrollDirection::HORIZONTAL)
	{
		auto _realX = _offset.x * m_container->getContentSize().width;
		auto _pageTotalWidth = _pageCnt * getCellTotalWidth() * m_columnCnt;
		_pageIdx = _realX / _pageTotalWidth * _pageCnt;
	}
	else
	{
		auto _realY = (1.f - _offset.y) * m_container->getContentSize().height;
		auto _pageTotalHeight = _pageCnt * getCellTotalHeight() * m_rowCnt;
		_pageIdx = _realY / _pageTotalHeight * _pageCnt;
	}
	_pageIdx = MIN(MAX(_pageIdx, 0), _pageCnt - 1);
	return _pageIdx;
}

bool TableView::init()
{
	if (!ScrollView::init(Size(100, 100)))
	{
		return false;
	}
	m_scrollDirection = ScrollDirection::VERTICAL;
	m_longPressInterval = LONG_PRESS_INTERVAL;
	m_totalCnt = 0;
	return true;
}

void TableView::getShowCells(const cocos2d::Rect& area, std::vector<uint16_t>* dst)
{
	Rect _cellRect;
	for (int i = 0; i < m_totalCnt; ++i)
	{
		getCellRect(i, &_cellRect);
		if (!area.intersectsRect(_cellRect))
			continue;
		dst->push_back(i);
	}
}

Rect TableView::getCellRect(uint32_t idx)
{
	Rect _rect;
	getCellRect(idx, &_rect);
	return _rect;
}

void TableView::getCellRect(uint32_t idx, cocos2d::Rect* dstRect)
{
	dstRect->size = m_cellSize;
	int _page = idx / (m_rowCnt * m_columnCnt);
	int _idxInPage = idx % (m_rowCnt * m_columnCnt);
	int _totalHeight = m_container->getContentSize().height;

	if (m_scrollDirection == ScrollDirection::HORIZONTAL)
	{
		int _deltaX = m_paddingLeft + _page * m_columnCnt * (m_hSpacing * 2 + m_cellSize.width);
		if (m_pageMode)
		{
			dstRect->origin.x = _deltaX + (_idxInPage % m_columnCnt) * getCellTotalWidth() + m_hSpacing;
			dstRect->origin.y = _totalHeight - ((_idxInPage / m_columnCnt) * getCellTotalHeight() + m_paddingTop + m_vSpacing + m_cellSize.height);
		}
		else
		{
			dstRect->origin.x = _deltaX + (_idxInPage / m_rowCnt) * getCellTotalWidth() + m_hSpacing;
			dstRect->origin.y = _totalHeight - ((_idxInPage % m_rowCnt) * getCellTotalHeight() + m_paddingTop + m_vSpacing + m_cellSize.height);
		}
	}
	else if (m_scrollDirection == ScrollDirection::VERTICAL)
	{
		int _pageCnt = std::ceil((1.f * m_totalCnt) / (m_rowCnt * m_columnCnt));
		dstRect->origin.x = m_paddingLeft + (_idxInPage % m_columnCnt) * getCellTotalWidth() + m_hSpacing;
		dstRect->origin.y = _totalHeight - (m_paddingTop + _page * getCellTotalHeight() * m_rowCnt + (_idxInPage / m_columnCnt) * getCellTotalHeight() + m_vSpacing + m_cellSize.height);
	}
}

cocos2d::Size TableView::calcContainerSize()
{
	int _pageCnt = std::ceil((1.f * m_totalCnt) / (m_rowCnt * m_columnCnt));
	Size _containerSize;
	if (m_scrollDirection == ScrollDirection::HORIZONTAL)
	{
		if (m_pageMode)
			_containerSize.width = m_paddingLeft + m_paddingRight + _pageCnt * m_columnCnt * getCellTotalWidth();
		else
			_containerSize.width = m_paddingLeft + m_paddingRight + std::ceil((1.f * m_totalCnt) / m_rowCnt) * getCellTotalWidth();
		_containerSize.height = m_paddingTop + m_paddingBottom + m_rowCnt * getCellTotalHeight();
	}
	else if (m_scrollDirection == ScrollDirection::VERTICAL)
	{
		_containerSize.width = m_paddingLeft + m_paddingRight + m_columnCnt * getCellTotalWidth();
		_containerSize.height = m_paddingTop + m_paddingBottom + std::ceil((1.f * m_totalCnt) / m_columnCnt) * getCellTotalHeight();
	}
	return _containerSize;
}

GridCell* TableView::getAvailableCachedCell(const std::vector<uint16_t>& showCells, uint16_t idx, uint16_t* originIdx)
{
	if (m_cachedCellList.empty())
		return nullptr;

	auto _it = m_cachedCellList.find(idx);
	if (_it != m_cachedCellList.end())
	{
		if (originIdx != nullptr)
			*originIdx = _it->first;
		return _it->second;
	}

	_it = std::find_if(m_cachedCellList.begin(), m_cachedCellList.end(), [&showCells](const std::pair<uint16_t, GridCell*>& it)->bool{
		auto _it = std::find(showCells.begin(), showCells.end(), it.first);
		return _it == showCells.end();
	});

	if (_it == m_cachedCellList.end())
		return nullptr;

	auto _tmpCell = _it->second;
	if (originIdx != nullptr)
		*originIdx = _it->first;
	m_cachedCellList.erase(_it);
	return _tmpCell;
}

int TableView::getCellIndex(const Vec2& position)
{
	if (m_totalCnt == 0)return -1;

	Rect _rect;
	for (int i = 0; i < m_totalCnt; ++i)
	{
		getCellRect(i, &_rect);
		if (_rect.containsPoint(position))
			return i;
	}
	return -1;
}

void TableView::arrangeCells(bool reload)
{
	//如果需要重新刷新，则清除掉缓存的m_showCells
	if (reload)
		m_showCells.clear();

	//获取当前哪些CELL会被显示
	std::vector<uint16_t> _showCells;
	getShowCells(Rect(-m_container->getPositionX(), -m_container->getPositionY(), _contentSize.width, _contentSize.height), &_showCells);
	
	//找出交集集合
	std::vector<uint16_t> _intersecCells;
	{
		for (auto it = _showCells.begin(); it != _showCells.end(); ++it)
		{
			auto _it = std::find(m_showCells.begin(), m_showCells.end(), *it);
			if (_it == m_showCells.end())continue;
			_intersecCells.push_back(*it);
		}
	}

	//找出差异集合
	std::vector<uint16_t> _diffCells;
	{
		for (auto it = _showCells.begin(); it != _showCells.end(); ++it)
		{
			auto _it = std::find(m_showCells.begin(), m_showCells.end(), *it);
			if (_it != m_showCells.end())continue;
			_diffCells.push_back(*it);
		}
	}

	//先让所有的缓存对象通通隐藏
	for (const auto& it : m_cachedCellList)
	{
		it.second->setVisible(false);
	}

	Rect _cellRect;

	if (!_diffCells.empty())
	{
		for (auto it = _diffCells.begin(); it != _diffCells.end(); ++it)
		{
			uint16_t _originIdx = -1;
			GridCell* _cachedCell = getAvailableCachedCell(_intersecCells, *it, &_originIdx);
			if (_cachedCell != nullptr)
			{
				if (_originIdx != *it || reload)
					m_cellForIndexCallfunc(this, _cachedCell, *it);
			}
			else
			{
				_cachedCell = m_cellForIndexCallfunc(this, _cachedCell, *it);
				m_container->addChild(_cachedCell);
			}

			_intersecCells.push_back(*it);
			m_cachedCellList[*it] = _cachedCell;

			getCellRect(*it, &_cellRect);
			_cachedCell->setPosition(_cellRect.getMidX(), _cellRect.getMidY());
			int a = 20;
		}
	}

	//让可显示的对象显示出来
	if (!_showCells.empty())
	{
		for (auto it = _showCells.begin(); it != _showCells.end(); ++it)
		{
			auto _it = m_cachedCellList.find(*it);
			if (_it == m_cachedCellList.end())continue;
			_it->second->setVisible(true);
		}
	}

	m_showCells = _showCells;
}

TableView::CachedCellList::iterator TableView::getCellNotIn(int first, int last)
{
	CachedCellList::iterator _it = m_cachedCellList.end();
	for (auto i = m_cachedCellList.begin(); i != m_cachedCellList.end(); i++)
	{
		if (i->first < first || i->first > last)
		{
			return i;
		}
	}
	return _it;
}

void TableView::onLongPress()
{
	do
	{
		CC_BREAK_IF(m_startFocusCellIndex < 0);
		auto _item = getCellByIndex(m_startFocusCellIndex);
		CC_BREAK_IF(!_item);
		_item->onLongPress();
		if (m_cellTouchCallfunc)
		{
			m_cellTouchCallfunc(this, _item, m_startFocusCellIndex, TouchType::LONG_PRESS);
		}

	} while (0);
}

void TableView::didScroll()
{
	arrangeCells();
}

void TableView::endScroll()
{
	if (m_loading)return;
	arrangeCells();
}

bool TableView::onTouchBegan(Touch* touch, Event* unusedEvent)
{
	bool _re = ScrollView::onTouchBegan(touch, unusedEvent);
	if (_re)
	{
		m_startFocusCellIndex = getCellIndex(m_container->convertTouchToNodeSpace(touch));
		GridCell* _item = getCellByIndex(m_startFocusCellIndex);
		if (_item)
		{
			_item->onFocus();
		}
	}
	return _re;
}

void TableView::onTouchMoved(Touch* touch, Event* unusedEvent)
{
	ScrollView::onTouchMoved(touch, unusedEvent);
	do
	{
		CC_BREAK_IF(m_startFocusCellIndex == -1);
		int _cellIndex = getCellIndex(m_container->convertTouchToNodeSpace(touch));
		//如果手指移动距离大于可移动值则判断无法触发点击
		if (touch->getDelta().getLength() > CLICK_MAX_MOVE_DISTANCE ||
			m_movedPos.getDistance(m_beganPos) > CLICK_MAX_MOVE_DISTANCE ||
			_cellIndex < 0 ||
			_cellIndex != m_startFocusCellIndex
			)
		{
			GridCell* _focusItem = _focusItem = getCellByIndex(m_startFocusCellIndex);
			if (_focusItem)
			{
				_focusItem->onBlur();
			}
			m_startFocusCellIndex = -1;
		}
	} while (0);
}

void TableView::onTouchEnded(Touch* touch, Event* unusedEvent)
{
	ScrollView::onTouchEnded(touch, unusedEvent);
	do
	{
		CC_BREAK_IF(m_startFocusCellIndex == -1);
		auto _focusItem = getCellByIndex(m_startFocusCellIndex);
		CC_BREAK_IF(!_focusItem);
		Rect _rect = { 0, 0, getContentSize().width, getContentSize().height };
		int32_t _cellIndex;
		if (_rect.containsPoint(this->convertTouchToNodeSpace(touch)))
			_cellIndex = getCellIndex(m_container->convertTouchToNodeSpace(touch));
		else
			_cellIndex = -1;

		if (_cellIndex == m_startFocusCellIndex)
		{
			_focusItem->onBlur();
			_focusItem->onClick();
			if (m_cellTouchCallfunc)
			{
				m_cellTouchCallfunc(this, _focusItem, m_startFocusCellIndex, TouchType::CLICK);
			}
		}
		else
			_focusItem->onBlur();
	} while (0);
}

void TableView::drawRenderer(bool updateSize)
{
	if (m_parsing)return;
	if (updateSize)
		updateSizeFromContent(Size(m_paddingLeft + m_paddingRight + m_columnCnt * getCellTotalWidth(), m_paddingTop + m_paddingBottom + m_rowCnt * getCellTotalHeight()));
}
TINA_NS_END