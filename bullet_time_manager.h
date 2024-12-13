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
        /*当 t 为0时，返回 start 值；当 t 为1时，返回 end 值
        (1 - t) * start 计算从 start 到 end 之间距离的剩余部分
        t * end 计算从 start 到 end 之间已经走过的部分
        将上述两部分相加，得到插值后的结果*/
        return (1 - t) * start + t * end;
    }
private:
    float progress = 0;
    Status status = Status::Exiting;
    const float SPEED_PROGRESS = 2.0f;
    const float DST_DELTA_FACTOR = 0.35f;//帧更新时间的缩放
    const float DST_COLOR_FACTOR = 0.5f;//画面色彩改变比例
};
#endif // !_BULLET_TIME_MANAGER_H_
