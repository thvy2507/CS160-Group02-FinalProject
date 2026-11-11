
cmake_minimum_required(VERSION 3.15)

project(PersonalFinanceManager LANGUAGES CXX)

add_executable(PersonalFinanceManager
    src/main.cpp
    src/core/FinanceManager.cpp
    src/core/Date.cpp
)

target_compile_features(PersonalFinanceManager PRIVATE cxx_std_17)

set_target_properties(PersonalFinanceManager PROPERTIES
    CXX_EXTENSIONS OFF
)

target_include_directories(PersonalFinanceManager PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)

if(MSVC)
    target_compile_options(PersonalFinanceManager PRIVATE /W4)
else()
    target_compile_options(PersonalFinanceManager PRIVATE
        -Wall -Wextra -Wpedantic
    )
endif()
