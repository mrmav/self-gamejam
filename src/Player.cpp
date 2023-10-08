#include "Player.h"

namespace GameContent
{
    
    KinematicMaterial kineticIceMaterial = KinematicMaterial(
        // sideForce, slideForce, friction, maxSideMoveSpeed, maxSlideSpeed
        80.0f, 10.0f, 0.9f, 300.0f, 250.0f
    );

    KinematicMaterial kineticSnowMaterial = KinematicMaterial(
        100.0f, 5.0f, 0.6f, 200.0f, 200.0f
    );

    Player::Player(int x, int y, Ref<AnimationSet> animations, Ref<InputCursor> controller, MapLoader* map)
     : Sprite(x, y, animations), _Controller(controller), _LoadedMap(map)
     {
        _Kinematics = &kineticIceMaterial;
     }

    Player::~Player()
    {

    }

    void Player::Update(float delta)
    {
        _Controller->Update(delta);
        
        // Get the tile we are stepping into:
        int tileWorldPosX = Position.x / _LoadedMap->GetTileSize().x;
        int tileWorldPosY = Position.y / _LoadedMap->GetTileSize().y;
        
        int tileIndex = tileWorldPosY * static_cast<int>(_LoadedMap->GetWorldTileSize().x) + tileWorldPosX;
    
        if (!(tileIndex > _LoadedMap->GetTerrainData().size() - 1))
        {
            int tileID = _LoadedMap->GetTerrainData()[tileIndex];

            if(_LoadedMap->CheckTilesIndex(_LoadedMap->SnowTilesIndex, tileID))
            {
                _Kinematics = &kineticSnowMaterial;
            }
            else
            {
                _Kinematics = &kineticIceMaterial;
            }

        } else
        {
            //_ENGINE_LOG("PLayer", "tile out of bounds " << tileIndex);
        }



        //_ENGINE_LOG("player", "stepping on tile id " << tileID);

        int dir = 0;
        if(_Controller->IsGamePad())
        {   
            if(_Controller->GetAxis() > 0)      dir =  1;
            else if(_Controller->GetAxis() < 0) dir = -1;
        }
        else
        {
            if(_Controller->IsKeyDown(ActionsIndex::LEFT))  dir = -1;
            if(_Controller->IsKeyDown(ActionsIndex::RIGHT)) dir =  1;
        }

        // Apply
        _Kinematics->CalculateContribution(delta, dir, _Acceleration);        
        _Velocity += _Acceleration;
        _Kinematics->CapVelocity(_Velocity);

        // decay over time
        _Velocity *= std::pow(_Kinematics->Friction, delta);

        Move(_Velocity * delta);

        if (Position.x < 0) Position.x = 0;
        if (Position.x + _LoadedMap->GetTileSize().x > _LoadedMap->GetWorldSize().x)
        {
            Position.x = _LoadedMap->GetWorldSize().x - _LoadedMap->GetTileSize().x;
        }

        if(!_Controller->IsGamePad())
        {
            //_ENGINE_LOG("Player", "Acceleration: " << glm::to_string(_Acceleration) << "Velocity: " << glm::to_string(_Velocity));
        }

        // Update animations and such:
        Sprite::Update(delta);
    }

} // namespace GameContent
