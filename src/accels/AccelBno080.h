#ifndef ACCEL_BNO_080
#define ACCEL_BNO_080

#include <accels/Accelerometer.h>
#include <accels/SparkFun_BNO080_Arduino_Library.h>

class AccelBNO080 : public Accelerometer
{

private:
    BNO080 *myIMU;
    int pin = 255;

    float offset_roll, offset_pitch;

    void setOffsets(double _offset_y, double _offset_z)
    {
        offset_roll = _offset_y;
        offset_pitch = _offset_z;
    }

public:
    AccelBNO080()
    {
        myIMU = new BNO080();
    }

    void calibrate()
    {

        update();
        setOffsets(((myIMU->getRoll()) * 180.0 / PI), ((myIMU->getPitch()) * 180.0 / PI));
    }

    void setIntPin(int _pin)
    {

        pin = _pin;
    }

    void begin()
    {
        if (!myIMU->begin(BNO080_DEFAULT_ADDRESS, Wire, pin))
        {
            Serial.println("BNO080 not detected at default I2C address. Check your jumpers and the hookup guide. Trying again...");
            delay(1000);

            if (!myIMU->begin(BNO080_DEFAULT_ADDRESS, Wire, pin))
            {
                Serial.println("Something wrong, freezing....");
                return;
            }
        }
        myIMU->enableRotationVector(9);
        myIMU->enableLinearAccelerometer(3); //Send data update every 50ms
        
        calibrate();
    }

    double getLinX()
    {
        return myIMU->getLinAccelX();
    }
    double getLinY()
    {
        return myIMU->getLinAccelY();
    }
    double getLinZ()
    {
        return myIMU->getLinAccelZ();
    }
    double getYaw()
    {
        return ((myIMU->getYaw()) * 180.0 / PI);
    }
    double getPitch()
    {
        double pitch = ((myIMU->getPitch()) * 180.0 / PI) - offset_pitch;

        if (pitch < -180)
            return pitch + 360;
        else
            return pitch;
        //return myIMU->getPitch();

        
    }
    double getRoll()
    {
        double roll = ((myIMU->getRoll()) * 180.0 / PI) - offset_roll;

        if (roll < -180)
            return roll + 360;
        else
            return roll;
        // return myIMU->getRoll();
    }



    void update()
    {
        // Serial.print(String(myIMU->dataAvailable()) + "\t");
        // Serial.println(String(myIMU->receivePacket()) + "\t");

        // if (!myIMU->dataAvailable()){

        //     Serial.println("dead");
        //     //begin();
        // }

        // Serial.println(myIMU->dataAvailable());
        // myIMU->receivePacket();
    }
};

#endif