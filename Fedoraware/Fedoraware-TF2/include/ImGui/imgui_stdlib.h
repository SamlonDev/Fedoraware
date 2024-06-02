#pragma once

#include <string>

namespace ImGui
{
    // ImGui::InputText() with std::string
    // Because text input needs dynamic resizing, we need to setup a callback to grow the capacity
    IMGUI_API bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr)
    {
        // Grow string capacity as needed
        if (str->capacity() < str->size() + 16)
            str->resize(str->size() + 16);

        // Use the regular InputText() function
        return ImGui::InputText(label, &(*str)[0], str->size(), flags, callback, user_data);
    }

    IMGUI_API bool InputTextMultiline(const char* label, std::string* str, const ImVec2& size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
    {
        // Grow string capacity as needed
        if (str->capacity() < str->size() + 4 * ImGui::GetTextLineHeight())
            str->resize(str->size() + 4 * ImGui::GetTextLineHeight());

        // Use the regular InputTextMultiline() function
        return ImGui::InputTextMultiline(label, &(*str)[0], size, str->size(), flags, callback, user_data);
    }

    IMGUI_API bool InputTextWithHint(const char* label, const char* hint, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
    {
        // Grow string capacity as needed
        if (str->capacity() < str->size() + 16)
            str->resize(str->size() + 16);

        // Use the regular InputTextWithHint() function
        return ImGui::InputTextWithHint(label, hint, &(*str)[0], str->size(), flags, callback, user_data);
    }
}
