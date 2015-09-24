/*!
 * \file AStarSearch.cpp
 *
 * \author yangs
 * \date 9 2015
 *
 * 
 */
#include <queue>
#include <cmath>
#include <list>
#include <map>
#include "snake.h"
Point placement[NEIGHBORS] = {
	//Point(-1,-1),
	Point(0,-1),
	//Point(1,-1),
	Point(-1,0),
	Point(1,0),
	//Point(-1,1),
	Point(0,1),
	//Point(1,1) 
};
extern char Screen[SCREEN_Y][SCREEN_X];

extern PointInfo SearchScreen[SCREEN_Y][SCREEN_X];
//************************************
// Method:    IsValidCoord
// FullName:  IsValidCoord
// Access:    public 
// Returns:   bool
// Qualifier:
// Parameter: const Point & p
//************************************
inline bool IsValidCoord(const Point & p) { return (p.x >= 0 && p.x < SCREEN_X&&p.y >= 0 && p.y < SCREEN_Y); }
//************************************
// Method:    Heuristics
// FullName:  Heuristics
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const Point & p1
// Parameter: const Point & p2
//************************************
inline int Heuristics(const Point & p1, const Point & p2)
{
	int len = int(abs(p1.x - p2.x)) + int(abs(p1.y - p2.y));
	return len * 10;
}
//************************************
// Method:    InitSearchScreen
// FullName:  InitSearchScreen
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
void InitSearchScreen()
{
	for (int y = 0; y < SCREEN_Y; y++)
	{
		for (int x = 0; x < SCREEN_X; x++)
		{
			SearchScreen[y][x].state = out;
			SearchScreen[y][x].F = INF;
			SearchScreen[y][x].G = 0;
		}
	}
}
inline PointInfo & SearchScr(const Point & p) { return SearchScreen[p.y][p.x]; }

//************************************
// Method:    Inflexion
// FullName:  Inflexion
// Access:    public 
// Returns:   void
// Qualifier: 
// Parameter: list<Point> & inflexions
// Parameter: const Point & end
//************************************
void Inflexion(list<Point> & inflexions, const Point & end)		
{
	Point last = end;
	Point current = SearchScr(end).lastPoint;
	bool stayX;
	bool stayY;
	if ((current.x == last.x&&current.y != last.y))
	{
		stayY = true;
		stayX = false;
	}
	else if (current.y == last.y&&current.x != last.x)
	{
		stayX = true;
		stayY = false;
	}
	inflexions.push_front(last);		//最后一个
	while (current.x != -1)
	{
		if (stayX&&current.x == last.x&&current.y != last.y)		//上下转向
		{
			inflexions.push_front(last);
			stayY = true;
			stayX = false;
		}
		if (stayY&&current.x != last.x&&current.y == last.y)		//左右转向
		{
			inflexions.push_front(last);
			stayX = true;
			stayY = false;
		}
		last = current;
		current = SearchScr(current).lastPoint;
	}
	inflexions.push_front(last);		//第一个
}
//************************************
// Method:    GetAiDirections
// FullName:  GetAiDirections
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: const list<Point> & iflx_lst
// Parameter: list<Direction> & drct_lst
//************************************
void GetAiDirections(const list<Point> & iflx_lst, list<Direction> & drct_lst)
{
	Point prev, next;
	list<Point>::const_iterator c_itr = iflx_lst.begin();
	for (size_t i = 0; i<iflx_lst.size() - 1; i++)
	{
		prev = (*c_itr);
		next = *(++c_itr);
		if (next.x == prev.x&&next.y > prev.y)
			drct_lst.push_back(SNK_DOWN);
		else if (next.x == prev.x&&next.y < prev.y)
			drct_lst.push_back(SNK_UP);
		else if (next.y == prev.y&&next.x>prev.x)
			drct_lst.push_back(SNK_RIGHT);
		else if (next.y == prev.y&&next.x<prev.x)
			drct_lst.push_back(SNK_LEFT);
	}
}
//************************************
// Method:    FindMinF
// FullName:  FindMinF
// Access:    public 
// Returns:   Point
// Qualifier:
//************************************
Point FindMinF()
{
	int Fmin = INF;
	Point minp(-1, -1);
	for (int i = 0; i < SCREEN_Y; i++)
	{
		for (int j = 0; j < SCREEN_X; j++)
		{
			if (SearchScreen[i][j].state == open&&Fmin > SearchScreen[i][j].F)
			{
				Fmin = SearchScreen[i][j].F;
				minp.x = j;
				minp.y = i;
			}
		}
	}
	return minp;
}
//************************************
// Method:    AStarSearch
// FullName:  AStarSearch
// Access:    public 
// Returns:   bool
// Qualifier:
// Parameter: const Point & start
// Parameter: const Point & end
// 2015.09.24 change the search operation from open list with map
//************************************
bool AStarSearch(const Point & start, const Point & end)
{
	/*repalced by the map,initailized by the map*/
	InitSearchScreen();
	SearchScr(start).lastPoint = Point(-1, -1);
	SearchScr(start).state = open;				//load the start node into index array
	SearchScr(start).F = 0;
	/*replaced by the map*/

	
	Point currentPoint;
	Point neighbor;
	int distance;
	bool found = false;
	
	currentPoint = FindMinF();
	while (currentPoint.x != -1)
	{
		//search the point with the min f 
		SearchScr(currentPoint).state = close;
		if (currentPoint == end)
		{
			found = true;
			break;
		}
		for (int i = 0; i < NEIGHBORS; i++)
		{
			//
			neighbor = currentPoint + placement[i];
			//
			if (IsValidCoord(neighbor) && SearchScr(neighbor).state != close&& Screen[neighbor.y][neighbor.x] == SCREEN_NONE|| Screen[neighbor.y][neighbor.x]==SCREEN_FOOD)
			{
				
				if (placement[i].x != 0 && placement[i].y != 0)	//
					distance = 14;
				else
					distance = 10;
				if (SearchScr(neighbor).state == out)	//search the neighbor in the map,if not exists	
				{
					SearchScr(neighbor).G = SearchScr(currentPoint).G + distance;
					SearchScr(neighbor).F = SearchScr(neighbor).G + Heuristics(neighbor, end);
					SearchScr(neighbor).lastPoint = currentPoint;
					SearchScr(neighbor).state = open;
				}
				else              //if exists
				{
					if (SearchScr(neighbor).G > SearchScr(currentPoint).G + distance)	//if updatble
					{
						SearchScr(neighbor).G = SearchScr(currentPoint).G + distance;
						SearchScr(neighbor).F = SearchScr(neighbor).G + Heuristics(neighbor, end);
						SearchScr(neighbor).lastPoint = currentPoint;
					}
				}
			}
		}
		currentPoint = FindMinF();
	}
	return found;
}
#if DEBUG
void DrawScreen()
{
	for (int i = 0; i < SCREEN_Y; i++)
	{
		for (int j = 0; j < SCREEN_X; j++)
		{
			if (Screen[i][j] == SCREEN_BARRIER)
				printf("*");
			else if (Screen[i][j] == SCREEN_FOOD)
				printf("@");
			else if (Screen[i][j] == SCREEN_PATH)
				printf("+");
			else if (Screen[i][j] == SCREEN_PATH_TRY)
				printf("-");
			else
				printf(" ");
		}
		printf("\n");
	}
}

void SetStartAndEnd(const Point & p1, const Point & p2)
{
	Screen[p1.y][p1.x] = SCREEN_FOOD;
	Screen[p2.y][p2.x] = SCREEN_FOOD;
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
#endif