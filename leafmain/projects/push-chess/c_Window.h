#pragma once

// 窗口类声明
namespace pushchess
{
	// 窗口类
	class Window
	{
	public:
		static const int WIDTH; // 窗口宽度
		static const int HEIGHT; // 窗口高度
	public:
		// 构造函数
		// @param flag = 0 窗口打开标志
		Window(const int flag = 0);

		// 打开窗口
		void open(); 

		// 关闭窗口
		void close(); 

		// 判断窗口是否已经打开
		// @return bool
		bool opened() const; 
	private:
		const int _FLAG = 0;
		bool _opened = false; // _窗口打开标志
	};
}