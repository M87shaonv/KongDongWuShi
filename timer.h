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

    // ���õ���ʱ����ʱʱ���õĻص�����
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
            // �ж��Ƿ���Դ�����ʱ��
            bool can_shot = (!one_shot || (one_shot && !shotted));
            shotted = true; // ����Ѵ���
            if (can_shot && on_timeout) // ������Դ����������˻ص�
                on_timeout(); // ���ûص�����

            // ��ȥ�ȴ���ʱ�������Ա����ظ���ʱ
            pass_time -= wait_time;
        }
    }

private:
    float pass_time = 0;
    float wait_time = 0;
    bool paused = false;
    bool shotted = false; // ��ʱ���Ƿ��Ѿ�������
    bool one_shot = false; // ��ʱ���Ƿ�Ϊ���δ���ģʽ
    std::function<void()> on_timeout; // ����ʱ����ʱʱ���õĻص�����
};

#endif // !_TIMER_H_