# cmakeのバージョン
cmake_minimum_required(VERSION 3.14)
project(MochiBeta)

# clangd が .rsp を読み込まないため、.rspを使用しないようにする。
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES OFF)
set(CMAKE_C_USE_RESPONSE_FILE_FOR_INCLUDES OFF)


# find_package は「外部ライブラリを探す」命令。wxWidgets の中でも core base コンポーネントを使用する，という意味
# 成功すると， インクルードパス、リンクすべきライブラリ一覧が自動で変数に入る．
find_package(wxWidgets COMPONENTS core base)

include(${wxWidgets_USE_FILE})

# --- SQLite3 を探す ---
# find_library は ライブラリファイル（.a や .lib）を直接探す命令
# 見つかったパスは SQLITE3_LIB に格納される
# wxWidgets のように CMake 用設定が用意されていないライブラリでは，この方法を使うことが多い
find_library(SQLITE3_LIB sqlite3)

# ソースを列挙
set(SOURCES 
	src/core/db/database.cpp
	src/gui/connect_db/connect_db.cpp
	src/gui/record/record_window.cpp
	src/gui/time_log/edit_category.cpp
	src/gui/time_log/time_log.cpp
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
)

# find_package や find_library で見つけた結果をここで使用する
target_link_libraries(MochiBeta ${wxWidgets_LIBRARIES} ${SQLITE3_LIB})

# MinGW 環境では libgcc, libstdc++ を静的リンクしないと，実行時に DLL が不足することがある
# これにより 単体で動く exe を作りやすくなる
set_target_properties(MochiBeta PROPERTIES LINK_FLAGS "--enable-utf8 -static -static-libgcc -static-libstdc++")
