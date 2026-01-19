file(GLOB_RECURSE Runtime_SRC CONFIGURE_DEPENDS "*.cpp" "*.h")

add_executable(Runtime ${Runtime_SRC})

include_directories(Runtime PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src

)

target_link_libraries(Runtime PRIVATE Engine)
