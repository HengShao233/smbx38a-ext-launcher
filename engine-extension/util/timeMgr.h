#pragma once

#include <chrono>
#include <ctime>
#include <functional>
#include <thread>

namespace XD::Util::TimeMgr
{
    /// @brief ��ʼ��
    void init();

    /// @brief �Ƿ��Ѿ���ʼ��
    /// @return �Ƿ��Ѿ���ʼ��
    bool inited();

    /// @brief ��ǰʱ���(�ӳ���ʼʱ����)
    /// @return ��ǰʱ���
    clock_t now();

    /// @brief ��ǰʱ���
    /// @return ��ǰʱ���
    [[maybe_unused]] std::chrono::high_resolution_clock::time_point nowTimePoint();

    /// @brief �ӳٻص�
    /// @param cb �ص�
    /// @param delay �ӳ�(ms)
    void delay(const std::function<void()>& cb, clock_t delay);

    /// @brief �����¼�
    void update();

    /// @brief ����
    void destroy();
} // namespace XD::Util::Time