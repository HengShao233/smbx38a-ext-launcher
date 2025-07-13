#include "strUtil.h"

#include <string>

namespace ExEngine::Util
{
  const std::string _EMPTY_STR = "";
  const std::string& Empty() { return _EMPTY_STR; }

  std::string W2S(LPCWSTR ws)
  {
    if (!ws) { return Empty(); }

    int bufferSize = WideCharToMultiByte(CP_ACP, 0, ws, -1, NULL, 0, NULL, NULL);
    if (bufferSize <= 0) return Empty();

    std::string multiByteString(bufferSize - 1, '\0');
    WideCharToMultiByte(CP_UTF8, 0, ws, -1, multiByteString.data(), bufferSize, NULL, NULL);
    return multiByteString;
  }
}