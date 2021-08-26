#ifndef APPLICATION_H
#define APPLICATION_H

#include "Vector2.h"
#include "PixelGrid.h"
#include "WorkerThread.h"
#include "Globals.h"

#include <SFML/Graphics.hpp>
#include <chrono>
#include <complex>
#include <iostream>
#include <thread>

class Application
{
public:
	int *m_fractal = nullptr;
	int m_mode = 2;
	int m_iterations = 1024;

	Application();
	~Application();
	void run();

private:
	sf::RenderWindow m_window;
	sf::RenderTexture m_renderTexture;
	PixelGrid m_pixelGrid{ Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT };
	sf::Font m_font;
	bool m_exitGame{ false };
	bool m_leftBtnClicked = false;
	bool m_rightBtnClicked = false;
	std::chrono::duration<double> m_elapsedTime;
	WorkerThread m_workers[Globals::MAX_THREADS];
	Vector2 m_offset = { 0.0f, 0.0f };
	Vector2 m_startPan = { 0.0f, 0.0f };
	Vector2 m_scale = { Globals::SCREEN_WIDTH / 2.0f, Globals::SCREEN_HEIGHT };

	void processEvents();
	void update();
	void draw();
	void drawString(int t_x, int t_y, std::string t_string, sf::Color t_colour, int t_size = 20);
	void drawText();
	void worldToScreen(const Vector2 &t_world, Vector2 &t_screen);
	void screenToWorld(const Vector2 &t_screen, Vector2 &t_world);
	void createFractalNoMT(const Vector2 &t_pixTL, const Vector2 &t_pixBR, const Vector2 &t_fracTL, const Vector2 &t_fracBR, const int t_iterations);
	void createFractalMT(const Vector2 &t_pixTL, const Vector2 &t_pixBR, const Vector2 &t_fracTL, const Vector2 &t_fracBR, const int t_iterations);
	void threadPoolInit();
};

#endif // !APPLICATION_H