#pragma once
#include "Simulator.h"
#include "Queue.h"
#include "OperationsAndUtilities.h"

typedef struct _runway
{
	int		 runwayNumber;
	plane    planeOnLane;
	int      numOfLandings;
	int      numOfTakeOffs;
	int      assignedPlanes;
	bool     occupied;
	int      numOfCrashes;
	int      numOfEmergencies;
}runway;

int   _runways;
runway* _runwayArr;

int FindLowest();
runway* CreateRunwaysArray(int runways);
int pickLane();
void CrashPlane(queue q, displayTime crashTime);
void ifLandOrDepart(queue q, int timeCount);
void freeRunways();
statistics GetStatistics();
void emergencyLanding(queue q, int timeCount);
