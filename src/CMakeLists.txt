add_executable(app main.cpp)

target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

corrosion_import_crate(MANIFEST_PATH ../rust_lib/Cargo.toml)

target_link_libraries(app PUBLIC rust_lib)
