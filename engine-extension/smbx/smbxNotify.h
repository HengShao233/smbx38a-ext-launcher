#include <string>

namespace ExEngine::SMBX::Notify
{
  struct Notify
  {
  public:
    Notify();
    Notify(const std::string& title, const std::string& content);

    const inline std::string& Title() const { return title; }
    const inline std::string& Content() const { return content; }

  private:
    const std::string title;
    const std::string content;
  };

  Notify Pop();
  void Push(bool trimColon, const std::string& title, const std::string& content);
}
