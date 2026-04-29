#include "pushchess.h"

// 游戏类实例化
namespace pushchess
{
	// 游戏睡眠
	void Game::sleep() const
	{
		Sleep(_sleep_time);
	}

	// 设置游戏睡眠时间
	void Game::sleep_time(int ms_time)
	{
		_sleep_time = ms_time;
	}

	// 获取游戏睡眠时间
	int Game::sleep_time() const
	{
		return _sleep_time;
	}

	// 退出游戏
	void Game::exit()
	{
		game_running = false;
		window.close();
	}

	// 运行游戏
	void Game::run()
	{
		game_running = true;
		window.open(); // 打开窗口
		ExMessage msg = getmessage(); // 获取消息
		Chessboard::MESSAGE chessboard_msg = 
			Chessboard::MESSAGE::NONE; // 棋盘消息
		chessboard.draw(); // 绘制棋盘
		chessboard.reset(); // 初始化棋盘
		controls.draw(); // 绘制控制面板
		while (game_running)
		{
			while (peekmessage(&msg))
			{
				chessboard.listen_event_message(msg);
				controls.listen_mouse_message(msg, chessboard, *this);
				controls.listen_chessboard_message(chessboard_msg, chessboard);
				// 全局绑定 ESC 键退出游戏
				if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE)
				{
					exit();
					break;
				}
			}
			Game::sleep();
		}
	}
}