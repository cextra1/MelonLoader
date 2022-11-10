#pragma once
#include <fstream>
#include <filesystem>
#include <iostream>
#include <mutex>
#include <string>
#include <jni.h>
#include "./Console.h"
#include "Log.h"


class Logger
{
    static std::mutex logMutex;

public:
    static int MaxLogs;
    static int MaxWarnings;
    static int MaxErrors;

    static int WarningCount;
    static int ErrorCount;

    static bool Initialize();
    static std::string GetTimestamp(std::string format = "%H:%M:%S");
    static void WriteSpacer();

    static void Internal_PrintModName(Console::Color meloncolor, Console::Color authorcolor, const char* name, const char* author, const char* version, const char* id);

    // Shorthand way to log to console and write to file with linebreak afterwards
    static void LogToConsoleAndFile(Log log);

    // Creates a log, and prints it to the console and file, all in one function.
    static void QuickLog(const char* txt, LogType logType = Msg)
    {LogToConsoleAndFile(Log(logType, nullptr, txt));}
    static void QuickLogf(const char* fmt, LogType logType, ...)
    {
        va_list args;
        va_start(args, fmt);
        // TODO: test this?
        QuickLog(string_format(fmt, args).c_str(), logType);
        va_end(args);
    }
    static void QuickLogf(const char* fmt, LogType logType, va_list args)
    {
        // TODO: test this?
        QuickLog(string_format(fmt, args).c_str(), logType);
    }

    static void Internal_Msg(Console::Color meloncolor, Console::Color txtcolor, const char* namesection, const char* txt);
    static void Internal_Warning(const char* namesection, const char* txt);
    static void Internal_Error(const char* namesection, const char* txt);

    class FileStream
    {
    public:
        std::ofstream coss;
        std::ofstream latest;
        template <class T>
        FileStream& operator<< (T val)
        {
            if (coss.is_open()) coss << val;
            if (latest.is_open()) latest << val;
            return *this;
        }
        FileStream& operator<< (std::ostream& (*pfun)(std::ostream&))
        {
            if (coss.is_open()) pfun(coss);
            if (latest.is_open()) pfun(latest);
            return *this; }
        void Flush() { if (coss.is_open()) coss.flush(); if (latest.is_open()) latest.flush(); }
    };
    static FileStream LogFile;
    static void Flush() { LogFile.Flush(); }

    static const char* LatestLogFileName;
    static const char* FileExtension;

private:
    static const char* FilePrefix;
    static void CleanOldLogs(const char* path);
    static std::string JavaInitialize();
    static std::string jstring2string(JNIEnv *env, jstring jStr);
    static bool CompareWritetime(const std::filesystem::directory_entry& first, const std::filesystem::directory_entry& second) { return first.last_write_time().time_since_epoch() >= second.last_write_time().time_since_epoch(); }
    static std::string string_format(const std::string& format, ...)
    {
        va_list args;
        int size_s = std::snprintf( nullptr, 0, format.c_str(), args ) + 1; // Extra space for '\0'
        if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
        auto size = static_cast<size_t>( size_s );
        std::unique_ptr<char[]> buf( new char[ size ] );
        std::snprintf( buf.get(), size, format.c_str(), args );
        return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
    }
};