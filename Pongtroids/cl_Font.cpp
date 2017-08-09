#include "cl_Font.h"

#pragma comment(lib, "FW1FontWrapper_1_1/FW1FontWrapper.lib")

void Font::drawText(const std::wstring& text, float size, float x, float y, ColorF color) {
  wrapper->DrawString(context, text.c_str(), size, x, y, color, FW1_RESTORESTATE);
}

FW1_RECTF Font::measure(const std::wstring& text, float size) {
  FW1_RECTF layoutRect = { 0, 0, 0, 0 };
  return wrapper->MeasureString(text.c_str(), fontFam.c_str(), size, &layoutRect, FW1_RESTORESTATE|FW1_NOWORDWRAP);
}

Font::Font(IFW1Factory* factory, ID3D11Device* device, ID3D11DeviceContext* context, const std::wstring& fontFace) :
  context(context),
  fontFam(fontFace)
{
  factory->CreateFontWrapper(device, fontFam.c_str(), &wrapper);
}

