#include "RunwaysManagement.h"

runway* CreateRunwaysArray(int runways)
{
	runway* runwayArr = (runway*)malloc(sizeof(runway)* runways);
	runway* temp = runwayArr;

	for (int i = 0; i < runways; i++)
	{
		temp->runwayNumber = i + 1;
		temp->assignedPlanes = 0;
		temp->numOfLandings = 0;
		temp->numOfTakeOffs = 0;
		temp->numOfCrashes = 0;
		temp->numOfEmergencies = 0;
		temp->occupied = false;
		temp++;
	}
	return runwayArr;
}

int pickLane()
{
	runway * temp = _runwayArr;
	int selectRunway = FindLowest();
	for (int i = 0; i < _runways; i++)
	{
		if (temp->runwayNumber == selectRunway)
		{
			// here we increment our number of planes waiting to proceed
			temp->assignedPlanes++;
		}
		temp++;
	}
	return selectRunway;
}

int FindLowest()
{
	runway * temp = _runwayArr;
	int min = temp->assignedPlanes;
	int runwayLocation = rand() % (_runways + 1 - 1) + 1;

	for (int i = 0; i < _runways; i++)
	{
		if (temp->assignedPlanes < min)
		{
			runwayLocation = i + 1;
		}
		temp++;
	}
	return runwayLocation;
}

char* CheckStatus(queue qAir)
{
	if (qAir->head == NULL)
	{
		return "OK";
	}
	plane p = Front(qAir);
	if (TimeUntilCrash(p.fuel) > 0)
	{
		return "OK";
	}
	if (TimeUntilCrash(p.fuel) == 0)
	{
		if (p.timeToLandOrDepart <= 10)
		{
			return "Emergency Landing";
		}
		return "Crash";
	}
	return "";
}

void CrashPlane(queue q, displayTime crashTime)
{
	char buf[BUFFER];

	plane p = Front(q);
	snprintf(buf, sizeof buf, "%s %s %d %d %d %d %02d %02d\n",
		"CL", p.airLine, p.iD, p.time.hour, p.time.minute,
		crashTime.hour, crashTime.minute, p.passengers);
	Dequeue(q);
	fprintf(_textFile, buf);
	runway* temp = _runwayArr;
	for (int i = 0; i < _runways; i++)
	{
		if (temp->runwayNumber == p.assignRunway)
		{
			temp->numOfCrashes++;
		}
		temp++;
	}
}

void ifLandOrDepart(queue q, int timeCount)
{
	if (q->head == NULL)
	{
		return;
	}
	static const plane emptyStruct;
	plane p = Front(q);

	if (p.timeToLandOrDepart == 0)
	{
		if (strcmp(p.status, "In Air") == 0)
		{
			if (TimeUntilCrash(p.fuel) == 0)
			{
				// if our plane is about to crash, there is
				// no need to proceed with the code
				return;
			}
		}
		runway* runwayArray = _runwayArr;
		for (int i = 0; i < _runways; i++)
		{
			if (runwayArray->runwayNumber == p.assignRunway && runwayArray->occupied == false)
			{
				char buf[BUFFER];
				displayTime newTime = CalculateTime(p.time, timeCount);
				// proceed with landing or departing
				runwayArray->planeOnLane = p;
				runwayArray->occupied = true;
				if (strcmp(p.status, "In Air") == 0)
				{
					runwayArray->numOfLandings++;
					snprintf(buf, sizeof buf, "%s %s %03d %02d %02d %04d\n",
						                      "EA", p.airLine, p.iD,
						                       newTime.hour, newTime.minute, p.passengers);
				}
				else if (strcmp(p.status, "On Ground") == 0)
				{
					runwayArray->numOfTakeOffs++;
					snprintf(buf, sizeof buf, "%s %s %03d %02d %02d %04d\n",
						                      "TO", p.airLine, p.iD,
						                       newTime.hour, newTime.minute, p.passengers);
				}
				fprintf(_textFile, buf);
				Dequeue(q);
				// we assign new plane from the front of the queue
				// but before, we assign our plane to empty struct 
				// to check if the value was changed, so we would
				// know that queue is not empty
				p = emptyStruct;
				if (p.iD == emptyStruct.iD)
				{
					return;
				}
				p = Front(q);
				
			}
			runwayArray++;
		}
	}
}

void freeRunways()
{
	runway* temp = _runwayArr;
	for (int i = 0; i < _runways; i++)
	{
		temp->occupied = false;
		temp++;
	}
}

statistics GetStatistics()
{
	statistics newStats;
	newStats.landings = 0;
	newStats.takeOffs = 0;
	newStats.crashes = 0;
	newStats.emegencies = 0;
	runway* temp = _runwayArr;
	for (int i = 0; i < _runways; i++)
	{
		newStats.landings += temp->numOfLandings;
		newStats.takeOffs += temp->numOfTakeOffs;
		newStats.crashes += temp->numOfCrashes;
		newStats.emegencies += temp->numOfEmergencies;
		temp++;
	}
	return newStats;
}

void emergencyLanding(queue q, int timeCount)
{
	char buf[BUFFER];
	plane p = Front(q);
	displayTime emergencyTime = CalculateTime(p.time, timeCount);
	runway* temp = _runwayArr;
	for (int i = 0; i < _runways; i++)
	{
		if (temp->runwayNumber == p.assignRunway)
		{
			temp->planeOnLane = p;
			temp->numOfEmergencies++;
		}
		temp++;
	}
	snprintf(buf, sizeof buf, "%s %s %03d %02d %02d %02d %02d %04d\n",
		                      "EL", p.airLine, p.iD, p.time.hour, p.time.minute,
		                       emergencyTime.hour, emergencyTime.minute, p.passengers);
	                           fprintf(_textFile, buf);
	Dequeue(q);
}