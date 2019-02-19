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
}

void ClimbStep::Execute() {
    if (IsFinished()) {
        // Prevent climb sequence from beginning if IsFinished conditions aren't met.
        return;
    }

    switch (m_Segment) {
        case 0:
            Robot::GetCreeperClimb().RotateArmToPosition("arm-climb");

            Robot::GetCreeperClimb().SetSolenoidDescend(false);
            Robot::GetCreeperClimb().SetSolenoidAscend(true);

            // if (Robot::GetCreeperClimb().GetSolenoidSwitch() == true) {
            //     Robot::GetCreeperClimb().SetSolenoidDescend(true);
            //     Robot::GetCreeperClimb().SetSolenoidAscend(false);
            // }
            break;
        case 1:
            /*
            Robot::GetCreeperClimb().SetArmWheels(true); // wheels need to be SLOWED!!
            Robot::GetDriveTrain().Drive(0.2, 0.2); // move robot wheels slow
            */
            break;
        case 2:
            /*
            Robot::GetCreeperClimb().SetSolenoidDescend(false);
            Robot::GetCreeperClimb().SetSolenoidAscend(true);
            */
            break;
        case 3:
            /*
            Robot::GetDriveTrain().Drive(0.2, 0.2); // move robot wheels slow
            */
            break;
        default:
            cout << "Unknown segment in ClimbStep! (segment:" << m_Segment << ")" << endl;
    }

    if (Robot::m_OI.GetOperatorJoystick().GetAButtonPressed()) {
        m_Segment++; // next segment every A button press
    } else if (Robot::m_OI.GetOperatorJoystick().GetBButtonPressed()) {
        m_Segment--; // let operator go back a segment in case of a mis-press
    }
}

bool ClimbStep::IsFinished() {
    return ! m_HasPrerequisites;
}

void ClimbStep::End() {
    // Make sure rollers are stopped.
    Robot::GetCreeperClimb().StopArmWheels();
}

void ClimbStep::Interrupted() {
    // Make sure rollers are stopped.
    Robot::GetCreeperClimb().StopArmWheels();
}
