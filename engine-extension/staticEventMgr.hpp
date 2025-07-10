#pragma once

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <list>
#include <mutex>

#include "uuid.h"

#include "logger.h"

namespace XD::Event
{
    /// @brief �¼����
    /// @example class OnMouseClick : public EventTypeBase<OnMouseClick, uint8_t> {};
    /// @tparam EType �¼��౾��
    /// @tparam ...ArgTypes �¼���ı�������
    template<class EType, typename... ArgTypes>
    class EventTypeBase
    {
    public:
        using _xd_eType = EventTypeBase<EType, ArgTypes...>;
        using _xd_fType = std::function<void(ArgTypes...)>;
        using _xd_isEventType = std::true_type;
    };

    class _xd_staticEvent_BaseFunc { public: virtual ~_xd_staticEvent_BaseFunc() = default; };
    template<class EType>
    class _xd_staticEvent_Func : public _xd_staticEvent_BaseFunc
    {
    public:
        explicit _xd_staticEvent_Func(typename EType::_xd_fType func) : func(func) {}
        typename EType::_xd_fType func;
    };

    class StaticEventMgr
    {
    private:
        struct EventAsyncHandler;
        struct EventHandler
        {
        public:
            EventHandler(uuids::uuid objId, std::unique_ptr<_xd_staticEvent_BaseFunc>&& cb)
                :objId(objId), cb(std::move(cb)), waiting(std::queue<std::list<EventAsyncHandler>::const_iterator>()) {}
            uuids::uuid objId = uuids::uuid();
            std::unique_ptr<_xd_staticEvent_BaseFunc> cb = nullptr;
            std::queue<std::list<EventAsyncHandler>::const_iterator> waiting;
        };

        struct EventAsyncHandler
        {
        public:
            EventAsyncHandler(EventHandler* event, std::function<void()> invoke)
                :event(event), invoke(std::move(invoke)) {}
            EventHandler* event = nullptr;
            std::function<void()> invoke;
        };

        class EventMgrData
        {
        public:
            /// @brief mutex
            std::recursive_mutex                                                           mtx;
            /// @brief ��̬�¼� <�¼�id, <�¼�������Ա, EventHandler>>
            std::unordered_map<std::size_t, std::map<uuids::uuid, EventHandler>> staticEvents;
            /// @brief �첽�¼��ĵȴ�����
            std::list<EventAsyncHandler>                                         waitingQueue;
            
        };
        static std::unique_ptr<EventMgrData> _inst;

    public:
        /// @brief ע���¼� ÿ�� obj ֻ��ע��һ����ͬ�¼� �����ע��ᱻ�Թ�
        /// @tparam EType ע����¼�����
        /// @param obj �������� id (һ���ö����ڴ��ַ����)
        /// @param cb �¼��Ļص�
        /// @return ע�ᵽ�¼��Ĺ�ϣֵ�� (����ʹ�������ϣֵע���¼�)
        template<class EType>
            requires EType::_xd_isEventType::value&& std::is_base_of<typename EType::_xd_eType, EType>::value
        static std::optional<std::size_t> registerEvent(uuids::uuid obj, typename EType::_xd_fType cb)
        {
            std::size_t hashCode = typeid(EType).hash_code();
            auto& eDic = _inst->staticEvents;
            if (eDic.find(hashCode) == eDic.end())
                eDic.insert({ hashCode, std::map<uuids::uuid, EventHandler>() });
            auto& lDic = eDic[hashCode];
            if (lDic.find(obj) != lDic.end()) return std::nullopt;
            lDic.insert(std::pair(obj, EventHandler(
                obj,
                std::unique_ptr<_xd_staticEvent_BaseFunc>
                (reinterpret_cast<_xd_staticEvent_BaseFunc*>(new _xd_staticEvent_Func<EType>(cb)))
            )));
            return std::make_optional<std::size_t>(hashCode);
        }

        /// @brief ע���¼�
        /// @tparam EType ע�����¼�����
        /// @param obj �������� id (һ���ö����ڴ��ַ����)
        template<class EType>
            requires EType::_xd_isEventType::value&& std::is_base_of<typename EType::_xd_eType, EType>::value
        static std::optional<std::size_t> unregisterEvent(uuids::uuid obj)
        {
            std::size_t hashCode = typeid(EType).hash_code();
            auto& eDic = _inst->staticEvents;
            if (eDic.find(hashCode) == eDic.end()) return std::nullopt;
            auto& lDic = eDic[hashCode];
            if (lDic.find(obj) == lDic.end()) return std::nullopt;
            auto eH = lDic.find(obj);

            while (!eH->second.waiting.empty())
            {
                _inst->waitingQueue.erase(eH->second.waiting.front());
                eH->second.waiting.pop();
            }

            lDic.erase(eH);
            return std::make_optional<std::size_t>(hashCode);
        }

        /// @brief ע���¼�
        /// @param hashCode �¼��Ĺ�ϣֵ
        /// @param obj �������� id (һ���ö����ڴ��ַ����)
        static void unregisterEvent(const std::size_t& hashCode, uuids::uuid obj);

        /// @brief ע���¼�
        /// @param hashCodeOpt �¼��Ĺ�ϣֵ��
        /// @param obj �������� id (һ���ö����ڴ��ַ����)
        static void unregisterEvent(const std::optional<std::size_t>& hashCodeOpt, uuids::uuid obj);

        /// @brief ĳ�¼������м���
        /// @tparam EType �¼�����
        template<class EType>
            requires EType::_xd_isEventType::value&& std::is_base_of<typename EType::_xd_eType, EType>::value
        static void clearEvent()
        {
            std::lock_guard<std::recursive_mutex> lock(_inst->mtx);

            std::size_t hashCode = typeid(EType).hash_code();
            auto& eDic = _inst->staticEvents;
            if (eDic.find(hashCode) == eDic.end()) return;
            for (auto& lDic : eDic[hashCode])
            {
                while (!lDic.second.waiting.empty())
                {
                    _inst->waitingQueue.erase(lDic.second.waiting.front());
                    lDic.second.waiting.pop();
                }
            }
            eDic.erase(hashCode);
        }

        /// @brief �㲥
        /// @tparam EType �¼�����
        /// @tparam ...ArgTypes ������ (�����¼�����ʱ��ָ���Ĳ���)
        /// @param ...args Ҫ���ݲ���
        template<class EType, typename... ArgTypes>
            requires EType::_xd_isEventType::value&& std::is_base_of<typename EType::_xd_eType, EType>::value
        && std::is_same<typename EType::_xd_fType, std::function<void(ArgTypes...)>>::value
            static void broadcast(ArgTypes... args)
        {
            std::lock_guard<std::recursive_mutex> lock(_inst->mtx);

            std::size_t hashCode = typeid(EType).hash_code();
            auto& eDic = _inst->staticEvents;
            if (eDic.find(hashCode) == eDic.end()) return;
            for (auto& lDic : eDic[hashCode])
            {
                reinterpret_cast<_xd_staticEvent_Func<EType>*>(lDic.second.cb.get())->
                    func(std::forward<ArgTypes>(args)...);
            }
        }

        /// @brief �첽�ع㲥
        /// @tparam EType �¼�����
        /// @tparam ...ArgTypes ������ (�����¼�����ʱ��ָ���Ĳ���)
        /// @param ...args Ҫ���ݵĲ���
        template<class EType, typename... ArgTypes>
            requires EType::_xd_isEventType::value&& std::is_base_of<typename EType::_xd_eType, EType>::value
        && std::is_same<typename EType::_xd_fType, std::function<void(ArgTypes...)>>::value
            static void broadcastAsync(ArgTypes... args)
        {
            std::lock_guard<std::recursive_mutex> lock(_inst->mtx);

            std::size_t hashCode = typeid(EType).hash_code();
            auto& eDic = _inst->staticEvents;
            if (eDic.find(hashCode) == eDic.end()) return;
            for (auto& lDic : eDic[hashCode])
            {
                auto cbPtr = reinterpret_cast<_xd_staticEvent_Func<EType>*>(lDic.second.cb.get());
                _inst->waitingQueue.emplace_back(
                    EventAsyncHandler(
                        &lDic.second,
                        [cbPtr, args...]() {cbPtr->func(args...); }
                    ));
                lDic.second.waiting.emplace(--(_inst->waitingQueue.end()));
            }
        }

        /// @brief �첽�ع㲥 (���㲥�����ص�)
        /// @tparam EType �¼�����
        /// @tparam ...ArgTypes ������ (�����¼�����ʱ��ָ���Ĳ���)
        /// @param ...args Ҫ���ݵĲ���
        template<class EType, typename... ArgTypes>
        requires EType::_xd_isEventType::value&& std::is_base_of<typename EType::_xd_eType, EType>::value
            && std::is_same<typename EType::_xd_fType, std::function<void(ArgTypes...)>>::value
            static void broadcastAsyncWithCallback(std::function<void(ArgTypes...)> onFinishCall, ArgTypes... args)
        {
            std::lock_guard<std::recursive_mutex> lock(_inst->mtx);

            std::size_t hashCode = typeid(EType).hash_code();
            auto& eDic = _inst->staticEvents;
            if (eDic.find(hashCode) == eDic.end()) return;
            for (auto& lDic : eDic[hashCode])
            {
                auto cbPtr = reinterpret_cast<_xd_staticEvent_Func<EType>*>(lDic.second.cb.get());
                _inst->waitingQueue.emplace_back(
                    EventAsyncHandler(
                        &lDic.second,
                        [cbPtr, args...]() {cbPtr->func(args...); }
                ));
                lDic.second.waiting.emplace(--(_inst->waitingQueue.end()));
            }

            // finish callback
            _inst->waitingQueue.emplace_back(
                EventAsyncHandler(
                    nullptr,
                    [onFinishCall]() {onFinishCall(args...); }
            ));
        }

    public:

        /// @brief ��ʼ��
        static void init();

        /// @brief ˢ��֡
        static void update();

        /// @brief ����
        static void destroy();
    };
} // namespace XD::Event