#pragma once

#include <sstream>
#include "global_include.h"

#if defined(ANDROID)
#include <android/log.h>
#else
#include <iostream>
#include <ctime>
#endif

#define LogDebug() LogDebugDetailed(__FILENAME__, __LINE__)
#define LogInfo() LogInfoDetailed(__FILENAME__, __LINE__)
#define LogWarn() LogWarnDetailed(__FILENAME__, __LINE__)
#define LogErr() LogErrDetailed(__FILENAME__, __LINE__)

namespace mavsdk {

enum class Color { Red, Green, Yellow, Blue, Gray, Reset };

void set_color(Color color);

class LogDetailed {
public:
    LogDetailed(const char* filename, int filenumber) :
        _s(),
        _caller_filename(filename),
        _caller_filenumber(filenumber)
    {}

    template<typename T> LogDetailed& operator<<(const T& x)
    {
        _s << x;
        return *this;
    }

    virtual ~LogDetailed()
    {
#if ANDROID
        switch (_log_level) {
            case LogLevel::Debug:
                __android_log_print(ANDROID_LOG_DEBUG, "Mavsdk", "%s", _s.str().c_str());
                break;
            case LogLevel::Info:
                __android_log_print(ANDROID_LOG_INFO, "Mavsdk", "%s", _s.str().c_str());
                break;
            case LogLevel::Warn:
                __android_log_print(ANDROID_LOG_WARN, "Mavsdk", "%s", _s.str().c_str());
                break;
            case LogLevel::Err:
                __android_log_print(ANDROID_LOG_ERROR, "Mavsdk", "%s", _s.str().c_str());
                break;
        }
        // Unused:
        (void)_caller_filename;
        (void)_caller_filenumber;
#else

        switch (_log_level) {
            case LogLevel::Debug:
#ifdef NDEBUG
                return;
#endif
                set_color(Color::Green);
                break;
            case LogLevel::Info:
                set_color(Color::Blue);
                break;
            case LogLevel::Warn:
                set_color(Color::Yellow);
                break;
            case LogLevel::Err:
                set_color(Color::Red);
                break;
        }

        // Time output taken from:
        // https://stackoverflow.com/questions/16357999#answer-16358264
        time_t rawtime;
        time(&rawtime);
        struct tm* timeinfo = localtime(&rawtime);
        char time_buffer[10]{}; // We need 8 characters + \0
        strftime(time_buffer, sizeof(time_buffer), "%I:%M:%S", timeinfo);
        std::cout << "[" << time_buffer;

        switch (_log_level) {
            case LogLevel::Debug:
                std::cout << "|Debug] ";
                break;
            case LogLevel::Info:
                std::cout << "|Info ] ";
                break;
            case LogLevel::Warn:
                std::cout << "|Warn ] ";
                break;
            case LogLevel::Err:
                std::cout << "|Error] ";
                break;
        }

        set_color(Color::Reset);

        std::cout << _s.str();
        std::cout << " (" << _caller_filename << ":" << std::dec << _caller_filenumber << ")";

        std::cout << '\n';
#endif
    }

    LogDetailed(const mavsdk::LogDetailed&) = delete;
    void operator=(const mavsdk::LogDetailed&) = delete;

protected:
    enum LogLevel { Debug, Info, Warn, Err } _log_level = LogLevel::Debug;

private:
    std::stringstream _s;
    const char* _caller_filename;
    int _caller_filenumber;
};

class LogDebugDetailed : public LogDetailed {
public:
    LogDebugDetailed(const char* filename, int filenumber) : LogDetailed(filename, filenumber)
    {
        _log_level = LogLevel::Debug;
    }
};

class LogInfoDetailed : public LogDetailed {
public:
    LogInfoDetailed(const char* filename, int filenumber) : LogDetailed(filename, filenumber)
    {
        _log_level = LogLevel::Info;
    }
};

class LogWarnDetailed : public LogDetailed {
public:
    LogWarnDetailed(const char* filename, int filenumber) : LogDetailed(filename, filenumber)
    {
        _log_level = LogLevel::Warn;
    }
};

class LogErrDetailed : public LogDetailed {
public:
    LogErrDetailed(const char* filename, int filenumber) : LogDetailed(filename, filenumber)
    {
        _log_level = LogLevel::Err;
    }
};

} // namespace mavsdk
