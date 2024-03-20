#include "DrawUtils.h"

#include <ranges>

#include "../../../Features/Vars.h"
#include "../../Includes/icons.h"

namespace S
{
	MAKE_SIGNATURE(CDraw_GetIcon, CLIENT_DLL, "55 8B EC 81 EC ? ? ? ? 83 7D 0C ? 56", 0x0);
}

void ScreenSize_t::Update()
{
	I::EngineClient->GetScreenSize(this->w, this->h);
	this->c = (this->w / 2);
}



void CDraw::RemakeFonts()
{
	m_Fonts[FONT_ESP] = { 0x0, Vars::Fonts::FONT_ESP::szName.Value.c_str(), int(Vars::Fonts::FONT_ESP::nTall.Value * Vars::Menu::DPI.Value), Vars::Fonts::FONT_ESP::nWeight.Value, Vars::Fonts::FONT_ESP::nFlags.Value };
	m_Fonts[FONT_NAME] = { 0x0, Vars::Fonts::FONT_NAME::szName.Value.c_str(), int(Vars::Fonts::FONT_NAME::nTall.Value * Vars::Menu::DPI.Value), Vars::Fonts::FONT_NAME::nWeight.Value, Vars::Fonts::FONT_NAME::nFlags.Value };
	m_Fonts[FONT_INDICATORS] = { 0x0, Vars::Fonts::FONT_INDICATORS::szName.Value.c_str(), int(Vars::Fonts::FONT_INDICATORS::nTall.Value * Vars::Menu::DPI.Value), Vars::Fonts::FONT_INDICATORS::nWeight.Value, Vars::Fonts::FONT_INDICATORS::nFlags.Value };

	ReloadFonts();
}

void CDraw::ReloadFonts()
{
	for (auto& v : m_Fonts)
		I::MatSystemSurface->SetFontGlyphSet(v.dwFont = I::MatSystemSurface->CreateFont(), v.szName, v.nTall, v.nWeight, 0, 0, v.nFlags);
}



void CDraw::String(const Font_t& font, int x, int y, const Color_t& clr, const EAlign& align, const char* str, ...)
{
	if (str == nullptr)
		return;

	va_list va_alist;
	char cbuffer[1024] = { '\0' };
	wchar_t wstr[1024] = { '\0' };

	va_start(va_alist, str);
	vsprintf_s(cbuffer, str, va_alist);
	va_end(va_alist);

	wsprintfW(wstr, L"%hs", cbuffer);

	const auto dwFont = font.dwFont;

	int w = 0, h = 0; I::MatSystemSurface->GetTextSize(dwFont, wstr, w, h);
	switch (align)
	{
	case ALIGN_TOPLEFT: break;
	case ALIGN_TOP: x -= w / 2; break;
	case ALIGN_TOPRIGHT: x -= w; break;
	case ALIGN_LEFT: y -= h / 2; break;
	case ALIGN_CENTER: x -= w / 2; y -= h / 2; break;
	case ALIGN_RIGHT: x -= w; y -= h / 2; break;
	case ALIGN_BOTTOMLEFT: y -= h; break;
	case ALIGN_BOTTOM: x -= w / 2; y -= h; break;
	case ALIGN_BOTTOMRIGHT: x -= w; y -= h; break;
	}

	I::MatSystemSurface->DrawSetTextPos(x, y);
	I::MatSystemSurface->DrawSetTextFont(dwFont);
	I::MatSystemSurface->DrawSetTextColor(clr.r, clr.g, clr.b, clr.a);
	I::MatSystemSurface->DrawPrintText(wstr, wcslen(wstr));
}
void CDraw::String(const Font_t& font, int x, int y, const Color_t& clr, const EAlign& align, const wchar_t* str, ...)
{
	if (str == nullptr)
		return;

	va_list va_alist;
	wchar_t wstr[1024] = { '\0' };

	va_start(va_alist, str);
	vswprintf_s(wstr, str, va_alist);
	va_end(va_alist);

	const auto dwFont = font.dwFont;

	int w = 0, h = 0; I::MatSystemSurface->GetTextSize(dwFont, wstr, w, h);
	switch (align)
	{
	case ALIGN_TOPLEFT: break;
	case ALIGN_TOP: x -= w / 2; break;
	case ALIGN_TOPRIGHT: x -= w; break;
	case ALIGN_LEFT: y -= h / 2; break;
	case ALIGN_CENTER: x -= w / 2; y -= h / 2; break;
	case ALIGN_RIGHT: x -= w; y -= h / 2; break;
	case ALIGN_BOTTOMLEFT: y -= h; break;
	case ALIGN_BOTTOM: x -= w / 2; y -= h; break;
	case ALIGN_BOTTOMRIGHT: x -= w; y -= h; break;
	}

	I::MatSystemSurface->DrawSetTextPos(x, y);
	I::MatSystemSurface->DrawSetTextFont(dwFont);
	I::MatSystemSurface->DrawSetTextColor(clr.r, clr.g, clr.b, clr.a);
	I::MatSystemSurface->DrawPrintText(wstr, wcslen(wstr));
}

void CDraw::Line(int x, int y, int x1, int y1, const Color_t& clr)
{
	I::MatSystemSurface->DrawSetColor(clr.r, clr.g, clr.b, clr.a);
	I::MatSystemSurface->DrawLine(x, y, x1, y1);
}
void CDraw::Polygon(int count, Vertex_t* vertices, const Color_t& clr)
{
	static int id = 0;
	if (!I::MatSystemSurface->IsTextureIDValid(id))
		id = I::MatSystemSurface->CreateNewTextureID();

	I::MatSystemSurface->DrawSetColor(clr.r, clr.g, clr.b, clr.a);
	I::MatSystemSurface->DrawSetTexture(id);
	I::MatSystemSurface->DrawTexturedPolygon(count, vertices);
}

void CDraw::DrawFillTriangle(const std::array<Vec2, 3>& points, const Color_t& clr)
{
	std::array<Vertex_t, 3> vertices{ Vertex_t(points.at(0)), Vertex_t(points.at(1)), Vertex_t(points.at(2)) };
	Polygon(3, vertices.data(), clr);
}
void CDraw::DrawLineTriangle(const std::array<Vec2, 3>& points, const Color_t& clr)
{
	Line(points.at(0).x, points.at(0).y, points.at(1).x, points.at(1).y, clr);
	Line(points.at(1).x, points.at(1).y, points.at(2).x, points.at(2).y, clr);
	Line(points.at(2).x, points.at(2).y, points.at(0).x, points.at(0).y, clr);
}

void CDraw::FillRect(int x, int y, int w, int h, const Color_t& clr)
{
	I::MatSystemSurface->DrawSetColor(clr.r, clr.g, clr.b, clr.a);
	I::MatSystemSurface->DrawFilledRect(x, y, x + w, y + h);
}
void CDraw::LineRect(int x, int y, int w, int h, const Color_t& clr)
{
	I::MatSystemSurface->DrawSetColor(clr.r, clr.g, clr.b, clr.a);
	I::MatSystemSurface->DrawOutlinedRect(x, y, x + w, y + h);
}
void CDraw::GradientRect(int x, int y, int w, int h, const Color_t& top_clr, const Color_t& bottom_clr, bool horizontal)
{
	I::MatSystemSurface->DrawSetColor(top_clr.r, top_clr.g, top_clr.b, top_clr.a);
	I::MatSystemSurface->DrawFilledRectFade(x, y, x + w, y + h, top_clr.a, bottom_clr.a, horizontal);
	I::MatSystemSurface->DrawSetColor(bottom_clr.r, bottom_clr.g, bottom_clr.b, bottom_clr.a);
	I::MatSystemSurface->DrawFilledRectFade(x, y, x + w, y + h, top_clr.a, bottom_clr.a, horizontal);
}
void CDraw::FillRectOutline(int x, int y, int w, int h, const Color_t& clr, const Color_t& out)
{
	FillRect(x, y, w, h, clr);
	LineRect(x - 1, y - 1, w + 2, h + 2, out);
}
void CDraw::LineRectOutline(int x, int y, int w, int h, const Color_t& clr, const Color_t& out, bool inside)
{
	LineRect(x, y, w, h, clr);
	LineRect(x - 1, y - 1, w + 2, h + 2, out);
	if (inside)
		LineRect(x + 1, y + 1, w - 2, h - 2, out);
}
void CDraw::FillRectPercent(int x, int y, int w, int h, float t, const Color_t& clr, const Color_t& out, const EAlign& align, bool adjust)
{
	if (!adjust)
		FillRect(x - 1, y - 1, w + 2, h + 2, out);
	int nw = w, nh = h;
	switch (align)
	{
	case ALIGN_LEFT: nw *= t; break;
	case ALIGN_RIGHT: nw *= t; x += w - nw; break;
	case ALIGN_TOP: nh *= t; break;
	case ALIGN_BOTTOM: nh *= t; y += h - nh; break;
	}
	if (adjust)
		FillRect(x - 1, y - 1, nw + 2, nh + 2, out);
	FillRect(x, y, nw, nh, clr);
}
void CDraw::RoundRect(int x, int y, int w, int h, int radius, const Color_t& clr)
{
	Vertex_t roundsquare[64];

	for (int i = 0; i < 4; i++)
	{
		const int _x = x + ((i < 2) ? (w - radius) : radius);
		const int _y = y + ((i % 3) ? (h - radius) : radius);

		const float a = 90.f * i;

		for (int j = 0; j < 16; j++)
		{
			const float _a = DEG2RAD(a + j * 6.f);

			roundsquare[(i * 16) + j] = Vertex_t(Vector2D(_x + radius * sin(_a), _y - radius * cos(_a)));
		}
	}

	Polygon(64, roundsquare, clr);
}

void CDraw::FillCircle(int x, int y, float radius, int segments, const Color_t clr)
{
	static std::vector<Vertex_t> vertices = {};

	const float step = static_cast<float>(PI) * 2.0f / segments;
	for (float a = 0; a < PI * 2.0f; a += step)
		vertices.emplace_back(Vertex_t{ { radius * cosf(a) + x, radius * sinf(a) + y } });

	Polygon(segments, vertices.data(), clr);

	vertices.clear();
}
void CDraw::LineCircle(int x, int y, float radius, int segments, const Color_t& clr)
{
	I::MatSystemSurface->DrawSetColor(clr.r, clr.g, clr.b, clr.a);
	I::MatSystemSurface->DrawOutlinedCircle(x, y, radius, segments);
}

void CDraw::Texture(int x, int y, int w, int h, int id, const EAlign& align)
{
	switch (align)
	{
	case ALIGN_TOPLEFT: break;
	case ALIGN_TOP: x -= w / 2; break;
	case ALIGN_TOPRIGHT: x -= w; break;
	case ALIGN_LEFT: y -= h / 2; break;
	case ALIGN_CENTER: x -= w / 2; y -= h / 2; break;
	case ALIGN_RIGHT: x -= w; y -= h / 2; break;
	case ALIGN_BOTTOMLEFT: y -= h; break;
	case ALIGN_BOTTOM: x -= w / 2; y -= h; break;
	case ALIGN_BOTTOMRIGHT: x -= w; y -= h; break;
	}

	int nTexture = 0;
	if (ICONS::ID[id] != ICONS::UNDEFINED)
		nTexture = ICONS::ID[id];
	else
	{
		nTexture = I::MatSystemSurface->CreateNewTextureID();
		I::MatSystemSurface->DrawSetTextureFile(nTexture, ICONS::TEXTURE[id].c_str(), false, true);
		ICONS::ID[id] = nTexture;
	}

	I::MatSystemSurface->DrawSetColor(255, 255, 255, 255);
	I::MatSystemSurface->DrawSetTexture(nTexture);
	I::MatSystemSurface->DrawTexturedRect(x, y, x + w, y + h);
}
CHudTexture* CDraw::GetIcon(const char* szIcon, int eIconFormat) // Thanks myzarfin
{
	using FN = CHudTexture * (__stdcall*)(const char*, int);
	static auto fnGetIcon = S::CDraw_GetIcon.As<FN>();
	return fnGetIcon(szIcon, eIconFormat);
}
int CDraw::CreateTextureFromArray(const unsigned char* rgba, int w, int h)
{
	const int nTextureIdOut = I::MatSystemSurface->CreateNewTextureID(true);
	I::MatSystemSurface->DrawSetTextureRGBAEx(nTextureIdOut, rgba, w, h, IMAGE_FORMAT_RGBA8888);
	return nTextureIdOut;
}
void CDraw::DrawHudTexture(float x0, float y0, float s0, const CHudTexture* texture, Color_t col0)
{
	if (!texture)
		return;

	if (texture->bRenderUsingFont)
	{
		I::MatSystemSurface->DrawSetTextFont(texture->hFont);
		I::MatSystemSurface->DrawSetTextColor(col0.r, col0.g, col0.b, col0.a);
		I::MatSystemSurface->DrawSetTextPos(x0, y0);
		I::MatSystemSurface->DrawUnicodeChar(texture->cCharacterInFont);
	}
	else if (texture->textureId != -1)
	{
		I::MatSystemSurface->DrawSetTexture(texture->textureId);
		I::MatSystemSurface->DrawSetColor(col0.r, col0.g, col0.b, col0.a);
		I::MatSystemSurface->DrawTexturedSubRect(x0, y0, x0 + (texture->rc.right - texture->rc.left) * s0, y0 + (texture->rc.bottom - texture->rc.top) * s0, texture->texCoords[0], texture->texCoords[1], texture->texCoords[2], texture->texCoords[3]);
	}
}
void CDraw::DrawHudTextureByName(float x0, float y0, float s0, const char* textureName, Color_t col0)
{
	const CHudTexture* pIcon = GetIcon(textureName, 0);

	if (!pIcon)
		return;

	if (pIcon->bRenderUsingFont)
	{
		I::MatSystemSurface->DrawSetTextFont(pIcon->hFont);
		I::MatSystemSurface->DrawSetTextColor(col0.r, col0.g, col0.b, col0.a);
		I::MatSystemSurface->DrawSetTextPos(x0, y0);
		I::MatSystemSurface->DrawUnicodeChar(pIcon->cCharacterInFont);
	}
	else if (pIcon->textureId != -1)
	{
		I::MatSystemSurface->DrawSetTexture(pIcon->textureId);
		I::MatSystemSurface->DrawSetColor(col0.r, col0.g, col0.b, col0.a);
		I::MatSystemSurface->DrawTexturedSubRect(x0, y0, x0 + (pIcon->rc.right - pIcon->rc.left) * s0, y0 + (pIcon->rc.bottom - pIcon->rc.top) * s0, pIcon->texCoords[0], pIcon->texCoords[1], pIcon->texCoords[2], pIcon->texCoords[3]);
	}
}
void CDraw::Avatar(int x, int y, int w, int h, const uint32 nFriendID, const EAlign& align)
{
	if (const auto nID = static_cast<uint64>(nFriendID + 0x0110000100000000))
	{
		switch (align)
		{
		case ALIGN_TOPLEFT: break;
		case ALIGN_TOP: x -= w / 2; break;
		case ALIGN_TOPRIGHT: x -= w; break;
		case ALIGN_LEFT: y -= h / 2; break;
		case ALIGN_CENTER: x -= w / 2; y -= h / 2; break;
		case ALIGN_RIGHT: x -= w; y -= h / 2; break;
		case ALIGN_BOTTOMLEFT: y -= h; break;
		case ALIGN_BOTTOM: x -= w / 2; y -= h; break;
		case ALIGN_BOTTOMRIGHT: x -= w; y -= h; break;
		}

		if (m_Avatars.contains(nID))
		{
			// The avatar has been cached
			I::MatSystemSurface->DrawSetColor(255, 255, 255, 255);
			I::MatSystemSurface->DrawSetTexture(m_Avatars[nID]);
			I::MatSystemSurface->DrawTexturedRect(x, y, x + w, y + h);
		}
		else
		{
			// Retrieve the avatar
			const int nAvatar = g_SteamInterfaces.Friends->GetMediumFriendAvatar(CSteamID(nID));

			uint32 newW = 0, newH = 0;
			if (g_SteamInterfaces.Utils->GetImageSize(nAvatar, &newW, &newH))
			{
				const size_t nSize = 4 * newW * newH * sizeof(uint8);
				auto* pData = static_cast<uint8*>(std::malloc(nSize));
				if (!pData)
					return;

				if (g_SteamInterfaces.Utils->GetImageRGBA(nAvatar, pData, static_cast<int>(nSize)))
				{
					const int nTextureID = I::MatSystemSurface->CreateNewTextureID(true);
					if (I::MatSystemSurface->IsTextureIDValid(nTextureID))
					{
						I::MatSystemSurface->DrawSetTextureRGBA(nTextureID, pData, newW, newH, 0, false);
						m_Avatars[nID] = nTextureID;
					}
				}

				std::free(pData);
			}
		}
	}
}
void CDraw::ClearAvatarCache()
{
	for (const auto& id : m_Avatars | std::views::values)
	{
		I::MatSystemSurface->DeleteTextureByID(id);
		I::MatSystemSurface->DestroyTextureID(id);
	}

	m_Avatars.clear();
}

float CDraw::EaseOut(float start, float end, float speed)
{
	if (start > end || speed <= 1)
		return end;

	if (Timer())
	{
		if (start < end)
			return start * speed;
		return end;
	}
	return start;
}
float CDraw::EaseIn(float start, float end, float speed)
{
	if (start < end || speed >= 1)
		return end;

	if (Timer())
	{
		if (start > end)
			return start * speed;
		return end;
	}
	return start;
}
float CDraw::Linear(float start, float end, float speed)
{
	if (start < end)
		return start + speed;
	return end;
}
bool CDraw::Timer() // This is to make sure that the animations don't get calculated in a split-second
{
	const int t = clock();
	static int i = 0;

	if (t > i)
	{
		i += 1;
		return true;
	}
	return false;
}