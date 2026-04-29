#include <graphics.h>
#include "c_Chess.h"

// 棋子类实例化
namespace pushchess
{
	// 棋子构造函数
	Chess::Chess(COLOR color, TYPE type, int x, int y, 
		int radius, bool visible, bool exist) :
		color(color), type(type), x(x), y(y), radius(radius),
		visible(visible), exist(exist), font_size(radius * 3 / 2), 
		outline_offset(OUTLINE_OFFSET_DIV == 0 ? 0 : 
			radius / OUTLINE_OFFSET_DIV) {}

	// 设置一个棋子的数据
	void Chess::set_data(COLOR color, TYPE type, int x, int y,
		int radius, bool visible, bool exist)
	{
		this->color = color;
		this->type = type;
		this->x = x;
		this->y = y;
		this->visible = visible;
		this->exist = exist;
		_drawn_ = false;
	}

	// 调整棋子半径，并调整棋子外观
	void Chess::resize(int radius)
	{
		this->radius = radius;
		this->font_size = radius * 3 / 2;
		// 这里检查是否为 0，防止除数为 0
		this->outline_offset = OUTLINE_OFFSET_DIV == 0 ? 0 :
			radius / OUTLINE_OFFSET_DIV;
	}

	// 绘制棋子，使用棋子对象坐标
	void Chess::draw() const
	{
		// 将 mutable 变量设为身后背景色，方便调用移除函数
		if (_board_color_ == TRANSPARENT)
		{
			// 防止获取到斜线处
			_board_color_ = getpixel(x, y - 2);
		}
		_drawn_ = true;
		// 如果棋子不存在则不绘制
		if (!exist)
		{
			return;
		}
		// 首先绘制棋子底座边框与填充
		setlinecolor(BORDER_COLOR);
		setlinestyle(PS_SOLID, BORDER_WIDTH);
		// 如果选中，则设置选中样式
		if (_selected)
		{
			setfillcolor(SELECTED_COLOR);
		}
		else
		{
			setfillcolor(FILL_COLOR);
		}
		fillcircle(x, y, radius);
		// 如果棋子不可见则不绘制内容
		if (!visible)
		{
			return;
		}
		// 其次绘制棋子边框
		setlinestyle(PS_SOLID, OUTLINE_WIDTH);
		settextstyle(font_size, 0, FONT);
		setbkmode(TRANSPARENT);
		if (color == COLOR::C_RED)
		{
			setlinecolor(RED_COLOR);
			settextcolor(RED_COLOR);
		}
		else if (color == COLOR::C_BLACK)
		{
			setlinecolor(BLACK_COLOR);
			settextcolor(BLACK_COLOR);
		}
		circle(x, y, radius - outline_offset);
		// 最后绘制棋子文字
		RECT rt = { x - radius, y - radius, x + radius, y + radius };
		drawtext(text(), &rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	// 设置棋子即将移除，但不移除
	void Chess::will_remove()
	{
		// 将棋子标记为不存在，并将颜色和类型设置为无效
		exist = false;
		color = COLOR::NONE;
		type = TYPE::NONE;
		// 注意，调用此函数后需要调用 remove() 函数才能真正移除
	}

	// 移除棋子，不修改对象状态
	void Chess::remove() const
	{
		_drawn_ = false; // 需要修改 _drawn_ 状态
		// 用棋盘背景颜色覆盖即可
		setfillcolor(_board_color_);
		solidcircle(x, y, radius + BORDER_WIDTH);
	}

	// 移除棋子，并修改对象存在状态
	void Chess::remove()
	{
		// 先标记棋子即将移除
		will_remove();
		// 然后调用常成员函数移除
		static_cast<const Chess&>(*this).remove();
	}

	// 选中棋子
	void Chess::select(bool use_toggle)
	{
		// 如果使用切换，则切换
		if (use_toggle && _selected)
		{
			unselect(); // 取消选中
			draw(); // 重绘
		}
		else
		{
			// 已选择，无需选择
			if (_selected) return;
			_selected = true;
			draw(); // 重绘
		}
	}

	// 取消选中棋子
	void Chess::unselect()
	{
		_selected = false;
		draw(); // 重绘
	}

	// 打开棋子
	void Chess::uncover()
	{
		// 已经打开则不打开
		if (visible) return;
		visible = true;
		draw();
	}

	// 棋子是否已经绘制
	bool Chess::has_drawn() const
	{
		return _drawn_;
	}

	// 返回棋子选中状态
	bool Chess::selected() const
	{
		return _selected;
	}

	// 判断一个点是否在棋子范围内
	bool Chess::in_area(int px, int py) const
	{
		// 计算距离中心的距离
		return (px - x) * (px - x) + (py - y) * (py - y) <
			radius * radius;
	}

	// 获取棋子文字
	LPCTSTR Chess::text() const
	{
		// 返回静态方法
		return Chess::text(color, type);
	}

	// 将一个棋子对象赋给原对象，但不修改坐标值
	Chess& Chess::operator=(const Chess& chess)
	{
		// 和拷贝构造函数不同，这里不全盘复制，只复制必要的属性
		// 如果属性有改变，则允许重复绘制
		if (color != chess.color ||
			type != chess.type ||
			visible != chess.visible ||
			exist != chess.exist)
			_drawn_ = false;
		// 赋值
		color = chess.color;
		type = chess.type;
		visible = chess.visible;
		exist = chess.exist;
		_selected = false;
		return *this;
	}

	// 比较两个棋子是否有完全相同的属性
	bool Chess::operator==(const Chess& chess) const
	{
		// 仅当两个棋子的颜色、类型、可见性、存在状态全部相等时，
		// 才认为这两个棋子有完全相同的属性
		return color == chess.color && type == chess.type &&
			visible == chess.visible && exist == chess.exist;
	}

	// 比较两个棋子是否有不同属性
	bool Chess::operator!=(const Chess& chess) const
	{
		return !(this->operator==(chess));
	}

	// 比较两个棋子的逻辑大小
	bool Chess::operator<(const Chess& chess) const
	{
		// 将两个棋子的存在状态、可见性、颜色、类型逐个比较
		if (exist != chess.exist) return exist < chess.exist;
		if (visible != chess.visible) return visible < chess.visible;
		if (color != chess.color) return color < chess.color;
		return type < chess.type;
	}

	// 静态方法，获取棋子文字
	LPCTSTR Chess::text(COLOR color, TYPE type)
	{
		switch (type)
		{
		case TYPE::KING:
			if (color == COLOR::C_RED) return _T("帥");
			else return _T("將");
		case TYPE::ADVISOR:
			if (color == COLOR::C_RED) return _T("仕");
			else return _T("士");
		case TYPE::BISHOP:
			if (color == COLOR::C_RED) return _T("相");
			else return _T("象");
		case TYPE::ROOK:
			return _T("車");
		case TYPE::KNIGHT:
			return _T("馬");
		case TYPE::CANNON:
			return _T("炮");
		case TYPE::PAWN:
			if (color == COLOR::C_RED) return _T("兵");
			else return _T("卒");
		default:
			return _T("");
		}
		// 如果都不是，则返回空字符串
		return _T("");
	}

	// 创建一个棋子并返回
	Chess Chess::create(COLOR color, TYPE type, int x, int y,
		int radius, bool visible, bool exist)
	{
		// 与构造函数不同，这里要求传入所有参数
		// 调用构造函数创建并返回
		return Chess(color, type, x, y, visible, exist);
	}
}