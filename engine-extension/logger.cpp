#include "pch.h"
#include "logger.h"

#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <cassert>
#include <fstream>
#include <mutex>
#include <cstdarg>

#include <Windows.h>

namespace ExEngine::Logger { void __DebugLogUtil_DeInitLog(); }

static std::mutex G_MUTEX;
#define MUTEX std::lock_guard<std::mutex> lock(G_MUTEX)

constexpr auto Runtime_LOG_FILE = "../engine-ext.log";
static std::fstream* G_LOG_FILE_RUNTIME = nullptr;
static bool CheckLogFileState_RUNTIME()
{
	if (G_LOG_FILE_RUNTIME) return false;
	G_LOG_FILE_RUNTIME = new std::fstream();
	G_LOG_FILE_RUNTIME->open(Runtime_LOG_FILE, std::ios::out);
	if (!G_LOG_FILE_RUNTIME->is_open())
	{
		delete G_LOG_FILE_RUNTIME;
		G_LOG_FILE_RUNTIME = nullptr;
		return true;
	}
	return false;
}

static std::string CurrentDateTime()
{
	auto now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);
	std::tm tm_now{};
	localtime_s(&tm_now, &now_time);

	std::ostringstream oss;
	oss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");
	return oss.str();
}

void ExEngine::Logger::RuntimeLog(const char* msg, ...) noexcept
{
	MUTEX;
	if (CheckLogFileState_RUNTIME()) return;

	const int bufferSize = 1024;
	char buffer[bufferSize]{ 0 };

	va_list args;
	va_start(args, msg);
	std::vsnprintf(buffer, bufferSize, msg, args);
	va_end(args);

	*G_LOG_FILE_RUNTIME << "[-      ] [" << CurrentDateTime() << "] " << buffer << std::endl;
}

void ExEngine::Logger::Assert(bool cond, const char* msg, ...)
{
	MUTEX;
	if (cond) return;
	if (CheckLogFileState_RUNTIME()) return;

	const int bufferSize = 1024;
	char buffer[bufferSize]{ 0 };

	va_list args;
	va_start(args, msg);
	std::vsnprintf(buffer, bufferSize, msg, args);
	va_end(args);

	*G_LOG_FILE_RUNTIME << "[Assert ] [" << CurrentDateTime() << "] " << buffer << std::endl;
	throw std::exception("Runtime assert");
}

#if _DEBUG

constexpr auto LOG_FILE = "../engine-ext-debug.log";

static std::fstream* G_LOG_FILE = nullptr;
static bool CheckLogFileState()
{
	if (G_LOG_FILE) return false;
	G_LOG_FILE = new std::fstream();
	G_LOG_FILE->open(LOG_FILE, std::ios::out);
	if (!G_LOG_FILE->is_open())
	{
		delete G_LOG_FILE;
		G_LOG_FILE = nullptr;
		return true;
	}
	return false;
}
#define TEST_LOG_FILE if (CheckLogFileState()) return

void ExEngine::Logger::Info(const char* msg, ...) noexcept
{
	MUTEX;
	TEST_LOG_FILE;

	const int bufferSize = 1024;
	char buffer[bufferSize]{0};

	va_list args;
	va_start(args, msg);
	std::vsnprintf(buffer, bufferSize, msg, args);
	va_end(args);

	*G_LOG_FILE << "[info   ] [" << CurrentDateTime() << "] " << buffer << std::endl;
}

void ExEngine::Logger::Error(const char* msg, ...) noexcept
{
	MUTEX;
	TEST_LOG_FILE;

	const int bufferSize = 1024;
	char buffer[bufferSize]{ 0 };

	va_list args;
	va_start(args, msg);
	std::vsnprintf(buffer, bufferSize, msg, args);
	va_end(args);

	*G_LOG_FILE << "[Error  ] [" << CurrentDateTime() << "] " << buffer << std::endl;
}

void ExEngine::Logger::Warning(const char* msg, ...) noexcept
{
	MUTEX;
	TEST_LOG_FILE;

	const int bufferSize = 1024;
	char buffer[bufferSize]{ 0 };

	va_list args;
	va_start(args, msg);
	std::vsnprintf(buffer, bufferSize, msg, args);
	va_end(args);

	*G_LOG_FILE << "[Warning] [" << CurrentDateTime() << "] " << buffer << std::endl;
}

void ExEngine::Logger::__DebugLogUtil_DeInitLog()
{
	if (G_LOG_FILE)
	{
		G_LOG_FILE->close();
		delete G_LOG_FILE;
		G_LOG_FILE = nullptr;
	}

	if (G_LOG_FILE_RUNTIME)
	{
		G_LOG_FILE_RUNTIME->close();
		delete G_LOG_FILE_RUNTIME;
		G_LOG_FILE_RUNTIME = nullptr;
	}
}

#else

// nothing to be done
void ExEngine::Logger::__DebugLogUtil_DeInitLog()
{
	if (G_LOG_FILE_RUNTIME)
	{
		G_LOG_FILE_RUNTIME->close();
		delete G_LOG_FILE_RUNTIME;
		G_LOG_FILE_RUNTIME = nullptr;
	}
}

void ExEngine::Logger::Info(const char* msg, ...) noexcept {}
void ExEngine::Logger::Error(const char* msg, ...) noexcept {}
void ExEngine::Logger::Warning(const char* msg, ...) noexcept {}

#endif
