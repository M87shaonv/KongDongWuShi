#ifndef _UTIL_H_
#define _UTIL_H_

#include <graphics.h>

#pragma comment(lib,"WINMM.lib")//���� Windows ��ý���
#pragma comment(lib,"MSIMG32.lib")//���� Microsoft ͼ�����

struct Rect
{
    int x, y;//���Ͻ�����λ��
    int w, h;//������x,y�����ϵĳ���
};

//ʹ�� AlphaBlend ������Դͼ���һ���ֻ��Ƶ�Ŀ��ͼ���ָ��λ�ã�֧��͸���Ȼ��
inline void putimage_ex(IMAGE* img, const Rect* rect_dst, const Rect* rect_src = nullptr)
{
    static BLENDFUNCTION blend_func = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };//��Ϻ�������������ν�Դͼ����Ŀ��ͼ����
    AlphaBlend(
        GetImageHDC(GetWorkingImage()),  // Ŀ���豸������
        rect_dst->x, rect_dst->y, rect_dst->w, rect_dst->h,  // Ŀ����ε�����ͳߴ�
        GetImageHDC(img),  // Դͼ����豸������
        rect_src ? rect_src->x : 0, rect_src ? rect_src->y : 0,  // Դ���ε�����
        rect_src ? rect_src->w : img->getwidth(), rect_src ? rect_src->h : img->getheight(),  // Դ���εĳߴ�
        blend_func  // ��Ϻ���
    );
}

inline void load_audio(LPCTSTR path, LPCTSTR id)
{
    static TCHAR str_cmd[512];//// ����һ����̬�ַ��������洢�����ַ���
    _stprintf_s(str_cmd, _T("open %s alias %s"), path, id);// ���������ַ�������ʽΪ "open [path] alias [id]"
    mciSendString(str_cmd, NULL, 0, NULL);// ʹ�� mciSendString �������������Ƶ�ļ�
}
inline void play_audio(LPCTSTR id, bool is_loop = false)
{
    static TCHAR str_cmd[512];
    // ���������ַ�������ʽΪ "play [id] [repeat] from 0"
    // ��� is_loop Ϊ true������� "repeat" ����
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