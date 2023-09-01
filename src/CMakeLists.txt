add_executable(main main.cpp vector.h utils.h)

# 以下でDXライブラリをinclude, linkします
# 注意：この例ではDXライブラリの「３Ｄモデル機能＆ソフトウエアレンダリング機能無しパッケージ」を使用していますが、
# 通常の「ＤＸライブラリ Windows版」を使用する場合にはおそらく以下の部分を修正する必要があります
target_include_directories(main PRIVATE ${DXLIB_DIR})
target_link_directories(main PRIVATE ${DXLIB_DIR})
add_dependencies(main DxLibDownload)