// WCH ch571: Flash 192K, RAM: 18K
#include "CH57x_common.h"

void main() {
  SetSysClock(CLK_SOURCE_PLL_60MHz);
  GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeOut_PP_5mA);  // onboard LED

  while (1) {
    GPIOA_InverseBits(GPIO_Pin_8);
    mDelaymS(100);
  }
}
