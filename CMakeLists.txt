cmake_minimum_required(VERSION 3.28)
project(Networking VERSION 0.1.0 LANGUAGES CXX)

# Include the src directory first to ensure SimpleClientLib is built
add_subdirectory(src)

# Define the Networking target first
add_executable(Networking 
    main.cpp  # Replace with your actual source files for Networking
)

# Link libraries to the Networking target
target_link_libraries(Networking PRIVATE SimpleClientLib)

# Add include directories
get_target_property(CLIENT_INCLUDES SimpleClientLib INTERFACE_INCLUDE_DIRECTORIES)
if(CLIENT_INCLUDES)
    target_include_directories(Networking PRIVATE ${CLIENT_INCLUDES})
endif()

# Set C++ standard
set_property(TARGET Networking PROPERTY CXX_STANDARD 17)
