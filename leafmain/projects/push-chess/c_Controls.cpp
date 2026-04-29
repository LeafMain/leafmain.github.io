#include "pushchess.h"

// 对控件类的实现
namespace pushchess
{
	// Controls 构造函数
	Controls::Controls() :
		chess(Chess::COLOR::NONE, Chess::TYPE::KING,
			LEFT + CHESS_X, TOP + CHESS_Y, CHESS_RADIUS, false)
	{
		LPCTSTR button_texts[4] =
		{
			CONTENT::REGRET, CONTENT::RESET,
			CONTENT::MODE, CONTENT::EXIT
		};
		for (int i = 0; i < BUTTONS_COUNT; i++)
		{
			buttons[i].set_data(button_texts[i],
				LEFT + BUTTONS_START_X + i * (BUTTON_WIDTH + BUTTON_GAP),
				TOP + BUTTONS_START_Y, BUTTON_WIDTH, BUTTON_HEIGHT,
				BUTTON_FONT_SIZE, BUTTON_COLOR, BUTTON_BACKGROUND_COLOR,
				BUTTON_COLOR, BUTTON_BORDER_WIDTH, BUTTON_LINE_TYPE);
		}
	}

	// 绘制控件
	void Controls::draw() const
	{
		// 保存背景颜色，只保存一次
		if (_background_before_draw_ == TRANSPARENT)
		{
			_background_before_draw_ = getpixel(LEFT, TOP);
		}
		setlinecolor(BORDER_COLOR);
		setlinestyle(BORDER_TYPE, BUTTON_BORDER_WIDTH);
		setfillcolor(BACKGROUND_COLOR);
		fillrectangle(LEFT, TOP, LEFT + WIDTH, TOP + HEIGHT);
		// 绘制棋子
		chess.draw();
		// 绘制按钮
		for (int i = 0; i < BUTTONS_COUNT; i++)
		{
			buttons[i].draw();
		}
		// 无需绘制文字
	}

	// 移除控件
	void Controls::remove() const
	{
		chess.remove();
		for (int i = 0; i < BUTTONS_COUNT; i++)
		{
			buttons[i].remove();
		}
		setlinecolor(_background_before_draw_);
		setfillcolor(_background_before_draw_);
		fillrectangle(LEFT, TOP, LEFT + WIDTH, TOP + HEIGHT);
	}

	// 监听鼠标消息
	void Controls::listen_mouse_message(ExMessage& msg,
		Chessboard& chessboard, Game& game)
	{
		if (buttons[B_REGRET].clicked(msg))
		{
			chessboard.regret();
		}
		else if (buttons[B_RESET].clicked(msg))
		{
			chessboard.reset();
		}
		else if (buttons[B_MODE].clicked(msg))
		{
			chessboard.blur();
			clear_text();
			RECT rect = { LEFT + TEXT_X, TOP + TEXT_Y,
			LEFT + TEXT_X + TEXT_WIDTH, TOP + TEXT_Y + TEXT_HEIGHT };
			int mode = DT_SINGLELINE | DT_CENTER | DT_VCENTER;
			settextstyle(TEXT_SIZE, 0, FONT);
			settextcolor(TEXT_COLOR);
			drawtext(CONTENT::NOT_OPEN_FUNCTION, &rect, mode);
		}
		else if (buttons[B_EXIT].clicked(msg))
		{
			game.game_running = false;
		}
		else if (msg.message == WM_LBUTTONDOWN &&
			msg.x >= LEFT && msg.x <= LEFT + WIDTH &&
			msg.y >= TOP && msg.y <= TOP + HEIGHT)
		{
			// 如果棋盘有焦点，则失去焦点
			if (chessboard.has_focus())
			{
				chessboard.blur(); // 点击控件时使棋盘失去焦点
				clear_text(); // 清除提示文字
				// 将之前的消息设置为棋盘失去焦点
				_previous_message = Chessboard::MESSAGE::BOARD_BLUR;
			}
		}
	}

	// 监听棋盘消息
	void Controls::listen_chessboard_message(
		Chessboard::MESSAGE& message, Chessboard& chessboard)
	{
		chessboard.peek_board_message(&message);
		if (message != Chessboard::MESSAGE::NONE)
		{
			update_chess(chessboard.get_player());
			update_text(message, chessboard);
		}
	}

	// 获取当前游戏模式
	Controls::MODE Controls::current_mode() const
	{
		return _mode;
	}

	// 更新棋子信息并绘制棋子
	void Controls::update_chess(const Player& player)
	{
		switch (player.current)
		{
		case Player::P_RED:
			// 如果颜色没有改变，则不重新绘制
			if (chess.color == Chess::COLOR::C_RED) return;
			chess.color = Chess::COLOR::C_RED;
			break;
		case Player::P_BLACK:
			// 如果颜色没有改变，则不重新绘制
			if (chess.color == Chess::COLOR::C_BLACK) return;
			chess.color = Chess::COLOR::C_BLACK;
			break;
		default:
			chess.color = Chess::COLOR::NONE;
			break;
		}
		// 如果没有确定棋子，则翻开
		if (!chess.visible && chess.color!= Chess::COLOR::NONE)
		{
			chess.uncover();
		}
		chess.draw();
	}

	// 更新文字信息并绘制文字
	void Controls::update_text(
		const Chessboard::MESSAGE& message, const Chessboard& chessboard)
	{
		_previous_message = message;
		RECT rect = { LEFT + TEXT_X, TOP + TEXT_Y,
			LEFT + TEXT_X + TEXT_WIDTH, TOP + TEXT_Y + TEXT_HEIGHT };
		int mode = DT_SINGLELINE | DT_CENTER | DT_VCENTER;
		clear_text();
		settextcolor(TEXT_COLOR);
		settextstyle(TEXT_SIZE, 0, FONT);
		setbkmode(TRANSPARENT); // 背景透明
		switch (message)
		{
		case Chessboard::MESSAGE::SELECT_KING:
			drawtext(CONTENT::KING, &rect, mode);
			break;
		case Chessboard::MESSAGE::SELECT_ADVISOR:
			drawtext(CONTENT::ADVISOR, &rect, mode);
			break;
		case Chessboard::MESSAGE::SELECT_BISHOP:
			drawtext(CONTENT::BISHOP, &rect, mode);
			break;
		case Chessboard::MESSAGE::SELECT_ROOK:
			drawtext(CONTENT::ROOK, &rect, mode);
			break;
		case Chessboard::MESSAGE::SELECT_KNIGHT:
			drawtext(CONTENT::KNIGHT, &rect, mode);
			break;
		case Chessboard::MESSAGE::SELECT_CANNON:
			drawtext(CONTENT::CANNON, &rect, mode);
			break;
		case Chessboard::MESSAGE::SELECT_PAWN:
			drawtext(CONTENT::PAWN, &rect, mode);
			break;
		case Chessboard::MESSAGE::KILL_VISIBLE:
			if (chessboard.get_player().current == Player::P_RED)
			{
				drawtext(CONTENT::PLAYER_BLACK_KILL_VISIBLE, &rect, mode);
			}
			else if (chessboard.get_player().current == Player::P_BLACK)
			{
				drawtext(CONTENT::PLAYER_RED_KILL_VISIBLE, &rect, mode);
			}
			break;
		case Chessboard::MESSAGE::KILL_INVISIBLE:
			if (chessboard.get_player().current == Player::P_RED)
			{
				drawtext(CONTENT::PLAYER_BLACK_KILL_INVISIBLE, &rect, mode);
			}
			else if (chessboard.get_player().current == Player::P_BLACK)
			{
				drawtext(CONTENT::PLAYER_RED_KILL_INVISIBLE, &rect, mode);
			}
			break;
		case Chessboard::MESSAGE::CHESS_UNCOVER:
			if (chessboard.get_player().current == Player::P_RED)
			{
				drawtext(CONTENT::PLAYER_BLACK_UNCOVER, &rect, mode);
			}
			else if (chessboard.get_player().current == Player::P_BLACK)
			{
				drawtext(CONTENT::PLAYER_RED_UNCOVER, &rect, mode);
			}
			break;
		case Chessboard::MESSAGE::BOARD_RESET:
			drawtext(CONTENT::RESET_TIP, &rect, mode);
			break;
		case Chessboard::MESSAGE::EXCEED_INVISIBLE_KILL:
			drawtext(CONTENT::EXCEED, &rect, mode);
			break;
		case Chessboard::MESSAGE::INVALID_MOVE:
			drawtext(CONTENT::INVALID, &rect, mode);
			break;
		case Chessboard::MESSAGE::NOT_IN_TURN:
			if (chessboard.get_player().current == Player::P_RED)
			{
				drawtext(CONTENT::PLAYER_RED, &rect, mode);
			}
			else if (chessboard.get_player().current == Player::P_BLACK)
			{
				drawtext(CONTENT::PLAYER_BLACK, &rect, mode);
			}
			break;
		case Chessboard::MESSAGE::REOCCUR_PREVIOUS:
			drawtext(CONTENT::REOCCUR, &rect, mode);
			break;
		case Chessboard::MESSAGE::PLAYER_LOSE:
			if (chessboard.get_player().current == Player::P_RED)
			{
				drawtext(CONTENT::PLAYER_BLACK_WIN, &rect, mode);
			}
			else if (chessboard.get_player().current == Player::P_BLACK)
			{
				drawtext(CONTENT::PLAYER_RED_WIN, &rect, mode);
			}
			break;
		default:
			break;
		}
	}

	// 清除文字
	void Controls::clear_text() const
	{
		setfillcolor(BACKGROUND_COLOR);
		solidrectangle(LEFT + TEXT_X, TOP + TEXT_Y,
			LEFT + TEXT_X + TEXT_WIDTH, TOP + TEXT_Y + TEXT_HEIGHT);
	}

	// 更新按钮信息并绘制按钮
	void Controls::update_button(BUTTON button_type, LPCTSTR text)
	{
		buttons[button_type].set_text(text);
		buttons[button_type].draw();
	}

	// 重置控件
	void Controls::reset()
	{
		chess.color = Chess::COLOR::NONE;
		chess.visible = false;
		clear_text();
	}

	// 切换模式
	void Controls::toggle_mode()
	{
		if (_mode == MODE::M_DOUBLE)
		{
			_mode = MODE::M_SINGLE;
			update_button(B_MODE, CONTENT::MODE_SINGLE);
		}
		else if (_mode == MODE::M_SINGLE)
		{
			_mode = MODE::M_DOUBLE;
			update_button(B_MODE, CONTENT::MODE_DOUBLE);
		}
	}
}