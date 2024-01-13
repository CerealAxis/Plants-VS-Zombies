#include <stdio.h>
#include <graphics.h>
#include "tools.h"

#define WIN_WIDTH 900
#define WIN_HEIGHT 600

enum {WAN_DOU,XIANG_RI_KUI,ZHI_WU_COUNT};

IMAGE imgBg;//背景图片
IMAGE imgBar;
IMAGE imgCards[ZHI_WU_COUNT];

void gameInit() {
	//字符集修改为了多字节字符集
	loadimage(&imgBg,"res/res/bg.jpg");
	loadimage(&imgBar, "res/res/bar5.png");

	//初始化植物卡牌
	char name[64];
	for (int i = 0; i < ZHI_WU_COUNT; i++) {
		sprintf_s(name, sizeof(name), "res/res/Cards/card_%d.png", i+1);
		loadimage(&imgCards[i], name);
	}

	//创建游戏窗口
	initgraph(WIN_WIDTH, WIN_HEIGHT);
}

void updateWindow() {
	putimage(0, 0, &imgBg);//（0，0）是放置背景图的坐标
	putimagePNG(250, 0, &imgBar);//加载工具栏

	//渲染卡牌
	for (int i = 0; i < ZHI_WU_COUNT; i++) {
		int x = 338 + i * 65;
		int y = 6;
		putimage(x, y, &imgCards[i]);
	}
}

int main(void) {
	gameInit();

	updateWindow();

	system("pause");
	return 0;
}