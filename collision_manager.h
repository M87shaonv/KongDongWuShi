#ifndef _COLLISITON_MANAGER_H_
#define _COLLISITON_MANAGER_H_

#include "collision_box.h"
#include <vector>

class CollisionManager
{
public:
    static CollisionManager* instance();

    CollisionBox* create_collision_box();
    void destory_collision_box(CollisionBox* collision_box);
    void process_collide();
    void on_debug_render();

private:
    static CollisionManager* manager;
    std::vector<CollisionBox*> collision_box_list;

private:
    CollisionManager();
    ~CollisionManager();
};
#endif // !_COLLISITON_MANAGER_H_