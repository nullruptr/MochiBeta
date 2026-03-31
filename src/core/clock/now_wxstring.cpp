#include <wx/wx.h>
#include "now_wxstring.hpp"

wxString Now_wxStr::now(ClockType  type){  
	int yy, mm, dd, day, wd, hour, min, sec;

	wxDateTime dt;
	// 引数にsys が入れば、システム時刻を、それ以外はUTCを返す。
	if (type == ClockType::System){
		dt = wxDateTime::Now();
	}
	else {
		dt = wxDateTime::Now().ToUTC();
	}

	yy = dt.GetYear();
	mm = dt.GetMonth() + 1; // 月は+1表示する必要がある。cf. 0は1月のこと
	day = dt.GetDay();
	wd = dt.GetWeekDay();
	hour = dt.GetHour();
	min = dt.GetMinute();
	sec = dt.GetSecond();

	// return するときは、この形式で返す必要アリ。文字列で返す。printfのような使い方
	return wxString::Format(
		"%04d-%02d-%02d %02d:%02d:%02d",
		yy, mm, day, hour, min, sec
			);
}
