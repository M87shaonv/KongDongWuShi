#ifndef _COLLISION_BOX_H_
#define _COLLISION_BOX_H_

#include "vector2.h"
#include "collision_layer.h"

#include <functional>

/*ǰ���������߱�����ĳ����������ڵķ�ʽ��������Ҫ�ṩ���������������
* ���߱����� CollisionManager ��һ���࣬�����������������ڱ�
���������Ա���������ڴ��� CollisionBox ��ʱ���� CollisionManager �����ö���֪������ʲô
��ͨ�����ڽ���������⣬�ر��ǵ������������໥����ʱ*/
class CollisionManager;

class CollisionBox
{
    friend class CollisionManager;//����CollisionManager��ֱ�ӷ����ڲ����˽�кͱ�����Ա
private:
    Vector2 size;
    Vector2 position;//���ĵ�����λ��
    bool enabled = true;
    std::function<void()> on_collide;
    CollisionLayer layer_src = CollisionLayer::None;//����������ײ��
    CollisionLayer layer_dst = CollisionLayer::None;//Ŀ��������ײ��

private://��Ϊ˽�г�Ա,��ζ�Ų����������ⴴ����ײ�����
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
