#include "OperationsAndUtilities.h"

void GeneratePlanes(queue q, int runways, int lambda, int timeCount)
{
	int k = rand() % runways + 1;
	int numberOfPlanes = PropabilityTable(k, lambda);

	if (numberOfPlanes != 0)
	{
		for (int i = 0; i <= numberOfPlanes; i++)
		{
			plane newPlane = GenerateValues(q, timeCount);
			if (CheckIfRepeat(q, newPlane))
			{
				// if the airplane repeats, then we report an error to a log, 
				// and increment looping value by one to create a new airplane.
				LogError(newPlane.airLine, newPlane.iD);
				numberOfPlanes++;
			}
			else
			{
				Enqueue(q, newPlane);
			}
		}
	}
}

displayTime CalculateTime(displayTime time, int addMinutes)
{
	time.hour += addMinutes / 60;
	time.minute += addMinutes % 60;
	if (time.minute > 60)
	{
		time.minute -= 60;
		time.hour++;
	}
	else if (time.minute == 60)
	{
		time.hour++;
		time.minute = 0;
	}
	if (time.hour >= 24)
	{
		time.hour = 0;
	}
	return time;
}

char* PickAirLine()
{
	char* airlines[9];

	airlines[0] = "RA"; // ryanair;
	airlines[1] = "WA"; // wizzair, atc...;
	airlines[2] = "EJ";
	airlines[3] = "ER";
	airlines[4] = "QA";
	airlines[5] = "TA";
	airlines[6] = "DA";
	airlines[7] = "WJ";
	airlines[8] = "JA";
	return airlines[rand() % 9];
}

displayTime GetTime()
{
	time_t curtime;
	struct tm *loc_time;
	displayTime displayT;
	char hours[TIMEBUF];
	char minutes[TIMEBUF];

	curtime = time(NULL);
	loc_time = localtime(&curtime);
	strftime(hours, TIMEBUF, "%H", loc_time);
	strftime(minutes, TIMEBUF, "%M", loc_time);
	displayT.hour = atoi(hours);
	displayT.minute = atoi(minutes);
	return displayT;
}

int PropabilityTable(int k, int lambda)
{
	double temp;
	double random;
	// setting maximum, to our first value in the table
	double max = pow(lambda, 0) * exp(-lambda) / Factorial(0);
	int result = 0;

	random = (float)rand() / (float)((unsigned)RAND_MAX + 1);
	for (int i = 0; i < k; i++)
	{
		temp = pow(lambda, i) * exp(-lambda) / Factorial(i);
		if (max < temp)
		{
			max = temp;
		}
		if (random <= max)
		{
			result = i;
		}
	}
	// result tells us how many planes will be generated during this interval
	return result;
}

plane GenerateValues(queue q, int timeCount)
{
	char* indentify = q->kind;
	plane newPlane;
	int maxFuel = 33000;
	int min = 10;
	int max = 20;
	int* newPossibility;

	newPlane.status = indentify;
	if (strcmp(indentify, "On Ground") == 0)
	{
		newPlane.timeOnGround = 0;
	}
	else
	{
		newPlane.fuel = rand() % maxFuel;
	}
	if (q->head == NULL)
	{
		// new random value within the range for the first node
		newPlane.timeToLandOrDepart = rand() % (max + 1 - min) + min;
	}
	else
	{
		// we take the queue tail value
		queueNode* tempNode = q->tail;
		int tailTimeTo = tempNode->plane.timeToLandOrDepart;
		int tailFuel = tempNode->plane.fuel;
		if (strcmp(indentify, "In Air") == 0)
		{
			newPossibility = choosePossibility(tailTimeTo, tailFuel);
			newPlane.timeToLandOrDepart = newPossibility[0];
			newPlane.fuel = newPossibility[1];
		}
		else
		{
			newPlane.timeToLandOrDepart = tailTimeTo + rand() % max;
		}
	}
	displayTime newTime = GetTime();
	newTime = CalculateTime(newTime, newPlane.timeToLandOrDepart + timeCount);
	newPlane.time = newTime;
	newPlane.iD = rand() % (1000 + 1 - 1) + 1;
	newPlane.passengers = rand() % (2000 + 1 - 300) + 300;
	newPlane.airLine = PickAirLine();
	newPlane.assignRunway = pickLane();
	return newPlane;
}

char* GetPlaneFormat(plane p)
{
	static char buf[BUFFER];

	if (strcmp(p.status, "In Air") == 0)
	{
		snprintf(buf, sizeof buf, "%s %s %03d %02d %02d %04d %d %d   %s%d\n",
			                      "EA", p.airLine, p.iD, p.time.hour,
			                       p.time.minute, p.passengers,
			                       p.timeToLandOrDepart, TimeUntilCrash(p.fuel), "Fuel: ", p.fuel);
		return buf;
	}
	// if status == "On Ground", proceed with the code bellow
	snprintf(buf, sizeof buf, "%s %s %03d %02d %02d %04d   %s%d\n",
		                       "TO", p.airLine, p.iD, p.time.hour, p.time.minute,
		                       p.passengers, "Waiting Time: ", p.timeOnGround);
	return buf;
}
int* choosePossibility(int tailTime, int tailFuel)
{
	static int arr[1];
	int newTime = 0;
	int newFuel = 0;
	int calculatedFuel = TimeUntilCrash(tailFuel);
	int maxRandom = 30;
	// random number between 1 and 2
	int randomizer = rand() % (2 + 1 - 1) + 1;

	if (calculatedFuel <= tailTime)
	{
		if (randomizer == 1)
		{
			newFuel = calculatedFuel + rand() % maxRandom;
			newTime = newFuel + rand() % maxRandom;
		}
		else if (randomizer == 2)
		{
			newTime = calculatedFuel + rand() % maxRandom;
			newFuel = newTime + rand() % maxRandom;
		}
	}
	else if (calculatedFuel >= tailTime)
	{
		if (randomizer == 1)
		{
			newFuel = tailTime + rand() % maxRandom;
			newTime = newFuel + rand() % maxRandom;
		}
		else
		{
			newTime = tailTime + rand() % maxRandom;
			newFuel = newTime + rand() % maxRandom;
		}
	}
	// array with the new time as first value,
	// and new fuel as second value
	arr[0] = newTime;
	arr[1] = newFuel * 240;
	return arr;
}

void LogError(char* airLine, int id)
{
	char error[30];

	snprintf(error, sizeof error, "%s %s %d\n", "ER", airLine, id);
	                              fprintf(_textFile, error);
	printf("Error\n");
}

void SetOutputText(char* output)
{
	snprintf(_outputText, sizeof _outputText, "%s%s%s", "100430319.", output, ".txt");
	         _textFile = fopen(_outputText, "w");
}

int TimeUntilCrash(int fuel)
{
	int fPM = 240; // fuel units per minute
	return fuel / fPM;
}


double Factorial(int k)
{
	double result = 1;
	for (int i = 1; i <= k; i++)
	{
		result = result * i;
	}
	return result;
}