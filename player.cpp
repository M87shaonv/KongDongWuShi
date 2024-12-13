#include "player.h"
#include "resources_manager.h"
#include "player_state_nodes.h"
#include <iostream>
#include "config.h"
#include "collision_manager.h"
#include "bullet_time_manager.h"
Player::Player()
{
    hp = PLAYER_HP;
    is_facing_left = false;
    sole_pos = { 250,200 };
    logic_height = 120;
    bullettime_box = CollisionManager::instance()->create_collision_box();
    hit_box->set_size({ 150,150 });
    hurt_box->set_size({ 40,80 });
    bullettime_box->set_size({ 60,80 });

    hit_box->set_layer_src(CollisionLayer::None);
    hit_box->set_layer_dst(CollisionLayer::Enemy);

    hurt_box->set_layer_src(CollisionLayer::Player);
    hurt_box->set_layer_dst(CollisionLayer::None);

    bullettime_box->set_layer_src(CollisionLayer::Player);
    bullettime_box->set_layer_dst(CollisionLayer::None);

    hit_box->set_enabled(false);//只在攻击时开启攻击碰撞箱
    hurt_box->set_enabled(true);
    hurt_box->set_one_collide([&] {
        decrease_hp();
        });

    bullettime_box->set_enabled(false);//只在翻滚时开启触发子弹时间碰撞箱
    bullettime_box->set_one_collide([&] {
        is_bullettimeing = true;
        play_audio(_T("bullet_time"), false);
        BulletTimeManager::instance()->set_status(BulletTimeManager::Status::Entering);
        });
    timer_bullettime.set_wait_time(DURATION_BULLETTIME);
    timer_bullettime.set_one_shot(true);
    timer_bullettime.set_on_timeout([&]() {
        get_hurt_box()->set_enabled(true);
        is_bullettimeing = false;
        stop_audio(_T("bullet_time"));
        BulletTimeManager::instance()->set_status(BulletTimeManager::Status::Exiting);
        });

    timer_roll_cd.set_wait_time(CD_ROLL);
    timer_roll_cd.set_one_shot(true);
    timer_roll_cd.set_on_timeout([&] {
        is_roll_cd_comp = true;
        });

    timer_attack_cd.set_wait_time(CD_ATTACK);
    timer_attack_cd.set_one_shot(true);
    timer_attack_cd.set_on_timeout([&] {
        is_attack_cd_comp = true;
        });
#pragma region 动画对象初始化

    {
        AnimationGroup& animation_attack = anim_pool["attack"];

        Animation& animation_attack_left = animation_attack.left;
        animation_attack_left.set_interval(0.05f);
        animation_attack_left.set_loop(false);
        animation_attack_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_attack_left.add_frame(ResourcesManager::instance()->find_image("player_attack_left"), 5);

        Animation& animation_attack_right = animation_attack.right;
        animation_attack_right.set_interval(0.05f);
        animation_attack_right.set_loop(false);
        animation_attack_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_attack_right.add_frame(ResourcesManager::instance()->find_image("player_attack_right"), 5);
    }

    {
        AnimationGroup& animation_dead = anim_pool["dead"];

        Animation& animation_dead_left = animation_dead.left;
        animation_dead_left.set_interval(0.1f);
        animation_dead_left.set_loop(false);
        animation_dead_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_dead_left.add_frame(ResourcesManager::instance()->find_image("player_dead_left"), 6);

        Animation& animation_dead_right = animation_dead.right;
        animation_dead_right.set_interval(0.1f);
        animation_dead_right.set_loop(false);
        animation_dead_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_dead_right.add_frame(ResourcesManager::instance()->find_image("player_dead_right"), 6);
    }

    {
        AnimationGroup& animation_fall = anim_pool["fall"];

        Animation& animation_fall_left = animation_fall.left;
        animation_fall_left.set_interval(0.15f);
        animation_fall_left.set_loop(true);
        animation_fall_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_fall_left.add_frame(ResourcesManager::instance()->find_image("player_fall_left"), 5);

        Animation& animation_fall_right = animation_fall.right;
        animation_fall_right.set_interval(0.15f);
        animation_fall_right.set_loop(true);
        animation_fall_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_fall_right.add_frame(ResourcesManager::instance()->find_image("player_fall_right"), 5);
    }

    {
        AnimationGroup& animation_idle = anim_pool["idle"];

        Animation& animation_idle_left = animation_idle.left;
        animation_idle_left.set_interval(0.15f);
        animation_idle_left.set_loop(true);
        animation_idle_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_idle_left.add_frame(ResourcesManager::instance()->find_image("player_idle_left"), 5);

        Animation& animation_idle_right = animation_idle.right;
        animation_idle_right.set_interval(0.15f);
        animation_idle_right.set_loop(true);
        animation_idle_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_idle_right.add_frame(ResourcesManager::instance()->find_image("player_idle_right"), 5);
    }

    {
        AnimationGroup& animation_jump = anim_pool["jump"];

        Animation& animation_jump_left = animation_jump.left;
        animation_jump_left.set_interval(0.15f);
        animation_jump_left.set_loop(false);
        animation_jump_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_jump_left.add_frame(ResourcesManager::instance()->find_image("player_jump_left"), 5);

        Animation& animation_jump_right = animation_jump.right;
        animation_jump_right.set_interval(0.15f);
        animation_jump_right.set_loop(false);
        animation_jump_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_jump_right.add_frame(ResourcesManager::instance()->find_image("player_jump_right"), 5);
    }

    {
        AnimationGroup& animation_roll = anim_pool["roll"];

        Animation& animation_roll_left = animation_roll.left;
        animation_roll_left.set_interval(0.05f);
        animation_roll_left.set_loop(false);
        animation_roll_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_roll_left.add_frame(ResourcesManager::instance()->find_image("player_roll_left"), 7);

        Animation& animation_roll_right = animation_roll.right;
        animation_roll_right.set_interval(0.05f);
        animation_roll_right.set_loop(false);
        animation_roll_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_roll_right.add_frame(ResourcesManager::instance()->find_image("player_roll_right"), 7);
    }

    {
        AnimationGroup& animation_run = anim_pool["run"];

        Animation& animation_run_left = animation_run.left;
        animation_run_left.set_interval(0.075f);
        animation_run_left.set_loop(true);
        animation_run_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_run_left.add_frame(ResourcesManager::instance()->find_image("player_run_left"), 10);

        Animation& animation_run_right = animation_run.right;
        animation_run_right.set_interval(0.075f);
        animation_run_right.set_loop(true);
        animation_run_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_run_right.add_frame(ResourcesManager::instance()->find_image("player_run_right"), 10);
    }

#pragma endregion

#pragma region 特效动画初始化
    {
        anim_slash_up.set_interval(0.07f);
        anim_slash_up.set_loop(false);
        anim_slash_up.set_anchor_mode(Animation::AnchorMode::Centered);
        anim_slash_up.add_frame(ResourcesManager::instance()->find_image("player_vfx_attack_up"), 5);
    }
    {
        anim_slash_down.set_interval(0.07f);
        anim_slash_down.set_loop(false);
        anim_slash_down.set_anchor_mode(Animation::AnchorMode::Centered);
        anim_slash_down.add_frame(ResourcesManager::instance()->find_image("player_vfx_attack_down"), 5);
    }
    {
        anim_slash_left.set_interval(0.07f);
        anim_slash_left.set_loop(false);
        anim_slash_left.set_anchor_mode(Animation::AnchorMode::Centered);
        anim_slash_left.add_frame(ResourcesManager::instance()->find_image("player_vfx_attack_left"), 5);
    }
    {
        anim_slash_right.set_interval(0.07f);
        anim_slash_right.set_loop(false);
        anim_slash_right.set_anchor_mode(Animation::AnchorMode::Centered);
        anim_slash_right.add_frame(ResourcesManager::instance()->find_image("player_vfx_attack_right"), 5);
    }
    {
        anim_jump_vfx.set_interval(0.05f);
        anim_jump_vfx.set_loop(false);
        anim_jump_vfx.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        anim_jump_vfx.add_frame(ResourcesManager::instance()->find_image("player_vfx_jump"), 5);
        anim_jump_vfx.set_on_finished([&]() {is_jump_vfx_visible = false; });
    }
    {
        anim_land_vfx.set_interval(0.1f);
        anim_land_vfx.set_loop(false);
        anim_land_vfx.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        anim_land_vfx.add_frame(ResourcesManager::instance()->find_image("player_vfx_land"), 2);
        anim_land_vfx.set_on_finished([&]() {is_land_vfx_visible = false; });
    }

#pragma endregion

#pragma region 状态机初始化
    state_machine.register_state("attack", new PlayerAttackState());
    state_machine.register_state("dead", new PlayerDeadState());
    state_machine.register_state("fall", new PlayerFallState());
    state_machine.register_state("idle", new PlayerIdleState());
    state_machine.register_state("jump", new PlayerJumpState());
    state_machine.register_state("roll", new PlayerRollState());
    state_machine.register_state("run", new PlayerRunState());

    state_machine.set_entry("idle");
#pragma endregion
}
Player::~Player()
{
    CollisionManager::instance()->destory_collision_box(bullettime_box);
}
void Player::on_input(const ExMessage& msg)
{
    if (hp <= 0) return;

    switch (msg.message)
    {
        case WM_KEYDOWN:
            switch (msg.vkcode)
            {
                case 0x41:
                case VK_LEFT:
                    is_left_key_down = true;
                    current_dir = AttackDir::Left;
                    break;
                case 0x44:
                case VK_RIGHT:
                    is_right_key_down = true;
                    current_dir = AttackDir::Right;
                    break;
                case VK_UP:
                case VK_SPACE:
                    is_jump_key_down = true;
                    current_dir = AttackDir::Up;
                    break;
                case 0x53:
                case VK_DOWN:
                    is_roll_key_down = true;
                    current_dir = AttackDir::Down;
                    break;
                case VK_ESCAPE:
                    MessageBox(GetHWnd(), L"点击确定继续游戏", L"暂停中", MB_OK);
                    break;
                case 0x4B://'K'
                    is_attack_key_down = true;
                    update_attack_dir();
                    break;
                default:
                    break;
            }
            break;
        case WM_KEYUP:
            switch (msg.vkcode)
            {
                case 0x41:
                case VK_LEFT:
                    is_left_key_down = false;
                    break;
                case 0x44:
                case VK_RIGHT:
                    is_right_key_down = false;
                    break;
                case VK_UP:
                case VK_SPACE:
                    is_jump_key_down = false;
                    break;
                case 0x53:
                case VK_DOWN:
                    is_roll_key_down = false;
                    break;
                case 0x4B://'K'
                    is_attack_key_down = false;
                    break;
                default:
                    break;
            }
            break;
    }
}
void Player::on_update(float delta)
{
    if (hp > 0 && !is_rolling)
        velocity.x = get_move_axis() * SPEED_RUN;
    if (get_move_axis() != 0)//移动方向不为0时同时更新朝向
        is_facing_left = (get_move_axis() < 0);

    timer_roll_cd.on_update(delta);
    timer_attack_cd.on_update(delta);
    if (is_bullettimeing)
    {
        timer_bullettime.on_update(delta);
    }
    bullettime_box->set_position(get_logic_center());
    anim_jump_vfx.on_update(delta);
    anim_land_vfx.on_update(delta);

    if (is_attacking)
    {
        current_slash_anim->set_position(get_logic_center());
        current_slash_anim->on_update(delta);
    }
    Character::on_update(delta);//基类的物理模拟等
}
void Player::on_render()
{
    if (is_jump_vfx_visible)
        anim_jump_vfx.on_render();
    if (is_land_vfx_visible)
        anim_land_vfx.on_render();

    Character::on_render();

    if (is_attacking)
        current_slash_anim->on_render();
}
void Player::on_hurt()
{
    play_audio(_T("player_hurt"), false);
}
void Player::on_jump()
{
    velocity.y -= SPEED_JUMP;
    is_jump_vfx_visible = true;
    anim_jump_vfx.set_position(sole_pos);
    anim_jump_vfx.reset();
}
void Player::on_land()
{
    is_land_vfx_visible = true;
    anim_land_vfx.set_position(sole_pos);
    anim_land_vfx.reset();
}
void Player::on_roll()
{
    timer_roll_cd.restart();
    timer_bullettime.restart();
    is_roll_cd_comp = false;
    velocity.x = is_facing_left ? -SPEED_ROLL : SPEED_ROLL;//根据角色当前朝向,设置其水平方向的移动速度
}
void Player::on_attack()
{
    timer_attack_cd.restart();
    is_attack_cd_comp = false;
    switch (attack_dir)
    {
        case Player::AttackDir::Up:
            current_slash_anim = &anim_slash_up;
            break;
        case Player::AttackDir::Down:
            current_slash_anim = &anim_slash_down;
            break;
        case Player::AttackDir::Left:
            current_slash_anim = &anim_slash_left;
            break;
        case Player::AttackDir::Right:
            current_slash_anim = &anim_slash_right;
            break;
    }
    current_slash_anim->set_position(get_logic_center());
    current_slash_anim->reset();
}
//void Player::update_attack_dir(int x, int y)
//{
//    static const float PI = 3.141592654f;
//    //使用反三角函数计算得到鼠标点击位置和角色位置夹角的弧度
//    float angle = std::atan2(y - sole_pos.y, x - sole_pos.x); //[-PI, PI]
//
//    //将弧度映射到玩家为中心的四等分圆中,便可以得到当前玩家期望的攻击方向枚举
//    if (angle >= -PI / 4 && angle < PI / 4)
//        attack_dir = AttackDir::Right;
//    else if (angle >= PI / 4 && angle < 3 * PI / 4)
//        attack_dir = AttackDir::Down;
//    else if ((angle >= 3 * PI / 4 && angle <= PI) || (angle >= -PI && angle < -3 * PI / 4))
//        attack_dir = AttackDir::Left;
//    else
//        attack_dir = AttackDir::Up;
//}

void Player::update_attack_dir()
{
    std::cout << "Player Attack Dir:" + attackDirToString(current_dir) << std::endl;
    attack_dir = static_cast<AttackDir>(current_dir);
}
std::string Player::attackDirToString(AttackDir dir)
{
    switch (dir)
    {
        case AttackDir::Up:    return "Up";
        case AttackDir::Down:  return "Down";
        case AttackDir::Left:  return "Left";
        case AttackDir::Right: return "Right";
    }
}