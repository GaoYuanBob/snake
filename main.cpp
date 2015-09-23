/*!
 * \class classname
 *
 * \brief 
 *
 * \author yangs
 * \date ���� 2015
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
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);		//��ȡ����̨���
	SetConsoleDisplayMode(hStdOut, 1, NULL);	//ȫ����ʾ
	CONSOLE_CURSOR_INFO curInfo;				//�����Ϣ
	GetConsoleCursorInfo(hStdOut, &curInfo);
	curInfo.bVisible = FALSE;
	SetConsoleCursorInfo(hStdOut, &curInfo);	//��겻�ɼ�
	SNAKE snake;				//̰��������˫�˶���
	bool ai = false;
	//��Ļλ��
	SetScreenPosition(80, 15);
	//��ʼ����Ļ����ʳ��������λ��
	//��ʼ��Ϸ
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