#pragma once

#include "util/ButtonBoard.h"

#include <cmath>

#include <frc/GenericHID.h>
#include <frc/Joystick.h>

// Buttons are 1-indexed.
// These will need to be redefined after we make the new Button Board.
// Intake Rotation Button Consts (GREEN)
#define bIntakeRotateToPickup 1
#define bIntakeRotateToRocket 2
#define bIntakeRotateToCargo  3
#define bIntakeRotateToStowed 4
// Hatch Grabber Button Consts (BLACK)
#define bHatchGrabberUp   1
#define bHatchGrabberDown 2
// Cargo Action Button Consts (ORANGE)
#define bCargoIntakeCargo 1
#define bCargoShootRocketOne 2
#define bCargoShootCargoShip 3
#define bCargoShootRocketTwo 4
// Camera Swap Button Const (BLACK)
#define bCameraSwap 1
// Creeper Button Consts (GREEN)
#define sCreeperClimb         1
#define bCreeperReadyArms     2
#define bCreeperHomeArms      3
#define bCreeperCrawlForward  4
#define bCreeperCrawlBackward 5

// A combo class that encapsules our button board and flight stick.
// This is only for our robot operator to use.
class OperatorHID {
    public:
        OperatorHID() {}

        bool GetIntakeRotateToPickupPressed() { return m_Board1.GetButtonPressed(bIntakeRotateToPickup); }
        bool GetIntakeRotateToRocketPressed() { return m_Board1.GetButtonPressed(bIntakeRotateToRocket); }
        bool GetIntakeRotateToCargoPressed()  { return m_Board1.GetButtonPressed(bIntakeRotateToCargo); }
        bool GetIntakeRotateToStowedPressed() { return m_Board1.GetButtonPressed(bIntakeRotateToStowed); }

        bool GetHatchGrabberUpPressed()   { return m_Board1.GetButtonPressed(bHatchGrabberUp); }
        bool GetHatchGrabberDownPressed() { return m_Board1.GetButtonPressed(bHatchGrabberDown); }
    
        bool GetCargoIntakeCargoPressed()    { return m_Board1.GetButtonPressed(bCargoIntakeCargo); }
        bool GetCargoShootRocketOnePressed() { return m_Board1.GetButtonPressed(bCargoShootRocketOne); }
        bool GetCargoShootCargoShipPressed() { return m_Board1.GetButtonPressed(bCargoShootCargoShip); }
        bool GetCargoShootRocketTwoPressed() { return m_Board1.GetButtonPressed(bCargoShootRocketTwo); }

        bool GetCameraSwapPressed() { return m_Board1.GetButtonPressed(bCameraSwap); }

        bool GetCreeperClimbThrown()      { return m_Board1.GetButtonPressed(sCreeperClimb); }
        bool GetCreeperReadyArmsPressed() { return m_Board1.GetButtonPressed(bCreeperReadyArms); }
        bool GetCreeperHomeArmsPressed()  { return m_Board1.GetButtonPressed(bCreeperHomeArms); }
        bool IsCreeperCrawlForwardDown()  { return m_Board1.GetButton(bCreeperCrawlForward); }
        bool IsCreeperCrawlBackwardDown() { return m_Board1.GetButton(bCreeperCrawlBackward); }

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

        ButtonBoard& GetActionPad() { return m_ActionPad; }
        ButtonBoard& GetRotationPad() { return m_RotationPad; }
        frc::Joystick& GetFlightStick() { return m_FlightStick; }

    private:
        // We use an `am-3753` Button Board controller.
        // We use 2 button boards for 24 different possible buttons.
        // If you use this system, be sure to test which button board is which in DriverStation before every match.
        // We do not have specific buttons connected to specific boards.
        ButtonBoard m_Board1{1};
        ButtonBoard m_Board2{1};
        
        frc::Joystick m_FlightStick{3}; // To control the intake and creeper arms *manually*.
        // NOTE TO PROGRAMMERS: BUTTONS ON FLIGHTSTICK SHOULD BE RESERVED FOR DEBUG PURPOSES ONLY.
};
