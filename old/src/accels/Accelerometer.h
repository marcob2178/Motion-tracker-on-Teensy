#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H
class Accelerometer
{
private:
    virtual void setOffsets(double _offset_y, double _offset_z);

public:
    virtual void calibrate();

    virtual void begin();

    virtual double getLinX();
    virtual double getLinY();
    virtual double getLinZ();

    virtual double getYaw();
    virtual double getPitch();
    virtual double getRoll();

    virtual void update();
};
#endif