#include "hook/hook.h"
#include "util/logger.h"

#include "util/timeMgr.h"
#include "util/staticEventMgr.hpp"

#include "wnd/window.h"
#include "render/render.h"

// 外挂引擎启动
void MainStart()
{
  try
  {
    // enginex start
    ExEngine::Logger::Info("engine-ext: launch");

    // hook
    ExEngine::Hook::Attach();

    // init managers
    XD::Util::TimeMgr::init();
    XD::Event::StaticEventMgr::init();

    // init module
    ExEngine::Window::DoInit();
    ExEngine::Render::DoInit();
  }
  catch (const std::exception& e)
  {
    auto s = e.what();
    if (!s || strnlen_s(s, 1) <= 0) ExitProcess(-1);
    ExEngine::Logger::RuntimeLog("[Exce] %s", e.what());
    ExitProcess(-1);
  }
}

// smbx 的主循环, 由 D3d9 的 Present 函数带动
// 所以 D3d9 设备实例创建前这个函数不会运行(所以就刚打开程序而言会延迟一些)
void SmbxUpdate()
{
  try
  {
    // nothing to be done.
  }
  catch (const std::exception& e)
  {
    auto s = e.what();
    if (!s || strnlen_s(s, 1) <= 0) ExitProcess(-1);
    ExEngine::Logger::RuntimeLog("[Exce] %s", e.what());
    ExitProcess(-1);
  }
}

// 外挂引擎主循环(和 smbx 主循环不在一个线程, 涉及到数据交互时注意竞争问题)
void MainUpdate(float t)
{
  try
  {
    // update managers
    XD::Util::TimeMgr::update();
    XD::Event::StaticEventMgr::update();

    // update module
    ExEngine::Render::Update(t);
  }
  catch (const std::exception& e)
  {
    auto s = e.what();
    if (!s || strnlen_s(s, 1) <= 0) ExitProcess(-1);
    ExEngine::Logger::RuntimeLog("[Exce] %s", e.what());
    ExitProcess(-1);
  }
}

// 外挂引擎销毁前执行
void MainEnd()
{
  try
  {
    // de init module
    ExEngine::Render::DeInit();

    // destroy managers
    XD::Util::TimeMgr::destroy();
    XD::Event::StaticEventMgr::destroy();

    // un-hook
    ExEngine::Hook::Detach();
  }
  catch (const std::exception& e)
  {
    ExEngine::Logger::RuntimeLog("[Exce] %s", e.what());
  }

  // enginex end
  ExEngine::Logger::Info("engine-ext: destroy");
}