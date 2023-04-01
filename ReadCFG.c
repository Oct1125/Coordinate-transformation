#include "main.h"

/*---------------------------------------------*/
//                  空行跳过
//                  首字母‘#’跳过
/*---------------------------------------------*/
// 字符串去掉尾巴
int pstr(char *src)
{
    while (*src)
    {
        if (*src == ' ')
        {
            *src = '\0';
            return 0;
        }
        src++;
    }
    return -1;
}

// 字符串拷贝，考虑前后有空格的情况
int strcopy(const char *src, char *dest, char *lable)
{
    while (*src)
    {
        if (*src == '#')
            return 0;
        else if (*src == ' ')
        {
            src++;
            continue;
        }
        else if (*src != ':')
        {
            *lable++ = *src++;
            continue;
        }
        else if (*src == ':')
        {
            src++;
            while (*src)
            {
                if (*src == ' ')
                {
                    src++;
                    continue;
                }
                if (*src == '#')
                    return 0;
                if (*src == '\n')
                    return 0;
                *dest++ = *src++;
            }
            return 0;
        }
        src++;
    }
    return -1;
}

void my_strcpy(const char *src, char *dest, char *lable)
{
    memset(lable, 0, MAX);
    memset(dest, 0, MAX);
    strcopy(src, dest, lable);
    pstr(dest);
}

// 读取
CFG readCFG(char *cfgname)
{
    CFG cfg = {0};
    FILE *fp;
    fp = fopen(cfgname, "r");
    char buff[MAX] = {0};
    char tmp[MAX] = {0};
    char lable[MAX] = {0};
    while (fgets(buff, MAX, fp))
    {
        my_strcpy(buff, tmp, lable);
        if (strcmp(lable, "data_address") == 0)
        {
            strcpy(cfg.data_address, tmp);
        }
        else if (strcmp(lable, "save_address") == 0)
        {
            strcpy(cfg.save_address, tmp);
        }
        else if (strcmp(lable, "FILE_NAME") == 0)
        {
            strcpy(cfg.FILE_NAME, tmp);
        }
        else if (strcmp(lable, "REFENCE_X") == 0)
        {
            cfg.REFENCE_X = str2num(tmp, 0, strlen(tmp));
        }
        else if (strcmp(lable, "REFENCE_Y") == 0)
        {
            cfg.REFENCE_Y = str2num(tmp, 0, strlen(tmp));
        }
        else if (strcmp(lable, "REFENCE_Z") == 0)
        {
            cfg.REFENCE_Z = str2num(tmp, 0, strlen(tmp));
        }
        else if (strcmp(lable, "PARAMATERS") == 0)
        {
            sscanf(tmp, "%lf%lf%lf%lf%lf%lf%lf", &(cfg.PARAMATERS[0]), &(cfg.PARAMATERS[1]), &(cfg.PARAMATERS[2]),
                   &(cfg.PARAMATERS[3]), &(cfg.PARAMATERS[4]), &(cfg.PARAMATERS[5]), &(cfg.PARAMATERS[6]));
        }
        else if (strcmp(lable, "I_SYS") == 0)
        {
            if (strcmp(tmp, "WGS84") == 0)
                cfg.I_SYS = 1;
            else if (strcmp(tmp, "CGCS2000") == 0)
                cfg.I_SYS = 2;
            else
                cfg.I_SYS = 0;
        }
        else if (strcmp(lable, "I_XYZ") == 0)
        {
            if (strcmp(tmp, "YES") == 0)
                cfg.I_XYZ = 1;
            else if (strcmp(tmp, "NO") == 0)
                cfg.I_XYZ = 0;
            else
                cfg.I_XYZ = -1;
        }
        else if (strcmp(lable, "I_BLH") == 0)
        {
            if (strcmp(tmp, "YES") == 0)
                cfg.I_BLH = 1;
            else if (strcmp(tmp, "NO") == 0)
                cfg.I_BLH = 0;
            else
                cfg.I_BLH = -1;
        }
        else if (strcmp(lable, "I_ENU") == 0)
        {
            if (strcmp(tmp, "YES") == 0)
                cfg.I_ENU = 1;
            else if (strcmp(tmp, "NO") == 0)
                cfg.I_ENU = 0;
            else
                cfg.I_ENU = -1;
        }
        else if (strcmp(lable, "O_SYS") == 0)
        {
            if (strcmp(tmp, "WGS84") == 0)
                cfg.O_SYS = 1;
            else if (strcmp(tmp, "CGCS2000") == 0)
                cfg.O_SYS = 2;
            else
                cfg.O_SYS = 0;
        }
        else if (strcmp(lable, "O_XYZ") == 0)
        {
            if (strcmp(tmp, "YES") == 0)
                cfg.O_XYZ = 1;
            else if (strcmp(tmp, "NO") == 0)
                cfg.O_XYZ = 0;
            else
                cfg.O_XYZ = -1;
        }
        else if (strcmp(lable, "O_BLH") == 0)
        {
            if (strcmp(tmp, "YES") == 0)
                cfg.O_BLH = 1;
            else if (strcmp(tmp, "NO") == 0)
                cfg.O_BLH = 0;
            else
                cfg.O_BLH = -1;
        }
        else if (strcmp(lable, "O_ENU") == 0)
        {
            if (strcmp(tmp, "YES") == 0)
                cfg.O_ENU = 1;
            else if (strcmp(tmp, "NO") == 0)
                cfg.O_ENU = 0;
            else
                cfg.O_ENU = -1;
        }
    }
    fclose(fp);
    return cfg;
}