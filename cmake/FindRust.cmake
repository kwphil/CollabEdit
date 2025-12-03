find_program(CARGO_EXECUTABLE cargo DOC "Rust Cargo executable")
find_program(RUSTC_EXECUTABLE rustc DOC "Rust compiler executable")

if(CARGO_EXECUTABLE AND RUSTC_EXECUTABLE)
    execute_process(
        COMMAND ${RUSTC_EXECUTABLE} --version
        OUTPUT_VARIABLE RUST_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    
    execute_process(
        COMMAND ${RUSTC_EXECUTABLE} -vV
        OUTPUT_VARIABLE RUSTC_VERBOSE
    )
    string(REGEX MATCH "host: ([^ \n]+)" _ ${RUSTC_VERBOSE})
    set(RUST_DEFAULT_TARGET ${CMAKE_MATCH_1})
    
    set(Rust_FOUND TRUE)
else()
    set(Rust_FOUND FALSE)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Rust
    REQUIRED_VARS CARGO_EXECUTABLE RUSTC_EXECUTABLE
    VERSION_VAR RUST_VERSION
)

function(add_rust_library TARGET CRATE_PATH)
    set(options RELEASE STATIC)
    set(oneValueArgs PROFILE FEATURES TARGET_TRIPLE)
    set(multiValueArgs DEPENDS)
    cmake_parse_arguments(RUST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    if(RUST_RELEASE)
        set(CARGO_PROFILE "--release")
        set(TARGET_SUBDIR "release")
    else()
        set(CARGO_PROFILE "")
        set(TARGET_SUBDIR "debug")
    endif()
    
    if(RUST_STATIC)
        set(LIB_TYPE "staticlib")
    else()
        set(LIB_TYPE "cdylib")
    endif()
    
    if(RUST_TARGET_TRIPLE)
        set(TARGET_FLAG "--target ${RUST_TARGET_TRIPLE}")
        set(TARGET_SUBDIR "${RUST_TARGET_TRIPLE}/${TARGET_SUBDIR}")
    endif()
    
    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        set(LIB_PREFIX "")
        set(LIB_SUFFIX ".lib")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        set(LIB_PREFIX "lib")
        set(LIB_SUFFIX ".a")
    else()
        set(LIB_PREFIX "lib")
        set(LIB_SUFFIX ".a")
    endif()

    set(LIB_FILE "${LIB_PREFIX}${TARGET}${LIB_SUFFIX}")
    set(OUTPUT_PATH "${RUST_TARGET_DIR}/${TARGET_SUBDIR}/${LIB_FILE}")
    
    add_custom_command(
        OUTPUT ${OUTPUT_PATH}
        COMMAND ${CARGO_EXECUTABLE} build ${CARGO_PROFILE} ${TARGET_FLAG}
                --manifest-path ${CRATE_PATH}/Cargo.toml
                --lib
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Building Rust library ${TARGET}"
        DEPENDS ${CRATE_PATH}/Cargo.toml ${CRATE_PATH}/src/*.rs
    )
    
    add_custom_target(${TARGET}_rust ALL DEPENDS ${OUTPUT_PATH})
    
    add_library(${TARGET} STATIC IMPORTED GLOBAL)
    set_target_properties(${TARGET} PROPERTIES
        IMPORTED_LOCATION ${OUTPUT_PATH}
        INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/bindings/cpp
    )
    
    if(RUST_DEPENDS)
        add_dependencies(${TARGET} ${RUST_DEPENDS})
    endif()
    add_dependencies(${TARGET} ${TARGET}_rust)
    
    file(MAKE_DIRECTORY ${RUST_TARGET_DIR})
endfunction()
