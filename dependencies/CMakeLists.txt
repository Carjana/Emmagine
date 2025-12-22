add_custom_target(CopySDL ALL COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/dependencies/SDL3/SDL3.dll
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/SDL3.dll
)

add_subdirectory(ImGui)