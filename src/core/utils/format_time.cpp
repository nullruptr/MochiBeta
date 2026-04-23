#include "format_time.hpp"

namespace TimeUtils {
	wxString FormatSeconds(long long total_seconds) {
		long long h = total_seconds / 3600;
		long long m = (total_seconds % 3600) / 60;
		long long s = total_seconds % 60;
		return wxString::Format("%02lld:%02lld:%02lld", h, m, s);
	}
}
