#include "PixelGrid.h"

/// <summary>
/// PixelGrid constructor.
/// </summary>
PixelGrid::PixelGrid(int t_x, int t_y)
{
	m_pixelArray.resize(t_x * t_y * 4);
	m_pixelBuffer.create(t_x, t_y);

	m_arrayWidth = t_x;
	m_arrayHeight = t_y;
	m_arrayTotalSize = m_arrayWidth * m_arrayHeight * 4; // Total elements in array

	clearPixelArray();
}

/// <summary>
/// PixelGrid destructor.
/// </summary>
PixelGrid::~PixelGrid()
{
	// Nothing to delete
}

/// <summary>
/// Set the colour of a pixel at a specified position.
/// Out of bound positions are ignored.
/// </summary>
/// <param name="t_x">The X position of the pixel.</param>
/// <param name="t_y">The Y position of the pixel.</param>
/// <param name="t_colour">The colour to set the pixel to.</param>
void PixelGrid::setPixel(int t_x, int t_y, sf::Color t_colour)
{
	// Check that X and Y values are valid
	if (t_x >= 0 && t_x < m_arrayWidth)
	{
		if (t_y >= 0 && t_y < m_arrayHeight)
		{
			m_pixelArray[(t_y * m_arrayWidth + t_x) * 4] = t_colour.r;
			m_pixelArray[(t_y * m_arrayWidth + t_x) * 4 + 1] = t_colour.g;
			m_pixelArray[(t_y * m_arrayWidth + t_x) * 4 + 2] = t_colour.b;
			m_pixelArray[(t_y * m_arrayWidth + t_x) * 4 + 3] = t_colour.a;
		}
	}
}

/// <summary>
/// Gets the colour of a pixel at a specified position.
/// Out of bound positions result in a transparent pixel being returned.
/// </summary>
/// <param name="t_x">The X position of the pixel.</param>
/// <param name="t_y">The Y position of the pixel.</param>
/// <returns>The colour of the selected pixel.</returns>
sf::Color PixelGrid::getPixel(int t_x, int t_y)
{
	sf::Color f_colour(0, 0, 0, 0);

	// Check that X and Y values are valid
	if (t_x >= 0 && t_x < m_arrayWidth)
	{
		if (t_y >= 0 && t_y < m_arrayHeight)
		{
			f_colour.r = m_pixelArray[(t_y * m_arrayWidth + t_x) * 4];
			f_colour.g = m_pixelArray[(t_y * m_arrayWidth + t_x) * 4 + 1];
			f_colour.b = m_pixelArray[(t_y * m_arrayWidth + t_x) * 4 + 2];
			f_colour.a = m_pixelArray[(t_y * m_arrayWidth + t_x) * 4 + 3];
		}
	}

	return f_colour;
}

/// <summary>
/// This function returns the pixel buffer/texture;
/// </summary>
/// <returns>A reference to a texture with the contents of the pixel array copied to it.</returns>
sf::Texture &PixelGrid::getPixelBuffer()
{
	m_pixelBuffer.update(&m_pixelArray.front());

	return m_pixelBuffer;
}

/// <summary>
/// Clears the pixel array to 0 values.
/// </summary>
void PixelGrid::clearPixelArray()
{
	for (int i = 0; i < m_arrayTotalSize; ++i)
	{
		m_pixelArray[i] = 0;
	}
}
