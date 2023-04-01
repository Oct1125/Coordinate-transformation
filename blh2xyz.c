#include "main.h"

extern void pos2ecef(const double *pos, double *r)
{
    double sinp = sin(pos[0]), cosp = cos(pos[0]), sinl = sin(pos[1]), cosl = cos(pos[1]);
    double e2 = FE_WGS84 * (2.0 - FE_WGS84), v = RE_WGS84 / sqrt(1.0 - e2 * sinp * sinp);

    r[0] = (v + pos[2]) * cosp * cosl;
    r[1] = (v + pos[2]) * cosp * sinl;
    r[2] = (v * (1.0 - e2) + pos[2]) * sinp;
}

extern XYZ blh2xyz(double B, double L,double H)
{
    double e2, v;
    XYZ xyz = {0};
    double sinB = sin(B), cosB = cos(B), sinL = sin(L), cosL = cos(L);
    e2 = FE_WGS84 * (2.0 - FE_WGS84);
    v = RE_WGS84 / sqrt(1.0 - e2 * sinB * sinB);

    xyz.X = (v + H) * cosB * cosL;
    xyz.Y = (v + H) * cosB * sinL;
    xyz.Z = (v * (1.0 - e2) + H) * sinB;
    return xyz;
}