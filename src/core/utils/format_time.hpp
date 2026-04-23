#pragma once
#include <wx/string.h>

namespace TimeUtils {
    // 秒を "HH:MM:SS" 形式に変換する
    wxString FormatSeconds(long long total_seconds);
}
