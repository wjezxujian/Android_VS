#pragma once

/****************************************************************************
�� �ܣ�GridView���
============================================================================
Copyright (c) 2015 Tungway
�����ߣ���ΰ
�������ڣ�2015-03-08
����޸��ߣ���ΰ
����޸����ڣ�2015-03-08
****************************************************************************/

#include "include/tinaMacros.h"
#include "node/TiNode.h"

TINA_NS_BEGIN
class WidgetReader;
class GridView;
class GridCell : public TiNode
{
public:
	enum class FocusBlurEffect
	{
		NONE = 0,
		SCALE
	};

public:
	static GridCell* create(const std::string& file = "");
	GridCell()
		: m_selected(false)
	{};
	~GridCell(){};
	void setFocusBlurEffect(FocusBlurEffect effect);
	void setSelected(bool selected);
	virtual void onFocus();
	virtual void onBlur();
	virtual void onClick(){};
	virtual void onLongPress(){};
	virtual void onSelected(bool selected){};
	bool isSelected(){ return m_selected; };

protected:
	virtual bool initWithTxml(const std::string& file) override;

protected:
	bool			m_selected;
	FocusBlurEffect	m_effect = FocusBlurEffect::NONE;
};
TINA_NS_END