#ifndef _BUTTON_H_
#define _BUTTON_H_
#include <easyx.h>
#include <graphics.h>
class Button
{
public:
    int x, y, width, height;
    const wchar_t* text;
    IMAGE* image;  // ��ťͼƬ
    COLORREF color;  // ��ť��ɫ

    // ���캯��
    Button(int x, int y, int width, int height, const wchar_t* text = nullptr, IMAGE* image = nullptr, COLORREF color = LIGHTGRAY)
        : x(x), y(y), width(width), height(height), text(text), image(image), color(color)
    {}

    // ���ư�ť
    void draw() const
    {
        if (image)
        {
            // ʹ��ͼƬ���ư�ť
            Rect rect_dst = { x, y, width, height };
            putimage_ex(image, &rect_dst);
        }
        else
        {
            // ʹ����ɫ���ư�ť
            setfillcolor(color);
            solidrectangle(x, y, x + width, y + height);
        }

        // ���ư�ť����
        if (text)
        {
            settextstyle(24, 0, L"Arial Black");
            SIZE textSize;
            GetTextExtentPoint32(GetDC(GetHWnd()), text, wcslen(text), &textSize);
            int textX = x + (width - textSize.cx) / 2;
            int textY = y + (height - textSize.cy) / 2;
            setbkmode(TRANSPARENT); // ���ñ���ģʽΪ͸��
            settextcolor(RGB(255, 255, 255));  // �����ı���ɫΪ��ɫ
            outtextxy(textX, textY, text);
        }
    }

    // ��ť������
    bool is_clicked(int mouse_x, int mouse_y) const
    {
        return mouse_x >= x && mouse_x <= x + width &&
            mouse_y >= y && mouse_y <= y + height;
    }
};
// �� std::string ת��Ϊ LPCTSTR
LPCTSTR stringToLPCTSTR(const std::string& str)
{
    int wSize = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, 0, 0);
    LPWSTR widestr = new WCHAR[wSize];
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, widestr, wSize);
    return widestr;
}

void draw_text(int x, int y, int height, int width, const std::string& font, const std::string& text, int color)
{
// ����������ɫ
    settextcolor(color);

    // �� std::string ת��Ϊ LPCTSTR
    LPCTSTR lpctstrFont = stringToLPCTSTR(font);
    LPCTSTR lpctstrText = stringToLPCTSTR(text);
    setbkmode(TRANSPARENT); // ���ñ���ģʽΪ͸��
    // ����������ʽ
    settextstyle(height, width, lpctstrFont);

    // �������ֻ��Ƶ���ʼλ�ã�ʹ�������ʾ
    int text_width = textwidth(lpctstrText);
    int text_height = textheight(lpctstrText);
    int start_x = x - text_width / 2;
    int start_y = y + text_height / 2;

    // ��������
    outtextxy(start_x, start_y, lpctstrText);

    // �ͷ�ת������ַ����ڴ�
    delete[] lpctstrFont;
    delete[] lpctstrText;
}

#endif // !_BUTTON_H_