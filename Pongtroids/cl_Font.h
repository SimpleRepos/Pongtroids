//https://fw1.codeplex.com/

#pragma once
#include <string>
#include <atlbase.h>
#include "FW1FontWrapper_1_1/FW1FontWrapper.h"
#include "st_ColorF.h"
#include <DirectXMath.h>

///<summary>Class for encapsulating a font that can be used to render text - Generate from Graphics::createFont()</summary>
class Font {
public:
  ///<summary>Draw text to the backbuffer</summary>
  ///<param name="text">Text to draw</param>
  ///<param name="size">Font size in points</param>
  ///<param name="x">Viewport position for top-left of text</param>
  ///<param name="y">Viewport position for top-left of text</param>
  ///<param name="color">Color to draw in (note that there is automatic conversion from ColorF to uCol32)</param>
  void drawText(const std::wstring& text, float size, float x, float y, ColorF color);

  void drawBorderedText(const std::wstring& text, float size, float x, float y, int borderWidth, ColorF foreColor, ColorF backColor);
  void drawShadowedText(const std::wstring& text, float size, float x, float y, int shadowOffset, ColorF color, ColorF shadowColor);

  ///<summary>Get a rect indicating the offset and size of the text rendered from the target x,y position</summary>
  ///<param name="text">Text to draw</param>
  ///<param name="size">Font size in points</param>
  ///<returns>An LTRB float rect - All 4 values are offsets from the x,y coord that the text is to be drawn at.</returns>
  FW1_RECTF measure(const std::wstring& text, float size);

private:
  friend class GfxFactory;
  Font(IFW1Factory* factory, ID3D11Device* device, ID3D11DeviceContext* context, const std::wstring& fontFace);

  CComPtr<IFW1FontWrapper> wrapper;
  CComPtr<ID3D11DeviceContext> context;
  const std::wstring fontFam;

};

