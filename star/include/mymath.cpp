#include <cmath>
#include <iostream>
#include "config.hpp"
using namespace std;
#define EPSILON (1e-8)
#define TORAID(X) (X * PI / 180.0f)
void fill_cycle(double z, float* buff) {
    if (z < -1 || z > 1)
        cerr << "error in z, it is " << z << endl;
    float step = 360.0f / CYCLE_SIDE;
    float r = sqrt(R*R - z*z);
    for (int i = 0; i < CYCLE_SIDE; ++i) {
        float degree = i * step;
        float x = r* cos(TORAID(degree));
        float y = r* sin(TORAID(degree));
        buff[3*i] = x;
        buff[3*i+1] = y;
        buff[3*i+2] = z;
    }
    buff[3*CYCLE_SIDE] = r*cos(0);
    buff[3*CYCLE_SIDE+1] = r*sin(0);
    buff[3*CYCLE_SIDE + 2] = z;
}

void fill_meridian(double theta, float* buff) {
    double step = 360.0 / CYCLE_SIDE;
    for (int i = 0; i < NUM_OF_MERIDIAN; ++i) {
        double fi = step * i;
        double theta_prime = theta;
        if (fi <= 180 + EPSILON) {
            theta_prime += PI;
        }
            double x = R * cos(theta_prime) * sin(fi);
            double y = R * sin(theta_prime) * sin(fi);
            double z = R * cos(fi);
            buff[3*i] = x;
            buff[3*i+1] = y;
            buff[3*i+2] = z;
    }
    buff[3*CYCLE_SIDE] = 0;
    buff[3*CYCLE_SIDE+1] = 0;
    buff[3*CYCLE_SIDE+2] = 1;
}