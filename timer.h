#ifndef _TIMER_H_
#define _TIMER_H_

#include <functional>

class Timer
{
public:
    Timer() = default;
    ~Timer() = default;

    void restart()
    {
        pass_time = 0;
        shotted = false;
    }

    void set_wait_time(float val)
    {
        wait_time = val;
    }

    void set_one_shot(bool flag)
    {
        one_shot = flag;
    }

    // 设置当定时器超时时调用的回调函数
    void set_on_timeout(std::function<void()> on_timeout)
    {
        this->on_timeout = on_timeout;
    }

    void pause()
    {
        paused = true;
    }

    void resume()
    {
        paused = false;
    }

    void on_update(float delta)
    {
        if (paused) return;

        pass_time += delta;
        if (pass_time > wait_time)
        {
            // 判断是否可以触发定时器
            bool can_shot = (!one_shot || (one_shot && !shotted));
            shotted = true; // 标记已触发
            if (can_shot && on_timeout) // 如果可以触发且设置了回调
                on_timeout(); // 调用回调函数

            // 减去等待的时间间隔，以便于重复计时
            pass_time -= wait_time;
        }
    }

private:
    float pass_time = 0;
    float wait_time = 0;
    bool paused = false;
    bool shotted = false; // 定时器是否已经触发过
    bool one_shot = false; // 定时器是否为单次触发模式
    std::function<void()> on_timeout; // 当定时器超时时调用的回调函数
};

#endif // !_TIMER_H_