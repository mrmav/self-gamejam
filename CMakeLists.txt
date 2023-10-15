cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)

project(self-gamejam)

option(GAME_INSTALL "Installs the game." ON)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCE})

target_include_directories(${PROJECT_NAME}    
    PUBLIC engine/external/glm
    PUBLIC engine/src
    PUBLIC src/
)

# add our content
add_subdirectory(src)

# build libraries
add_subdirectory(engine)

target_link_libraries(${PROJECT_NAME} PRIVATE simple-cpp-engine)
add_dependencies(${PROJECT_NAME} simple-cpp-engine)

# This is the full path to the resources
set(RES_DIR ${CMAKE_CURRENT_LIST_DIR}/assets)

# 
# Adds a custom command to the given target, that copies the given file list
#
function(copy_resources DEPENDENCY_TARGET)
    
    set(DESTINATION_DIR $<TARGET_FILE_DIR:${DEPENDENCY_TARGET}>/assets)

    add_custom_command(TARGET ${DEPENDENCY_TARGET}
        COMMAND ${CMAKE_COMMAND} -E rm -Rf ${DESTINATION_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${DESTINATION_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${RES_DIR} ${DESTINATION_DIR}
        COMMENT "Copying resources: ${RES_DIR} to ${DESTINATION_DIR}"
    )

    message("ASSET FOLDER COPY!")

endfunction()

copy_resources(${PROJECT_NAME})

if(GAME_INSTALL)

    message("Installing game!")

    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/dist)
    
    install(TARGETS ${PROJECT_NAME} CONFIGURATIONS Debug DESTINATION Debug)
    install(TARGETS ${PROJECT_NAME} CONFIGURATIONS Release DESTINATION Release)

    install(DIRECTORY assets DESTINATION Debug)
    install(DIRECTORY assets DESTINATION Release)

endif(GAME_INSTALL)

