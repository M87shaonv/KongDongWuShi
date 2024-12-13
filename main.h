#ifndef _MAIN_H_
#define _MAIN_H_
#include "config.h"
void ResetGame();
void RunGame();
void GameOver(std::string loser);
void draw_background();
void draw_remain_hp();
void draw_enemy_remain_hp();
ExMessage msg;
bool is_quit = false;
Button restart_btn(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, 200, 40, L"重新开始", nullptr, RGB(100, 67, 0));
Button exit_btn(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 50, 200, 40, L"退出游戏", nullptr, RGB(100, 67, 0));
#endif // !_MAIN_H_
