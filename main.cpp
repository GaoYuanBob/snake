/*!
 * \class classname
 *
 * \brief 
 *
 * \author yangs
 * \date 九月 2015
 */
#include "snake.h"

//************************************
// Method:    main
// FullName:  main
// Access:    public 
// Returns:   int
// Qualifier:
//************************************
int main()
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);		//获取控制台句柄
	SetConsoleDisplayMode(hStdOut, 1, NULL);	//全屏显示
	CONSOLE_CURSOR_INFO curInfo;				//光标信息
	GetConsoleCursorInfo(hStdOut, &curInfo);
	curInfo.bVisible = FALSE;
	SetConsoleCursorInfo(hStdOut, &curInfo);	//光标不可见
	SNAKE snake;				//贪吃蛇身体双端队列
	bool ai = false;
	//屏幕位置
	SetScreenPosition(80, 15);
	//初始化屏幕矩阵，食物和蛇身的位置
	//开始游戏
	Option menu;
	menu = ShowStartMenu(hStdOut);
	if (menu == CTL_EXIT)
		goto exit;
	if (menu == CTL_AI)
		ai = true;
	while (1)
	{
		init(hStdOut, snake,SNK_DOWN,2);
		Game(hStdOut, snake, SNK_DOWN, 1, 50,ai);
		menu=ShowGameOverMenu(hStdOut);
		if (menu == CTL_EXIT)
			goto exit;
		snake.erase(snake.begin(), snake.end());
	}
exit:
	return 0;
}