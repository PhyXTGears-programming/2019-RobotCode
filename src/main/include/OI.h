#pragma once

#include <frc/XboxController.h>

class OI {
    public:
        OI();

        frc::XboxController& GetDriverJoystick();
        frc::XboxController& GetOperatorJoystick();
        
    private:
        frc::XboxController m_HidDriver   {0};
        frc::XboxController m_HidOperator {1};
};
