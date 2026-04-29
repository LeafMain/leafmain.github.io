#pragma once

// 游戏类声明
namespace pushchess
{
	// 游戏类
	class Game
	{
	public:

		// 游戏睡眠
		void sleep() const;

		// 设置游戏睡眠时间
		// @param ms_time 睡眠时间毫秒
		void sleep_time(int ms_time);

		// 获取游戏睡眠时间
		// @return int 睡眠时间毫秒
		int sleep_time() const;

		// 退出游戏
		void exit();

		// 运行游戏
		void run();

		// 游戏是否正在运行
		bool game_running = false; 
	private:
		Window window; // 窗口对象
		Chessboard chessboard; // 棋盘对象
		Controls controls; // 控制对象
		int _sleep_time = 20; // 游戏睡眠时间
	};
}
