#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#define BUF 100
#define FORMAT_SIZE 50
#define TIMEBUF 3
#define BUFFER 255

int   _simulationDuration;
int   _d1Lambda;
int   _d2Lambda;
char* _output;
char  _outputText[BUF];
FILE* _textFile;

typedef struct _displayTime
{
	int hour;
	int minute;
}displayTime;

typedef struct _statistics
{
	int landings;
	int takeOffs;
	int crashes;
	int emegencies;

}statistics;

typedef struct _plane
{
	displayTime  time;
	int          fuel;
	char*        airLine;
	char*        status;
	int          iD;
	int          passengers;
	int          timeOnGround;
	int          timeToLandOrDepart;
	int          assignRunway;
}plane;

