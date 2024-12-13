#include "bullet_time_manager.h"
#include <graphics.h>

BulletTimeManager* BulletTimeManager::manager = nullptr;

BulletTimeManager* BulletTimeManager::instance()
{
    if (!manager)
        manager = new BulletTimeManager();
    return manager;
}

void BulletTimeManager::post_process()
{
    DWORD* buffer = GetImageBuffer();//指向图像缓冲区的指针,存储图像的像素数据
    int w = getwidth(), h = getheight();
    for (size_t y = 0; y < h; y++)
        for (size_t x = 0; x < w; x++)
        {
            int index = y * w + x;//计算当前像素在缓冲区中的索引
            DWORD color = buffer[index];//当前像素的颜色值
            BYTE r = (BYTE)(GetBValue(color) * lerp(1.0f, DST_COLOR_FACTOR, progress));
            BYTE g = (BYTE)(GetGValue(color) * lerp(1.0f, DST_COLOR_FACTOR, progress));
            BYTE b = (BYTE)(GetRValue(color) * lerp(1.0f, DST_COLOR_FACTOR, progress));
            /*通过 RGB(r, g, b) 创建了一个包含红、绿、蓝分量的颜色值，然后通过按位或操作符将不透明的Alpha值合并到这个颜色值中
            将 255 转换为 BYTE 类型，然后再转换为 DWORD 类型。这样做的目的是为了确保在进行位操作时，数值不会丢失
            将 255（即不透明）向左移动24位。在32位颜色值（DWORD）中，最高8位（从右边数第32位到第25位）通常用于存储透明度信息。通过左移24位将 255 放到了Alpha通道的位置*/
            buffer[index] = BGR(RGB(r, g, b) | (((DWORD)(BYTE)(255)) << 24));
        }
}

void BulletTimeManager::set_status(Status status)
{
    this->status = status;
}
float BulletTimeManager::on_update(float delta)
{
    float delta_progress = SPEED_PROGRESS * delta;
    progress += delta_progress * (status == Status::Entering ? 1 : -1);

    if (progress < 0) progress = 0;
    if (progress > 1.0f) progress = 1.0f;

    return delta * lerp(1.0f, DST_DELTA_FACTOR, progress);
}

BulletTimeManager::BulletTimeManager() = default;
BulletTimeManager::~BulletTimeManager() = default;