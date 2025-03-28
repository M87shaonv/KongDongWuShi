#include "collision_manager.h"
#include <graphics.h>

CollisionManager* CollisionManager::manager = nullptr;

CollisionManager* CollisionManager::instance()
{
    if (!manager)
        manager = new CollisionManager();

    return manager;
}
CollisionManager::CollisionManager() = default;
CollisionManager::~CollisionManager()
{
    for (auto box : collision_box_list)
    {
        delete box;
    }
}

CollisionBox* CollisionManager::create_collision_box()
{
    CollisionBox* collision_box = new CollisionBox();

    collision_box_list.push_back(collision_box);
    return collision_box;
}
void CollisionManager::destory_collision_box(CollisionBox* collision_box)
{
    /*
    erase从vector中移除指定范围的元素
    remove从vector中移除指定值的所有元素,不会真正删除元素，而是将要移除的元素移到范围的末尾，并返回一个迭代器指向第一个被移除的元素的新位置
    因此remove会将 collision_box 移动到 collision_box_list 的末尾，并返回一个迭代器指向第一个被移除的元素的新位置
    erase 会从这个迭代器开始，直到 collision_box_list 的末尾，删除所有这些元素
    */
    collision_box_list.erase(std::remove(collision_box_list.begin(),
        collision_box_list.end(), collision_box), collision_box_list.end());

    delete collision_box;//释放通过 new 动态分配的内存
}

void CollisionManager::process_collide()
{
    for (CollisionBox* collision_box_src : collision_box_list)
    {
        //确保自身启用碰撞且碰撞目标层级不为空
        if (!collision_box_src->enabled || collision_box_src->layer_dst == CollisionLayer::None)
            continue;
        for (CollisionBox* collision_box_dst : collision_box_list)
        {
            //不检测未启用碰撞或自身或同层级对象的碰撞
            if (!collision_box_dst->enabled || collision_box_src == collision_box_dst
                || collision_box_src->layer_dst != collision_box_dst->layer_src)
                continue;
            /*
            collision_box_src->position.y + collision_box_src->size.y / 2：源矩形的中心点 Y 坐标
            collision_box_dst->position.y + collision_box_dst->size.y / 2：目标矩形的中心点 Y 坐标
            collision_box_src->position.y - collision_box_src->size.y / 2：源矩形的下边界 Y 坐标
            collision_box_dst->position.y - collision_box_dst->size.y / 2：目标矩形的下边界 Y 坐标
            max(collision_box_src->position.y + collision_box_src->size.y / 2,
            collision_box_dst->position.y + collision_box_dst->size.y / 2)：两个矩形上边界的最大值。
            min(collision_box_src->position.y - collision_box_src->size.y / 2,
            collision_box_dst->position.y - collision_box_dst->size.y / 2)：两个矩形下边界中的最小值
            max(collision_box_src->position.y + collision_box_src->size.y / 2,
            collision_box_dst->position.y + collision_box_dst->size.y / 2)
            - min(collision_box_src->position.y - collision_box_src->size.y / 2,
            collision_box_dst->position.y - collision_box_dst->size.y / 2)：两个矩形在 Y 轴上的重叠部分的长度
            如果重叠部分的长度小于或等于两个矩形的高度之和，则认为在 Y 轴上发生了碰撞
            */
            bool is_collide_x = (max(collision_box_src->position.x + collision_box_src->size.x / 2,
                collision_box_dst->position.x + collision_box_dst->size.x / 2)
                - min(collision_box_src->position.x - collision_box_src->size.x / 2,
                    collision_box_dst->position.x - collision_box_dst->size.x / 2)
                <= collision_box_src->size.x + collision_box_dst->size.x);
            bool is_collide_y = (max(collision_box_src->position.y + collision_box_src->size.y / 2,
                collision_box_dst->position.y + collision_box_dst->size.y / 2)
                - min(collision_box_src->position.y - collision_box_src->size.y / 2,
                    collision_box_dst->position.y - collision_box_dst->size.y / 2)
                <= collision_box_src->size.y + collision_box_dst->size.y);

            if (is_collide_x && is_collide_y && collision_box_dst->on_collide)
                collision_box_dst->on_collide();
        }
    }
}

void CollisionManager::on_debug_render()
{
    for (CollisionBox* collision_box : collision_box_list)
    {
        setlinecolor(collision_box->enabled ? RGB(255, 195, 195) : RGB(115, 115, 175));
        rectangle((int)(collision_box->position.x - collision_box->size.x / 2),
            (int)(collision_box->position.y - collision_box->size.y / 2),
            (int)(collision_box->position.x + collision_box->size.x / 2),
            (int)(collision_box->position.y + collision_box->size.y / 2));
    }
}