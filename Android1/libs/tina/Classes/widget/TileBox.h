#pragma once

/****************************************************************************
功 能：瓦片布局类
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-03-14
最后修改者：董伟
最后修改日期：2015-03-14
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