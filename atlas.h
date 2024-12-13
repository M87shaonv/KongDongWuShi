#ifndef _ATLAS_H_
#define _ATLAS_H_

#include <vector>
#include <graphics.h>

class Atlas
{
public:
    Atlas() = default;
    ~Atlas() = default;

    void load(LPCTSTR path_template, int num)
    {
        clear();
        resize(num);

        TCHAR path_file[256];// 存储图像路径的缓冲区
        for (size_t i = 0; i < num; i++)
        {
            /*!!
             i + 1 是一个表达式，它返回 i 加 1 的结果，但不会改变 i 的值
             这意味着每次循环迭代时，i 的值保持不变，只是在格式化路径时使用了 i + 1 的值

             ++i 是前缀递增运算符，它会先将 i 的值加 1，然后再返回新的值。这意味着每次循环迭代时，i 的值都会被修改
            */
            _stprintf_s(path_file, path_template, i + 1);//格式化图像路径
            loadimage(&img_list[i], path_file);//加载图像到图像列表
        }
    }
    void clear()
    {
        img_list.clear();
    }
    void resize(int s)
    {
        img_list.resize(s);
    }
    int get_size() const
    {
        return (int)img_list.size();
    }
    //获取指定索引的图像
    IMAGE* get_image(int index)
    {
        if (index < 0 || index >= get_size())
            throw "图像索引错误";

        return &img_list[index];
    }
    void add_image(const IMAGE& img)
    {
        img_list.push_back(img);
    }
private:

    std::vector<IMAGE> img_list;// 存储图像的列表
};
#endif // !_ATLAS_H_
