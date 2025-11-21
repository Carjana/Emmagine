file(GLOB_RECURSE Engine_SRC CONFIGURE_DEPENDS "*.cpp")

add_library(Engine SHARED ${Engine_SRC})

target_include_directories(Engine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/dependencies/includes
)

target_compile_definitions(Engine PRIVATE EMMA_BUILD_DLL)

target_link_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/dependencies/libs)

target_link_libraries(Engine PUBLIC SDL3)
target_link_libraries(Engine PUBLIC spdlogd)

add_dependencies(Engine CopySDL)