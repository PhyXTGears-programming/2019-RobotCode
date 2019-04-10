/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "commands/LowerHatch.h"
#include "Robot.h"

#include <iostream>

LowerHatch::LowerHatch() {
    Requires(&Robot::GetHatchMechanism());
    m_IsFinishedCallback = nullptr;
}

// Called just before this Command runs the first time
void LowerHatch::Initialize() {}

// Called repeatedly when this Command is scheduled to run
void LowerHatch::Execute() {
    if (nullptr == m_IsFinishedCallback) {
        std::cerr << "LowerHatch command activated without IsFinished callback." << std::endl;
        return;
    }

    HatchMechanism& hatch = Robot::GetHatchMechanism();

    hatch.LowerHatch();
    hatch.ReleaseHatch();
}

// Make this return true when this Command no longer needs to run execute()
bool LowerHatch::IsFinished() {
    return nullptr == m_IsFinishedCallback || m_IsFinishedCallback();
}

// Called once after isFinished returns true
void LowerHatch::End() {
    Robot::GetHatchMechanism().StopRotation();
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void LowerHatch::Interrupted() {
    Robot::GetHatchMechanism().StopRotation();
}

LowerHatch* LowerHatch::Until(std::function<bool(void)> callback) {
    m_IsFinishedCallback = callback;
    return this;
}
