#include "c_Direction.h"

// 对方向类的实现
namespace pushchess
{
	// 静态方法，判断两个方向是否相反
	bool Direction::opposite(TYPE type1, TYPE type2)
	{
		return type1 == opposite(type2);
	}

	// 静态方法，返回相反的方向
	Direction::TYPE Direction::opposite(TYPE type)
	{
		if (type == NONE) return NONE;
		return static_cast<TYPE>((type - 1 + 4) % 8 + 1);
	}

	// 静态方法，返回(x1, y1) 相对于 (x2, y2) 的方向
	Direction::TYPE Direction::relative(int x1, int y1, int x2, int y2)
	{
		if (x1 == x2 && y1 < y2) return UP;
		if (x1 > x2 && y1 < y2) return UP_RIGHT;
		if (x1 > x2 && y1 == y2) return RIGHT;
		if (x1 > x2 && y1 > y2) return DOWN_RIGHT;
		if (x1 == x2 && y1 > y2) return DOWN;
		if (x1 < x2 && y1 > y2) return DOWN_LEFT;
		if (x1 < x2 && y1 == y2) return LEFT;
		if (x1 < x2 && y1 < y2) return UP_LEFT;
		return NONE;
	}
};