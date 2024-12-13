#ifndef _BULLET_TIME_MANAGER_H_
#define _BULLET_TIME_MANAGER_H_

class BulletTimeManager
{
public:
    enum class Status
    {
        Entering,
        Exiting
    };
public:
    static BulletTimeManager* instance();

    void post_process();
    void set_status(Status status);
    float on_update(float delta);

private:
    static BulletTimeManager* manager;
    BulletTimeManager();
    ~BulletTimeManager();

    float lerp(float start, float end, float t)
    {
        /*�� t Ϊ0ʱ������ start ֵ���� t Ϊ1ʱ������ end ֵ
        (1 - t) * start ����� start �� end ֮������ʣ�ಿ��
        t * end ����� start �� end ֮���Ѿ��߹��Ĳ���
        ��������������ӣ��õ���ֵ��Ľ��*/
        return (1 - t) * start + t * end;
    }
private:
    float progress = 0;
    Status status = Status::Exiting;
    const float SPEED_PROGRESS = 2.0f;
    const float DST_DELTA_FACTOR = 0.35f;//֡����ʱ�������
    const float DST_COLOR_FACTOR = 0.5f;//����ɫ�ʸı����
};
#endif // !_BULLET_TIME_MANAGER_H_
