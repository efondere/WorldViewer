#include <iostream>
#include <fstream>
#include <time.h>
#include <string>
#include <stb_image/stb_image.h>
#include <perlin_noise/PerlinNoise.hpp>

// run this: "C:\Users\16162488\Documents\programmation\cpp\worldGenerator\build\WorldGenerator.exe" 10 10
// or this: "C:\Users\16162488\Documents\programmation\cpp\worldGenerator\build\WorldGenerator.exe" --file ./res/worlds/height_map_01.png

uint8_t generateHeight(unsigned int x, unsigned int y)
{
    // make sure perlin noise generates a number from 0-255
    return (x * y) % 50 % 256;
}

uint8_t perlin(unsigned int x, unsigned int y,
               const unsigned int& columnCount, const unsigned int& rowCount,
               const siv::PerlinNoise& perlinNoiseObj)
{
    return static_cast<uint8_t>(
        perlinNoiseObj.accumulatedOctaveNoise2D_0_1(x,
                                   y, 4)
        * 255);
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cout << "Not enough arguments. Usage:\nWorldGenerator.exe [width] [height]\nor\nWorldGenerator.exe --file [filePath]" << std::endl;
        return -1;
    }

    int width = 0;
    int height = 0;
    int m_BPP = 0;
    uint8_t* imageData = nullptr;

    if (argc > 2 && std::string(argv[1]) == "--file")
    {
        imageData = stbi_load(argv[2], &width, &height, &m_BPP, 1);
    }

    if (!imageData)
    {
        width = std::stoi(argv[1]);
        height = std::stoi(argv[2]);
    }

    std::ofstream outputFile("./res/worlds/newWorld.world", std::ios::binary | std::ios::out);
    if (!outputFile.is_open())
    {
        std::cout << "Could not open file \"./res/worlds/newWorld.world\"" << std::endl;
        return -1;
    }

    unsigned int u_width = width;
    unsigned int u_height = height;

    outputFile.write((char*)&u_width, sizeof(unsigned int));
    outputFile.write((char*)&u_height, sizeof(unsigned int));

    uint8_t vertex_height = 0b10000000;

    const siv::PerlinNoise pn(time(NULL));

    for (unsigned int i = 0; i < u_height; i++)
    {
        for (unsigned int j = 0; j < u_width; j++)
        {
            //vertex_height = generateHeight(j, i);
            if (!imageData)
                vertex_height = perlin(j, i, width, height, pn);
            else
                vertex_height = imageData[i * width + j];
            outputFile.write((char*)&vertex_height, sizeof(uint8_t));
        }
    }

    outputFile.close();

    if (imageData)
        stbi_image_free(imageData);

    return 0;
}


/*#include <cstdlib>
#include <ctime>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <iostream>

// command g++ perlinNoise.cpp -lsfml-system -lsfml-window -lsfml-graphics -o perlinNoise.exe

#define YMAX 5
#define XMAX 10

void generateRandomGradient(float Gradient[YMAX][XMAX][2])
{
    srand(time(NULL));
    for (int i = 0; i < XMAX; i++) {
        for (int j = 0; j < YMAX; j++) {
            Gradient[i][j][0] = static_cast<float>(rand() / static_cast<float>(RAND_MAX)) * ((rand() % 2 == 0) ? 1.f : -1.f);
            Gradient[i][j][1] = sqrt(1 - Gradient[i][j][0]*Gradient[i][j][0]) * ((rand() % 2 == 0) ? 1.f : -1.f);
        }
    }
}

float lerp(float a0, float a1, float w) {
    return (1.f - w) * a0 + w * a1;
}

float dotGridGradient(int ix, int iy, float x, float y, float Gradient[YMAX][XMAX][2]) {
    float dx = x - (float)ix;
    float dy = y - (float)iy;


    return (dx*Gradient[ix][iy][0] + dy*Gradient[ix][iy][1]);
}

float perlin(float x, float y, float Gradient[YMAX][XMAX][2]) {
    int x0 = (int)x;
    int x1 = x0 + 1;
    int y0 = (int)y;
    int y1 = y0 + 1;
    
    float sx = x - (float)x0;
    float sy = y - (float)y0;

    float n0, n1, ix0, ix1, value;

    n0 = dotGridGradient(x0, y0, x, y, Gradient);
    n1 = dotGridGradient(x1, y0, x, y, Gradient);
    ix0 = lerp(n0, n1, sx);

    n0 = dotGridGradient(x0, y1, x, y, Gradient);
    n1 = dotGridGradient(x1, y1, x, y, Gradient);
    ix1 = lerp(n0, n1, sx);

    value = lerp(ix0, ix1, sy);
    return value;
}

float map(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main2(int argc, char* argv[])
{
    float Gradient[YMAX][XMAX][2];
    generateRandomGradient(Gradient);

    //sf::RenderWindow window(sf::VideoMode(1000, 500), "unit vectors");
    //sf::VertexArray vertices(sf::Lines, XMAX * YMAX * 2);
    //std::cout << vertices.getVertexCount() << std::endl;

    //int current = 0;
    //for (int i = 0; i < XMAX; i++) {
    //    for (int j = 0; j < YMAX; j++) {
    //        //(0,0)[0,1], (1,0)[10,11], [20,21], [30,31], [40,41]
    //        //(0,1)[2,3], (1,1)[12,13], [22,23], [32,33], [42,43]
    //        //(0,2)[4,5], (1,2)[14,15], [24,25], [34,35], [44,45]
    //        //(0,3)[6,7], (1,3)[16,17], [26,27], [36,37], [46,47]
    //        //(0,4)[8,9], (1,4)[18,19], [28,29], [38,39], [48,49]
//
    //        vertices[2*(i*YMAX+j)+0].position = sf::Vector2f(50 + 100 * i, 50 + 100 * j);
    //        vertices[2*(i*YMAX+j)+1].position = vertices[2*(i*YMAX+j)+0].position + (sf::Vector2f(Gradient[i][j][0], Gradient[i][j][1]) * 25.f);
    //        vertices[2*(i*YMAX+j)+0].color = sf::Color::White;
    //        vertices[2*(i*YMAX+j)+1].color = sf::Color::White;
    //        std::cout << current << ": (" << i << ", " << j << ") : (" << vertices[2*(i*YMAX+j)+0].position.x << ", " << vertices[2*(i*YMAX+j)+0].position.y << ") -> (" << vertices[2*(i*YMAX+j)+1].position.x << ", " << vertices[2*(i*YMAX+j)+1].position.x << ") [" << 2*(i*YMAX+j)+0 << " and " << 2*(i*YMAX+j)+1 << "] Gradient: [" << Gradient[i][j][0] << ", " << Gradient[i][j][1] << "]" << std::endl;
    //        current++;
    //    }
    //}

    //while (window.isOpen()) {
    //    sf::Event event;
    //    while (window.pollEvent(event)) {
    //        if (event.type == sf::Event::Closed)
    //            window.close();
    //    }
//
    //    window.clear();
    //    window.draw(vertices);
    //    window.display();
    //}

    sf::Image myImage;
    myImage.create(800, 400);

    for (int i = 0; i < 800; i++) {
        for (int j = 0; j < 400; j++) {
            sf::Color pixelColor = sf::Color::White;
            float perlin_num = perlin(map(static_cast<float>(i), 0, 800, 0, XMAX), map(static_cast<float>(j), 0, 400, 0, YMAX), Gradient);
            
            pixelColor.r = pixelColor.g = pixelColor.b = 255 * map(perlin_num, -1.0f, 1.0f, 0.0f, 1.0f);
            myImage.setPixel(i, j, pixelColor);
        }
    }

    myImage.saveToFile("./perlinNoise.png");

    return 0;
}*/
