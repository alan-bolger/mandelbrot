#ifndef GLOBALS_H
#define GLOBALS_H

#include <cstdint>
#include <atomic>

class Globals
{
public:
	static const int SCREEN_WIDTH = 1280;
	static const int SCREEN_HEIGHT = 720;
	static const int MAX_THREADS = 32;

	static std::atomic<int> WORKER_COMPLETE;

	static const uint8_t DEFAULT_FONT[];
};


#endif // !GLOBALS_H
