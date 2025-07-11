#include <fstream>

#include "hook/hook.h"
#include "util/logger.h"

#include "util/timeMgr.h"
#include "util/staticEventMgr.hpp"

#include "window.h"
#include "render/render.h"
#include "smbxContext.h"

// �����������
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

// smbx ����ѭ��, �� D3d9 �� Present ��������
// ���� D3d9 �豸ʵ������ǰ���������������(���Ծ͸մ򿪳�����Ի��ӳ�һЩ)
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

// ���������ѭ��(�� smbx ��ѭ������һ���߳�, �漰�����ݽ���ʱע�⾺������)
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

// �����������ǰִ��
void MainEnd()
{
	try
	{
		// de init module
		ExEngine::Render::DeInit();

		// destory managers
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
	ExEngine::Logger::Info("engine-ext: destory");
}