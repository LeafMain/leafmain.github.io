#include "c_Player.h"

// 对玩家类的实例化
namespace pushchess
{
	// 玩家类构造函数
	Player::Player(NAME current) :
		current(current) {}

	// 将当前玩家设为下一个玩家
	void Player::go_next()
	{
		current = next_name();
	}

	// 将当前玩家设为上一个玩家
	void Player::go_previous()
	{
		current = previous_name();
	}

	// 重置当前玩家为 NONE
	void Player::reset()
	{
		current = NONE;
	}

	// 获取下一个玩家的名称
	Player::NAME Player::next_name() const
	{
		if (current == NONE) return NONE;
		return static_cast<NAME>((current + 1) % LENGTH);
	}

	// 获取上一个玩家的名称
	Player::NAME Player::previous_name() const
	{
		if (current == NONE) return NONE;
		return static_cast<NAME>
			((current + LENGTH - 1) % LENGTH);
	}

	// 将对象隐式转换成 NAME 类型
	Player::operator NAME()
	{
		return current;
	}

}