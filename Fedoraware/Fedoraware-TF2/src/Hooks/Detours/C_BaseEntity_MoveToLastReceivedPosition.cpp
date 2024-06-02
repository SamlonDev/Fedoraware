#include "../Hooks.h"

// Use a static function pointer for the hook, to avoid the need for a HOOK_FUNC macro
static FN_C_BaseEntity_MoveToLastReceivedPosition Original_C_BaseEntity_MoveToLastReceivedPosition = nullptr;

// Rename the hook function to follow naming conventions and include the function signature
void Hook_C_BaseEntity_MoveToLastReceivedPosition(void* ecx, void* edx, bool force)
{
    // Call the original function using the function pointer
    return Original_C_BaseEntity_MoveToLastReceivedPosition(ecx, edx, force);
}

// Define the MAKE_HOOK macro to set up the hook and the function pointer
#define MAKE_HOOK(signature, pattern, ...) \
    MAKE_JMP_HOOK(C_BaseEntity_MoveToLastReceivedPosition, signature, pattern, &Hook_##signature, __VA_ARGS__) \
    Original_##signature = reinterpret_cast<FN_##signature>(pattern.GetOriginalFunction());

// Use the updated macro to define the hook
MAKE_HOOK(C_BaseEntity_MoveToLastReceivedPosition, g_Pattern.Find(L"client.dll", L"55 8B EC 80 7D 08 00 56 8B F1 75 0A 80 7E 58 17 0F 84 ? ? ? ? F3 0F 10 86 ? ? ? ? 0F 2E 86 ? ? ? ? 9F F6 C4 44 7A 2A F3 0F 10 86 ? ? ? ? 0F 2E 86 ? ? ? ? 9F F6 C4 44 7A 15 F3"), void, __fastcall)
