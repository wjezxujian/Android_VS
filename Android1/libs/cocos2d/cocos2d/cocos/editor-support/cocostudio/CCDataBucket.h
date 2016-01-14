

#ifndef __CCDATABUCKET_H__
#define __CCDATABUCKET_H__

#include <string>
#include <cstdint>
#include <map>
#include "cocostudio/CCArmatureDefine.h"
#include "cocostudio/CCDatas.h"
#include "cocostudio/CocosStudioExport.h"
namespace  cocostudio {

    class CC_STUDIO_DLL DataBucket
	{
	public:
		struct BucketInfo
		{
			void* getBuffer(){ return (char*)buffer + used; };
			void*		buffer = nullptr;
			uint32_t	size = 0;
			uint32_t	used = 0;
		};

	public:
		static DataBucket* getInstance();
		static void destroy();
		BucketInfo* pushBucket(const std::string& key, int32_t size = -1);
		void removeBucket(const std::string& key);
		void setCacheMode(bool cacheMode){ m_cacheMode = cacheMode; };
		bool getCacheMode(){ return m_cacheMode; };
		BucketInfo* getBucket();

		template<class T>
		T* allocaData()
		{
			T* _data = nullptr;
			auto _bucket = getBucket();
			if (m_cacheMode && _bucket != nullptr && (_bucket->size - _bucket->used) >= sizeof(T))
			{
				_data = new (_bucket->getBuffer())T;
				_data->retain();
				_bucket->used += sizeof(T);
			}
			else
			{
				_data = new (std::nothrow) T();
			}
			return _data;
		}

	private:
		bool init();

	private:
		std::map<std::string, BucketInfo>	m_dict;
		std::string							m_crtKey;
		bool								m_cacheMode = true;
	};
}

#endif /*__CCDATAREADERHELPER_H__*/
