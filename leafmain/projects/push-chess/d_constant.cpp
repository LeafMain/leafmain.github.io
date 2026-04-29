#include "pushchess.h"

// Window::
namespace pushchess
{
	const int Window::WIDTH = 1300; // 窗口宽度
	const int Window::HEIGHT = 800; // 窗口高度
}

// Chess::
namespace pushchess
{
	const LPCTSTR Chess::FONT = _T("Arial"); // 棋子字体
	const int Chess::BORDER_WIDTH = 2; // 边框线宽度
	const int Chess::OUTLINE_WIDTH = 1; // 棋子轮廓线宽度
	const int Chess::OUTLINE_OFFSET_DIV = 7; // 棋子轮廓偏移比率
	const COLORREF Chess::FILL_COLOR = 0xc89d1; // 棋子填充颜色
	const COLORREF Chess::BORDER_COLOR = 0xc79b7; // 棋子边框颜色
	const COLORREF Chess::RED_COLOR = RED; // 红色棋子颜色
	const COLORREF Chess::BLACK_COLOR = BLACK; // 黑色棋子颜色
	const COLORREF Chess::SELECTED_COLOR = CYAN; // 棋子选中颜色
}

// Chessboard::
namespace pushchess
{
	const int Chessboard::CHESS_RADIUS = GRID_SIZE / 2 - 3; // 棋子半径
	const int Chessboard::LEFT = 10; // 棋盘与窗口左边界的距离
	const int Chessboard::TOP = 10; // 棋盘与窗口上边界的距离
	const int Chessboard::DOT_OFFSET = 4; // 棋盘点偏移量
	const int Chessboard::DOT_LENGTH = 10; // 棋盘格点长度
	const int Chessboard::DOT_WIDTH = 1; // 棋盘点宽度
	const int Chessboard::GRID_SIZE = 150; // 棋盘网格大小
	const int Chessboard::GRID_MARGIN_LEFT = 40; // 棋盘格左边距
	const int Chessboard::GRID_MARGIN_TOP = 40; // 棋盘格上边距
	const int Chessboard::GRID_LINE_TYPE = PS_SOLID; // 棋盘格线类型
	const int Chessboard::GRID_LINE_WIDTH = 1; // 棋盘格线宽度
	const int Chessboard::WIDTH = COLUMN * GRID_SIZE; // 棋盘宽度
	const int Chessboard::HEIGHT = ROW * GRID_SIZE; // 棋盘高度
	const int Chessboard::OUTLINE_WIDTH = 3; // 棋盘外框宽度
	const int Chessboard::OUTLINE_OFFSET = 9; // 棋盘外框偏移量
	const int Chessboard::OUTLINE_TYPE = PS_SOLID; // 棋盘外框类型
	const COLORREF Chessboard::GRID_COLOR = BLACK; // 棋盘格线颜色
	const COLORREF Chessboard::BACKGROUND_COLOR = 0x87b8de; // 棋盘背景颜色
	const COLORREF Chessboard::DOT_COLOR = BLACK; // 棋盘点颜色
	const COLORREF Chessboard::OUTLINE_COLOR = 0x33551; // 棋盘外框颜色
}

// Button::
namespace pushchess
{
	const COLORREF Button::DISABLED_COLOR = LIGHTGRAY; // 按钮禁用颜色
	const LPCTSTR Button::FONT = _T("Arial"); // 按钮字体
}

// Controls::
namespace pushchess
{
	const int Controls::LEFT = Chessboard::LEFT + 
		Chessboard::OUTLINE_OFFSET; // 控件距窗口的左边界
	const int Controls::TOP = Chessboard::HEIGHT + Chessboard::TOP + 
		Chessboard::GRID_MARGIN_TOP * 2 + 5; // 控件距窗口的上边界
	const int Controls::WIDTH = Chessboard::WIDTH + (Chessboard::
		GRID_MARGIN_LEFT - Chessboard::OUTLINE_OFFSET) * 2; // 控件的宽度
	const int Controls::HEIGHT = 100; // 控件的高度
	const int Controls::BORDER_WIDTH = 1; // 控件的边框宽度
	const int Controls::BORDER_TYPE = PS_SOLID; // 控件的边框线型
	const int Controls::CHESS_X = 50; // 棋子相对于控件的 x 坐标
	const int Controls::CHESS_Y = 50; // 棋子相对于控件的 y 坐标
	const int Controls::CHESS_RADIUS = 45; // 棋子的半径
	const int Controls::TEXT_X = 120; // 文字相对于控件的 x 坐标
	const int Controls::TEXT_Y = 10; // 文字相对于控件的 y 坐标
	const int Controls::TEXT_WIDTH = 520; // 文字的宽度
	const int Controls::TEXT_HEIGHT = 80; // 文字的高度
	const int Controls::TEXT_SIZE = 75; // 文字的大小
	const int Controls::BUTTONS_START_X = 680; // 按钮组相对于控件的 x 坐标
	const int Controls::BUTTONS_START_Y = 20; // 按钮组相对于控件的 y 坐标
	const int Controls::BUTTONS_COUNT = 4; // 按钮的数量
	const int Controls::BUTTON_WIDTH = 120; // 按钮的宽度
	const int Controls::BUTTON_HEIGHT = 60; // 按钮的高度
	const int Controls::BUTTON_FONT_SIZE = 50; // 按钮的字体大小
	const int Controls::BUTTON_GAP = 20; // 按钮之间的间距
	const int Controls::BUTTON_BORDER_WIDTH = 2; // 按钮的边框宽度
	const int Controls::BUTTON_LINE_TYPE = PS_SOLID; // 按钮的线型
	const LPCTSTR Controls::FONT = _T("Arial"); // 字体
	const COLORREF Controls::BORDER_COLOR = 0x87b8de; // 控件的边框颜色
	const COLORREF Controls::BACKGROUND_COLOR = 0x202020; // 控件的背景颜色
	const COLORREF Controls::TEXT_COLOR = 0xc89d1; // 文字的颜色
	const COLORREF Controls::BUTTON_COLOR = BROWN; // 按钮的颜色
	const COLORREF Controls::BUTTON_BACKGROUND_COLOR = BLACK; // 按钮背景颜色
	// Controls::Content::
	const LPCTSTR Controls::CONTENT::KING = 
		_T("左键推或右键踢"); // 帅、将规则文本
	const LPCTSTR Controls::CONTENT::ADVISOR = 
		_T("点左键走或换棋"); // 仕、士规则文本
	const LPCTSTR Controls::CONTENT::BISHOP = 
		_T("点左键走或冲棋"); // 相、象规则文本
	const LPCTSTR Controls::CONTENT::ROOK = 
		_T("点左键走或撞棋"); // 车规则文本
	const LPCTSTR Controls::CONTENT::KNIGHT = 
		_T("点左键走或踢棋"); // 马规则文本
	const LPCTSTR Controls::CONTENT::CANNON = 
		_T("点左键走或打棋"); // 炮规则文本
	const LPCTSTR Controls::CONTENT::PAWN = 
		_T("点左键走或推棋"); // 兵、卒规则文本
	const LPCTSTR Controls::CONTENT::PLAYER_RED = 
		_T("现在是红方走棋"); // 红方走棋提示文本
	const LPCTSTR Controls::CONTENT::PLAYER_BLACK = 
		_T("现在是黑方走棋"); // 黑方走棋提示文本
	const LPCTSTR Controls::CONTENT::PLAYER_RED_WIN = 
		_T("红方已获得胜利"); // 红方胜利文本
	const LPCTSTR Controls::CONTENT::PLAYER_BLACK_WIN = 
		_T("黑方已获得胜利"); // 黑方胜利文本
	const LPCTSTR Controls::CONTENT::PLAYER_RED_KILL_VISIBLE = 
		_T("红方击杀了明棋"); // 红方击杀了明棋提示文本
	const LPCTSTR Controls::CONTENT::PLAYER_RED_KILL_INVISIBLE = 
		_T("红方击杀了暗棋"); // 红方击杀了暗棋提示文本
	const LPCTSTR Controls::CONTENT::PLAYER_BLACK_KILL_VISIBLE = 
		_T("黑方击杀了明棋"); // 黑方击杀了明棋提示文本
	const LPCTSTR Controls::CONTENT::PLAYER_BLACK_KILL_INVISIBLE = 
		_T("黑方击杀了暗棋"); // 黑方击杀了暗棋提示文本
	const LPCTSTR Controls::CONTENT::PLAYER_RED_UNCOVER =
		_T("红方翻开了一子"); // 红方翻开了一子提示文本
	const LPCTSTR Controls::CONTENT::PLAYER_BLACK_UNCOVER =
		_T("黑方翻开了一子"); // 黑方翻开了一子提示文本
	const LPCTSTR Controls::CONTENT::RESET_TIP =
		_T("翻一子以定身份"); // 重置提示文本
	const LPCTSTR Controls::CONTENT::INVALID = 
		_T("走法不符合规则"); // 无效移动文本
	const LPCTSTR Controls::CONTENT::EXCEED = 
		_T("暗棋已超出限制"); // 超出暗棋击杀限制文本
	const LPCTSTR Controls::CONTENT::REOCCUR = 
		_T("不能与历史重复"); // 存在重复局面文本
	const LPCTSTR Controls::CONTENT::NOT_OPEN_FUNCTION =
		_T("该功能尚未开放"); // 功能未开放文本
	const LPCTSTR Controls::CONTENT::REGRET = _T("悔棋"); // 悔棋按钮文本
	const LPCTSTR Controls::CONTENT::RESET = _T("重置"); // 重置按钮文本
	const LPCTSTR Controls::CONTENT::MODE = _T("模式"); // 模式按钮文本
	const LPCTSTR Controls::CONTENT::MODE_DOUBLE = _T("双人"); // 双人模式文本
	const LPCTSTR Controls::CONTENT::MODE_SINGLE = _T("单人"); // 单人模式文本
	const LPCTSTR Controls::CONTENT::EXIT = _T("退出"); // 退出按钮文本
}