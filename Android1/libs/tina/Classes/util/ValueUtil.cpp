#include "ValueUtil.h"

TINA_NS_BEGIN

int ValueUtil::readInt(cocos2d::Value* value, ...)
{
	va_list ap;
	va_start(ap, value);
	cocos2d::Value *ret = readValue(value, ap);
	va_end(ap);

	if (ret && !(ret->getType() == cocos2d::Value::Type::VECTOR || ret->getType() == cocos2d::Value::Type::MAP))
	{
		return ret->asInt();
	}

	return 0;
}

int ValueUtil::readInt(cocos2d::ValueMap* value, ...)
{
	va_list ap;
	va_start(ap, value);
	cocos2d::Value *ret = readValue(value, ap);
	va_end(ap);

	if (ret && !(ret->getType() == cocos2d::Value::Type::VECTOR || ret->getType() == cocos2d::Value::Type::MAP))
	{
		return ret->asInt();
	}

	return 0;
}

float ValueUtil::readFloat(cocos2d::Value* value, ...)
{
	va_list ap;
	va_start(ap, value);
	cocos2d::Value *ret = readValue(value, ap);
	va_end(ap);

	if (ret && !(ret->getType() == cocos2d::Value::Type::VECTOR || ret->getType() == cocos2d::Value::Type::MAP))
	{
		return ret->asFloat();
	}

	return 0.0f;
}

float ValueUtil::readFloat(cocos2d::ValueMap* value, ...)
{
	va_list ap;
	va_start(ap, value);
	cocos2d::Value *ret = readValue(value, ap);
	va_end(ap);

	if (ret && !(ret->getType() == cocos2d::Value::Type::VECTOR || ret->getType() == cocos2d::Value::Type::MAP))
	{
		return ret->asFloat();
	}

	return 0.0f;
}

const char* ValueUtil::readCString(cocos2d::Value* value, ...)
{
	va_list ap;
	va_start(ap, value);
	cocos2d::Value *ret = readValue(value, ap);
	va_end(ap);

	if (ret && !(ret->getType() == cocos2d::Value::Type::VECTOR || ret->getType() == cocos2d::Value::Type::MAP))
	{
		return ret->asString().c_str();
	}

	return nullptr;
}

const char* ValueUtil::readCString(cocos2d::ValueMap* value, ...)
{
	va_list ap;
	va_start(ap, value);
	cocos2d::Value *ret = readValue(value, ap);
	va_end(ap);

	if (ret && !(ret->getType() == cocos2d::Value::Type::VECTOR || ret->getType() == cocos2d::Value::Type::MAP))
	{
		return ret->asString().c_str();
	}

	return nullptr;
}

std::string ValueUtil::readString(cocos2d::Value* value, ...)
{
	va_list ap;
	va_start(ap, value);
	cocos2d::Value *ret = readValue(value, ap);
	va_end(ap);

	if (ret && !(ret->getType() == cocos2d::Value::Type::VECTOR || ret->getType() == cocos2d::Value::Type::MAP))
	{
		return ret->asString();
	}

	return "";
}

std::string ValueUtil::readString(cocos2d::ValueMap* value, ...)
{
	va_list ap;
	va_start(ap, value);
	cocos2d::Value *ret = readValue(value, ap);
	va_end(ap);

	if (ret && !(ret->getType() == cocos2d::Value::Type::VECTOR || ret->getType() == cocos2d::Value::Type::MAP))
	{
		return ret->asString();
	}

	return "";
}

cocos2d::ValueMap* ValueUtil::readDict(cocos2d::Value* value, ...)
{
	va_list ap;
	va_start(ap, value);
	cocos2d::Value *ret = readValue(value, ap);
	va_end(ap);

	if (ret && ret->getType() == cocos2d::Value::Type::MAP)
	{
		return &ret->asValueMap();
	}

	return nullptr;
}

cocos2d::ValueMap* ValueUtil::readDict(cocos2d::ValueMap* value, ...)
{
	va_list ap;
	va_start(ap, value);
	cocos2d::Value *ret = readValue(value, ap);
	va_end(ap);

	if (ret && ret->getType() == cocos2d::Value::Type::MAP)
	{
		return &ret->asValueMap();
	}

	return nullptr;
}

cocos2d::ValueVector* ValueUtil::readArray(cocos2d::Value* value, ...)
{
	va_list ap;
	va_start(ap, value);
	cocos2d::Value *ret = readValue(value, ap);
	va_end(ap);

	if (ret && ret->getType() == cocos2d::Value::Type::VECTOR)
	{
		return &ret->asValueVector();
	}

	return nullptr;
}

cocos2d::ValueVector* ValueUtil::readArray(cocos2d::ValueMap* value, ...)
{
	va_list ap;
	va_start(ap, value);
	cocos2d::Value *ret = readValue(value, ap);
	va_end(ap);

	if (ret && ret->getType() == cocos2d::Value::Type::VECTOR)
	{
		return &ret->asValueVector();
	}

	return nullptr;
}

cocos2d::Value* ValueUtil::readValue(const cocos2d::Value* value, va_list keys)
{
	if (value && value->getType() == cocos2d::Value::Type::MAP)
		return readValue(&value->asValueMap(), keys);

	return nullptr;
}

cocos2d::Value* ValueUtil::readValue(const cocos2d::ValueMap* value, va_list keys)
{
	const char* _key = nullptr;
	cocos2d::ValueMap* _dict = (cocos2d::ValueMap*)value;
	cocos2d::Value* _ret = nullptr;
	while ((_key = va_arg(keys, const char*)) && (_key != nullptr))
	{
		_ret = (cocos2d::Value*)&_dict->at(_key);
		if (_ret->getType() == cocos2d::Value::Type::MAP)
		{
			_dict = &_ret->asValueMap();
		}
	}

	return _ret;
}
TINA_NS_END
