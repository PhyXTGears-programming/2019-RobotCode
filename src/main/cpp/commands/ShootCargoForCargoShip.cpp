#include "commands/ShootCargoForCargoShip.h"
#include "Robot.h"

/* GOAL:
 *
 * Eject cargo from cargo-intake to shoot and score on cargo ship.
 * 
 *
 * Command start:
 *  - Turn on rollers. Direction to eject cargo. Speed configured via
 *    Shuffleboard.
 *  - Wait ? seconds for rollers to achieve full speed. Time configured via
 *    Shuffleboard.
 *  - Extend cargo kick.  Cargo hits rollers and ejects.
 * 
 * Command finished when:
 *  - Wait ? seconds after extending cargo kick. Time configured via
 *    Shuffleboard.
 * 
 * Command end:
 *  - Stop/disable cargo-intake rollers.
 *  - Retract cargo kick.
 * 
 * Follow-up options:
 *  - None.
 */

ShootCargoForCargoShip::ShootCargoForCargoShip()
    : m_HasPrerequisites(false)
    , m_Action(Action::End)
{
    // This command needs the drivetrain subsystem to be available while
    // running.
    Requires(&Robot::GetCargoIntake());
}

void ShootCargoForCargoShip::Initialize() {
    CargoIntake& intake = Robot::GetCargoIntake();

    m_HasPrerequisites = intake.HasCargo();

    m_Action = Action::TurnOnRollers;
}

void ShootCargoForCargoShip::Execute() {
    if (IsFinished()) {
        // Prevent shot sequence from beginning if IsFinished conditions aren't
        // met.
        return;
    }

    switch (m_Action) {
        case Action::TurnOnRollers:
            Robot::GetCargoIntake().SetRollerSpeed("cargo-ship");
            m_WaitForRollers.Start();
            m_Action = Action::WaitForSpeed;
            break;

        case Action::WaitForSpeed:
            if (m_WaitForRollers.IsDone()) {
                m_WaitForRollers.Stop();

                Robot::GetCargoIntake().ExtendEjector();
                m_WaitForEjector.Start();

                m_Action = Action::WaitForEjector;
            }
            break;

        case Action::WaitForEjector:
            if (m_WaitForEjector.IsDone()) {
                m_WaitForEjector.Stop();

                Robot::GetCargoIntake().RetractEjector();

                m_Action = Action::End;
            }
            break;

        default:
            break;
    }
}

bool ShootCargoForCargoShip::IsFinished() {
    return !m_HasPrerequisites || Action::End == m_Action;
}

void ShootCargoForCargoShip::End() {
    Robot::GetCargoIntake().StopRoller();
}

void ShootCargoForCargoShip::Interrupted() {
    Robot::GetCargoIntake().StopRoller();
}
