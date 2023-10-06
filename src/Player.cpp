#include "Player.h"

namespace GameContent
{
    
    Player::Player(int x, int y, Ref<AnimationSet> animations, Ref<InputCursor> controller)
     : Sprite(x, y, animations), _Controller(controller)
     {
        
     }

    Player::~Player()
    {

    }

    void Player::Update(float delta)
    {
        _Controller->Update(delta);

        glm::vec2 move(0.0f, 0.0f);

        if(_Controller->IsGamePad())
        {   
            int dir = 0;
            if(_Controller->GetAxis() > 0)      dir =  1;
            else if(_Controller->GetAxis() < 0) dir = -1;
            
            move.x += _SideSpeed * dir;
        }
        else
        {
            // process keyboard mode 

            if(_Controller->IsKeyDown(ActionsIndex::LEFT))
            {
                move.x -= _SideSpeed;
            }
            if(_Controller->IsKeyDown(ActionsIndex::RIGHT))
            {
                move.x += _SideSpeed;
            }

        }


        // constant descent
        move.y += _SlideSpeed;

        //std::cout << glm::to_string(Position) << "will move by: " << glm::to_string(move * delta) << std::endl;

        // Apply
        Move(move * delta);

        //std::cout << glm::to_string(Position) << "Moved by: " << glm::to_string(move * delta) << std::endl;

        // Update animations and such:
        Sprite::Update(delta);
    }

} // namespace GameContent
