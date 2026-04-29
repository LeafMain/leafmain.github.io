#pragma once

// 对控件类的声明
namespace pushchess
{
	// 对 Game 类的前置声明
	class Game;

	// 控件类
	class Controls
	{
	public:
		static const int LEFT; // 控件距窗口的左边界
		static const int TOP; // 控件距窗口的上边界
		static const int WIDTH; // 控件的宽度
		static const int HEIGHT; // 控件的高度
		static const int BORDER_WIDTH; // 控件的边框宽度
		static const int BORDER_TYPE; // 控件的边框线型
		static const int CHESS_X; // 棋子相对于控件的 x 坐标
		static const int CHESS_Y; // 棋子相对于控件的 y 坐标
		static const int CHESS_RADIUS; // 棋子的半径
		static const int TEXT_X; // 文字相对于控件的 x 坐标
		static const int TEXT_Y; // 文字相对于控件的 y 坐标
		static const int TEXT_WIDTH; // 文字的宽度
		static const int TEXT_HEIGHT; // 文字的高度
		static const int TEXT_SIZE; // 文字的大小
		static const int BUTTONS_START_X; // 按钮组相对于控件的 x 坐标
		static const int BUTTONS_START_Y; // 按钮组相对于控件的 y 坐标
		static const int BUTTONS_COUNT; // 按钮的数量
		static const int BUTTON_WIDTH; // 按钮的宽度
		static const int BUTTON_HEIGHT; // 按钮的高度
		static const int BUTTON_FONT_SIZE; // 按钮的字体大小
		static const int BUTTON_GAP; // 按钮之间的间距
		static const int BUTTON_BORDER_WIDTH; // 按钮的边框宽度
		static const int BUTTON_LINE_TYPE; // 按钮的线型
		static const LPCTSTR FONT; // 字体
		static const COLORREF BORDER_COLOR; // 控件的边框颜色
		static const COLORREF BACKGROUND_COLOR; // 控件的背景颜色
		static const COLORREF TEXT_COLOR; // 文字的颜色
		static const COLORREF BUTTON_COLOR; // 按钮的颜色
		static const COLORREF BUTTON_BACKGROUND_COLOR; // 按钮背景颜色
	public:
		enum class MODE // 模式枚举
		{
			M_SINGLE, // 单人模式
			M_DOUBLE, // 双人模式
		};

		// Controls 构造函数
		Controls();

		// 绘制控件
		void draw() const;

		// 移除控件
		void remove() const;

		// 监听鼠标消息
		// @param msg 鼠标消息
		// @param chessboard 推棋棋盘
		// @param window 推棋窗口
		void listen_mouse_message(ExMessage& msg, 
			Chessboard& chessboard, Game& game);

		// 监听棋盘消息
		// @param message 棋盘消息引用
		// @param chessboard 推棋棋盘引用
		void listen_chessboard_message(Chessboard::MESSAGE& message, 
			Chessboard& chessboard);

		// 获取当前游戏模式
		MODE current_mode() const;
	private:
		enum BUTTON // 按钮枚举
		{
			B_REGRET,	// 悔棋按钮
			B_RESET, 	// 重置按钮
			B_MODE, 	// 模式按钮
			B_EXIT, 	// 退出按钮
		};
		struct CONTENT // 内容文字
		{
			static const LPCTSTR KING; // 帅、将规则文本
			static const LPCTSTR ADVISOR; // 仕、士规则文本
			static const LPCTSTR BISHOP; // 相、象规则文本
			static const LPCTSTR ROOK; // 车规则文本
			static const LPCTSTR KNIGHT; // 马规则文本
			static const LPCTSTR CANNON; // 炮规则文本
			static const LPCTSTR PAWN; // 兵、卒规则文本
			static const LPCTSTR PLAYER_RED; // 红方走棋提示文本
			static const LPCTSTR PLAYER_BLACK; // 黑方走棋提示文本
			static const LPCTSTR PLAYER_RED_WIN; // 红方胜利文本
			static const LPCTSTR PLAYER_BLACK_WIN; // 黑方胜利文本
			static const LPCTSTR PLAYER_RED_KILL_VISIBLE; // 红方击杀了明棋
			static const LPCTSTR PLAYER_RED_KILL_INVISIBLE; // 红方击杀了暗棋
			static const LPCTSTR PLAYER_BLACK_KILL_VISIBLE; // 黑方击杀了明棋
			static const LPCTSTR PLAYER_BLACK_KILL_INVISIBLE; // 黑方击杀了暗棋
			static const LPCTSTR PLAYER_RED_UNCOVER; // 红方翻开了一子
			static const LPCTSTR PLAYER_BLACK_UNCOVER; // 黑方翻开了一子
			static const LPCTSTR RESET_TIP; // 重置提示文本
			static const LPCTSTR INVALID; // 无效移动文本
			static const LPCTSTR EXCEED; // 超出暗棋击杀限制文本
			static const LPCTSTR REOCCUR; // 存在重复局面文本
			static const LPCTSTR REGRET; // 悔棋按钮文本
			static const LPCTSTR RESET; // 重置按钮文本
			static const LPCTSTR MODE; // 模式按钮文本
			static const LPCTSTR MODE_DOUBLE; // 双人模式文本
			static const LPCTSTR MODE_SINGLE; // 单人模式文本
			static const LPCTSTR EXIT; // 退出按钮文本
			static const LPCTSTR NOT_OPEN_FUNCTION; // 功能未开放文本
		};
		Chess chess; // 棋子
		Button buttons[4]; // 按钮组
		MODE _mode = MODE::M_SINGLE; // 当前模式
		// 上一个棋盘消息
		Chessboard::MESSAGE _previous_message = Chessboard::MESSAGE::NONE;
		// 控件绘制前的背景颜色，用于恢复
		mutable COLORREF _background_before_draw_ = TRANSPARENT; 

		// 更新棋子信息并绘制棋子
		// @param player 玩家
		void update_chess(const Player& player);

		// 更新文字信息并绘制文字
		// @param message 棋盘信息
		void update_text(const Chessboard::MESSAGE& message,
			const Chessboard& chessboard);

		// 清除文字
		void clear_text() const;

		// 更新按钮信息并绘制按钮
		// @param button_type 按钮类型
		// @param text 按钮文本
		void update_button(BUTTON button_type, LPCTSTR text);

		// 重置控件
		void reset();

		// 切换模式
		void toggle_mode();
	};
}