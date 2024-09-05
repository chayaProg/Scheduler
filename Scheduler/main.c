﻿#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include "scheduler.h"
#include "rb_tree.h"
#include "real_time_task_queue.h"
#include "mutex.h"
#include "logger.h"
#include "general_settings.h"
#include "consts_logs.h"

void create_input_thread(HANDLE* thread_name, scheduler* sched);
void create_task_thread(HANDLE* thread_name, scheduler* sched);

int main() {
	scheduler sched;

	// Initialize the scheduler and other necessary components
	int success = logger_initFileLogger(LOGGER_FILE, MAX_LOGGER_FILE_SIZE, MAX_LOGGER_BUCKUP_FILES);
	if (success == 0) {
		printf(ERROR_MESSAGE_LOGGER_INIT_FAILED);
		exit(EXIT_FAILURE);
	}
	logger_setLevel(LogLevel_TRACE);

	sched.tasks_tree = initial_rb_tree();
	sched.queue = initialize_queue();

	create_tree_mutex();
	create_queue_mutex();

	HANDLE input_thread_handle, task_thread_handle;

	//Create the input thread
	create_input_thread(&input_thread_handle, &sched);

	// Create the task execution thread
	create_task_thread(&task_thread_handle, &sched);

	// Wait for threads to finish (they run indefinitely in this case)
	WaitForSingleObject(input_thread_handle, INFINITE);
	WaitForSingleObject(task_thread_handle, INFINITE);

	// Clean up resources
	CloseHandle(input_thread_handle);
	CloseHandle(task_thread_handle);

	CloseHandle(queue_mutex);
	CloseHandle(tree_mutex);

	free_queue(sched.queue);
	free_struct_rb_tree(sched.tasks_tree);

	return 0;
}

void create_input_thread(HANDLE* thread_name, scheduler* sched) {
	thread_name = CreateThread(NULL, 0, input_thread, sched, 0, NULL);

	if (thread_name == NULL) {
		LOG_ERROR(ERROR_MESSAGE_CREATE_INPUT_TREAD_FAILED);
		return EXIT_FAILURE;
	}
}

void create_task_thread(HANDLE* thread_name, scheduler* sched) {
	thread_name = CreateThread(NULL, 0, task_thread, sched, 0, NULL);

	if (thread_name == NULL) {
		LOG_ERROR(ERROR_MESSAGE_CREATE_TASK_TREAD_FAILED);
		return EXIT_FAILURE;
	}
}