#pragma once
#include "Queue.h"
#include "Simulator.h"

void GeneratePlanes(queue q, int runways, int lambda, int timeCount);
displayTime CalculateTime(displayTime time, int addMinutes);
char* PickAirLine();
displayTime GetTime();
int PropabilityTable(int k, int lambda);
plane GenerateValues(queue q, int timeCount);
void StatusScreen(queue airQ, queue groundQ, displayTime time);
void CrashScreen(displayTime time, int count);
char* GetPlaneFormat(plane p);
int TimeUntilCrash(int fuel);
void SetOutputText(char* output);
double Factorial(int k);
void LogError(char* airLine, int id);
int* choosePossibility(int tailTime, int tailFuel);