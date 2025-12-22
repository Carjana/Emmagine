file(GLOB_RECURSE Engine_SRC CONFIGURE_DEPENDS "*.cpp" "*.h")

add_library(Engine SHARED ${Engine_SRC})

target_include_directories(Engine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/dependencies/SDL3/includes
        ${CMAKE_SOURCE_DIR}/dependencies/spdlog/includes
)

if (MSVC)
target_compile_options(Engine PRIVATE /permissive- /W4)
endif ()

target_precompile_headers(Engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/emmapch.h)

target_compile_definitions(Engine PRIVATE EMMA_BUILD_DLL)

target_link_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/dependencies/SDL3)
target_link_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/dependencies/spdlog)

target_link_libraries(Engine PUBLIC SDL3)
target_link_libraries(Engine PUBLIC spdlogd)

add_dependencies(Engine CopySDL)