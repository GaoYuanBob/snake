#include <queue>		//deque<int>
#include <list>
#include <stdio.h>	// printf()
#include <conio.h>
#include <ctime>		//srand() rand()
#include <Windows.h>	//Sleep(),SetConsoleCursorPosition(),GetStdHandle()
/////////////////////////方向码
#define SNK_UP			0
#define SNK_DOWN			1
#define SNK_LEFT			2
#define SNK_RIGHT			3

#define SNK_DRCT_EMPTY		4
/////////////////////////控制码
#define CTL_EXIT			1
#define CTL_START			2
#define CTL_AI			3
#define CTL_ESC			4
#define CTL_PAUSE			5
#define CTL_CONTINUE		6
//////////////////////////
#define GAME_OVER			6
/////////////////////////屏幕大小
#define SCREEN_X			70
#define SCREEN_Y			30
////////////////////////屏幕单位类型
#define SCREEN_FOOD		2	//食物
#define SCREEN_NONE		0	//空
#define SCREEN_BARRIER		1	//障碍
#define SCREEN_SNK		3	//蛇身体

#define SCREEN_PATH		4
#define SCREEN_PATH_TRY 5

#define INF 10000000
#define DEBUG 0

#define NEIGHBORS 4
using namespace std;		//导入命名空间
enum PointState { open = 0, close = 1, out = 2 };
struct Point
{
	Point() {}
	Point(int _x, int _y) :x(_x), y(_y) {}
	Point(const Point & p) :x(p.x), y(p.y) {}
	int x;
	int y;
	friend Point operator+(const Point & p1, const Point & p2) { return Point(p1.x + p2.x, p1.y + p2.y); }
	friend bool operator==(const Point & p1, const Point & p2) { return (p1.x == p2.x&&p1.y == p2.y); }
	friend bool operator!=(const Point & p1, const Point & p2) { return !(p1 == p2); }
};
struct PointInfo
{
	PointInfo() {}
	int F;
	int G;
	PointState state;
	Point lastPoint;
	inline friend bool operator<(const PointInfo & p1, const PointInfo & p2)
	{
		return p1.F < p2.F;
	}
};

typedef char ScreenType;
typedef char Direction;
typedef char ExitType;
typedef char Option;
typedef deque<Point> SNAKE;

void init(HANDLE hInstance, SNAKE & snake,Direction direction,size_t size_snake);
Direction NextPoint(Point * LastPoint, Direction LastDirection, Direction NextDirection);
inline void UpdateScreen(const SNAKE & snake);
inline void UpdateScreen(const Point * point, ScreenType st);
inline bool Collision(Point * snakeHead);
inline char GetDirection(Direction lastDireciton);
inline char GetKey();
Option ShowStartMenu(HANDLE hInstance);
Option ShowGameOverMenu(HANDLE hInstance);
ExitType Game(HANDLE hInstance, SNAKE & snake, Direction initialDirection, int bodySize, DWORD speed,bool ai);
void SetScreenPosition(int x, int y);
void DrawPoint(HANDLE hInstance, int x, int y, ScreenType st);
void DrawScreen(HANDLE hInstance);
void DrawWall(HANDLE hInstance);
Point MakeFood(HANDLE hInstance, const SNAKE & snake);
void UpdateScores(HANDLE hInstance);
void UpdateFoodBuffer();
////////AI部分函数
bool AStarSearch(const Point & start, const Point & end);
void Inflexion(list<Point> & inflexions, const Point & end);
void GetAiDirections(const list<Point> & iflx_lst, list<Direction> & dirct_lst);
inline Direction GetAiDirection(list<Point> & iflx_lst,
	vector<Direction> & dirct_vct,
	Direction lastDirection,
	const Point & snakeHead);
