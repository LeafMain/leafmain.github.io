#include <graphics.h>

// 常量、变量、函数声明
namespace box0
{
	// 枚举及数据类型定义
	typedef int LEVEL; // 关卡类型
	enum DIRECTION { UP, DOWN, LEFT, RIGHT }; // 方向枚举
	enum ELEMENT { EMPTY, WALL, PLAYER, BOX, GOAL }; // 元素枚举
	// 常量定义
	const int WINDOW_WIDTH = 1000; // 窗口宽度
	const int WINDOW_HEIGHT = 800; // 窗口高度
	const int MAP_X = 0; // 地图左上角 x 坐标
	const int MAP_Y = 0; // 地图左上角 y 坐标
	const int MAP_WIDTH = 800; // 地图宽度
	const int MAP_HEIGHT = 800; // 地图高度
	const int GRID_SIZE = 40; // 网格大小
	const int GRID_RADIUS = GRID_SIZE / 2; // 网格半径
	const int GRID_ROW = MAP_HEIGHT / GRID_SIZE; // 网格行数
 	const int GRID_COL = MAP_WIDTH / GRID_SIZE; // 网格列数
	const int SELECT_X = MAP_X + MAP_WIDTH; // 选择框左上角 x 坐标
	const int SELECT_Y = MAP_Y; // 选择框左上角 y 坐标
	const int SELECT_WIDTH = WINDOW_WIDTH - SELECT_X; // 选择框宽度
	const int SELECT_HEIGHT = MAP_HEIGHT + SELECT_Y; // 选择框高度
	const int SELECT_BORDER_THICKNESS = 1; // 选择框边框线条粗细
	const int TITLE_X = SELECT_X + 28; // 标题 x 坐标
	const int TITLE_Y = SELECT_Y + 10; // 标题 y 坐标
	const int TITLE_SIZE = 40; // 标题字体大小
	const int NUMBER_BOX_X = SELECT_X + 10; // 网格左上角 x 坐标
	const int NUMBER_BOX_Y = SELECT_Y + TITLE_SIZE + 40; // 网格左上角 y 坐标
	const int NUMBER_COL = 4; // 数字列数
	const int NUMBER_ROW = 16; // 数字行数
	const int NUMBER_SIZE = 28; // 数字字体大小
	const int NUMBER_OFFSET = 7; // 数字偏移量
	const int NUMBER_COUNT = NUMBER_COL * NUMBER_ROW; // 数字总数
	const int NUMBER_ROW_GAP = 40; // 数字行间距
	const int NUMBER_COL_GAP = 45; // 数字列间距
	const int NUMBER_BOX_H = NUMBER_ROW_GAP * NUMBER_ROW; // 数字框高度
	const int BUTTON_X = NUMBER_BOX_X + 40; // 按钮左上角 x 坐标
	const int BUTTON_Y = NUMBER_BOX_Y + NUMBER_BOX_H + 10; // 按钮左上角 y 坐标
	const int BUTTON_GAP = 35; // 按钮间距
	const int BUTTON_FONT_SIZE = 28; // 按钮字体大小
	const int BUTTON_OFFSET = BUTTON_FONT_SIZE * 2 + BUTTON_FONT_SIZE / 2; // 按钮偏移量
	const int ELEMENT_OFFSET = 2; // 元素偏移量
	const int ELEMENT_RADIUS = GRID_RADIUS - ELEMENT_OFFSET; // 元素半径
	const int PLAYER_RADIUS = ELEMENT_RADIUS * 7 / 8; // 玩家半径
	const int PLAYER_OUTLINE_WIDTH = 2; // 玩家轮廓线条宽度
	const int WALL_RADIUS = GRID_RADIUS; // 墙壁半径
	const int BOX_RADIUS = ELEMENT_RADIUS; // 箱子半径
	const int GOAL_RADIUS = ELEMENT_RADIUS / 2; // 目标半径
	const COLORREF BACKGROUND_COLOR = BLACK; // 背景颜色
	const COLORREF BOX_COLOR = BROWN; // 箱子颜色
	const COLORREF CURRENT_LEVEL_COLOR = CYAN; // 当前关卡按钮颜色
	const COLORREF CURRENT_NUMBER_COLOR = YELLOW; // 当前关卡数字颜色
	const COLORREF GOAL_COLOR = RED; // 目标颜色
	const COLORREF GRID_COLOR = CYAN; // 网格颜色
	const COLORREF NUMBER_COLOR = LIGHTGRAY; // 选择框数字颜色
	const COLORREF PASSED_NUMBER_COLOR = CYAN; // 通过关卡数字颜色
	const COLORREF PLAYER_COLOR = CYAN; // 玩家颜色
	const COLORREF PLAYER_OUTLINE_COLOR = CYAN; // 玩家轮廓颜色
	const COLORREF RESTART_COLOR = BROWN; // 重新开始按钮颜色
	const COLORREF SELECT_BORDER_COLOR = CYAN; // 选择框边框颜色
	const COLORREF SELECT_BOX_COLOR = LIGHTGRAY; // 选择框网格颜色
	const COLORREF TITLE_COLOR = CYAN; // 选择框标题颜色
	const COLORREF WALL_COLOR = DARKGRAY; // 墙壁颜色
	const TCHAR* CURRENT_LEVEL_TEXT = _T("关卡："); // 当前关卡文本
	const TCHAR* REPLAY_TEXT = _T("重新开始"); // 重新开始按钮文本
	const TCHAR* SELECT_FONT = _T("Arial"); // 选择框字体
	const TCHAR* TITLE_TEXT = _T("选择关卡"); // 选择框标题文本
	// 变量定义
	int level_current = 1; // 当前关卡
	int level_limit = 0; // 已制作的关卡总数
	int player_x = 0; // 玩家 x 坐标
	int player_y = 0; // 玩家 y 坐标
	int select_page = 0; // 选择框当前页
	bool game_running = false; // 游戏是否正在运行
	bool level_playing = false; // 关卡是否正在进行
	bool level_passed[NUMBER_COUNT]; // 关卡是否通过
	bool use_mouse_track = true; // 是否使用鼠标跟踪
	bool use_shoutcut = true; // 是否使用快捷键
	ELEMENT element_data[NUMBER_COUNT][GRID_ROW][GRID_COL]; // 元素数据
	inline int abs_x(int x); // 将相对 x 坐标转换为绝对 x 坐标
	inline int abs_y(int y); // 将相对 y 坐标转换为绝对 y 坐标
	void game_exit(); // 退出游戏 
	void game_run(); // 运行游戏
	void initialize_data(); // 初始化元素数据
	void level_enter(LEVEL level);
	void level_next(); // 下一关
	void level_replay(); // 重新开始
	void listen_global_key(); // 全局键盘事件监听
	void listen_level_key(); // 关卡键盘事件监听
	void listen_level_mouse(MOUSEMSG* mouse); // 关卡鼠标事件监听
	void listen_select_mouse(MOUSEMSG* mouse); // 选框鼠标事件监听
	void listen_shortcut(); // 快捷键事件监听
	void map_draw(); // 绘制地图
	void map_clear(); // 清除地图
	void mark_current(COLORREF color); // 标记当前关卡
	void place_box(int x, int y); // 放置箱子
	void place_element(int x, int y, ELEMENT element); // 放置元素
	void place_goal(int x, int y); // 放置目标
	void place_player(int x, int y); // 放置玩家
	void place_wall(int x, int y); // 放置墙壁
	void player_move(DIRECTION direction); // 玩家移动
	void remove_element(int x, int y); // 移走元素
	void select_draw(); // 绘制选择框
	void select_draw_footer(); // 绘制当前关卡以及重玩按钮
	void select_draw_number(); // 绘制选择框数字
	void select_draw_title(); // 绘制选择框标题
	void set_data(LEVEL level, int x, int y, ELEMENT element); // 设置元素数据
	void show_goals(); // 重新显示目标
	void update_current(); // 更新当前关卡
	bool covered_all_goals(); // 判断箱子是否盖住了所有目标
	bool has_won(); // 判断玩家是否获胜
	bool key_down(int v_key); // 判断按键是否保持按下状态
	bool key_pressed(int v_key); // 判断按键是否按下
	bool is_wall(int x, int y); // 是否是墙
	bool is_box(int x, int y); // 是否是箱子
	bool is_goal(int x, int y); // 是否是目标
	bool is_empty(int x, int y); // 是否是空地
}

// 函数实现
namespace box0
{
	// 将相对坐标转换为绝对坐标
	inline int abs_x(int x)
	{
		// 使用 inline 定义函数，可以避免函数调用的开销
		// 相对坐标 * 网格大小 + 地图左上角坐标 = 绝对坐标
		return x * GRID_SIZE + MAP_X;
	}

	// 将相对坐标转换为绝对坐标
	inline int abs_y(int y)
	{
		// 使用 inline 定义函数，可以避免函数调用的开销
		// 相对坐标 * 网格大小 + 地图左上角坐标 = 绝对坐标
		return y * GRID_SIZE + MAP_Y;
	}

	// 判断按键是否保持按下状态
	bool key_down(int v_key)
	{
		// 按键保持按下状态为 0x8000
		return GetKeyState(v_key) & 0x8000;
	}

	// 判断按键是否按下
	bool key_pressed(int v_key)
	{
		// 按键按下，并忽略重复按键
		return GetAsyncKeyState(v_key) & 0x0001;
	}

	// 绘制地图
	void map_draw()
	{
		// 使用双重循环绘制网格，先绘制行，再绘制列
		// 循环是内层先执行，所以先枚举列再枚举行
		for (int col = 0; col < GRID_COL; col++) // 列
		{
			for (int row = 0; row < GRID_ROW; row++) // 行
			{
				// 在网格左上角绝对坐标处绘制网格顶点
				putpixel(abs_x(col), abs_y(row), GRID_COLOR);
			}
		}
	}

	// 清除地图
	void map_clear()
	{
		// 直接将地图矩形区域清除为背景色即可
		clearrectangle(MAP_X, MAP_Y, MAP_X + MAP_WIDTH, MAP_Y + MAP_HEIGHT);
	}

	// 绘制标题
	void select_draw_title()
	{
		// 设置线条样式
		setlinestyle(BS_SOLID, SELECT_BORDER_THICKNESS);
		// 设置线条颜色
		setlinecolor(SELECT_BORDER_COLOR);
		// 绘制选择框左边框，即与地图的分界线，加一可防止遮挡
		line(SELECT_X + 1, SELECT_Y, SELECT_X + 1, 
			SELECT_Y + SELECT_HEIGHT);
		// 绘制选择框顶部标题区域下边框，加一可防止遮挡
		line(SELECT_X + 1, TITLE_Y + TITLE_SIZE, 
			SELECT_X + SELECT_WIDTH, TITLE_Y + TITLE_SIZE);
		// 设置标题字体颜色
		settextcolor(TITLE_COLOR);
		// 设置标题字体
		settextstyle(TITLE_SIZE, 0, SELECT_FONT);
		// 绘制标题文字
		outtextxy(TITLE_X, TITLE_Y, TITLE_TEXT);
	}

	// 绘制选择框数字
	void select_draw_number()
	{
		// 初始化循环关卡索引为 1
		int current = 1;
		// 设置数字字体颜色
		settextcolor(NUMBER_COLOR);
		// 设置数字字体样式
		settextstyle(NUMBER_SIZE, 0, SELECT_FONT);
		// 设置数字边框颜色
		setlinecolor(SELECT_BOX_COLOR);
		// 设置数字边框样式
		setlinestyle(BS_SOLID, SELECT_BORDER_THICKNESS);
		// 使用双重循环绘制数字框边框
		// 由于先绘制行再绘制列，所以先枚举行再枚举列
		for (int row = 0; row < NUMBER_ROW; row++)
		{
			for (int col = 0; col < NUMBER_COL; col++)
			{
				// 定义宽字符串数组，用于存储数字
				TCHAR number[10];
				// 将数字转换为宽字符串
				_itot_s(current, number, 10);
				// 只有当不超过关卡总数时才显示数字
				if (current <= level_limit)
				{
					// 计算绝对坐标，并绘制数字
					outtextxy(
						NUMBER_BOX_X + NUMBER_OFFSET + col * NUMBER_COL_GAP,
						NUMBER_BOX_Y + NUMBER_OFFSET + row * NUMBER_ROW_GAP,
						number);
				}
				// 绘制数字框边框
				rectangle(
					NUMBER_BOX_X + col * NUMBER_COL_GAP,
					NUMBER_BOX_Y + row * NUMBER_ROW_GAP,
					NUMBER_BOX_X + (col + 1) * NUMBER_COL_GAP,
					NUMBER_BOX_Y + (row + 1) * NUMBER_ROW_GAP
				);
				// 更新循环到的关卡索引
				current++;
			}
		}
	}

	// 绘制当前关卡以及重玩按钮
	void select_draw_footer()
	{
		// 设置字体样式
		settextstyle(BUTTON_FONT_SIZE, 0, SELECT_FONT);
		// 设置当前关卡按钮字体颜色
		settextcolor(CURRENT_LEVEL_COLOR);
		// 绘制当前关卡按钮
		outtextxy(BUTTON_X, BUTTON_Y, CURRENT_LEVEL_TEXT);
		// 设置重玩按钮字体颜色
		settextcolor(RESTART_COLOR);
		// 绘制重玩按钮
		outtextxy(BUTTON_X, BUTTON_Y + BUTTON_GAP, REPLAY_TEXT);
	}

	// 绘制选择框
	void select_draw()
	{
		// 绘制选择框标题
		select_draw_title();
		// 绘制选择框数字
		select_draw_number();
		// 绘制选择框底部按钮
		select_draw_footer();
	}

	// 更新当前关卡
	void update_current()
	{
		// 定义宽字符串数组，用于存储数字
		TCHAR current[10];
		// 将当前关卡转换为宽字符串
		_itot_s(level_current, current, 10);
		// 清除原来显示的当前关卡的数字，防止遮挡
		clearrectangle(
			BUTTON_X + BUTTON_OFFSET, BUTTON_Y,
			BUTTON_X + BUTTON_OFFSET + BUTTON_FONT_SIZE * 2, 
			BUTTON_Y + BUTTON_FONT_SIZE
		);
		setbkmode(OPAQUE); // 设置背景非透明
		// 设置当前关卡数字样式
		settextstyle(BUTTON_FONT_SIZE, 0, CURRENT_LEVEL_TEXT);
		// 设置当前关卡数字颜色
		settextcolor(CURRENT_NUMBER_COLOR);
		// 绘制当前关卡数字
		outtextxy(BUTTON_X + BUTTON_OFFSET, BUTTON_Y, current);
	}

	// 标记当前关卡
	void mark_current(COLORREF color)
	{
		// 初始化循环关卡索引为 1
		int current = 1;
		// 循环搜索整个数字框，找到当前关卡的数字并标记颜色
		for (int row = 0; row < NUMBER_ROW; row++)
		{
			for (int col = 0; col < NUMBER_COL; col++)
			{
				// 当找到当前关卡的数字时，标记颜色并返回
				if (current == level_current)
				{
					// 定义宽字符串数组，用于存储数字
					TCHAR number[10];
					// 将数字转换为宽字符串
					_itot_s(current, number, 10);
					// 设置数字字体颜色
					settextcolor(color);
					// 设置数字字体样式
					settextstyle(NUMBER_SIZE, 0, SELECT_FONT);
					setbkmode(OPAQUE); // 设置背景非透明 
					// 在数字框内绘制数字
					outtextxy(
						NUMBER_BOX_X + NUMBER_OFFSET + col * NUMBER_COL_GAP,
						NUMBER_BOX_Y + NUMBER_OFFSET + row * NUMBER_ROW_GAP,
						number
					);
					// 找到了当前关卡，退出函数，不再继续搜索
					return; 
				}
				// 更新循环到的关卡索引
				current++;
			}
		}
	}

	// 设置元素数据
	void set_data(LEVEL level, int x, int y, ELEMENT element)
	{
		// 本函数是设置元素数据数组的安全函数，带有参数检查
		int row = y, col = x; // 转换为数组索引
		// 如果关卡不存在，则不进行操作
		if (level < 1 || level > NUMBER_COUNT) return;
		// 如果行数超出范围，则不进行操作
		if (row < 0 || row >= GRID_ROW) return;
		// 如果列数超出范围，则不进行操作
		if (col < 0 || col >= GRID_COL) return;
		// 如果元素不存在或不正确，则不进行操作
		if (element < ELEMENT::EMPTY || element > ELEMENT::GOAL) return;
		// 检查通过，设置元素数据
		element_data[level - 1][row][col] = element;
	}

	// 放置墙壁
	void place_wall(int x, int y)
	{
		// 本函数用于放置墙壁
		int cx = abs_x(x) + GRID_RADIUS; // 计算绝对 x 坐标
		int cy = abs_y(y) + GRID_RADIUS; // 计算绝对 y 坐标
		// 设置墙壁填充颜色
		setfillcolor(WALL_COLOR);
		// 绘制墙壁
		solidrectangle(cx - WALL_RADIUS, cy - WALL_RADIUS,
			cx + WALL_RADIUS, cy + WALL_RADIUS);
	}

	// 放置玩家
	void place_player(int x, int y)
	{
		// 本函数用于放置玩家
		player_x = x; // 记录玩家 x 坐标
		player_y = y; // 记录玩家 y 坐标
		int cx = abs_x(x) + GRID_RADIUS; // 计算绝对 x 坐标
		int cy = abs_y(y) + GRID_RADIUS; // 计算绝对 y 坐标
		// 设置玩家填充颜色
		setfillcolor(PLAYER_COLOR);
		// 设置玩家轮廓颜色
		setlinecolor(PLAYER_OUTLINE_COLOR);
		// 设置玩家轮廓宽度
		setlinestyle(PS_SOLID, PLAYER_OUTLINE_WIDTH);
		// 绘制玩家
		fillcircle(cx, cy, PLAYER_RADIUS);
	}

	// 放置箱子
	void place_box(int x, int y)
	{
		// 本函数用于放置箱子
		int cx = abs_x(x) + GRID_RADIUS; // 计算绝对 x 坐标
		int cy = abs_y(y) + GRID_RADIUS; // 计算绝对 y 坐标
		// 设置箱子填充颜色
		setfillcolor(BOX_COLOR);
		// 绘制箱子
		solidroundrect(cx - BOX_RADIUS, cy - BOX_RADIUS,
			cx + BOX_RADIUS, cy + BOX_RADIUS, 
			ELEMENT_OFFSET * 3, ELEMENT_OFFSET * 3);
	}

	// 放置目标
	void place_goal(int x, int y)
	{
		// 本函数用于放置目标
		int cx = abs_x(x) + GRID_RADIUS; // 计算绝对 x 坐标
		int cy = abs_y(y) + GRID_RADIUS; // 计算绝对 y 坐标
		// 设置目标填充颜色
		setfillcolor(GOAL_COLOR);
		// 绘制目标
		solidcircle(cx, cy, GOAL_RADIUS);
	}

	// 放置元素
	void place_element(int x, int y, ELEMENT element)
	{
		// 本函数根据传入的元素类型调用相应放置元素的函数
		switch (element)
		{
			// 放置墙壁
			case WALL: place_wall(x, y); break;
			// 放置玩家
			case PLAYER: place_player(x, y); break;
			// 放置箱子
			case BOX: place_box(x, y); break;
			// 放置目标
			case GOAL: place_goal(x, y); break;
			// 其他元素不进行操作
			default: break;
		}
	}

	// 移走元素
	void remove_element(int x, int y)
	{
		// 本函数用于移走元素
		int cx = abs_x(x) + GRID_RADIUS; // 计算绝对 x 坐标
		int cy = abs_y(y) + GRID_RADIUS; // 计算绝对 y 坐标
		// 如果没有元素，则不进行操作，因为没有必要移除空地
		if (getpixel(cx, cy) == BACKGROUND_COLOR)
		{
			return; // 没有元素
		}
		// 另外，不能移走墙壁，防止绘图错误
		if (getpixel(cx, cy) == WALL_COLOR)
		{
			return; // 不能移走墙壁
		}
		// 移走元素
		clearrectangle(
			cx - ELEMENT_RADIUS, cy - ELEMENT_RADIUS,
			cx + ELEMENT_RADIUS, cy + ELEMENT_RADIUS);
	}

	// 关卡进入
	void level_enter(LEVEL level)
	{
		// 检查关卡是否存在
		if (level < 1 || level > NUMBER_COUNT || level > level_limit)
		{
			return; // 关卡不存在
		}
		// 如果关卡已通过，则标记原来的关卡为已通过
		if (level_passed[level_current - 1])
		{
			// 标记原来关卡为已通过
			mark_current(PASSED_NUMBER_COLOR); 
		}
		// 否则标记原来的关卡为未通过
		else
		{
			// 标记原来关卡为未通过
			mark_current(NUMBER_COLOR);
		}
		// 更新当前关卡
		level_current = level;
		// 将关卡进行状态设置为正在进行
		level_playing = true;
		// 标记当前关卡为正在进行的关卡
		mark_current(CURRENT_NUMBER_COLOR);
		update_current(); // 更新并显示当前关卡
		map_clear(); // 清除地图
		map_draw(); // 绘制地图
		// 遍历地图网格，逐个放置元素
		for (int row = 0; row < GRID_ROW; row++)
		{
			for (int col = 0; col < GRID_COL; col++)
			{
				// 获取元素数据
				ELEMENT element = element_data[level - 1][row][col];
				// 如果元素不为空，则放置元素，因为空元素没必要放置
				if (element != ELEMENT::EMPTY)
				{
					// 放置数组中的元素
					place_element(col, row, element);
				}
			}
		}
	}

	// 关卡重新开始
	void level_replay()
	{
		// 重新开始关卡，直接进入当前关卡即可，因为会重新绘制
		level_enter(level_current);
	}

	// 下一关
	void level_next()
	{
		// 如果已经是最后一关，则回到第一关
		if (level_current >= NUMBER_COUNT || level_current >= level_limit)
		{
			// 回到第一关
			level_enter(1);
		}
		else
		{
			// 进入下一关
			level_enter(level_current + 1);
		}
	}

	// 重新显示目标
	void show_goals()
	{
		// 本函数用于重新显示目标，由于当玩家或箱子移动到目标位置时，
		// 目标会因元素覆盖而自动消失，所以当玩家或箱子离开目标位置时，
		// 目标需要重新显示，以防止目标意外消失
		LEVEL level = level_current; // 定义变量存储当前关卡
		// 循环遍历整个数组，找到所有目标位置
		for (int i = 0; i < GRID_ROW; i++)
		{
			for (int j = 0; j < GRID_COL; j++)
			{
				// 如果当前元素是目标，则可以考虑显示目标
				if (element_data[level - 1][i][j] == GOAL)
				{
					int cx = abs_x(j) + GRID_RADIUS; // 计算绝对 x 坐标
					int cy = abs_y(i) + GRID_RADIUS; // 计算绝对 y 坐标
					// 如果没有元素，则显示目标
					if (getpixel(cx, cy) == BACKGROUND_COLOR)
					{
						// 显示目标
						place_goal(j, i);
					}
				}
			}
		}
	}

	// 判断箱子是否盖住了所有目标
	bool covered_all_goals()
	{
		// 本函数用于判断箱子是否盖住了所有目标
		bool found = false; // 是否找到目标
		LEVEL level = level_current; // 定义变量存储当前关卡
		// 循环遍历整个数组，找到所有目标位置
		for (int i = 0; i < GRID_ROW; i++)
		{
			for (int j = 0; j < GRID_COL; j++)
			{
				int cx = abs_x(j) + GRID_RADIUS; // 计算绝对 x 坐标
				int cy = abs_y(i) + GRID_RADIUS; // 计算绝对 y 坐标
				// 如果当前元素是目标，则标记找到了目标，并判断是否有箱子
				if (element_data[level - 1][i][j] == GOAL)
				{
					found = true; // 至少找到了一个目标
					// 判断是否有箱子，只要一个目标点不是箱子，则游戏未获胜
					if (getpixel(cx, cy) != BOX_COLOR)
					{
						// 发现了一个目标点没有箱子，游戏未获胜
						return false; // 未获胜，无需继续搜索
					}
				}
			}
		}
		// 地图中根本就没有目标，游戏不可能获胜
		if (!found) return false; 
		return true; // 已经获胜
	}

	// 判断玩家是否获胜
	bool has_won()
	{
		// 本函数用于判断玩家是否获胜，
		// 只有当箱子盖住了所有目标，玩家才算获胜
		if (covered_all_goals()) // 箱子盖住了所有目标
		{
			// 标记当前关卡为已通过，注意数组索引从 0 开始，所以减 1
			level_passed[level_current - 1] = true;
			return true; // 已经获胜
		}
		return false; // 未获胜
	}

	// 是否是墙
	bool is_wall(int x, int y)
	{
		// 越界了，也算碰到墙壁
		if (x < 0 || x >= GRID_COL || y < 0 || y >= GRID_ROW)
		{
			return true;
		}
		// 碰到墙壁时，获取到墙壁颜色
		int cx = abs_x(x) + GRID_RADIUS; // 计算绝对 x 坐标
		int cy = abs_y(y) + GRID_RADIUS; // 计算绝对 y 坐标
		// 如果碰到墙壁，返回 true
		if (getpixel(cx, cy) == WALL_COLOR)
		{
			return true;
		}
		// 都没碰到，返回 false
		return false;
	}

	// 是否是空地
	bool is_empty(int x, int y)
	{
		// 越界了，不算是空地
		if (x < 0 || x >= GRID_COL || y < 0 || y >= GRID_ROW)
		{
			return false;
		}
		// 碰到空地时，获取到空地颜色
		int cx = abs_x(x) + GRID_RADIUS; // 计算绝对 x 坐标
		int cy = abs_y(y) + GRID_RADIUS; // 计算绝对 y 坐标
		// 如果碰到空地，返回 true
		if (getpixel(cx, cy) == BACKGROUND_COLOR)
		{
			return true;
		}
		// 都没碰到，返回 false
		return false;
	}

	// 是否是箱子
	bool is_box(int x, int y)
	{
		// 越界了，不算碰到箱子
		if (x < 0 || x >= GRID_COL || y < 0 || y >= GRID_ROW)
		{
			return false;
		}
		// 碰到箱子时，获取到箱子颜色
		int cx = abs_x(x) + GRID_RADIUS; // 计算绝对 x 坐标
		int cy = abs_y(y) + GRID_RADIUS; // 计算绝对 y 坐标
		// 如果碰到箱子，返回 true
		if (getpixel(cx, cy) == BOX_COLOR)
		{
			return true;
		}
		// 都没碰到，返回 false
		return false;
	}

	// 是否是目标
	bool is_goal(int x, int y)
	{
		// 越界了，不算碰到目标
		if (x < 0 || x >= GRID_COL || y < 0 || y >= GRID_ROW)
		{
			return false;
		}
		// 碰到目标时，获取到目标颜色
		int cx = abs_x(x) + GRID_RADIUS; // 计算绝对 x 坐标
		int cy = abs_y(y) + GRID_RADIUS; // 计算绝对 y 坐标
		// 如果碰到目标，返回 true
		if (getpixel(cx, cy) == GOAL_COLOR)
		{
			return true;
		}
		// 都没碰到，返回 false
		return false;
	}
	
	// 玩家移动
	void player_move(DIRECTION direction)
	{
		// 本函数用于玩家移动，首先定义四个变量，
		// 用于存储移动后的 x 坐标、y 坐标、
		// 移动间隔 x 坐标、移动间隔 y 坐标，
		// 并根据方向参数设置相应的变量；
		// 然后判断是否碰到墙壁、箱子、目标，
		// 并根据情况移动玩家、箱子；
		// 最后，如果已经获胜，则进入下一关。
		int next_x = player_x;// 移动后的 x 坐标
		int next_y = player_y; // 移动后的 y 坐标
		int gap_x = player_x;	// 移动间隔 x 坐标
		int gap_y = player_y; // 移动间隔 y 坐标
		// 根据方向参数设置相应的变量
		switch (direction)
		{
		case UP: // 向上
			next_y -= 1;
			gap_y -= 2;
			break;
		case DOWN: // 向下
			next_y += 1;
			gap_y += 2;
			break;
		case LEFT: // 向左
			next_x -= 1;
			gap_x -= 2;
			break;
		case RIGHT: // 向右
			next_x += 1;
			gap_x += 2;
			break;
		default: // 其他情况，不进行操作
			break;
		}
		// 如果碰到墙，无法移动
		if (is_wall(next_x, next_y))
		{
			return; // 碰到墙壁，无法移动
		}
		// 如果是箱子，分为以下情况：
		if (is_box(next_x, next_y))
		{
			// 如果箱子后面是墙壁，则无法移动
			if (is_wall(gap_x, gap_y))
			{
				return; // 箱子后面是墙壁，无法移动
			}
			// 如果箱子后面也是箱子，也无法移动
			if (is_box(gap_x, gap_y))
			{
				return; // 箱子后面也是箱子，无法移动
			}
			// 可以移动，先移走箱子
			remove_element(next_x, next_y);
			// 再把箱子放在新的位置
			place_box(gap_x, gap_y);
		}
		// 在原来的位置移走玩家
		remove_element(player_x, player_y);
		// 在新的位置放置玩家
		place_player(next_x, next_y);
		player_x = next_x; // 更新玩家 x 坐标
		player_y = next_y; // 更新玩家 y 坐标
		// 玩家可能碰到目标，当玩家离开目标位置时，
		// 或者将箱子推出目标位置时，需要重新显示目标
		show_goals(); // 重新显示目标
		// 如果已经获胜，则进入下一关
		if (has_won())
		{
			// 调用下一关函数
			level_next();
		}
	}

	// 关卡鼠标事件监听
	void listen_level_mouse(MOUSEMSG* mouse)
	{
		// 如果关卡未开始，则不进行操作
		if (!level_playing) return;
		// 如果不使用鼠标跟踪，则不进行操作
		if (!use_mouse_track) return;
		// 如果按下了 Ctrl 键，阻止关卡鼠标事件监听
		if (key_down(VK_MENU)) return;
		// 当鼠标在地图区域内，则进行以下操作：
		if (mouse->x > MAP_X && mouse->y > MAP_Y &&
			mouse->x < MAP_X + MAP_WIDTH && 
			mouse->y < MAP_Y + MAP_HEIGHT)
		{
			// 按下鼠标左键，向左移动
			if (mouse->uMsg == WM_LBUTTONDOWN)
			{
				player_move(LEFT); // 玩家向左移动
				mouse->uMsg = WM_NULL; // 清除消息
			}
			// 按下鼠标右键，向右移动
			else if (mouse->uMsg == WM_RBUTTONDOWN)
			{
				player_move(RIGHT); // 玩家向右移动
				mouse->uMsg = WM_NULL; // 清除消息
			}
			// 鼠标向上滚轮，向上移动
			else if (mouse->uMsg == WM_MOUSEWHEEL && mouse->wheel > 0)
			{
				player_move(UP); // 玩家向上移动
				mouse->uMsg = WM_NULL; // 清除消息
			}
			// 鼠标向下滚轮，向下移动
			else if (mouse->uMsg == WM_MOUSEWHEEL && mouse->wheel < 0)
			{
				player_move(DOWN); // 玩家向下移动
				mouse->uMsg = WM_NULL; // 清除消息
			}
			// 按下鼠标中键，重玩关卡
			else if (mouse->uMsg == WM_MBUTTONDOWN)
			{
				level_replay(); // 重玩关卡
				mouse->uMsg = WM_NULL; // 清除消息
			}
		}
	}

	// 选框鼠标事件监听
	void listen_select_mouse(MOUSEMSG* mouse)
	{
		// 本函数用于选框鼠标事件监听。
		// 由于事件监听描述比较复杂，具体实现见以下代码注释
		int current = 1; // 当前关卡
		// 循环遍历每个关卡的选框
		for (int row = 0; row < NUMBER_ROW; row++)
		{
			for (int col = 0; col < NUMBER_COL; col++)
			{
				// 计算选框的四个顶点坐标
				int x1 = NUMBER_BOX_X + col * NUMBER_COL_GAP; 
				int y1 = NUMBER_BOX_Y + row * NUMBER_ROW_GAP; 
				int x2 = NUMBER_BOX_X + (col + 1) * NUMBER_COL_GAP - 1; 
				int y2 = NUMBER_BOX_Y + (row + 1) * NUMBER_ROW_GAP - 1;
				// 判断鼠标是否在选框内且按下了鼠标左键
				// 先判断是否按下了鼠标左键，可以减少不必要的判断
				if (mouse->uMsg == WM_LBUTTONDOWN &&
					mouse->x >= x1 && mouse->x < x2 &&
					mouse->y >= y1 && mouse->y < y2)
				{
					// 选择了某个关卡，进入该关卡
					level_enter(current);
					// 处理完毕，清除消息
					mouse->uMsg = WM_NULL; 
					return; // 选择了某个关卡
				}
				// 没有选择某个关卡，继续循环
				current++;
			}
		}
		// 点击重玩按钮，重玩关卡
		if (mouse->uMsg == WM_LBUTTONDOWN &&
			mouse->x >= BUTTON_X && 
			mouse->x <= BUTTON_X + BUTTON_FONT_SIZE * 4 &&
			mouse->y >= BUTTON_Y + BUTTON_GAP && 
			mouse->y <= BUTTON_Y + BUTTON_GAP + BUTTON_FONT_SIZE)
		{
			// 重玩关卡
			level_replay();
			// 处理完毕，清除消息
			mouse->uMsg = WM_NULL;
			return; // 重玩了
		}
		// 如果按下了 Alt 键，则进行以下操作：
		if (key_down(VK_MENU))
		{
			// 如果同时向上滚动了鼠标滚轮，则移动到上一行或列的关卡，
			// 当然如果关卡不存在，则不会进入任何关卡
			if (mouse->uMsg == WM_MOUSEWHEEL && mouse->wheel > 0)
			{
				// 同时按下了 Shift 键，向上移动
				if (key_down(VK_SHIFT))
				{
					// 向上移动到上一行的关卡
					level_enter(level_current - NUMBER_COL);
				}
				// 否则向左移动
				else
				{
					// 向上移动到上一列的关卡
					level_enter(level_current - 1);
				}	
				mouse->uMsg = WM_NULL; // 处理完毕，清除消息
			}
			// 如果同时向下滚动了鼠标滚轮，则移动到下一行或列的关卡
			else if (mouse->uMsg == WM_MOUSEWHEEL && mouse->wheel < 0)
			{
				// 同时按下了 Shift 键，向下移动
				if (key_down(VK_SHIFT))
				{
					// 向下移动到下一行的关卡
					level_enter(level_current + NUMBER_COL);
				}
				// 否则向右移动
				else
				{
					// 向下移动到下一列的关卡
					level_enter(level_current + 1);
				}
				mouse->uMsg = WM_NULL; // 处理完毕，清除消息
			}
		}
	}

	// 关卡键盘事件监听
	void listen_level_key()
	{
		// 如果关卡未开始，则不进行操作
		if (!level_playing) return;
		// 如果按下了 Ctrl 键，阻止关卡键盘事件监听
		if (key_down(VK_MENU)) return;
		// 如果按下了向上键，向上移动
		if (key_pressed(VK_UP))
		{
			// 玩家向上移动
			player_move(UP);
		}
		// 如果按下了向下键，向下移动
		else if (key_pressed(VK_DOWN))
		{
			// 玩家向下移动
			player_move(DOWN);
		}
		// 如果按下了向左键，向左移动
		else if (key_pressed(VK_LEFT))
		{
			// 玩家向左移动
			player_move(LEFT);
		}
		// 如果按下了向右键，向右移动
		else if (key_pressed(VK_RIGHT))
		{
			// 玩家向右移动
			player_move(RIGHT);
		}
		// 如果按下了回车键，重玩关卡
		else if (key_pressed(VK_BACK))
		{
			// 重玩关卡
			level_replay();
		}
	}

	// 全局键盘事件监听
	void listen_global_key()
	{
		// 如果按下了 Esc 键，退出游戏
		if (key_pressed(VK_ESCAPE))
		{
			game_exit(); // 退出游戏
		}
		// 如果允许使用快捷键，则监听快捷键
		if (use_shoutcut)
		{
			listen_shortcut(); // 监听快捷键
		}
	}

	// 快捷键事件监听
	void listen_shortcut()
	{
		// 快捷键对应关卡为：key(F1-F9) * 10 + key(0-9)
		const int keys[10] =
		{
			VK_F10, VK_F1, VK_F2,
			VK_F3, VK_F4, VK_F5,
			VK_F6, VK_F7, VK_F8, VK_F9
		};
		// 计算将要进入的关卡
		LEVEL level = 0;
		// 快捷键对应关卡为：key(F1-F9) * 10 + key(0-9)
		for (int i = 0; i < 10; i++)
		{
			// 按下数字键，开始计算关卡
			if (key_pressed(i + '0'))
			{
				level += i;
				// 如果按下了 F1-F9 键中的一个
				for (int j = 0; j < 10; j++)
				{
					// 按下了其中一个键，将当前关卡加上 10 * Fn
					if (key_down(keys[j]))
					{
						level += 10 * j;
					}
				}
			}
		}
		// 按下快捷键，如果关卡存在，则进入该关卡
		if (level > 0 && level <= level_limit)
		{
			level_enter(level); // 进入关卡
			return; // 进入了关卡，不再监听快捷键
		}
		// 按下 Ctrl+方向键，移动到指定关卡
		if (key_down(VK_MENU))
		{
			// 如果按下了向上键，则移动到上一行的关卡
			if (key_pressed(VK_UP))
			{
				// 进入上一行的关卡
				level_enter(level_current - NUMBER_COL);
			}
			// 如果按下了向下键，则移动到下一行的关卡
			else if (key_pressed(VK_DOWN))
			{
				// 进入下一行的关卡
				level_enter(level_current + NUMBER_COL);
			}
			// 如果按下了向左键，则移动到上一列的关卡
			else if (key_pressed(VK_LEFT))
			{
				// 进入上一列的关卡
				level_enter(level_current - 1);
			}
			// 如果按下了向右键，则移动到下一列的关卡
			else if (key_pressed(VK_RIGHT))
			{
				// 进入下一列的关卡
				level_enter(level_current + 1);
			}
		}
	}

	// 退出游戏
	void game_exit()
	{
		game_running = false; // 游戏退出标志
		closegraph(); // 关闭图形界面
		exit(0); // 退出程序
	}

	// 运行游戏
	void game_run()
	{
		// 本函数用于运行游戏，是游戏主函数。
		initgraph(WINDOW_WIDTH + 1, WINDOW_HEIGHT + 1); // 初始化界面
		game_running = true; // 游戏运行标志
		setbkcolor(BACKGROUND_COLOR); // 设置背景颜色
		cleardevice(); // 清除屏幕
		initialize_data(); // 初始化元素数据
		map_draw(); // 绘制地图
		select_draw(); // 绘制选择框
		level_enter(1); // 进入第一关
		MOUSEMSG mouse; // 鼠标消息
		PeekMouseMsg(&mouse); // 读取鼠标消息
		// 循环监听事件
		while (game_running)
		{
			listen_global_key(); // 监听全局键盘事件
			listen_level_key(); // 监听关卡键盘事件
			listen_select_mouse(&mouse); // 监听全局鼠标事件
			listen_level_mouse(&mouse); // 监听关卡鼠标事件
			PeekMouseMsg(&mouse); // 读取鼠标消息
		}
	}

	// 初始化元素数据
	void initialize_data()
	{
		// 本函数用于初始化元素数据，
		// 包括地图数据、元素数据、目标数据、玩家数据等。
		// 地图数据：地图大小为 16 * 16，
		// 元素数据：元素类型有墙壁、箱子、目标、玩家四种，
		const int MX = GRID_COL - 1; // 地图最大列
		const int MY = GRID_ROW - 1; // 地图最大行
		level_limit = 0; // 总关卡数清零
		// 初始化元素数据，使用循环，方便管理
		for (int level = 1; level <= NUMBER_COUNT; level++)
		{
			// 第一关
			if (level == 1)
			{
				level_limit++; // 增加总关卡数
				// 画顶部墙壁
				for (int y = 0; y <= 5; y++)
				{
					for (int x = 0; x <= MX; x++)
					{
						set_data(level, x, y, WALL);
					}
				}
				// 画底部墙壁
				for (int y = MY - 5; y <= MY; y++)
				{
					for (int x = 0; x <= MX; x++)
					{
						set_data(level, x, y, WALL);
					}
				}
				// 画左侧墙壁，除去顶部和底部
				for (int x = 0; x <= 4; x++)
				{
					for (int y = 6; y <= MY - 6; y++)
					{
						set_data(level, x, y, WALL);
					}
				}
				// 画右侧墙壁，除去顶部和底部
				for (int x = MX - 4; x <= MX; x++)
				{
					for (int y = 6; y <= MY - 6; y++)
					{
						set_data(level, x, y, WALL);
					}
				}
				// 画箱子
				set_data(level, 7, 8, BOX);
				set_data(level, 12, 8, BOX);
				// 画目标
				set_data(level, 6, 12, GOAL);
				set_data(level, 13, 12, GOAL);
				// 画玩家
				set_data(level, 5, 6, PLAYER);
			}
			// 第二关
			else if (level == 2)
			{
				level_limit++; // 增加总关卡数
				// 画顶部墙壁
				for (int y = 0; y <= 4; y++)
				{
					for (int x = 0; x <= MX; x++)
					{
						set_data(level, x, y, WALL);
					}
				}
				// 画底部墙壁
				for (int y = MY - 4; y <= MY; y++)
				{
					for (int x = 0; x <= MX; x++)
					{
						set_data(level, x, y, WALL);
					}
				}
				// 画左侧墙壁
				for (int x = 0; x <= 4; x++)
				{
					for (int y = 6; y <= MY - 6; y++)
					{
						set_data(level, x, y, WALL);
					}
				}
				// 画右侧墙壁
				for (int x = MX - 4; x <= MX; x++)
				{
					for (int y = 6; y <= MY - 6; y++)
					{
						set_data(level, x, y, WALL);
					}
				}
				// 画正中间四个墙壁
				for (int y = 9; y <= MY - 9; y++)
				{
					for (int x = 9; x <= MX - 9; x++)
					{
						set_data(level, x, y, WALL);
					}
				}
				// 画外侧四个目标
				for (int y = 5; y <= MY - 5; y += MY - 10)
				{
					for (int x = 0; x <= MX; x += MX)
					{
						set_data(level, x, y, GOAL);
					}
				}
				// 画内侧四个目标
				for (int y = 5; y <= MY - 5; y += MY - 10)
				{
					for (int x = 4; x <= MX - 4; x += MX - 8)
					{
						set_data(level, x, y, GOAL);
					}
				}
				// 画外侧四个箱子
				for (int y = 5; y <= MY - 5; y += MY - 10)
				{
					for (int x = 2; x <= MX - 2; x += MX - 4)
					{
						set_data(level, x, y, BOX);
					}
				}
				// 画中间四个箱子
				set_data(level, 8, 10, BOX);
				set_data(level, 9, 8, BOX);
				set_data(level, 11, 9, BOX);
				set_data(level, 10, 11, BOX);
				// 画玩家
				set_data(level, 3, 5, PLAYER);
			}
			// 第三关
			else if (level == 3)
			{
				level_limit++; // 增加总关卡数
				// 画顶部墙壁
				for (int y = 0; y <= 5; y++)
				{
					for (int x = 0; x <= MX; x++)
					{
						if (x >= 8 && x <= MX - 8 && y >= 3)
						{
							continue;
						}
						set_data(level, x, y, WALL);
					}
				}
				// 画底部墙壁
				for (int y = MY - 5; y <= MY; y++)
				{
					for (int x = 0; x <= MX; x++)
					{
						if (x >= 8 && x <= MX - 8 && y <= MY - 3)
						{
							continue;
						}
						set_data(level, x, y, WALL);
					}
				}
				// 画左侧墙壁，除去顶部和底部
				for (int x = 0; x <= 5; x++)
				{
					for (int y = 5; y <= MY - 5; y++)
					{
						if (y >= 8 && y <= MY - 8 && x >= 3)
						{
							continue;
						}
						set_data(level, x, y, WALL);
					}
				}
				// 画右侧墙壁，除去顶部和底部
				for (int x = MX - 5; x <= MX; x++)
				{
					for (int y = 5; y <= MY - 5; y++)
					{
						if (y >= 8 && y <= MY - 8 && x <= MX - 3)
						{
							continue;
						}
						set_data(level, x, y, WALL);
					}
				}
				// 画上、下部的墙壁
				for (int y = 6; y <= MY - 6; y += MY - 12)
				{
					for (int x = 9; x <= MX - 9; x += MX - 18)
					{
						set_data(level, x, y, WALL);
					}
				}
				// 画左、右部的墙壁
				for (int y = 9; y <= MY - 9; y += MY - 18)
				{
					for (int x = 6; x <= MX - 6; x += MX - 12)
					{
						set_data(level, x, y, WALL);
					}
				}
				// 画上、下部的箱子
				for (int y = 4; y <= MY - 4; y += MY - 8)
				{
					for (int x = 9; x <= MX - 9; x += MX - 18)
					{
						set_data(level, x, y, BOX);
					}
				}
				// 画左、右部的箱子
				for (int y = 9; y <= MY - 9; y += MY - 18)
				{
					for (int x = 4; x <= MX - 4; x += MX - 8)
					{
						set_data(level, x, y, BOX);
					}
				}
				// 画内部的箱子 (1)
				for (int y = 7; y <= MY - 7; y += MY - 14)
				{
					for (int x = 7; x <= MX - 7; x += MX - 14)
					{
						set_data(level, x, y, BOX);
					}
				}
				// 画内部的箱子 (2)
				for (int y = 8; y <= MY - 8; y += MY - 16)
				{
					for (int x = 7; x <= MX - 7; x += MX - 14)
					{
						set_data(level, x, y, BOX);
					}
				}
				// 画内部的箱子 (3)
				for (int y = 7; y <= MY - 7; y += MY - 14)
				{
					for (int x = 8; x <= MX - 8; x += MX - 16)
					{
						set_data(level, x, y, BOX);
					}
				}
				// 画中间 12 个目标，除去中间区域
				for (int y = 8; y <= MY - 8; y++)
				{
					for (int x = 8; x <= MX - 8; x++)
					{
						if (x >= 9 && x <= MX - 9 && y >= 9 && y <= MY - 9)
						{
							continue;
						}
						set_data(level, x, y, GOAL);
					}
				}
				// 画上下 4 个目标
				for (int y = 7; y <= MY - 7; y += MY - 14)
				{
					for (int x = 9; x <= MX - 9; x += MX - 18)
					{
						set_data(level, x, y, GOAL);
					}
				}
				// 画左右 4 个目标
				for (int y = 9; y <= MY - 9; y += MY - 18)
				{
					for (int x = 7; x <= MX - 7; x += MX - 14)
					{
						set_data(level, x, y, GOAL);
					}
				}
				set_data(level, 10, 10, PLAYER);
			}
		}
	}
}

// 程序入口
int main()
{
	box0::game_run(); // 运行游戏  
	return 0; // 程序退出
}