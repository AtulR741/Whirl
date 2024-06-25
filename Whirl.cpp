#include <bits/stdc++.h>
#include <graphics.h>
#include <windows.h>
#include <ctime>
using namespace std;

enum plattype{PLAIN = 0, CLOUD, ICE, MOVING, SPIKE, PROP, TRAMPOLINE, BLINK};
enum direction{UP = 0, DOWN};
int upvel = 0, test = 0, gravity = 1, cloc = 0, bounce = 15, twiceclock = 0, highscore;
float score = 0;
char Score[20], Highscore[20];
POINT pos, temppos;

bool operator != (POINT pos, POINT temppos)
{
	if(pos.x != temppos.x)
		return true;
	else if(pos.y != temppos.y)
		return true;
	else
		return false;
}

class destroy
{
	public:
		int x, uy, ly, cloc = 20, vel = 0;
		destroy(int X, int Y)
		{
			x = X;
			uy = Y;
			ly = Y;
		}
		void Draww()
		{
			vel += gravity;
			uy += vel;
			ly += vel + 1;
			cloc--;
			setfillstyle(SOLID_FILL, 7);
			bar(x, uy - 9, x + 20, uy - 5);
			bar(x, ly - 4, x + 40, ly);
			bar(x + 25, uy - 9, x + 59, uy - 5);
			bar(x + 45, ly - 4, x + 59, ly);
		}
};
list<destroy> d;

class collision
{
	public:
		int xlu, xru, ylu, yru, cloc = 20;
		collision(int X, int Y)
		{
			xlu = X, xru = X;
			ylu = Y, yru = Y;
		}
		void Draww(int y)
		{
			bar(xlu, ylu, xlu + 3, ylu + 3);
			bar(xru, yru, xru + 3, yru + 3);
			xlu -= 2, ylu -= 2;
			xru += 2, yru -= 2;
			if(y < 400 && upvel < 0)
				ylu -= upvel, yru -= upvel;
			cloc--;
		}
};
list<collision> c;

class ball
{
	public:
		int x = 700, y = 600, proptime = 0;
		direction d = DOWN;
		void Draww()
		{
			if(x <= 450)
				x = 1080;
			else if(x >= 1080)
				x = 450;
			test = (test + 1) % 3;
			if(test == 0 && upvel < 14)
				upvel += gravity;
			if(cloc == 0)
			{
				if(y >= 400)
					y += upvel;
				else if(upvel >= 0)
					y += upvel;
			}
			if(upvel > 0)
				d = DOWN;
			setcolor(BLACK);
			if(cloc == 0)
			{
				circle(x, y, 10);
				setfillstyle(SOLID_FILL, BLACK);
				floodfill(x, y, BLACK);
			}
			else
			{
				arc(x, y, 180, 0, 10);
				line(x - 10, y, x + 10, y);
				setfillstyle(SOLID_FILL, BLACK);
				floodfill(x, y + 4, BLACK);
				setcolor(RED);
				arc(x, y, 0, 180, 10);
				line(x - 10, y, x + 10, y);
				setfillstyle(SOLID_FILL, RED);
				floodfill(x, y - 4, RED);
				proptime = (proptime + 1) % 5;
				if(proptime == 0)
				{
					setfillstyle(SOLID_FILL, YELLOW);
					bar(x - 10, y - 10, x + 10, y - 15);
				}
			}
		}
};
ball b;

class platform
{
	public:
		int x, y, dir, plus = 0, twice = 0, blink = 0;
		plattype type;
		
		platform(int Y)
		{
			y = Y;
			x = (rand() % 569) + 451;
			type = PLAIN;
		}
		platform(int Y, int X)
		{
			y = Y;
			x = X;
			type = PLAIN;
		}
		void Reset()
		{
			y = y - 837;
			x = (rand() % 569) + 451;
			int random = rand() % 56;
			if(random < 15)
				type = PLAIN;
			else if(random < 25)
				type = ICE;
			else if(random < 35)
			{
				type = MOVING;
				dir = (rand() % 3) - 1;
				while(dir == 0)
					dir = (rand() % 3) - 1;
			}
			else if(random < 40)
				type = TRAMPOLINE;
			else if(random < 42)
				type = CLOUD;
			else if(random < 45)
				type = SPIKE;
			else if(random < 55)
				type = BLINK;
			else
			{
				type = PROP;
				blink = 1;
			}
			random = rand() % 200;
			if(random == 0)
				twice = 1;
			if(twice == 0)
			{
				random = rand() % 50;
				if(random == 0)
					plus = 1;
			}
		}
		void Draww()
		{
			setcolor(BLACK);
			if(cloc > 0)
			{
				y += 20;
				if(twiceclock > 0)
				{
					twiceclock--;
					score += 0.2;
				}
				else
					score += 0.1;
			}
			else if(b.y < 400 && upvel < 0)
			{
				y -= upvel;
				if(twiceclock > 0)
				{
					twiceclock--;
					score += 0.2;
				}
				else
					score += 0.1;
			}
			switch(type)
			{
				case(PLAIN):
					setfillstyle(SOLID_FILL, BLACK);
					bar(x, y - 9, x + 59, y);
					break;
				case(BLINK):
					blink = (blink + 1) % 45;
					if(blink < 15)
						setcolor(WHITE);
					else if(blink < 30)
						setcolor(LIGHTGRAY);
					else
						setcolor(DARKGRAY);
					rectangle(x, y - 9, x + 59, y);
					break;
				case(CLOUD):
					arc(x, y - 5, 90, 270, 5);
					arc(x + 59, y - 5, 270, 90, 5);
					for(int i = 1; i <= 11; i = i + 2)
					{
						arc(x - 1 + i * 5, y - 9, 0, 180, 5);
					}
					line(x, y, x + 59, y);
					break;
				case(ICE):
					setfillstyle(SOLID_FILL, 7);
					bar(x, y - 9, x + 59, y);
					break;
				case(MOVING):
					if(x == 450 || x == 1020)
						dir = -dir;
					x += dir;
					line(x + 4, y, x + 55, y);
					line(x + 4, y - 9, x + 55, y - 9);
					arc(x + 4, y - 4, 90, 270, 5);
					arc(x + 55, y - 4, 270, 90, 5);
					break;
				case(SPIKE):
					line(x, y, x + 59, y);
					line(x, y, x, y - 4);
					line(x + 59, y, x + 59, y - 4);
					for(int i = 0; i <= 10; i = i + 2)
					{
						line(x - 1 + i * 5, y - 4, x + 4 + i * 5, y - 9);
						line(x + 4 + i * 5, y - 9, x + 9 + i * 5, y - 4);
					}
					setfillstyle(SOLID_FILL, BLACK);
					floodfill(x + 1, y - 1, BLACK);
					break;
				case(PROP):
					setfillstyle(SOLID_FILL, BLACK);
					bar(x, y - 4, x + 59, y);
					if(blink == 1)
					{
						setcolor(RED);
						arc(x + 29, y - 4, 0, 180, 10);
						line(x + 19, y - 4, x + 39, y - 4);
						setfillstyle(SOLID_FILL, RED);
						floodfill(x + 29, y - 5, RED);
						setfillstyle(SOLID_FILL, YELLOW);
						bar(x + 19, y - 14, x + 39, y - 19);
					}
					break;
				case(TRAMPOLINE):
					rectangle(x, y - 9, x + 59, y - 6);
					rectangle(x, y - 3, x + 59, y);
					rectangle(x + 24, y - 6, x + 34, y - 3);
					setfillstyle(SOLID_FILL, BLACK);
					floodfill(x + 1, y - 1, BLACK);
					floodfill(x + 1, y - 7, BLACK);
					floodfill(x + 25, y - 4, BLACK);
					break;
			}
			if(plus == 1)
			{
				setcolor(WHITE);
				//settextstyle(5, 0, 1);
				outtextxy(x + 10, y - 40, "+500");
			}
			else if(twice == 1)
			{
				setcolor(WHITE);
				//settextstyle(5, 0, 3);
				outtextxy(x + 10, y - 40, "x2");
			}
			if(y > 840)
				Reset();
		}
};

int main(int argc, char** argv) {
	initwindow(1920, 1080, "Bounce", -3, -3, true, true);
	int page = 0, choice = -100;
	fstream file;
	file.open("high score", ios :: in);
	file >> highscore;
	file.close();
	srand(time(NULL));
	settextstyle(0, 0, 0);
	map<int, int> m;
	m[0] = 37, m[1] = 39;
	GetCursorPos(&pos);
	temppos = pos;
	while(1)
	{
    setvisualpage(page);
    setactivepage(1 - page);
    cleardevice();
    settextstyle(0, 0, 5);
    outtextxy(600, 100, "BOUNCE");
    settextstyle(0, 0, 0);
    outtextxy(600, 300, "Play");
   // outtextxy(600, 400, "Change Controls");
    outtextxy(600, 500, "Credit Details");
    outtextxy(600, 600, "Quit");
    outtextxy(500, 200, "Use your mouse to navigate through the menu...");
    outtextxy(1400, 600, "Atul Raj");
    outtextxy(1400, 650, "21JE0195");
    GetCursorPos(&pos);
    if(temppos != pos && pos.x >= 600 && pos.x <= 700)
    {
    	if(pos.y >= 300 && pos.y <= 350)
    		choice = 0;
    	//else if(pos.y >= 400 && pos.y <= 450)
    	//	choice = 1;
    	else if(pos.y >= 500 && pos.y <= 550)
    		choice = 2;
    	else if(pos.y >= 600 & pos.y <= 650)
    		choice = 3;
    }
   /* if(GetAsyncKeyState(VK_UP))
    {
    	choice--;
    	if(choice == -1)
    		choice = 3;
	}
	else if(GetAsyncKeyState(VK_DOWN))
		choice = (choice + 1) % 4;*/
    line(500, 285 + 100*choice, 500, 335 + 100*choice);
    line(500, 285 + 100*choice, 550, 310 + 100*choice);
    line(500, 335 + 100*choice, 550, 310 + 100*choice);
	if(GetAsyncKeyState(VK_LBUTTON) && choice == 0)
	{
		setvisualpage(page);
		setactivepage(page);
		cleardevice();
		outtextxy(600, 500, "Loading...");
		delay(1000);
		start:
		upvel = 0, test = 0, cloc = 0, score = 0, b.x = 700, b.y = 600, b.d = DOWN, twiceclock = 0;
		platform plat[9] = {platform(93), platform(186), platform(279), platform(372), platform(465), platform(558), 
							platform(651), platform(744), platform(837, 650)};
	while(b.y < 850)
	{
		setvisualpage(page);
		setactivepage(1 - page);
		
		//Setup
		cleardevice();
		setcolor(WHITE);
		rectangle(450, 0, 1080, 840);
		setfillstyle(SOLID_FILL, WHITE);
		floodfill(451, 3, WHITE);
		if(cloc > 0)
		{
			upvel = 0;
			gravity = 0;
			cloc--;
			if(cloc == 0)
				upvel = -20;
		}
		else
			gravity = 1;
		
		//Ball
		if(GetAsyncKeyState(m[1]))
		{
			b.x += 5;
		}
		else if(GetAsyncKeyState(m[0]))
		{
			b.x -= 5;
		}
		b.Draww();
		
		int icex, icey, add = 0;
		
		//Destroy
		list<destroy> :: iterator iter;
		for(iter = d.begin(); iter != d.end(); iter++)
			iter->Draww();
		if(!d.empty())
			if((d.front()).cloc == 0)
				d.pop_front();
		
		//Collision
		list<collision> :: iterator iter2;
		for(iter2 = c.begin(); iter2 != c.end(); iter2++)
			iter2->Draww(b.y);
		if(!c.empty())
			if((c.front()).cloc == 0)
				c.pop_front();
		
		//Platforms
		for(int i = 0; i < 9; i++)
		{
			plat[i].Draww();
			
			if(b.d == DOWN)
			{
				if(b.y <= plat[i].y - 5 && b.y >= plat[i].y - 20 && b.x >= plat[i].x - 8 && b.x <= plat[i].x + 67)
				{
					if(plat[i].plus == 1)
					{
						plat[i].plus = 0;
						score += 500;
					}
					else if(plat[i].twice == 1)
					{
						plat[i].twice = 0;
						twiceclock += 5000;
					}
					switch(plat[i].type)
					{
						case(PLAIN):
							b.y = plat[i].y - 9;
							b.d = UP;
							upvel = -bounce;
							test = 0;
							add = 2;
							break;
						case(BLINK):
							b.y = plat[i].y - 9;
							b.d = UP;
							upvel = -bounce;
							test = 0;
							add = 2;
							break;
						case(MOVING):
							b.y = plat[i].y - 9;
							b.d = UP;
							upvel = -bounce;
							test = 0;
							add = 2;
							break;
						case(ICE):
							b.y = plat[i].y - 9;
							b.d = UP;
							upvel = -bounce;
							test = 0;
							add = 1;
							icex = plat[i].x, icey = plat[i].y;
							plat[i].Reset();
							break;
						case(CLOUD):
							plat[i].Reset();
							break;
						case(SPIKE):
							b.y = 850;
							break;
						case(TRAMPOLINE):
							b.y = plat[i].y - 9;
							b.d = UP;
							upvel = -30;
							test = 0;
							break;
						case(PROP):
							plat[i].blink = 0;
							b.y = plat[i].y - 9;
							b.d = UP;
							upvel = 0;
							cloc = 200;
							break;
					}
				}
			}
		}
		if(add == 1)
		{
			destroy temp(icex, icey);
			d.push_back(temp);
		}
		else if(add == 2)
		{
			collision temp(b.x, b.y);
			c.push_back(temp);
		}
		//Score
		int intscore = score;
		sprintf(Score, "%d", intscore);
		setcolor(WHITE);
	//	settextstyle(5, 0, 5);
		outtextxy(1, 1, Score);
		if(twiceclock > 0)
			outtextxy(1, 50, "x2");
		
		page = 1 - page;
		delay(1);
	}
	d.clear();
	c.clear();
	int choice2 = 0;
	while(1)
	{
	setvisualpage(page);
	setactivepage(1 - page);
	
	cleardevice();
	//settextstyle(3, 0, 5);
	outtextxy(500, 150, "Your Score : ");
	outtextxy(800, 150, Score);
	if(highscore >= score)
	{
		outtextxy(500, 200, "High Score : ");
		sprintf(Highscore, "%d", highscore);
		outtextxy(800, 200, Highscore);
	}
	else
	{
		fstream file;
		file.open("high score", ios :: out);
		highscore = score;
		file << highscore;
		file.close();
		outtextxy(500, 200, "New High Score!");
	}
	outtextxy(600, 500, "Try Again");
	outtextxy(600, 600, "Main Menu");
	GetCursorPos(&pos);
    if(pos != temppos && pos.x >= 600 && pos.x <= 700)
    {
    	if(pos.y >= 500 && pos.y <= 550)
    		choice2 = 0;
    	else if(pos.y >= 600 && pos.y <= 650)
    		choice2 = 1;
    }
    /*if(GetAsyncKeyState(VK_UP))
    {
    	choice2--;
    	if(choice2 == -1)
    		choice2 = 1;
	}
	else if(GetAsyncKeyState(VK_DOWN))
		choice2 = (choice2 + 1) % 2;*/
    line(500, 485 + 100*choice2, 500, 535 + 100*choice2);
    line(500, 485 + 100*choice2, 550, 510 + 100*choice2);
    line(500, 535 + 100*choice2, 550, 510 + 100*choice2);
	if(GetAsyncKeyState(VK_LBUTTON) && choice2 == 1)
	{
		setvisualpage(page);
		setactivepage(page);
		cleardevice();
		outtextxy(600, 500, "Please Wait...");
		delay(1000);
		break;
	}
	else if(GetAsyncKeyState(VK_LBUTTON) && choice2 == 0)
	{
		setvisualpage(page);
		setactivepage(page);
		cleardevice();
		outtextxy(600, 500, "Loading...");
		delay(1000);
		goto start;
	}
	temppos = pos;
	page = 1 - page;
	delay(100);
	}
	}
	else if(GetAsyncKeyState(VK_LBUTTON) && choice == 1)
	{
		setvisualpage(page);
		setactivepage(page);
		cleardevice();
		outtextxy(500, 500, "Press key for left");
		m[0] = getch();
		if(m[0] >= 97 && m[0] <= 122)
			m[0] -= 32;
		cleardevice();
		outtextxy(500, 500, "Assigning...");
		delay(1000);
		cleardevice();
		outtextxy(500, 500, "Press key for right");
		m[1] = getch();
		if(m[1] >= 97 && m[1] <= 122)
			m[1] -= 32;
		cleardevice();
		outtextxy(500, 500, "Assigning...");
		delay(1000);
	}
	else if(GetAsyncKeyState(VK_LBUTTON) && choice == 2)
	{
		setvisualpage(page);
		setactivepage(page);
		cleardevice();
		outtextxy(400, 500, "Created by - Atul Raj, IIT (ISM)");
		outtextxy(400, 550, "This application is to be distributed only under the creator's banner.");
		outtextxy(400, 600, "(c) Copyright 2023");
		outtextxy(400, 700, "Click left mouse button to go back...");
		delay(2000);
		while(1)
		{
			if(GetAsyncKeyState(VK_LBUTTON))
			{
				cleardevice();
				outtextxy(500, 500, "Please Wait...");
				delay(1000);
				break;
			}
		}
	}
	else if(GetAsyncKeyState(VK_LBUTTON) && choice == 3)
		exit(0);
	temppos = pos;
	page = 1 - page;
	delay(50);
	}
	return 0;
}
