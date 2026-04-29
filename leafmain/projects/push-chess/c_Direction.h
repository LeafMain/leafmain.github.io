#pragma once

// 对方向类的声明
namespace pushchess
{
	// 方向类
	class Direction
	{
	public:
		enum TYPE // 方向枚举
		{
			NONE, // 无方向
			UP, // 上
			UP_RIGHT, // 右上
			RIGHT, // 右
			DOWN_RIGHT, // 右下
			DOWN, // 下
			DOWN_LEFT, // 左下
			LEFT, // 左
			UP_LEFT // 左上
		};

		// 禁止创建实例
		Direction() = delete;

		// 静态方法，判断两个方向是否相反
		// @param type1 第一个方向
		// @param type2 第二个方向
		// @return bool 是否相反
		static bool opposite(TYPE type1, TYPE type2);

		// 静态方法，返回相反的方向
		// @param type1 第一个方向
		// @return Direction::TYPE 相反的方向
		static TYPE opposite(TYPE type);

		// 静态方法，返回(x1, y1) 相对于 (x2, y2) 的方向
		// @param x1 第一个点的横坐标
		// @param y1 第一个点的纵坐标
		// @param x2 第二个点的横坐标
		// @param y2 第二个点的纵坐标
		// @return Direction::TYPE 相对方向
		static TYPE relative(int x1, int y1, int x2, int y2);
	};
}