# add_custom_target(CopySDL ALL COMMAND ${CMAKE_COMMAND} -E copy_if_different
#         ${CMAKE_SOURCE_DIR}/dependencies/SDL3/SDL3.dll
#         ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/SDL3.dll
# )

add_subdirectory(ImGui)
add_subdirectory(GlMathematics)

set(SDL3_SHARED OFF CACHE BOOL "" FORCE)
set(SDL3_STATIC ON CACHE BOOL "" FORCE)
set(SDL3_TEST OFF CACHE BOOL "" FORCE)
add_subdirectory(SDL EXCLUDE_FROM_ALL)
set(spirv_cross_c_shared_DIR "${CMAKE_SOURCE_DIR}/dependencies/SPIRV-Cross-Local/share/spirv_cross_c_shared/cmake")


if(NOT TARGET SDL3::SDL3)
    add_library(SDL3::SDL3 ALIAS SDL3-static)
endif()

add_subdirectory(SDL_shadercross)

add_custom_target(CopySPIRV ALL COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/dependencies/SPIRV-Cross-Local/bin/spirv-cross-c-shared.dll
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/spirv-cross-c-shared.dll
)

add_custom_target(CopyDxCompiler ALL COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/dependencies/dxcompiler.dll
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/dxcompiler.dll
)

add_custom_target(CopyDxil ALL COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/dependencies/dxil.dll
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/dxil.dll
)