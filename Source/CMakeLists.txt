add_library (Kernel STATIC Kernel/SharedLib.cpp)

target_include_directories (Kernel PUBLIC ${ROOT_DIR}/Include)
target_link_libraries      (Kernel PRIVATE dl)
