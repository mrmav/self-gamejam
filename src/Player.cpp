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

        }

        HandleVulnerability();


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
            _Acceleration += dir * 4000.0f;
            
            if(!(_CurrentAnimation->IsPlaying()))
                SetAnimation("ski")->Play();

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

        // Update animations and such:
        Sprite::Update(delta);
    }

    bool Player::IsColliding() const
    {
        std::vector<glm::ivec2> corners;
        corners.push_back(Position / _LoadedMap->GetTileSize());  //worldTilePositionTopLeft    
        corners.push_back(glm::vec2(Position.x + _LoadedMap->GetTileSize().x, Position.y) / _LoadedMap->GetTileSize());  //worldTilePositionTopRight   
        corners.push_back(glm::vec2(Position.x, Position.y + _LoadedMap->GetTileSize().y) / _LoadedMap->GetTileSize());  //worldTilePositionBottomLeft 
        corners.push_back(glm::vec2(Position.x + _LoadedMap->GetTileSize().x, Position.y + _LoadedMap->GetTileSize().y) + _LoadedMap->GetTileSize().y);  //worldTilePositionBottomRight
        
        for(auto& corner : corners)
        {
            int tileIndex = corner.y * static_cast<int>(_LoadedMap->GetWorldTileSize().x) + corner.x;

            if (tileIndex > _LoadedMap->GetTerrainData().size() - 1) continue;

            int tileID = _LoadedMap->GetCollisionData()[tileIndex];
            if( tileID != 0)
            {                
                return true;
            }

        }

        return false;

    }

    void Player::Hurt()
    {
        if(!_Vulnerable) return;

        _Health--;

        _Vulnerable = false;
        _VulnerableTimer = Timer::Get();
        _VulnerableTimeSince = _VulnerableTimer - _FlashRate;
        
    }

    void Player::HandleVulnerability()
    {
        if(_VulnerableTimer + _VulnerableRate < Timer::Get())
        {
            _Vulnerable = true;
            TintColor = glm::vec4(1);
        }

        if(IsColliding())
        {
            Hurt();
        };

        if(!_Vulnerable)
        {   

            if(Timer::Get() > _VulnerableTimeSince + _FlashRate)
            {
                _VulnerableTimeSince = Timer::Get() + _FlashRate;
                TintColor = _Tinted ? glm::vec4(1) : glm::vec4(1, 0.541176, 0.513725, 1);
                _Tinted = !_Tinted;
            }
        
        }
    }


} // namespace GameContent
