﻿#include "resources_manager.h"
#include "util.h"
#include <iostream>
#include <tchar.h>

ResourcesManager* ResourcesManager::manager = nullptr;

ResourcesManager* ResourcesManager::instance()//返回私有的manager
{
    if (!manager)
        manager = new ResourcesManager();

    return manager;
}

ResourcesManager::ResourcesManager() = default;
ResourcesManager::~ResourcesManager()
{
    // 释放所有图像资源
    for (auto& pair : image_pool)
    {
        delete pair.second;
    }
    image_pool.clear();

    // 释放所有图集资源
    for (auto& pair : atlas_pool)
    {
        delete pair.second;
    }
    atlas_pool.clear();
}

struct ImageResInfo
{
    std::string id;
    LPCTSTR path;
};
struct AtlasResInfo
{
    std::string id;
    LPCTSTR path;
    int num_frame = 0;
};
static const std::vector<ImageResInfo> image_info_list =
{
    { "background",       _T(R"(resources\background.png)") },
    { "ui_heart",         _T(R"(resources\ui_heart.png)") },
    {"ui_heart_enemy",_T(R"(resources\ui_heart_enemy.png)")},

    { "player_attack_right", _T(R"(resources\player\attack.png)") },
    { "player_dead_right",   _T(R"(resources\player\dead.png)") },
    { "player_fall_right",   _T(R"(resources\player\fall.png)") },
    { "player_idle_right",   _T(R"(resources\player\idle.png)") },
    { "player_jump_right",   _T(R"(resources\player\jump.png)") },
    { "player_run_right",    _T(R"(resources\player\run.png)") },
    { "player_roll_right",   _T(R"(resources\player\roll.png)") },
    { "player_vfx_attack_down", _T(R"(resources\player\vfx_attack_down.png)") },

    { "player_vfx_attack_left", _T(R"(resources\player\vfx_attack_left.png)") },
    { "player_vfx_attack_right",_T(R"(resources\player\vfx_attack_right.png)") },
    { "player_vfx_attack_up",   _T(R"(resources\player\vfx_attack_up.png)") },
    { "player_vfx_jump",       _T(R"(resources\player\vfx_jump.png)") },
    { "player_vfx_land",       _T(R"(resources\player\vfx_land.png)") },
};

static const std::vector<AtlasResInfo> atlas_info_list =
{
    { "barb_break",    _T(R"(resources\enemy\barb_break\%d.png)"), 3 },
    { "barb_loose",    _T(R"(resources\enemy\barb_loose\%d.png)"), 5 },
    { "silk",         _T(R"(resources\enemy\silk\%d.png)"), 9 },
    { "sword_left",   _T(R"(resources\enemy\sword\%d.png)"), 3 },

    { "enemy_aim_left",          _T(R"(resources\enemy\aim\%d.png)"), 9 },
    { "enemy_dash_in_air_left",  _T(R"(resources\enemy\dash_in_air\%d.png)"), 2 },
    { "enemy_dash_on_floor_left",_T(R"(resources\enemy\dash_on_floor\%d.png)"), 2 },
    { "enemy_fall_left",         _T(R"(resources\enemy\fall\%d.png)"), 4 },
    { "enemy_idle_left",         _T(R"(resources\enemy\idle\%d.png)"), 6 },
    { "enemy_jump_left",         _T(R"(resources\enemy\jump\%d.png)"), 8 },
    { "enemy_run_left",          _T(R"(resources\enemy\run\%d.png)"), 8 },
    { "enemy_squat_left",        _T(R"(resources\enemy\squat\%d.png)"), 10},
    { "enemy_throw_barb_left",   _T(R"(resources\enemy\throw_barb\%d.png)"), 8 },
    { "enemy_throw_silk_left",   _T(R"(resources\enemy\throw_silk\%d.png)"), 17 },
    { "enemy_throw_sword_left",  _T(R"(resources\enemy\throw_sword\%d.png)"), 16 },
    { "enemy_vfx_dash_in_air_left", _T(R"(resources\enemy\vfx_dash_in_air\%d.png)"), 5 },
    { "enemy_vfx_dash_on_floor_left", _T(R"(resources\enemy\vfx_dash_on_floor\%d.png)"), 6 },
};

// 查找并返回指定 ID 的图集
Atlas* ResourcesManager::find_atlas(const std::string& id) const
{
    const auto& iterator = atlas_pool.find(id);// 在 atlas_pool 中查找指定 ID 的图集
    if (iterator == atlas_pool.end())// 如果找不到，返回 nullptr
        return nullptr;

    return iterator->second;
}
IMAGE* ResourcesManager::find_image(const std::string& id) const
{
    const auto& iterator = image_pool.find(id);
    if (iterator == image_pool.end())
        return nullptr;

    return iterator->second;
}

//翻转图像
void ResourcesManager::flip_image(IMAGE* src_img, IMAGE* dst_img, int num_h)
{
    int w = src_img->getwidth();
    int h = src_img->getheight();

    int w_frame = w / num_h;//计算每帧宽度
    Resize(dst_img, w, h);// 调整目标图像的大小以匹配源图像

    // 获取源图像和目标图像的缓冲区
    DWORD* src_buffer = GetImageBuffer(src_img);
    DWORD* dst_buffer = GetImageBuffer(dst_img);

    // 遍历每一帧
    for (size_t i = 0; i < num_h; i++)
    {
        // 计算当前帧的左右边界
        int x_left = i * w_frame;
        int x_right = (i + 1) * w_frame;

        for (size_t y = 0; y < h; y++)// 遍历当前帧的每一行
            for (size_t x = x_left; x < x_right; x++)// 遍历当前帧的每一列
            {
                int index_src = y * w + x;// 计算源图像中的索引
                int index_dst = y * w + x_right - (x - x_left);// 计算目标图像中的索引（水平翻转）
                dst_buffer[index_dst] = src_buffer[index_src];// 将源图像中的像素复制到目标图像中
            }
    }
}
void ResourcesManager::flip_image(const std::string& src_id, const std::string dst_id, int num_h)
{
    IMAGE* src_img = image_pool[src_id];
    IMAGE* dst_img = new IMAGE();

    flip_image(src_img, dst_img, num_h);
    image_pool[dst_id] = dst_img;
}

void ResourcesManager::flip_atlas(const std::string& src_id, const std::string dst_id)
{
    Atlas* src_atlas = atlas_pool[src_id];
    Atlas* dst_atlas = new Atlas();

    for (size_t i = 0; i < src_atlas->get_size(); i++)// 遍历源图集中的每一个图像
    {
        IMAGE img_flipped;// 创建一个临时图像对象
        flip_image(src_atlas->get_image(i), &img_flipped);// 翻转当前图像
        dst_atlas->add_image(img_flipped);// 将翻转后的图像添加到目标图集中
    }
    atlas_pool[dst_id] = dst_atlas;// 将翻转后的图集添加到图集池中
}
// 调用 GetImageBuffer 函数，返回表示图像是否有效
static inline bool check_image_valid(IMAGE* image)
{
    return GetImageBuffer(image);
}
//加载资源
void ResourcesManager::load()
{
    for (const auto& info : image_info_list)
    {
        IMAGE* image = new IMAGE();
        loadimage(image, info.path);
        if (!check_image_valid(image))
            throw info.path;// 如果图像无效，抛出异常，异常信息为图像文件路径
        image_pool[info.id] = image;// 将有效的图像添加到图像池中，键为图像 ID

        std::wcout << L"Loading image: " << info.path << std::endl;
    }

    for (const auto& info : atlas_info_list)
    {
        Atlas* atlas = new Atlas();
        atlas->load(info.path, info.num_frame);
        for (size_t i = 0; i < atlas->get_size(); i++)
        {
            IMAGE* image = atlas->get_image(i);
            if (!check_image_valid(image))
                throw info.path;
        }
        atlas_pool[info.id] = atlas;

        std::wcout << L"Loading atlas: " << info.path << std::endl;
    }

    flip_image("player_attack_right", "player_attack_left", 5);
    flip_image("player_dead_right", "player_dead_left", 6);
    flip_image("player_fall_right", "player_fall_left", 5);
    flip_image("player_idle_right", "player_idle_left", 5);
    flip_image("player_jump_right", "player_jump_left", 5);
    flip_image("player_run_right", "player_run_left", 10);
    flip_image("player_roll_right", "player_roll_left", 7);

    flip_atlas("sword_left", "sword_right");
    flip_atlas("enemy_aim_left", "enemy_aim_right");
    flip_atlas("enemy_dash_in_air_left", "enemy_dash_in_air_right");
    flip_atlas("enemy_dash_on_floor_left", "enemy_dash_on_floor_right");
    flip_atlas("enemy_fall_left", "enemy_fall_right");
    flip_atlas("enemy_idle_left", "enemy_idle_right");
    flip_atlas("enemy_jump_left", "enemy_jump_right");
    flip_atlas("enemy_run_left", "enemy_run_right");
    flip_atlas("enemy_squat_left", "enemy_squat_right");
    flip_atlas("enemy_throw_barb_left", "enemy_throw_barb_right");
    flip_atlas("enemy_throw_silk_left", "enemy_throw_silk_right");
    flip_atlas("enemy_throw_sword_left", "enemy_throw_sword_right");
    flip_atlas("enemy_vfx_dash_in_air_left", "enemy_vfx_dash_in_air_right");
    flip_atlas("enemy_vfx_dash_on_floor_left", "enemy_vfx_dash_on_floor_right");

    load_audio(_T(R"(resources\audio\bgm.mp3)"), _T("bgm"));
    load_audio(_T(R"(resources\audio\barb_break.mp3)"), _T("barb_break"));
    load_audio(_T(R"(resources\audio\bullet_time.mp3)"), _T("bullet_time"));

    load_audio(_T(R"(resources\audio\enemy_dash.mp3)"), _T("enemy_dash"));
    load_audio(_T(R"(resources\audio\enemy_run.mp3)"), _T("enemy_run"));
    load_audio(_T(R"(resources\audio\enemy_hurt_1.mp3)"), _T("enemy_hurt_1"));
    load_audio(_T(R"(resources\audio\enemy_hurt_2.mp3)"), _T("enemy_hurt_2"));
    load_audio(_T(R"(resources\audio\enemy_hurt_3.mp3)"), _T("enemy_hurt_3"));
    load_audio(_T(R"(resources\audio\enemy_throw_barbs.mp3)"), _T("enemy_throw_barbs"));
    load_audio(_T(R"(resources\audio\enemy_throw_silk.mp3)"), _T("enemy_throw_silk"));
    load_audio(_T(R"(resources\audio\enemy_throw_sword.mp3)"), _T("enemy_throw_sword"));

    load_audio(_T(R"(resources\audio\player_attack_1.mp3)"), _T("player_attack_1"));
    load_audio(_T(R"(resources\audio\player_attack_2.mp3)"), _T("player_attack_2"));
    load_audio(_T(R"(resources\audio\player_attack_3.mp3)"), _T("player_attack_3"));
    load_audio(_T(R"(resources\audio\player_dead.mp3)"), _T("player_dead"));
    load_audio(_T(R"(resources\audio\player_hurt.mp3)"), _T("player_hurt"));
    load_audio(_T(R"(resources\audio\player_jump.mp3)"), _T("player_jump"));
    load_audio(_T(R"(resources\audio\player_land.mp3)"), _T("player_land"));
    load_audio(_T(R"(resources\audio\player_roll.mp3)"), _T("player_roll"));
    load_audio(_T(R"(resources\audio\player_run.mp3)"), _T("player_run"));
}