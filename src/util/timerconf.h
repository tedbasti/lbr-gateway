#ifndef TIMERCONF_H
#define TIMERCONF_H

#include <stdint.h>

namespace TimerConf {
enum Values {
	Prescale = 256,
	PeriodLength = 0x0800,
	Frequency = 2000,
};
}

#endif
