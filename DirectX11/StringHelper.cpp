#include "StringHelper.h"
#include <Windows.h>
using namespace std;

std::string StringHelper::ConvertACPToUTF8(const std::string& UTF8String)
{
    int wideSize = MultiByteToWideChar(CP_ACP, 0, UTF8String.c_str(), -1, NULL, 0);
    std::wstring wideStr(wideSize, 0);
    MultiByteToWideChar(CP_ACP, 0, UTF8String.c_str(), -1, &wideStr[0], wideSize);

    int utf8Size = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, NULL, 0, NULL, NULL);
    std::string utf8Str(utf8Size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, &utf8Str[0], utf8Size, NULL, NULL);

    return utf8Str;
}
