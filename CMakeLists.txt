# cmakeのバージョン
cmake_minimum_required(VERSION 3.14)

# --- 1. クロスコンパイル設定 (projectより前に記述) ---
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

# ビルドタイプ（Debug/Release）のデフォルト設定
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

project(MochiBeta)

# --- 2. 探索パスの設定 ---
# Dockerfileで分けた wxWidgets のパスを、ビルドタイプに応じて切り替える
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(WX_ROOT_PATH "/usr/x86_64-w64-mingw32/debug")
    add_definitions(-D_DEBUG)
else()
    set(WX_ROOT_PATH "/usr/x86_64-w64-mingw32/release")
endif()

# 優先的に探す場所を指定
set(CMAKE_FIND_ROOT_PATH ${WX_ROOT_PATH} /usr/x86_64-w64-mingw32)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# clangd が .rsp を読み込まないため、.rspを使用しないようにする。
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES OFF)
set(CMAKE_C_USE_RESPONSE_FILE_FOR_INCLUDES OFF)


# find_package は「外部ライブラリを探す」命令。wxWidgets の中でも core base コンポーネントを使用する，という意味
# 成功すると， インクルードパス、リンクすべきライブラリ一覧が自動で変数に入る．
set(wxWidgets_ROOT_DIR ${WX_ROOT_PATH})
find_package(wxWidgets COMPONENTS core base propgrid aui)

include(${wxWidgets_USE_FILE})

# --- SQLite3 を探す ---
# find_library は ライブラリファイル（.a や .lib）を直接探す命令
# 見つかったパスは SQLITE3_LIB に格納される
# wxWidgets のように CMake 用設定が用意されていないライブラリでは，この方法を使うことが多い
find_library(SQLITE3_LIB sqlite3 REQUIRED)

# SOCI
find_library(SOCI_CORE_LIB soci_core REQUIRED)
find_library(SOCI_SQLITE3_LIB soci_sqlite3 REQUIRED)

# ソースを列挙
set(SOURCES 
	src/core/clock/now_wxstring.cpp
	src/core/db/database.cpp
	src/core/db/db_activity_report.cpp
	src/core/db/db_record.cpp
	src/core/db/db_total.cpp
	src/core/utils/format_time.cpp
	src/gui/connect_db/connect_db.cpp
	src/gui/mainwnd/activity_report/activity_report.cpp
	src/gui/mainwnd/clock/clock.cpp
	src/gui/mainwnd/dashboard/dashboard.cpp
	src/gui/mainwnd/inspector/inspector.cpp
	src/gui/mainwnd/recording/recording.cpp
	src/gui/mainwnd/statistic/statistic.cpp
	src/gui/mainwnd/treectrl/treectrl.cpp
	src/gui/mainwnd/mainwnd.cpp
	src/gui/record/record_window.cpp
	src/gui/time_log/edit_category.cpp
	src/gui/time_log/edit_parent_id.cpp
	src/gui/time_log/time_log.cpp
	src/gui/todo/todo.cpp
	src/main.cpp 
	resource.rc
)

# 実行ファイルを作る
# WIN32 表記で、コンソールウィンドウを表示しない、GUI アプリとして扱われる
add_executable(MochiBeta WIN32 ${SOURCES})

# C++14 を使う
target_compile_features(MochiBeta PUBLIC cxx_std_14)

# インクルードディレクトリの設定
target_include_directories(MochiBeta
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
		/usr/x86_64-w64-mingw32/include # SOCIとSQLiteのヘッダー用
)

# find_package や find_library で見つけた結果をここで使用する
target_link_libraries(MochiBeta 
    ${wxWidgets_LIBRARIES} 
    ${SOCI_SQLITE3_LIB} 
    ${SOCI_CORE_LIB} 
    ${SQLITE3_LIB}
)

# MinGW 環境では libgcc, libstdc++ を静的リンクしないと，実行時に DLL が不足することがある
# これにより 単体で動く exe を作りやすくなる
set_target_properties(MochiBeta PROPERTIES LINK_FLAGS "--enable-utf8 -static -static-libgcc -static-libstdc++")
