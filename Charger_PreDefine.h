
#ifndef __CHARGER_PRE_DEFINE_H__
#define __CHARGER_PRE_DEFINE_H__

// 统一编译选项, 把AC/DC, MT7628/AM3354/SSD202合并到一个项目中, 集中处理各种编译选项
#define COMPILE_FOR_AC_X86          0
#define COMPILE_FOR_AC_MT7628       1           // MT7628主板, 目前用于家用塑壳和商用铁壳/落地
#define COMPILE_FOR_DC_AM3354       2           // AM3354主板, 目前用于嘉盛直流桩
#define COMPILE_FOR_AC_SSD202       3           // SSD202主板, 目前替代MT7628主板, 后续可能替换直流桩, 待定
#define COMPILE_FOR_CABINET_AM3354  4           // AM3354主板, 目前用于分体式直流桩
#define COMPILE_FOR_CABINET_SSD202  5           // SSD202控制盒主板, 分体式直流桩的终端控制盒用于小直流和移动桩时使用
#define COMPILE_FOR_SSD202_43       6           //4.3寸屏幕

#define CHARGER_TYPE_AC_ONLY        1           // 全交流枪充电桩
#define CHARGER_TYPE_DC             2           // 直流枪充电桩, 可包含交流(交直流一体)
#define CHARGER_TYPE_DC_CABINET     3           // 直流分体充电桩(可包含交流终端) 和采用分体架构的小直流等

#endif
