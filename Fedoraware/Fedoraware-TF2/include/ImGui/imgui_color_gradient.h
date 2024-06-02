// imgui_color_gradient.h
#pragma once

#include "imgui.h"
#include <list>
#include <array>
#include <memory>
#include <algorithm>

struct ImGradientMark {
	ImVec4 Color;
	float Position{}; // 0 to 1
};

class ImGradient {
public:
	ImGradient() = default;
	~ImGradient() = default;

	void GetColorAt(float position, ImVec4* color) const noexcept;
	void AddMark(float position, ImColor color) noexcept;
	void RemoveMark(const ImGradientMark* mark) noexcept;
	void ClearMarks() noexcept;
	void RefreshCache() noexcept;
	std::list<ImGradientMark>& GetMarks() noexcept { return Marks; }

	void ComputeColorAt(float position, ImVec4* color) const noexcept;

private:
	std::list<ImGradientMark> Marks;
	mutable std::array<ImVec4, 256> CachedValues;
};

namespace ImGui {

void GradientRect(ImGradient* gradient, const ImVec2& bar_pos, float maxWidth, float height);

bool GradientButton(ImGradient* gradient);

bool GradientEditor(const char* label, ImGradient* gradient,
	std::unique_ptr<ImGradientMark>& draggingMark,
	std::unique_ptr<ImGradientMark>& selectedMark);

} // namespace ImGui


// imgui_color_gradient.cpp
#include "imgui_color_gradient.h"

void ImGradient::GetColorAt(float position, ImVec4* color) const noexcept {
	ComputeColorAt(position, color);
}

void ImGradient::AddMark(float position, ImColor color) noexcept {
	Marks.emplace_back(position, color);
	RefreshCache();
}

void ImGradient::RemoveMark(const ImGradientMark* mark) noexcept {
	Marks.remove_if([mark](const ImGradientMark& m) { return &m == mark; });
	RefreshCache();
}

void ImGradient::ClearMarks() noexcept {
	Marks.clear();
	RefreshCache();
}

void ImGradient::ComputeColorAt(float position, ImVec4* color) const noexcept {
	// Implementation
}

void ImGradient::RefreshCache() noexcept {
	// Implementation
}

void ImGui::GradientRect(ImGradient* gradient, const ImVec2& bar_pos, float maxWidth, float height) {
	// Implementation
}

bool ImGui::GradientButton(ImGradient* gradient) {
	// Implementation
}

bool ImGui::GradientEditor(const char* label, ImGradient* gradient,
	std::unique_ptr<ImGradientMark>& draggingMark,
	std::unique_ptr<ImGradientMark>& selectedMark) {
	bool updated = false;

	if (ImGui::Begin(label, nullptr, ImGuiWindowFlags_NoMove)) {
		if (draggingMark && ImGui::IsWindowFocused() && ImGui::IsMouseDown(0)) {
			float mouseX = ImGui::GetIO().MousePos.x - bar_pos.x;
			float markPosition = mouseX / maxWidth;
			draggingMark->Position = std::clamp(markPosition, 0.0f, 1.0f);
			updated = true;
		}

		if (ImGui::IsMouseReleased(0) && draggingMark) {
			selectedMark = std::move(draggingMark);
			draggingMark.reset();
			updated = true;
		}

		// Draw gradient
		ImGui::SetCursorScreenPos(bar_pos);
		ImGui::InvisibleButton("gradient", ImVec2(maxWidth, height));
		ImVec4 col;
		gradient->GetColorAt(ImGui::GetCursorScreenPos().x / maxWidth, &col);
		ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(),
			col, col, ImColor(0, 0, 0, 0), ImColor(0, 0, 0, 0));

		// Draw marks
		std::for_each(gradient->GetMarks().begin(), gradient->GetMarks().end(),
			[&](const ImGradientMark& mark) {
				float markX = mark.Position * maxWidth;
				ImGui::GetWindowDrawList()->AddLine(ImVec2(markX, 0), ImVec2(markX, height), ImColor(0, 0, 0));
			});

		// Draw selected mark
		if (selectedMark) {
			float markX = selectedMark->Position * maxWidth;
			ImGui::GetWindowDrawList()->AddCircle(ImVec2(markX, height / 2), 4, ImColor(0, 0, 0));
		}
	}
	ImGui::End();

	return updated;
}
