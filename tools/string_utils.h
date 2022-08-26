//
// Created by ChengKeKe on 2022/8/25.
//

#ifndef MEDIACAPTURE_STRING_UTILS_H
#define MEDIACAPTURE_STRING_UTILS_H
#include <string>

#ifdef WIN32
#include <wchar.h>
#include <windows.h>
#endif

namespace tools{
    inline std::string ToUtf8(wchar_t *pwstr, uint32_t len){
        if (len == 0) return std::string();
        int len8 = ::WideCharToMultiByte(CP_UTF8, 0, pwstr, static_cast<int>(len),
                                         nullptr, 0, nullptr, nullptr);

        std::string ns(len8, 0);
        ::WideCharToMultiByte(CP_UTF8, 0, pwstr, static_cast<int>(len), &*ns.begin(),
                              len8, nullptr, nullptr);
        return ns;

    }

   inline std::string ToHex(const int i) {
        char buffer[50];
        snprintf(buffer, sizeof(buffer), "%x", i);

        return std::string(buffer);
    }
}

#endif //MEDIACAPTURE_STRING_UTILS_H
