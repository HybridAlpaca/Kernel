add_executable (Basic 00-Basic/Main.cpp)
target_include_directories (Basic PUBLIC ${ROOT_DIR}/Include)
target_link_libraries      (Basic PRIVATE Kernel)

add_library (HelloModule SHARED 01-HelloModule/Main.cpp)
set_target_properties      (HelloModule PROPERTIES PREFIX "m" LINKER_LANGUAGE CXX)
target_include_directories (HelloModule PUBLIC ${ROOT_DIR}/Include)

install (TARGETS Basic HelloModule DESTINATION ${ROOT_DIR}/Binaries/Examples)
