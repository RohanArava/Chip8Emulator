# Projects Settings
cmake_minimum_required (VERSION 3.1)
project(chip8)

# The chip8 executable
add_executable(chip8
  src/GLUtils/Application.hpp
  src/GLUtils/Application.cpp
  src/Application/MyApplication.hpp
  src/Application/MyApplication.cpp
  src/Application/Emulator.hpp
  src/Application/Emulator.cpp
  src/GLUtils/glError.hpp
  src/GLUtils/glError.cpp
  src/main.cpp
  src/GLUtils/Shader.hpp
  src/GLUtils/Shader.cpp
)

if (EMSCRIPTEN)
  target_link_libraries(chip8 PUBLIC "-s USE_GLFW=3")
  target_link_libraries(chip8 PUBLIC "-s USE_WEBGL2=1")
  target_link_libraries(chip8 PUBLIC "-s WASM=1")

  # Copy the index.html file.
  foreach(file "index.html" "run_webassembly.py")
    configure_file(src/Web/${file} ${file})
  endforeach(file)

  foreach(file "shader/shader.frag" "shader/shader.vert")
    configure_file(${file} ${file})
  endforeach(file)

  # Allow the directories /shader and /data to be fetched by the web browser.
  set_property(TARGET chip8 APPEND_STRING PROPERTY LINK_FLAGS
    "--preload-file ${CMAKE_CURRENT_BINARY_DIR}/shader/@/shader/")
  
else() # Not Web

  # glfw
  add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
  target_link_libraries(chip8 PRIVATE glfw)

  # glew
  add_definitions(-DGLEW_STATIC)
  add_subdirectory(lib/glew EXCLUDE_FROM_ALL)
  target_link_libraries(chip8 PRIVATE libglew_static)

endif()

# glm
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)
target_link_libraries(chip8 PRIVATE glm)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/src/GLUtils/asset.hpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/src/GLUtils/asset.hpp
)
target_include_directories(chip8
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src
)
