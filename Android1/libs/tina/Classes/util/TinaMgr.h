#pragma once

/****************************************************************************
功 能：Tina数据管理器
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-03-01
最后修改者：董伟
最后修改日期：2015-03-01
****************************************************************************/

#include "include/tinaMacros.h"
#include "include/TinaConfig.h"
#include <string>

TINA_NS_BEGIN

#define TINA_MGR TinaMgr::getInstance()
class TinaMgr
{
public:
	static TinaMgr* getInstance();
	static void destroy();
	TinaConfig* getConfig();

public:
	std::string getImageFullPath(const std::string& file);
	std::string getResFullPath(const std::string& file);
	std::string getTxmlFullPath(const std::string& file);
	std::string getAnimFullPath(const std::string& file);
	std::string getFontFullPath(const std::string& file);
	std::string getXmlFullPath(const std::string& file);
	std::string getLangFullPath(const std::string& file);
	std::string getSoundFullPath(const std::string& file);
	std::string getShaderFullPath(const std::string& file);
	std::string getParticleFullPath(const std::string& file);
	cocos2d::TTFConfig getTTFConfigWithFontName(const std::string& fontName);

private:
	bool init();
	~TinaMgr();

private:
	TinaConfig*		m_config;
};
TINA_NS_END