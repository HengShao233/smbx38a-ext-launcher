#pragma once

#include <string>
#include <Windows.h>

namespace ExEngine::Util
{
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const std::string& Empty();

	/// <summary>
	/// LPCWSTR to std::string in cpp style
	/// </summary>
	/// <param name="wideString"> src </param>
	/// <returns></returns>
	std::string W2S(LPCWSTR wideString);
}
