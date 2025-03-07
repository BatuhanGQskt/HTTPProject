cmake_minimum_required(VERSION 3.28)

project(Networking)

# Clear any existing Boost variables
unset(Boost_FOUND CACHE)
unset(Boost_INCLUDE_DIRS CACHE)
unset(Boost_LIBRARIES CACHE)
unset(Boost_LIBRARY_DIRS CACHE)

# Set explicit paths for Boost headers
set(BOOST_ROOT "C:\\Boost\\boost_1_82_0")
set(BOOST_INCLUDEDIR "${BOOST_ROOT}")

# Print paths for debugging
message(STATUS "BOOST_ROOT: ${BOOST_ROOT}")

# Find Boost headers only, no components needed for header-only use
find_package(Boost 1.82.0 REQUIRED)

if (Boost_FOUND)
    # Print Boost paths for debugging
    message(STATUS "Boost found:")
    message(STATUS "  Include dirs: ${Boost_INCLUDE_DIRS}")
    message(STATUS "  Version: ${Boost_VERSION}")
    
    add_executable(${PROJECT_NAME} main.cpp)
    
    # Explicitly specify include directories for the target
    target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
    
    # Define necessary preprocessor definitions for header-only mode
    target_compile_definitions(${PROJECT_NAME} PRIVATE 
        BOOST_ASIO_HEADER_ONLY
        BOOST_ASIO_STANDALONE
        BOOST_SYSTEM_NO_DEPRECATED
        BOOST_ERROR_CODE_HEADER_ONLY
        BOOST_SYSTEM_NO_LIB
        BOOST_DATE_TIME_NO_LIB
        BOOST_REGEX_NO_LIB
    )
    
    target_link_libraries(${PROJECT_NAME} PRIVATE print_lib)
    add_subdirectory(src)
    
    # Ensure C++11 or higher is used
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)
endif()
