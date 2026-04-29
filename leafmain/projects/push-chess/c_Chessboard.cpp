#include "pushchess.h"

// 棋盘数据实例化
namespace pushchess
{
	// 棋盘数据默认构造函数
	ChessboardData::ChessboardData() :
		ChessboardData(Chessboard::CHESS_RADIUS, 
			Chessboard::GRID_SIZE) {}

	// 棋盘数据构造函数
	ChessboardData::ChessboardData(const int CHESS_RADIUS, 
		const int GRID_SIZE) :
		CHESS_RADIUS(CHESS_RADIUS), GRID_SIZE(GRID_SIZE)
	{
		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COLUMN; j++)
			{
				data[i][j].resize(CHESS_RADIUS);
				data[i][j].x = Chessboard::LEFT + Chessboard::GRID_MARGIN_LEFT + 
					j * GRID_SIZE + GRID_SIZE / 2;
				data[i][j].y = Chessboard::TOP + Chessboard::GRID_MARGIN_LEFT + 
					i * GRID_SIZE + GRID_SIZE / 2;
			}
		}
	}

	// 设置数据，带边界检查
	void ChessboardData::set_data(int row, int column, Chess::COLOR color,
		Chess::TYPE type, bool visible, bool exist)
	{
		// 先检查边界
		if (!in_range(row, column)) return;
		// 再调用 Chess::set_data 设置数据
		data[row][column].set_data(color, type, data[row][column].x, 
			data[row][column].y, data[row][column].radius, 
			visible, exist);
	}

	// 检查行号和列号是否在合理范围内
	bool ChessboardData::in_range(int row, int column) const
	{
		return row >= 0 && row < ROW && column >= 0 && column < COLUMN;
	}

	// 将所有棋盘数据打乱
	void ChessboardData::shuffle(bool visible)
	{
		LIST cd = ordered_list();
		std::random_device rd;
		// 使用 Mersenne Twister 引擎
		std::mt19937 gen(rd());
		std::shuffle(cd.begin(), cd.end(), gen);
		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COLUMN; j++)
			{
				int index = i * COLUMN + j;
				data[i][j].set_data(cd[index].color, cd[index].type, 
					data[i][j].x, data[i][j].y, data[i][j].radius, 
					visible, true);
			}
		}
	}

	// 将棋盘数据赋给原来棋盘数据
	ChessboardData& ChessboardData::operator=(const ChessboardData& cd)
	{
		// 对棋子逐个赋值
		for (int i = 0; i < Chessboard::ROW; i++)
		{
			for (int j = 0; j < Chessboard::COLUMN; j++)
			{
				data[i][j] = cd.data[i][j];
			}
		}
		return *this;
	}

	// 比较两个棋盘数据是否完全相等
	bool ChessboardData::operator==(const ChessboardData& cd) const
	{
		// 对棋子逐个比较
		for (int i = 0; i < Chessboard::ROW; i++)
		{
			for (int j = 0; j < Chessboard::COLUMN; j++)
			{
				if (data[i][j] != cd.data[i][j])
				{
					return false;
				}
			}
		}
		return true;
	}

	// 比较两个棋盘数据是否不完全相等
	bool ChessboardData::operator!=(const ChessboardData& cd) const
	{
		return !(this->operator==(cd));
	}

	// 比较两个棋盘数据的逻辑大小
	bool ChessboardData::operator<(const ChessboardData& cd) const
	{
		// 棋盘地址相等时直接返回 false
		if (this == &cd) return false;
		// 按照行优先，对棋子逐个比较
		for (int i = 0; i < Chessboard::ROW; i++)
		{
			for (int j = 0; j < Chessboard::COLUMN; j++)
			{
				if (data[i][j] != cd.data[i][j])
				{
					return data[i][j] < cd.data[i][j];
				}
			}
		}
		return false;
	}

	// 静态方法，获取棋盘棋子有序列表
	ChessboardData::LIST ChessboardData::ordered_list()
	{
		// 如果曾经生成过则直接返回曾经生成的值
		static bool called = false;
		static LIST cd;
		if (called) return cd;
		called = true;
		// 设置单种颜色棋子数量初始值
		std::map<Chess::TYPE, int> type_map;
		type_map[Chess::TYPE::KING] = 1;
		type_map[Chess::TYPE::ADVISOR] = 2;
		type_map[Chess::TYPE::BISHOP] = 2;
		type_map[Chess::TYPE::KNIGHT] = 2;
		type_map[Chess::TYPE::ROOK] = 2;
		type_map[Chess::TYPE::CANNON] = 2;
		type_map[Chess::TYPE::PAWN] = 5;
		// 获得顺序列表
		for (int color = 1; color <= 2; color++)
		{
			for (int type = 1; type <= 7; type++)
			{
				Chess::COLOR cl = static_cast<Chess::COLOR>(color);
				Chess::TYPE ty = static_cast<Chess::TYPE>(type);
				Chess::Pair p = { cl, ty };
				for (int i = 0; i < type_map[ty]; i++)
				{
					cd.push_back(p);
				}
			}
		}
		return cd;
	}
}

// 棋盘类实例化
namespace pushchess
{
	// 棋盘构造函数
	Chessboard::Chessboard() : 
		chesses(CHESS_RADIUS, GRID_SIZE) {}

	// 绘制棋盘
	void Chessboard::draw() const
	{
		draw_background(); // 绘制棋盘背景
		draw_grid(); // 绘制棋盘格线
		draw_dot(); // 绘制棋盘点
	}

	// 撤销一步，悔棋
	void Chessboard::regret()
	{
		if (history_stack.size() <= 1) return;
		blur(); // 取消选中
		// 从历史记录和栈中移除数据
		history_set.erase(history_stack.top());
		history_stack.pop();
		// 将历史记录栈顶数据赋值给棋盘数据
		chesses = history_stack.top();
		place_chesses(true);
		_step--;
		// 如果回到了开局，则重置玩家
		if (_step == 0)
		{
			_player.reset(); // 重置玩家
		}
		_player.go_previous(); // 切换回上一手玩家
		send_message(MESSAGE::BOARD_REGRET);
		// 撤回后，如果吃了暗棋，则将其计数减一
		if (invisible_kill_stack.empty()) return;
		if (invisible_kill_stack.top())
		{
			_invisible_kill_count--;
		}
		invisible_kill_stack.pop();
		// 如果游戏结束，则设置为未结束
		if (_game_ended)
		{
			_game_ended = false;
		}
	}

	// 重置棋盘，回到初始状态
	void Chessboard::reset()
	{
		blur();
		// 清空栈和集合
		history_set.clear();
		// 清空历史记录栈和暗棋击杀栈
		while (!history_stack.empty())
		{
			history_stack.pop();
		}
		while (!invisible_kill_stack.empty())
		{
			invisible_kill_stack.pop();
		}
		// 调用 Chess::shuffle 将数据打乱，且棋子不可见
		chesses.shuffle(false);
		// 重置步数和玩家以及暗棋击杀计数
		_step = 0;
		_player.reset();
		_invisible_kill_count = 0;
		// 重置历史记录栈和集合
		history_stack.push(chesses);
		history_set.insert(chesses);
		place_chesses(false);
		send_message(MESSAGE::BOARD_RESET);
		_game_ended = false;
	}

	// 监听事件，根据事件进行响应
	void Chessboard::listen_event_message(ExMessage& msg)
	{
		// 鼠标左键按下
		if (msg.message == WM_LBUTTONDOWN)
		{
			// 计算相对坐标，并检查是否在棋盘范围内且在棋子圆形范围内
			int rx = rel_x(msg.x), ry = rel_y(msg.y);
			if (_game_ended) return;
			if (!in_range(rx, ry)) return;
			Chess& chess = chesses.data[ry][rx]; // 引用当前棋子
			if (!chess.in_area(msg.x, msg.y)) return;
			// 已有选中棋子
			if (has_selected()) 
			{
				// 按照规则移动选中棋子
				if (rx != selected_column || ry != selected_row)
				{
					move_chess_by_rule(ry, rx); // 按照规则移动棋子
				}
				// 如果再次选中同一个棋子，则取消选中，并发送消息
				else
				{
					blur();
					send_message(MESSAGE::BOARD_BLUR);
				}
			}
			// 无选中棋子
			else
			{
				// 如果棋子不可见，则将棋子揭开
				if (!chess.visible)
				{
					uncover_chess(ry, rx);
					// 如果是第一步，还没有确定玩家，则确定玩家
					if (_player.current == Player::NONE)
					{
						switch (chess.color)
						{
						case Chess::COLOR::C_RED:
							_player.current = Player::P_RED;
							break;
						case Chess::COLOR::C_BLACK:
							_player.current = Player::P_BLACK;
							break;
						default: break;
						}
					}
					step_forward(chesses);
				}
				else
				{
					int rx = rel_x(chess.x), ry = rel_y(chess.y);
					// 选中棋子
					select_chess(ry, rx);
				}
			}
		}
		// 鼠标右键按下
		else if (msg.message == WM_RBUTTONDOWN)
		{
			// 计算相对坐标，并检查是否在棋盘范围内且在棋子圆形范围内
			int rx = rel_x(msg.x), ry = rel_y(msg.y);
			if (!in_range(rx, ry) || !chesses.data[ry][rx]
				.in_area(msg.x, msg.y)) return;
			// 只有选中帅或将时才有右键吃子功能
			if (chesses.data[selected_row][selected_column]
				.type == Chess::TYPE::KING && has_selected())
			{
				if (_game_ended) return;
				// 如果不是再次点击自己，则吃子
				if (rx != selected_column || ry != selected_row)
				{
					move_chess_by_rule(ry, rx, true);
				}
			}
			// 否则如果没有选中棋子，则悔棋
			else if (!has_selected())
			{
				regret();
			}
		}
		// 鼠标中键按下
		else if (msg.message == WM_MBUTTONDOWN)
		{
			// 计算相对坐标，并检查是否在棋盘范围内且在棋子圆形范围内
			int rx = rel_x(msg.x), ry = rel_y(msg.y);
			if (!in_range(rx, ry) || !chesses.data[ry][rx]
				.in_area(msg.x, msg.y)) return;
			// 按下鼠标中键时，重置棋盘
			reset();
		}
		// 按下 Backspace 键撤销一步
		else if (msg.message == WM_KEYDOWN && msg.vkcode == VK_BACK)
		{
			regret();
		}
		// 按下 Delete 键重置棋盘
		else if (msg.message == WM_KEYDOWN && msg.vkcode == VK_DELETE)
		{
			reset();
		}
		// 按下 Space 键失去焦点
		else if (msg.message == WM_KEYDOWN && msg.vkcode == VK_SPACE)
		{
			if (has_selected())
			{
				blur();
				send_message(MESSAGE::BOARD_BLUR);
			}
		}
	}

	// 侦查棋盘消息
	void Chessboard::peek_board_message(MESSAGE* msg)
	{
		// 消息相同时无需赋值
		if (*msg != _message)
		{
			*msg = _message;
		}
		// 如果有消息，则将消息赋给 *msg，并立即清空
		if (_message != MESSAGE::NONE)
		{
			_message = MESSAGE::NONE;
		}
	}

	// 取消棋盘的选中状态
	void Chessboard::blur()
	{
		if (has_selected() && in_range(selected_column, selected_row))
		{
			chesses.data[selected_row][selected_column].unselect();
			selected_row = selected_column = -1;
		}
	}

	// 判断棋盘是否有焦点，即是否有选中棋子
	bool Chessboard::has_focus() const
	{
		return has_selected() && history_set.size() >= 1;
	}

	// 操作棋子，两个棋子相同则揭开
	bool Chessboard::operate_chess(int from_row, int from_column,
		int to_row, int to_column, bool use_right)
	{
		if (_game_ended) return false;
		// 如果没有选中棋子，则揭开棋子
		if (!chesses.data[from_row][from_column].visible)
		{
			uncover_chess(from_row, from_column);
			return true;
		}
		// 选中并移动棋子
		select_chess(from_row, from_column);
		return move_chess_by_rule(to_row, to_column, use_right);
	}

	// 获取棋盘当前步数
	int Chessboard::get_step() const
	{
		return _step;
	}

	// 获取当前玩家的棋子数量
	int Chessboard::get_chess_count() const
	{
		int result = 0;
		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COLUMN; j++)
			{
				if (is_player_chess(i, j))
				{
					result++;
				}
			}
		}
		return result;
	}

	// 获取当前吃掉的暗棋数量
	int Chessboard::get_invisible_kill_count() const
	{
		return _invisible_kill_count;
	}

	// 获取当前玩家
	const Player& Chessboard::get_player() const
	{
		return _player;
	}

	// 返回棋盘数据的常引用
	const ChessboardData& Chessboard::data() const
	{
		return chesses;
	}

	// 检查相对坐标是否在合理范围内
	bool Chessboard::in_range(int rel_x, int rel_y) const
	{
		return rel_x >= 0 && rel_x < COLUMN &&
			rel_y >= 0 && rel_y < ROW;
	}

	// 计算棋盘绝对 x 坐标
	int Chessboard::abs_x(int rel_x) const
	{
		return LEFT + GRID_MARGIN_LEFT + rel_x * GRID_SIZE;
	}

	// 计算棋盘绝对 y 坐标
	int Chessboard::abs_y(int rel_y) const
	{
		return TOP + GRID_MARGIN_TOP + rel_y * GRID_SIZE;
	}

	// 计算棋盘相对 x 坐标
	int Chessboard::rel_x(int abs_x) const
	{
		return (abs_x - LEFT - GRID_MARGIN_LEFT) / GRID_SIZE;
	}

	// 计算棋盘相对 y 坐标
	int Chessboard::rel_y(int abs_y) const
	{
		return (abs_y - TOP - GRID_MARGIN_LEFT) / GRID_SIZE;
	}

	// 绘制棋盘格线
	void Chessboard::draw_grid() const
	{
		// 绘制网格线
		for (int x = 0; x < COLUMN; x++)
		{
			for (int y = 0; y < ROW; y++)
			{
				// 绘制网格线，调用补充棋盘格函数
				supple_grid(x, y);
			}
		}
	}

	// 绘制棋盘背景
	void Chessboard::draw_background() const
	{
		setlinecolor(OUTLINE_COLOR);
		setlinestyle(OUTLINE_TYPE, OUTLINE_WIDTH);
		setfillcolor(BACKGROUND_COLOR);
		int right = LEFT + GRID_MARGIN_LEFT * 2 + WIDTH;
		int bottom = TOP + GRID_MARGIN_TOP * 2 + HEIGHT;
		int offset = OUTLINE_OFFSET;
		// 绘制棋盘背景
		solidrectangle(LEFT, TOP, right, bottom);
		// 绘制棋盘外框
		rectangle(LEFT + offset, TOP + offset,
			right - offset, bottom - offset);
	}

	// 绘制棋盘点
	void Chessboard::draw_dot() const
	{
		for (int y = 1; y <= 2; y++)
		{
			for (int x = 0; x <= COLUMN; x++)
			{
				// 绘制网格线上的点
				if (y == 1 && x % 2 == 0 || y == 2 && x % 6 == 1)
				{
					supple_dot(x, y);
				}
			}
		}
	}

	// 补充棋盘点
	void Chessboard::supple_dot(int x, int y) const
	{
		const int ax = abs_x(x), ay = abs_y(y);
		const int offset = DOT_OFFSET;
		const int length = DOT_LENGTH;
		// 这里列数和行数无需减 1，因为相对坐标系以 0 开始，
		// 点是绘制在网格线上的，每行（列）的网格线数比网格数多 1。
		setlinecolor(DOT_COLOR);
		setlinestyle(GRID_LINE_TYPE, DOT_WIDTH);
		// 左上角
		if (x != 0 && y != 0)
		{
			line(ax - offset, ay - offset,
				ax - offset - length, ay - offset);
			line(ax - offset, ay - offset,
				ax - offset, ay - offset - length);
		}
		// 右上角
		if (x != COLUMN && y != 0)
		{
			line(ax + offset, ay - offset,
				ax + offset + length, ay - offset);
			line(ax + offset, ay - offset,
				ax + offset, ay - offset - length);
		}
		// 左下角
		if (x != 0 && y != ROW)
		{
			line(ax - offset, ay + offset,
				ax - offset - length, ay + offset);
			line(ax - offset, ay + offset,
				ax - offset, ay + offset + length);
		}
		// 右下角
		if (x != COLUMN && y != ROW)
		{
			line(ax + offset, ay + offset,
				ax + offset + length, ay + offset);
			line(ax + offset, ay + offset,
				ax + offset, ay + offset + length);
		}
	}

	// 补充棋盘 45 度斜线
	void Chessboard::supple_slope(int x, int y, bool slope_1) const
	{
		setlinecolor(GRID_COLOR);
		setlinestyle(GRID_LINE_TYPE, GRID_LINE_WIDTH);
		if (slope_1)
		{
			line(abs_x(x), abs_y(y), abs_x(x + 1), abs_y(y + 1));
		}
		else
		{
			line(abs_x(x + 1), abs_y(y), abs_x(x), abs_y(y + 1));
		}
	}

	// 补充棋盘格
	void Chessboard::supple_grid(int x, int y) const
	{
		setlinecolor(GRID_COLOR);
		setlinestyle(GRID_LINE_TYPE, GRID_LINE_WIDTH);
		// 绘制网格
		rectangle(abs_x(x), abs_y(y), abs_x(x + 1), abs_y(y + 1));

		if (x == 3 && y == 2 || x == 4 && y == 3)
		{
			// 绘制 45 度斜线
			supple_slope(x, y, true);
		}
		else if (x == 3 && y == 3 || x == 4 && y == 2)
		{
			supple_slope(x, y, false);
		}
	}

	// 摆放棋子
	void Chessboard::place_chesses(bool flush) const
	{
		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COLUMN; j++)
			{
				// 存在并且未绘制才绘制
				if (!chesses.data[i][j].exist)
				{
					if (flush || chesses.data[i][j].has_drawn())
					{
						chesses.data[i][j].remove();
						supple_grid(j, i);
					}
				}
				else if (!chesses.data[i][j].has_drawn())
				{
					chesses.data[i][j].draw();
				}
				
			}
		}
	}

	// 揭开棋子
	void Chessboard::uncover_chess(int row, int column)
	{
		chesses.data[row][column].uncover();
		send_message(MESSAGE::CHESS_UNCOVER);
	}

	// 棋盘的下一步
	void Chessboard::step_forward(ChessboardData& chessboard_data)
	{
		// 保存当前棋盘数据
		history_stack.push(chessboard_data);
		history_set.insert(chessboard_data);
		// 如果没有击杀暗棋，则向暗棋击杀栈中压入 false
		if (!_invisible_pushed)
		{
			invisible_kill_stack.push(false);
		}
		// 由于值可能被修改，因此重新赋值
		_invisible_pushed = false;
		_step++; // 步数加 1
		_player.go_next(); // 切换到下一个玩家
		if (get_chess_count() == 0)
		{
			_game_ended = true;
			send_message(MESSAGE::PLAYER_LOSE);
			blur(); // 取消选中
			return;
		}
	}

	// 发送棋盘消息
	void Chessboard::send_message(const MESSAGE&& message)
	{
		_message = message; // 保存消息
	}

	// 选中指定行列的棋子
	void Chessboard::select_chess(int row, int column)
	{
		// 超出棋盘范围则不选中
		if (!in_range(column, row)) return;
		// 如果试图选择不是自己的棋子，则忽略
		if (!is_player_chess(row, column))
		{
			// 发送不在回合消息
			send_message(MESSAGE::NOT_IN_TURN);
			return;
		}
		// 棋子不存在则不选中
		if (!chesses.data[row][column].exist) return;
		// 已有选中棋子则取消选中
		if (has_selected())
		{
			blur();
		}
		selected_row = row;
		selected_column = column;
		// 调用棋子的 select 方法选中棋子
		chesses.data[row][column].select();
		// 使用 switch 映射转换，发送消息
		switch (chesses.data[row][column].type)
		{
		case Chess::TYPE::ADVISOR:
			send_message(MESSAGE::SELECT_ADVISOR);
			break;
		case Chess::TYPE::BISHOP:
			send_message(MESSAGE::SELECT_BISHOP);
			break;
		case Chess::TYPE::CANNON:
			send_message(MESSAGE::SELECT_CANNON);
			break;
		case Chess::TYPE::KING:
			send_message(MESSAGE::SELECT_KING);
			break;
		case Chess::TYPE::KNIGHT:
			send_message(MESSAGE::SELECT_KNIGHT);
			break;
		case Chess::TYPE::PAWN:
			send_message(MESSAGE::SELECT_PAWN);
			break;
		case Chess::TYPE::ROOK:
			send_message(MESSAGE::SELECT_ROOK);
			break;
		default:
			break;
		}
	}

	// 设置指定行列的棋子将被移除
	void Chessboard::set_remove(int row, int column)
	{
		// 保存将被移除棋子的行列
		removed_row = row; 
		removed_column = column;
	}

	// 清除将被移除的棋子
	void Chessboard::clear_remove()
	{
		// 清除将被移除棋子的行列，置为 -1 表示无棋子
		removed_row = -1;
		removed_column = -1;
	}

	// 判断是否有棋子被选中
	bool Chessboard::has_selected() const
	{
		return selected_row >= 0 && selected_column >= 0;
	}

	// 判断是否有棋子将被移除
	bool Chessboard::has_set_remove() const
	{
		return removed_row >= 0 && removed_column >= 0;
	}

	// 发送吃掉一个暗棋信息，并检查是否成功
	bool Chessboard::kill_invisible()
	{
		if (_invisible_kill_count < MAX_INVISIBLE_KILL_COUNT)
		{
			_invisible_kill_count++;
			invisible_kill_stack.push(true);
			_invisible_pushed = true;
			send_message(MESSAGE::KILL_INVISIBLE);
		}
		else
		{
			send_message(MESSAGE::EXCEED_INVISIBLE_KILL);
			return false;
		}
		return true;
	}

	// 检查指定位置的棋子是否属于当前玩家
	bool Chessboard::is_player_chess(int row, int column) const
	{
		Chess::COLOR color = chesses.data[row][column].color;
		switch (_player.current)
		{
		case Player::P_RED:
			return Chess::COLOR::C_RED == color;
		case Player::P_BLACK:
			return Chess::COLOR::C_BLACK == color;
		default:
			return false;
		}
		return false;
	}

	// 将棋子往指定方向推，可指兵、卒、帅、将
	bool Chessboard::chess_push(ChessboardData* chessboard_data,
		int row, int column, Direction::TYPE dir)
	{
		// 移动规则：
		// 1. 只能朝四个方向移动一格；
		// 2. 如果目标点有棋子，则会将该棋子以及与它相邻的棋子
		//	  一起朝指定方向移动一格，即整体推动；
		// 3. 如果有边缘的棋子推出了棋盘，则边缘的棋子会被吃掉；
		// 4. 允许吃掉暗棋以及自己的棋子。
		int emp = 0;
		switch (dir)
		{
		case Direction::UP:
			if (row != selected_row - 1)
			{
				return false;
			}
			for (emp = row; emp >= 0; emp--)
			{
				if (!chessboard_data->data[emp][column].exist)
				{
					break;
				}
			}
			if (emp < 0)
			{
				emp = 0;
				if (chessboard_data->data[emp][column].visible)
				{
					send_message(MESSAGE::KILL_VISIBLE);
				}
				else if (!kill_invisible())
				{
					return false;
				}
			}
			for (int i = emp; i <= row; i++)
			{
				chessboard_data->data[i][column] =
					chessboard_data->data[i + 1][column];
			}
			return true;
		case Direction::DOWN:
			if (row != selected_row + 1)
			{
				return false;
			}
			for (emp = row; emp < ROW; emp++)
			{
				if (!chessboard_data->data[emp][column].exist)
				{
					break;
				}
			}
			if (emp >= ROW)
			{
				emp = ROW - 1;
				if (chessboard_data->data[emp][column].visible)
				{
					send_message(MESSAGE::KILL_VISIBLE);
				}
				else if (!kill_invisible())
				{
					return false;
				}
			}
			for (int i = emp; i >= row; i--)
			{
				chessboard_data->data[i][column] =
					chessboard_data->data[i - 1][column];
			}
			return true;
		case Direction::LEFT:
			if (column != selected_column - 1)
			{
				return false;
			}
			for (emp = column; emp >= 0; emp--)
			{
				if (!chessboard_data->data[row][emp].exist)
				{
					break;
				}
			}
			if (emp < 0)
			{
				emp = 0;
				if (chessboard_data->data[row][emp].visible)
				{
					send_message(MESSAGE::KILL_VISIBLE);
				}
				else if (!kill_invisible())
				{
					return false;
				}
			}
			for (int i = emp; i <= column; i++)
			{
				chessboard_data->data[row][i] =
					chessboard_data->data[row][i + 1];
			}
			return true;
		case Direction::RIGHT:
			if (column != selected_column + 1)
			{
				return false;
			}
			for (emp = column; emp < COLUMN; emp++)
			{
				if (!chessboard_data->data[row][emp].exist)
				{
					break;
				}
			}
			if (emp >= COLUMN)
			{
				emp = COLUMN - 1;
				if (chessboard_data->data[row][emp].visible)
				{
					send_message(MESSAGE::KILL_VISIBLE);
				}
				else if (!kill_invisible())
				{
					return false;
				}
			}
			for (int i = emp; i >= column; i--)
			{
				chessboard_data->data[row][i] =
					chessboard_data->data[row][i - 1];
			}
			return true;
		default:
			return false;
		}
		return false;
	}

	// 将棋子往指定方向踢，可指马、帅、将
	bool Chessboard::chess_kick(ChessboardData* chessboard_data,
		int row, int column, Direction::TYPE dir, bool die)
	{
		// 移动规则：
		// 1. 只能朝四个方向移动一格；
		// 2. 如果目标点有棋子，则会将该棋子向指定方向踢，
		//    即让它移动到后面第一个空格上；
		// 3. 如果目标棋子后面没有空格，则目标棋子会被吃掉；
		// 4. 允许吃掉对方的棋子、暗棋以及自己的棋子。
		int emp = 0;
		switch (dir)
		{
		case Direction::UP:
			if (row != selected_row - 1)
			{
				return false;
			}
			for (emp = row; emp >= 0; emp--)
			{
				if (!chessboard_data->data[emp][column].exist)
				{
					break;
				}
			}
			if (emp < 0)
			{
				if (row < 0) return false;
				if (chessboard_data->data[row][column].visible)
				{
					send_message(MESSAGE::KILL_VISIBLE);
				}
				else if (!kill_invisible())
				{
					return false;
				}
			}
			else
			{
				chessboard_data->data[emp][column] =
					chessboard_data->data[row][column];
			}
			chessboard_data->data[row][column] =
				chessboard_data->data[selected_row][column];
			if (die)
			{
				set_remove(row, column);
			}
			return true;
		case Direction::DOWN:
			if (row != selected_row + 1)
			{
				return false;
			}
			for (emp = row; emp < ROW; emp++)
			{
				if (!chessboard_data->data[emp][column].exist)
				{
					break;
				}
			}
			if (emp >= ROW)
			{
				if (row >= ROW) return false;
				if (chessboard_data->data[row][column].visible)
				{
					send_message(MESSAGE::KILL_VISIBLE);
				}
				else if (!kill_invisible())
				{
					return false;
				}
			}
			else
			{
				chessboard_data->data[emp][column] =
					chessboard_data->data[row][column];
			}
			chessboard_data->data[row][column] =
				chessboard_data->data[selected_row][column];
			if (die)
			{
				set_remove(row, column);
			}
			return true;
		case Direction::LEFT:
			if (column != selected_column - 1)
			{
				return false;
			}
			for (emp = column; emp >= 0; emp--)
			{
				if (!chessboard_data->data[row][emp].exist)
				{
					break;
				}
			}
			if (emp < 0)
			{
				if (column < 0) return false;
				if (chessboard_data->data[row][column].visible)
				{
					send_message(MESSAGE::KILL_VISIBLE);
				}
				else if (!kill_invisible())
				{
					return false;
				}
			}
			else
			{
				chessboard_data->data[row][emp] =
					chessboard_data->data[row][column];
			}
			chessboard_data->data[row][column] =
				chessboard_data->data[row][selected_column];
			if (die)
			{
				set_remove(row, column);
			}
			return true;
		case Direction::RIGHT:
			if (column != selected_column + 1)
			{
				return false;
			}
			for (emp = column; emp < COLUMN; emp++)
			{
				if (!chessboard_data->data[row][emp].exist)
				{
					break;
				}
			}
			if (emp >= COLUMN)
			{
				if (column >= COLUMN) return false;
				if (chessboard_data->data[row][column].visible)
				{
					send_message(MESSAGE::KILL_VISIBLE);
				}
				else if (!kill_invisible())
				{
					return false;
				}
			}
			else
			{
				chessboard_data->data[row][emp] =
					chessboard_data->data[row][column];
			}
			chessboard_data->data[row][column] =
				chessboard_data->data[row][selected_column];
			if (die)
			{
				set_remove(row, column);
			}
			return true;
		default:
			return false;
		}
		return false;
	}

	// 将棋子往指定方向撞，尤指车
	bool Chessboard::chess_crush(ChessboardData* chessboard_data,
		int row, int column, Direction::TYPE dir)
	{
		// 移动规则：
		// 1. 在不锁定目标棋子时，只能朝四个方向移动一格；
		// 2. 如果锁定了目标棋子，则会先多格移动到对方的位置的前面，
		//    紧接着向指定方向整体推动一格，和普通的推动规则一样；
		// 3. 如果有边缘的棋子推出了棋盘，则边缘的棋子会被吃掉；
		// 4. 允许吃掉对方的棋子、暗棋以及自己的棋子。
		int emp = 0;
		switch (dir)
		{
		case Direction::UP:
			for (emp = selected_row - 1; emp >= 0; emp--)
			{
				if (chessboard_data->data[emp][column].exist)
				{
					break;
				}
			}
			if (row != emp)
			{
				return chess_push(chessboard_data, row, column, dir);
			}
			for (emp = row; emp >= 0; emp--)
			{
				if (!chessboard_data->data[emp][column].exist)
				{
					break;
				}
			}
			if (emp < 0)
			{
				emp = 0;
				if (chessboard_data->data[emp][column].visible)
				{
					send_message(MESSAGE::KILL_VISIBLE);
				}
				else if (!kill_invisible())
				{
					return false;
				}
			}
			for (int i = emp; i < row; i++)
			{
				chessboard_data->data[i][column] =
					chessboard_data->data[i + 1][column];
			}
			if (row >= 0)
			{
				chessboard_data->data[row][column] =
					chessboard_data->data[selected_row][column];
			}
			return true;
		case Direction::DOWN:
			for (emp = selected_row + 1; emp < ROW; emp++)
			{
				if (chessboard_data->data[emp][column].exist)
				{
					break;
				}
			}
			if (row != emp)
			{
				return chess_push(chessboard_data, row, column, dir);
			}
			for (emp = row; emp < ROW; emp++)
			{
				if (!chessboard_data->data[emp][column].exist)
				{
					break;
				}
			}
			if (emp >= ROW)
			{
				emp = ROW - 1;
				if (chessboard_data->data[emp][column].visible)
				{
					send_message(MESSAGE::KILL_VISIBLE);
				}
				else if (!kill_invisible())
				{
					return false;
				}
			}
			for (int i = emp; i > row; i--)
			{
				chessboard_data->data[i][column] =
					chessboard_data->data[i - 1][column];
			}
			if (row < ROW)
			{
				chessboard_data->data[row][column] =
					chessboard_data->data[selected_row][column];
			}
			return true;
		case Direction::LEFT:
			for (emp = selected_column - 1; emp >= 0; emp--)
			{
				if (chessboard_data->data[row][emp].exist)
				{
					break;
				}
			}
			if (column != emp)
			{
				return chess_push(chessboard_data, row, column, dir);
			}
			for (emp = column; emp >= 0; emp--)
			{
				if (!chessboard_data->data[row][emp].exist)
				{
					break;
				}
			}
			if (emp < 0)
			{
				emp = 0;
				if (chessboard_data->data[row][emp].visible)
				{
					send_message(MESSAGE::KILL_VISIBLE);
				}
				else if (!kill_invisible())
				{
					return false;
				}
			}
			for (int i = emp; i < column; i++)
			{
				chessboard_data->data[row][i] =
					chessboard_data->data[row][i + 1];
			}
			if (column >= 0)
			{
				chessboard_data->data[row][column] =
					chessboard_data->data[row][selected_column];
			}
			return true;
		case Direction::RIGHT:
			for (emp = selected_column + 1; emp < COLUMN; emp++)
			{
				if (chessboard_data->data[row][emp].exist)
				{
					break;
				}
			}
			if (column != emp)
			{
				return chess_push(chessboard_data, row, column, dir);
			}
			for (emp = column; emp < COLUMN; emp++)
			{
				if (!chessboard_data->data[row][emp].exist)
				{
					break;
				}
			}
			if (emp >= COLUMN)
			{
				emp = COLUMN - 1;
				if (chessboard_data->data[row][emp].visible)
				{
					send_message(MESSAGE::KILL_VISIBLE);
				}
				else if (!kill_invisible())
				{
					return false;
				}
			}
			for (int i = emp; i > column; i--)
			{
				chessboard_data->data[row][i] =
					chessboard_data->data[row][i - 1];
			}
			if (column < COLUMN)
			{
				chessboard_data->data[row][column] =
					chessboard_data->data[row][selected_column];
			}
			return true;
		default:
			return false;
		}
		return false;
	}

	// 将棋子往指定方向冲，尤指相、象
	bool Chessboard::chess_dash(ChessboardData* chessboard_data,
		int row, int column, Direction::TYPE dir)
	{
		// 移动规则：
		// 1. 在不锁定目标棋子时，只能朝四个方向移动一格；
		// 2. 如果锁定了目标棋子，则会先多格移动到对方的位置的前面，若符合
		//    移动限制，则向指定方向整体踢一格，和普通的踢棋规则一样；
		// 3. 移动限制：
		// (1) 棋子对暗棋作用无效，不可踢动，无论是否踢出棋盘；
		// (2) 棋子可以踢动自己的棋子，但不能踢出棋盘；
		// (3) 一旦棋子将对方的棋子踢出棋盘，则会与对方的棋子同归于尽。
		// 
		int emp = 0;
		bool sur = false;
		switch (dir)
		{
		case Direction::UP:
			for (emp = selected_row - 1; emp >= 0; emp--)
			{
				if (chessboard_data->data[emp][column].exist)
				{
					break;
				}
			}
			for (int i = row; i >= 0; i--)
			{
				if (!chessboard_data->data[i][column].exist)
				{
					sur = true;
					break;
				}
			}
			if (row >= 0 && 
				(!chessboard_data->data[row][column].visible ||
				!sur && chessboard_data->data[row][column].color == 
				chessboard_data->data[selected_row][column].color))
			{
				return false;
			}
			if (row != emp)
			{
				return chess_kick(chessboard_data, row, column, dir, !sur);
			}
			for (emp = row; emp >= 0; emp--)
			{
				if (!chessboard_data->data[emp][column].exist)
				{
					break;
				}
			}
			if (emp < 0)
			{
				if (row < 0) return false;
				if (chessboard_data->data[row][column].visible)
				{
					send_message(MESSAGE::KILL_VISIBLE);
				}
				else if (!kill_invisible())
				{
					return false;
				}
			}
			else
			{
				chessboard_data->data[emp][column] =
					chessboard_data->data[row][column];
			}
			chessboard_data->data[row][column] =
				chessboard_data->data[selected_row][column];
			if (!sur)
			{
				set_remove(row, column);
			}
			return true;
		case Direction::DOWN:
			for (emp = selected_row + 1; emp < ROW; emp++)
			{
				if (chessboard_data->data[emp][column].exist)
				{
					break;
				}
			}
			for (int i = row; i < ROW; i++)
			{
				if (!chessboard_data->data[i][column].exist)
				{
					sur = true;
					break;
				}
			}
			if (row < ROW && 
				(!chessboard_data->data[row][column].visible ||
				!sur && chessboard_data->data[row][column].color ==
				chessboard_data->data[selected_row][column].color))
			{
				return false;
			}
			if (row != emp)
			{
				return chess_kick(chessboard_data, row, column, dir, !sur);
			}
			for (emp = row; emp < ROW; emp++)
			{
				if (!chessboard_data->data[emp][column].exist)
				{
					break;
				}
			}
			if (emp >= ROW)
			{
				if (row >= ROW) return false;
				if (chessboard_data->data[row][column].visible)
				{
					send_message(MESSAGE::KILL_VISIBLE);
				}
				else if (!kill_invisible())
				{
					return false;
				}
			}
			else
			{
				chessboard_data->data[emp][column] =
					chessboard_data->data[row][column];
			}
			chessboard_data->data[row][column] =
				chessboard_data->data[selected_row][column];
			if (!sur)
			{
				set_remove(row, column);
			}
			return true;
		case Direction::LEFT:
			for (emp = selected_column - 1; emp >= 0; emp--)
			{
				if (chessboard_data->data[row][emp].exist)
				{
					break;
				}
			}
			for (int i = column; i >= 0; i--)
			{
				if (!chessboard_data->data[row][i].exist)
				{
					sur = true;
					break;
				}
			}
			if (column >= 0 && 
				(!chessboard_data->data[row][column].visible ||
				!sur && chessboard_data->data[row][column].color ==
					chessboard_data->data[row][selected_column].color))
			{
				return false;
			}
			if (column != emp)
			{
				return chess_kick(chessboard_data, row, column, dir, !sur);
			}
			for (emp = column; emp >= 0; emp--)
			{
				if (!chessboard_data->data[row][emp].exist)
				{
					break;
				}
			}
			if (emp < 0)
			{
				if (column < 0) return false;
				if (chessboard_data->data[row][column].visible)
				{
					send_message(MESSAGE::KILL_VISIBLE);
				}
				else if (!kill_invisible())
				{
					return false;
				}
			}
			else
			{
				chessboard_data->data[row][emp] =
					chessboard_data->data[row][column];
			}
			chessboard_data->data[row][column] =
				chessboard_data->data[row][selected_column];
			if (!sur)
			{
				set_remove(row, column);
			}
			return true;
		case Direction::RIGHT:
			for (emp = selected_column + 1; emp < COLUMN; emp++)
			{
				if (chessboard_data->data[row][emp].exist)
				{
					break;
				}
			}
			for (int i = column; i < COLUMN; i++)
			{
				if (!chessboard_data->data[row][i].exist)
				{
					sur = true;
					break;
				}
			}
			if (column < COLUMN && 
				(!chessboard_data->data[row][column].visible ||
				!sur && chessboard_data->data[row][column].color ==
					chessboard_data->data[row][selected_column].color))
			{
				return false;
			}
			if (column != emp)
			{
				return chess_kick(chessboard_data, row, column, dir, !sur);
			}
			for (emp = column; emp < COLUMN; emp++)
			{
				if (!chessboard_data->data[row][emp].exist)
				{
					break;
				}
			}
			if (emp >= COLUMN)
			{
				if (column >= COLUMN) return false;
				if (chessboard_data->data[row][column].visible)
				{
					send_message(MESSAGE::KILL_VISIBLE);
				}
				else if (!kill_invisible())
				{
					return false;
				}
			}
			else
			{
				chessboard_data->data[row][emp] =
					chessboard_data->data[row][column];
			}
			chessboard_data->data[row][column] =
				chessboard_data->data[row][selected_column];
			if (!sur)
			{
				set_remove(row, column);
			}
			return true;
		default:
			return false;
		}
		return false;
	}

	// 将棋子往指定方向打，尤指炮
	bool Chessboard::chess_shoot(ChessboardData* chessboard_data,
		int row, int column, Direction::TYPE dir)
	{
		// 移动规则：
		// 1. 在不锁定目标棋子时，只能朝四个方向移动一格，且不能推动；
		// 2. 如果锁定了目标棋子，则会检查是否跳过了一个任意棋子，
		//    若符合此条件，则会先多格飞跃到对方的位置的前面，无论该点
		//    是否有棋子，都会向指定方向踢一格，和普通的踢棋规则一样；
		// 3. 移动限制：
		// (1) 必须跳过一个棋子，即棋子与目标之间恰好只有一个棋子，才能锁定目标；
		// (2) 可以跳过任意一个棋子，没有类型限制；
		// 4. 允许打掉对方的棋子、暗棋以及自己的棋子。
		int emp = 0, cnt = 0;
		switch (dir)
		{
		case Direction::UP:
			if (row == selected_row - 1)
			{
				if (chessboard_data->data[row][column].color ==
					Chess::COLOR::NONE)
				{
					chessboard_data->data[row][column] =
						chessboard_data->data[selected_row][column];
					return true;
				}
				return false;
			}
			for (emp = selected_row - 1; emp >= 0; emp--)
			{
				if (chessboard_data->data[emp][column].exist)
				{
					cnt++;
				}
				if (cnt == 2)
				{
					break;
				}
			}
			if (row != emp) return false;
			for (emp = row; emp >= 0; emp--)
			{
				if (!chessboard_data->data[emp][column].exist)
				{
					break;
				}
			}
			if (emp < 0)
			{
				if (row < 0) return false;
				if (chessboard_data->data[row][column].visible)
				{
					send_message(MESSAGE::KILL_VISIBLE);
				}
				else if (!kill_invisible())
				{
					return false;
				}
			}
			else
			{
				chessboard_data->data[emp][column] =
					chessboard_data->data[row][column];
			}
			chessboard_data->data[row][column] =
				chessboard_data->data[selected_row][column];
			return true;
		case Direction::DOWN:
			if (row == selected_row + 1)
			{
				if (chessboard_data->data[row][column].color ==
					Chess::COLOR::NONE)
				{
					chessboard_data->data[row][column] =
						chessboard_data->data[selected_row][column];
					return true;
				}
				return false;
			}
			for (emp = selected_row + 1; emp < ROW; emp++)
			{
				if (chessboard_data->data[emp][column].exist)
				{
					cnt++;
				}
				if (cnt == 2)
				{
					break;
				}
			}
			if (row != emp) return false;
			for (emp = row; emp < ROW; emp++)
			{
				if (!chessboard_data->data[emp][column].exist)
				{
					break;
				}
			}
			if (emp >= ROW)
			{
				if (row >= ROW) return false;
				if (chessboard_data->data[row][column].visible)
				{
					send_message(MESSAGE::KILL_VISIBLE);
				}
				else if (!kill_invisible())
				{
					return false;
				}
			}
			else
			{
				chessboard_data->data[emp][column] =
					chessboard_data->data[row][column];
			}
			chessboard_data->data[row][column] =
				chessboard_data->data[selected_row][column];
			return true;
		case Direction::LEFT:
			if (column == selected_column - 1)
			{
				if (chessboard_data->data[row][column].color ==
					Chess::COLOR::NONE)
				{
					chessboard_data->data[row][column] =
						chessboard_data->data[row][selected_column];
					return true;
				}
				return false;
			}
			for (emp = selected_column - 1; emp >= 0; emp--)
			{
				if (chessboard_data->data[row][emp].exist)
				{
					cnt++;
				}
				if (cnt == 2)
				{
					break;
				}
			}
			if (column != emp) return false;
			for (emp = column; emp >= 0; emp--)
			{
				if (!chessboard_data->data[row][emp].exist)
				{
					break;
				}
			}
			if (emp < 0)
			{
				if (column < 0) return false;
				if (chessboard_data->data[row][column].visible)
				{
					send_message(MESSAGE::KILL_VISIBLE);
				}
				else if (!kill_invisible())
				{
					return false;
				}
			}
			else
			{
				chessboard_data->data[row][emp] =
					chessboard_data->data[row][column];
			}
			chessboard_data->data[row][column] =
				chessboard_data->data[row][selected_column];
			return true;
		case Direction::RIGHT:
			if (column == selected_column + 1)
			{
				if (chessboard_data->data[row][column].color ==
					Chess::COLOR::NONE)
				{
					chessboard_data->data[row][column] =
						chessboard_data->data[row][selected_column];
					return true;
				}
				return false;
			}
			for (emp = selected_column + 1; emp < COLUMN; emp++)
			{
				if (chessboard_data->data[row][emp].exist)
				{
					cnt++;
				}
				if (cnt == 2)
				{
					break;
				}
			}
			if (column != emp) return false;
			for (emp = column; emp < COLUMN; emp++)
			{
				if (!chessboard_data->data[row][emp].exist)
				{
					break;
				}
			}
			if (emp >= COLUMN)
			{
				if (column >= COLUMN) return false;
				if (chessboard_data->data[row][column].visible)
				{
					send_message(MESSAGE::KILL_VISIBLE);
				}
				else if (!kill_invisible())
				{
					return false;
				}
			}
			else
			{
				chessboard_data->data[row][emp] =
					chessboard_data->data[row][column];
			}
			chessboard_data->data[row][column] =
				chessboard_data->data[row][selected_column];
			return true;
		default:
			return false;
		}
		return false;
	}

	// 让棋子和目标棋子同归于尽，尤指仕、士
	bool Chessboard::chess_die_together(ChessboardData* chessboard_data,
		int row, int column, Direction::TYPE dir)
	{
		// 移动规则：
		// 1. 移动到同一对角线上的相邻位置，若符合移动限制，
		//    则会将占用该位置的棋子吃掉；
		// 2. 移动限制：
		// (1) 必须在同一对角线上相邻，否则无效；
		// (2) 必须是对方的棋子，不能是自己的棋子或者暗棋；
		// (3) 一旦吃掉了对方的棋子，则会与对方的棋子同归于尽。 
		switch (dir)
		{
		case Direction::UP_RIGHT:
			if (selected_row - row != 1 || selected_column - column != -1)
			{
				return false;
			}
			break;
		case Direction::DOWN_RIGHT:
			if (selected_row - row != -1 || selected_column - column != -1)
			{
				return false;
			}
			break;
		case Direction::DOWN_LEFT:
			if (selected_row - row != -1 || selected_column - column != 1)
			{
				return false;
			}
			break;
		case Direction::UP_LEFT:
			if (selected_row - row != 1 || selected_column - column != 1)
			{
				return false;
			}
			break;
		default:
			return false; // 其他方向无效
		}
		if (!chessboard_data->data[row][column].visible ||
			chessboard_data->data[row][column].color ==
			chessboard_data->data[selected_row]
			[selected_column].color)
		{
			return false;
		}
		if (chessboard_data->data[row][column].color == 
			Chess::COLOR::NONE)
		{
			chessboard_data->data[row][column] =
				chessboard_data->data[selected_row][selected_column];
		}
		else
		{
			send_message(MESSAGE::KILL_VISIBLE);
			set_remove(row, column);
		}
		return true;
	}

	// 按照规则移动棋子
	bool Chessboard::move_chess_by_rule(int to_row, int to_column, 
		bool use_right)
	{
		if (!has_selected() || !in_range(to_column, to_row))
		{
			return false;
		}
		Chess chess = chesses.data[selected_row][selected_column];
		Direction::TYPE dir = Direction::relative(
			to_column, to_row, selected_column, selected_row);
		ChessboardData c_data = chesses;
		bool resolved = false; // 是否符合规则
		// 根据棋子类型，根据规则走棋
		switch (chesses.data[selected_row][selected_column].type)
		{
		case Chess::TYPE::PAWN:
			if (chess_push(&c_data, to_row, to_column, dir))
			{
				resolved = true;
			}
			break;
		case Chess::TYPE::KNIGHT:
			if (chess_kick(&c_data, to_row, to_column, dir))
			{
				resolved = true;
			}
			break;
		case Chess::TYPE::CANNON:
			if (chess_shoot(&c_data, to_row, to_column, dir))
			{
				resolved = true;
			}
			break;
		case Chess::TYPE::ROOK:
			if (chess_crush(&c_data, to_row, to_column, dir))
			{
				resolved = true;
			}
			break;
		case Chess::TYPE::BISHOP:
			if (chess_dash(&c_data, to_row, to_column, dir))
			{
				resolved = true;
			}
			break;
		case Chess::TYPE::ADVISOR:
			if (chess_die_together(&c_data, to_row, to_column, dir))
			{
				resolved = true;
			}
			break;
		case Chess::TYPE::KING:
			if (!use_right && 
				chess_push(&c_data, to_row, to_column, dir) || 
				use_right && 
				chess_kick(&c_data, to_row, to_column, dir))
			{
				resolved = true;
			}
			break;
		default:
			break;
		}
		// 在调用棋子移动函数时，移动可能被拒绝：返回false，因此需要判断
		// 如果符合规则，则移动棋子
		if (resolved)
		{
			// 将标记为即将移除的棋子设置为将要移除
			if (has_set_remove())
			{
				c_data.data[removed_row][removed_column].will_remove();
			}
			c_data.data[selected_row][selected_column].will_remove();
			// 如果历史记录中有相同的局面，则拒绝移动，并发送重复移动消息
			if (history_set.count(c_data))
			{
				send_message(MESSAGE::REOCCUR_PREVIOUS);
				return false;
			}
			// 如果没有获取到消息，则发送棋盘失去焦点消息
			if (_message == MESSAGE::NONE)
			{
				send_message(MESSAGE::BOARD_BLUR);
			}
			// 将标记为即将移除的棋子移除
			if (has_set_remove())
			{
				c_data.data[removed_row][removed_column].remove();
				supple_grid(removed_column, removed_row);
				clear_remove();
			}
			c_data.data[selected_row][selected_column].remove();
			// 更新棋盘数据
			chesses = c_data;
			supple_grid(selected_column, selected_row);
			// 将棋盘数据中的棋子重新绘制
			place_chesses();
			// 记录历史记录，并下一步
			step_forward(chesses);
			// 将棋盘失去焦点
			blur();
		}
		else
		{
			// 拒绝移动，如果没有发送超过暗棋限制的消息，就发送无效移动信息
			if (_message != MESSAGE::EXCEED_INVISIBLE_KILL)
			{
				send_message(MESSAGE::INVALID_MOVE);
			}
		}
		// 返回是否移动成功
		return resolved;
	}
}