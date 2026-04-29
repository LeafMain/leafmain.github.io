#pragma once

// 玩家类声明
namespace pushchess
{
	// 玩家类
	class Player
	{
	public:
		enum NAME // 玩家名称枚举
		{
			P_RED, // 红方玩家
			P_BLACK, // 黑方玩家
			NONE, // 无玩家
			LENGTH = NONE, // 玩家总数
		};
		NAME current = NONE; // 当前玩家

		// 玩家类构造函数
		// @param current 初始化当前玩家
		Player(NAME current = NONE);

		// 将当前玩家设为下一个玩家
		void go_next();

		// 将当前玩家设为上一个玩家
		void go_previous();

		// 重置当前玩家为 NONE
		void reset();

		// 获取下一个玩家的名称
		// @return Player::NAME 玩家名称
		NAME next_name() const;

		// 获取上一个玩家的名称
		// @return Player::NAME 玩家名称
		NAME previous_name() const;

		// 将对象隐式转换成 NAME 类型
		// return Player::NAME 玩家名称
		operator NAME();
	};
}