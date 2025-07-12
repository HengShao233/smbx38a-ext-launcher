// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "Windows.h"
#include <string>

void MainEnd();
void MainStart();
void MainUpdate(float t);

namespace ExEngine::SMBX {
  void __LoadWindowSupplement();
  void __Init_WndName(const std::string& name);
}
namespace ExEngine::Logger { void __DebugLogUtil_DeInitLog(); }

#include <thread>
#include <atomic>
#include <chrono>
#include <fstream>

#include "../util/logger.h"

class Updater
{
public:
  const double TARGET_FPS = 120;
  const double FRAME_DURATION = 1000.0 / TARGET_FPS;

public:
  Updater() : _running(true)
  {
    // 初始化窗口名
    auto f = std::fstream("../assets/game/main", std::ios::in);
    if (f) {
      auto wndName = std::string();
      if (std::getline(f, wndName) && std::getline(f, wndName)) {
        ExEngine::SMBX::__Init_WndName(wndName);
        ExEngine::Logger::Info("set wnd name: %s", wndName.c_str());
      }
    }

    MainStart();

    _updateThread = std::make_unique<std::thread>(&Updater::Update, this);
  }

  ~Updater()
  {
    _running = false;
    if (_updateThread->joinable()) {
      _updateThread->join();
    }
    MainEnd();
    ExEngine::Logger::__DebugLogUtil_DeInitLog();
  }

private:
  void Update()
  {
    using clock = std::chrono::steady_clock;
    std::chrono::duration<double, std::milli> time = std::chrono::duration<double, std::milli>::zero();

    while (_running)
    {
      auto start = clock::now();
      ExEngine::SMBX::__LoadWindowSupplement();
      MainUpdate((float)(time.count() / 1000.0));

      // 如果帧率过高就空转
      do {
        auto end = clock::now();
        time = end - start;
      } while (time.count() < FRAME_DURATION);

      if (!_running) break;
    }
  }

  std::unique_ptr<std::thread> _updateThread;
  std::atomic_bool _running;
};
static std::unique_ptr<Updater> G_UPD = nullptr;

extern "C" __declspec(dllexport) void engineStart()
{
  if (G_UPD) return;
  G_UPD = std::make_unique<Updater>();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) { return TRUE; }