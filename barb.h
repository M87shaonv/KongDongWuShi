#ifndef _BARB_H_
#define _BARB_H_
#include "animation.h"
#include "collision_box.h"
class Barb
{
public:
    Barb();
    ~Barb();

    void on_update(float delta);
    void on_render();

    void set_position(const Vector2& position)
    {
        this->base_position = position;
        this->current_position = position;
    }
    bool check_valid() const
    {
        return is_valid;
    }
private:
    enum class Stage
    {
        Idle,
        Aim,
        Dash,
        Break
    };
    void on_break();
private:
    const float SPEED_DASH = 1500.0f;
    Timer timer_idle;
    Timer timer_aim; //��׼״̬����ʱ��
    int diff_period = 0;//���Ƹ������˶�����ƫ��
    bool is_valid = true;
    float total_delta_time = 0;//���������ȹ���ʱ��

    Vector2 velocity;
    Vector2 base_position;
    Vector2 current_position;

    Animation anim_loose;
    Animation anim_break;
    Animation* current_anim = nullptr;

    Stage stage = Stage::Idle;
    CollisionBox* collision_box = nullptr;
};
#endif // !_BARB_H_
