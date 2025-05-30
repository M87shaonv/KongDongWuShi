#include "barb.h"
#include "resources_manager.h"
#include "collision_manager.h"
#include "character_manager.h"

Barb::Barb()
{
    diff_period = range_random(0, 6);

    anim_loose.set_interval(0.15);
    anim_loose.set_loop(true);
    anim_loose.set_anchor_mode(Animation::AnchorMode::Centered);
    anim_loose.add_frame(ResourcesManager::instance()->find_atlas("barb_loose"));

    anim_break.set_interval(0.1f);
    anim_break.set_loop(false);
    anim_break.set_anchor_mode(Animation::AnchorMode::Centered);
    anim_break.add_frame(ResourcesManager::instance()->find_atlas("barb_break"));
    anim_break.set_on_finished([&]() {is_valid = false; });

    collision_box = CollisionManager::instance()->create_collision_box();
    collision_box->set_layer_src(CollisionLayer::None);
    collision_box->set_layer_dst(CollisionLayer::Player);
    collision_box->set_size({ 20,20 });
    collision_box->set_one_collide([&]() {on_break(); });

    timer_idle.set_wait_time((float)range_random(3, 10));
    timer_idle.set_one_shot(true);
    timer_idle.set_on_timeout([&]() {
        if (stage != Stage::Idle) return;

        stage = Stage::Aim;
        base_position = current_position;
        });

    timer_aim.set_wait_time(0.75f);
    timer_aim.set_one_shot(true);
    timer_aim.set_on_timeout([&]() {
        if (stage != Stage::Aim) return;

        stage = Stage::Dash;
        const Vector2& pos_player = CharacterManager::instance()->get_player()->get_sole_pos();
        velocity = (pos_player - current_position).normalize() * SPEED_DASH;
        });
}
Barb::~Barb()
{
    CollisionManager::instance()->destory_collision_box(collision_box);
}

void Barb::on_update(float delta)
{
    if (stage == Stage::Idle)
        timer_idle.on_update(delta);
    if (stage == Stage::Aim)
        timer_aim.on_update(delta);

    total_delta_time += delta;
    switch (stage)
    {
        case Barb::Stage::Idle:
            current_position.y = base_position.y + sin(total_delta_time * 2 + diff_period) * 30;
            break;
        case Barb::Stage::Aim:
            current_position.x = base_position.x + range_random(-10, 10);
            break;
        case Barb::Stage::Dash:
            current_position += velocity * delta;
            if (current_position.y >= CharacterManager::instance()->get_player()->get_floor_y())
                on_break();
            if (current_position.y <= 0)
                is_valid = false;
            break;
        case Barb::Stage::Break:
            break;
        default:
            break;
    }
    collision_box->set_position(current_position);

    current_anim = (stage == Stage::Break ? &anim_break : &anim_loose);
    current_anim->set_position(current_position);
    current_anim->on_update(delta);
}

void Barb::on_render()
{
    current_anim->on_render();
}

void Barb::on_break()
{
    if (stage == Stage::Break) return;

    stage = Stage::Break;
    collision_box->set_enabled(false);
    play_audio(_T("barb_break"), false);
}