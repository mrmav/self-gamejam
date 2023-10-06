set(HEADERS
    ${HEADERS}
    MapLoader.h
    SkiGame.h
    Sprite.h
)

set(SOURCE
    ${SOURCE}
    MapLoader.cpp
    SkiGame.cpp
    Sprite.cpp
    main.cpp
)

target_sources(${PROJECT_NAME} 
    PUBLIC ${HEADERS}
    PUBLIC ${SOURCE}
)