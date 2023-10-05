set(HEADERS
    ${HEADERS}
    MapLoader.h
    SkiGame.h    
)

set(SOURCE
    ${SOURCE}
    MapLoader.cpp
    SkiGame.cpp
    main.cpp
)

target_sources(${PROJECT_NAME} 
    PUBLIC ${HEADERS}
    PUBLIC ${SOURCE}
)