set(HEADERS
    ${HEADERS}
    MapLoader.h
    SkiGame.h
    Sprite.h
    Player.h
    InputCursor.h
)

set(SOURCE
    ${SOURCE}
    MapLoader.cpp
    SkiGame.cpp
    Sprite.cpp
    main.cpp
    Player.cpp
    InputCursor.cpp
)

target_sources(${PROJECT_NAME} 
    PUBLIC ${HEADERS}
    PUBLIC ${SOURCE}
)