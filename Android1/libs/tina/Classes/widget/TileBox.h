#pragma once

/****************************************************************************
�� �ܣ���Ƭ������
============================================================================
Copyright (c) 2015 Tungway
�����ߣ���ΰ
�������ڣ�2015-03-14
����޸��ߣ���ΰ
����޸����ڣ�2015-03-14
****************************************************************************/

#include "base/BaseBox.h"

TINA_NS_BEGIN
class TileBox : public BaseBox
{
public:
	static TileBox* create();
	virtual void parseAttr(std::vector<std::string>* keys, std::vector<std::string>* values) override;

protected:
	virtual bool init();
};
TINA_NS_END