#ifndef _UTIL_H_
#define _UTIL_H_

#include <graphics.h>

#pragma comment(lib,"WINMM.lib")//链接 Windows 多媒体库
#pragma comment(lib,"MSIMG32.lib")//链接 Microsoft 图像处理库

struct Rect
{
    int x, y;//左上角坐标位置
    int w, h;//矩形在x,y方向上的长度
};

//使用 AlphaBlend 函数将源图像的一部分绘制到目标图像的指定位置，支持透明度混合
inline void putimage_ex(IMAGE* img, const Rect* rect_dst, const Rect* rect_src = nullptr)
{
    static BLENDFUNCTION blend_func = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };//混合函数，定义了如何将源图像与目标图像混合
    AlphaBlend(
        GetImageHDC(GetWorkingImage()),  // 目标设备上下文
        rect_dst->x, rect_dst->y, rect_dst->w, rect_dst->h,  // 目标矩形的坐标和尺寸
        GetImageHDC(img),  // 源图像的设备上下文
        rect_src ? rect_src->x : 0, rect_src ? rect_src->y : 0,  // 源矩形的坐标
        rect_src ? rect_src->w : img->getwidth(), rect_src ? rect_src->h : img->getheight(),  // 源矩形的尺寸
        blend_func  // 混合函数
    );
}

inline void load_audio(LPCTSTR path, LPCTSTR id)
{
    static TCHAR str_cmd[512];//// 定义一个静态字符数组来存储命令字符串
    _stprintf_s(str_cmd, _T("open %s alias %s"), path, id);// 构建命令字符串，格式为 "open [path] alias [id]"
    mciSendString(str_cmd, NULL, 0, NULL);// 使用 mciSendString 发送命令，加载音频文件
}
inline void play_audio(LPCTSTR id, bool is_loop = false)
{
    static TCHAR str_cmd[512];
    // 构建命令字符串，格式为 "play [id] [repeat] from 0"
    // 如果 is_loop 为 true，则添加 "repeat" 参数
    _stprintf_s(str_cmd, _T("play %s %s from 0"), id, is_loop ? _T("repeat") : _T(""));
    mciSendString(str_cmd, NULL, 0, NULL);
}
inline void stop_audio(LPCTSTR id)
{
    static TCHAR str_cmd[512];
    _stprintf_s(str_cmd, _T("stop %s"), id);
    mciSendString(str_cmd, NULL, 0, NULL);
}
inline int range_random(int min, int max)
{
    return min + rand() % (max - min + 1);
}
#endif