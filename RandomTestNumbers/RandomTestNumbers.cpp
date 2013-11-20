// RandomTestNumbers.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
using namespace std;


float RandomFloat(float min, float max)
{
    // this  function assumes max > min, you may want 
    // more robust error checking for a non-debug build

    float random = ((float) rand()) / (float) RAND_MAX;

    // generate (in your case) a float between 0 and (4.5-.78)
    // then add .78, giving you a float between .78 and 4.5
    float range = max - min;  
    return (random*range) + min;
}

double RandomDouble(float min, float max)
{
    // this  function assumes max > min, you may want 
    // more robust error checking for a non-debug build

    double random = ((double) rand()) / (double) RAND_MAX;

    // generate (in your case) a float between 0 and (4.5-.78)
    // then add .78, giving you a float between .78 and 4.5
    double range = max - min;

    return (random*range) + min;
}

int _tmain(int argc, _TCHAR* argv[])
{


	srand(time(NULL)); /* seed random number generator */

	int n = 300;
	cout << "number of electronics :";
	//cin >> n;




	ofstream myfile ("arrayOfElectrons.txt");
	if (myfile.is_open())
	{
		myfile << n << endl;
		for(int i = 0; i < n; i++){

			myfile << i << " ";
			// x coordinate
			myfile << (int)RandomFloat(300, 760) << " ";

			// y coordinate
			myfile << (int)RandomFloat(200, 336) << " ";

			// z coordinate
			myfile << 0 << " ";

			// energy
			myfile << RandomFloat(1.0f, 20.0f) << endl;


		}
		
		myfile.close();
	}
	else cout << "Unable to open file";

	cout << "Done!";
	return 0;
}

