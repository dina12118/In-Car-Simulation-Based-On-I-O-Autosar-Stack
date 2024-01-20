#ifndef LM393_H_
#define LM393_H_

#include "../../library/Platform_Types.h"
#include "../../library/Std_Types.h"
#include "../../library/common_macros.h"

#define LM393_PORT       5
#define LM393_PIN        5
#define LM393_ID         5

/* Description: Initializing the GPIO pins interfaced with lm393 */
void LM393_Init(void);
/* Description: Calculating speed */
void LM393_GetSpeed(uint32 *speed);
/* Description: Resetting speed */
void LM393_ResetSpeed(void);

#endif