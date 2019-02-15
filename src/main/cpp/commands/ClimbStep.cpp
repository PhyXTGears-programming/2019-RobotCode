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

void ClimbStep::Initialize() {}

void ClimbStep::Execute() {
    switch (m_Segment) {
        case 0:
            /*
            Robot::GetCreeperClimb().SetArmAngle( someAngle );

            Robot::GetCreeperClimb().SetSolenoidDescend(true);

            if (Robot::GetCreeperClimb().GetSolenoidSwitch() == true) {
                Robot::GetCreeperClimb().SetSolenoidAscend(true);
            }
            */
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

    if (Robot::m_Oi.GetOperatorJoystick().GetAButtonPressed) {
        m_Segment++; // next segment every A button press
    } else if (Robot::m_Oi.GetOperatorJoystick().GetBButtonPressed) {
        m_Segment--; // let operator go back a segment in case of a mis-press
    }
}

bool ClimbStep::IsFinished() { return false; }

void ClimbStep::End() {}

void ClimbStep::Interrupted() {}
