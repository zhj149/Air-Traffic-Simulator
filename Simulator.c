#include "Simulator.h"
#include "Queue.h"
#include "RunwaysManagement.h"
#include "OperationsAndUtilities.h"

void StatusScreen(queue airQ, queue groundQ, displayTime time);
void CrashScreen(displayTime time, int count);

int main(int argc, char* argv[])
{
	time_t start, end;
	double seconds;
	double interval = 1.00; // 1 minute interval is virtually 1 seconds
	int timeCount = 0;
	int crashCounter = 15;
	char* possibility;
	char* oK = "OK";
	char* emergency = "Emergency Landing";
	char* crash = "Crash";
	bool crashStatus = false;

	if (argc != 6)
	{
		printf("Error. The program does not pass enough arguments from command line.\n");
		return EXIT_FAILURE;
	}
	else
	{
		_runways = atoi(argv[1]);
		_simulationDuration = atoi(argv[2]);
		_d1Lambda = atoi(argv[3]);
		_d2Lambda = atoi(argv[4]);
		_output = argv[5];
		queue airSpaceQueue = CreateQueue("In Air");
		queue onGroundQueue = CreateQueue("On Ground");
		_runwayArr = CreateRunwaysArray(_runways);
		SetOutputText(_output);
		// seed random number generator before the loop starts
		srand((unsigned)time(NULL));
		start = time(NULL);
		displayTime timeDisplay = GetTime();
		while (_simulationDuration != 0)
		{
			end = time(NULL);
			seconds = difftime(end, start);
			if (seconds == interval)
			{

				if (!crashStatus)
				{
					ifLandOrDepart(airSpaceQueue, timeCount);
				}
				possibility = CheckStatus(airSpaceQueue);
				// if the status is "OK" and there arent any crashes
				if (strcmp(possibility, oK) == 0 && !crashStatus)
				{
					ifLandOrDepart(onGroundQueue, timeCount);
					GeneratePlanes(airSpaceQueue, _runways, _d1Lambda, timeCount);
					GeneratePlanes(onGroundQueue, _runways, _d2Lambda, timeCount);
					UpdateValues(airSpaceQueue);
					UpdateValues(onGroundQueue);
					StatusScreen(airSpaceQueue, onGroundQueue, CalculateTime(timeDisplay, timeCount));
					freeRunways();
				}
				else if (strcmp(possibility, crash) == 0 || crashStatus)
				{
					if (!crashStatus)
					{
						CrashPlane(airSpaceQueue, CalculateTime(timeDisplay, timeCount));
						crashStatus = true;
					}
					CrashScreen(CalculateTime(timeDisplay, timeCount), crashCounter);
					UpdateValues(airSpaceQueue);
					UpdateValues(onGroundQueue);
					possibility = CheckStatus(airSpaceQueue);
					if (strcmp(possibility, crash) == 0)
					{
						CrashPlane(airSpaceQueue, CalculateTime(timeDisplay, timeCount));
						crashCounter += 15;
					}
					crashCounter--;
					if (crashCounter == 0)
					{
						crashStatus = false;
						crashCounter = 15;
					}
				}
				else if (strcmp(possibility, emergency) == 0 && !crashStatus)
				{
					emergencyLanding(airSpaceQueue, timeCount);
					UpdateValues(airSpaceQueue);
					UpdateValues(onGroundQueue);
					StatusScreen(airSpaceQueue, onGroundQueue, CalculateTime(timeDisplay, timeCount));
				}
				timeCount++;
				_simulationDuration--;
				start = time(NULL);
			}
		}
		fclose(_textFile);
	}
	return 0;
}

void StatusScreen(queue airQ, queue groundQ, displayTime time)
{
	system("cls");
	statistics stats = GetStatistics();
	printf(" ------------------------------------------------ \n");
	printf("|                Minutes left: %02d                |\n", _simulationDuration);
	printf("|                The time: %02d:%02d                 |\n", time.hour, time.minute);
	printf("|                                                |\n");
	printf("|           %02d planes in the air queue           |\n", Count(airQ));
	printf("|           %02d planes in the ground queue        |\n", Count(groundQ));
	printf("|           %02d successful landings               |\n", stats.landings);
	printf("|           %02d succesfull take offs              |\n", stats.takeOffs);
	printf("|           %02d emergency landings                |\n", stats.emegencies);
	printf("|           %02d crashed planes                    |\n", stats.crashes);
	printf("|                                                |\n");
	printf(" ------------------------------------------------ \n\n");
	printf("First three airplanes in the air queue: \n\n");
	Print(airQ);
	printf("First three airplanes in the ground queue: \n\n");
	Print(groundQ);
}

void CrashScreen(displayTime time, int count)
{
	system("cls");

	printf(" ------------------------------------------------ \n");
	printf("|                Minutes left: %02d                |\n", _simulationDuration);
	printf("|                The time: %02d:%02d                 |\n", time.hour, time.minute);
	printf("|                                                |\n");
	printf("|                Plane has crashed.              |\n");
	printf("|                %02d minutes left                 |\n", count);
	printf("|                                                |\n");
	printf(" ------------------------------------------------ \n");
}





