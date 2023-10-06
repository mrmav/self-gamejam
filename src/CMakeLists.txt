set(HEADERS
    ${HEADERS}
    MapLoader.h
    SkiGame.h
    Sprite.h
    Player.h
    InputCursor.h
    GameCamera.h
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
)

target_sources(${PROJECT_NAME} 
    PUBLIC ${HEADERS}
    PUBLIC ${SOURCE}
)