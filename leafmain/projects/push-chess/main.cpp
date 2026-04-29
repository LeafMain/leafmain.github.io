#include "pushchess.h"

// 测试函数
static void pc_test()
{
	using namespace std;
	namespace pc = pushchess;
	using namespace pc;
	/*Chessboard chessboard;
	chessboard.draw();
	chessboard.reset();
	while (true)
	{
		ExMessage msg = getmessage();
		Chessboard::MESSAGE mm;
		chessboard.listen_mouse_message(msg);
		if (msg.message == WM_MBUTTONDOWN)
		{
			chessboard.regret();
		}
		chessboard.peek_board_message(&mm);
	}*/
	system("pause");
}

// 程序的入口
int main()
{
	pushchess::Game game;
	game.run();
	// 测试函数
	//pc_test();
	return 0;
}