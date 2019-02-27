#pragma once

#include "OperatorHID.h"
#include <frc/XboxController.h>

class OI {
    public:
        OI();

        frc::XboxController& GetDriverJoystick();
        OperatorHID& GetOperatorJoystick();
        
    private:
        frc::XboxController m_HidDriver   {0};
        OperatorHID m_HidOperator {};
};
