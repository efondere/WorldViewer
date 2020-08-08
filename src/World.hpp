#ifndef WORLD_HPP
#define WORLD_HPP

#include "Mesh.hpp"

class World : public Mesh
{
public:

    World();

    ~World();

    virtual void loadFromFile(const std::string& filePath) override;

    virtual void update(float dt) override;

    float getHeightAt(float x, float z) const;

private:

    unsigned int m_width;
    unsigned int m_height;
};

#endif //WORLD_HPP
