#include <iostream>
#include <cmath>
#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "../dynddssub/ScanDynamicSub.h"

#include "visualization.h"

using eprosima::fastdds::dds::Log;


int main()
{

    int unconnectedcnt = 1000;
    int radius = 1, lightsignal = 3, lightsignal2 = 3, filtermodus = 0;    //variables for the cartesian plot
    sf::Color colors[5];
    getColors(colors);
    CustomColors customColors;
    bool initilized1 = false, initilized2 = false;

    /* DDS Subscriber with vector for current and old data  */
    //DDS Subscriber1
    ScanDynamicSub *mysub;
    mysub = new ScanDynamicSub();
    std::string mytopic = "Scanner1Topic";
	std::vector<std::any> anyArray(3);

    // Scan data in polar coordinates
    std::uint16_t cscan[362]={0};

    //x offset and theta offset
    double x_offset = 0.0;
    float theta_offset = 135.0;

    // current xys and istnulls
    std::vector<double> xs, ys;
    std::vector<bool> istnull;

    // old xys and old istnulls 
    std::vector<double> oldxs, oldys;
    std::vector<bool> oldistnull;

    initilized1 = mysub->init("pub1.xml", "SensorData", "Scanner1Topic");

    // Initialize vectors to size 360(current) and 720(old)
    initializeData(xs, ys, istnull, 360);
    initializeData(oldxs, oldys, oldistnull, 720);


    //DDS Subscriber2
    ScanDynamicSub *mysub2;
    mysub2 = new ScanDynamicSub();
    std::string mytopic2 = "Scanner2Topic";
	std::vector<std::any> anyArray2(3);

    // Scan data in polar coordinates
    std::uint16_t cscan2[362]={0};

    //x offset and theta offset
    double x_offset2 = 750;
    float theta_offset2 = 45;    

    // current xys and istnulls
    std::vector<double> xs2, ys2;
    std::vector<bool> istnull2;

    // old xys and old istnulls 
    std::vector<double> oldxs2, oldys2;
    std::vector<bool> oldistnull2;

    initilized2 = mysub2->init("pub2.xml", "SensorData2", "Scanner2Topic");

    // Initialize vectors to size 360(current) and 720(old)
    initializeData(xs2, ys2, istnull2, 360);
    initializeData(oldxs2, oldys2, oldistnull2, 720);

    if (initilized1)
    {
		printf("init success\n");
		mysub->run(anyArray.data(), anyArray.size(), mytopic);
		printf("run success\n");
    }
    if (initilized2)
    {
		printf("init2 success\n");
		mysub2->run(anyArray2.data(), anyArray2.size(), mytopic2);
		printf("run2 success\n");
    }

    /*Window/Video mode*/
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktopMode, "Plot Window");
    sf::View plotView(sf::FloatRect(-(BREITEGRENZE * radius), -(HOEHENGRENZE * radius), (2*BREITEGRENZE * radius), 2*(HOEHENGRENZE * radius)));
    window.setView(plotView);
    //window.setFramerateLimit(60); //24 is enough for human eye, but 30-60 is smoother

    /*Background, Robot, Sensor, Font*/
    // Create a grid
    sf::VertexArray grid(sf::Lines, 0);
    getGrid(&grid);
    //TH Logo
    sf::Sprite logo;
    sf::Texture logoTexture;
    SpriteTexture logoStruct = {logo, logoTexture, "../visuSub/images/TH_Koeln_Logo2.png", 2, (BREITEGRENZE -900), (HOEHENGRENZE -600)};
    getImage(logoStruct);
    //Sensor Image
    sf::Sprite lidarImage;
    sf::Texture lidarTexture;
    SpriteTexture lidarStruct = {lidarImage, lidarTexture, "../visuSub/images/rplidar-a1m8-135.png", 0.5, -125, -125};
    getImage(lidarStruct);
    //Robot Image
    sf::Sprite roboImage;
    sf::Texture roboTexture;
    SpriteTexture roboStruct = {roboImage, roboTexture, "../visuSub/images/sidearm_nomotor.png", 2, 120, -710};
    getImage(roboStruct);
    //Table and poles
    sf::RectangleShape poles[5];
    getPoles(poles, radius);
    //Font for labels
    sf::Font font;
    if (!font.loadFromFile("../visuSub/images/OpenSans.ttf"))
    {
        std::cout << "Error loading font" << std::endl;
        return 1;
    }


    // sf::VertexArray blindspot(sf::LinesStrip, 0);
    // getBlindSpot(&blindspot);

    // Create a convex shape to represent a triangle
    sf::ConvexShape triangle;
    
    // Set the initial position, color, and vertices for the triangle
    sf::Vector2f point1(750, 0);
    sf::Vector2f point2(-2056, 1620);
    sf::Vector2f point3(3556, 1620);

    createTriangle(triangle, sf::Color(230, 230, 230,120), sf::Color(0, 0, 255, 120), point1, point2, point3);

    // std::vector<sf::Sprite> light;
    // std::vector<sf::Texture> lightTexture;
    // getLights(light, lightTexture, radius);

    sf::CircleShape billoLight[4];
    getbilloLight(billoLight, radius);

    sf::CircleShape circles[4];
    sf::CircleShape circles2[4];
    float cradius[4] = {1500.f, 1000.0f, 500.0f, 100.0f};
    getCircles(circles, colors, cradius, x_offset);
    getCircles(circles2, colors, cradius, x_offset2);

    sf::Text labels[3]; 
    getLables(labels, cradius, font);



    sf::CircleShape lidarPoints[360];
    sf::CircleShape oldlidarPoints[1080];
    sf::CircleShape allLidarPoints[360];

    sf::CircleShape lidarPoints2[360];
    sf::CircleShape oldlidarPoints2[1080];
    sf::CircleShape allLidarPoints2[360];
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                printf("Closing window\n");
                window.close();
            }
        }

        handleKeyboardInput(&filtermodus);

        if (anyArray[1].type() == typeid(uint16_t*)){
            uint16_t* buffer_cscan = std::any_cast<std::uint16_t*>(anyArray[1]);
            for (int i = 0; i < 362; i++){
                cscan[i] = buffer_cscan[i];
            }
        } 
        else{
            //printf("wrong type1\n");
        }

        //if(cscan[360] == 2 && cscan[361] == 2){
        if(mysub->m_listener.n_samples == 0){    
            if(unconnectedcnt > 1000){
                lightsignal = 3;
            } 
            else{
                unconnectedcnt++;
            }   
        }
        else{
             // delete first 360 entrys of oldxs and oldys and add new values from xs and ys to the end of the vectors if new values are available
            oldxs.erase(oldxs.begin(), oldxs.begin() + 360);
            oldys.erase(oldys.begin(), oldys.begin() + 360);
            oldistnull.erase(oldistnull.begin(), oldistnull.begin() + 360);
            oldxs.insert(oldxs.end(), xs.begin(), xs.end());
            oldys.insert(oldys.end(), ys.begin(), ys.end());
            oldistnull.insert(oldistnull.end(), istnull.begin(), istnull.end());

            // set indicator for new values to false
            mysub->m_listener.n_samples = 0;

            unconnectedcnt = 0;
        }

        xs.clear();
        ys.clear();
        istnull.clear();

        filterpolar2cartesian(cscan, xs, ys, istnull, &lightsignal, cradius, x_offset, theta_offset);
        getLidarPoints(lidarPoints, radius, xs, ys, istnull, sf::Color::Red);
        getOldPoints(oldlidarPoints, radius, oldxs, oldys, oldistnull,sf::Color(255,0,100,20), sf::Color(255,0,100,50));
        getAllPoints(allLidarPoints, xs, ys, oldxs, oldys, radius, sf::Color::Red);


//sud2
        if (anyArray2[1].type() == typeid(uint16_t*)){
            uint16_t* buffer_cscan2 = std::any_cast<std::uint16_t*>(anyArray2[1]);
            for (int i = 0; i < 120; i++){//TODO: 362
                cscan2[i] = buffer_cscan2[i];
            }
        } 
        else{
            //printf("wrong type2\n");
        }

        //if(cscan[360] == 2 && cscan[361] == 2){
        if(mysub2->m_listener.n_samples == 0){    
            if(unconnectedcnt > 1000){
                lightsignal2 = 3; //TODO: lightsignal2
            } 
            else{
                unconnectedcnt++;
            }   
        }
        else{
             // delete first 360 entrys of oldxs and oldys and add new values from xs and ys to the end of the vectors if new values are available
            oldxs2.erase(oldxs2.begin(), oldxs2.begin() + 360);
            oldys2.erase(oldys2.begin(), oldys2.begin() + 360);
            oldistnull2.erase(oldistnull2.begin(), oldistnull2.begin() + 360);
            oldxs2.insert(oldxs2.end(), xs2.begin(), xs2.end());
            oldys2.insert(oldys2.end(), ys2.begin(), ys2.end());
            oldistnull2.insert(oldistnull2.end(), istnull2.begin(), istnull2.end());

            // set indicator for new values to false
            mysub2->m_listener.n_samples = 0;

            unconnectedcnt = 0;
        }

        xs2.clear();
        ys2.clear();
        istnull2.clear();

        filterpolar2cartesian(cscan2, xs2, ys2, istnull2, &lightsignal2, cradius, x_offset2, theta_offset2);
        getLidarPoints(lidarPoints2, radius, xs2, ys2, istnull2, sf::Color::Blue);
        getOldPoints(oldlidarPoints2, radius, oldxs2, oldys2, oldistnull2,sf::Color(0,100,255,20), sf::Color(0,100,255,50));
        getAllPoints(allLidarPoints2, xs2, ys2, oldxs2, oldys2, radius, sf::Color::Blue);

        if(lightsignal == 2 || lightsignal2 == 2){
            lightsignal = 2;//red
        }
        else if(lightsignal == 0 && lightsignal2 == 0){
            lightsignal = 0;//green
        }
        else if(lightsignal == 3 && lightsignal2 == 3){
            lightsignal = 3;//grey
        }
        else{
            lightsignal = 1;//yellow
        }

        plotValues(window, circles, circles2, labels, lidarPoints, lidarPoints2, grid, poles,logo, lidarImage, roboImage, billoLight[lightsignal]
                    , oldlidarPoints, allLidarPoints, oldlidarPoints2, allLidarPoints2,filtermodus, triangle);
        
    }

    delete mysub;
    delete mysub2;
    Log::Reset();
    return 0;
}
