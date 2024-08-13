#include "deletion_queue.h"

void DeletionQueue::push_function(std::function<void()>&& function) {
	deletors.push_back(function);
}

void DeletionQueue::flush() {
	// Reverse iterate the deletion queue to execute all the functions
	for (auto it = deletors.rbegin(); it != deletors.rend(); it++) {
		(*it)(); // Call functions
	}

	deletors.clear();
}