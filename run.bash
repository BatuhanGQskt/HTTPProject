#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

first_arg="$1"

# executable error 42
# folder error 43
# cmake configuration error 44
# cmake build error 45
error_array=(42 43 44 45)

# Function to build the project
build_project() {
  echo -e "${YELLOW}Setting up build directory...${NC}"
  mkdir -p build
  cd build || { echo -e "${RED}Failed to enter build directory!${NC}"; return 43; }

  echo -e "${YELLOW}Configuring CMake...${NC}"
  cmake .. || { echo -e "${RED}CMake configuration failed!${NC}"; return 44; }

  echo -e "${YELLOW}Building project...${NC}"
  cmake --build . --config Release || { echo -e "${RED}Build failed!${NC}"; return 45; }
  
  # Return to original directory
  cd ..
  return 0;  # Return 0 for success, not 1
}

# Function to run the executable
run_executable() {
  echo -e "${YELLOW}Running executable...${NC}"
  cd build || { echo -e "${RED}Failed to enter build directory!${NC}"; return 43; }
  
  # Navigate to Release directory if on Windows with MSVC
  if [ -d "Release" ]; then
    cd Release
  fi

  # Run the executable
  echo -e "${GREEN}Starting Networking application...${NC}"
  ./Networking.exe || { echo -e "${RED}Execution failed! Current folder is $PWD ${NC}"; return 42;}
  
  # Return to original directory
  cd ../..
  return 0;  # Return 0 for success, not 1
}

ret=NULL
# Process the command line argument
if [[ "$first_arg" == "run" ]]; then
    echo -e "${YELLOW}Running...${NC}"
    run_executable
    ret=$?
elif [[ "$first_arg" == "build" ]]; then
    echo -e "${YELLOW}Building...${NC}"
    build_project
    ret=$?
elif [[ "$first_arg" == "combined" ]]; then
    echo -e "${YELLOW}Building and running...${NC}"
    build_project
    ret=$?
    echo -e "${YELLOW}Build complete!${NC}"

    # Check if build was successful before running
    if [[ $ret -ne 0 ]]; then
        echo -e "${RED}An error occurred in building! Error code: $ret ${NC}"
        return ;
    fi
    
    # If build was successful, run the executable
    echo -e "${YELLOW}Build successful, now running executable...${NC}"
    run_executable
    ret=$?
else
  echo -e "${RED}Invalid argument: $first_arg${NC}"
  echo -e "${YELLOW}Valid arguments are: 'run', 'build', or 'build and run'${NC}"
  exit 1
fi

for str in ${error_array[@]}; do
    if [[ $ret == $str ]]; then
        echo -e "${RED}An error occurred! Error code: $ret ${NC}"
    fi
done

echo -e "${GREEN}Done!${NC}"
