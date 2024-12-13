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
        timer.set_one_shot(false);//设置定时器为非一次性触发
        timer.set_on_timeout(
            /*这个 Lambda 表达式会以引用的方式捕获外部作用域中的所有变量
            如果需要以值捕获变量，可以使用 = 替换 &
            如果需要捕获特定变量，可以在 & 或 = 后面跟一个变量列表，如 [&var1, var2]*/
            [&]() {
                index_frame++;
                if (index_frame >= frame_list.size())
                {
                    index_frame = is_loop ? 0 : frame_list.size() - 1;
                    if (!is_loop && on_finished)//不循环并且有回调函数
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
        int width_frame = width / num_h;//计算每帧宽度

        for (size_t i = 0; i < num_h; i++)
        {
            Rect rect_src;
            rect_src.x = i * width_frame, rect_src.y = 0;//设置源矩形的左上角坐标
            rect_src.w = width_frame, rect_src.h = height;//设置源矩形的尺寸

            frame_list.emplace_back(image, rect_src);//将帧添加到帧列表
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
        // 计算目标矩形的位置
        rect_dst.x = (int)position.x - frame.rect_src.w / 2;
        rect_dst.y = (anchor_mode == AnchorMode::Centered)
            ? (int)position.y - frame.rect_src.h / 2 : (int)position.y - frame.rect_src.h;

        rect_dst.w = frame.rect_src.w, rect_dst.h = frame.rect_src.h;
        putimage_ex(frame.image, &rect_dst, &frame.rect_src);// 绘制当前帧
    }
private:
    struct Frame
    {
        Rect rect_src;// 源矩形
        IMAGE* image = nullptr;//图像指针

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
