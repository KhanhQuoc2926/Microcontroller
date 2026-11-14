#ifndef SCH_PORT_H
#define SCH_PORT_H

#include <stdint.h>
#ifdef SCH_HOST_TEST
static inline uint32_t SCH_Lock(void){
	return 0u;
}
static inline void SCH_Unlock(uint32_t m) {(void)m;}
#else

#include "main.h"

static inline uint32_t SCH_Lock(void){
	uint32_t primask = __get_PRIMASK();								// Store the state before lock
	__disable_irq();																	// Lock interrupt in the change data
	return primask;
}

static inline void SCH_Unlock(uint32_t primask){
	__set_PRIMASK(primask);
}	
#endif
#endif
