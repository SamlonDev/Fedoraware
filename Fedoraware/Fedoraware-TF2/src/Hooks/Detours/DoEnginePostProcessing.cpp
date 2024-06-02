#include "../Hooks.h"

// Use a typedef to make the hook declaration easier to read and write
typedef void(__cdecl *DoEnginePostProcessing_t)(int x, int y, int w, int h, bool bFlashlightIsOn, bool bPostVGui);

