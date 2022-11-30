get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(PROJECT_NAME "My_${PROJECT_NAME}")
file(GLOB SOURCE_FILES *.cpp)
file(GLOB HEADER_FILES *.h)
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE framework)
target_precompile_headers(${PROJECT_NAME} REUSE_FROM framework)