#include "main.h"

XYZ WGS84toCGCS2000(CFG cfg,double x, double y, double z)
{
    //const double params[7] = {-0.9919, -1.6975, 2.9427, 0.00089055, -0.00001853, 0.00001250, 1.0000000675};

    double dx, dy, dz, rx, ry, rz, k;
    XYZ xyz = {0};

    dx = cfg.PARAMATERS[0];
    dy = cfg.PARAMATERS[1];
    dz = cfg.PARAMATERS[2];
    rx = cfg.PARAMATERS[3];
    ry = cfg.PARAMATERS[4];
    rz = cfg.PARAMATERS[5];
    k  = cfg.PARAMATERS[6];

    rx = rx * PI / 180.0;
    ry = ry * PI / 180.0;
    rz = rz * PI / 180.0;

    double cos_rx = cos(rx);
    double sin_rx = sin(rx);
    double cos_ry = cos(ry);
    double sin_ry = sin(ry);
    double cos_rz = cos(rz);
    double sin_rz = sin(rz);

    double r11 = cos_ry * cos_rz;
    double r12 = cos_rx * sin_rz + sin_rx * sin_ry * cos_rz;
    double r13 = sin_rx * sin_rz - cos_rx * sin_ry * cos_rz;
    double r21 = -cos_ry * sin_rz;
    double r22 = cos_rx * cos_rz - sin_rx * sin_ry * sin_rz;
    double r23 = sin_rx * cos_rz + cos_rx * sin_ry * sin_rz;
    double r31 = sin_ry;
    double r32 = -sin_rx * cos_ry;
    double r33 = cos_rx * cos_ry;

    double x_rot = r11 * x + r12 * y + r13 * z;
    double y_rot = r21 * x + r22 * y + r23 * z;
    double z_rot = r31 * x + r32 * y + r33 * z;

    xyz.X = k * x_rot + dx;
    xyz.Y = k * y_rot + dy;
    xyz.Z = k * z_rot + dz;

    return xyz;
}
