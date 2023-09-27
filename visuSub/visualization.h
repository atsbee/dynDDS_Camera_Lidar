//header for visualization.cpp

#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <iostream>
#include <cmath>
#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "../dynddssub/ScanDynamicSub.h"

#define BREITEGRENZE 2880
#define HOEHENGRENZE 1620

struct CustomColors {
    sf::Color lightGray = sf::Color(240, 240, 240, 120);
    sf::Color lighterGray = sf::Color(225, 225, 225, 120);
    sf::Color gray = sf::Color(205, 205, 205, 120);
    sf::Color darkerGray = sf::Color(185, 185, 185, 120);
    sf::Color darkGray = sf::Color(170, 170, 170, 120);
    sf::Color gridColor = sf::Color(165, 165, 165,120);
    sf::Color blindSpotColor = sf::Color(40, 120, 200, 150);
    sf::Color polesColor = sf::Color(90, 90, 90, 255);
    sf::Color polesOutlineColor = sf::Color::Black;
    sf::Color tableColor = sf::Color(200, 200, 200, 255);
    sf::Color tableOutlineColor = sf::Color(90, 90, 90, 255);
    sf::Color lightRed = sf::Color(255, 0, 0, 120); //Circle
    sf::Color lightYellow = sf::Color(255, 255, 0, 120); //Circle
};

struct SpriteTexture {
    sf::Sprite &sprite; 
    sf::Texture &texture;
    std::string path;
    float scale;
    float posX;
    float posY;
};

void getImage(SpriteTexture spriteStruct);

void getColors(sf::Color *colors);

void getGrid(sf::VertexArray *grid);

void getBlindSpot(sf::VertexArray *blindSpot);

void handleKeyboardInput(int *filtermodus);

void initializeData(std::vector<double>& xs, std::vector<double>& ys, std::vector<bool>& istnull, int size);

void createTriangle(sf::ConvexShape& convexShape, const sf::Color& color1, const sf::Color& color2, 
                    const sf::Vector2f& point1, const sf::Vector2f& point2, const sf::Vector2f& point3);

void getPoles(sf::RectangleShape *poles, const int radius);

void getTHLogo(sf::Sprite& logo, sf::Texture &logoTexture, const int radius);

void getLights(std::vector<sf::Sprite> &light, std::vector<sf::Texture> &lightTexture, const int radius);

void getbilloLight(sf::CircleShape *billoLight, const int radius);

void getLidarImage(sf::Sprite& lidar, sf::Texture &lidarTexture, const int radius);

void getRoboImage(sf::Sprite& robo, sf::Texture &roboTexture, const int radius);

void getCircles(sf::CircleShape *circles, sf::Color *colors, float *cradius, float x_offset);

void getLables(sf::Text *labels, float *cradius, const sf::Font& font);

void getLidarPoints(sf::CircleShape *lidarPoints, int radius, const std::vector<double>& xs, const std::vector<double>& ys, 
            const std::vector<bool>& istnull, sf::Color color);

double median(double a, double b, double c);

void getAllPoints(sf::CircleShape *allLidarPoints, std::vector<double>& xs, std::vector<double>& ys, const std::vector<double>& oldxs, 
                        const std::vector<double>& oldys, int radius, sf::Color color);

void getOldPoints(sf::CircleShape *oldlidarPoints, int radius, std::vector<double>& oldxs, std::vector<double>& oldys, std::vector<bool>& oldistnull, sf::Color color1, sf::Color color2);

//außer Betrieb
void polar2cartesian(std::uint16_t *cscan, std::vector<double>& xs, std::vector<double>& ys, std::vector<bool>& istnull, int *lightsignal, float *cradius);

void filterpolar2cartesian(std::uint16_t *cscan, std::vector<double>& xs, std::vector<double>& ys, std::vector<bool>& istnull, int *lightsignal, 
                            float *cradius, double x_offset, int theta_offset);

void plotValues(sf::RenderWindow& window, sf::CircleShape *circles, sf::CircleShape *circles2, sf::Text *labels, sf::CircleShape *lidarPoints, sf::CircleShape *lidarPoints2, sf::VertexArray grid, 
                        sf::RectangleShape *poles, sf::Sprite& logo, sf::Sprite& lidarImage, sf::Sprite& roboImage, sf::CircleShape &billoLight, 
                        sf::CircleShape *oldPoints, sf::CircleShape *allLidarPoints, sf::CircleShape *oldPoints2, sf::CircleShape *allLidarPoints2, int filtermodus, sf::ConvexShape &triangle);


#endif // VISUALIZATION_H