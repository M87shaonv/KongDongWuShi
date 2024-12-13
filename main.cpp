#include "util.h"
#include "resources_manager.h"
#include "collision_manager.h"
#include "character_manager.h"
#include "bullet_time_manager.h"
#include <graphics.h>
#include <chrono>
#include <thread>
#include "Config.h"
#include "button.h"
#include "main.h"
#include "player.h"
#include "enemy.h"

int main(int argc, char** argv)
{
    RunGame();
    return 0;
}
void RunGame()
{
    using namespace std::chrono;

    HWND hwnd = initgraph(SCREEN_WIDTH, SCREEN_HEIGHT, EW_SHOWCONSOLE);
    SetWindowText(hwnd, _T("Hollow Katana"));

#pragma region 加载资源
    try
    {
        ResourcesManager::instance()->load();
    } catch (const LPCTSTR id)
    {
        TCHAR err_msg[512];
        _stprintf_s(err_msg, _T("无法加载: %s"), id);
        MessageBox(hwnd, err_msg, _T("资源加载失败"), MB_OK | MB_ICONERROR);
    }
    play_audio(_T("bgm"), true);
#pragma endregion

    const nanoseconds frame_duration(1000000000 / 144);
    steady_clock::time_point last_tick = steady_clock::now();

    BeginBatchDraw();

    while (!is_quit)
    {
        int player_hp = CharacterManager::instance()->get_player()->get_hp();
        int enemy_hp = CharacterManager::instance()->get_enemy()->get_hp();
        if (enemy_hp <= 0)
        {
            GameOver("enemy");
            break;
        }
        if (player_hp <= 0)
        {
            GameOver("player");
            break;
        }
        while (peekmessage(&msg))//处理消息
        {
            CharacterManager::instance()->on_input(msg);
        }
        steady_clock::time_point frame_start = steady_clock::now();
        duration<float> delta = duration<float>(frame_start - last_tick);

        //CharacterManager::instance()->on_update(delta.count());
        float scaled_delta = BulletTimeManager::instance()->on_update(delta.count());
        Player* player = (Player*)CharacterManager::instance()->get_player();
        Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
        player->on_update(delta.count());
        enemy->on_update(scaled_delta);
        //CharacterManager::instance()->on_update(scaled_delta);
        CollisionManager::instance()->process_collide();
#pragma region 处理更新
        setbkcolor(RGB(0, 0, 0));
        cleardevice();
#pragma endregion

#pragma region 处理绘图
        draw_background();
        CharacterManager::instance()->on_render();
        CollisionManager::instance()->on_debug_render();

        draw_remain_hp();
        draw_enemy_remain_hp();

        FlushBatchDraw();
        last_tick = frame_start;
        nanoseconds sleep_duration = frame_duration - (steady_clock::now() - frame_start);
        if (sleep_duration > nanoseconds(0))
            std::this_thread::sleep_for(sleep_duration);

#pragma endregion
    }
    EndBatchDraw();
}
void GameOver(std::string loser)
{
    if (loser == "player")
    {
        draw_text(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, 40, 0, "Arial Black", "不行,这样不对...", RGB(255, 0, 0)); // 红色表示错误信息
    }
    else
    {
        draw_text(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, 40, 0, "Arial Black", "很好,就是这样", RGB(0, 255, 0)); // 绿色表示正确信息
    }
    is_quit = true;
    while (true)
        if (MouseHit())
        {
            restart_btn.draw();
            exit_btn.draw();
            MOUSEMSG msg = GetMouseMsg();
            FlushBatchDraw();

            if (msg.uMsg == WM_LBUTTONDOWN)
            {
                if (restart_btn.is_clicked(msg.x, msg.y))
                {
                    MessageBox(GetHWnd(), L"重新开始游戏", L"提示", MB_OK);
                    is_quit = false;
                    ResetGame();
                    RunGame();
                    break;
                }
                if (exit_btn.is_clicked(msg.x, msg.y))
                {
                    MessageBox(GetHWnd(), L"退出游戏", L"提示", MB_OK);
                    closegraph();
                    exit(0);
                }
            }
        }
}
void ResetGame()
{
    Player* player = (Player*)CharacterManager::instance()->get_player();
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
    StateMachine stateMachine_player = player->get_state_machine();
    StateMachine stateMachine_enemy = enemy->get_state_machine();
    player->set_hp(PLAYER_HP);
    enemy->set_hp(ENEMY_HP);
    enemy->remove_all();
    player->reset_keydown_state();
    player->set_sole_pos({ 250,200 });
    enemy->set_sole_pos({ 1050, 200 });
    stateMachine_player.switch_to("idle");
    stateMachine_enemy.switch_to("idle");
}
void draw_background()
{
    static IMAGE* img_bg = ResourcesManager::instance()->find_image("background");
    static Rect rect_dst = {
        (getwidth() - img_bg->getwidth()) / 2,
        (getheight() - img_bg->getheight()) / 2,
        img_bg->getwidth(),
        img_bg->getheight()
    };
    putimage_ex(img_bg, &rect_dst);
}
void draw_remain_hp()
{
    static IMAGE* img_ui_heart = ResourcesManager::instance()->find_image("ui_heart");
    Rect rect_dst = { 0, 10, img_ui_heart->getwidth(), img_ui_heart->getheight() };
    for (int i = 0; i < CharacterManager::instance()->get_player()->get_hp(); i++)
    {
        rect_dst.x = 10 + i * 40;
        putimage_ex(img_ui_heart, &rect_dst);
    }
}
void draw_enemy_remain_hp()
{
    static IMAGE* img_ui_heart = ResourcesManager::instance()->find_image("ui_heart_enemy");
    Rect rect_dst = { 860, 10, img_ui_heart->getwidth(), img_ui_heart->getheight() };
    for (int i = 0; i < CharacterManager::instance()->get_enemy()->get_hp(); i++)
    {
        rect_dst.x = 860 + 10 + i * 40;
        putimage_ex(img_ui_heart, &rect_dst);
    }
}