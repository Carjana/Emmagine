file(GLOB_RECURSE Sandbox_SRC CONFIGURE_DEPENDS "*.cpp" "*.h")

add_library(Sandbox SHARED ${Sandbox_SRC})

target_include_directories(Sandbox PUBLIC
        include
)

target_link_libraries(Sandbox PRIVATE Engine)