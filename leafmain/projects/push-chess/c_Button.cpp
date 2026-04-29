#include <graphics.h>
#include "c_Button.h"

// 对按钮类的实现
namespace pushchess
{
	// 按钮构造函数
	Button::Button(LPCTSTR text, int x, int y, int width, int height,
		int font_size, COLORREF text_color, COLORREF background_color,
		COLORREF border_color, int border_width, int border_style) :
		_text(_tcsdup(text)), x(x), y(y), width(width), height(height),
		font_size(font_size), border_width(border_width), text_color(text_color),
		border_color(border_color), background_color(background_color), 
		border_style(border_style) {}

	// 按钮析构函数
	Button::~Button()
	{
		free(_text);
	}

	// 绘制按钮
	void Button::draw() const
	{
		// 如果没有设置绘制前背景色，则获取当前背景色
		if (_background_before_draw_ != TRANSPARENT)
		{
			_background_before_draw_ = getpixel(x, y);
		}
		RECT rect = { x, y, x + width, y + height };
		setlinecolor(border_color);
		setlinestyle(border_style, border_width);
		setfillcolor(background_color);
		fillrectangle(x, y, x + width, y + height);
		settextstyle(font_size, 0, _T(""));
		settextcolor(text_color);
		setbkmode(TRANSPARENT);
		drawtext(_text, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	// 移除按钮
	void Button::remove() const
	{
		// 用颜色为按钮的背景色填充按钮区域即可
		setlinecolor(_background_before_draw_);
		setfillcolor(_background_before_draw_);
		fillrectangle(x, y, x + width, y + height);
	}

	// 设置按钮数据
	void Button::set_data(LPCTSTR text, int x, int y, int width, int height,
		int font_size, COLORREF text_color, COLORREF background_color,
		COLORREF border_color, int border_width, int border_style)
	{
		_text = _tcsdup(text);
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->font_size = font_size;
		this->border_width = border_width;
		this->text_color = text_color;
		this->border_color = border_color;
		this->background_color = background_color;
		this->border_style = border_style;
	}

	// 获取按钮文本
	LPCTSTR Button::text() const
	{
		return _text;
	}

	// 设置按钮文本
	void Button::set_text(LPCTSTR text)
	{
		free(_text);
		_text = _tcsdup(text);
	}

	// 显示按钮
	void Button::show()
	{
		_visible = true;
		draw();
	}

	// 隐藏按钮
	void Button::hide()
	{
		_visible = false;
		remove();
	}

	// 禁用按钮
	void Button::disable()
	{
		_disabled = true;
		draw();
	}

	// 启用按钮
	void Button::enable()
	{
		_disabled = false;
		draw();
	}

	// 判断按钮是否隐藏
	bool Button::hidden() const
	{
		return !_visible;
	}

	// 判断按钮是否被禁用
	bool Button::disabled() const
	{
		return _disabled;
	}

	// 判断坐标是否在按钮区域内
	bool Button::in_area(int x, int y) const
	{
		return x >= this->x && x <= this->x + width &&
			y >= this->y && y <= this->y + height;
	}

	// 判断按钮是否被点击
	bool Button::clicked(ExMessage& msg, bool right) const
	{
		if (!right && msg.message == WM_LBUTTONDOWN)
		{
			if (in_area(msg.x, msg.y)) return true;
		}
		else if (right && msg.message == WM_RBUTTONDOWN)
		{
			if (in_area(msg.x, msg.y)) return true;
		}
		return false;
	}
}