#include "Debug.h"
#include "Logger.h"
#include "../Assertion.h"
#include "./Logger.h"
#include "../Encoding.h"
#include <iostream>
#include <string>

#ifdef DEBUG
bool Debug::Enabled = true;
#else
bool Debug::Enabled = false;
#endif

void Debug::Msg(const char* txt)
{
    if (!Enabled || !Assertion::ShouldContinue)
        return;
    DirectWrite(txt);
}

void Debug::Msgf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    Logger::QuickLog(Encoding::string_format(fmt, args).c_str());
    va_end(args);
}

void Debug::DirectWrite(const char* txt)
{
    Logger::LogToConsoleAndFile(Log(LogType::Debug, nullptr, txt));
}

void Debug::Internal_Msg(Console::Color meloncolor, Console::Color txtcolor, const char* namesection, const char* txt)
{
    if (!Enabled || !Assertion::ShouldContinue)
        return;

    Logger::LogToConsoleAndFile(Log(LogType::Debug, meloncolor, txtcolor, namesection, txt));
}