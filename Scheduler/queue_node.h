#ifndef QUEUE_NODE_H
#define QUEUE_NODE_H
#include "real_time_task.h"

struct queue_node {
	real_time_task* task;
	struct queue_node* next;
}typedef queue_node;

queue_node* create_queue_node(int priority, double execution_time, long double weight);
void free_queue_node(queue_node* node);
#endif