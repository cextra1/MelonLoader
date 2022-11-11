#include "Log.h"

#include "Logger.h"
#ifdef __ANDROID__
#include <android/log.h>
#endif

void Log::BuildConsoleString(std::ostream& stream) const
{
    // Always initialize stream with timestamp
    stream <<
           Console::ColorToAnsi(logMeta->GetColorOverride(Console::Color::Gray)) <<
           "[" <<
           Console::ColorToAnsi(logMeta->GetColorOverride(Console::Color::Green)) <<
           Logger::GetTimestamp() <<
           Console::ColorToAnsi(logMeta->GetColorOverride(Console::Color::Gray)) <<
           "] ";

    // If the logging melon has a name, print it
    if (namesection != nullptr) stream << "[" <<
                                       Console::ColorToAnsi(melonAnsiColor) <<
                                       namesection <<
                                       Console::ColorToAnsi(logMeta->GetColorOverride(Console::Color::Gray)) <<
                                       "] ";

    // Print the [LOGTYPE] prefix if needed
    if (logMeta->printLogTypeName) stream << "[" << Console::ColorToAnsi(logMeta->logCategoryColor) << logMeta->logTypeString << Console::ColorToAnsi(logMeta->GetColorOverride(Console::Color::Gray)) << "] ";

    // If we're not coloring the whole line, use the specified input text color. If we are, the color would already be declared
    if (!logMeta->colorFullLine) stream << Console::ColorToAnsi(textAnsiColor);

    stream << txt <<
           Console::ColorToAnsi(logMeta->GetColorOverride(Console::Color::Gray), false);
}

void Log::BuildConsoleString() const
{
#ifndef __ANDROID__
    // Always initialize stream with timestamp
    std::string log =
            Console::ColorToAnsi(logMeta->GetColorOverride(Console::Color::Gray)) +
            "[" +
            Console::ColorToAnsi(logMeta->GetColorOverride(Console::Color::Green)) +
            Logger::GetTimestamp() +
            Console::ColorToAnsi(logMeta->GetColorOverride(Console::Color::Gray)) +
            "] ";
#else
    std::string log = Console::ColorToAnsi(logMeta->GetColorOverride(Console::Color::Gray));
#endif

    // If the logging melon has a name, print it
    if (namesection != nullptr) log += "[" +
                                       Console::ColorToAnsi(melonAnsiColor) +
                                       namesection +
                                       Console::ColorToAnsi(logMeta->GetColorOverride(Console::Color::Gray)) +
                                       "] ";

    // Print the [LOGTYPE] prefix if needed
    if (logMeta->printLogTypeName) log += "[" + Console::ColorToAnsi(logMeta->logCategoryColor) + logMeta->logTypeString + Console::ColorToAnsi(logMeta->GetColorOverride(Console::Color::Gray)) + "] ";

    // If we're not coloring the whole line, use the specified input text color. If we are, the color would already be declared
    if (!logMeta->colorFullLine) log += Console::ColorToAnsi(textAnsiColor);

    log += txt + Console::ColorToAnsi(Console::Color::Gray);

#ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_INFO, "MelonLoader", "%s", log.c_str());
#endif
}

std::string Log::BuildLogString() const
{
    std::string logStr = "[" + Logger::GetTimestamp() + "] ";
    if (namesection != nullptr)	logStr = logStr + "[" + namesection + "] ";
    if (logMeta->printLogTypeName) logStr = logStr + "[" + logMeta->logTypeString + "] ";
    return logStr + txt;
}