#pragma once

#include "OperatorHID.h"
#include <frc/XboxController.h>

class OI {
    public:
        OI();

        // Return references to either the driver gamepad or operator joystick/button-board.
        // E.g.: passing the joysticks to the drive subsystem.
        frc::XboxController& GetDriverJoystick() { return m_HidDriver; };
        OperatorHID& GetOperatorConsole() { return m_HidOperator; };
        
    private:
        frc::XboxController m_HidDriver   {0};
        OperatorHID m_HidOperator {};
};
