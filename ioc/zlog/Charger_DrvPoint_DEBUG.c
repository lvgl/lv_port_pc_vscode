
#include "ioc_app.h"
#include "zlog.h"

#include <unistd.h>
#include <sys/stat.h>

typedef time_t dateTime;

#define LOG_FILE_SELF_PREFIX    "lvgl"

#define LOG_FILE_SIZE_LIMIT     "256KB * 1"

#ifndef MODBUS_MASTER
#define ZLOG_CONF_FILE "/tmp/zlog_config_dispenser_lvgl.conf"
#define ZLOG_LOCK_FILE "/tmp/zlog_config_dispenser_lvgl.lock"
#else
#define ZLOG_CONF_FILE "/tmp/zlog_config_modbus.conf"
#define ZLOG_LOCK_FILE "/tmp/zlog_config_modbus.lock"
#endif

static char ZLOG_EXT_PATH[32] = "";
static dateTime s_zlogLoadTime = 0;

zlog_category_t *c_charger = NULL;
zlog_category_t *c_hex = NULL;


char RUNDIR[512];

void Zlog_GenerateConf()
{
    char confBuffer[8192] = "";

    strcat(confBuffer, "[global]\n");
    strcat(confBuffer, "strict init = true\n");
    strcat(confBuffer, "buffer min = 1024\n");
    strcat(confBuffer, "buffer max = 2MB\n");
    strcat(confBuffer, "rotate lock file = "ZLOG_LOCK_FILE"\n");
    strcat(confBuffer, "default format = \"[%d(%T).%ms|%f|%L]%m%n%n\"\n");
    strcat(confBuffer, "file perms = 600\n");
    strcat(confBuffer, "\n");
    strcat(confBuffer, "[levels]\n");
    strcat(confBuffer, "TRACE = 10\n");
    strcat(confBuffer, "CRIT = 130, LOG_CRIT\n");
    strcat(confBuffer, "\n");
    strcat(confBuffer, "[formats]\n");
    strcat(confBuffer, "fileFormat = \"[%d(%T).%ms]%m%n%n\"\n");
    strcat(confBuffer, "hexFileFormat = \"[%d(%T).%ms]%m%n%n\"\n");
    strcat(confBuffer, "\n");
    strcat(confBuffer, "[rules]\n");
#ifdef x86
    strcat(confBuffer, "charger.DEBUG        >stdout; fileFormat\n");
#else
    // 网跑状态直接输出日志
    const char * DftPath = "/mnt/";
    if (memcmp(RUNDIR, DftPath, strlen(DftPath)) == 0)
    {
        strcat(confBuffer, "charger.DEBUG        >stdout; fileFormat\n");
    }
#endif
#ifndef MODBUS_MASTER
    strcat(confBuffer, "charger.DEBUG        \""IOC_PATH"" LOG_FILE_SELF_PREFIX ".%d(%F).log\", "LOG_FILE_SIZE_LIMIT";fileFormat\n");
#else
    strcat(confBuffer, "charger.DEBUG        \""IOC_PATH"master.%d(%F).log\", "LOG_FILE_SIZE_LIMIT";fileFormat\n");
#endif
    strcat(confBuffer, "\n");
#ifdef x86
    strcat(confBuffer, "hex.DEBUG            >stdout; hexFileFormat\n");
#endif
#ifndef MODBUS_MASTER
    strcat(confBuffer, "hex.DEBUG            \""IOC_PATH""LOG_FILE_SELF_PREFIX".%d(%F).log\", "LOG_FILE_SIZE_LIMIT";fileFormat\n");
#else
    strcat(confBuffer, "hex.DEBUG            \""IOC_PATH"master.%d(%F).log\", "LOG_FILE_SIZE_LIMIT";fileFormat\n");
#endif
    strcat(confBuffer, "\n");

    if (ZLOG_EXT_PATH[0] != '\0')
    {
        strcat(confBuffer, "charger.DEBUG        \"");
        strcat(confBuffer, ZLOG_EXT_PATH);
#ifndef MODBUS_MASTER
        strcat(confBuffer, LOG_FILE_SELF_PREFIX ".%d(%F).log\", 100MB;fileFormat\n");
#else
        strcat(confBuffer, "master.%d(%F).log\", 100MB;fileFormat\n");
#endif
        strcat(confBuffer, "hex.DEBUG        \"");
        strcat(confBuffer, ZLOG_EXT_PATH);
#ifndef MODBUS_MASTER
        strcat(confBuffer, LOG_FILE_SELF_PREFIX ".%d(%F).log\", 100MB;hexFileFormat\n");
#else
        strcat(confBuffer, "master.%d(%F).log\", 100MB;hexFileFormat\n");
#endif
        strcat(confBuffer, "\n");
    }

    FILE * fp = fopen(ZLOG_CONF_FILE, "w+");
    if (fp == NULL)
    {
        printf("can't open "ZLOG_CONF_FILE);
        return;
    }

    fputs(confBuffer, fp);

    fclose(fp);
}

int Zlog_Load(bool init)
{
    int rc;

    // 根据当前外置存储生成配置文件
    Zlog_GenerateConf();

    if (init)
    {
        rc = zlog_init(ZLOG_CONF_FILE);
    }
    else
    {
        rc = zlog_reload(ZLOG_CONF_FILE);
    }
    if (rc)
    {
        printf("init %d failed\n", init);
        return -1;
    }

    c_charger = zlog_get_category("charger");
    if (!c_charger)
    {
        printf("get c_charger fail\n");
        zlog_fini();
        return -2;
    }

    c_hex = zlog_get_category("hex");
    if (!c_hex)
    {
        printf("get c_hex fail\n");
        zlog_fini();
        return -2;
    }

    time_t now = time(NULL);
    s_zlogLoadTime = now;

    zlog_notice(c_charger, "ZlogInit(%d) for "ZLOG_CONF_FILE" EXT %s\n", init, ZLOG_EXT_PATH);
    return 0;
}

int Charger_zlog_init()
{
    printf("zlog_init!\n");
    return Zlog_Load(true);
}

void Charger_zlog_release()
{
    zlog_fini();
}

