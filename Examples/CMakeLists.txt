add_library (HelloStatic STATIC 01-HelloStatic/Module.cpp)
set_target_properties      (HelloStatic PROPERTIES PREFIX "m" LINKER_LANGUAGE CXX)
target_include_directories (HelloStatic PUBLIC ${KERNEL_INCLUDE_DIR} ./Include)

add_executable (Basic 00-Basic/Main.cpp)
target_include_directories (Basic PUBLIC ${KERNEL_INCLUDE_DIR} ./Include)
target_link_libraries      (Basic PRIVATE Kernel)

install (TARGETS Basic HelloStatic DESTINATION ${KERNEL_OUTPUT_DIR}/Examples)
