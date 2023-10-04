set(HEADERS
    ${HEADERS}     
)

set(SOURCE
    ${SOURCE}
    main.cpp
)

target_sources(${PROJECT_NAME} 
    PUBLIC ${HEADERS}
    PUBLIC ${SOURCE}
)