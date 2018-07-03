/*
 * databuffer.h
 *
 *  Created on: Jul 2, 2018
 *      Author: rambonator
 */

#ifndef SRC_RADIO_DATABUFFER_H_
#define SRC_RADIO_DATABUFFER_H_

#include "dataset.h"
#include <stdint.h>


typedef uint16_t Buffersize;

template <Buffersize Size>
class DataBuffer {
public:
	DataBuffer() {
		readIndex = 0;
		writeIndex = 0;
		space = size;
	}

	const DataSet * peekFront() {
		if(isEmpty()) {
			return nullptr;
		}
		const DataSet * data = &dataSetArray[readIndex];
		return data;
	}

	const DataSet * peekBack() {
		if(isEmpty()) {
			return nullptr;
		}
		const DataSet * data = &dataSetArray[(writeIndex-1)%Size];
		return data;
	}

	bool isEmpty() {
		return (space == size);
	}

	bool isFull() {
		return (space == 0);
	}

	Buffersize size() {
		return (Size - space);
	}

	Buffersize spaceRemaining() {
		return space;
	}

	bool pushFront(DataSet dataSet) {
		if(isFull()) {
			return false;
		}
		readIndex = (readIndex - 1)%Size;
		dataSetArray[readIndex] = dataSet;
		space -= 1;
		return true;
	}

	bool pushFront(const DataSet * const dataSet) {
		if(isFull()) {
			return false;
		}
		readIndex = (readIndex - 1)%Size;
		DataSet * const data = &dataSetArray[readIndex];
		data->payload = dataSet->payload;
		space -= 1;
		return true;
	}

	bool pushBack(DataSet dataSet) {
		if(isFull()) {
			return false;
		}
		dataSetArray[writeIndex] = dataSet;
		writeIndex = (writeIndex + 1)%Size;
		space -= 1;
		return true;
	}

	bool pushBack(const DataSet * const dataSet) {
		if(isFull()) {
			return false;
		}
		DataSet * const data = &dataSetArray[writeIndex];
		data->payload = dataSet->payload;
		writeIndex = (writeIndex + 1)%Size;
		space -= 1;
		return true;
	}

	DataSet popFront() {
		if(isEmpty()) {
			return false;
		}
		space += 1;
		return dataSetArray[readIndex++];
	}


	DataSet popBack() {
		if(isEmpty()) {
			return nullptr;
		}
		space += 1;
		return dataSetArray[--writeIndex];
	}


private:
	DataSet dataSetArray[size];
	Buffersize readIndex;
	Buffersize writeIndex;
	Buffersize space;

};


#endif /* SRC_RADIO_DATABUFFER_H_ */
