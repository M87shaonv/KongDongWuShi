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
    const float FLOOR_Y = 620;//地板的竖直方向坐标
    const float GRAVITY = 980 * 2;//重力大小
protected:
    int hp = ENEMY_HP;
    Vector2 sole_pos;//角色脚底位置
    Vector2 velocity;//角色速度
    float logic_height = 0;//角色逻辑高度
    bool is_facing_left = true;//当前是否朝向左
    StateMachine state_machine;//角色逻辑状态机
    bool enable_gravity = true;//是否启用重力模拟
    bool is_invulnerable = false;//当前是否无敌
    Timer timer_invulnerable_blink;//无敌闪烁状态定时器
    Timer timer_invulnerable_status;//无敌状态定时器
    bool is_blink_invisible = false;//当前是否处于闪烁的不可见帧
    CollisionBox* hit_box = nullptr;//攻击碰撞箱
    CollisionBox* hurt_box = nullptr;//受击碰撞箱

    AnimationGroup* current_anim = nullptr;
    std::unordered_map<std::string, AnimationGroup> anim_pool;//角色动画池
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
