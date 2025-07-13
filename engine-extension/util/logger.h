#pragma once

namespace ExEngine::Logger
{
  bool IsDebug() noexcept;

  void Info(const char* msg, ...) noexcept;
  void Warning(const char* msg, ...) noexcept;
  void Error(const char* msg, ...) noexcept;

  void RuntimeLog(const char* msg, ...) noexcept;
  void Assert(bool condition, const char* msg, ...);
}