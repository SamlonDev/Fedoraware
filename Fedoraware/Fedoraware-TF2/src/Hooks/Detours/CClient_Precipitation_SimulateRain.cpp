#include "../Hooks.h"

// Use a typedef to make the hook signature more readable
typedef bool(*FN_CClient_Precipitation_SimulateRain)(void* ecx, void* edx, void* pParticle, float dt);

// Use a static variable for the hook to make it easier to access from other parts of the code
static Hook<FN_CClient_Precipitation_SimulateRain> g_hCClient_Precipitation_SimulateRain(
    g_Pattern.Find(L"client.dll", L"55 8B EC A1 ? ? ? ? 83 EC 60 F3 0F 10 40 ? 53 57 8B 7D 08 8B D9 F3 0F 5C 47 ?"
                                  L"F3 0F 10 8B ? ? ? ? F3 0F 5C C8 0F 57 C0 0F 2F C1 0F 87 ? ? ? ? 8B 07 B9 ?"
                                  L"? ? ? ? F3 0F 10 4D ?"));

bool CClient_Precipitation_SimulateRain_Hooked(void* ecx, void* edx, void* pParticle, float dt)
{
    // Do something before the original function is called

    // Call the original function
    bool result = g_hCClient_Precipitation_SimulateRain.Get()(ecx, edx, pParticle, dt);

    // Do something after the original function is called

    return result;
}
