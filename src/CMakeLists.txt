set(HEADERS
    ${HEADERS}
    MapLoader.h
    SkiGame.h
    Sprite.h
    Player.h
    InputCursor.h
    GameCamera.h
    PlayState.h
)

set(SOURCE
    ${SOURCE}
    MapLoader.cpp
    SkiGame.cpp
    Sprite.cpp
    main.cpp
    Player.cpp
    InputCursor.cpp
    GameCamera.cpp
    PlayState.cpp
)

target_sources(${PROJECT_NAME} 
    PUBLIC ${HEADERS}
    PUBLIC ${SOURCE}
)