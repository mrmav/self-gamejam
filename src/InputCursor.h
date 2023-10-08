#pragma once

#include <Engine.h>

using namespace Engine;

namespace GameContent
{

    enum class ActionsIndex
    {
        LEFT = 0,
        RIGHT,
        ACTION,
        COUNT
    };


    /// @brief This class represents a set of keys to act as a cursor
    /// It takes two keys for left or right, and another for action.    
    class InputCursor
    {

        private:

        std::vector<uint16_t> _KeyCodes = std::vector<uint16_t>(static_cast<std::size_t>(ActionsIndex::COUNT), 0);
        std::vector<bool> _KeyStates = std::vector<bool>(static_cast<std::size_t>(ActionsIndex::COUNT), false);
        std::vector<bool> _KeyJustDownStates = std::vector<bool>(static_cast<std::size_t>(ActionsIndex::COUNT), false);

        int _GamePadID = -1;
        bool _GamePadUseAxis = false;
        float _GamePadAxis = 0;
        GamePadAxisCode _AxisCode = 0;

        public:

        InputCursor(KeyCode left, KeyCode right, KeyCode action);

        /// @brief This constructor would be for gamepad controller.
        /// Implementation should be postponed.
        InputCursor(int gamePadID, bool useAxis, GamePadAxisCode axis, GamePadButtonCode action);
        InputCursor(int gamePadID, GamePadButtonCode left, GamePadButtonCode right, GamePadButtonCode action);

        ~InputCursor();

        void Update(float delta);

        bool IsKeyDown(ActionsIndex code) const;
        float GetAxis() const;
        bool IsGamePad() const {return _GamePadID > -1; };
        bool IsKeyJustDown(ActionsIndex code) const;

    };

} // namespace GameContent
