/** @file GPUResourceManager.h
 *
 *  @author Kristofer Björnson
 */

#include "../include/GPUResourceManager.h"
#include "../include/TBTKMacros.h"

#include <iostream>

using namespace std;

namespace TBTK{

int GPUResourceManager::numDevices;
bool *GPUResourceManager::busyDevices;
omp_lock_t GPUResourceManager::busyDevicesLock;


GPUResourceManager::StaticConstructor GPUResourceManager::staticConstructor;
GPUResourceManager::StaticConstructor::StaticConstructor(){
	numDevices = 0;
	busyDevices = NULL;
	omp_init_lock(&busyDevicesLock);

	createDeviceTable();
}

int GPUResourceManager::allocateDevice(){
	TBTKAssert(
		numDevices > 0,
		"GPUResourceManager::allocateDevice()",
		"No GPU devices available on this machine.",
		"Use CPU version instead."
	);

	int device = 0;
	bool done = false;
	while(!done){
		omp_set_lock(&busyDevicesLock);
		#pragma omp flush
		{
			for(int n = 0; n < numDevices; n++){
				if(!busyDevices[n]){
					device = n;
					busyDevices[n] = true;
					done = true;
					break;
				}
			}
		}
		#pragma omp flush
		omp_unset_lock(&busyDevicesLock);
	}

	return device;
}

void GPUResourceManager::freeDevice(int device){
	omp_set_lock(&busyDevicesLock);
	#pragma omp flush
	{
		busyDevices[device] = false;
	}
	#pragma omp flush
	omp_unset_lock(&busyDevicesLock);
}

};	//End of namespace TBTK