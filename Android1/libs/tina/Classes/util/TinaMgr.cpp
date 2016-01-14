#include "TinaMgr.h"
#include "include/TinaConfig.h"
#include "widget/util/TXMLMgr.h"

TINA_NS_BEGIN
static TinaMgr* m_instance = nullptr;

TinaMgr* TinaMgr::getInstance()
{
	if (!m_instance)
	{
		m_instance = new TinaMgr;
		if (!m_instance->init())
		{
			CC_SAFE_DELETE(m_instance);
		}
	}
	return m_instance;
}

void TinaMgr::destroy()
{
	CC_SAFE_DELETE(m_instance);
	TXMLMgr::destroy();
}

TinaConfig* TinaMgr::getConfig()
{
	return m_config;
}

std::string TinaMgr::getImageFullPath(const std::string& file)
{
	return m_config->getImageDir() + file;
}

std::string TinaMgr::getResFullPath(const std::string& file)
{
	return m_config->getResDir() + file;
}

std::string TinaMgr::getTxmlFullPath(const std::string& file)
{
	return m_config->getTxmlDir() + file;
}

std::string TinaMgr::getAnimFullPath(const std::string& file)
{
	return m_config->getAnimDir() + file;
}

std::string TinaMgr::getFontFullPath(const std::string& file)
{
	return m_config->getFontDir() + file;
}

std::string TinaMgr::getXmlFullPath(const std::string& file)
{
	return m_config->getXmlDir() + file;
}

std::string TinaMgr::getLangFullPath(const std::string& file)
{
	return m_config->getLangDir() + file;
}

std::string TinaMgr::getSoundFullPath(const std::string& file)
{
	return m_config->getSoundDir() + file;
}

std::string TinaMgr::getShaderFullPath(const std::string& file)
{
	return m_config->getShaderDir() + file;
}

std::string TinaMgr::getParticleFullPath(const std::string& file)
{
	return m_config->getParticleDir() + file;
}

cocos2d::TTFConfig TinaMgr::getTTFConfigWithFontName(const std::string& fontName)
{
	cocos2d::TTFConfig _cfg;

	auto _fontDef = m_config->getFontDefWithName(fontName);
	if (!_fontDef)
		return _cfg;

	_cfg.fontFilePath = getFontFullPath(_fontDef->file);
	return _cfg;
}

bool TinaMgr::init()
{
	m_config = new TinaConfig;
	return true;
}

TinaMgr::~TinaMgr()
{
	if (m_config != nullptr)
		CC_SAFE_DELETE(m_config);
}
TINA_NS_END