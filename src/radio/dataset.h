/*
 * dataset.h
 *
 *  Created on: Jul 3, 2018
 *      Author: rambonator
 */

#ifndef SRC_RADIO_DATASET_H_
#define SRC_RADIO_DATASET_H_

#include <stdint.h>
#include "../util/configExtern.h"


struct DataSet {
	uint8_t payload[MAX_PAYLOAD_LEN];
};


#endif /* SRC_RADIO_DATASET_H_ */
