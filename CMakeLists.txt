cmake_minimum_required(VERSION 3.31)

find_package(cmake-bare REQUIRED PATHS node_modules/cmake-bare)
find_package(cmake-fetch REQUIRED PATHS node_modules/cmake-fetch)

project(bare_image_resample C)

fetch_package("github:nothings/stb#802cd45" SOURCE_DIR stb)

add_bare_module(bare_image_resample)

target_sources(
  ${bare_image_resample}
  PRIVATE
    binding.c
)

target_include_directories(
  ${bare_image_resample}
  PRIVATE
    ${stb}
)
