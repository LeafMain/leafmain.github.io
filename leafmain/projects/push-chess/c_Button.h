#pragma once

// 对按钮类的声明
namespace pushchess
{
	// 按钮类
	class Button
	{
	public:
		static const COLORREF DISABLED_COLOR; // 按钮禁用颜色
		static const LPCTSTR FONT; // 按钮字体
	public:
		int x; // 按钮 x 坐标
		int y; // 按钮 y 坐标
		int width; // 按钮宽度
		int height; // 按钮高度
		int font_size; // 按钮字体大小
		int border_width; // 按钮边框宽度
		COLORREF text_color; // 按钮文本颜色
		COLORREF border_color; // 按钮边框颜色
		COLORREF background_color; // 按钮背景颜色
		int border_style; // 按钮边框线型

		// 按钮构造函数
		// @param text = _T("") 按钮文本
		// @param x = 0 按钮 x 坐标
		// @param y = 0 按钮 y 坐标
		// @param width = 0 按钮宽度
		// @param height = 0 按钮高度
		// @param font_size = 12 按钮字体大小
		// @param text_color = WHITE 按钮文本颜色
		// @param background_color = BLACK 按钮背景颜色
		// @param border_color = WHITE 按钮边框颜色
		// @param border_width = 1 按钮边框宽度
		// @param border_style = PS_SOLID 按钮边框线型
		explicit Button(LPCTSTR text = _T(""), 
			int x = 0, int y = 0, int width = 0, int height = 0, 
			int font_size = 12, COLORREF text_color = WHITE,
			COLORREF background_color = BLACK, COLORREF border_color = WHITE,
			int border_width = 1,int border_style = PS_SOLID);

		// 按钮析构函数
		~Button();

		// 关闭拷贝
		Button(const Button& button) = delete;
		Button& operator=(const Button& button) = delete;

		// 绘制按钮
		void draw() const;

		// 移除按钮
		void remove() const;

		// 设置按钮数据
		// @param text 按钮文本
		// @param x 按钮 x 坐标
		// @param y 按钮 y 坐标
		// @param width 按钮宽度
		// @param height 按钮高度
		// @param font_size 按钮字体大小
		// @param text_color 按钮文本颜色
		// @param background_color 按钮背景颜色
		// @param border_color 按钮边框颜色
		// @param border_width 按钮边框宽度
		// @param border_style 按钮边框线型
		void set_data(LPCTSTR text, int x, int y, int width, int height,
			int font_size, COLORREF text_color, COLORREF background_color,
			COLORREF border_color, int border_width, int border_style);

		// 获取按钮文本
		// @return LPCTSTR 按钮文本
		LPCTSTR text() const;

		// 设置按钮文本
		// @param text 按钮文本
		void set_text(LPCTSTR text);

		// 显示按钮
		void show();

		// 隐藏按钮
		void hide();

		// 禁用按钮
		void disable();

		// 启用按钮
		void enable();

		// 判断按钮是否隐藏
		// @return bool 按钮是否隐藏
		bool hidden() const;

		// 判断按钮是否被禁用
		// @return bool 按钮是否被禁用
		bool disabled() const;

		// 判断坐标是否在按钮区域内
		// @param x 坐标 x
		// @param y 坐标 y
		// @return bool 坐标是否在按钮区域内
		bool in_area(int x, int y) const;

		// 判断按钮是否被点击
		// @param msg 鼠标消息
		// @param right 是否为右键
		bool clicked(ExMessage& msg, bool right = false) const; 
	private:
		LPTSTR _text = nullptr; // 按钮文本
		bool _visible = true; // 按钮是否可见
		bool _disabled = false; // 按钮是否被禁用
		// 按钮绘制前的背景颜色
		mutable COLORREF _background_before_draw_ = TRANSPARENT;
	};
}
