#include "LayoutUtil.h"

USING_NS_CC;

TINA_NS_BEGIN
Size LayoutUtil::layout(const LayoutParam& param, const Size& givenSize, const Vector<Widget*>& widgetList)
{
	Size _containerSize;
	if (param.type == LayoutType::ABSOLUTE)
	{
		_containerSize = layoutAbsolute(param, givenSize, widgetList);
	}
	else if (param.type == LayoutType::VERTICAL)
	{
		_containerSize = layoutVertical(param, givenSize, widgetList);
	}
	else if (param.type == LayoutType::HORIZONTAL)
	{
		_containerSize = layoutHorizontal(param, givenSize, widgetList);
	}
	else if (param.type == LayoutType::TILE)
	{
		_containerSize = layoutTile(param, givenSize, widgetList);
	}
	return _containerSize;
}

Size LayoutUtil::layoutTest(const LayoutParam& param, const Size& givenSize, const Vector<Widget*>& widgetList)
{
	Size _size;
	if (param.type == LayoutType::VERTICAL)
	{
		_size.height = param.vGap * (widgetList.size() - 1);
		for (auto child : widgetList)
		{
			if (!child->isVisible())continue;
			_size.width = MAX(_size.width, child->getWidth() + child->getMarginLeft() + child->getMarginRight());
			_size.height += child->getHeight() + child->getMarginTop() + child->getMarginBottom();
		}
	}
	else if (param.type == LayoutType::HORIZONTAL)
	{
		_size.width = param.hGap * (widgetList.size() - 1);
		for (auto child : widgetList)
		{
			if (!child->isVisible())continue;
			_size.width += child->getWidth() + child->getMarginLeft() + child->getMarginRight();
			_size.height = MAX(_size.height, child->getHeight() + child->getMarginTop() + child->getMarginBottom());
		}
	}
	else if (param.type == LayoutType::TILE)
	{
		const Size& _winsize = Director::getInstance()->getWinSize();
		float _maxWidth = (givenSize.width < 0) ? _winsize.width : givenSize.width;
		Size _realSize;
		float _lineWidth = 0;
		float _lineHeight = 0;
		for (auto child : widgetList)
		{
			if (!child->isVisible())continue;
			float _width = child->getContentSize().width + child->getMarginLeft() + child->getMarginRight();
			float _height = child->getContentSize().height + child->getMarginTop() + child->getMarginBottom();
			if (_lineWidth + _width + param.hGap > _maxWidth)
			{
				_realSize.width = MAX(_realSize.width, _lineWidth);
				_realSize.height += _lineHeight;
				if (_realSize.height > 0)
					_realSize.height += param.vGap;
				_lineWidth = 0;
				_lineHeight = 0;
			}
			if (_lineWidth > 0)
				_lineWidth += param.hGap;
			_lineWidth += _width;
			_lineHeight = MAX(_height, _lineHeight);
		}
		_realSize.width = MAX(_realSize.width, _lineWidth);
		if (_realSize.height > 0)
			_realSize.height += param.vGap;
		_realSize.height += _lineHeight;
		_size = _realSize;
	}
	else if (param.type == LayoutType::ABSOLUTE)
	{
		for (auto child : widgetList)
		{
			auto _left = child->getLeft();
			auto _bottom = child->getBottom();
			auto _width = MAX(child->getWidth() + _left, 0);
			auto _height = MAX(child->getHeight() + _bottom, 0);
			_size.width = MAX(_width, _size.width);
			_size.height = MAX(_height, _size.height);
		}
	}
	return _size;
}

Size LayoutUtil::layout(const LayoutParam& param, const Size& givenSize, const Vector<Node*>& nodeList)
{
	Size _containerSize;

	if (param.type == LayoutType::VERTICAL)
	{
		_containerSize = layoutVertical(param, givenSize, nodeList);
	}
	else if (param.type == LayoutType::HORIZONTAL)
	{
		_containerSize = layoutHorizontal(param, givenSize, nodeList);
	}
	return _containerSize;
}


Size LayoutUtil::layoutVertical(const LayoutParam& param, const Size& givenSize, const Vector<Widget*>& widgetList)
{
	Size _containerSize;
	float _lineWidth = 0;
	float _lineHeight = param.vGap * (widgetList.size() - 1);
	for (auto child : widgetList)
	{
		if (!child->isVisible())continue;
		_lineWidth = MAX(_lineWidth, child->getWidth() + child->getMarginLeft() + child->getMarginRight());
		_lineHeight += child->getHeight() + child->getMarginTop() + child->getMarginBottom();
	}

	_containerSize.width = (givenSize.width < 0) ? _lineWidth : givenSize.width;
	_containerSize.height = (givenSize.height < 0) ? _lineHeight : givenSize.height;

	float _bottomPadding = 0;
	if (param.vAlign == VAlign::TOP)
		_bottomPadding = _containerSize.height - _lineHeight;
	else if (param.vAlign == VAlign::CENTER)
		_bottomPadding = (_containerSize.height - _lineHeight) * 0.5f;

	float _drawHeight = 0;
	for (int i = 0; i < widgetList.size(); ++i)
	{
		Widget* _child = widgetList.at(i);
		//如果自对象visible为false，则不参与排版
		if (!_child->isVisible())continue;

		Vec2 _pos = { 0, _bottomPadding };
		float _width = _child->getWidth();
		float _height = _child->getHeight();
		uint16_t _marginLeft = _child->getMarginLeft();
		uint16_t _marginRight = _child->getMarginRight();
		uint16_t _marginTop = _child->getMarginTop();
		uint16_t _marginBottom = _child->getMarginBottom();
		Vec2 _anchor = _child->getRealAnchor();

		switch (param.hAlign)
		{
		case HAlign::RIGHT:
			_pos.x = _containerSize.width - (1 - _anchor.x) * _width - _marginRight;
			break;
		case HAlign::CENTER:
			_pos.x = (_containerSize.width - _width - _marginLeft - _marginRight) * 0.5f + _marginLeft + _anchor.x * _width;
			break;
		case HAlign::LEFT:
			_pos.x = _marginLeft + _anchor.x * _width;
			break;
		default:
			break;
		}
		_pos.y += _lineHeight - _drawHeight - (1 - _anchor.y) * _height - _marginTop;
		_child->Node::setPosition(_pos.x, _pos.y);

		float _widgetTotalHeight = _height + _marginBottom + _marginTop;
		_drawHeight += _widgetTotalHeight + param.vGap;
	}

	return _containerSize;
}

cocos2d::Size LayoutUtil::layoutVertical(const LayoutParam& param, const Size& givenSize, const Vector<Node*>& nodeList)
{
	Size _containerSize;
	float _lineWidth = 0;
	float _lineHeight = param.vGap * (nodeList.size() - 1);
	for (auto child : nodeList)
	{
		if (!child->isVisible())continue;
		_lineWidth = MAX(_lineWidth, child->getContentSize().width);
		_lineHeight += child->getContentSize().height;
	}

	_containerSize.width = (givenSize.width < 0) ? _lineWidth : givenSize.width;
	_containerSize.height = (givenSize.height < 0) ? _lineHeight : givenSize.height;

	float _bottomPadding = 0;
	if (param.vAlign == VAlign::TOP)
		_bottomPadding = _containerSize.height - _lineHeight;
	else if (param.vAlign == VAlign::CENTER)
		_bottomPadding = (_containerSize.height - _lineHeight) * 0.5f;

	float _drawHeight = 0;
	for (int i = 0; i < nodeList.size(); ++i)
	{
		Node* _child = nodeList.at(i);
		//如果自对象visible为false，则不参与排版
		if (!_child->isVisible())continue;

		Vec2 _pos = { 0, _bottomPadding };
		float _width = _child->getContentSize().width;
		float _height = _child->getContentSize().height;
		uint16_t _marginLeft = 0;
		uint16_t _marginRight = 0;
		uint16_t _marginTop = 0;
		uint16_t _marginBottom = 0;
		Vec2 _anchor = getRealAnchor(_child);

		switch (param.hAlign)
		{
		case HAlign::RIGHT:
			_pos.x = _containerSize.width - (1 - _anchor.x) * _width - _marginRight;
			break;
		case HAlign::CENTER:
			_pos.x = (_containerSize.width - _width - _marginLeft - _marginRight) * 0.5f + _marginLeft + _anchor.x * _width;
			break;
		case HAlign::LEFT:
			_pos.x = _marginLeft + _anchor.x * _width;
			break;
		default:
			break;
		}
		_pos.y += _lineHeight - _drawHeight - (1 - _anchor.y) * _height - _marginTop;
		_child->Node::setPosition(_pos.x, _pos.y);

		float _widgetTotalHeight = _height + _marginBottom + _marginTop;
		_drawHeight += _widgetTotalHeight + param.vGap;
	}

	return _containerSize;
}

Size LayoutUtil::layoutHorizontal(const LayoutParam& param, const Size& givenSize, const Vector<Widget*>& widgetList)
{
	Size _containerSize;
	float _lineWidth = param.hGap * (widgetList.size() - 1);
	float _lineHeight = 0;
	for (auto child : widgetList)
	{
		if (!child->isVisible())continue;
		_lineWidth += child->getWidth() + child->getMarginLeft() + child->getMarginRight();
		_lineHeight = MAX(_lineHeight, child->getHeight() + child->getMarginTop() + child->getMarginBottom());
	}

	_containerSize.width = (givenSize.width < 0) ? _lineWidth : givenSize.width;
	_containerSize.height = (givenSize.height < 0) ? _lineHeight : givenSize.height;

	float _leftPadding = 0;
	if (param.hAlign == HAlign::RIGHT)
		_leftPadding = _containerSize.width - _lineWidth;
	else if (param.hAlign == HAlign::CENTER)
		_leftPadding = (_containerSize.width - _lineWidth) * 0.5f;
	
	float _drawWidth = 0;
	for (int i = 0; i < widgetList.size(); ++i)
	{
		Widget* _child = widgetList.at(i);
		//如果自对象visible为false，则不参与排版
		if (!_child->isVisible())continue;

		Vec2 _pos = { _leftPadding, 0 };
		float _width = _child->getWidth();
		float _height = _child->getHeight();
		uint16_t _marginLeft = _child->getMarginLeft();
		uint16_t _marginRight = _child->getMarginRight();
		uint16_t _marginTop = _child->getMarginTop();
		uint16_t _marginBottom = _child->getMarginBottom();
		Vec2 _anchor = _child->getRealAnchor();

		switch (param.vAlign)
		{
		case VAlign::TOP:
			_pos.y = _containerSize.height - (1 - _anchor.y) * _height - _marginTop;
			break;
		case VAlign::CENTER:
			_pos.y = (_containerSize.height - _height - _marginTop - _marginBottom) * 0.5f + _marginBottom + _anchor.y * _height;
			break;
		case VAlign::BOTTOM:
			_pos.y = _marginBottom + _anchor.y * _height;
			break;
		default:
			break;
		}

		_pos.x += _drawWidth + _anchor.x * _width;
		_pos.x += _marginLeft;
		if (_drawWidth > 0)
			_pos.x += param.hGap;
		_child->Node::setPosition(_pos.x, _pos.y);

		float _widgetTotalWidth = _width + _marginLeft + _marginRight;
		if (_drawWidth > 0)
			_widgetTotalWidth += param.hGap;
		_drawWidth += _widgetTotalWidth;
	}

	return _containerSize;
}

Size LayoutUtil::layoutHorizontal(const LayoutParam& param, const Size& givenSize, const Vector<Node*>& nodeList)
{
	Size _containerSize;
	float _lineWidth = param.hGap * (nodeList.size() - 1);
	float _lineHeight = 0;
	for (auto child : nodeList)
	{
		if (!child->isVisible())continue;
		_lineWidth += child->getContentSize().width;
		_lineHeight = MAX(_lineHeight, child->getContentSize().height);
	}

	_containerSize.width = (givenSize.width < 0) ? _lineWidth : givenSize.width;
	_containerSize.height = (givenSize.height < 0) ? _lineHeight : givenSize.height;

	float _leftPadding = 0;
	if (param.hAlign == HAlign::RIGHT)
		_leftPadding = _containerSize.width - _lineWidth;
	else if (param.hAlign == HAlign::CENTER)
		_leftPadding = (_containerSize.width - _lineWidth) * 0.5f;

	float _drawWidth = 0;
	for (int i = 0; i < nodeList.size(); ++i)
	{
		Node* _child = nodeList.at(i);
		//如果自对象visible为false，则不参与排版
		if (!_child->isVisible())continue;

		Vec2 _pos = { _leftPadding, 0 };
		float _width = _child->getContentSize().width;
		float _height = _child->getContentSize().height;
		uint16_t _marginLeft = 0;
		uint16_t _marginRight = 0;
		uint16_t _marginTop = 0;
		uint16_t _marginBottom = 0;
		Vec2 _anchor = getRealAnchor(_child);

		switch (param.vAlign)
		{
		case VAlign::TOP:
			_pos.y = _containerSize.height - (1 - _anchor.y) * _height - _marginTop;
			break;
		case VAlign::CENTER:
			_pos.y = (_containerSize.height - _height - _marginTop - _marginBottom) * 0.5f + _marginBottom + _anchor.y * _height;
			break;
		case VAlign::BOTTOM:
			_pos.y = _marginBottom + _anchor.y * _height;
			break;
		default:
			break;
		}

		_pos.x += _drawWidth + _anchor.x * _width;
		_pos.x += _marginLeft;
		if (_drawWidth > 0)
			_pos.x += param.hGap;
		_child->Node::setPosition(_pos.x, _pos.y);

		float _widgetTotalWidth = _width + _marginLeft + _marginRight;
		if (_drawWidth > 0)
			_widgetTotalWidth += param.hGap;
		_drawWidth += _widgetTotalWidth;
	}

	return _containerSize;
}

Size LayoutUtil::layoutAbsolute(const LayoutParam& param, const Size& givenSize, const Vector<Widget*>& widgetList)
{
	Size _containerSize;
	Size _realSize;
	for (auto child : widgetList)
	{
		auto _left = child->getLeft();
		auto _bottom = child->getBottom();
		auto _width = MAX(child->getWidth() + _left, 0);
		auto _height = MAX(child->getHeight() + _bottom, 0);
		_realSize.width = MAX(_width, _realSize.width);
		_realSize.height = MAX(_height, _realSize.height);
	}
	_containerSize.width = (givenSize.width < 0) ? _realSize.width : givenSize.width;
	_containerSize.height = (givenSize.height < 0) ? _realSize.height : givenSize.height;
	return _containerSize;
}

cocos2d::Size LayoutUtil::layoutTile(const LayoutParam& param, const Size& givenSize, const Vector<Widget*>& widgetList)
{
	const Size& _winsize = Director::getInstance()->getWinSize();
	float _maxWidth = (givenSize.width < 0) ? _winsize.width : givenSize.width;
	std::vector<std::vector<Widget*>> _tileList;
	_tileList.push_back({});
	std::vector<Size> _sizeList;
	Size _realSize;
	Size _containerSize;
	float _lineWidth = 0;
	float _lineHeight = 0;
	for (auto child : widgetList)
	{
		if (!child->isVisible())continue;
		float _width = child->getContentSize().width + child->getMarginLeft() + child->getMarginRight();
		float _height = child->getContentSize().height + child->getMarginTop() + child->getMarginBottom();
		if (_lineWidth + _width + param.hGap > _maxWidth && !_tileList.back().empty())
		{
			_sizeList.push_back(Size(_lineWidth, _lineHeight));
			_realSize.width = MAX(_realSize.width, _lineWidth);
			_realSize.height += _lineHeight;
			if (_sizeList.size() > 1)
				_realSize.height += param.vGap;
			_tileList.push_back({});
			_lineWidth = 0;
			_lineHeight = 0;
		}
		if (_lineWidth > 0)
			_lineWidth += param.hGap;
		_lineWidth += _width;
		_lineHeight = MAX(_height, _lineHeight);
		_tileList.back().push_back(child);
	}
	_realSize.width = MAX(_realSize.width, _lineWidth);
	if (_realSize.height > 0)
		_realSize.height += param.vGap;
	_realSize.height += _lineHeight;
	_sizeList.push_back(Size(_lineWidth, _lineHeight));

	_containerSize.width = (givenSize.width < 0) ? _realSize.width : givenSize.width;
	_containerSize.height = (givenSize.height < 0) ? _realSize.height : givenSize.height;

	float _bottomPadding = 0;
	if (param.vAlign == VAlign::TOP)
		_bottomPadding = _containerSize.height - _realSize.height;
	else if (param.vAlign == VAlign::CENTER)
		_bottomPadding = (_containerSize.height - _realSize.height) * 0.5f;

	float _drawHeight = 0;

	for (uint16_t i = 0; i < _tileList.size(); ++i)
	{
		const auto& _lineChildList = _tileList.at(i);
		float _leftPadding = 0;
		if (param.hAlign == HAlign::CENTER)
			_leftPadding = (_containerSize.width - _sizeList.at(i).width) * 0.5f;
		else if (param.hAlign == HAlign::RIGHT)
			_leftPadding = _containerSize.width - _sizeList.at(i).width;
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
			_pos.x += _leftPadding;
			_pos.y = _realSize.height - _drawHeight - _lineHeight + _lineHeight - (1-_anchor.y) * _height - _marginTop;
			_pos.y += _bottomPadding;
			child->Node::setPosition(_pos.x, _pos.y);
			_lineWidth += param.hGap + _marginLeft + _width + _marginRight;
		}
		_drawHeight += _lineHeight + param.vGap;
	}

	return _containerSize;
}

Vec2 LayoutUtil::getRealAnchor(cocos2d::Node* node)
{
	if (node->isIgnoreAnchorPointForPosition())
		return Vec2::ZERO;
	else
		return node->getAnchorPoint();
}
TINA_NS_END