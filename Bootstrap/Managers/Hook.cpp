#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef __ANDROID__
#include "AndroidData.h"
#endif

#include "Hook.h"
#include "../Utils/Console/Logger.h"
#include "../Utils/Console/Debug.h"

#include "../external/dobby/include/dobby.h"

void Hook::Attach(void** target, void* detour)
{
    Debug::Msg("[Hook::Attach] Reached!");
    void** origin = nullptr;
    int res = DobbyHook(*target, (dobby_dummy_func_t)detour, (dobby_dummy_func_t*)origin);
    *target = origin;
    // kode!
    std::string dobbyRes = "[Hook::Attach] [Dobby] " + std::to_string(res);
    Debug::Msg(dobbyRes.c_str());
}

void Hook::Detach(void** target)
{
    Debug::Msg("[Hook::Detatch] Reached!");
    int res = DobbyDestroy(*target);
    std::string dobbyRes = "[Hook::Detach] [Dobby] " + std::to_string(res);
    Debug::Msg(dobbyRes.c_str());
}