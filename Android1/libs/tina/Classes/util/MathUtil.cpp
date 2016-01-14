#include "MathUtil.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>
#include "StringUtil.h"
#include<math.h>

TINA_NS_BEGIN

static bool __randomSrand = false;
int MathUtil::random(int min, int max)
{
	if (!__randomSrand)
	{
		std::srand(time(0));
		__randomSrand = true;
	}
	if (min > max)
		std::swap(max, min);

	return std::rand() % (max - min + 1) + min;
}

bool MathUtil::compareVersion(const std::string& ver1, const std::string& ver2)
{
	std::vector<std::string> _vec1;
	StringUtil::split(ver1, ".", &_vec1);
	std::vector<std::string> _vec2;
	StringUtil::split(ver2, ".", &_vec2);

	for (int i = 0; i < _vec1.size(); ++i)
	{
		if (_vec2.size() < i+1)
		{
			return true;
		}

		int _a = atoi(_vec1.at(i).c_str());
		int _b = atoi(_vec2.at(i).c_str());

		if (_a == _b)continue;
		return _a >= _b;
	}
	return false;
}

bool MathUtil::cmpFloat(float n1, float n2, float range)
{
	return fabs(n1 - n2) <= range;
}
TINA_NS_END