#pragma once

#include "lib.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "util\cuPrintf.cu"

#define BLOCKSIZEX 16

__device__ __constant__ float dConstantElectronsMap[1200];

 int numRowsPointsMap = 16*10, numColsPointsMap = 8*16*4, numElectronsMap;
 float *hEnergyPointsMap,*hElectronsMap;
 float gridspacing = 3.0f;

 float xLimitedLeft = 0.0f;//116.0f;
 float yLimitedBottom = 0.0f;//82.0f;
 float xLimitedRight = numColsPointsMap*3.0;//884.0f;
 float yLimitedTop = numRowsPointsMap*3.0;//418.0f;

__global__ void energy(float *dElectronsMap, float *energyID, float gridspacing, int numatoms){

	int yIndex = blockIdx.y;
	int xIndex = threadIdx.x + threadIdx.y*BLOCKSIZEX*8;
	int outaddr = (blockDim.x*blockDim.y*8)*yIndex + xIndex;
	
//	cuPrintf("Hello world from the thread having block ID");

	float coory = gridspacing * (float)yIndex;
	float coorx = gridspacing * (float)xIndex;
	float gridspacing_coalesce = gridspacing*BLOCKSIZEX;
	
	int atomarrdim = numatoms*4;
	
	float energyvalx1 = 0;
	float energyvalx2 = 0;
	float energyvalx3 = 0;
	float energyvalx4 = 0;
	float energyvalx5 = 0;
	float energyvalx6 = 0;
	float energyvalx7 = 0;
	float energyvalx8 = 0;
	
	for(int atomid = 0; atomid < atomarrdim; atomid+= 4 ){
		float dy = coory - dConstantElectronsMap[atomid+1];
		float dyz2 = (dy*dy) + dConstantElectronsMap[atomid+2];
		float energyOfAtom = dConstantElectronsMap[atomid+3];

		float dx1 = coorx - dConstantElectronsMap[atomid];
		float dx2 = dx1 + gridspacing_coalesce;
		float dx3 = dx2 + gridspacing_coalesce;
		float dx4 = dx3 + gridspacing_coalesce;
		float dx5 = dx4 + gridspacing_coalesce;
		float dx6 = dx5 + gridspacing_coalesce;
		float dx7 = dx6 + gridspacing_coalesce;
		float dx8 = dx7 + gridspacing_coalesce;
		
		

		float distance1 = (int)sqrtf(dx1*dx1+dyz2);
		float distance2 = (int)sqrtf(dx2*dx2+dyz2);
		float distance3 = (int)sqrtf(dx3*dx3+dyz2);
		float distance4 = (int)sqrtf(dx4*dx4+dyz2);
		float distance5 = (int)sqrtf(dx5*dx5+dyz2);
		float distance6 = (int)sqrtf(dx6*dx6+dyz2);
		float distance7 = (int)sqrtf(dx7*dx7+dyz2);
		float distance8 = (int)sqrtf(dx8*dx8+dyz2);

		if((int)distance1 == 0){
			distance1 = 1;
		}

		if((int)distance2 == 0){
			distance2 = 1;
		}

		if((int)distance3 == 0){
			distance3 = 1.0;
		}

		if((int)distance4 == 0){
			distance4 = 1.0;
		}

		if((int)distance5 == 0){
			distance5 = 1.0;
		}

		if((int)distance6 == 0){
			distance6 = 1.0;
		}

		if((int)distance7 == 0){
			distance1 = 1.0;
		}

		if((int)distance8 == 0){
			distance7 = 1.0;
		}
		energyvalx1 += energyOfAtom/distance1;
		energyvalx2 += energyOfAtom/distance2;
		energyvalx3 += energyOfAtom/distance3;
		energyvalx4 += energyOfAtom/distance4;
		energyvalx5 += energyOfAtom/distance5;
		energyvalx6 += energyOfAtom/distance6;
		energyvalx7 += energyOfAtom/distance7;
		energyvalx8 += energyOfAtom/distance8;
		
	}

	energyID[outaddr	] = /*outaddr;*/energyvalx1;
	energyID[outaddr + BLOCKSIZEX]   = /*outaddr + BLOCKSIZEX;*/energyvalx2;
	energyID[outaddr + BLOCKSIZEX*2] = /*outaddr + BLOCKSIZEX*2;*/energyvalx3;
	energyID[outaddr + BLOCKSIZEX*3] = /*outaddr + BLOCKSIZEX*3;*/energyvalx4;
	energyID[outaddr + BLOCKSIZEX*4] = /*outaddr + BLOCKSIZEX*4;*/energyvalx5;
	energyID[outaddr + BLOCKSIZEX*5] = /*outaddr + BLOCKSIZEX*5;*/energyvalx6;
	energyID[outaddr + BLOCKSIZEX*6] = /*outaddr + BLOCKSIZEX*6;*/energyvalx7;
	energyID[outaddr + BLOCKSIZEX*7] = /*outaddr + BLOCKSIZEX*7;*/energyvalx8;
	
}

void calculateEnergyGridsOnGPU(){
   

    float *dEnergyPointsMap;//, *dElectronsMap = NULL;
    int energyPointsMapSize = numRowsPointsMap*numColsPointsMap*sizeof(float);
	int electronsMapSize = numElectronsMap*4*sizeof(float);

	cudaMemcpyToSymbol(dConstantElectronsMap, hElectronsMap,electronsMapSize);
 


	
	cudaError err = cudaMalloc((void **)&dEnergyPointsMap,energyPointsMapSize);
	
	if ( cudaSuccess != err )
		return;

	//err = cudaMalloc((void **)&dElectronsMap,electronsMapSize);
	//// Check error for cudaMalloc
	//if ( cudaSuccess != err )
	//	return;
	// cudaMemcpy(dElectronsMap, hElectronsMap, electronsMapSize, cudaMemcpyHostToDevice);

	 //cudaMemcpy(dElectronsMap, hElectronsMap, electronsMapSize, cudaMemcpyHostToDevice);

	// Excute multi matrix in device
	
	// number of grid.x = n*8*16
	
	dim3 blockD(16, numColsPointsMap/(8*16), 1);
	dim3 gridD(1, numRowsPointsMap,1);
	
	//cudaPrintfInit();

	 energy<<<gridD, blockD>>>(NULL,dEnergyPointsMap,gridspacing,numElectronsMap);
		// display the device's greeting
	//cudaPrintfDisplay();
	// clean up after cuPrintf
	//cudaPrintfEnd();


    // copy memories device to host
    cudaMemcpy(hEnergyPointsMap, dEnergyPointsMap, energyPointsMapSize, cudaMemcpyDeviceToHost);
	for(int i = 0; i < numRowsPointsMap/10; i++){
		 for(int j = 0; j < numColsPointsMap/10; j++){
			 cout << hEnergyPointsMap[i * numColsPointsMap + j] << " ";
		 }
		 cout << endl;
	 }



    // free memories
     cudaFree(dEnergyPointsMap);
    // cudaFree(dElectronsMap);





	  cudaDeviceSynchronize();

}

void readFromFile(char* fileName){
	ifstream myfile;
	myfile.open (fileName);
	
	// get number of elements
	// contruction m n k  /matrix 1 mxn/ matrix 2 nxk 

	myfile >> numElectronsMap;


	hElectronsMap = new float[numElectronsMap*4];
	
	// get hElectronsMap array
	for(int i = 0; i < numElectronsMap*4; i+= 4){
			int c = 0;
			myfile >> c;
			myfile >> hElectronsMap[i] ; // x
			myfile >> hElectronsMap[i + 1] ; // y
			myfile >> hElectronsMap[i + 2]; // z
			myfile >> hElectronsMap[i + 3]; // energy

	//		cout << hElectronsMap[i] << " " << hElectronsMap[i+1] << " " << hElectronsMap[i+2] << " " << hElectronsMap[i+3] <<endl;
	//		hElectronsMap[i] += 50;
	//		hElectronsMap[i+1] += 50;
		}

	


	myfile.close();
}




int cudaMain(int argc,char *argv[]){


   readFromFile("arrayOfElectrons.txt");
	hEnergyPointsMap = new float[numColsPointsMap*numRowsPointsMap];
	
	
	// run with GPU
	 //calculateEnergyGridsOnGPU(3.0f);
	/*
	 for(int i = 0; i < numRowsPointsMap; i++){
		 for(int j = 0; j < numColsPointsMap; j++){
			 cout << hEnergyPointsMap[i * numColsPointsMap + j] << " ";
		 }
		 cout << endl;
	 }
	 
	 */



	return 0;
	
}

