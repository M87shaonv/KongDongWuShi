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
    DWORD* buffer = GetImageBuffer();//ָ��ͼ�񻺳�����ָ��,�洢ͼ�����������
    int w = getwidth(), h = getheight();
    for (size_t y = 0; y < h; y++)
        for (size_t x = 0; x < w; x++)
        {
            int index = y * w + x;//���㵱ǰ�����ڻ������е�����
            DWORD color = buffer[index];//��ǰ���ص���ɫֵ
            BYTE r = (BYTE)(GetBValue(color) * lerp(1.0f, DST_COLOR_FACTOR, progress));
            BYTE g = (BYTE)(GetGValue(color) * lerp(1.0f, DST_COLOR_FACTOR, progress));
            BYTE b = (BYTE)(GetRValue(color) * lerp(1.0f, DST_COLOR_FACTOR, progress));
            /*ͨ�� RGB(r, g, b) ������һ�������졢�̡�����������ɫֵ��Ȼ��ͨ����λ�����������͸����Alphaֵ�ϲ��������ɫֵ��
            �� 255 ת��Ϊ BYTE ���ͣ�Ȼ����ת��Ϊ DWORD ���͡���������Ŀ����Ϊ��ȷ���ڽ���λ����ʱ����ֵ���ᶪʧ
            �� 255������͸���������ƶ�24λ����32λ��ɫֵ��DWORD���У����8λ�����ұ�����32λ����25λ��ͨ�����ڴ洢͸������Ϣ��ͨ������24λ�� 255 �ŵ���Alphaͨ����λ��*/
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