#include <SFML/Graphics/Image.hpp>
#include <iostream>
#include <sstream>

// usage: decomposeSkybox.exe [textureName] .[extension]

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Please enter the correct number of arguments." << std::endl;
        return -1;
    }

    std::stringstream fileNameStream;
    fileNameStream << "./" << argv[1] << "/" << argv[1];
    std::cout << fileNameStream.str() << std::endl;

    sf::Image image;
    image.loadFromFile(fileNameStream.str() + argv[2]);
    fileNameStream << "-";
    std::cout << fileNameStream.str() << std::endl;
    int textureSize = image.getSize().y / 2;
    sf::Image t_image;
    t_image.create(textureSize, textureSize);
    int imageCoords[12] = {
        2 * textureSize, textureSize, //
        0,               textureSize,
        textureSize,     0, //
        2 * textureSize, 0, //
        textureSize,     textureSize,
        3 * textureSize, textureSize,
    };

    for (int i = 0; i < 6; i++)
    {
        for (int y = 0; y < textureSize; y++)
        {
            for (int x = 0; x < textureSize; x++)
            {
                t_image.setPixel(x, y, image.getPixel(x + imageCoords[2*i], y + imageCoords[2*i + 1]));
            }
        }
        t_image.saveToFile(fileNameStream.str() + std::to_string(i) + argv[2]);
    }

    return 0;
}
