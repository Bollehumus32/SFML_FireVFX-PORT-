#include "Particle.h"
#include "util.h"

Particle::Particle(float x, float y, float slider0_val)
{
    m_x = x;
    m_y = y;

    m_maxlife = random_integer(13 + (int)(slider0_val * 5), 27 + (int)(slider0_val * 10));
    m_life = m_maxlife;

    m_dir = random_choice(std::vector{-2, -1, 1, 2});
    m_sin = random_integer(0, 2);
    m_sinr = random_integer(5, 10);
    m_r = random_integer(5, 10);

    m_ox = random_integer(-1, 1);
    m_oy = random_integer(-1, 1);
}