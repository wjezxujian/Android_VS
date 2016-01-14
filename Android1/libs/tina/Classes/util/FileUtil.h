#pragma once

/****************************************************************************
�� �ܣ��ļ�ϵͳ����������
============================================================================
Copyright (c) 2015 Tungway
�����ߣ���ΰ
�������ڣ�2015-08-18
����޸��ߣ���ΰ
����޸����ڣ�2015-08-18
****************************************************************************/

#include "include/tinaMacros.h"
#include <string>

TINA_NS_BEGIN
class FileUtil
{
public:
	/*	ɾ��һ���ļ��У�����ɾ����
	*	@param folderPath �ļ���·��
	*/
	static void delDirectory(const std::string& folderPath, int depth = 0);

	/*	�����ļ���
	*	@param folderPath �ļ���·��
	*/
	static bool createDirectory(const std::string& folderPath);

	/*	ɾ��һ���ļ�
	*	@param filePath �ļ�·��
	*/
	static void delFile(const std::string& filePath);
};
TINA_NS_END