#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include "Vector2.h"
#include "Globals.h"

#include <thread>
#include <condition_variable>
#include <immintrin.h>
#include <atomic>
#include <complex>

class WorkerThread
{
public:	
	std::thread m_thread;
	std::mutex m_mutex;
	std::condition_variable m_cvStart;
	bool m_alive = true;	
	Vector2 m_pixTL = { 0, 0 };
	Vector2 m_pixBR = { 0, 0 };
	Vector2 m_fracTL = { 0, 0 };
	Vector2 m_fracBR = { 0, 0 };
	int m_iterations = 0;
	int m_screenWidth = 0;
	int *m_fractal = nullptr;

	WorkerThread();
	~WorkerThread();
	void start(const Vector2 &t_pixTL, const Vector2 &t_pixBR, const Vector2 &t_fracTL, const Vector2 &t_fracBR, const int t_iterations);
	void createFractal();	
};

#endif // !WORKERTHREAD_H
