
/*!
 * \file snake.cpp
 * \date 2015/09/23 13:01
 *
 * \author yangs
 * Contact: yangshuoliu@sina.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#include "snake.h"


char Screen[SCREEN_Y][SCREEN_X];	/*屏幕矩阵*/
PointInfo SearchScreen[SCREEN_Y][SCREEN_X];	/*寻路屏幕矩阵*/
COORD curPos;						/*光标位置*/
COORD scrPos;						/*屏幕位置*/
DWORD rNum;						/*API 输出参数*/
bool SearchNewPath;
int scores = 0;



void SetScreenPosition(int x,int y)			//设置左上角位置屏幕位置
{
	scrPos.X = x;
	scrPos.Y = y;
}
//************************************
// Method:    DrawPoint
// FullName:  DrawPoint
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: HANDLE hInstance
// Parameter: int x
// Parameter: int y
// Parameter: ScreenType st
//************************************
void DrawPoint(HANDLE hInstance, int x, int y, ScreenType st)
{
	curPos.X = x;
	curPos.Y = y;

	curPos.X += scrPos.X;
	curPos.Y += scrPos.Y;
	SetConsoleCursorPosition(hInstance, curPos);
	switch (st)
	{
	case SCREEN_SNK:
		printf("O");
		break;
	case SCREEN_BARRIER:
		printf("*");
		//WriteConsoleOutputCharacter(hInstance, TEXT("*"), 1, curPos, &rNum);
		break;
	case SCREEN_FOOD:
		printf("#");
		//WriteConsoleOutputCharacter(hInstance, TEXT("@"), 1, curPos, &rNum);
		break;
	case SCREEN_NONE:
		printf(" ");
		//WriteConsoleOutputCharacter(hInstance, TEXT(" "), 1, curPos, &rNum);
		break;
	default:
		break;
	}
}
void DrawWall(HANDLE hInstance)
{
}

//************************************
// Method:    DrawScreen
// FullName:  DrawScreen
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: HANDLE hInstance
//************************************
void DrawScreen(HANDLE hInstance)
{
	SetConsoleCursorPosition(hInstance, scrPos);
	COORD newLine;
	newLine.X = scrPos.X;
	newLine.Y = scrPos.Y;
	for (int i = 0; i < SCREEN_Y; i++)
	{
		for (int j = 0; j < SCREEN_X; j++)
		{
			if (Screen[i][j] == SCREEN_BARRIER)
				printf("*");
			else if (Screen[i][j] == SCREEN_FOOD)
				printf("@");
			else
				printf(" ");
		}
		//printf("\n");
		newLine.Y = (i+1)+scrPos.Y;
		SetConsoleCursorPosition(hInstance, newLine);
	}
}

//************************************
// Method:    MakeFood
// FullName:  MakeFood
// Access:    public 
// Returns:   Point
// Qualifier: //随机产生食物
// Parameter: HANDLE hInstance
// Parameter: const SNAKE & snake
//************************************
Point MakeFood(HANDLE hInstance,const SNAKE & snake)		//随机产生食物
{
	/**/
	int crood_x;
	int crood_y;
	bool same = true;
	crood_x = rand() % (SCREEN_X - 1) + 1;
	crood_y = rand() % (SCREEN_Y - 1) + 1;
	while (same)
	{
		/*same = false;*/
		if (Screen[crood_y][crood_x] == SCREEN_NONE)
			same = false;
		else
		{
			crood_x = rand() % (SCREEN_X - 1) + 1;
			crood_y = rand() % (SCREEN_Y - 1) + 1;
		}
	}
	Screen[crood_y][crood_x] = SCREEN_FOOD;
	DrawPoint(hInstance, crood_x, crood_y, SCREEN_FOOD);
	return Point(crood_x, crood_y);
}

//************************************
// Method:    UpdateScores
// FullName:  UpdateScores
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: HANDLE hInstance
//************************************
void UpdateScores(HANDLE hInstance)
{
	curPos.X = 5;				//相对于游戏屏幕的位置
	curPos.Y = 0;
	SetConsoleCursorPosition(hInstance, curPos);
	DWORD rNum;
	curPos.X += scrPos.X;
	curPos.Y += scrPos.Y;
	WriteConsoleOutputCharacter(hInstance, TEXT("SCORES="), 7, curPos, &rNum);
	curPos.X = 12;
	curPos.Y = 0;
	curPos.X += scrPos.X;
	curPos.Y += scrPos.Y;
	SetConsoleCursorPosition(hInstance, curPos);
	printf("%d", scores);
}

void UpdateFoodBuffer()
{

}

//************************************
// Method:    init
// FullName:  init
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: HANDLE hInstance
// Parameter: SNAKE & snake
// Parameter: Direction direction
// Parameter: size_t size_snake
//************************************
void init(HANDLE hInstance,SNAKE & snake, Direction direction, size_t size_snake)
{
	/*初始化墙壁*/
	for (int i = 0; i < SCREEN_Y; i++)
	{
		for (int j = 0; j < SCREEN_X; j++)
		{
			if (i == 0 || i == SCREEN_Y - 1 || j == 0 || j == SCREEN_X - 1)
				Screen[i][j] = SCREEN_BARRIER;
			else
				Screen[i][j] = SCREEN_NONE;
		}
	}
	srand((unsigned)time(NULL));		//随机数种子
	int crood_x;
	int crood_y;
	crood_x = int(SCREEN_X / 2);		//在中间产生蛇头
	crood_y = int(SCREEN_Y / 2);
	snake.push_front(Point(crood_x, crood_y));
	Screen[crood_y][crood_x] = SCREEN_SNK;
	for (int i = 1; i < size_snake; i++)
	{
		if (direction == SNK_DOWN)
			crood_y--;
		else if (direction == SNK_UP)
			crood_y++;
		else if (direction == SNK_LEFT)
			crood_x++;
		else
			crood_x--;
		snake.push_back(Point(crood_x, crood_y));
		Screen[crood_y][crood_x] = SCREEN_SNK;
		UpdateScreen(&Point(crood_x, crood_y), SCREEN_SNK);
	}
	DrawScreen(hInstance);
	UpdateScores(hInstance);
}

//************************************
// Method:    NextPoint
// FullName:  NextPoint
// Access:    public 
// Returns:   Direction
// Qualifier:
// Parameter: Point * LastPoint
// Parameter: Direction LastDirection
// Parameter: Direction NextDirection
//************************************
Direction NextPoint(Point * LastPoint, Direction LastDirection, Direction NextDirection)
{
	switch (NextDirection)
	{
	case SNK_UP:
		if (SNK_LEFT == LastDirection || SNK_RIGHT == LastDirection)
		{
			LastPoint->y--;
			return NextDirection;
		}
		else if (SNK_UP == LastDirection)
			LastPoint->y--;
		else
			LastPoint->y++;
		return LastDirection;
		break;
	case SNK_DOWN:
		if (SNK_LEFT == LastDirection || SNK_RIGHT == LastDirection)
		{
			LastPoint->y++;
			return NextDirection;
		}
		else if (SNK_UP == LastDirection)
			LastPoint->y--;
		else
			LastPoint->y++;
		break;
	case SNK_LEFT:
		if (SNK_UP == LastDirection || SNK_DOWN == LastDirection)
		{
			LastPoint->x--;
			return NextDirection;
		}
		else if (SNK_LEFT == LastDirection)
			LastPoint->x--;
		else
			LastPoint->x++;
		break;
	case SNK_RIGHT:
		if (SNK_UP == LastDirection || SNK_DOWN == LastDirection)
		{
			LastPoint->x++;
			return NextDirection;
		}
		else if (SNK_RIGHT == LastDirection)
			LastPoint->x++;
		else
			LastPoint->x--;
		break;
	default:
		break;
	}
	return LastDirection;
}

//************************************
// Method:    UpdateScreen
// FullName:  UpdateScreen
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: const SNAKE & snake
//************************************
inline void UpdateScreen(const SNAKE & snake)
{
	for (SNAKE::const_iterator itr = snake.begin(); itr != snake.end(); ++itr)
		Screen[(*itr).y][(*itr).x] = SCREEN_SNK;
}

inline void UpdateScreen(const Point * point, ScreenType st){Screen[point->y][point->x] = st;}

inline bool Collision(Point * snakeHead)
{
	if (Screen[snakeHead->y][snakeHead->x] == SCREEN_BARRIER || Screen[snakeHead->y][snakeHead->x] == SCREEN_SNK)
		return true;
	return false;
}

//************************************
// Method:    GetDirection
// FullName:  GetDirection
// Access:    public 
// Returns:   char
// Qualifier:
// Parameter: Direction lastDireciton
//************************************
inline char GetDirection(Direction lastDireciton)
{
	if (GetAsyncKeyState(VK_UP))
		return SNK_UP;
	if (GetAsyncKeyState(VK_LEFT))
		return SNK_LEFT;
	if (GetAsyncKeyState(VK_RIGHT))
		return SNK_RIGHT;
	if (GetAsyncKeyState(VK_DOWN))
		return SNK_DOWN;
	return lastDireciton;
}
void SetBarrier(const Point & LeftUp, const Point & RightDown)
{
	for (int i = 0; i < RightDown.y - LeftUp.y; i++)
	{
		for (int j = 0; j < RightDown.x - LeftUp.x; j++)
		{
			Screen[LeftUp.y + i][LeftUp.x + j] = SCREEN_BARRIER;
		}
	}
}
//************************************
// Method:    ShowStartMenu
// FullName:  ShowStartMenu
// Access:    public 
// Returns:   Option
// Qualifier:
// Parameter: HANDLE hInstance
//************************************
Option ShowStartMenu(HANDLE hInstance)
{
	COORD optionPosition;
	for (int i = 0; i < SCREEN_Y; i++)
	{
		for (int j = 0; j < SCREEN_X; j++)
		{
			if (i == 0 || i == SCREEN_Y - 1 || j == 0 || j == SCREEN_X - 1)
			{
				optionPosition.X = j + scrPos.X;
				optionPosition.Y = i + scrPos.Y;
				WriteConsoleOutputCharacter(hInstance, TEXT("*"), 1, optionPosition, &rNum);
			}
		}
	}
	Option opt;

	const int cnt = 3;
	char * options[cnt] = {
		{"1.START GAME"},
		{"2.AI"},
		{"3.EXIT GAME"}
	};
	optionPosition.X = scrPos.X + SCREEN_X / 2-5;
	for (int i = 0; i < cnt; i++)
	{
		optionPosition.Y = scrPos.Y + (SCREEN_Y / (cnt *2))*(i+2);
		SetConsoleCursorPosition(hInstance, optionPosition);
		printf("%s", options[i]);
	}
	while (true)
	{
		opt = _getch();
		switch (opt)
		{
		case '1':
			opt = CTL_START;
			break;
		case '2':
			opt = CTL_AI;
			break;
		case '3':
			opt = CTL_EXIT;
			break;
		default:
			continue;
		}
		break;
	}
	return opt;
}
//************************************
// Method:    ShowGameOverMenu
// FullName:  ShowGameOverMenu
// Access:    public 
// Returns:   Option
// Qualifier:
// Parameter: HANDLE hInstance
//************************************
Option ShowGameOverMenu(HANDLE hInstance)
{
	Option opt;
	COORD optionPosition;
	const int cnt = 1;
	char * options[cnt] = {
		{ "GAME OVER?TYR AGAIN?(y or n)" },
	};
	optionPosition.X = scrPos.X + SCREEN_X / 2 - 13 ;
	optionPosition.Y = scrPos.Y + SCREEN_Y / 2-3;
	SetConsoleCursorPosition(hInstance, optionPosition);
	printf("%s", options[0]);
	while (true)
	{
		opt = _getch();
		switch (opt)
		{
		case 'y':
			opt = CTL_START;
			break;
		case 'n':
			opt = CTL_EXIT;
			break;
		default:
			continue;
		}
		break;
	}
	return opt;
}
//************************************
// Method:    GetAiDirection
// FullName:  GetAiDirection
// Access:    public 
// Returns:   Direction
// Qualifier:
// Parameter: list<Point> & iflx_lst
// Parameter: list<Direction> & drct_lst
// Parameter: Direction lastDirection
// Parameter: const Point & snakeHead
//************************************
inline Direction GetAiDirection(list<Point> & iflx_lst,
	list<Direction> & drct_lst,
	Direction lastDirection,
	const Point & snakeHead)
{
	list<Point>::iterator itr_iflx = iflx_lst.begin();
	if (snakeHead.x != itr_iflx->x||snakeHead.y != itr_iflx->y)		//蛇头没到拐点，继续上一个方向
	{
		return lastDirection;
	}
	iflx_lst.erase(itr_iflx);		//删除这个拐点
	list<Direction>::iterator itr_drct = drct_lst.begin();
	Direction direction = *itr_drct;
	if (direction == SNK_DRCT_EMPTY)
	{
		SearchNewPath = true;
		return lastDirection;
	}
	drct_lst.erase(itr_drct);
	return direction;
}

//************************************
// Method:    Game
// FullName:  Game
// Access:    public 
// Returns:   ExitType
// Qualifier:
// Parameter: HANDLE hInstance
// Parameter: SNAKE & snake
// Parameter: Direction initialDirection
// Parameter: int bodySize
// Parameter: DWORD speed
// Parameter: bool ai
//************************************
ExitType Game(HANDLE hInstance,SNAKE & snake,Direction initialDirection,int bodySize,DWORD speed,bool ai)
{
	Direction lastDirection = initialDirection;		//初始方向
	Direction nextDirection;
	Point snakeHead, snakeTail;
	int aiStepCycle=0;					//暂时先设置20步一个周期，这个应该根据蛇身长度来变化
	int aiSteps = 0;
	Point foodPosition=MakeFood(hInstance,snake);
	scores = 0;
	SearchNewPath = false;
	list<Point> inflexions_lst;
	list<Direction> dirctions_lst;
	while (1)
	{

		snakeHead = snake.front();					//取出蛇头位置
		if(ai==false)
			nextDirection = GetDirection(lastDirection);
		else
		{
			if (1||SearchNewPath)			//
			{
				inflexions_lst.erase(inflexions_lst.begin(),inflexions_lst.end());
				dirctions_lst.erase(dirctions_lst.begin(),dirctions_lst.end());
				SearchNewPath = false;
				AStarSearch(snakeHead, foodPosition);		//路径保存在SearchScreen数组里,寻路不一定成功，但是蛇比较小的时候还是可以完成的
				Inflexion(inflexions_lst, foodPosition);
				GetAiDirections(inflexions_lst, dirctions_lst);
				dirctions_lst.push_back(SNK_DRCT_EMPTY);		//
				aiSteps = 0;
			}
			nextDirection = GetAiDirection(inflexions_lst, dirctions_lst, lastDirection, snakeHead);
			aiSteps++;
		}
		lastDirection = NextPoint(&snakeHead, lastDirection, nextDirection);
		//如果转向成功，那么返回转向之前的方向，如果转向不成功（转向和当前方向相同），lastDirection则不变
		if (Screen[snakeHead.y][snakeHead.x] == SCREEN_FOOD)			//如果前方是食物
		{
			snake.push_front(Point(snakeHead.x, snakeHead.y));		//把食物坐标压入队列前端
			UpdateScreen(&Point(snakeHead.x, snakeHead.y), SCREEN_SNK);		//用身体替代食物
			DrawPoint(hInstance,snakeHead.x,snakeHead.y,SCREEN_SNK);			//更新屏幕
			//清除头部位置的食物缓冲队列	
			foodPosition=MakeFood(hInstance,snake);					//随机产生食物
			scores++;								//增加一分
			UpdateScores(hInstance);					//更新分数
		}
		else
		{
			snakeTail = snake.back();		//从队列后端取出蛇尾坐标
			UpdateScreen(&Point(snakeTail.x, snakeTail.y), SCREEN_NONE);		//把屏幕的蛇尾位置清空
			if (Collision(&snakeHead))//碰撞则游戏结束
				return GAME_OVER;
			snake.pop_back();			//删除队列后端
			DrawPoint(hInstance, snakeTail.x, snakeTail.y, SCREEN_NONE);		//清楚尾部的点
			snake.push_front(Point(snakeHead.x, snakeHead.y));				//添加新位置的蛇头
			UpdateScreen(&Point(snakeHead.x, snakeHead.y), SCREEN_SNK);
			DrawPoint(hInstance, snakeHead.x, snakeHead.y, SCREEN_SNK);
		}
		Sleep(speed);			//每个动作的速度是游戏等级
	}
}
