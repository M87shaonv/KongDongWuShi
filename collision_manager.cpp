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
    erase��vector���Ƴ�ָ����Χ��Ԫ��
    remove��vector���Ƴ�ָ��ֵ������Ԫ��,��������ɾ��Ԫ�أ����ǽ�Ҫ�Ƴ���Ԫ���Ƶ���Χ��ĩβ��������һ��������ָ���һ�����Ƴ���Ԫ�ص���λ��
    ���remove�Ὣ collision_box �ƶ��� collision_box_list ��ĩβ��������һ��������ָ���һ�����Ƴ���Ԫ�ص���λ��
    erase ��������������ʼ��ֱ�� collision_box_list ��ĩβ��ɾ��������ЩԪ��
    */
    collision_box_list.erase(std::remove(collision_box_list.begin(),
        collision_box_list.end(), collision_box), collision_box_list.end());

    delete collision_box;//�ͷ�ͨ�� new ��̬������ڴ�
}

void CollisionManager::process_collide()
{
    for (CollisionBox* collision_box_src : collision_box_list)
    {
        //ȷ������������ײ����ײĿ��㼶��Ϊ��
        if (!collision_box_src->enabled || collision_box_src->layer_dst == CollisionLayer::None)
            continue;
        for (CollisionBox* collision_box_dst : collision_box_list)
        {
            //�����δ������ײ�������ͬ�㼶�������ײ
            if (!collision_box_dst->enabled || collision_box_src == collision_box_dst
                || collision_box_src->layer_dst != collision_box_dst->layer_src)
                continue;
            /*
            collision_box_src->position.y + collision_box_src->size.y / 2��Դ���ε����ĵ� Y ����
            collision_box_dst->position.y + collision_box_dst->size.y / 2��Ŀ����ε����ĵ� Y ����
            collision_box_src->position.y - collision_box_src->size.y / 2��Դ���ε��±߽� Y ����
            collision_box_dst->position.y - collision_box_dst->size.y / 2��Ŀ����ε��±߽� Y ����
            max(collision_box_src->position.y + collision_box_src->size.y / 2,
            collision_box_dst->position.y + collision_box_dst->size.y / 2)�����������ϱ߽�����ֵ��
            min(collision_box_src->position.y - collision_box_src->size.y / 2,
            collision_box_dst->position.y - collision_box_dst->size.y / 2)�����������±߽��е���Сֵ
            max(collision_box_src->position.y + collision_box_src->size.y / 2,
            collision_box_dst->position.y + collision_box_dst->size.y / 2)
            - min(collision_box_src->position.y - collision_box_src->size.y / 2,
            collision_box_dst->position.y - collision_box_dst->size.y / 2)������������ Y ���ϵ��ص����ֵĳ���
            ����ص����ֵĳ���С�ڻ�����������εĸ߶�֮�ͣ�����Ϊ�� Y ���Ϸ�������ײ
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