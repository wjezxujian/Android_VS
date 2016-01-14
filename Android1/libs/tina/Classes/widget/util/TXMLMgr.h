#pragma once

/****************************************************************************
功 能：TXML读取与管理器
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-03-01
最后修改者：董伟
最后修改日期：2015-03-01
****************************************************************************/

#include "include/tinaMacros.h"
#include "cocos2d.h"
#include "rapidxml.h"

TINA_NS_BEGIN
class TXMLData : public cocos2d::Ref
{
public:
	static TXMLData* create();
	static TXMLData* create(const std::string& bytes);
	void setBytes(const std::string& bytes);
	rapidxml::xml_document<>* getXml();

private:
	bool init(const std::string& bytes);
	~TXMLData();

private:
	std::string					m_bytes;
	rapidxml::xml_document<>	m_xmldoc;
};

class TXMLMgr
{
public:
	static TXMLMgr* getInstance();
	static void destroy();
	TXMLData* loadTXML(const std::string& file, bool fromCache = true);
	void removeTXMLByKey(const std::string& key);
	void removeAllTXML();

private:
	bool init();

private:
	cocos2d::Map<std::string, TXMLData*>	m_dict;
};
TINA_NS_END