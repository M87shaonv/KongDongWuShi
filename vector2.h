#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include <cmath>

class Vector2
{
public:
    float x = 0;
    float y = 0;

public:
    Vector2() = default;
    ~Vector2() = default;

    Vector2(float x, float y) :x(x), y(y) {}

    Vector2 operator+(const Vector2& vec) const
    {
        return Vector2(x + vec.x, y + vec.y);
    }
    Vector2& operator+=(const Vector2& vec)
    {
        x += vec.x, y += vec.y;
        return *this; // 返回当前对象的引用
    }
    Vector2 operator-(const Vector2& vec) const
    {
        return Vector2(x - vec.x, y - vec.y);
    }
    Vector2& operator-=(const Vector2& vec)
    {
        x -= vec.x, y -= vec.y;
        return *this;
    }
    Vector2 operator*(float val) const
    {
        return Vector2(x * val, y * val);
    }
    Vector2 operator*(const Vector2& vec) const
    {
        return Vector2(x * vec.x, y * vec.y);
    }
    Vector2& operator*=(float val)
    {
        x *= val, y *= val;
        return *this;
    }
    float length()//计算向量的长度（或模），即从原点到点 (x, y) 的直线距离
    {
        return sqrt(x * x + y * y);
    }
    /*
    调用 length 函数计算当前向量的长度。
    检查长度是否为0。如果长度为0，返回一个零向量 (0, 0)，因为零向量无法归一化。
    如果长度不为0，将向量的每个分量除以长度，得到归一化后的向量
    */
    Vector2 normalize()
    {
        float len = length();

        if (len == 0)
            return Vector2(0, 0);

        return Vector2(x / len, y / len);
    }
};
#endif
