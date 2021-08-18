#ifndef APPLICATION_H
#define APPLICATION_H

#include "Vector2.h"
#include "PixelGrid.h"

#include <SFML/Graphics.hpp>
#include <chrono>
#include <complex>
#include <iostream>

constexpr int g_screenWidth = 1280;
constexpr int g_screenHeight = 720;

class Application
{
public:
	int *m_fractal = nullptr;
	int m_mode = 0;
	int m_iterations = 128;

	Application();
	~Application();
	void run();

private:
	sf::RenderWindow m_window;
	sf::RenderTexture m_renderTexture;
	PixelGrid *m_pixelGrid;
	sf::Font m_font;
	bool m_exitGame{ false };
	Vector2 m_offset = { 0.0f, 0.0f };
	Vector2 m_startPan = { 0.0f, 0.0f };
	Vector2 m_scale = { g_screenWidth / 2.0f, g_screenHeight };
	bool m_leftBtnClicked = false;
	bool m_rightBtnClicked = false;
	std::chrono::duration<double> m_elapsedTime;

	void processEvents();
	void update();
	void draw();
	void drawString(int t_x, int t_y, std::string t_string, sf::Color t_colour, int t_size = 20);
	void drawUI();
	void worldToScreen(const Vector2 &t_world, Vector2 &t_screen);
	void screenToWorld(const Vector2 &t_screen, Vector2 &t_world);
	void createFractalBasic(const Vector2 &t_pixTL, const Vector2 &t_pixBR, const Vector2 &t_fracTL, const Vector2 &t_fracBR, const int t_iterations);
};

#endif // !APPLICATION_H
