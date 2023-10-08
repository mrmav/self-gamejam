

# Local Multiplayer Ski Game!
This game is part of a self hosted game jam with just two people. It is an excuse to force myself to build something!

This game uses the [little framework](https://github.com/mrmav/simple-cpp-engine) that I build in my spare time, as a learning exercise.

The todo list below has items for both the framework and game mixed in.

## ![Static Badge](https://img.shields.io/badge/tag-mvp1-blue) MVP 1 :beers::partying_face: 
- [x] Build animation system; 
- [x] Build game-centric input system; 
- [x] Add players;
- [x] Camera zooms out to ensure both players are in the game scene?;
- [x] Add acceleration and drag effect (ice like);
- [x] Ice blocks you are fast;
- [x] Snow blocks you are slow; (needs tweaking)
- [x] Win condition first one to cross the finish line;

## MVP 2
- [ ] Build simple arcade physics engine; 
    - Maybe just overlap tests are needed.
- [ ] Each player has an health amount; 
- [ ] Obstacles deduct health; 

## MVP 3
- [ ] Main menu;
- [ ] Multiple prebuilt tracks, selectable in the main menu; (mvp, only one track)
    - Requires building a tiled importer for the engine;
    - Also build custom object import;

## MVP 4
- [ ] Each player has to collect its color flags to gain points?;
- [ ] Able to spawn snow flakes so the other player crashes against them (soft obstacle);
    - For that you need to collect snowballs!

### Nice to have (unordered list):
- [ ] Particles!;
- [ ] Trails!;
- [ ] Single player mode;
- [ ] Jumps!
- [ ] Transporter thingy as an obstacle.


## Learning goals:

- Better understand cmake and how I should be structuring the framework and game;
- Continuous improvement in c++ skills;
- Solidify opengl framework knowledge;
- Solidify gamedev concepts learnt through the years.