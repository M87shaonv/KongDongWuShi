#ifndef _COLLISION_BOX_H_
#define _COLLISION_BOX_H_

#include "vector2.h"
#include "collision_layer.h"

#include <functional>

/*前向声明告诉编译器某个类或函数存在的方式，而不需要提供完整的类或函数定义
* 告诉编译器 CollisionManager 是一个类，但是它的完整定义在别处
这样做可以避免编译器在处理 CollisionBox 类时遇到 CollisionManager 的引用而不知道它是什么
这通常用于解决依赖问题，特别是当两个或多个类相互引用时*/
class CollisionManager;

class CollisionBox
{
    friend class CollisionManager;//允许CollisionManager类直接访问内部类的私有和保护成员
private:
    Vector2 size;
    Vector2 position;//中心点所处位置
    bool enabled = true;
    std::function<void()> on_collide;
    CollisionLayer layer_src = CollisionLayer::None;//自身所处碰撞层
    CollisionLayer layer_dst = CollisionLayer::None;//目标所处碰撞层

private://设为私有成员,意味着不能自行随意创建碰撞箱对象
    CollisionBox() = default;
    ~CollisionBox() = default;

public:
    void set_enabled(bool flag)
    {
        enabled = flag;
    }
    bool get_enabled() const
    {
        return enabled;
    }
    void set_layer_src(CollisionLayer layer)
    {
        layer_src = layer;
    }
    void set_layer_dst(CollisionLayer layer)
    {
        layer_dst = layer;
    }
    void set_one_collide(std::function<void()> on_collide)
    {
        this->on_collide = on_collide;
    }
    void set_size(const Vector2& size)
    {
        this->size = size;
    }
    const Vector2& get_size() const
    {
        return size;
    }
    void set_position(const Vector2& pos)
    {
        this->position = pos;
    }
};
#endif // !_COLLISION_BOX_H_
