#include "Application.h"

/// <summary>
/// Application constructor.
/// </summary>
Application::Application() : m_window{ sf::VideoMode{ g_screenWidth, g_screenHeight, 32 }, "Mandelbrot Fractals", sf::Style::Default }
{
	// Load app font
	m_font.loadFromFile("assets/fonts/default.ttf");

	// Set render texture size
	m_renderTexture.create(g_screenWidth, g_screenHeight);

	// Align memory
	m_fractal = (int*)_aligned_malloc(size_t(g_screenWidth) * size_t(g_screenHeight) * sizeof(int), 64);

	// Create pixel grid
	m_pixelGrid = new PixelGrid(g_screenWidth, g_screenHeight);
}

/// <summary>
/// Application destructor.
/// </summary>
Application::~Application()
{
	delete[] m_pixelGrid;
}

/// <summary>
/// Run.
/// </summary>
void Application::run()
{
	sf::Clock f_clock;
	sf::Time f_timeSinceLastUpdate = sf::Time::Zero;
	sf::Time f_timePerFrame = sf::seconds(1.f / 60.f);

	while (m_window.isOpen() && !m_exitGame)
	{
		processEvents();
		f_timeSinceLastUpdate += f_clock.restart();

		while (f_timeSinceLastUpdate > f_timePerFrame)
		{
			f_timeSinceLastUpdate -= f_timePerFrame;
			processEvents();
			update();
			draw();
		}
	}
}

/// <summary>
/// Process events.
/// </summary>
void Application::processEvents()
{
	sf::Event f_event;

	while (m_window.pollEvent(f_event))
	{
		if (sf::Event::Closed == f_event.type)
		{
			m_window.close();
		}

		if (sf::Event::KeyPressed == f_event.type)
		{
			if (sf::Keyboard::Escape == f_event.key.code)
			{
				m_exitGame = true;
			}
		}
	}
}

/// <summary>
/// Update.
/// </summary>
void Application::update()
{
	// Get mouse location this frame
	Vector2 f_mouse = sf::Vector2f(sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y);

	// Start panning routine once the right mouse button has been clicked
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !m_rightBtnClicked)
	{
		m_startPan = f_mouse;
		m_rightBtnClicked = true;
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && m_rightBtnClicked)
	{
		m_offset -= (f_mouse - m_startPan) / m_scale;
		m_startPan = f_mouse;
	}
	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Right) && m_rightBtnClicked)
	{
		m_rightBtnClicked = false;
	}

	Vector2 f_mouseBeforeZoom;
	screenToWorld(f_mouse, f_mouseBeforeZoom);

	// Use Q and A to scale up or down
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		m_scale *= 1.1f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_scale *= 0.9f;
	}	

	Vector2 f_mouseAfterZoom;
	screenToWorld(f_mouse, f_mouseAfterZoom);
	m_offset += (f_mouseBeforeZoom - f_mouseAfterZoom);

	Vector2 f_pixTL = { 0, 0 };
	Vector2 f_pixBR = { g_screenWidth, g_screenHeight };
	Vector2 f_fracTL = { -2.0, -1.0 };
	Vector2 f_fracBR = { 1.0, 1.0 };

	screenToWorld(f_pixTL, f_fracTL);
	screenToWorld(f_pixBR, f_fracBR);

	// Adjust iteration amount
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		m_iterations += 64;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		m_iterations -= 64;
	}

	if (m_iterations < 64)
	{
		m_iterations = 64;
	}

	// Start timing
	auto f_start = std::chrono::high_resolution_clock::now();

	// Do the computation
	switch (m_mode)
	{
		case 0: 
		{
			createFractalBasic(f_pixTL, f_pixBR, f_fracTL, f_fracBR, m_iterations);
			break;
		} 
	}

	// Stop timing
	auto f_stop = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsedTime = f_stop - f_start;

	// Render result to screen
	for (int y = 0; y < g_screenHeight; y++)
	{
		for (int x = 0; x < g_screenWidth; x++)
		{
			int i = m_fractal[y * g_screenWidth + x];
			float n = (float)i;
			float a = 0.1f;

			// Credit to @Eriksonn for this - it converts the fractal into a colour
			m_pixelGrid->setPixel(x, y, sf::Color(
				(0.5f * sin(a * n) + 0.5f) * 255.0f,			// R
				(0.5f * sin(a * n + 2.094f) + 0.5f) * 255.0f,	// G
				(0.5f * sin(a * n + 4.188f) + 0.5f) * 255.0f)); // B
		}
	}

	m_elapsedTime = elapsedTime;
}

/// <summary>
/// Draw.
/// </summary>
void Application::draw()
{
	m_renderTexture.display();

	m_window.clear();

	// Draw pixel grid to render texture
	sf::Sprite f_pixelGrid(m_pixelGrid->getPixelBuffer());
	m_renderTexture.draw(f_pixelGrid);

	drawUI(); // This gets drawn the the render texture

	// Display the render texture
	sf::Sprite f_finishedRender(m_renderTexture.getTexture());
	m_window.draw(f_finishedRender);

	m_window.display();	

	m_renderTexture.clear();
}

/// <summary>
/// Draws a string to a given position.
/// </summary>
/// <param name="t_x">The X position.</param>
/// <param name="t_y">The Y position.</param>
/// <param name="t_string">The string to draw.</param>
/// <param name="t_colour">The colour of the string.</param>
/// <param name="t_size">The character size (default value of 20).</param>
void Application::drawString(int t_x, int t_y, std::string t_string, sf::Color t_colour, int t_size)
{
	sf::Text f_text;

	f_text.setCharacterSize(t_size);
	f_text.setFont(m_font);
	f_text.setString(t_string);
	f_text.setPosition(t_x, t_y);
	f_text.setFillColor(t_colour);

	m_renderTexture.draw(f_text);
}

/// <summary>
/// Draw the information text.
/// </summary>
void Application::drawUI()
{
	switch (m_mode)
	{
		case 0: 
		{
			drawString(10, g_screenHeight - 70, "1. BASIC METHOD", sf::Color::White);
			break;
		}
	}

	drawString(10, g_screenHeight - 50, "TIME TAKEN: " + std::to_string(m_elapsedTime.count()) + "s", sf::Color::White);
	drawString(10, g_screenHeight - 30, "ITERATIONS: " + std::to_string(m_iterations), sf::Color::White);
}

/// <summary>
/// Converts world coordinates to screen coordinates.
/// </summary>
/// <param name="t_world">World coordinates.</param>
/// <param name="t_screen">Screen coordinates.</param>
void Application::worldToScreen(const Vector2 &t_world, Vector2 &t_screen)
{
	t_screen.x = (int)((t_world.x - m_offset.x) * m_scale.x);
	t_screen.y = (int)((t_world.y - m_offset.y) * m_scale.y);
}

/// <summary>
/// Converts screen coordinates to world coordinates.
/// </summary>
/// <param name="t_screen">Screen coordinates.</param>
/// <param name="t_world">World coordinates.</param>
void Application::screenToWorld(const Vector2 &t_screen, Vector2 &t_world)
{
	t_world.x = (double)(t_screen.x) / m_scale.x + m_offset.x;
	t_world.y = (double)(t_screen.y) / m_scale.y + m_offset.y;
}

/// <summary>
/// Create fractal using a basic and unoptimised algorithm.
/// </summary>
/// <param name="t_pixTL">Pixel top left coordinate.</param>
/// <param name="t_pixBR">Pixel top right coordinate.</param>
/// <param name="t_fracTL">Fractal top left coordinate.</param>
/// <param name="t_fracBR">Fractal top right coordinate.</param>
/// <param name="t_iterations">The number of iterations.</param>
void Application::createFractalBasic(const Vector2 &t_pixTL, const Vector2 &t_pixBR, const Vector2 &t_fracTL, const Vector2 &t_fracBR, const int t_iterations)
{
	double x_scale = (t_fracBR.x - t_fracTL.x) / (double(t_pixBR.x) - double(t_pixTL.x));
	double y_scale = (t_fracBR.y - t_fracTL.y) / (double(t_pixBR.y) - double(t_pixTL.y));

	for (int y = t_pixTL.y; y < t_pixBR.y; y++)
	{
		for (int x = t_pixTL.x; x < t_pixBR.x; x++)
		{
			std::complex<double> f_c(x * x_scale + t_fracTL.x, y * y_scale + t_fracTL.y);
			std::complex<double> f_z(0, 0);

			int f_n = 0;

			while (std::abs(f_z) < 2.0 && f_n < t_iterations)
			{
				f_z = (f_z * f_z) + f_c;
				f_n++;
			}

			m_fractal[y * g_screenWidth + x] = f_n;
		}
	}
}

