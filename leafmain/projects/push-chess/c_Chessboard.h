#pragma once

// 棋盘类声明
namespace pushchess
{
	// 棋盘数据类
	class ChessboardData
	{
	public:
		typedef std::vector<Chess::Pair> LIST;
		static constexpr int ROW = 4; // 棋盘行数
		static constexpr int COLUMN = 8; // 棋盘列数
		const int CHESS_RADIUS; // 棋子半径
		const int GRID_SIZE; // 棋盘网格大小
		Chess data[ROW][COLUMN]; // 棋盘数据
	public:
		// 棋盘数据默认构造函数
		ChessboardData();

		// 棋盘数据构造函数
		// @param CHESS_RADIUS 棋子半径
		// @param GRID_SIZE 棋盘格子大小
		ChessboardData(const int CHESS_RADIUS, const int GRID_SIZE);

		// 设置数据，带边界检查
		// @param row 要设置数据的行数
		// @param column 要设置数据的列数
		// @param color 棋子颜色枚举
		// @param type 棋子类型枚举
		// @param visible 棋子是否可见
		// @param exist 棋子是否存在
		void set_data(int row, int column, Chess::COLOR color, 
			Chess::TYPE type, bool visible, bool exist);

		// 将所有棋盘数据打乱
		// @param visible = true 打乱后的棋子是否可见
		void shuffle(bool visible = true);

		// 检查行号和列号是否在合理范围内
		// @param row 要设置数据的行数
		// @param column 要设置数据的列数
		// @return bool 是否没有越界
		bool in_range(int row, int column) const;

		// 将棋盘数据赋给原来棋盘数据
		// @param chessboard_data 要比较的棋盘数据引用
		ChessboardData& operator=(const ChessboardData& chessboard_data);

		// 比较两个棋盘数据是否完全相等
		// @param chessboard_data 要比较的棋盘数据引用
		// @return bool 是否完全相等
		bool operator==(const ChessboardData& chessboard_data) const;

		// 比较两个棋盘数据是否不完全相等
		// @param chessboard_data 要比较的棋盘数据引用
		// @return bool 是否不完全相等
		bool operator!=(const ChessboardData& chessboard_data) const;

		// 比较两个棋盘数据的逻辑大小
		// @param chessboard_data 要比较的棋盘数据引用
		// @return bool 是否比 chessboard_data 更小
		bool operator<(const ChessboardData& chessboard_data) const;

		// 静态方法，获取棋盘棋子有序列表
		// @return ChessboardData::LIST 有序棋盘数据 vector
		static LIST ordered_list();
	};

	// 棋盘类
	class Chessboard
	{
	public:
		static constexpr int ROW = ChessboardData::ROW; // 棋盘行数
		static constexpr int COLUMN = ChessboardData::COLUMN; // 列数
		static constexpr int GRID_COUNT = COLUMN * ROW; // 棋盘格数
		static const int CHESS_RADIUS; // 棋子半径
		static const int LEFT; // 棋盘与窗口左边界的距离
		static const int TOP; // 棋盘与窗口上边界的距离
		static const int DOT_OFFSET; // 棋盘点偏移量
		static const int DOT_LENGTH; // 棋盘格点长度
		static const int DOT_WIDTH; // 棋盘点宽度
		static const int GRID_SIZE; // 棋盘网格大小
		static const int GRID_MARGIN_LEFT; // 棋盘格左边距
		static const int GRID_MARGIN_TOP; // 棋盘格上边距
		static const int GRID_LINE_TYPE; // 棋盘格线类型
		static const int GRID_LINE_WIDTH; // 棋盘格线宽度
		static const int WIDTH; // 棋盘宽度
		static const int HEIGHT; // 棋盘高度
		static const int OUTLINE_WIDTH; // 棋盘外框宽度
		static const int OUTLINE_OFFSET; // 棋盘外框偏移量
		static const int OUTLINE_TYPE; // 棋盘外框类型
		static const COLORREF GRID_COLOR; // 棋盘格线颜色
		static const COLORREF BACKGROUND_COLOR; // 棋盘背景颜色
		static const COLORREF DOT_COLOR; // 棋盘点颜色
		static const COLORREF OUTLINE_COLOR; // 棋盘外框颜色
	public:
		enum class MESSAGE
		{
			NONE, // 无消息
			SELECT_KING, // 选中帅、将
			SELECT_ADVISOR, //选中仕、士
			SELECT_BISHOP, // 选中相、象
			SELECT_ROOK, // 选中车
			SELECT_KNIGHT, // 选中马
			SELECT_CANNON, // 选中炮
			SELECT_PAWN, // 选中兵、卒
			BOARD_BLUR, // 棋盘取消选棋
			BOARD_REGRET, // 悔棋
			BOARD_RESET, // 重置棋盘
			CHESS_UNCOVER, // 揭开棋子
			KILL_INVISIBLE, // 吃了一个暗棋
			KILL_VISIBLE, // 吃了一个明棋
			EXCEED_INVISIBLE_KILL, // 吃了过多暗棋
			INVALID_MOVE, // 棋子无效移动
			NOT_IN_TURN, // 没有轮到玩家走棋
			REOCCUR_PREVIOUS,// 当前局面在先前已存在 
			PLAYER_LOSE // 玩家失败
		};

		// 棋盘构造函数
		Chessboard();

		// 绘制棋盘
		void draw() const;

		// 撤销一步，悔棋
		void regret();

		// 重置棋盘，回到初始状态
		void reset();

		// 监听事件，根据事件进行响应
		// @param msg 消息事件引用 
		void listen_event_message(ExMessage& msg);

		// 侦查棋盘消息
		// @param msg 棋盘消息事件指针
		void peek_board_message(MESSAGE* msg);

		// 取消棋盘的选中状态
		void blur();

		// 判断棋盘是否有焦点，即是否有选中棋子
		bool has_focus() const;

		// 操作棋子，两个棋子相同则揭开
		// @param from_row 起始行号
		// @param from_column 起始列号
		// @param to_row 目标行号
		// @param to_column 目标列号
		// @param dir 移动方向
		// @return bool 是否成功移动
		bool operate_chess(int from_row, int from_column, int to_row, 
			int to_column, bool use_right = false);

		// 获取棋盘当前步数
		// @return int 当前步数
		int get_step() const;

		// 获取指定玩家的棋子数量
		// @return int 指定玩家的棋子数量
		int get_chess_count() const;

		// 获取当前吃掉的暗棋数量
		// @return int 当前吃掉的暗棋数量
		int get_invisible_kill_count() const;

		// 获取当前玩家
		// @return Player 当前玩家引用
		const Player& get_player() const;

		// 返回棋盘数据的常引用
		// @return const ChessboardData& 棋盘数据 
		const ChessboardData& data() const;
	private:
		// 最大允许吃暗棋的数量
		static constexpr int MAX_INVISIBLE_KILL_COUNT = 16;
		typedef std::stack<ChessboardData> STACK; // 棋盘历史栈类型
		typedef std::stack<bool> KILL_STACK; // 吞吃暗棋类型
		typedef std::set<ChessboardData> SET; // 棋盘历史集合类型
		ChessboardData chesses; // 棋子数据
		STACK history_stack; // 棋盘历史栈
		KILL_STACK invisible_kill_stack; // 吞吃暗棋栈
		SET history_set; //  棋盘历史集合
		int selected_row = -1; // 选中棋子的行号
		int selected_column = -1; // 
		int removed_row = -1; // 将要被移除棋子的行号
		int removed_column = -1; // 将要被移除棋子的列号
		int _step = 0; // 步数
		int _invisible_kill_count = 0; // 已吃暗棋的数量
		MESSAGE _message = MESSAGE::NONE; // 棋盘消息
		Player _player = Player::NONE; // 玩家数据
		bool _invisible_pushed = false; // 是否吃了暗棋
		bool _game_ended = false; // 是否游戏结束

		// 检查相对坐标是否在合理范围内
		// @param rel_x : 棋盘相对横坐标
		// @param rel_y : 棋盘相对纵坐标
		// @return bool 是否在合理范围内
		bool in_range(int rel_x, int rel_y) const;

		// 计算棋盘绝对 x 坐标
		// @param rel_x : 棋盘相对横坐标
		// @return int 棋盘绝对 x 坐标m
		int abs_x(int rel_x) const;

		// 计算棋盘绝对 y 坐标
		// @param rel_y : 棋盘相对纵坐标
		// @return int 棋盘绝对 y 坐标
		int abs_y(int rel_y) const;

		// 计算棋盘相对 x 坐标
		// @param abs_x : 棋盘绝对纵坐标
		// @return int 棋盘相对 x 坐标
		int rel_x(int abs_x) const;

		// 计算棋盘相对 y 坐标
		// @param abs_y : 棋盘绝对纵坐标
		// @return int 棋盘相对 y 坐标
		int rel_y(int abs_y) const;

		// 绘制棋盘背景
		void draw_grid() const;

		// 绘制棋盘格线
		void draw_background() const;

		// 绘制棋盘点
		void draw_dot() const;

		// 补充棋盘点
		// @param x : 棋盘格相对横坐标
		// @param y : 棋盘格相对纵坐标
		void supple_dot(int x, int y) const;

		// 补充棋盘 45 度斜线
		// @param x : 棋盘格相对横坐标
		// @param y : 棋盘格相对纵坐标
		// @param slope_1 : 斜线斜率是否为 1 ，默认 true
		void supple_slope(int x, int y, bool slope_1 = true) const;

		// 补充棋盘格
		// @param x : 补充的棋盘格相对横坐标
		// @param y : 补充的棋盘格相对纵坐标
		void supple_grid(int x, int y) const;

		// 摆放棋子
		// @param flush = false 是否清理并刷新棋盘
		void place_chesses(bool flush = false) const;

		// 揭开棋子
		// @param row 要揭开棋子的行号
		// @param column 要揭开棋子的列号
		void uncover_chess(int row, int column);

		// 选中指定行列的棋子
		// @param row 棋子行号
		// @param column 棋子列号
		void select_chess(int row, int column);

		// 设置指定行列的棋子将被移除
		// @param row 棋子行号
		// @param column 棋子列号
		void set_remove(int row, int column);

		// 清除将被移除的棋子
		void clear_remove();

		// 棋盘的下一步
		// @param chessboard_data 棋子数据引用
		void step_forward(ChessboardData& chessboard_data);

		// 发送棋盘消息
		// @param message 棋盘消息枚举
		void send_message(const MESSAGE&& message);

		// 判断是否有棋子被选中
		// @return bool 是否有选中棋子
		bool has_selected() const;

		// 判断是否有棋子将被移除
		// @return bool 是否有棋子
		bool has_set_remove() const;

		// 发送吃掉一个暗棋信息，并检查是否成功
		// @return bool 是否成功
		bool kill_invisible();

		// 检查指定位置的棋子是否属于当前玩家
		// @param row 棋子行号
		// @param column 棋子列号
		// @return bool 是否属于当前玩家
		bool is_player_chess(int row, int column) const;

		// 将棋子往指定方向推，可指兵、卒、帅、将
		// @param chessboard_data 棋子数据指针
		// @param row 目标棋子的行号
		// @param column 目标棋子的列号
		// @param dir 要推的方向
		// @return bool 是否成功
		bool chess_push(ChessboardData* chessboard_data, 
			int row, int column, Direction::TYPE dir);

		// 将棋子往指定方向踢，可指马、帅、将
		// @param chessboard_data 棋子数据指针
		// @param row 目标棋子的行号
		// @param column 目标棋子的列号
		// @param dir 要踢的方向
		// @param die = false 是否踢完后死亡
		// @return bool 是否成功
		bool chess_kick(ChessboardData* chessboard_data, int row, 
			int column, Direction::TYPE dir, bool die = false);

		// 将棋子往指定方向撞，尤指车
		// @param chessboard_data 棋子数据指针
		// @param row 目标棋子的行号
		// @param column 目标棋子的列号
		// @param dir 要撞的方向
		// @return bool 是否成功
		bool chess_crush(ChessboardData* chessboard_data, 
			int row, int column, Direction::TYPE dir);

		// 将棋子往指定方向冲，尤指相、象
		// @param chessboard_data 棋子数据指针
		// @param row 目标棋子的行号
		// @param column 目标棋子的列号
		// @param dir 要冲的方向
		// @return bool 是否成功
		bool chess_dash(ChessboardData* chessboard_data,
			int row, int column, Direction::TYPE dir);

		// 将棋子往指定方向打，尤指炮
		// @param chessboard_data 棋子数据指针
		// @param row 目标棋子的行号
		// @param column 目标棋子的列号
		// @param dir 要打的方向
		// @return bool 是否成功
		bool chess_shoot(ChessboardData* chessboard_data, 
			int row, int column, Direction::TYPE dir);

		// 让棋子和目标棋子同归于尽，尤指仕、士
		// @param chessboard_data 棋子数据指针
		// @param row 目标棋子的行号
		// @param column 目标棋子的列号
		// @param dir 目标棋子的方向
		// @return bool 是否成功
		bool chess_die_together(ChessboardData* chessboard_data, 
			int row, int column, Direction::TYPE dir);

		// 按照规则移动棋子
		// @param to_row 目标位置的行号
		// @param to_column 目标位置的列号
		// @param use_right 是否使用右键点击
		// @return bool 是否成功
		bool move_chess_by_rule(int to_row, int to_column,
			bool use_right = false);
	};
}