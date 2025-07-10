#include "pch.h"
#include "staticEventMgr.hpp"
#include "timeMgr.h"

namespace XD::Event
{
    std::unique_ptr<StaticEventMgr::EventMgrData> StaticEventMgr::_inst = nullptr;
    static clock_t startTime;
    static const clock_t maxWaitTime = 160;

    void StaticEventMgr::init()
    {
        _inst = std::make_unique<StaticEventMgr::EventMgrData>();
    }

    void StaticEventMgr::update()
    {
        startTime = Util::TimeMgr::now();

        auto isEmpty = false;
        while (!isEmpty && Util::TimeMgr::now() - startTime <= maxWaitTime)
        {
            std::lock_guard<std::recursive_mutex> lock(_inst->mtx);
            isEmpty = _inst->waitingQueue.empty();
            if (isEmpty) return;

            _inst->waitingQueue.front().invoke();
            const auto event = _inst->waitingQueue.front().event;
            if (event) event->waiting.pop();
            _inst->waitingQueue.pop_front();
        }
    }

    [[maybe_unused]] void StaticEventMgr::unregisterEvent(const std::size_t& hashCode, uuids::uuid obj)
    {
        std::lock_guard<std::recursive_mutex> lock(_inst->mtx);

        auto& eDic = _inst->staticEvents;
        if (eDic.find(hashCode) == eDic.end()) return;
        auto& lDic = eDic[hashCode];
        if (lDic.find(obj) == lDic.end()) return;
        auto eH = lDic.find(obj);

        while (!eH->second.waiting.empty())
        {
            _inst->waitingQueue.erase(eH->second.waiting.front());
            eH->second.waiting.pop();
        }

        lDic.erase(eH);
    }

    [[maybe_unused]] void StaticEventMgr::unregisterEvent(const std::optional<std::size_t>& hashCodeOpt, uuids::uuid obj)
    {
        std::lock_guard<std::recursive_mutex> lock(_inst->mtx);

        if (!hashCodeOpt) return;
        const auto& hashCode = hashCodeOpt.value();
        auto& eDic = _inst->staticEvents;
        if (eDic.find(hashCode) == eDic.end()) return;
        auto& lDic = eDic[hashCode];
        if (lDic.find(obj) == lDic.end()) return;
        auto eH = lDic.find(obj);

        while (!eH->second.waiting.empty())
        {
            _inst->waitingQueue.erase(eH->second.waiting.front());
            eH->second.waiting.pop();
        }

        lDic.erase(eH);
    }

    void StaticEventMgr::destroy()
    {
        _inst.reset();
    }
} // namespace XD::App