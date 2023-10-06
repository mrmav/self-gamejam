#include "InputCursor.h"

namespace GameContent
{
    
    InputCursor::InputCursor(KeyCode left, KeyCode right, KeyCode action)
    {
        _KeyCodes[static_cast<std::size_t>(ActionsIndex::LEFT)] = left;
        _KeyCodes[static_cast<std::size_t>(ActionsIndex::RIGHT)] = right;
        _KeyCodes[static_cast<std::size_t>(ActionsIndex::ACTION)] = action;
    }

    InputCursor::InputCursor(int gamePadID, bool useAxis, GamePadAxisCode axis, GamePadButtonCode action)
    {   
        _ENGINE_FAIL_WITH_MSG(useAxis, "This constructor is only meant to use with axis.")
        _GamePadUseAxis = true;
        _GamePadID = gamePadID;

        _AxisCode = axis;

        _KeyCodes[static_cast<std::size_t>(ActionsIndex::ACTION)] = action;
    }

    InputCursor::InputCursor(int gamePadID, GamePadButtonCode left, GamePadButtonCode right, GamePadButtonCode action)
    {
        _GamePadID = gamePadID;

        _KeyCodes[static_cast<std::size_t>(ActionsIndex::LEFT)] = left;
        _KeyCodes[static_cast<std::size_t>(ActionsIndex::RIGHT)] = right;
        _KeyCodes[static_cast<std::size_t>(ActionsIndex::ACTION)] = action;
    }

    InputCursor::~InputCursor()
    {

    }

    void InputCursor::Update(float delta)
    {
        for(int i = 0; i < _KeyCodes.size(); i++)
        {
            if (_GamePadID >= 0)
            {
                _KeyStates[i] = Input::IsButtonPressed(_GamePadID, _KeyCodes[i]);
                _GamePadAxis = Input::GetAxisStrength(_GamePadID, _AxisCode);
            } else
            {
                _KeyStates[i] = Input::IsKeyPressed(_KeyCodes[i]);
            }
        }
    }

    bool InputCursor::IsKeyDown(ActionsIndex code) const
    {
        return _KeyStates[static_cast<std::size_t>(code)];
    }

    float InputCursor::GetAxis() const
    {
        if(!_GamePadUseAxis) return 0;

        return _GamePadAxis;
    }

} // namespace GameContent
