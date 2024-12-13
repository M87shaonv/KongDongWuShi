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
        return *this; // ���ص�ǰ���������
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
    float length()//���������ĳ��ȣ���ģ��������ԭ�㵽�� (x, y) ��ֱ�߾���
    {
        return sqrt(x * x + y * y);
    }
    /*
    ���� length �������㵱ǰ�����ĳ��ȡ�
    ��鳤���Ƿ�Ϊ0���������Ϊ0������һ�������� (0, 0)����Ϊ�������޷���һ����
    ������Ȳ�Ϊ0����������ÿ���������Գ��ȣ��õ���һ���������
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
