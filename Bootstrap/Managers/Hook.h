#pragma once
#include <vector>
#include <unordered_map>

class Hook
{
public:
    static void Attach(void** target, void* detour);
    static void Detach(void** target);
};
