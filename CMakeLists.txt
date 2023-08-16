cmake_minimum_required(VERSION 3.2)

project(event_system)

set(APP_SOURCES 
	src/main.cpp
)


set(TEST_SOURCES 
	test/test.cpp
	test/test_circular.h
	test/test_stack.h
)

set(COMMON_SOURCES 
	include/handler.h
	include/buffer.h
	include/stack.h
	include/circular.h
	include/event.h
	include/statics.h
	include/background.h
	include/timer.h
)

add_subdirectory(
    "${CMAKE_CURRENT_SOURCE_DIR}/googletest" 
    "googletest"
)

add_executable(${PROJECT_NAME} 
	${APP_SOURCES}
	${COMMON_SOURCES}
)

add_executable(unit_test 
	${TEST_SOURCES}
	${COMMON_SOURCES}
)

target_include_directories(
	unit_test PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
)

target_link_libraries(
    unit_test PRIVATE
		gtest gtest_main
)

target_include_directories(
	${PROJECT_NAME} PRIVATE 
        "${PROJECT_SOURCE_DIR}/include"
)

if (MSVC)
    # @see https://github.com/google/googletest/blob/master/googletest/README.md
    # Visual Studio Dynamic vs Static Runtimes
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    target_compile_options(
        ${PROJECT_NAME} PRIVATE
        /W4
    )
    target_compile_options(
        unit_test PRIVATE
        /W4
    )
else ()
    target_compile_options(
        ${PROJECT_NAME} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(
        unit_test PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()
