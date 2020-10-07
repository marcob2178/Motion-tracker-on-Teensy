#ifndef BODY_H
#define BODY_H

#define BODY_STATE_STRAIGHT
#define BODY_STATE_BEND

#include <accels/Accelerometer.h>

class Chest
{

private:
    Accelerometer *chestSensors;

public:
    Chest(Accelerometer *_chestSensors)
    {
        chestSensors = _chestSensors;
    }

    //===========================================================
    //  crouch logic

    bool isCrouch()
    {
        //return chestSensors->getAltitude() < -0.6;
        return 0;
    }

    double getCrouchPower()
    {
        // double val = chestSensors->getAltitude() + 0.6;
        // return val > 0 ? val : -val;
        return 0;
    }

    //===========================================================
    //  jump logic

    double getJumpingPower()
    {

        return -chestSensors->getLinZ() /*> 0  ? chestSensors->getLinZ() : 0*/;
    }

    double isJumping()
    {
        return getJumpingPower() > 10;
    }
    //===========================================================
    //  bend logic

    int getBendingDirection()
    {
        double y = chestSensors->getPitch();
        double x = chestSensors->getRoll();

        double val = 0;
        if ((x > 0) && (y >= 0))
        {
            val = atan(y / x);
        }

        else if ((x > 0) && (y < 0))
        {
            val = atan(y / x) + 2 * PI;
        }

        else if (x < 0)
        {
            val = atan(y / x) + PI;
        }

        else if ((x == 0) && (y > 0))
        {
            val = PI / 2;
        }

        else if ((x == 0) && (y < 0))
        {
            val = (3 * PI) / 2;
        }

        else if ((x == 0) && (y == 0))
        {
            val = 0;
        }

        return (val * 180.0) / PI;
    }

    double getBendingPower()
    {

        double x = chestSensors->getPitch();
        double y = chestSensors->getRoll();

        return sqrt((x * x) + (y * y));
    }

    bool isBending()
    {
        return getBendingPower() > 5;
    }
};

#endif