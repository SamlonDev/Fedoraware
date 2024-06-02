#include "../Hooks.h"

// Use a static function pointer for the hook, to avoid the need for a Hook object
static FN_CClient_Precipitation_Simulate Original_CClient_Precipitation_Simulate = nullptr;

// Use a typedef to make the function pointer declaration more readable
typedef void(__fastcall* FN_CClient_Precipitation_Simulate)(void* ecx, void* edx, float dt);

// Define the hook function with a more descriptive name
void
