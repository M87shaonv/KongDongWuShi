#include "player.h"
#include "enemy.h"
#include "character_manager.h"
#include "bullet_time_manager.h"

CharacterManager* CharacterManager::manager = nullptr;

CharacterManager* CharacterManager::instance()
{
    if (!manager)
        manager = new CharacterManager();

    return manager;
}

void CharacterManager::on_input(const ExMessage& msg)
{
    player->on_input(msg);
}
void CharacterManager::on_update(float delta)
{
    player->on_update(delta);
    enemy->on_update(delta);
}
void CharacterManager::on_render()
{
    BulletTimeManager::instance()->post_process();
    player->on_render();
    enemy->on_render();
}
CharacterManager::CharacterManager()
{
    player = new Player();
    enemy = new Enemy();
}
CharacterManager::~CharacterManager()
{
    delete player;
    delete enemy;
}