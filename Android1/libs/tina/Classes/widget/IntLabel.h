#pragma once

/****************************************************************************
功 能：整型文本类
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-03-16
最后修改者：董伟
最后修改日期：2015-03-16
****************************************************************************/

#include "Label.h"

TINA_NS_BEGIN
class IntLabel : public Label
{
public:
	static IntLabel* create();
	static IntLabel* create(int num, const std::string& fontName, int fontSize);
	virtual void parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values) override;

public:
	virtual void setNumber(int number);
	int getNumber();
	virtual void setDuration(float duration){ m_duration = duration; };
	float getDuration(){ return m_duration; };

protected:
	virtual bool init(int number, const std::string& fontName, int fontSize);
	virtual bool init();
	void exeSetNumber(float);

protected:
	float				m_dtMax;
	float				m_dt;
	float				m_duration = 1.f;
	int					m_newNum;
	int					m_originNum;

};
TINA_NS_END