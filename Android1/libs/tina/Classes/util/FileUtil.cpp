#include "FileUtil.h"
#include <cstdint>
#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <io.h>
#include <direct.h>
#include <stdio.h>
#else
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#endif

TINA_NS_BEGIN
void FileUtil::delDirectory(const std::string& folderPath, int depth)
{
#ifdef WIN32
	_finddata_t FileInfo;
	std::string strfind = folderPath + "\\*";
	int64_t Handle = _findfirst(strfind.c_str(), &FileInfo);


	if (Handle == -1L)
	{
		printf("%s", "can not match the folder path\n");
		return;
	}

	do
	{
		//判断是否有子目录
		if (FileInfo.attrib & _A_SUBDIR)
		{
			//这个语句很重要
			if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0))
			{
				std::string newPath = folderPath + "\\" + FileInfo.name;
				delDirectory(newPath);
				_rmdir(newPath.c_str());
			}
		}
		else
		{
			std::string filename = (folderPath + "\\" + FileInfo.name);
			remove(filename.c_str());
			//std::cout << folderPath << "\\" << FileInfo.name << " " << std::endl;
			printf("%s\\%s\n", folderPath.c_str(), FileInfo.name);
		}
	} while (_findnext(Handle, &FileInfo) == 0);
	_findclose(Handle);
	_rmdir(folderPath.c_str());
#else
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	if ((dp = opendir(folderPath.c_str())) == NULL) {
		fprintf(stderr, "cannot open directory: %s\n", folderPath.c_str());
		return;
	}
	chdir(folderPath.c_str());
	while ((entry = readdir(dp)) != NULL) {
		lstat(entry->d_name, &statbuf);
		if (S_ISDIR(statbuf.st_mode)) {


			if (strcmp(".", entry->d_name) == 0 ||
				strcmp("..", entry->d_name) == 0)
				continue;
			delDirectory(entry->d_name, depth + 4);
			rmdir(entry->d_name);
			printf("%*s%s/\n", depth, "", entry->d_name);
		}
		else
		{
			std::string filename = entry->d_name;
			remove(filename.c_str());
			printf("%*s%s\n", depth, "", entry->d_name);
		}
	}
	chdir("..");
	closedir(dp);
	rmdir(folderPath.c_str());
#endif
}

bool FileUtil::createDirectory(const std::string& folderPath)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	mode_t processMask = umask(0);
	int ret = mkdir(folderPath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	umask(processMask);
	if (ret != 0 && (errno != EEXIST))
	{
		return false;
	}

	return true;
#else
	bool ret = CreateDirectoryA(folderPath.c_str(), NULL);
	if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
	{
		return false;
	}
	return true;
#endif
}

void FileUtil::delFile(const std::string& filePath)
{
	remove(filePath.c_str());
}
TINA_NS_END