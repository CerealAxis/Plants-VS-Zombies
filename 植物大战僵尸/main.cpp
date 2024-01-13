#include <stdio.h>
#include <graphics.h>
#include "tools.h"

#define WIN_WIDTH 900
#define WIN_HEIGHT 600

enum { WAN_DOU, XIANG_RI_KUI, ZHI_WU_COUNT };

IMAGE imgBg;
IMAGE imgBar;
IMAGE imgCards[ZHI_WU_COUNT];
IMAGE* imgZhiWu[ZHI_WU_COUNT][20];

int curX, curY;//当前选中植物运动过程中的x,y坐标
int curZhiWu;//0:无选择 1:第一种

bool fileExist(const char* name) {
	FILE* fp = fopen(name, "r");
	if (fp == NULL) {
		return false;
	}
	else {
		fclose(fp);
		return true;
	}
}

void gameInit() {
	//字符集修改为了多字节字符集
	loadimage(&imgBg, "res/res/bg.jpg");
	loadimage(&imgBar, "res/res/bar5.png");

	memset(imgZhiWu, 0, sizeof(imgZhiWu));

	//初始化植物卡牌
	char name[64];
	for (int i = 0; i < ZHI_WU_COUNT; i++) {
		sprintf_s(name, sizeof(name), "res/res/Cards/card_%d.png", i + 1);
		loadimage(&imgCards[i], name);

		for (int j = 0; j < 20; j++) {
			sprintf_s(name, sizeof(name), "res/res/zhiwu/%d/%d.png", i ,j+1);
			if (fileExist(name)) {
				imgZhiWu[i][j] = new IMAGE;
				loadimage(imgZhiWu[i][j], name);
			}
			else {
				break;
			}
		}
	}
	curZhiWu = 0;
	//创建游戏窗口
	initgraph(WIN_WIDTH, WIN_HEIGHT, 1);
}

void updateWindow() {
	BeginBatchDraw();

	putimage(0, 0, &imgBg);//（0，0）是放置背景图的坐标
	putimagePNG(250, 0, &imgBar);//加载工具栏

	//渲染卡牌
	for (int i = 0; i < ZHI_WU_COUNT; i++) {
		int x = 338 + i * 65;
		int y = 6;
		putimage(x, y, &imgCards[i]);
	}

	//渲染拖动过程
	if (curZhiWu>0) {
		IMAGE* img = imgZhiWu[curZhiWu - 1][0];
		putimagePNG(curX-img->getwidth()/2.0, curY-img->getheight()/2.0, img);
	}

	EndBatchDraw();
}

void userClick() {
	ExMessage msg;
	static int status = 0;
	if (peekmessage(&msg)) {
		if (msg.message == WM_LBUTTONDOWN) {
			if (msg.x > 338 && msg.x < 338 + 65 * ZHI_WU_COUNT && msg.y < 96) {
				int index = (msg.x - 338) / 65;
				status = 1;
				curZhiWu = index + 1;
			}
		}
		else if (msg.message == WM_MOUSEMOVE && status == 1) {
			curX = msg.x;
			curY = msg.y;
		}
		else if (msg.message == WM_LBUTTONUP) {

		}
	}
}

int main(void) {
	gameInit();
	while (1) {
		userClick();
		updateWindow();
	}
	system("pause");
	return 0;
}