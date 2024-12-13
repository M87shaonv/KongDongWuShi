#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "util.h"
#include "timer.h"
#include "atlas.h"
#include "vector2.h"

#include <vector>
#include <functional>
class Animation
{
public:
    enum class AnchorMode
    {
        Centered,
        BottomCentered
    };
public:
    Animation()
    {
        timer.set_one_shot(false);//���ö�ʱ��Ϊ��һ���Դ���
        timer.set_on_timeout(
            /*��� Lambda ���ʽ�������õķ�ʽ�����ⲿ�������е����б���
            �����Ҫ��ֵ�������������ʹ�� = �滻 &
            �����Ҫ�����ض������������� & �� = �����һ�������б��� [&var1, var2]*/
            [&]() {
                index_frame++;
                if (index_frame >= frame_list.size())
                {
                    index_frame = is_loop ? 0 : frame_list.size() - 1;
                    if (!is_loop && on_finished)//��ѭ�������лص�����
                        on_finished();
                }
            }
        );
    }
    ~Animation()
    {}

    void reset()
    {
        timer.restart();
        index_frame = 0;
    }
    void set_anchor_mode(AnchorMode mode)
    {
        anchor_mode = mode;
    }
    void set_position(const Vector2& position)
    {
        this->position = position;
    }
    void set_loop(bool is_loop)
    {
        this->is_loop = is_loop;
    }
    void set_interval(float interval)
    {
        timer.set_wait_time(interval);
    }
    void set_on_finished(std::function<void()> on_finished)
    {
        this->on_finished = on_finished;
    }
    void add_frame(IMAGE* image, int num_h)
    {
        int width = image->getwidth();
        int height = image->getheight();
        int width_frame = width / num_h;//����ÿ֡���

        for (size_t i = 0; i < num_h; i++)
        {
            Rect rect_src;
            rect_src.x = i * width_frame, rect_src.y = 0;//����Դ���ε����Ͻ�����
            rect_src.w = width_frame, rect_src.h = height;//����Դ���εĳߴ�

            frame_list.emplace_back(image, rect_src);//��֡��ӵ�֡�б�
        }
    }
    void add_frame(Atlas* atlas)
    {
        for (size_t i = 0; i < atlas->get_size(); i++)
        {
            IMAGE* image = atlas->get_image(i);
            int width = image->getwidth();
            int height = image->getheight();

            Rect rect_src;
            rect_src.x = 0, rect_src.y = 0;
            rect_src.w = width, rect_src.h = height;

            frame_list.emplace_back(image, rect_src);
        }
    }

    void on_update(float delta)
    {
        timer.on_update(delta);
    }
    void on_render()
    {
        const Frame& frame = frame_list[index_frame];
        Rect rect_dst;
        // ����Ŀ����ε�λ��
        rect_dst.x = (int)position.x - frame.rect_src.w / 2;
        rect_dst.y = (anchor_mode == AnchorMode::Centered)
            ? (int)position.y - frame.rect_src.h / 2 : (int)position.y - frame.rect_src.h;

        rect_dst.w = frame.rect_src.w, rect_dst.h = frame.rect_src.h;
        putimage_ex(frame.image, &rect_dst, &frame.rect_src);// ���Ƶ�ǰ֡
    }
private:
    struct Frame
    {
        Rect rect_src;// Դ����
        IMAGE* image = nullptr;//ͼ��ָ��

        Frame() = default;
        Frame(IMAGE* image, const Rect& rect_src) :image(image), rect_src(rect_src) {}
        ~Frame() = default;
    };
private:
    Timer timer;
    Vector2 position;
    bool is_loop = true;
    size_t index_frame = 0;
    std::vector<Frame> frame_list;
    std::function<void()> on_finished;
    AnchorMode anchor_mode = AnchorMode::Centered;
};
#endif // !_ANIMATION_H_
