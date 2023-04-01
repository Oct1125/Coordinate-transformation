#include "main.h"

// 读取pos
void readdata(CFG cfg, FILE *fp, int row, pPOS pos)
{
    int i, j;
    double x, y, z;
    char arr[20] = {0};
    char buff[MAX] = {0};
    for (i = 0; i < row; i++)
    {
        fgets(buff, MAX, fp);
        if (cfg.I_XYZ == 1)
            sscanf(buff, "%lf%lf%lf%s",
                   &(pos[i].xyz[0]), &(pos[i].xyz[1]), &(pos[i].xyz[2]), pos[i].name);
        else if (cfg.I_BLH == 1)
            sscanf(buff, "%lf%lf%lf%s",
                   &(pos[i].blh[0]), &(pos[i].blh[1]), &(pos[i].blh[2]), pos[i].name);
        else if (cfg.I_ENU == 1)
            sscanf(buff, "%lf%lf%lf%s",
                   &(pos[i].enu[0]), &(pos[i].enu[1]), &(pos[i].enu[2]), pos[i].name);
        else
            printf("####Error#### No Input Files!!!!\n");
    }
}
