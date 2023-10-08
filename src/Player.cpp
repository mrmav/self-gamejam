#include "Player.h"

namespace GameContent
{
    
    KinematicMaterial kineticIceMaterial = KinematicMaterial(
        // sideForce, slideForce, friction, changeDirectionForce, maxSideMoveSpeed, maxSlideSpeed
        80.0f, 10.0f, 0.9f, 0.3f, 300.0f, 250.0f
    );

    KinematicMaterial kineticSnowMaterial = KinematicMaterial(
        100.0f, 6.0f, 0.4f, 0.9f, 200.0f, 100.0f
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

        _Acceleration.x = 0;
        _Acceleration.y = 0;

        int dir = 0;
        bool accelerate = false;
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

        if(_Controller->IsKeyJustDown(ActionsIndex::ACTION))
        {
            glm::vec2 dir = glm::normalize(_Velocity);
            _Acceleration += dir * 1000.0f;
            
            if(!(_CurrentAnimation->IsPlaying()))
                SetAnimation("ski")->Play();

            _ENGINE_LOG("player", "accelerate! " << glm::to_string(_Acceleration) << ", direction: " << glm::to_string(_Velocity))
        }

        // Apply
        _Kinematics->CalculateContribution(delta, dir, _Acceleration, _Velocity);
        _Velocity += _Acceleration;


        // moving direction
        int movingDirectionSide = _Velocity.x > 0 ? 1 : -1;
        
        // deceleration based on abrupt acceleration changes
        if (dir != 0 && dir != movingDirectionSide)
        {
            //  direction change, should decrease the descending force
            //float brakeForce = ChangingSpeedBreakForce;
            float brakeDamping = 0.2f;
            _Velocity *= std::pow(brakeDamping, delta);
        }

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
