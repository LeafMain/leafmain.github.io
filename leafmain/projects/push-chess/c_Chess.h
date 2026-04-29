#pragma once

// 棋子类声明
namespace pushchess
{
	// 棋子类
	class Chess
	{
	public: 
		static const LPCTSTR FONT; // 棋子字体
		static const int BORDER_WIDTH; // 边框线宽度
		static const int OUTLINE_WIDTH; // 棋子轮廓线宽度
		static const int OUTLINE_OFFSET_DIV; // 棋子轮廓偏移除数
		static const COLORREF FILL_COLOR; // 棋子填充颜色
		static const COLORREF BORDER_COLOR; // 棋子边框颜色
		static const COLORREF RED_COLOR; // 红色棋子颜色
		static const COLORREF BLACK_COLOR; // 黑色棋子颜色
		static const COLORREF SELECTED_COLOR; // 棋子选中颜色
	public:
		enum class COLOR : char // 棋子颜色
		{
			NONE, // 无颜色
			C_RED, // 红色 
			C_BLACK // 黑色 
		};
		enum class TYPE : char // 棋子类型
		{
			NONE, // 无棋子
			KING, // 帅、将
			ADVISOR, //仕、士
			BISHOP, // 相、象
			ROOK, // 车
			KNIGHT, // 马
			CANNON, // 炮
			PAWN // 兵、卒
		};
		// 棋子对数据结构
		struct Pair
		{
			COLOR color = COLOR::NONE;
			TYPE type = TYPE::NONE;
		};
		int x; // 棋子中心 x 坐标
		int y; // 棋子中心 y 坐标
		int radius; // 棋子半径
		bool visible; // 棋子是否可见
		bool exist; // 棋子是否存在
		TYPE type; // 棋子类型
		COLOR color; // 棋子颜色

		// 棋子构造函数
		// @param color = NONE 棋子颜色枚举
		// @param type = NONE 棋子类型枚举
		// @param x = 0 棋子中心 x 坐标
		// @param y = 0 棋子中心 y 坐标
		// @param visible = true 棋子是否可见
		// @param exist = true 棋子是否存在
		Chess(COLOR color = COLOR::NONE, TYPE type = TYPE::NONE, 
			int x = 0, int y = 0, int radius = 0, 
			bool visible = true, bool exist = true);

		// 设置一个棋子的数据
		// @param color 棋子颜色枚举
		// @param type 棋子类型枚举
		// @param x 棋子中心 x 坐标
		// @param y 棋子中心 y 坐标
		// @param radius 要绘制棋子的半径
		// @param visible 棋子是否可见
		// @param exist 棋子是否存在
		void set_data(COLOR color, TYPE type, int x, int y, 
			int radius, bool visible, bool exist);

		// 调整棋子半径，并调整棋子外观
		// @param radius 要调整的棋子半径
		void resize(int radius);

		// 绘制棋子，使用棋子对象坐标
		void draw() const;

		// 设置棋子即将移除，但不移除
		void will_remove();

		// 移除棋子，不修改对象状态
		void remove() const;

		// 移除棋子，并修改对象存在状态
		void remove();

		// 选中棋子
		// @param use_toggle = false 是否启用切换选中状态模式
		void select(bool use_toggle = false);

		// 取消选中棋子
		void unselect();

		// 打开棋子
		void uncover();

		// 棋子是否已经绘制
		// @return bool 是否已经绘制
		bool has_drawn() const;

		// 返回棋子选中状态
		// @return 棋子是否选中
		bool selected() const;

		// 判断一个点是否在棋子范围内
		// @param px 点的 x 坐标
		// @param py 点的 y 坐标
		// @return bool 是否在棋子范围内
		bool in_area(int px, int py) const;

		// 获取棋子文字
		// @return LPCTSTR 棋子文字
		LPCTSTR text() const;

		// 将一个棋子对象赋给原对象，但不修改坐标值
		// @param chess 棋子对象引用
		Chess& operator=(const Chess& chess);

		// 比较两个棋子是否有完全相同的属性
		// @param chess 棋子对象引用
		// @return bool 是否有完全相同的属性
		bool operator==(const Chess& chess) const;

		// 比较两个棋子是否有不同属性
		// @param chess 棋子对象引用
		// @return bool 是否有不同属性
		bool operator!=(const Chess& chess) const;

		// 比较两个棋子的逻辑大小
		// @param chess 棋子对象引用
		// @return bool 是否比 chess 小
		bool operator<(const Chess& chess) const;

		// 静态方法，获取棋子文字
		// @param color 棋子颜色
		// @param type 棋子类型名称
		// @return LPCTSTR 棋子文字
		static LPCTSTR text(COLOR color, TYPE type);

		// 创建一个棋子并返回
		// @param color 棋子颜色枚举
		// @param type 棋子类型枚举
		// @param x = 0 棋子中心 x 坐标
		// @param y = 0 棋子中心 y 坐标
		// @param radius 要绘制棋子的半径
		// @param visible = true 棋子是否可见
		// @param exist = true 棋子是否存在
		// @return Chess 一个新的 Chess 棋子对象实例
		static Chess create(COLOR color, TYPE type, 
			int x = 0, int y = 0, int radius = 0, 
			bool visible = true, bool exist = true);
	protected:
		int font_size; // 棋子字体宽度
		int outline_offset; // 棋子轮廓偏移量
	private:
		// 后面棋盘色，移除时用一个圆补充回去
		mutable COLORREF _board_color_ = TRANSPARENT;
		mutable bool _drawn_ = false; // 棋子是否已经绘制
		bool _selected = false; // 棋子是否选中
	};
}