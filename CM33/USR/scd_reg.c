/**
 * @file scd_reg.cpp
 * @brief SCD注册文件
 * @author Tangent (498339337@qq.com)
 * @date 2022-04-08
 *
 * @copyright Copyright (c) 2022 @Tangent
 */

#include "scd_inc.h"

 /**
  * @brief 注册宏起始，尾缀标识具体实例名称
  */
#define SCD_REG_BEGIN(TYJ_SUFFIX)                                                                      \
    volatile unsigned char recBuff##TYJ_SUFFIX[2][SCD_REVBUFF_SIZE];                                   \
    volatile unsigned char printBuff##TYJ_SUFFIX[SCD_PRINTBUFF_SIZE];                                  \
    volatile unsigned char endBuff1##TYJ_SUFFIX[4] = {0x00, 0x00, 0x81, 0x7f};                         \
    volatile unsigned char endBuff2##TYJ_SUFFIX[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0x7f}; \
    volatile unsigned char endBuff3##TYJ_SUFFIX[4] = {0x00, 0x00, 0x83, 0x7f};                         \
    static const int _begin_index##TYJ_SUFFIX = __LINE__;                                              \
    const struct TYJ_UNIT_STRUCT tyj_unit_struct##TYJ_SUFFIX[] = {

  /**
   * @brief 注册宏，中间不可加入空行或注释
   * @param TYJ_NAME 需加入的参数名称
   * @param TYJ_TYPE 需加入的参数类型
   */
  #define SCD_REG_ADD(TYJ_NAME, TYJ_TYPE)     \
      {                                       \
          (void *)&(TYJ_NAME), TYJ_##TYJ_TYPE \
                                              \
      },

   /**
    * @brief 注册宏结尾，尾缀标识具体实例名称
    */
   #define SCD_REG_END(TYJ_SUFFIX)                                                            \
    }                                                                                      \
    ;                                                                                      \
    static const int _end_index##TYJ_SUFFIX = __LINE__;                                    \
    struct SCD_CTRL_STRUCT scd##TYJ_SUFFIX;                                                \
    void scd_init##TYJ_SUFFIX()                                                            \
    {                                                                                      \
        scd##TYJ_SUFFIX._unit_struct = tyj_unit_struct##TYJ_SUFFIX;                        \
        scd##TYJ_SUFFIX.structToSendTab[0] = 0;                                            \
        scd##TYJ_SUFFIX.structToSendTab[1] = 9999;                                         \
        scd##TYJ_SUFFIX._recBuff = recBuff##TYJ_SUFFIX;                                    \
        scd##TYJ_SUFFIX._printBuff = printBuff##TYJ_SUFFIX;                                \
        scd##TYJ_SUFFIX._endbyte1 = endBuff1##TYJ_SUFFIX;                                  \
        scd##TYJ_SUFFIX._endbyte2 = endBuff2##TYJ_SUFFIX;                                  \
        scd##TYJ_SUFFIX._endbyte3 = endBuff3##TYJ_SUFFIX;                                  \
        scd##TYJ_SUFFIX.structNum = _end_index##TYJ_SUFFIX - _begin_index##TYJ_SUFFIX - 1; \
        scd##TYJ_SUFFIX.bufNum = 0;                                                        \
        scd##TYJ_SUFFIX.bytesRec = 0;                                                      \
        scd##TYJ_SUFFIX.structToSend = 0;                                                  \
        scd##TYJ_SUFFIX.byteToSend = 0;                                                    \
        scd##TYJ_SUFFIX.tempBuff = 0;                                                      \
        scd##TYJ_SUFFIX.sco_protocol_num = 0;                                              \
        scd##TYJ_SUFFIX.sco_protocol_num_next = 0;                                         \
        scd##TYJ_SUFFIX.structToSend2 = 0;                                                 \
        scd##TYJ_SUFFIX.byteToSend2 = 0;                                                   \
        scd##TYJ_SUFFIX.byteToSend3 = 0;                                                   \
        scd##TYJ_SUFFIX.structToSend3 = 0;                                                 \
        scd##TYJ_SUFFIX.dumpTarget = 0;                                                    \
        scd##TYJ_SUFFIX.dumpNumCnt = 0;                                                    \
        scd##TYJ_SUFFIX.dumpPkgCnt = 0;                                                    \
        scd##TYJ_SUFFIX.ifContPkg = 0;                                                     \
        scd##TYJ_SUFFIX.isPrintBusyFlg = 0;                                                \
        scd##TYJ_SUFFIX.byteToPrint = 0;                                                   \
        scd##TYJ_SUFFIX.byteToSend4 = 0;                                                   \
    }

#include "math.h"
#include "main.h"

    /**
     * @brief 注册宏1#scd的观测内容
     */

SCD_REG_BEGIN(_1)
SCD_REG_ADD(millisCnt, uint32_t)
SCD_REG_ADD(test1, uint16_t)
SCD_REG_ADD(test2, uint16_t)
SCD_REG_ADD(test3, uint16_t)
SCD_REG_ADD(test4, uint16_t)
SCD_REG_ADD(test1r, uint16_t)
SCD_REG_ADD(test2r, uint16_t)
SCD_REG_ADD(test3r, uint16_t)
SCD_REG_ADD(test4r, uint16_t)
SCD_REG_END(_1)
