#pragma once

/****************************************************************************
功 能：文件系统操作工具类
============================================================================
Copyright (c) 2015 Tungway
创建者：董伟
创建日期：2015-08-18
最后修改者：董伟
最后修改日期：2015-08-18
****************************************************************************/

#include "include/tinaMacros.h"
#include <string>

TINA_NS_BEGIN
class FileUtil
{
public:
	/*	删除一个文件夹（遍历删除）
	*	@param folderPath 文件夹路径
	*/
	static void delDirectory(const std::string& folderPath, int depth = 0);

	/*	创建文件夹
	*	@param folderPath 文件夹路径
	*/
	static bool createDirectory(const std::string& folderPath);

	/*	删除一个文件
	*	@param filePath 文件路径
	*/
	static void delFile(const std::string& filePath);
};
TINA_NS_END