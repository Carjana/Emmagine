file(GLOB_RECURSE Editor_SRC CONFIGURE_DEPENDS "*.cpp" "*.h")

add_executable(Editor ${Editor_SRC})

include_directories(Editor PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/dependencies/SDL3/includes
)

target_link_libraries(Editor PRIVATE Engine)
