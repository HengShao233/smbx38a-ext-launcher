#include <queue>
#include <mutex>

#include "smbxNotify.h"
#include "util/logger.h"

#define MAX_NOTIFY_CNT 512
#define MUTEX std::lock_guard<std::mutex> lock(G_MUTEX)

namespace ExEngine::SMBX::Notify {
  static std::mutex G_MUTEX;
  static std::queue<Notify> _msgQueue = std::queue<Notify>();

  Notify::Notify() {}
  Notify::Notify(const std::string& _title, const std::string& _content)
    : title(_title), content(_content) {}

  void Push(bool trimColon, const std::string &title, const std::string &content)
  {
    MUTEX;

    if (_msgQueue.size() >= MAX_NOTIFY_CNT) {
      _msgQueue.pop();
      Logger::Warning("msg queue full");
    }

    if (trimColon) _msgQueue.emplace(Notify(title.substr(2), content));
    else _msgQueue.emplace(Notify(title, content));

    const auto& nty = _msgQueue.front();
    Logger::Info("receive notify: \n    [title]: %s\n    [content]: %s", nty.Title().c_str(), nty.Content().c_str());
  }

  Notify Pop() {
    MUTEX;

    auto nty = _msgQueue.front();
    _msgQueue.pop();

    Logger::Info("handle notify: \n    [title]: %s\n    [content]: %s", nty.Title().c_str(), nty.Content().c_str());
    return nty;
  }
}

