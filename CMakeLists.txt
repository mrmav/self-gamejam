cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)

project(self-gamejam)

option(GAME_INSTALL "Installs the game." ON)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCE})

# add our content
add_subdirectory(src)

# build libraries
add_subdirectory(engine)

target_include_directories(${PROJECT_NAME}
    PUBLIC engine/
    PUBLIC src/
)

target_link_directories(${PROJECT_NAME} PRIVATE engine)
add_dependencies(${PROJECT_NAME} simple-cpp-engine)

if(GAME_INSTALL)

    message("Installing game!")

    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/dist)
    
    install(TARGETS ${PROJECT_NAME} CONFIGURATIONS Debug DESTINATION Debug)
    install(TARGETS ${PROJECT_NAME} CONFIGURATIONS Release DESTINATION Release)

    install(DIRECTORY assets DESTINATION Debug)
    install(DIRECTORY assets DESTINATION Release)

endif(GAME_INSTALL)