#ifndef _BUTTON_H_
#define _BUTTON_H_
#include <easyx.h>
#include <graphics.h>
class Button
{
public:
    int x, y, width, height;
    const wchar_t* text;
    IMAGE* image;  // 按钮图片
    COLORREF color;  // 按钮颜色

    // 构造函数
    Button(int x, int y, int width, int height, const wchar_t* text = nullptr, IMAGE* image = nullptr, COLORREF color = LIGHTGRAY)
        : x(x), y(y), width(width), height(height), text(text), image(image), color(color)
    {}

    // 绘制按钮
    void draw() const
    {
        if (image)
        {
            // 使用图片绘制按钮
            Rect rect_dst = { x, y, width, height };
            putimage_ex(image, &rect_dst);
        }
        else
        {
            // 使用颜色绘制按钮
            setfillcolor(color);
            solidrectangle(x, y, x + width, y + height);
        }

        // 绘制按钮文字
        if (text)
        {
            settextstyle(24, 0, L"Arial Black");
            SIZE textSize;
            GetTextExtentPoint32(GetDC(GetHWnd()), text, wcslen(text), &textSize);
            int textX = x + (width - textSize.cx) / 2;
            int textY = y + (height - textSize.cy) / 2;
            setbkmode(TRANSPARENT); // 设置背景模式为透明
            settextcolor(RGB(255, 255, 255));  // 设置文本颜色为白色
            outtextxy(textX, textY, text);
        }
    }

    // 按钮点击检测
    bool is_clicked(int mouse_x, int mouse_y) const
    {
        return mouse_x >= x && mouse_x <= x + width &&
            mouse_y >= y && mouse_y <= y + height;
    }
};
// 将 std::string 转换为 LPCTSTR
LPCTSTR stringToLPCTSTR(const std::string& str)
{
    int wSize = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, 0, 0);
    LPWSTR widestr = new WCHAR[wSize];
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, widestr, wSize);
    return widestr;
}

void draw_text(int x, int y, int height, int width, const std::string& font, const std::string& text, int color)
{
// 设置文字颜色
    settextcolor(color);

    // 将 std::string 转换为 LPCTSTR
    LPCTSTR lpctstrFont = stringToLPCTSTR(font);
    LPCTSTR lpctstrText = stringToLPCTSTR(text);
    setbkmode(TRANSPARENT); // 设置背景模式为透明
    // 设置文字样式
    settextstyle(height, width, lpctstrFont);

    // 计算文字绘制的起始位置，使其居中显示
    int text_width = textwidth(lpctstrText);
    int text_height = textheight(lpctstrText);
    int start_x = x - text_width / 2;
    int start_y = y + text_height / 2;

    // 绘制文字
    outtextxy(start_x, start_y, lpctstrText);

    // 释放转换后的字符串内存
    delete[] lpctstrFont;
    delete[] lpctstrText;
}

#endif // !_BUTTON_H_