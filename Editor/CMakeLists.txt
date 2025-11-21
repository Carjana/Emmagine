file(GLOB_RECURSE Editor_SRC CONFIGURE_DEPENDS "*.cpp" "*.h")

add_executable(Editor ${Editor_SRC})

target_link_libraries(Editor PRIVATE Engine)