#include "tools.h"

// 函数：将PNG图像透明贴图到当前画布
void _putimagePNG(int picture_x, int picture_y, IMAGE* picture) // x为在画布上的X坐标，y为Y坐标
{
    DWORD* dst = GetImageBuffer();    // 获取画布的像素数据指针，EASYX库提供的函数
    DWORD* draw = GetImageBuffer();
    DWORD* src = GetImageBuffer(picture); // 获取图像的像素数据指针
    int picture_width = picture->getwidth(); // 获取图像宽度，EASYX库提供的函数
    int picture_height = picture->getheight(); // 获取图像高度，EASYX库提供的函数
    int graphWidth = getwidth();       // 获取画布宽度，EASYX库提供的函数
    int graphHeight = getheight();     // 获取画布高度，EASYX库提供的函数
    int dstX = 0;    // 目标图像的X坐标

    // 实现透明贴图算法：Cp = αp * Fp + (1 - αp) * Bp，其中αp为像素的透明度
    for (int iy = 0; iy < picture_height; iy++)
    {
        for (int ix = 0; ix < picture_width; ix++)
        {
            int srcX = ix + iy * picture_width; // 计算图像像素在数组中的索引
            int sa = ((src[srcX] & 0xff000000) >> 24); // 0xAArrggbb; AA为透明度
            int sr = ((src[srcX] & 0xff0000) >> 16); // 获取RGB中的R
            int sg = ((src[srcX] & 0xff00) >> 8);   // G
            int sb = src[srcX] & 0xff;              // B
            if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
            {
                dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; // 计算目标图像像素在数组中的索引
                int dr = ((dst[dstX] & 0xff0000) >> 16);
                int dg = ((dst[dstX] & 0xff00) >> 8);
                int db = dst[dstX] & 0xff;
                draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)
                    | ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)
                    | (sb * sa / 255 + db * (255 - sa) / 255);
            }
        }
    }
}

// 函数：将PNG图像透明贴图到当前画布，处理图像边界情况
void putimagePNG(int x, int y, IMAGE* picture) {

    IMAGE imgTmp, imgTmp2, imgTmp3;
    int winWidth = getwidth();
    int winHeight = getheight();

    // 处理图像上溢出的情况
    if (y < 0) {
        SetWorkingImage(picture);
        getimage(&imgTmp, 0, -y, picture->getwidth(), picture->getheight() + y);
        SetWorkingImage();
        y = 0;
        picture = &imgTmp;
    }
    // 处理图像下溢出或x超出画布宽度的情况
    else if (y >= getheight() || x >= getwidth()) {
        return; // 不进行贴图操作
    }
    // 处理图像下溢出的情况
    else if (y + picture->getheight() > winHeight) {
        SetWorkingImage(picture);
        getimage(&imgTmp, x, y, picture->getwidth(), winHeight - y);
        SetWorkingImage();
        picture = &imgTmp;
    }

    // 处理图像左溢出的情况
    if (x < 0) {
        SetWorkingImage(picture);
        getimage(&imgTmp2, -x, 0, picture->getwidth() + x, picture->getheight());
        SetWorkingImage();
        x = 0;
        picture = &imgTmp2;
    }

    // 处理图像右溢出的情况
    if (x > winWidth - picture->getwidth()) {
        SetWorkingImage(picture);
        getimage(&imgTmp3, 0, 0, winWidth - x, picture->getheight());
        SetWorkingImage();
        picture = &imgTmp3;
    }

    // 调用透明贴图函数
    _putimagePNG(x, y, picture);
}

// 函数：获取两次调用之间的时间间隔
int getDelay() {
    static unsigned long long lastTime = 0;
    unsigned long long currentTime = GetTickCount();
    if (lastTime == 0) {
        lastTime = currentTime;
        return 0;
    }
    else {
        int ret = currentTime - lastTime;
        lastTime = currentTime;
        return ret;
    }
}
