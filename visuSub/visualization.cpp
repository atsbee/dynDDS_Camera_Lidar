#include "visualization.h"


void getColors(sf::Color *colors){
    colors[0] = sf::Color(240, 240, 240,120); //light gray
    colors[1] = sf::Color(225, 225, 225,120); //lighter gray
    colors[2] = sf::Color(205, 205, 205,120); //gray
    colors[3] = sf::Color(185, 185, 185,120); //darker gray
    colors[4] = sf::Color(170, 170, 170,120); //dark gray
}

void getGrid(sf::VertexArray *grid){
    sf::Color gridcolor = sf::Color(165, 165, 165,120); //dark gray
    // Add vertical lines
    for (int x = 0; x <= BREITEGRENZE; x += 100) {
        grid->append(sf::Vertex(sf::Vector2f(x, -HOEHENGRENZE), gridcolor));
        grid->append(sf::Vertex(sf::Vector2f(x, HOEHENGRENZE), gridcolor));
        grid->append(sf::Vertex(sf::Vector2f(-x, -HOEHENGRENZE), gridcolor));
        grid->append(sf::Vertex(sf::Vector2f(-x, HOEHENGRENZE), gridcolor));        
    }
    // Add horizontal lines
    for (int y = 0; y <= HOEHENGRENZE; y += 100) {
        grid->append(sf::Vertex(sf::Vector2f(-BREITEGRENZE, y), gridcolor));
        grid->append(sf::Vertex(sf::Vector2f(BREITEGRENZE, y), gridcolor));
        grid->append(sf::Vertex(sf::Vector2f(-BREITEGRENZE, -y), gridcolor));
        grid->append(sf::Vertex(sf::Vector2f(BREITEGRENZE, -y), gridcolor));
    }
    printf("Grid set\n");
}

void getBlindSpot(sf::VertexArray *blindSpot){
    sf::Color blindSpotColor = sf::Color(40, 120, 200, 150); //blue
    // Add vertical lines
    blindSpot->append(sf::Vertex(sf::Vector2f(-285, -HOEHENGRENZE), blindSpotColor));
    blindSpot->append(sf::Vertex(sf::Vector2f(0, 0), blindSpotColor));
    blindSpot->append(sf::Vertex(sf::Vector2f(BREITEGRENZE , 508), blindSpotColor));
}

void getTriangle(sf::ConvexShape& convexShape, const sf::Color& color1, const sf::Color& color2, 
                    const sf::Vector2f& point1, const sf::Vector2f& point2, const sf::Vector2f& point3)
{
    convexShape.setPointCount(3);  // Set the number of vertices to 3 (for a triangle)

    // Set the positions of the vertices
    convexShape.setPoint(0, point1);
    convexShape.setPoint(1, point2);
    convexShape.setPoint(2, point3);

    convexShape.setFillColor(color1);
    convexShape.setOutlineThickness(5.f);
    convexShape.setOutlineColor(color2);
}

void initializeData(std::vector<double>& xs, std::vector<double>& ys, std::vector<bool>& istnull, int size) {
    // Initialize xs, ys, and istnull
    for (int i = 0; i < size; i++) {
        xs.push_back(0);
        ys.push_back(0);
        istnull.push_back(true);
    }
}


void handleKeyboardInput(int *filtermodus){
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
    {
        *filtermodus = 1; //just new points
        printf("1 pressed\n");
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
    {
        *filtermodus = 2; //all points median filter
        printf("2 pressed\n");
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
    {
        *filtermodus = 3; //old and new glow
        printf("3 pressed\n");
    }
}

void getPoles(sf::RectangleShape *poles, const int radius){
    for(int i = 0; i < 4; i++){
        poles[i].setSize(sf::Vector2f(45*radius, 45*radius));
        poles[i].setFillColor(sf::Color(90, 90, 90, 255)); //greyish
        poles[i].setOutlineColor(sf::Color::Black); //black 
        poles[i].setOutlineThickness(3);
        poles[i].setOrigin(0,0);
    }
    poles[0].setPosition(50, -95);
    poles[1].setPosition(50, -685);
    poles[2].setPosition(740, -95);
    poles[3].setPosition(740, -685);
    //pole 4 ist der untergrund
    poles[4].setSize(sf::Vector2f(755*radius, 665*radius));
    poles[4].setFillColor(sf::Color(200, 200, 200, 255)); //greyish
    poles[4].setOutlineColor(sf::Color(90, 90, 90, 255)); //dark greyish
    poles[4].setOutlineThickness(3);
    poles[4].setOrigin(0,0);
    poles[4].setPosition(40, -695);
}

void getImage(SpriteTexture spriteStruct){
    // Load the image from a file
    if (!spriteStruct.texture.loadFromFile(spriteStruct.path)) {
        // Failed to load the image
        printf("Failed to load image: %s\n", spriteStruct.path.c_str());
    }
    spriteStruct.sprite.setTexture(spriteStruct.texture);
    spriteStruct.sprite.setScale(spriteStruct.scale, spriteStruct.scale);
    spriteStruct.sprite.setPosition(spriteStruct.posX, spriteStruct.posY);
    printf("Image set\n");
}

void getLights(std::vector<sf::Sprite> &light, std::vector<sf::Texture> &lightTexture, const int radius){
    // Load the image from a file
    sf::Texture texture;
    if (!texture.loadFromFile("../images/gruen.png")) {
        // Failed to load the image
        printf("Failed to load image\n");
    }
    lightTexture.push_back(texture);
    if (!texture.loadFromFile("../images/gelb.png")) {
        // Failed to load the image
        printf("Failed to load image\n");
    }
    lightTexture.push_back(texture);
    if (!texture.loadFromFile("../images/rot.png")) {
        // Failed to load the image
        printf("Failed to load image\n");
    }
    lightTexture.push_back(texture);
    if (!texture.loadFromFile("../images/aus.png")) {
        // Failed to load the image
        printf("Failed to load image\n");
    }
    lightTexture.push_back(texture);

    // Create a light and set the lightTexture
    for(int i = 0; i < 4; i++){
        light.push_back(sf::Sprite());
        light[i].setTexture(lightTexture[i]);
        light[i].setScale(4 * radius, 4 * radius);
        // Position the light in the lower right corner
        light[i].setPosition((BREITEGRENZE -500) *radius, (-HOEHENGRENZE + 200) *radius);
        //light[i].setPosition((BREITEGRENZE -600) *radius,-1000);
    }
}

void getdotLight(sf::CircleShape *dotLight, const int radius){
    for(int i = 0; i < 4; i++){
        dotLight[i].setRadius(90*radius);
        dotLight[i].setOrigin(90*radius,90*radius);
        dotLight[i].setPosition((BREITEGRENZE -500) *radius, (-HOEHENGRENZE + 200) *radius);
    }
    dotLight[0].setFillColor(sf::Color::Green);
    dotLight[1].setFillColor(sf::Color::Yellow);
    dotLight[2].setFillColor(sf::Color::Red);
    dotLight[3].setFillColor(sf::Color::Black);
}

void getCircles(sf::CircleShape *circles, sf::Color *colors, float *cradius, float y_offset){
    sf::Color red = sf::Color(255, 0, 0, 120); //red
    sf::Color yellow = sf::Color(255, 255, 0, 120); //yellow
    for(int i = 0; i < 4; i++){
        circles[i].setRadius(cradius[i]);
        circles[i].setFillColor(colors[i]);
        circles[i].setOrigin(cradius[i], cradius[i]);
        circles[i].setPosition(0.f, y_offset);
    }
    circles[0].setOutlineThickness(5.0f);
    circles[0].setOutlineColor(colors[0]);
    circles[1].setOutlineThickness(5.0f);
    circles[1].setOutlineColor(yellow);
    circles[2].setOutlineThickness(5.0f);
    circles[2].setOutlineColor(red);
    circles[3].setOutlineThickness(5.0f);
    circles[3].setOutlineColor(colors[3]);    
}

void getLables(sf::Text *labels, float *cradius, const sf::Font& font){
    for(int i = 0; i < 3; i++){//was 4
        labels[i].setString(std::to_string((int)(cradius[i]/10)) + "cm");
        labels[i].setFont(font);
        //labels[i].setRotation(-10);
        labels[i].setCharacterSize(60);
        labels[i].setFillColor(sf::Color::Black);
        labels[i].setPosition(-110, -cradius[i]);
    }
}

void getLidarPoints(sf::CircleShape *lidarPoints, int radius, const std::vector<double>& xs, const std::vector<double>& ys, 
            const std::vector<bool>& istnull, sf::Color color){
    
    for(int i = 0; i < 360; i++){
        //set point properties 
        lidarPoints[i].setRadius(10);
        lidarPoints[i].setPosition(xs[i], ys[i]);
        lidarPoints[i].setOrigin(10, 10);
        //if point is null or out of range, set color to transparent point and connecting line
        if(istnull[i] == true || std::abs(xs[i]) > BREITEGRENZE * radius || std::abs(ys[i]) > HOEHENGRENZE * radius){
            //lidarPoints[i].setFillColor(sf::Color::Transparent);
        }
        else{
            lidarPoints[i].setFillColor(color);

        }
    }

}   

double median(double a, double b, double c) {
    double arr[] = {a, b, c};
    std::sort(arr, arr + 3);
    return arr[1];
}

void getAllPoints(sf::CircleShape *allLidarPoints, std::vector<double>& xs, std::vector<double>& ys, const std::vector<double>& oldxs, 
            const std::vector<double>& oldys, int radius, sf::Color color){
    double allxs[360], allys[360];            
    for(int i = 0; i < 360; i++){
        allxs[i] = median(xs[i], oldxs[i], oldxs[i+360]);
        allys[i] = median(ys[i], oldys[i], oldys[i+360]);
    }
    
    for(int i = 0; i < 360; i++){
        //set point properties 
        allLidarPoints[i].setRadius(10);
        allLidarPoints[i].setPosition(allxs[i], allys[i]);
        allLidarPoints[i].setOrigin(10, 10);
        //if point is null or out of range, set color to transparent point and connecting line
        if((allxs[i]==0 && allys[i]==0) || std::abs(oldxs[i]) > BREITEGRENZE * radius || std::abs(oldys[i]) > HOEHENGRENZE * radius){
            //allLidarPoints[i].setFillColor(sf::Color::Transparent);
        }
        else{
            allLidarPoints[i].setFillColor(color);
        }
    }
}

void getOldPoints(sf::CircleShape *oldlidarPoints, int radius, std::vector<double>& oldxs, std::vector<double>& oldys, std::vector<bool>& oldistnull, sf::Color color1, sf::Color color2){
    for(int i = 0; i < 360; i++){
        //set point properties 
        oldlidarPoints[i].setRadius(10);
        oldlidarPoints[i].setPosition(oldxs[i], oldys[i]);
        oldlidarPoints[i].setOrigin(10, 10);
        //if point is null or out of range, set color to transparent point and connecting line
        if(oldistnull[i] == true || std::abs(oldxs[i]) > BREITEGRENZE * radius || std::abs(oldys[i]) > HOEHENGRENZE * radius){
            //oldlidarPoints[i].setFillColor(sf::Color::Transparent);
        }
        else{
            oldlidarPoints[i].setFillColor(color1);
        }
    }
    for(int i = 360; i < 720; i++){
        //set point properties 
        oldlidarPoints[i].setRadius(10);
        oldlidarPoints[i].setPosition(oldxs[i], oldys[i]);
        oldlidarPoints[i].setOrigin(10, 10);
        //if point is null or out of range, set color to transparent point and connecting line
        if(oldistnull[i] == true || std::abs(oldxs[i]) > BREITEGRENZE * radius || std::abs(oldys[i]) > HOEHENGRENZE * radius){
           //oldlidarPoints[i].setFillColor(sf::Color::Transparent);
        }
        else{
            oldlidarPoints[i].setFillColor(color2);
        }
    }
}


void filterpolar2cartesian(std::uint16_t *cscan, std::vector<double>& xs, std::vector<double>& ys, std::vector<bool>& istnull, int *lightsignal, 
                            float *cradius, double y_offset, int theta_offset)
{
    double x, y, rho, theta;
    int gelb = 0, rot = 0;//counter for consecutive rho values in the yellow and red range

    //last scan was red
    if(*lightsignal == 2){ 
        *lightsignal = 1;
    }
    else {
        *lightsignal = 0;
    }

    for (int radian = 0; radian < 360; ++radian)
    {    
        theta = (((359-radian) + theta_offset) % 360)* M_PI / 180.0; //wrong way around and 45° offset +90°
        //if both neighbouring points are 0 and the current point is bigger than 0 -> set rho to 0
        //or if the current point is in the blindspot between 0°(0) and 270°(4.712389) -> set rho to 0     //gegen den uhrzeigersinn 100° und 350°
        //if((cscan[(radian+359) % 360] == 0 && cscan[(radian+1) % 360] == 0 && cscan[radian] > 0) || (theta < 0 || theta > 4.712389)){ 
        if((cscan[(radian+359) % 360] == 0 && cscan[(radian+1) % 360] == 0 && cscan[radian] > 0) ){ 
            rho = 0;
        }
        else{
            rho = static_cast<double>(cscan[radian]);
        }

        //if 5consecutive rho values are bigger than 0 and smaller than big radius -> gelb++ or if 5consecutive rho values are smaller than small radius -> rot++
        if(rho > 0 && rho < cradius[1] && theta > 0 && theta < 4.712389){ //blank out the 90° where the robot is
            gelb++;
        }
        if(rho > 0 && rho < cradius[2] && theta > 0 && theta < 4.712389){
            rot++;
        }
        if (rho > cradius[1]){
            gelb = 0;
        }
        if(rho > cradius[2]){
            rot = 0;
        }
        //set lightsignal if 10 consecutive rho values are in the yellow or 15 in the red range
        if(gelb == 10 && *lightsignal != 2){
            *lightsignal = 1;
        }
        if(rot == 15){
            *lightsignal = 2;
        }
        //polar to cartesian
        x = rho * std::cos(theta);
        y = rho * std::sin(theta);
        xs.push_back(x);
        ys.push_back(y+y_offset);
        //if rho is 0 -> set istnull to true to check for null values later (easier)
        if(rho == 0){
            istnull.push_back(true);
        }
        else{
            istnull.push_back(false);
        }
    }
}



// void plotValues(sf::RenderWindow& window, sf::CircleShape *circles, sf::Text *labels, sf::CircleShape *lidarPoints, sf::VertexArray path, 
//             const sf::Sprite& sprite, const sf::Sprite& light, sf::VertexArray grid, sf::Sprite& lidarImage, sf::VertexArray blindspot, sf::Sprite& info)
void plotValues(sf::RenderWindow& window, sf::CircleShape *circles, sf::CircleShape *circles2, sf::Text *labels, sf::CircleShape *lidarPoints, sf::CircleShape *lidarPoints2, sf::VertexArray grid, sf::RectangleShape *poles, sf::Sprite& logo, sf::Sprite& lidarImage, 
            sf::Sprite& roboImage, sf::CircleShape &dotLight, sf::CircleShape *oldPoints, sf::CircleShape *allLidarPoints, sf::CircleShape *oldPoints2, sf::CircleShape *allLidarPoints2, int filtermodus, sf::ConvexShape &triangle)
{
    window.clear(sf::Color::White);
    window.draw(grid);
    window.draw(logo);
    
    //window.draw(light);
    window.draw(dotLight);

    window.draw(triangle);
    for(int i = 0; i < 4; i++){
        window.draw(circles[i]);
        //window.draw(circles2[i]);
    }
    window.draw(poles[4]);
    for(int i = 0; i < 4; i++){
        window.draw(poles[i]);
    }
    for(int i = 0; i < 3; i++){//was 4
        window.draw(labels[i]);
    }
    switch (filtermodus)
    {
    case 1:
        for(int i = 0; i < 360; i++){
            window.draw(lidarPoints[i]);
        }
        break;
    case 2:
        for(int i = 0; i < 360; i++){
            window.draw(allLidarPoints[i]);
        }   
        break;
    case 3:
        for(int i = 0; i < 360; i++){
            window.draw(lidarPoints[i]);
        }        
        for(int i = 0; i < 720; i++){
            window.draw(oldPoints[i]);
        }
        break;
    
    default:    
        for(int i = 0; i < 360; i++){
            window.draw(lidarPoints[i]);
        }
        break;
    }

//sub2
    switch (filtermodus)
    {
    case 1:
        for(int i = 0; i < 360; i++){
            window.draw(lidarPoints2[i]);
        }
        break;
    case 2:
        for(int i = 0; i < 360; i++){
            window.draw(allLidarPoints2[i]);
        }   
        break;
    case 3:
        for(int i = 0; i < 360; i++){
            window.draw(lidarPoints2[i]);
        }        
        for(int i = 0; i < 720; i++){
            window.draw(oldPoints2[i]);
        }
        break;
    
    default:    
        for(int i = 0; i < 360; i++){
            window.draw(lidarPoints2[i]);
        }
        break;
    }


    //window.draw(path);
    
    //window.draw(blindspot);
    window.draw(lidarImage);
    window.draw(roboImage);
    window.display();
}
