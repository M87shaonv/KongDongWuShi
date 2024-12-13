#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "vector2.h"
#include "animation.h"
#include "collision_box.h"
#include "state_machine.h"

#include <string>
#include <graphics.h>
#include <unordered_map>
#include "config.h"

class Character
{
public:
    Character();
    ~Character();
protected:
    struct AnimationGroup
    {
        Animation left;
        Animation right;
    };
protected:
    const float FLOOR_Y = 620;//�ذ����ֱ��������
    const float GRAVITY = 980 * 2;//������С
protected:
    int hp = ENEMY_HP;
    Vector2 sole_pos;//��ɫ�ŵ�λ��
    Vector2 velocity;//��ɫ�ٶ�
    float logic_height = 0;//��ɫ�߼��߶�
    bool is_facing_left = true;//��ǰ�Ƿ�����
    StateMachine state_machine;//��ɫ�߼�״̬��
    bool enable_gravity = true;//�Ƿ���������ģ��
    bool is_invulnerable = false;//��ǰ�Ƿ��޵�
    Timer timer_invulnerable_blink;//�޵���˸״̬��ʱ��
    Timer timer_invulnerable_status;//�޵�״̬��ʱ��
    bool is_blink_invisible = false;//��ǰ�Ƿ�����˸�Ĳ��ɼ�֡
    CollisionBox* hit_box = nullptr;//������ײ��
    CollisionBox* hurt_box = nullptr;//�ܻ���ײ��

    AnimationGroup* current_anim = nullptr;
    std::unordered_map<std::string, AnimationGroup> anim_pool;//��ɫ������
public:
    void decrease_hp();
    int get_hp() const
    {
        return hp;
    }
    void set_hp(int hp)
    {
        this->hp = hp;
    }
    void set_sole_pos(const Vector2& pos)
    {
        this->sole_pos = pos;
    }
    const Vector2& get_sole_pos() const
    {
        return sole_pos;
    }
    void set_velocity(const Vector2& ve)
    {
        velocity = ve;
    }
    const Vector2& get_velocity() const
    {
        return velocity;
    }
    Vector2 get_logic_center() const
    {
        return Vector2(sole_pos.x, sole_pos.y - logic_height / 2);
    }
    void set_gravity_enabled(bool flag)
    {
        enable_gravity = flag;
    }
    CollisionBox* get_hit_box()
    {
        return hit_box;
    }
    CollisionBox* get_hurt_box()
    {
        return hurt_box;
    }
    bool is_on_floor() const
    {
        return sole_pos.y >= FLOOR_Y;
    }
    float get_floor_y() const
    {
        return FLOOR_Y;
    }
    void make_invulnerable()
    {
        is_invulnerable = true;
        timer_invulnerable_status.restart();
    }
    StateMachine get_state_machine()
    {
        return state_machine;
    }
    virtual void on_input(const ExMessage& msg);
    virtual void on_update(float delta);
    virtual void on_render();
    virtual void on_hurt();

    void switch_state(const std::string& id);
    void set_anim(const std::string& id);
};
#endif // !_CHARACTER_H_
