#ifndef FIREVFX_PARTICLE_H
#define FIREVFX_PARTICLE_H

#include <iostream>
#include <random>
#include <algorithm>
#include <vector>

class Particle
{
public:
    float m_x;
    float m_y;
    float m_life;
    float m_maxlife;

    int m_dir;
    int m_sin;
    int m_sinr;
    int m_r;

    int m_ox;
    int m_oy;

    Particle(float x, float y, float slider0_val);

    bool operator==(const Particle& other) const
    {
        bool comparison =
                m_x == other.m_x &&
                m_y == other.m_y &&
                m_life == other.m_life &&
                m_maxlife == other.m_maxlife &&
                m_dir == other.m_dir &&
                m_sin == other.m_sin &&
                m_sinr == other.m_sinr &&
                m_r == other.m_r &&
                m_ox == other.m_ox &&
                m_oy == other.m_oy;

        return comparison;

    }


};


#endif //FIREVFX_PARTICLE_H
