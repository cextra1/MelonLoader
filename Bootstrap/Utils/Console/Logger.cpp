#include "Logger.h"
#include "../Assertion.h"
#include "Debug.h"
#include "../../Managers/Game.h"
#include "../../Core.h"
#include <stdio.h>

#ifdef _WIN32
#include <direct.h>
#elif defined(__ANDROID__)
#include <android/log.h>
#endif

#include <sstream>

#include <list>
#include <iostream>
#include <shared_mutex>
#include <filesystem>
namespace fs = std::filesystem;

const char* Logger::FilePrefix = "MelonLoader_";
const char* Logger::FileExtension = ".log";
const char* Logger::LatestLogFileName = "Latest";
int Logger::MaxLogs = 10;
int Logger::MaxWarnings = 100;
int Logger::MaxErrors = 100;
int Logger::WarningCount = 0;
int Logger::ErrorCount = 0;
std::mutex Logger::logMutex;
Logger::FileStream Logger::LogFile;

bool Logger::Initialize()
{
	if (Debug::Enabled)
	{
		MaxLogs = 0;
		MaxWarnings = 0;
		MaxErrors = 0;
	}

#ifdef __ANDROID__
    std::string timeStamp = GetTimestamp("%y-%m-%d_%OH-%OM-%OS");
    std::string baseFilePath = JavaInitialize();
    LogFile.coss = std::ofstream(baseFilePath + "/logs/" + FilePrefix + timeStamp + FileExtension);
    LogFile.latest = std::ofstream(baseFilePath + "/" + LatestLogFileName + FileExtension);
#else
    std::string logFolderPath = std::string(Core::BasePath) + "\\MelonLoader\\Logs";
	if (Core::DirectoryExists(logFolderPath.c_str()))
		CleanOldLogs(logFolderPath.c_str());
	else if (_mkdir(logFolderPath.c_str()) != 0)
	{
		Assertion::ThrowInternalFailure("Failed to Create Logs Folder!");
		return false;
	}
	std::chrono::system_clock::time_point now;
	std::chrono::milliseconds ms;
	std::tm bt;
	Core::GetLocalTime(&now, &ms, &bt);
	std::stringstream filepath;
	filepath << logFolderPath << "\\" << FilePrefix << std::put_time(&bt, "%y-%m-%d_%H-%M-%S") << "." << std::setfill('0') << std::setw(3) << ms.count() << FileExtension;
	LogFile.coss = std::ofstream(filepath.str());
	std::string latest_path = (std::string(Core::BasePath) + "\\MelonLoader\\" + LatestLogFileName + FileExtension);
	if (Core::FileExists(latest_path.c_str()))
		std::remove(latest_path.c_str());
	LogFile.latest = std::ofstream(latest_path.c_str());
#endif

	return true;
}

std::string Logger::JavaInitialize()
{
    auto env = Core::GetEnv();

    jclass jCore = env->FindClass("com/melonloader/helpers/nativehelpers/Logger");
    if (jCore == NULL)
    {
        Assertion::ThrowInternalFailure("Failed to find class com.melonloader.helpers.nativehelpers.Logger");
        return "";
    }

    jmethodID mid = env->GetStaticMethodID(jCore, "Initialize", "()Ljava/lang/String;");
    if (mid == NULL)
    {
        Assertion::ThrowInternalFailure("Failed to find method com.melonloader.helpers.nativehelpers.Logger.Initialize()");
        return "";
    }

    jobject jObj = env->CallStaticObjectMethod(jCore, mid);
    if (jObj == NULL)
    {
        Assertion::ThrowInternalFailure("Failed to invoke com.melonloader.helpers.nativehelpers.Logger.Initialize()");
        return "";
    }

    std::string filePath = jstring2string(env, (jstring)jObj);
    std::string log = "[Bootstrap] Current log path at: " + filePath;
    Logger::QuickLog(log.c_str());
    return filePath;
}

std::string Logger::jstring2string(JNIEnv *env, jstring jStr) {
    if (!jStr)
        return "";

    const jclass stringClass = env->GetObjectClass(jStr);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray) env->CallObjectMethod(jStr, getBytes, env->NewStringUTF("UTF-8"));

    size_t length = (size_t) env->GetArrayLength(stringJbytes);
    jbyte* pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    std::string ret = std::string((char *)pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
    return ret;
}

std::string Logger::GetTimestamp(std::string format)
{
    std::chrono::system_clock::time_point now;
    std::chrono::milliseconds ms;
    std::tm bt;
    Core::GetLocalTime(&now, &ms, &bt);
    std::stringstream timeStamp;
    timeStamp << std::put_time(&bt, format.c_str()) << "." << std::setfill('0') << std::setw(3) << ms.count();
    return timeStamp.str();
}

void Logger::LogToConsoleAndFile(Log log)
{
    std::lock_guard guard(logMutex);
#if __ANDROID__
    log.BuildConsoleString();
#else
    log.BuildConsoleString(std::cout);
#endif
    LogFile << log.BuildLogString();
    WriteSpacer();
}

void Logger::CleanOldLogs(const char* path)
{
#ifndef __ANDROID__
    if (MaxLogs <= 0)
        return;
    std::list<std::filesystem::directory_entry>entry_list;
    for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(path))
    {
        if (!entry.is_regular_file())
            continue;
        std::string entry_file = entry.path().filename().generic_string();
        if ((entry_file.rfind(FilePrefix, NULL) == NULL) && (entry_file.rfind(FileExtension) == (entry_file.size() - std::string(FileExtension).size())))
            entry_list.push_back(entry);
    }
    if (entry_list.size() < MaxLogs)
        return;
    entry_list.sort(Logger::CompareWritetime);
    for (std::list<std::filesystem::directory_entry>::iterator it = std::next(entry_list.begin(), (MaxLogs - 1)); it != entry_list.end(); ++it)
        remove((*it).path().u8string().c_str());
#endif
}

void Logger::WriteSpacer()
{
    LogFile << std::endl;
    std::cout << std::endl;
}

void Logger::Internal_PrintModName(Console::Color meloncolor, Console::Color authorcolor, const char* name, const char* author, const char* version, const char* id)
{
    // Not using log object for this as we're modifying conventional coloring
    std::string timestamp = GetTimestamp();
    LogFile << "[" << timestamp << "] " << name << " v" << version;

    std::cout
            << Console::ColorToAnsi(Console::Color::Gray)
            << "["
            << Console::ColorToAnsi(Console::Color::Green)
            << timestamp
            << Console::ColorToAnsi(Console::Color::Gray)
            << "] "
            << Console::ColorToAnsi(meloncolor)
            << name
            << Console::ColorToAnsi(Console::Color::Gray)
            << " v"
            << version;

    if (id != NULL)
    {
        LogFile << " (" << id << ")";

        std::cout
                << " ("
                << Console::ColorToAnsi(meloncolor)
                << id
                << Console::ColorToAnsi(Console::Color::Gray)
                << ")";
    }

    LogFile << std::endl;
    std::cout
            << std::endl
            << Console::ColorToAnsi(Console::Color::Gray, false);

    if (author != NULL)
    {
        timestamp = GetTimestamp();
        LogFile << "[" << timestamp << "] by " << author << std::endl;

        std::cout
                << Console::ColorToAnsi(Console::Color::Gray)
                << "["
                << Console::ColorToAnsi(Console::Color::Green)
                << timestamp
                << Console::ColorToAnsi(Console::Color::Gray)
                << "] by "
                << Console::ColorToAnsi(authorcolor)
                << author
                << std::endl
                << Console::ColorToAnsi(Console::Color::Gray, false);
    }
}

void Logger::Internal_Msg(Console::Color meloncolor, Console::Color txtcolor, const char* namesection, const char* txt)
{
    LogToConsoleAndFile(Log(Msg, meloncolor, txtcolor, namesection, txt));
}

void Logger::Internal_Warning(const char* namesection, const char* txt)
{
    if (MaxWarnings > 0)
    {
        if (WarningCount >= MaxWarnings)
            return;
        WarningCount++;
    }
    else if (MaxWarnings < 0)
        return;

    LogToConsoleAndFile(Log(Warning, namesection, txt));
}

void Logger::Internal_Error(const char* namesection, const char* txt)
{
    if (MaxErrors > 0)
    {
        if (ErrorCount >= MaxErrors)
            return;
        ErrorCount++;
    }
    else if (MaxErrors < 0)
        return;

    LogToConsoleAndFile(Log(Error, namesection, txt));
}