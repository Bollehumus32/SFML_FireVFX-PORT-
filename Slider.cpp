#include "Slider.h"
#include <SFML/Graphics.hpp>

Slider::Slider(float x, float y, float w, float h, float min, float max) :
        m_x(x), m_y(y), m_w(w), m_h(h), m_min(min), m_max(max)
{
    m_val = m_min;

    m_valx = x;
}

void Slider::set_val(float val)
{
    m_val = val;
    m_valx = m_x + m_w*(m_val/(m_max-m_min));

    m_init_val = m_val;
    m_init_val_x = m_valx;
}

void Slider::update(float mx, float my, bool mp, bool ms)
{
    auto rect = sf::RectangleShape({m_w, m_h});

    rect.setPosition({m_x, m_y});

    if(mp && ms)
    {
        if(rect.getGlobalBounds().contains(mx, my))
        {
            m_val = ((mx - m_x)/m_w)*(m_max-m_min)+m_min;
            m_valx = mx;
        }
    }
}

void Slider::reset()
{
    m_val = m_init_val;
    m_valx = m_init_val_x;
}
