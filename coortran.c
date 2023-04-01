#include "main.h"

void xyz2all(POS *pos, double ref[], int row)
{
    int i;
    BLH blh;
    ENU enu;
    for (i = 0; i < row; i++)
    {
        blh = xyz2blh(pos[i].xyz[0], pos[i].xyz[1], pos[i].xyz[2]);
        pos[i].blh[0] = blh.B;
        pos[i].blh[1] = blh.L;
        pos[i].blh[2] = blh.H;
        enu = xyz2enu(ref[0], ref[1], ref[2], pos[i].xyz[0], pos[i].xyz[1], pos[i].xyz[2]);
        pos[i].enu[0] = enu.E;
        pos[i].enu[1] = enu.N;
        pos[i].enu[2] = enu.U;
    }
}

void blh2all(POS *pos, double ref[], int row)
{
    int i;
    XYZ xyz;
    ENU enu;
    for (i = 0; i < row; i++)
    {
        xyz = blh2xyz(pos[i].blh[0], pos[i].blh[1], pos[i].blh[2]);
        pos[i].xyz[0] = xyz.X;
        pos[i].xyz[1] = xyz.Y;
        pos[i].xyz[2] = xyz.Z;
        enu = xyz2enu(ref[0], ref[1], ref[2], pos[i].xyz[0], pos[i].xyz[1], pos[i].xyz[2]);
        pos[i].enu[0] = enu.E;
        pos[i].enu[1] = enu.N;
        pos[i].enu[2] = enu.U;
    }
}

void enu2all(POS *pos, double ref[], int row)
{
    int i;
    XYZ xyz;
    BLH blh;
    for (i = 0; i < row; i++)
    {
        xyz = enu2xyz(ref[0], ref[1], ref[2], pos[i].enu[0], pos[i].enu[1], pos[i].enu[2]);
        pos[i].xyz[0] = xyz.X + ref[0];
        pos[i].xyz[1] = xyz.Y + ref[1];
        pos[i].xyz[2] = xyz.Z + ref[2];
        blh = xyz2blh(pos[i].xyz[0], pos[i].xyz[1], pos[i].xyz[2]);
        pos[i].blh[0] = blh.B;
        pos[i].blh[1] = blh.L;
        pos[i].blh[2] = blh.H;
    }
}

void output(pPOS pos2, CFG cfg, int row, int k)
{
    int i;
    char tmp[MAX], tmp2[MAX], tmp3[MAX] = {0};
    FILE *fp = NULL;
    setstr(tmp, cfg.save_address, k);
    setstr(tmp2, cfg.save_address, k);
    setstr(tmp3, cfg.save_address, k);
    if (cfg.O_XYZ == 1)
    {
        if (cfg.O_SYS == 1)
            setstr(tmp + k, "WGS84_XYZ", 9);
        else
            setstr(tmp + k, "CGCS2000_XYZ", 12);
        fp = fopen(tmp, "w");
        for (i = 0; i < row; i++)
        {
            fprintf(fp, "%20.6lf%20.6lf%20.6lf%10s\n", pos2[i].xyz[0], pos2[i].xyz[1], pos2[i].xyz[2], pos2[i].name);
        }
        fclose(fp);
    }
    if (cfg.O_BLH == 1)
    {
        if (cfg.O_SYS == 1)
            setstr(tmp2 + k, "WGS84_BLH", 9);
        else
            setstr(tmp2 + k, "CGCS2000_BLH", 12);
        fp = fopen(tmp2, "w");
        for (i = 0; i < row; i++)
        {
            fprintf(fp, "%20.6lf%20.6lf%20.6lf%10s\n", pos2[i].blh[0] * 180 / PI, pos2[i].blh[1] * 180 / PI, pos2[i].blh[2], pos2[i].name);
        }
        fclose(fp);
    }
    if (cfg.O_ENU == 1)
    {
        if (cfg.O_SYS == 1)
            setstr(tmp3 + k, "WGS84_ENU", 9);
        else
            setstr(tmp3 + k, "CGCS2000_ENU", 12);
        fp = fopen(tmp3, "w");
        for (i = 0; i < row; i++)
        {
            fprintf(fp, "%20.6lf%20.6lf%20.6lf%10s\n", pos2[i].enu[0], pos2[i].enu[1], pos2[i].enu[2], pos2[i].name);
        }
        fclose(fp);
    }
}

void coortran()
{
    int i, j, k, l, row;
    double ref[3];
    FILE *fp;
    pPOS pos, pos2;
    BLH blh;
    ENU enu;
    XYZ xyz;

    /*----------------------------Reading CFG file----------------------------*/
    char cfgname[] = "ReadData.cfg";
    CFG cfg = readCFG(cfgname);
    l = (int)strlen(cfg.data_address);
    k = (int)strlen(cfg.save_address);
    strcpy(cfg.dop_address, cfg.data_address);
    setstr(cfg.data_address + l, cfg.FILE_NAME, strlen(cfg.FILE_NAME));
    ref[0] = cfg.REFENCE_X;
    ref[1] = cfg.REFENCE_Y;
    ref[2] = cfg.REFENCE_Z;

    /*------------------------------Reading Pos-------------------------------*/
    fp = fopen(cfg.data_address, "r");
    row = getrow(fp);
    rewind(fp);
    pos = (pPOS)malloc(sizeof(POS) * row);
    pos2 = (pPOS)malloc(sizeof(POS) * row);
    readdata(cfg, fp, row, pos);

    /*------------------------------------------------------------------------*/

    if (cfg.I_SYS == 1)
    {
        if (cfg.I_XYZ == 1)
        {
            xyz2all(pos, ref, row);
        }
        else if (cfg.I_BLH == 1)
        {
            blh2all(pos, ref, row);
        }
        else if (cfg.I_ENU == 1)
        {
            enu2all(pos, ref, row);
        }
        else
            printf("####Error#### No Input Files!!!!\n");
        switch (cfg.O_SYS)
        {
        case 0:
            printf("####Error#### No System Imformation!!!\n");
            break;
        case 1:
            memcpy(pos2, pos, sizeof(POS) * row);
            break;
        case 2:
            for (i = 0; i < row; i++)
            {
                xyz = WGS84toCGCS2000(cfg, pos[i].xyz[0], pos[i].xyz[1], pos[i].xyz[2]);
                pos2[i].xyz[0] = xyz.X + pos[i].xyz[0];
                pos2[i].xyz[1] = xyz.Y + pos[i].xyz[1];
                pos2[i].xyz[2] = xyz.Z + pos[i].xyz[2];
            }
            xyz2all(pos2, ref, row);
            break;
        }
    }
    else if (cfg.I_SYS == 2)
    {
        if (cfg.I_XYZ == 1)
        {
            xyz2all(pos, ref, row);
        }
        else if (cfg.I_BLH == 1)
        {
            blh2all(pos, ref, row);
        }
        else if (cfg.I_ENU == 1)
        {
            enu2all(pos, ref, row);
        }
        else
            printf("####Error#### No Input Files!!!!\n");
        switch (cfg.O_SYS)
        {
        case 0:
            printf("####Error#### No System Imformation!!!\n");
            break;
        case 1:
            for (i = 0; i < row; i++)
            {
                xyz = CGCS2000toWGS84(cfg, pos[i].xyz[0], pos[i].xyz[1], pos[i].xyz[2]);
                pos2[i].xyz[0] = xyz.X + pos[i].xyz[0];
                pos2[i].xyz[1] = xyz.Y + pos[i].xyz[1];
                pos2[i].xyz[2] = xyz.Z + pos[i].xyz[2];
            }
            xyz2all(pos2, ref, row);
            break;
        case 2:
            memcpy(pos2, pos, sizeof(POS) * row);
            break;
        }
    }

    /*-------------------------------------Out Put-----------------------------------*/
    output(pos2, cfg, row, k);

    free(pos), free(pos2);
}