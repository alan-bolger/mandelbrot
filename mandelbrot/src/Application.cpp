#include "Application.h"

std::atomic<int> Globals::WORKER_COMPLETE = 0;

/// <summary>
/// Application constructor.
/// </summary>
Application::Application() : m_window{ sf::VideoMode{ Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT, 32 }, "Mandelbrot", sf::Style::Default }
{
	// Load app font
	m_font.loadFromMemory(Globals::DEFAULT_FONT, (size_t)75864 * sizeof(uint8_t));

	// Set render texture size
	m_renderTexture.create(Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT);

	// Align memory
	m_fractal = (int*)_aligned_malloc(size_t(Globals::SCREEN_WIDTH) * size_t(Globals::SCREEN_HEIGHT) * sizeof(int), 64);

	// Initialise the thread pool
	threadPoolInit();
}

/// <summary>
/// Application destructor.
/// </summary>
Application::~Application()
{
	// Stop worker threads
	for (int i = 0; i < Globals::MAX_THREADS; i++)
	{
		m_workers[i].m_alive = false;
		m_workers[i].m_cvStart.notify_one();
	}

	// Clean up worker threads
	for (int i = 0; i < Globals::MAX_THREADS; i++)
	{
		m_workers[i].m_thread.join();
	}		

	// Clean up memory
	_aligned_free(m_fractal);
}

/// <summary>
/// Run.
/// </summary>
void Application::run()
{
	sf::Clock f_clock;
	sf::Time f_timeSinceLastUpdate = sf::Time::Zero;
	sf::Time f_timePerFrame = sf::seconds(1.f / 30.f);

	while (m_window.isOpen() && !m_exitGame)
	{
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
	Vector2 f_pixBR = { Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT };
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
	createFractal(f_pixTL, f_pixBR, f_fracTL, f_fracBR, m_iterations);

	// Stop timing
	auto f_stop = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsedTime = f_stop - f_start;

	// Render result to screen
	for (int y = 0; y < Globals::SCREEN_HEIGHT; y++)
	{
		for (int x = 0; x < Globals::SCREEN_WIDTH; x++)
		{
			int i = m_fractal[y * Globals::SCREEN_WIDTH + x];
			float n = (float)i;
			float a = 0.1f;

			// Credit to @Eriksonn for this - it converts the fractal into a colour
			// RBG values are normalised between 0 and 1 so I've multiplied them by 255
			// so they work properly with SFML ~ AB
			m_pixelGrid.setPixel(x, y, sf::Color(
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
	sf::Sprite f_pixelGrid(m_pixelGrid.getPixelBuffer());
	m_renderTexture.draw(f_pixelGrid);

	drawText(); // This gets drawn to the render texture

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
void Application::drawText()
{
	drawString(10, Globals::SCREEN_HEIGHT - 50, "TIME TAKEN: " + std::to_string(m_elapsedTime.count()) + "s", sf::Color::White);
	drawString(10, Globals::SCREEN_HEIGHT - 30, "ITERATIONS: " + std::to_string(m_iterations), sf::Color::White);
	drawString(Globals::SCREEN_WIDTH - 136, Globals::SCREEN_HEIGHT - 30, "MANDELBROT", sf::Color::White);
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
/// Create fractal using multi-threading (thread pooling).
/// </summary>
/// <param name="t_pixTL">Pixel top left coordinate.</param>
/// <param name="t_pixBR">Pixel top right coordinate.</param>
/// <param name="t_fracTL">Fractal top left coordinate.</param>
/// <param name="t_fracBR">Fractal top right coordinate.</param>
/// <param name="t_iterations">The number of iterations.</param>
void Application::createFractal(const Vector2 &t_pixTL, const Vector2 &t_pixBR, const Vector2 &t_fracTL, const Vector2 &t_fracBR, const int t_iterations)
{
	int f_sectionWidth = (t_pixBR.x - t_pixTL.x) / Globals::MAX_THREADS;
	double f_fractalWidth = (t_fracBR.x - t_fracTL.x) / double(Globals::MAX_THREADS);

	Globals::WORKER_COMPLETE = 0;

	for (size_t i = 0; i < Globals::MAX_THREADS; i++)
	{
		m_workers[i].start(
			Vector2(t_pixTL.x + f_sectionWidth * i, t_pixTL.y),
			Vector2(t_pixTL.x + f_sectionWidth * (i + 1), t_pixBR.y),
			Vector2(t_fracTL.x + f_fractalWidth * double(i), t_fracTL.y),
			Vector2(t_fracTL.x + f_fractalWidth * double(i + 1), t_fracBR.y),
			t_iterations);
	}

	// Wait for all workers to complete
	while (Globals::WORKER_COMPLETE < Globals::MAX_THREADS)
	{
		// Blip, bloop, bleep!
	}
}

/// <summary>
/// Initialise the thread pool.
/// </summary>
void Application::threadPoolInit()
{
	for (int i = 0; i < Globals::MAX_THREADS; i++)
	{
		m_workers[i].m_alive = true;
		m_workers[i].m_fractal = m_fractal;
		m_workers[i].m_screenWidth = Globals::SCREEN_WIDTH;
		m_workers[i].m_thread = std::thread(&WorkerThread::createFractal, &m_workers[i]);
	}
}