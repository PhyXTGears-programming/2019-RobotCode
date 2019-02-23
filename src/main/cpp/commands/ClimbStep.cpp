#include "commands/ClimbStep.h"
#include "Robot.h"

#include <iostream>
using std::cout;
using std::endl;

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
    Requires(&Robot::m_DriveTrain);
    Requires(&Robot::GetCreeperClimb());
}

void ClimbStep::Initialize() {
    // Prevent drive team from activating ClimbStep before ReadyCreeperArm
    // runs.  Otherwise, the climb arm may swing into the cargo-intake.
    m_HasPrerequisites = Robot::GetCreeperClimb().IsArmAtPosition("arm-ready");
    m_Segment = Segment::Initialize;

    Robot::GetCreeperClimb().SetOutputRange(-0.7, 0.7);
}

void ClimbStep::Execute() {
    if (IsFinished()) {
        // Prevent climb sequence from beginning if IsFinished conditions aren't met.
        return;
    }

    switch (m_Segment) {
        case Segment::Initialize: // Initialization, runs once
            Robot::GetCreeperClimb().RotateArmToPosition("arm-climb");

            Robot::GetCreeperClimb().SetSolenoidDescend(false);
            Robot::GetCreeperClimb().SetSolenoidAscend(true);

            m_Segment = Segment::CheckSwitch;
            break;
        case Segment::CheckSwitch:
            if (Robot::GetCreeperClimb().GetSolenoidSwitch()) {
                Robot::GetCreeperClimb().SetSolenoidDescend(true);
                Robot::GetCreeperClimb().SetSolenoidAscend(true);

                m_Segment = Segment::CheckArm;
            }
            break;
        case Segment::CheckArm:
            if (Robot::GetCreeperClimb().IsArmAtPosition("arm-climb")) {
                m_Segment = Segment::RollCreeper;
            }
        case Segment::RollCreeper:
            Robot::GetCreeperClimb().SetArmWheels(true);
            m_Delay.Start();

            m_Segment = Segment::StopCreeper;
            break;
        case Segment::StopCreeper:
            if (m_Delay.IsDone()) {
                m_Delay.Stop();
                Robot::GetCreeperClimb().SetArmWheels(false);
                m_Segment = Segment::RaiseSolenoids;
            }
            break;
        case Segment::RaiseSolenoids:
            Robot::GetCreeperClimb().RotateArmToPosition("home");
            Robot::GetCreeperClimb().SetSolenoidDescend(true);
            Robot::GetCreeperClimb().SetSolenoidAscend(false);
            m_Segment = Segment::End;
            break;
        case Segment::End:
            break;
        default:
            cout << "Unknown segment in ClimbStep! (segment:" << m_Segment << ")" << endl;
    }

    cout << "segment: " << m_Segment << endl;
    Robot::GetDriveTrain().ArcadeDrive(0, 0);
}

bool ClimbStep::IsFinished() {
    return !m_HasPrerequisites || m_Segment == Segment::End;
}

void ClimbStep::End() {
    // Make sure rollers are stopped.
    Robot::GetCreeperClimb().StopArmWheels();
    Robot::GetCreeperClimb().SetOutputRange(-1, 1);
}

void ClimbStep::Interrupted() {
    // Make sure rollers are stopped.
    Robot::GetCreeperClimb().StopArmWheels();
    Robot::GetCreeperClimb().SetOutputRange(-1, 1);
}
