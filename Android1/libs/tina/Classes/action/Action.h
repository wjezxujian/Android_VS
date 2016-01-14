#pragma once

/****************************************************************************
�� �ܣ�tina������520һ��Ҫ������������
============================================================================
Copyright (c) 2015 Tungway
�����ߣ���ΰ
�������ڣ�2015-05-20
����޸��ߣ���ΰ
����޸����ڣ�2015-08-17
****************************************************************************/

#include "include/tinaMacros.h"
#include "cocos2d.h"

TINA_NS_BEGIN
//�ı�ߴ�Ķ���
class SizeTo : public cocos2d::ActionInterval
{
public:
	static SizeTo* create(float duration, float dstWidth, float dstHeight);
	static SizeTo* create(float duration, const cocos2d::Size& dstSize);

	virtual SizeTo* clone() const override;
	virtual SizeTo* reverse() const override;
	virtual void startWithTarget(cocos2d::Node *target) override;
	virtual void update(float time) override;

protected:
	SizeTo(){};
	virtual ~SizeTo() {}
	bool initWithDuration(float duration, const cocos2d::Size& dstSize);

protected:
	cocos2d::Size	m_startSize;
	cocos2d::Size	m_diffSize;
	cocos2d::Size	m_dstSize;
};

//����ģ���Ķ���
class RadialBlurTo : public cocos2d::ActionInterval
{
public:
	static RadialBlurTo* create(float duration, int dstStrength);

	virtual RadialBlurTo* clone() const override;
	virtual RadialBlurTo* reverse() const override;
	virtual void startWithTarget(cocos2d::Node *target) override;
	virtual void update(float time) override;

protected:
	RadialBlurTo(){};
	virtual ~RadialBlurTo() {}
	bool initWithDuration(float duration, int dstStrength);

protected:
	int		m_startStrength;
	int		m_diffStrength;
	int		m_dstStrength;
};
TINA_NS_END