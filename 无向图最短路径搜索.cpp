
 
#include<stdio.h>		//调用内部包
#include<malloc.h>		//调用内存分配文件包
#include<string.h>
#define InAccessible   100000		//无法抵达时的总长度
#define Status int
#define OK 2
#define TRUE 1
#define change 1					//用于扩大1位，进行占位和存放
#define FALSE 0
#define PLen 0
#define none -1
#define ERROR -1					//用于判定是否出现错误
#define OVERFLOW -2					//用于判断是否分配成功
#define ERROR_01 -3					//入参不正确
#define ERROR_02 -4					//无法生成虽短路径	
#define MaxNum 20					//最大支持的存放的节点数目
#define NUM 13                      //总的地址名称个数
 
//图的数据结构
typedef struct Graph
{
	int vexs[MaxNum];				//定义结点矩阵，用于存放所有的结点
	int arcs[MaxNum][MaxNum];		//定义邻接矩阵，用于之后的计算和分析
	int vexnum;						//总的节点数目
	int arcnum;						//总的边的数目
}Graph;								//图的结构体的名称
 
 
//所有的地点名称及其所对应的在图中顶点的脚标
const char* PlaceName[NUM] = { "北门","饮水思源","传送门3","传送门1","腾飞塔","图书馆","传送门2","传送门4","教学主楼","宪梓堂","西迁馆","活动中心","南门" };
							  //0        1          2         3         4        5        6         7         8         9         10       11       12
 
//对图片进行初始化
Status InitMap(Graph* map)
{
	if (!map) return ERROR;									//鲁棒性检测
	map->vexnum = 13;										//所有结点的个数
	map->arcnum = 18;										//所有边的条数
	//初始化所有的结点和边
	int i = 0, j = 0;				
	for (i = 0; i < map->vexnum; ++i) {
		map->vexs[i] = i;
		for (j = 0; j < map->vexnum; ++j)					//对角线上初始化为0
			if (i == j) map->arcs[i][j] = 0;
			else {
				map->arcs[i][j] = InAccessible;				//全部先初始化为不连通
			}
	}
	//初始化邻接矩阵
	map->arcs[0][2] = map->arcs[2][0] = 22;
	map->arcs[0][1] = map->arcs[1][0] = 18;
	map->arcs[1][3] = map->arcs[3][1] = 27;
	map->arcs[1][4] = map->arcs[4][1] = 19;
	map->arcs[4][2] = map->arcs[2][4] = 4;
	map->arcs[4][7] = map->arcs[7][4] = 32;
	map->arcs[3][5] = map->arcs[5][3] = 4;
	map->arcs[5][6] = map->arcs[6][5] = 4;
	map->arcs[4][5] = map->arcs[5][4] = 23;
	map->arcs[5][8] = map->arcs[8][5] = 15;
	map->arcs[8][11] = map->arcs[11][8] = 21;
	map->arcs[11][12] = map->arcs[12][11] = 30;
	map->arcs[7][9] = map->arcs[9][7] = 4;
	map->arcs[8][9] = map->arcs[9][8] = 8;
	map->arcs[6][12] = map->arcs[12][6] = 21;
	map->arcs[6][10] = map->arcs[10][6] = 43;
	map->arcs[10][12] = map->arcs[12][10] = 20;
	map->arcs[9][12] = map->arcs[12][9] = 14;
	return OK;
}
 
typedef int PathMatrix[MaxNum][MaxNum+change];		//定义路径矩阵
typedef struct {
	int weight[MaxNum];								//定义最短路径长度和
	Status Visited[MaxNum];							//定义列表标记是否被访问过
}ShortPathList;
 
//判断入参数量是否正确
Status IfError_01(int argc)
{
	if (argc == 3) return FALSE;
	else 
	{
		printf("ERROR_01");
		return TRUE;
	}
}
 
//寻找输入的地点在地点列表中的位置,如果找到了，则返回下标,如果找不到，则返回ERROR_02.
int FindPlace(const char* PlaceName[], char* name,Graph* map)
{
	if (!PlaceName || !name|| !map) return ERROR;					//鲁棒性检测
	int i = 0;
	while (i < map->vexnum)											//进行寻址
	{
		if (strcmp(PlaceName[i], name) == 0) return i;
		i++;
	}
	printf("ERROR_02");												//当跳出循环还没返回答案时，认为出错了
	return ERROR_02;
}
 
//定义函数初始化最短路径列表，所有visit状态更新为FALSE。所有权重进行处理，除自己到自己之外全部为无穷大
Status InitList(ShortPathList* List,Graph map,int start)
{
	if (!List) return ERROR;
	for (int i = 0; i < map.vexnum; i++)
	{
		List->Visited[i] = FALSE;
		List->weight[i] = InAccessible;
	}
	List->weight[start] = 0;
	return OK;
}
//让path矩阵的每行第一列表示路径长度.
//每行第一列全部初始化为-1
Status InitPath(PathMatrix& path,Graph map,int start)
{
	if (!path) return ERROR;
	for (int i = 0; i < map.vexnum; i++)
	{
		for (int j = 0; j < map.vexnum + change; j++)
		{
			path[i][j] = none;						//将所有的长度初始化为-1
		}
	}
	path[start][0] = 1;
	path[start][1] = start;
	return OK;
}
//更新路径，使得路径更小，只是一个交换作用的函数，具体实现依靠ChangeWeight

Status UpdatePath(PathMatrix &path,int before,int now,Graph map)
{
	if (!path) return ERROR;
	for (int i = 0; i < map.vexnum; i++)
	{
		path[before][i] = path[now][i];				//将小的直接抄过来
	}
	return OK;
}
 
//修改结点到起点的路径长度最小值，并调用函数UpdatePath更新每个结点的最短路径。

Status ChangeWeight(Graph map, ShortPathList* list, int curnode, PathMatrix& path)
{
	if (!list||!path) return ERROR;
	for (int i = 0; i < map.vexnum; i++)
	{
		if (list->Visited[i] == FALSE)
		{
			if (list->weight[i] > list->weight[curnode] + map.arcs[curnode][i])         //原先路径长度大于当前路径
			{                                                                           //如果新的长度小于原有长度，则进行更新。
			                                                                            //更新后最后一个结点路径指向被更新的结点。
				UpdatePath(path, i, curnode,map);										//当满足条件时更新路径
				path[i][PLen]++;														//总的路径长度path[i][0]加一
				path[i][path[i][PLen]] = i;												//补上当前的位置
				list->weight[i] = list->weight[curnode] + map.arcs[curnode][i];			//将权重更新为较小的值
			}
		}
	}
	return OK;
}
//寻找最短的路径的结点并修改访问状态


int FindShort(ShortPathList* list,Graph map)                    //map用于存放图的信息
{
	if (!list) return ERROR;                                    //list存放有每个节点到起点的距离
	int shortest=none;
	int flag = TRUE;
	int i = 0;
	while ( i < map.vexnum && flag)								//寻找第一个未被访问的结点
	{
		if (list->Visited[i] == FALSE)
		{
			shortest = i;
			flag =FALSE;
		}
		i++;
	}
	for (int i = 0; i < map.vexnum; i++)						//寻找所有节点中未被访问的最小长度结点
	{
		if (list->Visited[i] == FALSE)
		{
			if (list->weight[i] < list->weight[shortest])       //如果找到一个未访问的节点，且新路径更短
			{
				shortest = i;
			}
		}
	}

	return shortest;											//返回角标，如果角标是none，认为所有都访问完了，可以给结果了
}


//求出指定起点到终点的最短路径，并且存放最短路径

int ShortestPath(Graph map, PathMatrix& path, ShortPathList* list, int start, int end) //输入已经完成初始化的图
{
	if (!path || ! list) return ERROR;                        //path存放路径脚步
	InitList(list,map,start);                                 //start为起点序号，list是放最短路径长度的数组
	InitPath(path, map,start);
	int cursor;
	while (FindShort(list, map) != none)                      //找到具体的角标，说明还有更短的路径，继续更新
	{
		cursor = FindShort(list, map);
		list->Visited[cursor] = TRUE;
		ChangeWeight(map, list, cursor, path);
	}
	return list->weight[end];
}
 
Status PrintPath(PathMatrix path, int end,const char** PlaceName)
{
	if (!PlaceName) return ERROR;
	int flag = TRUE;
	for (int i = 0; i < path[end][0]; i++)
	{
		if (flag)
		{
			printf("\n%s", PlaceName[path[end][i + change]]);
			flag = FALSE;
		}

	}
	return OK;
}
int main(int argc, char* argv[])
{
	if(IfError_01(argc)) return ERROR_01;
	Graph* map = (Graph*)malloc(sizeof(Graph));
	if (!map) return OVERFLOW;
	InitMap(map);
	PathMatrix path = { none };														//定义记录路径的矩阵
	ShortPathList* list=(ShortPathList*) malloc(sizeof(ShortPathList));				//定义所有结点到起点的路径长度，
	if (!list) return OVERFLOW;								
	int start = FindPlace(PlaceName, argv[1], map);			//起始地点的坐标
	if (start == ERROR_02) return ERROR_02;
	int end = FindPlace(PlaceName, argv[2], map);			//终止地点的坐标
	if (end == ERROR_02) return ERROR_02;
	int shortestdistance = 0;
	shortestdistance=ShortestPath(*map, path, list, start, end);					//寻找最小路径长度并将其存放进shortestdistance
	printf("%d", shortestdistance);
	//PrintPath(path,end,PlaceName);
	free(map);																		//释放map指针
	free(list);																		//释放list指针
	return 0;
}
 
