#include <iostream>
#include <SFML/Graphics.hpp>
#include "Particle.h"
#include "util.h"
#include "Slider.h"
#include <iomanip>
#include <sstream>

#define FONT_SIZE 16

int WIN_WIDTH = 1280;
int WIN_HEIGHT = 720;

sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Procedurally Generated Fire");

sf::Clock Clock;

std::vector<sf::Color> palette =
{
        sf::Color(61, 38, 48),
        sf::Color(115, 61, 56),
        sf::Color(191, 74, 46),
        sf::Color(247, 117, 33),
        sf::Color(255, 173, 51),
        sf::Color(255, 255, 0)
};

int res = 1;

float j = 0;
float ox = sf::Mouse::getPosition(window).x;
float oy = sf::Mouse::getPosition(window).y;
bool fs = false;
int fa = 255;
bool ms = false;

int main()
{

    sf::RenderTexture uiSurface;
    if(!uiSurface.create(180, WIN_HEIGHT))
    {
        std::cerr << "Couldn't create UI surface ..." << std::endl;
        return -1;
    }

    sf::Font font;
    if(!font.loadFromFile("assets/Retro Gaming.ttf"))
    {
        std::cerr << "Couldn't load font from file ..." << std::endl;
        return -1;
    }

    auto rect_Reset_Button = sf::RectangleShape({120, 40});
    rect_Reset_Button.setPosition(10, WIN_HEIGHT - rect_Reset_Button.getSize().y -10);
    rect_Reset_Button.setOutlineColor(sf::Color(11, 120, 184));
    rect_Reset_Button.setFillColor(sf::Color(44, 36, 66));
    rect_Reset_Button.setOutlineThickness(4);
    sf::Text resetButtonText("Reset Sliders", font, FONT_SIZE);
    resetButtonText.setPosition(16, WIN_HEIGHT - rect_Reset_Button.getSize().y+4);

    sf::Text paletteText("PALETTE: ", font, FONT_SIZE);
    paletteText.setPosition(10, 76);
    paletteText.setFillColor(sf::Color::White);

    std::vector<Particle> particles;
    std::vector<Particle> dead;

    std::vector<sf::Text> texts(7, sf::Text("", font, FONT_SIZE));

    std::vector<Slider> sliders;

    float g_frameCount = 0;
    float elapsedTime = 0;
    int fps = 0;

    // Size
    sliders.emplace_back(11, 106+21, 150, 10, 0.25, 15);

    // Density
    sliders.emplace_back(11, 150+21, 150, 10, 1, 10);

    // Rise
    sliders.emplace_back(11, 194+21, 150, 10, -4, 10);

    // Spread
    sliders.emplace_back(11, 238+21, 150, 10, 0, 4);

    // Wind
    sliders.emplace_back(11, 282+21, 150, 10, -4, 4);

    sliders[0].set_val(2.9);
    sliders[1].set_val(3);
    sliders[2].set_val(1.25);
    sliders[3].set_val(1.0);
    sliders[4].set_val(0);

    int iter_ = 0;
    for(auto &text : texts)
    {
        text.setPosition(sliders[iter_].m_x, sliders[iter_].m_y);
        text.setFillColor(sf::Color::White);
        iter_++;
    }

    bool mp;

    window.setFramerateLimit(64);

    while(window.isOpen())
    {
        j++;
        if(j > 360)
        {
            j = 0;
        }

        float mx = sf::Mouse::getPosition(window).x;
        float my = sf::Mouse::getPosition(window).y;

        float dist = sqrt(pow((mx - ox), 2) + pow((my - oy), 2));

        sf::Event e;
        while(window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
            {
                window.close();
            }

            if(e.type == sf::Event::MouseButtonPressed)
            {
                if(e.mouseButton.button == sf::Mouse::Left)
                {
                    if(rect_Reset_Button.getGlobalBounds().contains(mx, my))
                    {
                        for(auto &slider : sliders)
                        {
                            slider.reset();
                        }
                    }

                    if(mx < 180)
                        ms = true;
                    else
                        ms = false;
                }
            }
        }


    // Uncomment this for debugging:
    //    mp = true;
    // -----------------------------

        mp = sf::Mouse::isButtonPressed(sf::Mouse::Left);

        if (!ms)
        {
            if (mp) fs = true;

            double a = atan2(my - oy, mx - ox);

            for (int d = 0; d < dist; d += 10)
            {
                float _x = mx + cos(a) * d;
                float _y = my + sin(a) * d;
                if (mp)
                {
                    for (int i = 0; i < round(sliders[1].m_val); ++i)
                    {
                        particles.push_back({_x / res, _y / res, sliders[0].m_val});
                    }
                }
            }

            if (mp)
            {
                for (int i = 0; i < round(sliders[1].m_val); ++i)
                {
                    particles.push_back({mx / res, my / res, sliders[0].m_val});
                }
            }
        }
        window.clear(sf::Color(24, 20, 36));

        if(fs)
        {
            fa = -5;
            if(fa < 0)
            {
                fs = false;
            }
        }

        if(mp && !ms)
        {
            auto a = sf::CircleShape(2);
            a.setPointCount(10);
            a.setPosition({mx/res, my/res-2});
            a.setFillColor(palette[5]);
            window.draw(a);
        }

        for(size_t s = 0; s < particles.size(); ++s)
        {
            auto *p = &particles[s];
            p->m_life -= 1;
            if(p->m_life == 0)
            {
                dead.push_back(*p);
                continue;
            }

            int i = static_cast<int>((p->m_life * 6) / p->m_maxlife);

            size_t index = i % palette.size();

            p->m_y -= sliders[2].m_val * 4;
            p->m_x += ((p->m_sin * sin(j/(p->m_sinr)))/2*4)*sliders[3].m_val + sliders[4].m_val;

            if(!random_integer(0, 5))
            {
                p->m_r += 8.8;
            }

            int x = p->m_x;
            int y = p->m_y;

            x += p->m_ox*(5-index);
            y += p->m_oy*(5-index);

            int alpha = 255;

            if(p->m_life < p->m_maxlife/4)
            {
                alpha = (int)(p->m_life/p->m_maxlife)*255;
            }

            sf::CircleShape a = sf::CircleShape(p->m_r);
            a.setPointCount(10);
            a.setPosition(x, y);
            a.setFillColor(sf::Color(palette[index].r, palette[index].g, palette[index].b, alpha));

            if(index == 0)
            {
                sf::CircleShape b = sf::CircleShape(p->m_r);
                a.setPosition(x + random_integer(-1, 1), y - 4);
                a.setFillColor({0, 0, 0, 0});
                a.setRadius(p->m_r*(((p->m_maxlife - p->m_life)/p->m_maxlife)/0.88));
                window.draw(a);
            }

            else
            {
                a.setFillColor(sf::Color(palette[index-1].r, palette[index-1].g, palette[index-1].b, alpha));
                a.setPosition(x + random_integer(-1, 1), y - 3);
                a.setRadius(p->m_r/1.5);
                window.draw(a);
            }
        }


        for(auto &p : dead)
        {
            particles.erase(std::remove(particles.begin(), particles.end(), p), particles.end());
        }

        dead.clear();

        g_frameCount++;
        elapsedTime += Clock.restart().asSeconds();

        uiSurface.clear(sf::Color(10, 10, 10, 85));

        if (elapsedTime >= 1.0f)
        {
            fps = (int)g_frameCount;  // Store the frame count once per second
            texts[0].setString("FPS: " + std::to_string(fps));  // Update the FPS text
            elapsedTime = 0.0f;
            g_frameCount = 0;
        }

        texts[0].setString("FPS: " + std::to_string(fps));
        texts[1].setString("PARTICLES: " + std::to_string(particles.size()));

        sf::RectangleShape a = sf::RectangleShape({170, 1});
        a.setPosition(5, 60);
        a.setFillColor(sf::Color(251, 250, 252));

        window.draw(a);

        std::stringstream stream;

        stream << std::fixed << std::setprecision(2) << sliders[0].m_val;
        texts[2].setString("SIZE: " + stream.str());
        stream.str("");

        stream << std::fixed << std::setprecision(2) << sliders[1].m_val;
        texts[3].setString("DENSITY: " + stream.str());
        stream.str("");

        stream << std::fixed << std::setprecision(2) << sliders[2].m_val;
        texts[4].setString("RISE: " + stream.str());
        stream.str("");

        stream << std::fixed << std::setprecision(2) << sliders[3].m_val;
        texts[5].setString("SPREAD: " + stream.str());
        stream.str("");

        stream << std::fixed << std::setprecision(2) << sliders[4].m_val;
        texts[6].setString("WIND: " + stream.str());
        stream.str("");

        for(int i = 0; i < 2; ++i)
        {
            texts[i].setPosition(5, 20*(i+1));
            texts[i].setFillColor(sf::Color::White);
            uiSurface.draw(texts[i]);
        }

        for(int i = 2; i < texts.size(); ++i)
        {
            // Problem was here...
            // Didn't subtract from sliders indices, so it tried to
            // access the vector out of bounds...
            texts[i].setPosition(sliders[i-2].m_x, sliders[i-2].m_y-20);
            texts[i].setFillColor(sf::Color::White);
            uiSurface.draw(texts[i]);
        }

        sf::CircleShape slider_circles[3];
        for(auto& slider : sliders)
        {
            slider.update(mx, my, mp, ms);
            auto rect1 = sf::RectangleShape({slider.m_w, slider.m_h});
            for(int i = 0; i < 3; ++i)
            {
                slider_circles[i] = sf::CircleShape(slider.m_h / 2);
            }

            rect1.setPosition(slider.m_x + slider.m_h/2, slider.m_y);

            slider_circles[0].setPosition(slider.m_x+slider.m_h/2-5, slider.m_y);
            slider_circles[0].setFillColor(sf::Color::White);
            slider_circles[1].setPosition(slider.m_x+slider.m_w - slider.m_h/2+4, slider.m_y);
            slider_circles[1].setFillColor(sf::Color::White);
            slider_circles[2].setPosition(slider.m_valx, slider.m_y);
            slider_circles[2].setFillColor(sf::Color(160, 200, 168));

            uiSurface.draw(rect1);
            for(auto &circle : slider_circles)
            {
                uiSurface.draw(circle);
            }
        }

        auto PaletteRect = sf::RectangleShape({62, 18});
        PaletteRect.setPosition(105, 76);
        PaletteRect.setFillColor(sf::Color(251, 250, 252));
        uiSurface.draw(PaletteRect);

        for(int i = 0; i < palette.size(); ++i)
        {
            auto colorRect = sf::RectangleShape({10, 16});
            colorRect.setPosition(106+(i*10), 77);
            colorRect.setFillColor(sf::Color(palette[i]));
            uiSurface.draw(colorRect);
        }

        uiSurface.draw(rect_Reset_Button);
        uiSurface.draw(resetButtonText);
        uiSurface.draw(paletteText);


        uiSurface.display();
        sf::Sprite uiSprite(uiSurface.getTexture());
        window.draw(uiSprite);

        // Update display:
        window.display();

        ox = mx;
        oy = my;
    }


    return 0;
}
