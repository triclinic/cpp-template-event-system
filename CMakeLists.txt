cmake_minimum_required(VERSION 3.2)

project(event_system)

set(APP_SOURCES src/main.cpp)
set(TEST_SOURCES test/test.cpp)
set(COMMON_SOURCES 
	include/handler.h
	include/buffer.h
	include/stack.h
	include/circular.h
	include/event.h
	include/statics.h
)

add_executable(${PROJECT_NAME} ${APP_SOURCES} ${COMMON_SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)


add_executable(unit_test ${TEST_SOURCES} ${COMMON_SOURCES})

target_include_directories(unit_test
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)