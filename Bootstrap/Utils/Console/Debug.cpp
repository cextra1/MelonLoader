#include "Debug.h"
#include "Logger.h"
#include "../Assertion.h"
#include "./Logger.h"
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
    // TODO: test this?
    Logger::QuickLog(string_format(fmt, args).c_str());
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

std::string Debug::string_format(const std::string& format, ...)
{
    va_list args1, args2;
    va_start(args1, format);
    va_copy(args2, args1);

    int size_s = vsnprintf(nullptr, 0, format.c_str(), args1) + 1;

    if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
    auto size = static_cast<size_t>(size_s);
    std::unique_ptr<char[]> buf(new char[size]);

    vsnprintf(buf.get(), size, format.c_str(), args2);
    std::string str{ buf.get(), buf.get() + size - 1 };

    va_end(args1);
    va_end(args2);

    return str;
}