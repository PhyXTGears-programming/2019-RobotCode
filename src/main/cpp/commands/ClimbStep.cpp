#include "commands/ClimbStep.h"
#include "Robot.h"

#include "util/StopWatch.h"

#include <iostream>

static StopWatch logTimer;

/* GOAL:
 *
 * Climb the top level platform on the field
 *
 *
 * Command start:
 *  - set both creeper arm set point to desired angle and enable the descend solenoid
 *  - when the solenoid reed switch is triggered, enable the ascend solenoid to balance the solenoid
 *  - wait for a button press, then move robot forward slowly until another input for a stop
 *  - retract the solenoid and creeper arm and wait for button press
 *  - drive slowly forward until last button press
 *
 * Command finished when:
 *  - the last drive-slowly segment is stopped by a button press.
 *
 * Command end:
 *  - fully reset creeper arm and solenoids
 *
 * Follow-up options:
 *  - None.
 */

ClimbStep::ClimbStep() {
    Requires(&Robot::GetCreeperClimb());
}

void ClimbStep::Initialize() {
    // Prevent drive team from activating ClimbStep before ReadyCreeperArm
    // runs.  Otherwise, the climb arm may swing into the cargo-intake.
    m_HasPrerequisites = Robot::GetCreeperClimb().IsArmAtPosition("arm-ready");
    m_Segment = Segment::Initialize;

    logTimer.Reset();

    // Slow climb arm to match piston lift speed.
    Robot::GetCreeperClimb().SetRotatePIDOutputRange(-0.6, 0.6);
}

void ClimbStep::Execute() {
    if (IsFinished()) {
        // Prevent climb sequence from beginning if IsFinished conditions aren't met.
        std::cout << logTimer.Split() << "ClimbStep.Execute: Pre-empted by IsFinished" << std::endl;
        return;
    }

    static Segment lastSegment = Segment::Initialize;

    if (lastSegment != m_Segment) {
        switch (m_Segment) {
            case Segment::Initialize:
                std::cout << logTimer.Split() << "ClimbStep.Execute: Segment: Initialize" << std::endl;
                break;

            case Segment::CheckSwitch:
                std::cout << logTimer.Split() << "ClimbStep.Execute: Segment: Check Piston Limit" << std::endl;
                break;
            
            case Segment::CheckArm:
                std::cout << logTimer.Split() << "ClimbStep.Execute: Segment: Check Arm" << std::endl;
                break;

            case Segment::RollCreeper:
                std::cout << logTimer.Split() << "ClimbStep.Execute: Segment: Roll Creeper" << std::endl;
                break;
                
            case Segment::StopCreeper:
                std::cout << logTimer.Split() << "ClimbStep.Execute: Segment: Stop Creeper" << std::endl;
                break;

            case Segment::RaiseSolenoids:
                std::cout << logTimer.Split() << "ClimbStep.Execute: Segment: Raise Piston" << std::endl;
                break;
            
        }
        lastSegment = m_Segment;
    }

    switch (m_Segment) {
        case Segment::Initialize: // Initialization, runs once
            Robot::GetCreeperClimb().ResetPistonLimitLatch();
            Robot::GetCreeperClimb().RotateArmToPosition("arm-climb");
            Robot::GetCreeperClimb().PistonExtend();

            std::cout
                << logTimer.Split()
                << "ClimbStep::Execute: Expect PistonLimitSwitch to be false.  Limit switch is "
                << Robot::GetCreeperClimb().IsPistonAtLimit()
                << std::endl;

            m_Segment = Segment::CheckSwitch;
            break;
        case Segment::CheckSwitch:
            if (Robot::GetCreeperClimb().IsPistonAtLimit()) {
                Robot::GetCreeperClimb().PistonHold();

                m_Segment = Segment::CheckArm;
            }
            break;
        case Segment::CheckArm:
            if (Robot::GetCreeperClimb().IsArmAtPosition("arm-climb")) {
                m_Segment = Segment::RollCreeper;
            }
            break;
        case Segment::RollCreeper:
            Robot::GetCreeperClimb().SetArmWheels(true);
            m_Delay.Start();

            m_Segment = Segment::StopCreeper;
            break;
        case Segment::StopCreeper:
            if (m_Delay.IsDone()) {
                m_Delay.Stop();
                Robot::GetCreeperClimb().SetArmWheels(false);
                Robot::GetCreeperClimb().SetRotatePIDOutputRange(-1, 1);
                m_Segment = Segment::RaiseSolenoids;
            }
            break;
        case Segment::RaiseSolenoids:
            Robot::GetCreeperClimb().RotateArmToPosition("home");
            Robot::GetCreeperClimb().PistonRetract();
            m_Segment = Segment::End;
            break;
        case Segment::End:
            break;

        default:
            std::cout
                << logTimer.Split()
                << "ClimbStep.Execute: Unknown segment in ClimbStep! (segment:"
                << m_Segment
                << ")"
                << std::endl;
    }
}

bool ClimbStep::IsFinished() {
    return !m_HasPrerequisites || m_Segment == Segment::End;
}

void ClimbStep::End() {
    // Make sure rollers are stopped.
    Robot::GetCreeperClimb().StopArmWheels();
    Robot::GetCreeperClimb().SetRotatePIDOutputRange(-1, 1);
}

void ClimbStep::Interrupted() {
    // Make sure rollers are stopped.
    Robot::GetCreeperClimb().StopArmWheels();
    Robot::GetCreeperClimb().SetRotatePIDOutputRange(-1, 1);
}
