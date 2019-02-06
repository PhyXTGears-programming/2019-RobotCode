/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

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
