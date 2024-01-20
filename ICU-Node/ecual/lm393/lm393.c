#include "lm393.h"
#include "../../mcal/port/port.h"
#include "../../mcal/icu/Icu.h"
#include "../../mcal/dio/Dio.h"

/* Description: Initializing the GPIO pins interfaced with lm393 */
void LM393_Init(void){
    Port_Init(&Port_PinConfig);
    Dio_Init(&Dio_Configuration);
    Icu_Init(&Icu_Config);
}
/* Description: Calculating speed */
void LM393_GetSpeed(uint32 *speed){
    *speed = Icu_GetEdgeNumbers(2);
}
/* Description: Resetting speed */
void LM393_ResetSpeed(void){
    Icu_ResetEdgeCount(2);
}