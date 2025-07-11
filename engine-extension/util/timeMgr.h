#pragma once

#include <chrono>
#include <ctime>
#include <functional>
#include <thread>

namespace XD::Util::TimeMgr
{
    /// @brief 初始化
    void init();

    /// @brief 是否已经初始化
    /// @return 是否已经初始化
    bool inited();

    /// @brief 当前时间戳(从程序开始时计算)
    /// @return 当前时间戳
    clock_t now();

    /// @brief 当前时间点
    /// @return 当前时间点
    [[maybe_unused]] std::chrono::high_resolution_clock::time_point nowTimePoint();

    /// @brief 延迟回调
    /// @param cb 回调
    /// @param delay 延迟(ms)
    void delay(const std::function<void()>& cb, clock_t delay);

    /// @brief 更新事件
    void update();

    /// @brief 销毁
    void destroy();
} // namespace XD::Util::Time