#pragma once
#include "Simulator.h"

typedef struct _node
{
	plane		    plane;
	struct _node *	next;
} queueNode;

typedef struct _typeNode
{
	queueNode *  head;
	queueNode *  tail;
	char      *  kind;
} queueType;

typedef queueType * queue;

queue CreateQueue(char* qKind);
void Enqueue(queue q, plane pPlane);
void Dequeue(queue q);
bool CheckIfRepeat(queue q, plane pPlane);
plane Front(queue q);
void Print(queue q);
void UpdateValues(queue q);
int Count(queue q);
char* CheckStatus(queue qAir);