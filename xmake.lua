set_project("toy_log")

-- set xmake minimum version
set_xmakever("2.5.1")

target("toy_log")
    set_kind("binary")
    add_files("test.cpp")