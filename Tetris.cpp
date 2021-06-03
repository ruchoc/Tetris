#include <iostream>
#include <string>
#include <cstdlib>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <cstdio>
#include <iomanip>

#pragma warning(disable:4996)；//VS时使用

using namespace std;

struct PlayersInfo
{
	char name[10];
	int rank;
	int score;
}players[200];  //玩家信息

int playerCount;

void save();  //存储玩家信息
void load();  //读取玩家信息

class Tetris
{
private:
	int rank;
	int score;
	int id;
	int point[2];
	int top;
	bool speedUp;
	int count;
public:
	Tetris();
	void welcome();
	void setting();
	void rankingList();
	void developer();
	void drawMap();
	void setColor(int);
	void draw(int, int, int);
	void run();
	void reDraw(int, int, int);
	bool judgeNot(int, int, int);
	void turn(int);
	void update();
	void pause();
	void settlement();
	void clearing();
};

const int sharp[19][8] =					//组成图形的各个点的各个坐标，先纵后横
{
{0,0,1,0,2,0,3,0},{0,0,0,1,0,2,0,3},
{0,0,1,0,0,1,1,1},
{0,0,1,0,1,1,1,2},{0,1,1,1,2,0,2,1},{0,0,0,1,0,2,1,2},{0,0,0,1,1,0,2,0},
{1,0,1,1,1,2,0,2},{0,0,0,1,1,1,2,1},{0,0,0,1,0,2,1,0},{0,0,1,0,2,0,2,1},
{0,0,0,1,1,1,1,2},{0,1,1,0,1,1,2,0},
{0,1,0,2,1,0,1,1},{0,0,1,0,1,1,2,1},
{0,1,1,0,1,1,1,2},{0,0,1,0,1,1,2,0},{0,0,0,1,0,2,1,1},{0,1,1,0,1,1,2,1}
};

const int high[19] = { 4,1,2,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3 };
int map[28][16];

#define a1  0	//I形
#define a2  1

#define b 2		// O形

#define c1 3	//J形
#define c2 4
#define c3 5
#define c4 6

#define d1 7	//L形
#define d2 8 
#define d3 9
#define d4 10

#define e1 11	//Z形
#define e2 12

#define f1 13	//S形
#define f2 14

#define g1 15 //T形
#define g2 16
#define g3 17
#define g4 18

int originalPoint[2] = { 0,0 };  //坐标原点

void setPosition(int i, int j)  //设置光标坐标
{
	COORD coord = { i + originalPoint[1],j + originalPoint[0] };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void modeSet(int row, int col) {  //设置控制台窗口大小
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = false;  //光标不可视化
	SetConsoleCursorInfo(hOut, &cci);
	COORD size = { col,row };
	SetConsoleScreenBufferSize(hOut, size);
	SMALL_RECT rc = { 1,1,col,row };
	SetConsoleWindowInfo(hOut, true, &rc);
}

Tetris::Tetris() {
	speedUp = false;
	rank = 1;
	point[0] = 0;
	point[1] = 5;
	score = 0;
	top = 25;
}

void Tetris::setColor(int colorNum)  //设置控制台文本颜色
{
	int n = 0;
	switch (colorNum)
	{
	case 0:n = 0x08; break;  //灰白
	case 1:n = 0x0C; break;  //红色
	case 2:n = 0x0D; break;  //紫色
	case 3:n = 0x0E; break;  //金色
	case 4:n = 0x0A; break;  //绿色
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), n);
}

void Tetris::welcome()
{//开始菜单
T:
	originalPoint[0] = 8;
	originalPoint[1] = 10 * 2;
	setColor(1);
	system("cls");
	setPosition(0, 0);
	cout << "■■                       ■■";
	setPosition(0, 1);
	cout << "  ■■      Tetris           ■";
	setPosition(0, 2);
	cout << "                             ■";
	setPosition(0, 3);
	cout << "           		";
	setPosition(0, 4);
	cout << "     ■■■■■■■■■■■";
	setPosition(0, 5);
	cout << "           		";
	setPosition(0, 6);
	cout << "		Game Start			";
	setPosition(0, 7);
	cout << "		Setting			";
	setPosition(0, 8);
	cout << "		Ranking List		";
	setPosition(0, 9);
	cout << "		Developer			";
	setPosition(0, 10);
	cout << "		Exit			";
	setPosition(0, 11);
	cout << "■";
	setPosition(0, 12);
	cout << "■                         ■■";
	setPosition(0, 13);
	cout << "■                         ■■";
	setPosition(0, 14);
	cout << "■";
	int menu[5][2] = { {6,7},{7,7},{8,7},{9,7},{10,7} };  //菜单选项坐标
	int i = 0;
	setPosition(menu[i][1], menu[i][0]);
	cout << "■";  //默认菜单选项坐标

	bool flag = false;

	while (true) {
		int key, key2;  //获取上下左右键需使用两个字符获取
		if (_kbhit()) {
			key = _getch();
			if (key == 224) {
				key2 = _getch();
				if (key2 == 72) {
					setPosition(menu[i][1], menu[i][0]);
					cout << "  ";
					i = (i + 4) % 5;
					setPosition(menu[i][1], menu[i][0]);
					cout << "■";
				}
				else if (key2 == 80) {
					setPosition(menu[i][1], menu[i][0]);
					cout << "  ";
					i = (i + 6) % 5;
					setPosition(menu[i][1], menu[i][0]);
					cout << "■";
				}
			}
			else if (key == 13) {
				switch (i) {  //跳转至其他各处
				case 0:flag = true; break;
				case 1:setting(); goto T;
				case 2:rankingList(); goto T;
				case 3:developer(); goto T;
				case 4:system("cls"); exit(0); break;
				}
			}
			if (flag) {
				run();
				break;
			}
		}
	}
}

void Tetris::rankingList() {
	system("cls");
	originalPoint[0] = 8;
	originalPoint[1] = 16;
	setPosition(0, 0);
	cout << "■■                              ■";
	setPosition(0, 1);
	cout << "■■           Charts             ■";
	setPosition(34, 2);
	cout << "■";
	setPosition(0, 3);
	cout << "      ■■■■■■■■■■■■■  ■";
	setPosition(0, 1);
	setPosition(6, 4);
	cout << "rank      name      score" << endl;
	for (int i = 0; i < playerCount && i < 10; ++i) {  //获取前十名玩家信息
		setPosition(7, i + 5);
		switch (i) {  //标注前三名玩家颜色
		case 0:setColor(3); break;
		case 1:setColor(2); break;
		case 2:setColor(4); break;
		default:setColor(0); break;
		}
		cout << left << setw(9) << players[i].rank;
		cout << setw(11) << players[i].name;
		cout << players[i].score << endl;
	}
	setColor(1);
	setPosition(0, 15);
	cout << "■                                ■";
	setPosition(0, 16);
	cout << "■                ■              ■■";
	setPosition(0, 17);
	cout << "■■            ■■■              ■";
	_getch();  //滞留画面，按任意键继续
}

void Tetris::developer() {  //制作团队信息
	originalPoint[0] = 6;
	originalPoint[1] = 12;
	system("cls");
	setColor(3);
	setPosition(0, 0);
	cout << "  ■";
	setPosition(0, 1);
	cout << "■■■";
	setPosition(0, 2);
	cout << "  ■";
	setPosition(0, 3);
	cout << "      开发团队：six-zero-eight";
	setPosition(0, 4);
	cout << "      团队成员：刘润超，李子良，梁以伟，梁涛";
	setPosition(0, 6);
	cout << "      主策划：刘润超                  ■";
	setPosition(0, 7);
	cout << "      美术策划：梁涛                ■■";
	setPosition(0, 8);
	cout << "      程序策划：李子良，梁以伟      ■";
	setPosition(0, 10);
	cout << "    ■■■■■■■■■■■■■■■■■■■■";
	setPosition(0, 12);
	cout << "        ■";
	setPosition(0, 13);
	cout << "        ■■■";
	setPosition(0, 15);
	cout << "                        ■■■";
	setPosition(0, 16);
	cout << "                          ■";
	setPosition(0, 17);
	cout << "    ■                                      ■";
	setPosition(0, 18);
	cout << "    ■■      ■■                        ■■";
	setPosition(0, 19);
	cout << "              ■              ■■■";
	setPosition(0, 20);
	cout << "              ■                ■";
	setPosition(0, 21);
	_getch();
}

void Tetris::setting() {  //设置难度系数rank
	originalPoint[0] = 8;
	originalPoint[1] = 20;
	int menu1[2][4] = { {5,16,5,24},{6,16,6,28} };
	int menu2[3][4] = { {7,16,7,23},{8,16,8,23},{9,16,9,26} };
	setColor(2);
	while (true) {
		system("cls");
		setPosition(0, 0);
		cout << "  ■                        ■■";
		setPosition(0, 1);
		cout << "  ■                          ■";
		setPosition(0, 2);
		cout << "  ■                          ■";
		setPosition(0, 3);
		cout << "  ■         Setting";
		setPosition(0, 4);
		cout << "      ---------------------";
		setPosition(0, 5);
		cout << "    Game Mode :  Classic";
		setPosition(0, 6);
		cout << "                 Competitive";
		setPosition(0, 7);
		cout << "      Degree  :  Simple";
		setPosition(0, 8);
		cout << "                 Normal";
		setPosition(0, 9);
		cout << "    ■■         Difficult    ■";
		setPosition(0, 10);
		cout << "    ■■                      ■";
		setPosition(0, 11);
		cout << "                            ■■";
		int i = 0;
		setPosition(menu1[i][1], menu1[i][0]);
		cout << "<";
		setPosition(menu1[i][3], menu1[i][2]);
		cout << ">";
		while (true) {
			int key, key2;
			if (_kbhit()) {
				key = _getch();
				if (key == 224) {
					key2 = _getch();
					if (key2 == 72) {
						setPosition(menu1[i][1], menu1[i][0]);
						cout << " ";
						setPosition(menu1[i][3], menu1[i][2]);
						cout << " ";
						i = (i + 1) % 2;
						setPosition(menu1[i][1], menu1[i][0]);
						cout << "<";
						setPosition(menu1[i][3], menu1[i][2]);
						cout << ">";
					}
					else if (key2 == 80) {
						setPosition(menu1[i][1], menu1[i][0]);
						cout << " ";
						setPosition(menu1[i][3], menu1[i][2]);
						cout << " ";
						i = (i + 3) % 2;
						setPosition(menu1[i][1], menu1[i][0]);
						cout << "<";
						setPosition(menu1[i][3], menu1[i][2]);
						cout << ">";
					}
				}
				else if (key == 13) {
					switch (i) {
					case 0:goto T; break;
					case 1:rank = 0; return;
					}
				}
			}
		}
	T:
		setPosition(menu2[i][1], menu2[i][0]);
		cout << "<";
		setPosition(menu2[i][3], menu2[i][2]);
		cout << ">";
		while (true) {
			int key, key2;
			if (_kbhit()) {
				key = _getch();
				if (key == 224) {
					key2 = _getch();
					if (key2 == 72) {
						setPosition(menu2[i][1], menu2[i][0]);
						cout << " ";
						setPosition(menu2[i][3], menu2[i][2]);
						cout << " ";
						i = (i + 2) % 3;
						setPosition(menu2[i][1], menu2[i][0]);
						cout << "<";
						setPosition(menu2[i][3], menu2[i][2]);
						cout << ">";
					}
					else if (key2 == 80) {
						setPosition(menu2[i][1], menu2[i][0]);
						cout << " ";
						setPosition(menu2[i][3], menu2[i][2]);
						cout << " ";
						i = (i + 4) % 3;
						setPosition(menu2[i][1], menu2[i][0]);
						cout << "<";
						setPosition(menu2[i][3], menu2[i][2]);
						cout << ">";
					}
				}
				else if (key == 13) {
					switch (i) {
					case 0:rank = 1; return;
					case 1:rank = 2; return;
					case 2:rank = 3; return;
					}
				}
			}
		}
	}
}

void Tetris::drawMap()
{//打印游戏界面，划分功能区块
	originalPoint[0] = 6;
	originalPoint[1] = 6 * 2;
	system("cls");
	setColor(3);
	setPosition(0, -1);
	cout << "    难度 " << rank;
	cout << "        得分 " << score;
	cout << "          下一图形      ";
	setColor(4);
	setPosition(0, -1);
	cout << "■";
	setPosition(14, -1);
	cout << "■";
	setPosition(28, -1);
	cout << "■";
	setPosition(46, -1);
	cout << "■";
	setColor(2);
	setPosition(0, -4);
	cout << "■                    ■                      ■";
	setPosition(-2, -3);
	cout << "■■                  ■■■                    ■■";
	setPosition(-2, -2);
	cout << "■";
	setPosition(48, -2);
	cout << "■";
	setPosition(-2, 25);
	cout << "■";
	setPosition(48, 25);
	cout << "■";
	setPosition(-2, 26);
	cout << "■";
	setPosition(48, 26);
	cout << "■";
	setPosition(-2, 27);
	cout << "■■                  ■■■■                  ■■";
	setColor(1);
	setPosition(34, 17);
	cout << "■■■■";
	setPosition(34, 18);
	cout << "■      ■";
	setPosition(34, 19);
	cout << "■      ■";
	setPosition(34, 20);
	cout << "■■■■";
	setPosition(34, 21);
	cout << "■  ■";
	setPosition(34, 22);
	cout << "■    ■";
	setPosition(34, 23);
	cout << "■      ■";
	setColor(4);
	setPosition(0, -2);
	for (int i = 0; i < 24; ++i)  //宽24格
		cout << "■";
	for (int i = 0; i < 24; ++i)  //宽24格
	{
		setPosition(i * 2, 0);
		cout << "■";
		setPosition(i * 2, 26);
		cout << "■";
	}
	for (int i = 0; i < 26; ++i)  //高26格
	{
		setPosition(0, i);
		cout << "■";
		setPosition(28, i);
		cout << "■";
		setPosition(46, i);
		cout << "■";
	}
	for (int i = 14; i < 24; ++i)
	{
		setPosition(i * 2, 6);
		cout << "■";
	}
	setColor(0);
	setPosition(34, 8);
	cout << "操作方式";
	setPosition(32, 10);
	cout << "↑ - 旋转";
	setPosition(32, 11);
	cout << "↓ - 加速下移";
	setPosition(32, 12);
	cout << "← - 左移";
	setPosition(32, 13);
	cout << "→ - 右移";
	setPosition(32, 14);
	cout << "space - 暂停";
}

void Tetris::run()
{//主运行函数
	drawMap();  //绘制游戏桌面
	srand((int)time(0));  //随机数以时间为种子
	id = rand() % 19;  //获取0~18的随机数
	int nextID = rand() % 19;
	draw(point[0], point[1], id);  //在point数组位置打印编号为id的俄罗斯方块
	draw(1, 17, nextID);  //在预告区绘制下一个俄罗斯方块
	if (rank == 1)  //根据难度系数rank调整count与speedUp
		count = 150;
	else if (rank == 2)
		count = 100;
	else if (rank == 3)
		count = 50;
	else {
		count = 160;
		speedUp = true;
	}
	int i = 0;
	while (1)
	{
		if (i >= count)  //i与count控制时间
		{
			i = 0;
			if (judgeNot(point[0] + 1, point[1], id)) {  //在某位置不能下落时
				update();  //更新方块信息
				id = nextID;
				reDraw(1, 17, nextID);
				nextID = rand() % 19;

				point[0] = 0;  //重新生成新的俄罗斯方块
				point[1] = 5;
				draw(point[0], point[1], id);
				draw(1, 17, nextID);

				if (judgeNot(point[0], point[1], id)) {  //无法在初始位置放方块，游戏进程结束
					settlement();  //进入结算页面
					return;
				}
			}
			else {//继续下落
				reDraw(point[0], point[1], id);  //擦除原来的图形
				point[0]++;
				draw(point[0], point[1], id);  //下降一格后绘制图形
			}
		}

		if (_kbhit()) {  //键盘输入值
			int key, key2;  //检测“上下左右”时需设立两个字符收集信息
			key = _getch();
			if (key == 224) {
				key2 = _getch();
				if (key2 == 72) {  //按向上方向键时
					int temp = id;
					reDraw(point[0], point[1], id);
					turn(id);  //转向
					if (judgeNot(point[0], point[1], id))  //无法转向
						id = temp;
					draw(point[0], point[1], id);
				}
				else if (key2 == 80) {  //按向下方向键时
					if (!(judgeNot(point[0] + 1, point[1], id))) {
						reDraw(point[0], point[1], id);
						point[0] += 1;  //直接下降一个
						draw(point[0], point[1], id);
					}
				}
				else if (key2 == 75) {  //按向左方向键时
					if (!judgeNot(point[0], point[1] - 1, id)) {
						reDraw(point[0], point[1], id);
						point[1] -= 1;  //左移一格
						draw(point[0], point[1], id);
					}
				}
				else if (key2 == 77) {  //按向右方向键时
					if (!judgeNot(point[0], point[1] + 1, id)) {
						reDraw(point[0], point[1], id);
						point[1] += 1;  //右移一格
						draw(point[0], point[1], id);
					}
				}
			}
			else if (key == 32)  //按下空格键时
				pause();
		}
		Sleep(1);  //等待1ms
		++i;	//控制间隔
	}
}

void Tetris::settlement() {
	system("cls");
	setColor(4);
	int ranking = 0;
	for (int i = 0; i < playerCount; ++i) {  //获得此次游戏排名
		if (score < players[i].score)
			++ranking;
		else
			break;
	}//安插新的玩家数据
	for (int i = playerCount; i > ranking; --i) {  
		players[i] = players[i - 1];
		players[i].rank = i + 1;
	}
	++playerCount;
	players[ranking].rank = ranking + 1;
	players[ranking].score = score;
	originalPoint[0] = 12;
	originalPoint[1] = 24;
	setPosition(-4, -2);
	cout << "■■■";
	setPosition(-4, -1);
	cout << "■";
	setPosition(20, 5);
	cout << "■";
	setPosition(16, 6);
	cout << "■■■";
	setPosition(0, 0);
	cout << "游戏结束";
	setPosition(0, 1);
	cout << "你的分数：" << score;
	setPosition(0, 2);
	cout << "你的排名" << ranking + 1;
	setPosition(0, 3);
	cout << "请输入你的名字：";
	setPosition(6, 4);
	char name[10];
	cin >> name;
	strcpy(players[ranking].name, name);
	save();  //覆写玩家数据
	clearing();  //初始化
	load();  //重载玩家数据
	welcome();  //回到欢迎界面
}

void Tetris::clearing() {  //初始化玩家数据
	memset(map, 0, sizeof(map));
	int originalPoint[2] = { 0,0 };
	speedUp = false;
	rank = 1;
	point[0] = 0;
	point[1] = 5;
	score = 0;
	top = 25;
}

void Tetris::update() {  //更新图形数据
	int i, flag;
	int nx, ny;
	setColor(0);
	for (i = 0; i < 4; ++i) {  //固定图形
		nx = point[0] + sharp[id][2 * i];
		ny = point[1] + sharp[id][2 * i + 1];
		setPosition((ny + 1) * 2, nx + 1);
		cout << "■";
		map[nx][ny] = 1;  //标记(nx,ny)处已打印方块
	}

	if (point[0] < top)
		top = point[0];  //更新最高点
	for (i = point[0]; i < point[0] + high[id]; ++i) {  //扫描high[id]层数的方块，从point[0]开始网上扫
		flag = 1;
		for (int j = 0; j < 13; ++j)  //检测有无铺满一层方块
			if (map[i][j] == 0) {
				flag = 0;
				break;
			}
		if (flag == 1) {
			for (int k = i; k >= top; --k) {
				for (int p = 0; p < 13; ++p) {
					map[k][p] = map[k - 1][p];  //把上一层的方块信息复制下来
					setPosition((p + 1) * 2, k + 1);
					if (map[k][p] == 1)
						cout << "■";
					else
						cout << "  ";
				}
			}  //根据speedUp修改下落时间系数count
			if (speedUp) {
				if (count > 20) {
					count -= 10;
				}
			}  //根据rank选择加分
			switch (rank) {
			case 1:score += 7; break;
			case 2:score += 11; break;
			case 3:score += 15; break;
			case 0:score += 1600 / count; break;
			}
			setColor(3);
			setPosition(23, -1);
			cout << score;
			setColor(0);
		}
	}
}

void Tetris::pause() {  //暂停游戏
	setColor(1);
	setPosition(38, 21);
	cout << "  ";
	setPosition(40, 22);
	cout << "  ";
	setPosition(42, 23);
	cout << "  ";
	char temp;
	while (1) {
		temp = _getch();
		if (temp == 32)  //检测空格
			break;
	}
	setPosition(38, 21);
	cout << "■";
	setPosition(40, 22);
	cout << "■";
	setPosition(42, 23);
	cout << "■";
}

void Tetris::turn(int num) {  //旋转图形
	switch (num) {
	case a1:id = a2; break;  //I形互换
	case a2:id = a1; break;

	case b:id = b; break;  //O形不变

	case c1:id = c2; break;  //J形互换
	case c2:id = c3; break;
	case c3:id = c4; break;
	case c4:id = c1; break;

	case d1:id = d2; break;  //L形互换
	case d2:id = d3; break;
	case d3:id = d4; break;
	case d4:id = d1; break;

	case e1:id = e2; break;  //Z形互换
	case e2:id = e1; break;

	case f1:id = f2; break;  //S形互换
	case f2:id = f1; break;

	case g1:id = g2; break;  //T形互换
	case g2:id = g3; break;
	case g3:id = g4; break;
	case g4:id = g1; break;
	}
}

void Tetris::reDraw(int x, int y, int num) {
	int nx, ny;
	for (int i = 0; i < 4; ++i) {
		nx = x + sharp[num][2 * i];
		ny = y + sharp[num][2 * i + 1];
		setPosition((ny + 1) * 2, nx + 1);
		cout << "  ";
	}
}

bool Tetris::judgeNot(int x, int y, int num)
{//判断在(x,y)的位置是否可画出编号位num的图形，若不可画则返回true
	int nx, ny;
	for (int i = 0; i < 4; ++i)
	{
		nx = x + sharp[num][2 * i];
		ny = y + sharp[num][2 * i + 1];
		if (!(nx < 25 && nx >= 0 && ny < 13 && ny >= 0 && !map[nx][ny]))  //判断所有坐标是否在规范内且未打印
			return true;
	}
	return false;
}

void Tetris::draw(int x, int y, int num)
{//绘制俄罗斯方块
	setColor(rand() % 4 + 1);  //闪烁效果
	int nx, ny;
	for (int i = 0; i < 4; ++i)
	{  //根据sharp数组确立俄罗斯方块坐标并依次打印
		nx = x + sharp[num][2 * i];
		ny = y + sharp[num][2 * i + 1];
		setPosition((ny + 1) * 2, nx + 1);
		//setColor(i + 1);  //碎花
		cout << "■";
	}
}

void save()
{  //将玩家数据存储进文件
	int i;
	FILE* fp;
	fp = fopen("database.txt", "w");
	for (i = 0; i < playerCount; i++)
	{
		fprintf(fp, "%d\n", players[i].rank);
		fprintf(fp, "%s\n", players[i].name);
		fprintf(fp, "%d\n", players[i].score);
	}
	fclose(fp);
}

void load()
{//从文件中读取玩家数据进players数组
	FILE* fp;
	fp = fopen("database.txt", "r");
	playerCount = 0;
	while ((fscanf(fp, "%d", &players[playerCount].rank)) != EOF)
	{
		fscanf(fp, "%s", players[playerCount].name);
		fscanf(fp, "%d", &players[playerCount].score);
		playerCount++;
	}
	fclose(fp);
}

int main()
{
	modeSet(40, 36 * 2);  //调整控制台窗口大小
	SetConsoleTitle("Tetris");  //修改控制台窗口标题
	Tetris game;
	load();
	game.welcome();
	system("cls");
}