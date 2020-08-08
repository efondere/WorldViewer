#include <glad/gl.h> // To make sure it is included before anything else
#include "World.hpp"
#include <fstream>
#include <iostream>
#include <cmath>

#define MAP_SCALE_TILE_SIZE 2.5f
#define MAP_Y_SCALE_FACTOR  1.f

World::World()
: Mesh("greyShader", "basicLighting", "./res/textures/height_texture_01.fw.png")
, m_width(0)
, m_height(0)
{
}

World::~World()
{
}

void World::loadFromFile(const std::string& filePath)
{
    std::ifstream inputFile(filePath, std::ios::in | std::ios::binary);
    if (!inputFile.is_open())
    {
        std::cout << "Could not open world file!" << std::endl;
        return;
    }

    inputFile.read((char*)&m_width, sizeof(unsigned int));
    inputFile.read((char*)&m_height, sizeof(unsigned int));

    if (m_width == 0 || m_height == 0)
    {
        m_width = 0;
        std::cout << "Invalid map: Width or height is set to zero." << std::endl;
        inputFile.close();
        return;
    }
    
    m_vertices.reserve(m_width * m_height);
    uint8_t currentHeight = 0;

    for (unsigned int i = 0; i < m_height; i++)
    {
        for (unsigned int j = 0; j < m_width; j++)
        {
            inputFile.read((char*)&currentHeight, sizeof(uint8_t));
            m_vertices.emplace_back(
                j * MAP_SCALE_TILE_SIZE + ((m_width - 1) * -(MAP_SCALE_TILE_SIZE / 2)),
                static_cast<float>(currentHeight) * MAP_Y_SCALE_FACTOR,
                i * MAP_SCALE_TILE_SIZE + ((m_height -1) * -(MAP_SCALE_TILE_SIZE / 2)),
                0.f, 1.f, 0.f);
        }
    }

    for (unsigned int i = 0; i < m_height; i++)
    {
        for (unsigned int j = 0; j < m_width; j++)
        {
            if (i == 0 || j == 0 || i == m_height - 1 || j == m_width - 1)
                continue; // already set at (0, 1, 0)

            glm::vec3 pos   = m_vertices[i       * m_width +       j].position;
            glm::vec3 south = m_vertices[(i + 1) * m_width +       j].position - pos;
            glm::vec3 east  = m_vertices[i       * m_width + (j + 1)].position - pos;
            glm::vec3 north = m_vertices[(i - 1) * m_width +       j].position - pos;
            glm::vec3 west  = m_vertices[i       * m_width + (j - 1)].position - pos;

            m_vertices[i * m_width + j].normal =
                glm::normalize(glm::normalize(glm::cross(south, east)) +
                               glm::normalize(glm::cross(east, north)) +
                               glm::normalize(glm::cross(north, west)) +
                               glm::normalize(glm::cross(west, south)));
        }
    }

    glGenVertexArrays(1, &m_vaoID);
    glBindVertexArray(m_vaoID);

    glGenBuffers(1, &m_vboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Mesh::Vertex), m_vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * 3 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * 3 * sizeof(float), (const void*)(3 * sizeof(float)));

    m_faces.reserve((m_width - 1) * (m_height - 1) * 2 * 3);

    unsigned int current = 0;
    for (unsigned int i = 0; i < m_height - 1; i++)
    {
        for (unsigned int j = 0; j < m_width - 1; j++)
        {
            current = i * m_width + j;
            m_faces.emplace_back(
                Mesh::TriangleIndices(current, current + m_width + 1, current + 1),
                Mesh::TriangleIndices(current, current + m_width, current + m_width + 1)
            );
        }
    }

    //std::cout << "Size of indices: " << indices.size() << std::endl;

    glGenBuffers(1, &m_iboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_faces.size() * sizeof(Mesh::FaceIndices), m_faces.data(), GL_STATIC_DRAW);

    inputFile.close();
}

void World::update(float dt)
{
}

float World::getHeightAt(float x, float z) const
{
    float column = x / MAP_SCALE_TILE_SIZE;
    float row    = z / MAP_SCALE_TILE_SIZE;

    column += m_width / 2.f;
    row += m_height / 2.f;

    if (column < 0.f)
        column = 0.f;
    else if (column > m_width)
        column = m_width;
    
    if (row < 0.f)
        row = 0.f;
    else if (row > m_height)
        row = m_height;

    // we get an error when we try to access the height at (m_width, m_height). Make sure it is not offset by default for some reason

    return m_vertices[static_cast<unsigned int>(std::round(row)) * m_width + static_cast<unsigned int>(std::round(column))].position.y;
}
