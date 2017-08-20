#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include <string>
#include <memory>
#include <vector>

#include "st_ColorF.h"

namespace GDIFontTool {
  enum Style : int {
    Regular    = Gdiplus::FontStyleRegular,
    Bold       = Gdiplus::FontStyleBold,
    Italic     = Gdiplus::FontStyleItalic,
    Underline  = Gdiplus::FontStyleUnderline,
    Strikeout  = Gdiplus::FontStyleStrikeout
  };

  struct RawTexture {
    int width, height;
    std::vector<ColorF::uCol32> data;
  };

  struct RectF {
    float X, Y, Width, Height;
  };

  class Font;

  class Factory {
    friend class GDIFontTool::Font;
  public:
    Factory(HWND hwnd);
    GDIFontTool::Font createFont(const std::wstring& face, float size, int style = Regular);

  private:
    RectF getBounds(const GDIFontTool::Font& font, const std::wstring& text);
    HWND bootStrapHack(HWND hwnd);

    Gdiplus::Graphics gdiGfx;
    Gdiplus::SolidBrush brush;

  };

  class Font {
    friend class GDIFontTool::Factory;
  public:
    RawTexture print(const std::wstring& text);
    RawTexture print(const std::wstring& text, int borderWidth);
    RawTexture print(const std::wstring& text, int shadowXOffset, int shadowYOffset);

    void setFace(const std::wstring& face);
    void setSize(float size);
    void setStyle(int style);

    Font(const Font&);
    Font(Font&&);
    Font& operator=(const Font&);
    Font& operator=(Font&&);

  private:
    Font(GDIFontTool::Factory* factory, const std::wstring& face, float size, int style);

    RawTexture generateTexels(const std::wstring& text);
    void resetFont();

    std::wstring faceName;
    float size;
    GDIFontTool::Style style;
    std::unique_ptr<Gdiplus::FontFamily> fontFamily;
    std::unique_ptr<Gdiplus::Font> font;
    GDIFontTool::Factory* factory;

  };

}

