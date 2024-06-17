include(MethaneModules)

CPMAddPackage(
    NAME DirectXShaderCompilerBinary
    GITHUB_REPOSITORY MethanePowered/DirectXShaderCompilerBinary
    VERSION 1.7.2308
)

get_platform_arch_dir(PLATFORM_ARCH_DIR CPP_EXT)
if(APPLE)
    set(PLATFORM_ARCH_DIR MacOS)
elseif(ANDROID)
    if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
        set(PLATFORM_ARCH_DIR MacOS)
    elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
        if(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "x86")
            set(PLATFORM_ARCH_DIR Windows-x86)
        else()
            set(PLATFORM_ARCH_DIR Windows-x64)
        endif()
    endif()
endif()

set(DXC_BINARY_DIR "${DirectXShaderCompilerBinary_SOURCE_DIR}/binaries/${PLATFORM_ARCH_DIR}/bin" PARENT_SCOPE)
