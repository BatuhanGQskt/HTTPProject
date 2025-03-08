# Battleship Online on CLI

## Project Description
This project will be handle GUI using SDL, and networking using Boost asio to create two player game against each other to try and find opponents ships with turn based fight.

### Tools
I have used tools to develop this project as follows:
* CMake to make it cross-platform to run the code everywhere.
* bash to execute the code via terminal without needing to know how to use CMake for users.
* Cpp in actual development via SDL and Boost/asio to develop the project.

## RUN ON LOCAL
Before running the code, make sure you have your environment set up correctly.
Download Boost to your computer and set the environment variables from CMakeLists.txt in the project folder if required with some collision on Boost folders.
Change the expilict paths for Boost to your own Boost folder location. If you don't have anaconda downloaded in your environment,
use find_package to find Boost instead of setting up environment variables.

```bash
# If it is your first time run, use combined version to build and run
# Using bash
source run.sh combined

# To build the code if it is your first time, or want to update the codebase
source run.sh build

# To run the code if you build it
source run.sh run
```

## RUN VIA DOCKER-IMAGE
Why to use docker image?, even though running on local is simpler. It is because every required library or setup is done automatically by the docker container and you just need to run the game after running the container. Otherwise, you should have been downloaded every required library to your system, bind them to the path, and try to run it with several errors.


```bash
# First we need to create the image using Dockerfile
docker build --tag battleship .

# interactive run of the container
docker run -it battleship bash

# If there is build folder in the app folder, remove it
rm -r build

# Run the bash as in local
# If it is your first time run, use combined version to build and run
# Using bash
source run.sh combined

# To build the code if it is your first time, or want to update the codebase
source run.sh build

# To run the code if you build it
source run.sh run
```