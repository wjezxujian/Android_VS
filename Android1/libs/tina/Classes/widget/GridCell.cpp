#include "GridCell.h"
#include "GridView.h"
#include "util/WidgetReader.h"

USING_NS_CC;

TINA_NS_BEGIN
#define ACTION_SCALE_TAG 90001

GridCell* GridCell::create(const std::string& file)
{
	auto _ref = new GridCell;
	if (file.empty())
	{
		if (!_ref || !_ref->init())
			CC_SAFE_DELETE(_ref);
		else
			_ref->autorelease();
	}
	else
	{
		if (!_ref || !_ref->initWithTxml(file))
			CC_SAFE_DELETE(_ref);
		else
			_ref->autorelease();
	}
	return _ref;
}

void GridCell::setFocusBlurEffect(FocusBlurEffect effect)
{ 
	m_effect = effect; 
}

void GridCell::setSelected(bool selected)
{
	onSelected(selected);
}

void GridCell::onFocus()
{
	if (m_effect == FocusBlurEffect::SCALE)
	{
		this->stopAllActionsByTag(ACTION_SCALE_TAG);
		auto _action = ScaleTo::create(0.06f, 0.98f);
		_action->setTag(ACTION_SCALE_TAG);
		this->runAction(_action);
	}
}

void GridCell::onBlur()
{
	if (m_effect == FocusBlurEffect::SCALE)
	{
		this->stopAllActionsByTag(ACTION_SCALE_TAG);
		auto _action = ScaleTo::create(0.06f, 1.f);
		_action->setTag(ACTION_SCALE_TAG);
		this->runAction(_action);
	}
}

bool GridCell::initWithTxml(const std::string& file)
{
	if (!TiNode::initWithTxml(file))
	{
		return false;
	}
	setFocusBlurEffect(FocusBlurEffect::SCALE);
	return true;
}
TINA_NS_END