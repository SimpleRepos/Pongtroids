#include "GDIFontTool.h"

using namespace Gdiplus;

GDIFontTool::Font::Font(Factory* factory, const std::wstring& face, float size, int style) :
  size(size),
  style((Style)style),
  factory(factory)
{
  //setting the face regenerates the other attributes,
  //so this will bootstrap the object effectively
  setFace(face);
}

GDIFontTool::Font::Font(const Font& other) :
  faceName(other.faceName),
  size(other.size),
  fontFamily(other.fontFamily->Clone()),
  font(other.font->Clone()),
  factory(other.factory)
{
  if(fontFamily->GetLastStatus()) { throw std::runtime_error("GDIFontTool::Font::Font() - Failed to create internal GDI font family."); }
  if(font->GetLastStatus())       { throw std::runtime_error("GDIFontTool::Font::Font() - Failed to create internal GDI font object"); }
}

GDIFontTool::Font::Font(Font&& other) :
  faceName(std::move(other.faceName)),
  size(other.size),
  fontFamily(other.fontFamily.release()),
  font(other.font.release()),
  factory(other.factory)
{
  //nop
}

GDIFontTool::Font& GDIFontTool::Font::operator=(const Font& other) {
  faceName = other.faceName;
  size = other.size;
  fontFamily.reset(other.fontFamily->Clone());
  if(fontFamily->GetLastStatus()) { throw std::runtime_error("GDIFontTool::Font::operator=() - Failed to create internal GDI font family."); }
  font.reset(other.font->Clone());
  if(font->GetLastStatus())       { throw std::runtime_error("GDIFontTool::Font::operator=() - Failed to create internal GDI font object"); }
  return *this;
}

GDIFontTool::Font& GDIFontTool::Font::operator=(Font&& other) {
  faceName = std::move(other.faceName);
  size = other.size;
  fontFamily.reset(other.fontFamily.release());
  font.reset(other.font.release());
  return *this;
}

GDIFontTool::RawTexture GDIFontTool::Font::print(const std::wstring& text) {
  return generateTexels(text);
}

GDIFontTool::RawTexture GDIFontTool::Font::print(const std::wstring & text, int borderWidth) {
  RawTexture map = generateTexels(text);

  //addative-blur the red channel into the green channel with a kernel of the user-specified width
  for(int dx = 0; dx < map.width; dx++) for(int dy = 0; dy < map.height; dy++) {
    auto& dest = map.data[dx + (dy * map.width)];

    for(int ox = -borderWidth; ox <= borderWidth; ox++) for(int oy = -borderWidth; oy <= borderWidth; oy++) {
      int sx = dx + ox;
      int sy = dy + oy;
      if(sx < 0 || sx >= map.width) { continue; }
      if(sy < 0 || sy >= map.height) { continue; }
      auto src = map.data[sx + (sy * map.width)];

      int sum = dest.g + src.r;
      if(sum > 0xFF) { sum = 0xFF; }
      dest.g = (uint8_t)sum;
    }

    //recalc alpha
    int sum = dest.r + dest.g;
    if(sum > 0xFF) { sum = 0xFF; }
    dest.a = (uint8_t)sum;
  }

  return map;
}

GDIFontTool::RawTexture GDIFontTool::Font::print(const std::wstring & text,int shadowXOffset,int shadowYOffset) {
  RawTexture map = generateTexels(text);

  for(int sx = 0; sx < map.width; sx++) for(int sy = 0; sy < map.height; sy++) {
    auto src = map.data[sx + (sy * map.width)];
    int dx = sx + shadowXOffset;
    int dy = sy + shadowYOffset;
    if(dx < 0 || dx >= map.width) { continue; }
    if(dy < 0 || dy >= map.height) { continue; }
    auto& dest = map.data[dx + (dy * map.width)];

    dest.g = src.r;
    if(src.a > dest.a) { dest.a = src.a; }
  }

  return map;
}

void GDIFontTool::Font::setFace(const std::wstring& face) {
  faceName = face;
  fontFamily = std::make_unique<FontFamily>(faceName.c_str());
  if(fontFamily->GetLastStatus()) { throw std::runtime_error("GDIFontTool::Font::setFace() - Failed to create internal GDI font family."); }
  resetFont();
}

void GDIFontTool::Font::setSize(float newSize) {
  size = newSize;
  resetFont();
}

void GDIFontTool::Font::setStyle(int newStyle) {
  style = (Style)newStyle;
  resetFont();
}

GDIFontTool::RawTexture GDIFontTool::Font::generateTexels(const std::wstring& text) {
  RectF bounds = factory->getBounds(*this, text);

  BitmapData data;
  data.Width  = (UINT)bounds.Width  + 1;
  data.Height = (UINT)bounds.Height + 1;
  data.Stride = data.Width * 4;
  data.PixelFormat = PixelFormat32bppARGB;
    RawTexture tex;
    tex.width  = (int)data.Width;
    tex.height = (int)data.Height;
    tex.data.resize(data.Width * data.Height);
  data.Scan0 = tex.data.data();

  Bitmap bmp(data.Width, data.Height, data.PixelFormat);
  if(bmp.GetLastStatus()) { throw std::runtime_error("GDIFontTool::Font::generateTexels() - Failed to generate GDI render target."); }

  Gdiplus::Graphics gg(&bmp);
  if(gg.GetLastStatus()) { throw std::runtime_error("GDIFontTool::Font::generateTexels() - Failed to create GDI renderer."); }

  auto status = gg.DrawString(text.c_str(), text.size(), font.get(), PointF(0,0), &factory->brush);
  if(status) { throw std::runtime_error("GDIFontTool::Font::generateTexels() - Failed DrawString call"); }

  UINT lockFlags = ImageLockModeRead|ImageLockModeUserInputBuf;
  //using an existing buffer, so this will fill tex.data as soon as the lock is complete
  //and it can be immediately unlocked since copying the data is all that's needed
  status = bmp.LockBits(&Rect(0, 0, data.Width, data.Height), lockFlags, data.PixelFormat, &data);
  if(status) { throw std::runtime_error("GDIFontTool::Font::generateTexels() - unable to lock GDI render target"); }
  status = bmp.UnlockBits(&data);
  if(status) { throw std::runtime_error("GDIFontTool::Font::generateTexels() - failed to unlock GDI render target"); }

  for(auto& texel : tex.data) {
    uint32_t alpha = (((uint32_t)texel.r + (uint32_t)texel.g + (uint32_t)texel.b) / 3);
    texel.r = (uint8_t)alpha;
    texel.g = 0;
    texel.b = 0;
    texel.a = texel.r;
  }

  return tex;
}

void GDIFontTool::Font::resetFont() {
  font = std::make_unique<Gdiplus::Font>(fontFamily.get(), size, style, UnitPixel);
  if(font->GetLastStatus()) { throw std::runtime_error("GDIFontTool::Font::resetFont() - Failed to create internal GDI font object"); }
}


