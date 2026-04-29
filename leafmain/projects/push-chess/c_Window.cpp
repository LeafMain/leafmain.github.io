#include <graphics.h>
#include "c_Window.h"

// 窗口类实例化
namespace pushchess
{
	Window::Window(const int flag) :
		_FLAG(flag) {}

	// 打开窗口
	void Window::open()
	{
		initgraph(WIDTH, HEIGHT, _FLAG);
		_opened = true;
	}

	// 关闭窗口
	void Window::close()
	{
		closegraph();
		_opened = false;
	}

	// 判断窗口是否已经打开
	bool Window::opened() const
	{
		return _opened;
	}
}