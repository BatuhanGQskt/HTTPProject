# Simple Client project
project(SimpleClient)

# Find Boost headers only, no components needed for header-only use
find_package(Boost 1.82.0 REQUIRED)

if (Boost_FOUND)
    # Print Boost paths for debugging
    message(STATUS "Boost found:")
    message(STATUS "  Include dirs: ${Boost_INCLUDE_DIRS}")
    message(STATUS "  Version: ${Boost_VERSION}")
    
    # Define the source files
    set(SimpleClientSources
        net_message.hpp
        net_common.hpp
        olc_net.hpp
    )
    
    # Create a library target first
    add_library(SimpleClientLib INTERFACE)
    
    # Set the include directories for the library
    target_include_directories(SimpleClientLib INTERFACE 
        ${CMAKE_CURRENT_SOURCE_DIR}  # Include current directory
        ${Boost_INCLUDE_DIRS}
    )
    
    message(STATUS "  PASSED target includes")

    # Apply the same compile definitions to the library
    target_compile_definitions(SimpleClientLib INTERFACE
        BOOST_ASIO_HEADER_ONLY
        BOOST_ASIO_STANDALONE
        BOOST_SYSTEM_NO_DEPRECATED
        BOOST_ERROR_CODE_HEADER_ONLY
        BOOST_SYSTEM_NO_LIB
        BOOST_DATE_TIME_NO_LIB
        BOOST_REGEX_NO_LIB
    )
    
    # Create the executable
    add_executable(SimpleClient
        SimpleClient.cpp
    )

    # Link the executable with the library
    target_link_libraries(SimpleClient PRIVATE SimpleClientLib)
    
    # Optional: Set C++ standard if needed
    target_compile_features(SimpleClientLib INTERFACE cxx_std_17)
    target_compile_features(SimpleClient PRIVATE cxx_std_17)
endif()