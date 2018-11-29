cmake_minimum_required(VERSION 2.6)
project(RAYTRACER)

include_directories(
	src
	contrib
    contrib/msvc-windows-x86/glew/include
    contrib/msvc-windows-x86/SDL2/include
    contrib/msvc-windows-x86/SDL2_image/include
    contrib/msvc-windows-x86/SDL2_ttf/include
)

  link_directories(
    ${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/SDL2/lib
    ${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/SDL2_image/lib
    ${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/glew/lib
    ${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/SDL2_ttf/lib
  )
  
  add_definitions(-DSDL_MAIN_HANDLED)

add_executable(raytracer
	src/main.cpp
	src/ray.h
	src/camera.h
	src/camera.cpp
	src/object.h
	src/object.cpp
	src/sphere.h
	src/sphere.cpp
	src/tracer.h
	src/tracer.cpp
)

target_link_libraries(raytracer
	
    SDL2 SDL2_image glew32 SDL2_ttf
)

 add_custom_command(TARGET raytracer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/SDL2/bin/SDL2.dll"
      "$<TARGET_FILE_DIR:raytracer>"
  )

   add_custom_command(TARGET raytracer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/SDL2_image/bin/SDL2_image.dll"
      "$<TARGET_FILE_DIR:raytracer>"
  )
  
   add_custom_command(TARGET raytracer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/SDL2_ttf/bin/SDL2_ttf.dll"
      "$<TARGET_FILE_DIR:raytracer>"
  )

   add_custom_command(TARGET raytracer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/SDL2_ttf/bin/libfreetype-6.dll"
      "$<TARGET_FILE_DIR:raytracer>"
  )

     add_custom_command(TARGET raytracer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/SDL2_image/bin/libpng16-16.dll"
      "$<TARGET_FILE_DIR:raytracer>"
  )

  add_custom_command(TARGET raytracer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/glew/bin/glew32.dll"
      "$<TARGET_FILE_DIR:raytracer>"
  )