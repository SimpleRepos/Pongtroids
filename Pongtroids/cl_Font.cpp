#include "cl_Font.h"

#pragma comment(lib, "FW1FontWrapper_1_1/FW1FontWrapper.lib")

void Font::drawText(const std::wstring& text, float size, float x, float y, ColorF color) {
  wrapper->DrawString(context, text.c_str(), size, x, y, color, FW1_RESTORESTATE);
}

void Font::drawBorderedText(const std::wstring& text, float size, float x, float y, int borderWidth, ColorF foreColor, ColorF backColor) {
  for(int xo = -borderWidth; xo <= borderWidth; xo++) for(int yo = -borderWidth; yo <= borderWidth; yo++) {
    wrapper->DrawString(context, text.c_str(), size, x + xo, y + yo, backColor, FW1_RESTORESTATE);
  }
  wrapper->DrawString(context, text.c_str(), size, x, y, foreColor, FW1_RESTORESTATE);
}

void Font::drawShadowedText(const std::wstring& text, float size, float x, float y, int shadowOffset, ColorF color, ColorF shadowColor) {
  wrapper->DrawString(context, text.c_str(), size, x - shadowOffset, y + shadowOffset, shadowColor, FW1_RESTORESTATE);
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

