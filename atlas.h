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

        TCHAR path_file[256];// �洢ͼ��·���Ļ�����
        for (size_t i = 0; i < num; i++)
        {
            /*!!
             i + 1 ��һ�����ʽ�������� i �� 1 �Ľ����������ı� i ��ֵ
             ����ζ��ÿ��ѭ������ʱ��i ��ֵ���ֲ��䣬ֻ���ڸ�ʽ��·��ʱʹ���� i + 1 ��ֵ

             ++i ��ǰ׺����������������Ƚ� i ��ֵ�� 1��Ȼ���ٷ����µ�ֵ������ζ��ÿ��ѭ������ʱ��i ��ֵ���ᱻ�޸�
            */
            _stprintf_s(path_file, path_template, i + 1);//��ʽ��ͼ��·��
            loadimage(&img_list[i], path_file);//����ͼ��ͼ���б�
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
    //��ȡָ��������ͼ��
    IMAGE* get_image(int index)
    {
        if (index < 0 || index >= get_size())
            throw "ͼ����������";

        return &img_list[index];
    }
    void add_image(const IMAGE& img)
    {
        img_list.push_back(img);
    }
private:

    std::vector<IMAGE> img_list;// �洢ͼ����б�
};
#endif // !_ATLAS_H_
