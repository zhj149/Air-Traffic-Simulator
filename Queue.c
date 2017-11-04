#include "Queue.h"

queue CreateQueue(char* qKind)
{
	queue newQueue = (queueType *)malloc(sizeof(queueType));
	newQueue->head = NULL;
	newQueue->tail = NULL;
	newQueue->kind = qKind;
	return newQueue;
}

void Enqueue(queue q, plane pPlane)
{
	if (q == NULL)
	{
		return;
	}
	queueNode * newNode = (queueNode *)malloc(sizeof(queueNode));

	newNode->plane = pPlane;
	newNode->next = NULL;
	if (q->head == NULL && q->tail == NULL)
	{
		q->head = q->tail = newNode;
		return;
	}
	q->tail->next = newNode;
	q->tail = newNode;
}

void Dequeue(queue q)
{
	queueNode * headTemp = q->head;
	if (q == NULL || q->head == NULL || q->tail == NULL)
	{
		return;
	}
	if (q->head == q->tail)
	{
		q->head = q->tail = NULL;
	}
	else
	{
		q->head = q->head->next;
	}
	free(headTemp);
}

bool CheckIfRepeat(queue q, plane p)
{
	queueNode * temp = q->head;
	while (temp != NULL)
	{
		if (p.airLine == temp->plane.airLine &&
			p.iD == temp->plane.iD)
		{
			return true;
		}
		temp = temp->next;
	}
	return false;
}

plane Front(queue q)
{
	queueNode * temp = q->head;
	if (temp == NULL)
	{
		static const plane empty;
		return empty;
	}
	return temp->plane;
}

void Print(queue q)
{
	queueNode* temp = q->head;

	if (temp != NULL)
	{
		for (int i = 0; i < 3; i++)
		{
			printf("%s", GetPlaneFormat(temp->plane));
			temp = temp->next;
			if (temp == NULL)
			{
				break;
			}
		}
	}
		
	printf("\n");
}

int Count(queue q)
{
	int counter = 0;
	queueNode* temp = q->head;
	while (temp != NULL)
	{
		counter++;
		temp = temp->next;
	}
	return counter;
}

void UpdateValues(queue q)
{
	queueNode* temp = q->head;
	char* status = q->kind;
	int fPM = 240; // fuel per minute

	if (temp == NULL)
	{
		return;
	}
	while (temp != NULL)
	{
		if (temp->plane.timeToLandOrDepart == 0)
		{
			// if estimated time is 0 then do nothing
		}
		else
		{
			temp->plane.timeToLandOrDepart--;
		}
		if (strcmp(status, "In Air") == 0)
		{
			temp->plane.fuel -= fPM;
		}
		else if (strcmp(status, "On Ground") == 0)
		{
			temp->plane.timeOnGround++;
		}
		temp = temp->next;
	}
}


