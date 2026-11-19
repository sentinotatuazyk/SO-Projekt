
find_package(Threads REQUIRED)

add_executable(dyrektor main.cpp dyrektor.cpp)
add_executable(kasa kasa.cpp)
add_executable(klient klient.cpp)

target_include_directories(dyrektor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(kasa PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(klient PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

corrosion_import_crate(MANIFEST_PATH ../rust_lib/Cargo.toml)

target_link_libraries(dyrektor PRIVATE rust_lib)
target_link_libraries(kasa PRIVATE Threads::Threads)
