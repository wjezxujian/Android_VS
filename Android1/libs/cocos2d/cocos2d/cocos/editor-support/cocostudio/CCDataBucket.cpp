#include "CCDataBucket.h"

#define BUCKET_SIZE (1024 * 1024 * 5)

namespace cocostudio
{
	static DataBucket* m_instance = nullptr;

	DataBucket* DataBucket::getInstance()
	{
		if (!m_instance)
		{
			m_instance = new DataBucket;
			m_instance->init();
		}
		return m_instance;
	}

	void DataBucket::destroy()
	{
		if (m_instance != nullptr)
			delete m_instance;
		m_instance = nullptr;
	}

	DataBucket::BucketInfo* DataBucket::pushBucket(const std::string& key, int32_t size)
	{
		if (!m_cacheMode)return nullptr;
		auto _it = m_dict.find(key);
		if (_it != m_dict.end())return &_it->second;

		BucketInfo _bucketInfo;
		{
			uint32_t _bucketSize = BUCKET_SIZE;
			if (size > 0)
				_bucketSize = size;
			_bucketInfo.buffer = malloc(_bucketSize);
			_bucketInfo.size = _bucketSize;
		}
		m_dict.insert({key, _bucketInfo});
		m_crtKey = key;
		return &m_dict.at(key);
	}

	void DataBucket::removeBucket(const std::string& key)
	{
		if (!m_cacheMode)return;
		auto _it = m_dict.find(key);
		if (_it == m_dict.end())return;

		if (_it->second.buffer != nullptr)
			free(_it->second.buffer);

		m_dict.erase(_it);
	}

	DataBucket::BucketInfo* DataBucket::getBucket()
	{
		auto _it = m_dict.find(m_crtKey);
		if (_it == m_dict.end())return nullptr;

		return &_it->second;
	}

	bool DataBucket::init()
	{
		//do nothing
		return true;
	}
}