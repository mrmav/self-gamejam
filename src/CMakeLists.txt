set(HEADERS
    ${HEADERS}
    SkiGame.h
)

set(SOURCE
    ${SOURCE}
    SkiGame.cpp
    main.cpp
)

target_sources(${PROJECT_NAME} 
    PUBLIC ${HEADERS}
    PUBLIC ${SOURCE}
)