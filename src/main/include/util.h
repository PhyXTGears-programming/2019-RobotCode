/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

namespace util {
    template <class T>
    T clamp(T value, T min, T max) {
        if (min > value) {
            return min;
        } else if (max < value) {
            return max;
        } else {
            return value;
        }
    }
}