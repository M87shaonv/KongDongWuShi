#include "character.h"
#include "collision_manager.h"

Character::Character()
{
    hit_box = CollisionManager::instance()->create_collision_box();
    hurt_box = CollisionManager::instance()->create_collision_box();
    timer_invulnerable_status.set_wait_time(1.0f);
    timer_invulnerable_status.set_one_shot(true);
    timer_invulnerable_status.set_on_timeout([&]()
        {
            is_invulnerable = false;
        });
    timer_invulnerable_blink.set_wait_time(0.075f);
    timer_invulnerable_blink.set_one_shot(false);
    timer_invulnerable_blink.set_on_timeout([&]()
        {
            is_blink_invisible = !is_blink_invisible;
        });
}
Character::~Character()
{
    delete current_anim;
    CollisionManager::instance()->destory_collision_box(hit_box);
    CollisionManager::instance()->destory_collision_box(hurt_box);
}
void Character::decrease_hp()
{
    if (is_invulnerable)
        return;

    hp -= 1;
    if (hp > 0)
        make_invulnerable();
    on_hurt();
}
void Character::on_input(const ExMessage& msg) {}

void Character::on_update(float delta)
{
    state_machine.on_update(delta);

    if (hp <= 0)
        velocity.x = 0;//死亡后不再水平移动

    if (enable_gravity)
        velocity.y += GRAVITY * delta;//更新竖直方向速度
    sole_pos += velocity * delta;

    if (sole_pos.y >= FLOOR_Y)//角色脚底位于地板下时,修正位置并设置竖直方向速度为0
    {
        sole_pos.y = FLOOR_Y;
        velocity.y = 0;
    }
    //对水平方向限制防止角色移出屏幕外
    if (sole_pos.x <= 0)
        sole_pos.x = 0;
    if (sole_pos.x >= getwidth())
        sole_pos.x = (float)getwidth();

    hurt_box->set_position(get_logic_center());//受击碰撞箱跟随玩家移动

    timer_invulnerable_status.on_update(delta);
    if (is_invulnerable)
        timer_invulnerable_blink.on_update(delta);

    if (!current_anim)
        return;
    Animation& animation = (is_facing_left ? current_anim->left : current_anim->right);

    animation.on_update(delta);
    animation.set_position(sole_pos);
    hit_box->set_position(get_logic_center());
    hurt_box->set_position(get_logic_center());
}
void Character::on_render()
{
    if (!current_anim || (is_invulnerable && is_blink_invisible))
        return;
    (is_facing_left ? current_anim->left : current_anim->right).on_render();
}
void Character::on_hurt() {}

void Character::switch_state(const std::string& id)
{
    state_machine.switch_to(id);
}
void Character::set_anim(const std::string& id)
{
    current_anim = &anim_pool[id];
    current_anim->left.reset();
    current_anim->right.reset();
}