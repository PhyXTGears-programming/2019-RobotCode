#pragma once

#include "util/ButtonBoard.h"

#include <cmath>

#include <frc/GenericHID.h>
#include <frc/Joystick.h>

// Buttons are 1-indexed.
// These will need to be redefined after we make the new Button Board.
// Intake Rotation Button Consts (GREEN)  board 1
#define bIntakeRotateToCargo  4
#define bIntakeRotateToPickup 2
#define bIntakeRotateToRocket 3
// Hatch Grabber Button Consts (BLACK)  board 2
#define bHatchGrabberUp   6
#define bHatchGrabberDown 7
// Cargo Action Button Consts (ORANGE)  board 2
#define bCargoIntakeCargo 3
#define bCargoShootRocketOne 1
#define bCargoShootCargoShip 4
#define bCargoShootRocketTwo 2
// Camera Swap Button Const (BLACK)  board 2
#define bCameraSwap 5
// Creeper Button Consts (GREEN)  board 1
#define sCreeperClimb         6
#define bCreeperReadyArms     10
#define bCreeperHomeArms      9
#define bCreeperCrawlForward  7
#define bCreeperCrawlBackward 8

// A combo class that encapsules our button board and flight stick.
// This is only for our robot operator to use.
class OperatorHID {
    public:
        OperatorHID() {}

        bool GetIntakeRotateToPickupPressed()     { return m_Board1.GetButtonPressed(bIntakeRotateToPickup); }
        bool GetIntakeRotateToRocketPressed()     { return m_Board1.GetButtonPressed(bIntakeRotateToRocket); }
        bool GetIntakeRotateToCargoShipPressed()  { return m_Board1.GetButtonPressed(bIntakeRotateToCargo); }

        bool GetHatchGrabPressed()     { return m_Board2.GetButtonPressed(bHatchGrabberUp); }
        bool GetHatchGrabReleased()    { return m_Board2.GetButtonReleased(bHatchGrabberUp); }
        bool GetHatchReleasePressed()  { return m_Board2.GetButtonPressed(bHatchGrabberDown); }
        bool GetHatchReleaseReleased() { return m_Board2.GetButtonReleased(bHatchGrabberDown); }
    
        bool GetCargoIntakeCargoPressed()    { return m_Board2.GetButtonPressed(bCargoIntakeCargo); }
        bool GetCargoShootRocketOnePressed() { return m_Board2.GetButtonPressed(bCargoShootRocketOne); }
        bool GetCargoShootCargoShipPressed() { return m_Board2.GetButtonPressed(bCargoShootCargoShip); }
        bool GetCargoShootRocketTwoPressed() { return m_Board2.GetButtonPressed(bCargoShootRocketTwo); }

        bool GetCameraSwapPressed() { return m_Board2.GetButtonPressed(bCameraSwap); }

        bool GetCreeperClimbEnabled()     { return m_Board1.GetButtonPressed(sCreeperClimb); }
        bool GetCreeperReadyArmPressed()  { return m_Board1.GetButtonPressed(bCreeperReadyArms); }
        bool GetCreeperHomeArmPressed()   { return m_Board1.GetButtonPressed(bCreeperHomeArms); }

        bool IsCreeperCrawlForwardDown()      { return m_Board1.GetButton(bCreeperCrawlForward); }
        bool GetCreeperCrawlForwardReleased() { return m_Board1.GetButtonReleased(bCreeperCrawlForward); }

        bool GetCreeperRetractPistonPressed() { return false; }

        double GetThrottle() { return m_FlightStick.GetThrottle(); }; // Throttle is the switch on the base of the stick
        double GetJoystickY() {
            double posY = -m_FlightStick.GetY();
            if (std::abs(posY) > 0.1) {
                return posY;
            } else {
                return 0;
            }
        };

        bool GetFlightStickPressed(int button) { return m_FlightStick.GetRawButtonPressed(button); }

        ButtonBoard& GetBoard1() { return m_Board1; }
        ButtonBoard& GetBoard2() { return m_Board2; }
        frc::Joystick& GetFlightStick() { return m_FlightStick; }

    private:
        // We use an `am-3753` Button Board controller.
        // We use 2 button boards for 24 different possible buttons.
        // If you use this system, be sure to test which button board is which in DriverStation before every match.
        // We do not have specific buttons connected to specific boards.
        ButtonBoard m_Board1{1};
        ButtonBoard m_Board2{2};
        
        frc::Joystick m_FlightStick{3}; // To control the intake and creeper arms *manually*.
        // NOTE TO PROGRAMMERS: BUTTONS ON FLIGHTSTICK SHOULD BE RESERVED FOR DEBUG PURPOSES ONLY.
};
