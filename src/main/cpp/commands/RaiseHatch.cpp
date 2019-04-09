/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "commands/RaiseHatch.h"
#include "Robot.h"

#include <iostream>

RaiseHatch::RaiseHatch() {
    Requires(&Robot::GetHatchMechanism());
    m_IsFinishedCallback = nullptr;
}

// Called just before this Command runs the first time
void RaiseHatch::Initialize() {}

// Called repeatedly when this Command is scheduled to run
void RaiseHatch::Execute() {
    if (nullptr == m_IsFinishedCallback) {
        std::cerr << "RaiseHatch command activated without IsFinished callback." << std::endl;
        return;
    }

    HatchMechanism& hatch = Robot::GetHatchMechanism();

    hatch.RaiseHatch();
    hatch.GrabHatch();
}

// Make this return true when this Command no longer needs to run execute()
bool RaiseHatch::IsFinished() {
    return nullptr == m_IsFinishedCallback || m_IsFinishedCallback();
}

// Called once after isFinished returns true
void RaiseHatch::End() {
    Robot::GetHatchMechanism().StopRotation();
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void RaiseHatch::Interrupted() {
    Robot::GetHatchMechanism().StopRotation();
}

RaiseHatch* RaiseHatch::Until(std::function<bool(void)> callback) {
    m_IsFinishedCallback = callback;
    return this;
}
