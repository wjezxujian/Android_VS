#include "GridView.h"
#include "util/ParamUtil.h"
#include "Box.h"


USING_NS_CC;

TINA_NS_BEGIN

#define CLICK_MAX_MOVE_DISTANCE (20)
#define LONG_PRESS_INTERVAL (1.f)

GridView* GridView::create()
{
	auto _ref = new GridView();
	if (_ref && _ref->init())
	{
		_ref->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(_ref);
	}
	return _ref;
}

GridView* GridView::create(ScrollDirection direction, uint16_t columnCnt, uint16_t rowCnt, uint16_t hspacing, uint16_t vspacing)
{
	auto _ref = GridView::create();
	if (_ref != nullptr)
	{
		_ref->setScrollDirection(direction);
		_ref->setColumnCnt(columnCnt);
		_ref->setRowCnt(rowCnt);
		_ref->setHSpacing(hspacing);
		_ref->setVSpacing(vspacing);
	}
	return _ref;
}

void GridView::parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values)
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
		else if (_key == "testmode")
		{
			setTestMode(ParamUtil::readBool(_value));
		}
		else if (_key == "template" || _key == "templatefile")
		{
			setTemplateFile(_value);
		}
		else if (_key == "testtotalcnt" || _key == "totalcnt")
		{
			setTestTotalCnt(ParamUtil::readUInt(_value));
		}
	}
}

void GridView::setScrollDirection(ScrollDirection direction)
{
	ScrollView::setScrollDirection(direction);
	DRAW_RENDERER;
}

void GridView::setHSpacing(uint16_t spacing)
{
	if (m_hSpacing == spacing)return;
	m_hSpacing = spacing;
	DRAW_RENDERER;
}

void GridView::setVSpacing(uint16_t spacing)
{
	if (m_vSpacing == spacing)return;
	m_vSpacing = spacing;
	DRAW_RENDERER;
}

void GridView::setPaddingTop(uint16_t padding)
{
	if (m_paddingTop == padding)return;
	m_paddingTop = padding;
	DRAW_RENDERER;
}

void GridView::setPaddingRight(uint16_t padding)
{
	if (m_paddingRight == padding)return;
	m_paddingRight = padding;
	DRAW_RENDERER;
}

void GridView::setPaddingBottom(uint16_t padding)
{
	if (m_paddingBottom == padding)return;
	m_paddingBottom = padding;
	DRAW_RENDERER;
}

void GridView::setPaddingLeft(uint16_t padding)
{
	if (m_paddingLeft == padding)return;
	m_paddingLeft = padding;
	DRAW_RENDERER;
}

void GridView::setPadding(uint16_t padding)
{
	m_paddingTop = padding;
	m_paddingRight = padding;
	m_paddingBottom = padding;
	m_paddingLeft = padding;
	DRAW_RENDERER;
}

void GridView::setCellSize(const Size& size)
{
	if (m_cellSize.equals(size))return;
	m_cellSize = size;
	DRAW_RENDERER;
}

void GridView::setCellSize(int width, int height)
{
	m_cellSize.width = width;
	m_cellSize.height = height;
	DRAW_RENDERER;
}

void GridView::setPageMode(bool pageMode)
{
	if (m_pageMode == pageMode)return;
	m_pageMode = pageMode;
	DRAW_RENDERER;
}

void GridView::setColumnCnt(uint16_t cnt)
{
	if (m_columnCnt == cnt)return;
	m_columnCnt = cnt;
	DRAW_RENDERER;
}

void GridView::setRowCnt(uint16_t cnt)
{
	if (m_rowCnt == cnt)return;
	m_rowCnt = cnt;
	DRAW_RENDERER;
}

void GridView::setTestMode(bool mode)
{
	if (m_testMode == mode)return;
	m_testMode = mode;
	DRAW_RENDERER;
}

void GridView::setTemplateFile(const std::string& file)
{
	if (m_templateFile == file)return;
	m_templateFile = file;
	DRAW_RENDERER;
}

void GridView::setTestTotalCnt(uint32_t cnt)
{
	if (m_testTotalCnt == cnt)return;
	m_testTotalCnt = cnt;
	DRAW_RENDERER;
}

void GridView::selectCell(uint32_t cellIdx)
{
	auto _cellRect = getCellRect(cellIdx);

	for (auto it = m_cachedCellList.begin(); it != m_cachedCellList.end(); ++it)
	{
		it->second->onSelected(cellIdx == it->first);
	}
}

void GridView::testLoad()
{
	if (!m_testMode)return;
	m_loading = true;
	m_totalCnt = m_testTotalCnt;

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

void GridView::load()
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

void GridView::fresh()
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
	m_loading = true;
	scrollTo(_newPos - m_container->getPosition(), false);
	arrangeCells(true);
	m_loading = false;
}

void GridView::pageTo(uint32_t pageIndex, bool withAnimate, float speed)
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

void GridView::previous(uint32_t itemNum, bool withAnimate, float speed)
{
	Vec2 _offset;
	if (m_scrollDirection == ScrollDirection::HORIZONTAL)
		_offset.x = getCellTotalWidth() * itemNum;
	else
		_offset.y = -getCellTotalHeight() * itemNum;
	ScrollView::scrollTo(_offset, withAnimate, speed);
}

void GridView::next(uint32_t itemNum, bool withAnimate, float speed)
{
	Vec2 _offset;
	if (m_scrollDirection == ScrollDirection::HORIZONTAL)
		_offset.x = -getCellTotalWidth() * itemNum;
	else
		_offset.y = getCellTotalHeight() * itemNum;
	ScrollView::scrollTo(_offset, withAnimate, speed);
}

GridCell* GridView::getCellByIndex(uint32_t index)
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

void GridView::setSorptionMode(bool sorption)
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

bool GridView::isBegin()
{
	do
	{
		CC_BREAK_IF(m_totalCnt <= m_columnCnt * m_rowCnt);
		return ScrollView::isBegin();
	} while (0);
	return true;
}

bool GridView::isEnd()
{
	do
	{
		CC_BREAK_IF(m_totalCnt <= m_columnCnt * m_rowCnt);
		return ScrollView::isEnd();
	} while (0);
	return true;
}

GridView::CachedCellList* GridView::getCachedCells()
{
	return &m_cachedCellList;
}

uint32_t GridView::getPageCount()
{
	return std::ceil(1.f * m_totalCnt / (m_rowCnt * m_columnCnt));
}

uint32_t GridView::getPageIndex()
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

bool GridView::init()
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

void GridView::getShowCells(const cocos2d::Rect& area, std::vector<uint16_t>* dst)
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

Rect GridView::getCellRect(uint32_t idx)
{
	Rect _rect;
	getCellRect(idx, &_rect);
	return _rect;
}

void GridView::getCellRect(uint32_t idx, cocos2d::Rect* dstRect)
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

cocos2d::Size GridView::calcContainerSize()
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

GridCell* GridView::getAvailableCachedCell(const std::vector<uint16_t>& showCells, uint16_t idx, uint16_t* originIdx)
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

GridCell* GridView::requestCell(GridCell* cell, uint32_t idx)
{
	GridCell* _cell = cell;
	if (!m_testMode)
		_cell = m_cellForIndexCallfunc(this, cell, idx);
	else
	{
		if (_cell == nullptr)
			_cell = GridCell::create(m_templateFile);
	}
	return _cell;
}

int GridView::getCellIndex(const Vec2& position)
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

void GridView::arrangeCells(bool reload)
{
	//如果需要重新刷新，则清除掉缓存的m_showCells
	if (reload)
	{
		m_showCells.clear();
		auto _tmpCachedCells = m_cachedCellList;
		m_cachedCellList.clear();
		for (const auto& it : _tmpCachedCells)
		{
			m_cachedCellList[it.first + 100] = it.second;
		}
	}

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
					requestCell(_cachedCell, *it);
			}
			else
			{
				_cachedCell = requestCell(_cachedCell, *it);
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

	if (reload)
	{
		for (const auto& it : m_cachedCellList)
		{
			it.second->onSelected(false);
		}
	}

	m_showCells = _showCells;
}

GridView::CachedCellList::iterator GridView::getCellNotIn(int first, int last)
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

void GridView::onLongPress()
{
	do
	{
		CC_BREAK_IF(m_startFocusCellIndex < 0);
		auto _item = getCellByIndex(m_startFocusCellIndex);
		CC_BREAK_IF(!_item);
		_item->onLongPress();
		if (m_cellTouchCallfunc)
		{
			m_triggerLongPress = true;
			m_cellTouchCallfunc(this, _item, m_startFocusCellIndex, TouchType::LONG_PRESS);
		}

	} while (0);
}

void GridView::didScroll()
{
	arrangeCells();
}

void GridView::endScroll()
{
	if (m_loading)return;
	arrangeCells();
}

bool GridView::onTouchBegan(Touch* touch, Event* unusedEvent)
{
	bool _re = ScrollView::onTouchBegan(touch, unusedEvent);
	if (_re)
	{
		m_triggerLongPress = false;
		m_startFocusCellIndex = getCellIndex(m_container->convertTouchToNodeSpace(touch));
		GridCell* _item = getCellByIndex(m_startFocusCellIndex);
		if (_item)
		{
			_item->onFocus();
		}
	}
	return _re;
}

void GridView::onTouchMoved(Touch* touch, Event* unusedEvent)
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
				if (m_cellTouchCallfunc)
				{
					m_cellTouchCallfunc(this, _focusItem, m_startFocusCellIndex, TouchType::MOVE_OUT);
				}
				_focusItem->onBlur();
			}
			m_startFocusCellIndex = -1;
		}
	} while (0);
}

void GridView::onTouchEnded(Touch* touch, Event* unusedEvent)
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
			_focusItem->onSelected(true);

			for (const auto& it : m_cachedCellList)
			{
				if (!it.second->isVisible() || it.second == _focusItem)
					continue;
				it.second->onSelected(false);
			}
			if (m_cellTouchCallfunc)
			{
				m_cellTouchCallfunc(this, _focusItem, m_startFocusCellIndex, m_triggerLongPress ? TouchType::RELEASE : TouchType::CLICK);
			}
		}
		else
			_focusItem->onBlur();
	} while (0);
}

void GridView::drawRenderer(bool updateSize)
{
	if (m_parsing)return;
	if (updateSize)
		updateSizeFromContent(Size(m_paddingLeft + m_paddingRight + m_columnCnt * getCellTotalWidth(), m_paddingTop + m_paddingBottom + m_rowCnt * getCellTotalHeight()));
}

void GridView::onEnter()
{
	ScrollView::onEnter();
	if (m_testMode)
		testLoad();
}
TINA_NS_END