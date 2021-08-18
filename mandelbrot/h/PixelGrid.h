#ifndef PIXELBUFFER_H
#define PIXELBUFFER_H

#include <SFML/Graphics.hpp>
#include <cstdint>
#include <vector>

class PixelGrid
{
public:
	PixelGrid(int t_x, int t_y);
	~PixelGrid();
	void setPixel(int t_x, int t_y, sf::Color t_colour);
	sf::Color getPixel(int t_x, int t_y);
	sf::Texture &getPixelBuffer();

private:
	uint8_t *m_pixelArray;
	sf::Texture m_pixelBuffer;
	int m_arrayWidth;
	int m_arrayHeight;
	int m_arrayTotalSize;

	void clearPixelArray();
};

#endif // !PIXELBUFFER_H
