/**
 * @file CBase4618.h
 * @brief Declaration of the CBase4618 class.
 *
 * This file contains the declaration of the CBase4618 class, which serves as a base class
 * for projects in this course. It encapsulates common functionalities such as drawing on an
 * OpenCV Mat object and controlling hardware through the CControl class as well as an update function.
 */

#pragma once

#include "CControler.h" // Make sure you have this file for the CControl class
#include <opencv2/core.hpp> // For OpenCV Mat

 /**
  * @class CBase4618
  * @brief Base class encapsulating common functionalities for drawing and hardware control.
  *
  * CBase4618 provides a common interface and functionalities for projects, including
  * drawing on an OpenCV Mat object and interfacing with hardware via the CControl class.
  * It defines pure virtual methods for updating and drawing that must be implemented by derived classes.
  */
class CBase4618 {
public:
    CBase4618(); ///< Constructor.
    ~CBase4618(); ///< Destructor.

    CControl _control; ///< Object for hardware control.
    cv::Mat _canvas; ///< OpenCV Mat object for drawing.

    virtual void update() = 0; ///< Pure virtual method for updating the object's state.
    virtual void draw() = 0; ///< Pure virtual method for drawing on the _canvas.
    void run(); ///< Main method to run the update-draw loop.
};

