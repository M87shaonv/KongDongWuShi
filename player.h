#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "character.h"

class Player :public Character
{
public:
    enum class AttackDir
    {
        Up,
        Down,
        Left,
        Right
    };
private:
    Timer timer_roll_cd;//������ȴCD
    bool is_rolling = false;//�Ƿ��ڷ���״̬
    bool is_roll_cd_comp = true;//������ȴ�Ƿ����

    Timer timer_attack_cd;//������ȴCD
    bool is_attacking = false;//�Ƿ��ڹ���״̬
    bool is_attack_cd_comp = true;//������ȴ�Ƿ����
    AttackDir current_dir = AttackDir::Right;//������һ�εİ�������ֵ
    CollisionBox* bullettime_box = nullptr;//�ӵ�ʱ����ײ��
    Timer timer_bullettime;
    bool is_bullettimeing = false;
    bool success_dodge = false;
//��λ״̬
    bool is_left_key_down = false;
    bool is_right_key_down = false;
    bool is_jump_key_down = false;
    bool is_roll_key_down = false;
    bool is_attack_key_down = false;

    Animation anim_slash_up;
    Animation anim_slash_down;
    Animation anim_slash_left;
    Animation anim_slash_right;
    AttackDir attack_dir = AttackDir::Right;
    Animation* current_slash_anim = nullptr;
    //�̳���Ч
    bool is_jump_vfx_visible = false;
    Animation anim_jump_vfx;
    bool is_land_vfx_visible = false;
    Animation anim_land_vfx;

private:
    const float CD_ROLL = 0.75f;
    const float CD_ATTACK = 0.75f;
    const float SPEED_RUN = 300.0f;
    const float SPEED_JUMP = 780.0f;
    const float SPEED_ROLL = 800.0f;
    const float DURATION_BULLETTIME = 0.5f;

public:
    std::string attackDirToString(AttackDir dir);
    Player();
    ~Player();
    void on_input(const ExMessage& msg) override;
    void on_update(float delta) override;
    void on_render() override;
    void on_hurt() override;
    void reset_keydown_state()
    {
        is_left_key_down = is_right_key_down = is_jump_key_down = is_roll_key_down = is_attack_key_down
            = is_jump_vfx_visible = is_land_vfx_visible = false;
    }

    void set_rolling(bool flag)
    {
        is_rolling = flag;
    }
    bool get_rolling() const
    {
        return is_rolling;
    }
    bool can_roll() const
    {
        return is_roll_cd_comp && !is_rolling && is_roll_key_down;
    }

    void set_attacking(bool flag)
    {
        is_attacking = flag;
    }
    bool get_attacking()  const
    {
        return is_attacking;
    }
    bool can_attack() const
    {
        return (is_attack_cd_comp && !is_attacking && is_attack_key_down);
    }

    bool can_jump() const
    {
        return is_on_floor() && is_jump_key_down;
    }
    int get_move_axis() const//��ȡ�ƶ�����,1�����ƶ�,-1�����ƶ�
    {
        return is_right_key_down - is_left_key_down;
    }
    AttackDir get_attack_dir() const
    {
        return attack_dir;
    }
    CollisionBox* get_bullettime_box()
    {
        return bullettime_box;
    }
    void set_success_dodge(bool flag)
    {
        success_dodge = flag;
    }
    bool get_success_dodge() const
    {
        return success_dodge;
    }
    void on_jump();
    void on_land();
    void on_roll();
    void on_attack();
private:
    void update_attack_dir();
};
#endif // !_PLAYER_H_
