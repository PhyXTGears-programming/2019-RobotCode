/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <frc/commands/Command.h>

class ShootCargoForLevelOneRocket : public frc::Command {
public:
  ShootCargoForLevelOneRocket();
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  bool IsInterruptible();
  void End() override;
  void Interrupted() override;
};
