#include "TXMLMgr.h"

USING_NS_CC;

TINA_NS_BEGIN

TXMLData* TXMLData::create()
{
	return create(nullptr);
}

TXMLData* TXMLData::create(const std::string& bytes)
{
	auto _ref = new TXMLData();
	if (_ref && _ref->init(bytes))
	{
		_ref->autorelease();
		return _ref;
	}
	CC_SAFE_DELETE(_ref);
	return nullptr;
}

void TXMLData::setBytes(const std::string& bytes)
{
	m_bytes = bytes;
	if (!m_bytes.empty())
		m_xmldoc.parse<0>(const_cast<char*>(m_bytes.c_str()));
}

rapidxml::xml_document<>* TXMLData::getXml()
{
	return &m_xmldoc;
}

bool TXMLData::init(const std::string& bytes)
{
	m_bytes = bytes;
	if (!m_bytes.empty())
		m_xmldoc.parse<0>(const_cast<char*>(m_bytes.c_str()));
	return true;
}

TXMLData::~TXMLData()
{
	//do nothing
}

static TXMLMgr* m_instance = nullptr;

TXMLMgr* TXMLMgr::getInstance()
{
	if (!m_instance)
	{
		m_instance = new TXMLMgr;
		if (!m_instance->init())
		{
			CC_SAFE_DELETE(m_instance);
		}
	}
	return m_instance;
}

void TXMLMgr::destroy()
{
	CC_SAFE_DELETE(m_instance);
}

TXMLData* TXMLMgr::loadTXML(const std::string& file, bool fromCache)
{
	auto _it = m_dict.find(file);
	if (_it != m_dict.end() && fromCache)
	{
		return _it->second;
	}
	else
	{
		auto _start = clock();
		std::string _strBytes;
		ssize_t _size;
		unsigned char* _bytes = FileUtils::getInstance()->getFileData(file, "rb", &_size);
		if (_size == 0)return nullptr;
		_strBytes = std::string((char*)_bytes, _size);
		free(_bytes);
		CCLOG("load txml : %s", file.c_str());
		CCLOG("[0]load txml: %d", clock() - _start);
		auto _data = TXMLData::create(_strBytes);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		m_dict.insert(file, _data);
#endif
		CCLOG("[1]parse txml: %d", clock() - _start);
		CCLOG("=======================");
		return _data;
	}
}

void TXMLMgr::removeTXMLByKey(const std::string& key)
{
	m_dict.erase(key);
}

void TXMLMgr::removeAllTXML()
{
	m_dict.clear();
}

bool TXMLMgr::init()
{
	return true;
}

TINA_NS_END