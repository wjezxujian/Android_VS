#pragma once

/****************************************************************************
�� �ܣ�cocos2d::Value����������
============================================================================
Copyright (c) 2015 Tungway
�����ߣ���ΰ
�������ڣ�2015-08-18
����޸��ߣ���ΰ
����޸����ڣ�2015-08-18
****************************************************************************/

#include "include/tinaMacros.h"
#include "base/CCValue.h"

TINA_NS_BEGIN

class ValueUtil
{
public:

	/* ��ȡValueMap�е�ĳ������
	* ���� 1��Դ����
	* ���� x�������ֶ���
	* ����ֵ��Ŀ��ֵ
	*/
	static int readInt(cocos2d::Value* value, ...);
	static int readInt(cocos2d::ValueMap* value, ...);

	/* ��ȡValueMap�е�ĳ��������
	* ���� 1��Դ����
	* ���� x�������ֶ���
	* ����ֵ��Ŀ��ֵ
	*/
	static float readFloat(cocos2d::Value* value, ...);
	static float readFloat(cocos2d::ValueMap* value, ...);

	/* ��ȡValueMap�е�ĳ���ַ���
	* ���� 1��Դ����
	* ���� x�������ֶ���
	* ����ֵ��Ŀ��ֵ
	*/
	static const char* readCString(cocos2d::Value* value, ...);
	static const char* readCString(cocos2d::ValueMap* value, ...);

	/* ��ȡValueMap�е�ĳ���ַ���
	* ���� 1��Դ����
	* ���� x�������ֶ���
	* ����ֵ��Ŀ��ֵ
	*/
	static std::string readString(cocos2d::Value* value, ...);
	static std::string readString(cocos2d::ValueMap* value, ...);

	/* ��ȡValueMap�е�ĳ���ֵ�����
	* ���� 1��Դ����
	* ���� x�������ֶ���
	* ����ֵ��Ŀ��ֵ
	*/
	static cocos2d::ValueMap* readDict(cocos2d::Value* value, ...);
	static cocos2d::ValueMap* readDict(cocos2d::ValueMap* value, ...);

	/* ��ȡValueMap�е�ĳ����������
	* ���� 1��Դ����
	* ���� x�������ֶ���
	* ����ֵ��Ŀ��ֵ
	*/
	static cocos2d::ValueVector* readArray(cocos2d::Value* value, ...);
	static cocos2d::ValueVector* readArray(cocos2d::ValueMap* value, ...);

	/* ��ȡValueMap�е�ĳ������
	* ���� 1��Դ����
	* ���� 2�������ֶ���
	* ����ֵ��Ŀ��ֵ
	*/
	static cocos2d::Value* readValue(const cocos2d::Value* value, va_list keys);
	static cocos2d::Value* readValue(const cocos2d::ValueMap* value, va_list keys);

};

TINA_NS_END
