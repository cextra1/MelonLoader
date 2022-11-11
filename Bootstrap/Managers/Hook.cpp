#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef __ANDROID__
#include "AndroidData.h"
#endif

#include "Hook.h"
#include "../Utils/Console/Logger.h"
#include "../Utils/Console/Debug.h"


#ifdef _WIN64
#include "../Base/MSDetours/detours_x64.h"
#elif _WIN32
#include "../Base/MSDetours/detours_x86.h"
#endif

#ifdef _WIN32
void Hook::Attach(void** target, void* detour)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(target, detour);
	DetourTransactionCommit();
}

void Hook::Detach(void** target, void* detour)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(target, detour);
	DetourTransactionCommit();
}
#endif

#ifdef __ANDROID__
std::unordered_map<void*, Hook::FunchookDef*> Hook::HookMap;

void Hook::Attach(void** target, void* detour)
{
    //Debug::Msg("attaching");

    int rv;
    void* trueTarget = detour;
    Hook::FunchookDef* handle = nullptr;

    if (HookMap.find(trueTarget) == HookMap.end())
    {
        HookMap[trueTarget] = handle = (Hook::FunchookDef*)malloc(sizeof(Hook::FunchookDef));
        handle->original = *target;
        handle->handle = funchook_create();
        rv = funchook_prepare(handle->handle, target, detour);
        if (rv != 0)
        {
            Logger::QuickLog("Failed to prepare hook", LogType::Error);
            return;
        }
    } else
        handle = HookMap[trueTarget];

    rv = funchook_install(handle->handle, 0);
    if (rv != 0)
    {
        Logger::QuickLogf("Failed to install hook (%d, %s)", LogType::Error, rv, funchook_error_message(handle->handle));
        return;
    }

    return;
}

void Hook::Detach(void** target, void* detour)
{
    Debug::Msg("detaching");

    int rv;

    void* trueTarget = detour;
    Hook::FunchookDef* handle = nullptr;

    if (HookMap.find(trueTarget) == HookMap.end())
    {
        Logger::QuickLog("Hook doesn't exist", LogType::Error);
        return;
    } else
        handle = HookMap[trueTarget];

    void* reset = handle->original;

    rv = funchook_uninstall(handle->handle, 0);
    if (rv != 0)
    {
        Logger::QuickLog("Failed to uninstall hook", LogType::Error);
        return;
    }

    *target = reset;

    return;
}
#endif

#ifdef __ANDROID__
bool Hook::FunchookPrepare() {
    funchook_set_debug_file((std::string(AndroidData::DataDir) + "/funchook.log").c_str());

    return true;
}
#endif