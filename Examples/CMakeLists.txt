add_library (HelloStatic STATIC 01-HelloStatic/Module.cpp)
set_target_properties      (HelloStatic PROPERTIES PREFIX "m" LINKER_LANGUAGE CXX)
target_include_directories (HelloStatic PUBLIC ${ROOT_DIR}/Include ./Include)

add_executable (Basic 00-Basic/Main.cpp)
target_include_directories (Basic PUBLIC ${ROOT_DIR}/Include ./Include)
target_link_libraries      (Basic PRIVATE Kernel HelloStatic)

install (TARGETS Basic HelloStatic DESTINATION ${ROOT_DIR}/Binaries/Examples)
