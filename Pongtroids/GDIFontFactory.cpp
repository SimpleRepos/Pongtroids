#include "GDIFontTool.h"

#pragma comment(lib, "gdiplus.lib")

namespace {
  struct GDI_RAII {
    GDI_RAII()  {
      auto status = Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
      if(status) { throw std::runtime_error("Failed to initialize GDI+"); }
    }

    ~GDI_RAII() {
      Gdiplus::GdiplusShutdown(gdiplusToken);
    }

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
  };
}

GDIFontTool::Font GDIFontTool::Factory::createFont(const std::wstring& face, float size, int style) {
  return Font(this, face, size, style);
}

GDIFontTool::Factory::Factory(HWND hwnd) :
  gdiGfx(bootStrapHack(hwnd)),
  brush(Gdiplus::Color(Gdiplus::Color::White))
{
  //nop
}

GDIFontTool::RectF GDIFontTool::Factory::getBounds(const Font& font, const std::wstring& text) {
  Gdiplus::RectF bounds;
  auto status = gdiGfx.MeasureString(text.c_str(), text.size(), font.font.get(), Gdiplus::PointF(0,0), &bounds);
  if(status) { throw std::runtime_error("GDIFontTool::Factory::getBounds() - failed to measure string"); }
  return GDIFontTool::RectF{ bounds.X, bounds.Y, bounds.Width, bounds.Height };
}

HWND GDIFontTool::Factory::bootStrapHack(HWND hwnd) {
  static GDI_RAII raiiObj;
  return hwnd;
}

