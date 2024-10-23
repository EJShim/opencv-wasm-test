cmake_minimum_required(VERSION 3.13)
project(util)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



# TODO : Add OpenCV
include(FetchContent)

FetchContent_Declare(
        opencv
        GIT_REPOSITORY https://github.com/opencv/opencv.git
        GIT_TAG 4.6.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
set(opencv_configure_options)
list(APPEND opencv_configure_options
    "SHELL:-DCPU_DISPATCH=''" 
    "SHELL:-DCV_TRACE=OFF" 
    "SHELL:-DBUILD_SHARED_LIBS=OFF" 
    "SHELL:-DWITH_1394=OFF" 
    "SHELL:-DWITH_ADE=OFF" 
    "SHELL:-DWITH_VTK=OFF" 
    "SHELL:-DWITH_EIGEN=OFF" 
    "SHELL:-DWITH_FFMPEG=OFF" 
    "SHELL:-DWITH_GSTREAMER=OFF" 
    "SHELL:-DWITH_GTK=OFF" 
    "SHELL:-DWITH_GTK_2_X=OFF" 
    "SHELL:-DWITH_IPP=OFF" 
    "SHELL:-DWITH_JASPER=OFF" 
    "SHELL:-DWITH_JPEG=OFF" 
    "SHELL:-DWITH_WEBP=OFF" 
    "SHELL:-DWITH_OPENEXR=OFF" 
    "SHELL:-DWITH_OPENGL=OFF" 
    "SHELL:-DWITH_OPENVX=OFF" 
    "SHELL:-DWITH_OPENNI=OFF" 
    "SHELL:-DWITH_OPENNI2=OFF" 
    "SHELL:-DWITH_PNG=OFF" 
    "SHELL:-DWITH_TBB=OFF" 
    "SHELL:-DWITH_TIFF=OFF" 
    "SHELL:-DWITH_V4L=OFF" 
    "SHELL:-DWITH_OPENCL=OFF" 
    "SHELL:-DWITH_OPENCL_SVM=OFF" 
    "SHELL:-DWITH_OPENCLAMDFFT=OFF" 
    "SHELL:-DWITH_OPENCLAMDBLAS=OFF" 
    "SHELL:-DWITH_GPHOTO2=OFF" 
    "SHELL:-DWITH_LAPACK=OFF" 
    "SHELL:-DWITH_ITT=OFF" 
    "SHELL:-DWITH_QUIRC=OFF" 
    "SHELL:-DBUILD_ZLIB=ON" 
    "SHELL:-DBUILD_opencv_apps=OFF" 
    "SHELL:-DBUILD_opencv_calib3d=ON" 
    "SHELL:-DBUILD_opencv_dnn=ON" 
    "SHELL:-DBUILD_opencv_features2d=ON" 
    "SHELL:-DBUILD_opencv_flann=ON",  # No bindings provided. This module is used as a dependency for other modules 
    "SHELL:-DBUILD_opencv_gapi=OFF" 
    "SHELL:-DBUILD_opencv_ml=OFF" 
    "SHELL:-DBUILD_opencv_photo=ON" 
    "SHELL:-DBUILD_opencv_imgcodecs=OFF" 
    "SHELL:-DBUILD_opencv_shape=OFF" 
    "SHELL:-DBUILD_opencv_videoio=OFF" 
    "SHELL:-DBUILD_opencv_videostab=OFF" 
    "SHELL:-DBUILD_opencv_highgui=OFF" 
    "SHELL:-DBUILD_opencv_superres=OFF" 
    "SHELL:-DBUILD_opencv_stitching=OFF" 
    "SHELL:-DBUILD_opencv_java=OFF" 
    "SHELL:-DBUILD_opencv_js=ON" 
    "SHELL:-DBUILD_opencv_python2=OFF" 
    "SHELL:-DBUILD_opencv_python3=OFF" 
    "SHELL:-DBUILD_EXAMPLES=ON" 
    "SHELL:-DBUILD_PACKAGE=OFF" 
    "SHELL:-DBUILD_TESTS=ON" 
    "SHELL:-DBUILD_PERF_TESTS=ON"
)

FetchContent_GetProperties(opencv)
FetchContent_Populate(opencv
    ${opencv_configure_options}
)
FetchContent_MakeAvailable(opencv)

# find_package(OpenCV REQUIRED)


# -----------------------------------------------------------------------------
# Compile example code
# -----------------------------------------------------------------------------

add_executable(${CMAKE_PROJECT_NAME} cvmanager.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
    opencv_core opencv_highgui
)

target_include_directories(${CMAKE_PROJECT_NAME}
    PRIVATE
    ${OPENCV_CONFIG_FILE_INCLUDE_DIR}
    ${OPENCV_MODULE_opencv_core_LOCATION}/include
    ${OPENCV_MODULE_opencv_highgui_LOCATION}/include
)

# set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED YES)


# -----------------------------------------------------------------------------
# WebAssembly build options
# -----------------------------------------------------------------------------
set(emscripten_link_options)
set(emscripten_compile_options)

list(APPEND emscripten_link_options
    "--bind"
    "SHELL:-s MODULARIZE=1"  
    "SHELL:-s WASM=1"
    "SHELL:-s EXPORT_ES6=1"
    "SHELL:-s EXPORT_NAME=managerModule"
    "SHELL:-s EXPORTED_RUNTIME_METHODS=['FS']"
)

set(emscripten_debug_options)
set(DEBUGINFO "PROFILE" CACHE STRING "Type of debug info")
set_property(CACHE DEBUGINFO PROPERTY
    STRINGS
        NONE              # -g0
        READABLE_JS       # -g1
        PROFILE           # -g2
        DEBUG_NATIVE      # -g3
)

if(DEBUGINFO STREQUAL "NONE")
    list(APPEND emscripten_debug_options
        "-g0"
    )
    elseif(DEBUGINFO STREQUAL "READABLE_JS")
    list(APPEND emscripten_debug_options
        "-g1"
    )
    list(APPEND emscripten_link_options
        "SHELL:-s DEMANGLE_SUPPORT=1"
    )
    elseif(DEBUGINFO STREQUAL "PROFILE")
    list(APPEND emscripten_debug_options
        "-g2"
    )
    list(APPEND emscripten_link_options
        "SHELL:-s DEMANGLE_SUPPORT=1"
    )
    elseif(DEBUGINFO STREQUAL "DEBUG_NATIVE")
    list(APPEND emscripten_debug_options
        "-g3"
    )
    list(APPEND emscripten_link_options
        "SHELL:-s ASSERTIONS=1"
        "SHELL:-s DEMANGLE_SUPPORT=1"
    )
endif()

# -----------------------------------------------------------------------------
# Build options
# -----------------------------------------------------------------------------
set(emscripten_optimizations)
set(OPTIMIZE "SMALLEST_WITH_CLOSURE" CACHE STRING "Emscripten optimization")
set_property(CACHE OPTIMIZE PROPERTY
    STRINGS
        NO_OPTIMIZATION       # -O0
        LITTLE                # -O1
        MORE                  # -O2
        BEST                  # -O3
        SMALL                 # -Os
        SMALLEST              # -Oz
        SMALLEST_WITH_CLOSURE # -Oz --closure 1
)

if(OPTIMIZE STREQUAL "NO_OPTIMIZATION")
    list(APPEND emscripten_optimizations
        "-O0"
    )
    elseif(OPTIMIZE STREQUAL "LITTLE")
    list(APPEND emscripten_optimizations
        "-O1"
    )
    elseif(OPTIMIZE STREQUAL "MORE")
    list(APPEND emscripten_optimizations
        "-O2"
    )
    elseif(OPTIMIZE STREQUAL "BEST")
    list(APPEND emscripten_optimizations
        "-O3"
    )
    elseif(OPTIMIZE STREQUAL "SMALL")
    list(APPEND emscripten_optimizations
        "-Os"
    )
    elseif(OPTIMIZE STREQUAL "SMALLEST")
    list(APPEND emscripten_optimizations
        "-Oz"
    )
    elseif(OPTIMIZE STREQUAL "SMALLEST_WITH_CLOSURE")
    list(APPEND emscripten_optimizations
        "-Oz"
    )
    list(APPEND emscripten_link_options
        "--closure 1"
    )
endif()

target_compile_options(${CMAKE_PROJECT_NAME}
    PUBLIC
        ${emscripten_compile_options}
        ${emscripten_optimizations}
        ${emscripten_debug_options}
)

target_link_options(${CMAKE_PROJECT_NAME}
    PUBLIC
        ${emscripten_link_options}
        ${emscripten_optimizations}
        ${emscripten_debug_options}
)
