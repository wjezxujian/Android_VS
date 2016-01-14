#include "Layout.h"

USING_NS_CC;

TINA_NS_BEGIN
const cocos2d::Size& Layout::calcVirtualSize(const cocos2d::Vector<Widget*>& children, const cocos2d::Size& givenSize)
{
	Size _contentSize;
	Size _virtualSize;

	m_percentSize = Size::ZERO;
	uint8_t _percentWidthChildCnt = 0;
	uint8_t _percentHeightChildCnt = 0;

	for (auto child : children)
	{
		if (!child->isVisible())continue;
		float _width = child->getWidth();
		float _height = child->getHeight();
		uint16_t _marginLeft = child->getMarginLeft();
		uint16_t _marginRight = child->getMarginRight();
		uint16_t _marginTop = child->getMarginTop();
		uint16_t _marginBottom = child->getMarginBottom();
		Vec2 _realAnchor = child->getRealAnchor();

		if (child->getPercentWidth() == NaN)
		{
			if (m_type == LayoutType::ABSOLUTE)
			{
				if (child->getX() != NaN || child->getLeft() != NaN)
					_width += child->getPositionX() - _realAnchor.x * child->getWidth();
				if (child->getRight() != NaN)
					_width += child->getRight();
				_contentSize.width = MAX(_contentSize.width, _width);
			}
			else if (m_type == LayoutType::HORIZONTAL)
			{
				_width += _marginLeft + _marginRight;
				if (_contentSize.width > 0)
					_width += m_hGap;
				_contentSize.width += _width;
			}
			else if (m_type == LayoutType::VERTICAL)
			{
				_width += _marginLeft + _marginRight;
				_contentSize.width = MAX(_contentSize.width, _width);
			}
			else if (m_type == LayoutType::TILE)
			{
				_width += _marginLeft + _marginRight;
				_contentSize.width = MAX(_contentSize.width, _width);
			}
		}

		if (child->getPercentHeight() == NaN)
		{
			if (m_type == LayoutType::ABSOLUTE)
			{
				if (child->getY() != NaN || child->getBottom() != NaN)
					_height += child->getPositionY() - _realAnchor.y * child->getHeight();
				if (child->getTop() != NaN)
					_height += child->getTop();
				_contentSize.height = MAX(_contentSize.height, _height);
			}
			else if (m_type == LayoutType::HORIZONTAL)
			{
				_height += _marginTop + _marginBottom;
				_contentSize.height = MAX(_contentSize.height, _height);
			}
			else if (m_type == LayoutType::VERTICAL)
			{
				_height += _marginTop + _marginBottom;
				if (_contentSize.height > 0)
					_height += m_vGap;
				_contentSize.height += _height;
			}
			else if (m_type == LayoutType::TILE)
			{
				_height += _marginTop + _marginBottom;
				_contentSize.height = MAX(_contentSize.height, _height);
			}
		}

		if (child->getPercentWidth() != NaN)
		{
			_percentWidthChildCnt++;
			m_percentSize.width += child->getPercentWidth();
		}
			
		if (child->getPercentHeight() != NaN)
		{
			_percentHeightChildCnt++;
			m_percentSize.height += child->getPercentHeight();
		}
	}

	if (m_type == LayoutType::HORIZONTAL)
	{
		if (givenSize.width < 0)
			_virtualSize.width = 0;
		else
		{
			_virtualSize.width = MAX(0, givenSize.width - m_paddingLeft - m_paddingRight - _contentSize.width - _percentWidthChildCnt * m_hGap);
		}

		if (givenSize.height < 0)
			_virtualSize.height = _contentSize.height;
		else
			_virtualSize.height = MAX(0, givenSize.height - m_paddingBottom - m_paddingTop);
	}
	else if (m_type == LayoutType::VERTICAL)
	{
		if (givenSize.height < 0)
			_virtualSize.height = 0;
		else
		{
			_virtualSize.height = MAX(0, givenSize.height - m_paddingBottom - m_paddingTop - _contentSize.height - _percentHeightChildCnt * m_vGap);
		}

		if (givenSize.width < 0)
			_virtualSize.width = _contentSize.width;
		else
			_virtualSize.width = MAX(0, givenSize.width - m_paddingLeft - m_paddingRight);
	}
	else if (m_type == LayoutType::ABSOLUTE)
	{
		if (givenSize.width < 0)
			_virtualSize.width = _contentSize.width;
		else
			_virtualSize.width = givenSize.width;

		if (givenSize.height < 0)
			_virtualSize.height = _contentSize.height;
		else
			_virtualSize.height = givenSize.height;
	}
	else if (m_type == LayoutType::TILE)
	{
		_virtualSize = _contentSize;
	}

	m_virtualSize = _virtualSize;
	return m_virtualSize;
}

Size Layout::doLayout(const Vector<Widget*>& children, const Size& givenSize)
{
	Size _containerSize;
	if (m_type == LayoutType::ABSOLUTE)
	{
		_containerSize = absoluteLayout(children, givenSize);
	}
	else if (m_type == LayoutType::HORIZONTAL)
	{
		_containerSize = horizontalLayout(children, givenSize);
	}
	else if (m_type == LayoutType::VERTICAL)
	{
		_containerSize = verticalLayout(children, givenSize);
	}
	else if (m_type == LayoutType::TILE)
	{
		_containerSize = tileLayout(children, givenSize);
	}
	return _containerSize;
}

Size Layout::absoluteLayout(const Vector<Widget*>& children, const Size& givenSize)
{
	return m_virtualSize;
}

Size Layout::horizontalLayout(const Vector<Widget*>& children, const Size& givenSize)
{
	float _contentWidth = 0;
	float _contentHeight = 0;
	for (auto child : children)
	{
		if (!child->isVisible())continue;
		if (_contentWidth > 0)
			_contentWidth += m_hGap;

		float _width = child->getWidth() + child->getMarginLeft() + child->getMarginRight();
		_contentWidth += _width;

		float _height = child->getHeight() + child->getMarginBottom() + child->getMarginTop();
		_contentHeight = MAX(_contentHeight, _height);
	}

	Size _containerSize = givenSize;
	if (givenSize.width < 0)
		_containerSize.width = _contentWidth + m_paddingLeft + m_paddingRight;
	if (givenSize.height < 0)
		_containerSize.height = _contentHeight + m_paddingTop + m_paddingBottom;

	float _baseX = m_paddingLeft;
	if (m_align == HAlign::CENTER)
	{
		_baseX += ((_containerSize.width - m_paddingLeft - m_paddingRight) - _contentWidth) * 0.5f;
	}
	else if (m_align == HAlign::RIGHT )
	{
		_baseX += (_containerSize.width - m_paddingLeft - m_paddingRight) - _contentWidth;
	}

	float _layoutWidth = _baseX;
	for (auto child : children)
	{
		if (!child->isVisible())continue;
		Vec2 _pos = Vec2::ZERO;
		float _width = child->getWidth();
		float _height = child->getHeight();
		Vec2 _anchor = child->getRealAnchor();
		uint16_t _marginLeft = child->getMarginLeft();
		uint16_t _marginRight = child->getMarginRight();
		uint16_t _marginTop = child->getMarginTop();
		uint16_t _marginBottom = child->getMarginBottom();
		_pos.x += _layoutWidth + _marginLeft + _anchor.x * _width;
		if (m_valign == VAlign::CENTER)
			_pos.y += (_containerSize.height - m_paddingBottom - m_paddingTop - _height - _marginBottom - _marginTop) * 0.5f;
		else if (m_valign == VAlign::TOP)
			_pos.y += _containerSize.height - m_paddingBottom - m_paddingTop - _height - _marginBottom - _marginTop;
		_pos.y += _anchor.y * _height + m_paddingBottom + _marginBottom;
		child->Node::setPosition(_pos.x, _pos.y);
		_layoutWidth += _width + _marginLeft + _marginRight + m_hGap;
	}

	return _containerSize;
}

Size Layout::verticalLayout(const Vector<Widget*>& children, const Size& givenSize)
{
	float _contentWidth = 0;
	float _contentHeight = 0;
	for (auto child : children)
	{
		if (!child->isVisible())continue;
		float _width = child->getWidth() + child->getMarginLeft() + child->getMarginRight();
		_contentWidth = MAX(_width, _contentWidth);

		float _height = child->getHeight() + child->getMarginBottom() + child->getMarginTop();
		if (_contentHeight > 0)
			_contentHeight += m_vGap;
		_contentHeight += _height;
	}

	Size _containerSize = givenSize;
	if (givenSize.width < 0)
		_containerSize.width = _contentWidth + m_paddingLeft + m_paddingRight;
	if (givenSize.height < 0)
		_containerSize.height = _contentHeight + m_paddingTop + m_paddingBottom;

	float _baseY = m_paddingBottom;
	if (m_valign == VAlign::CENTER)
	{
		_baseY += ((_containerSize.height - m_paddingBottom - m_paddingTop) - _contentHeight) * 0.5f;
	}
	else if (m_valign == VAlign::TOP)
	{
		_baseY += (_containerSize.height - m_paddingBottom - m_paddingTop) - _contentHeight;
	}

	float _layoutHeight = _baseY + _contentHeight;
	for (auto child : children)
	{
		if (!child->isVisible())continue;
		Vec2 _pos = Vec2::ZERO;
		float _width = child->getWidth();
		float _height = child->getHeight();
		Vec2 _anchor = child->getRealAnchor();
		uint16_t _marginLeft = child->getMarginLeft();
		uint16_t _marginRight = child->getMarginRight();
		uint16_t _marginTop = child->getMarginTop();
		uint16_t _marginBottom = child->getMarginBottom();

		_pos.x += m_paddingLeft + _marginLeft + _anchor.x * _width;
		if (m_align == HAlign::CENTER)
			_pos.x += (_containerSize.width - m_paddingLeft - m_paddingRight - _width - _marginLeft - _marginRight) * 0.5f;
		else if (m_align == HAlign::RIGHT)
			_pos.x += (_containerSize.width - m_paddingLeft - m_paddingRight - _width - _marginLeft - _marginRight);

		_pos.y += _marginBottom + _anchor.y * _height;
		_pos.y += _layoutHeight - (_height + _marginBottom + _marginTop);
		child->Node::setPosition(_pos.x, _pos.y);
		_layoutHeight -= _height + _marginBottom + _marginTop + m_vGap;
	}

	return _containerSize;
}

Size Layout::tileLayout(const Vector<Widget*>& children, const Size& givenSize)
{
	const Size& _winsize = Director::getInstance()->getWinSize();
	float _maxWidth = (givenSize.width < 0) ? _winsize.width : givenSize.width;
	_maxWidth -= m_paddingLeft + m_paddingRight;
	std::vector<std::vector<Widget*>> _tileList;
	_tileList.push_back({});
	std::vector<Size> _sizeList;
	Size _realSize;
	Size _containerSize;
	float _lineWidth = 0;
	float _lineHeight = 0;
	for (auto child : children)
	{
		if (!child->isVisible())continue;
		float _width = child->getWidth() + child->getMarginLeft() + child->getMarginRight();
		float _height = child->getHeight() + child->getMarginTop() + child->getMarginBottom();
		if (_lineWidth + _width + m_hGap > _maxWidth && !_tileList.back().empty())
		{
			_sizeList.push_back(Size(_lineWidth, _lineHeight));
			_realSize.width = MAX(_realSize.width, _lineWidth);
			_realSize.height += _lineHeight;
			if (_sizeList.size() > 1)
				_realSize.height += m_vGap;
			_tileList.push_back({});
			_lineWidth = 0;
			_lineHeight = 0;
		}
		if (_lineWidth > 0)
			_lineWidth += m_hGap;
		_lineWidth += _width;
		_lineHeight = MAX(_height, _lineHeight);
		_tileList.back().push_back(child);
	}

	_realSize.width = MAX(_realSize.width, _lineWidth);
	if (_realSize.height > 0)
		_realSize.height += m_vGap;
	_realSize.height += _lineHeight;
	_sizeList.push_back(Size(_lineWidth, _lineHeight));

	_containerSize.width = (givenSize.width < 0) ? (_realSize.width + m_paddingLeft + m_paddingRight) : givenSize.width;
	_containerSize.height = (givenSize.height < 0) ? (_realSize.height + m_paddingTop + m_paddingBottom) : givenSize.height;

	float _baseY = m_paddingBottom;
	if (m_valign == VAlign::TOP)
		_baseY = MAX(0, _containerSize.height - m_paddingBottom - m_paddingTop) - _realSize.height;
	else if (m_valign == VAlign::CENTER)
		_baseY = (MAX(0, _containerSize.height - m_paddingBottom - m_paddingTop) - _realSize.height) * 0.5f;

	float _drawHeight = 0;

	for (uint16_t i = 0; i < _tileList.size(); ++i)
	{
		const auto& _lineChildList = _tileList.at(i);
		float _baseX = m_paddingLeft;
		if (m_align == HAlign::CENTER)
			_baseX += (MAX(0, _containerSize.width - m_paddingLeft - m_paddingRight) - _sizeList.at(i).width) * 0.5f;
		else if (m_align == HAlign::RIGHT)
			_baseX += MAX(0, _containerSize.width - m_paddingLeft - m_paddingRight) - _sizeList.at(i).width;
		_lineWidth = 0;
		_lineHeight = _sizeList.at(i).height;

		for (auto child : _lineChildList)
		{
			float _width = child->getContentSize().width;
			float _height = child->getContentSize().height;
			uint16_t _marginLeft = child->getMarginLeft();
			uint16_t _marginRight = child->getMarginRight();
			uint16_t _marginTop = child->getMarginTop();
			uint16_t _marginBottom = child->getMarginBottom();
			Vec2 _anchor = child->getRealAnchor();
			Vec2 _pos;
			_pos.x = _lineWidth + _marginLeft + _anchor.x * _width;
			_pos.x += _baseX;
			_pos.y = _realSize.height - _drawHeight - _lineHeight + _lineHeight - (1 - _anchor.y) * _height - _marginTop;
			_pos.y += _baseY;
			child->Node::setPosition(_pos.x, _pos.y);
			_lineWidth += m_hGap + _marginLeft + _width + _marginRight;
		}
		_drawHeight += _lineHeight + m_vGap;
	}

	return _containerSize;
}
TINA_NS_END