#ifndef FIREVFX_SLIDER_H
#define FIREVFX_SLIDER_H


class Slider
{
public:
    float m_x;
    float m_y;
    float m_w;
    float m_h;
    float m_min;
    float m_max;

    float m_val;
    float m_valx;

    float m_init_val;
    float m_init_val_x;

    Slider(float x, float y, float w, float h, float min, float max);

    void set_val(float val);

    void update(float mx, float my, bool mp, bool ms);

    void reset();
};


#endif //FIREVFX_SLIDER_H
