# これにより CMake は Windows 用の設定、Windows 用ライブラリ探索を行うようになる
set(CMAKE_SYSTEM_NAME Windows)

# コンパイラの指定
# Linux 標準の gcc ではなく， MinGW のクロスコンパイラを使うよう明示している
# windres は .rc を処理するために必要
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

# MinGW 用のライブラリが入っている場所を指定
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)


# --- 探し方のルールを制限する ---
# プログラム(コンパイラ等)はホスト(Linux)側も探すが、
# ライブラリとインクルードパスは ROOT_PATH (MinGW側) のみを探す設定
# プログラム（gcc など） Linux 側を使う → NEVER
# ライブラリとヘッダ MinGW 側のみを見る → ONLY
# これをしないと， Linux 用ライブラリを誤ってリンクするという事故が起きる．
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)