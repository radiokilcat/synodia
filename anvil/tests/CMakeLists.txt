project (anviltest)
add_subdirectory(../third_party/googletest gtest)

enable_testing()
include_directories(${GTEST_INCLUDE_DIRS})

set(public_headers
    gtest/gtest.h
)


add_executable(
  anviltests
  serializer_test.cpp
)

target_include_directories(anviltests PUBLIC gtest)

target_link_libraries(
  anviltests
  PUBLIC
  GTest::gtest_main
  anvil
)

