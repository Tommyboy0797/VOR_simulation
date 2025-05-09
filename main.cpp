#include <iostream> 
#include <vector> 
#include <string>
#include <algorithm>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <corecrt_math_defines.h>
using namespace std;

int AIRCRAFT_HEADING = 0; // default to north so the labels position correctly
float AIRCRAFT_LAT = 51.4700;
float AIRCRAFT_LNG = -0.4543;

float VOR_LAT = 49.0097;
float VOR_LNG = 2.5479;

float get_radians(float degrees){ // convert degrees to radians

    float radians = (degrees * 3.14159) / 180; // multiply degrees by pi, and divide by 180

    return radians;
}
float get_degrees(float radians){
    
    float degrees = radians * (180 / 3.14159);

    return degrees;
}

int get_bearing(float lat1, float lng1, float lat2, float lng2, int hdg){

    lat1 = get_radians(lat1); // convert lat and lngs to radians 
    lat2 = get_radians(lat2);
    lng1 = get_radians(lng1);
    lng2 = get_radians(lng2);

    float lng_diff = lng2 - lng1; // get difference between longitudes must do 2 - 1

    float result = atan2(sin(lng_diff) * cos(lat2), cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(lng_diff));

    result = get_degrees(result);

    if (result < 0) result += 360.0f; // rationalise the result, if its a minus, add 360 to make it positive

    return round(result); // round result to a whole number
}

int get_turn_angle(float bearing, float heading){

    float turn_amount = bearing - heading;

    if (turn_amount > 180){ // rationalise the result
        turn_amount -= 360;
    } else if (turn_amount < -180) {
        turn_amount += 360;
    }

    return round(turn_amount); // round result
}

int WinMain(){

    sf::RenderWindow window(sf::VideoMode({800,600}), "VOR Simulator"); // create main window

    const sf::Font font("ui_resources/arial.ttf");

    sf::Text text(font, "VOR", 24);
    text.setFillColor(sf::Color::Black);
    sf::FloatRect bounds = text.getLocalBounds();        // returns sf::Rect<float> :contentReference[oaicite:1]{index=1}
    float originX = bounds.position.x + bounds.size.x  / 2.f;  // position.x is old ‘left’, size.x is old ‘width’
    float originY = 0.f;                                       // keep it flush with the top
    text.setOrigin({ originX, originY });
    float windowMidX = window.getSize().x / 2.f;
    float topMargin  = 10.f;  
    text.setPosition({ windowMidX, topMargin });

    sf::Text current_heading(font, "HDG: " + to_string(AIRCRAFT_HEADING) + "\260", 20); // use ascii value for the degrees symbol
    current_heading.setFillColor(sf::Color::Black);
    current_heading.setPosition(sf::Vector2f(5.f, 20.f)); // Vector2f == "vector to float"

    sf::Text reqd_bearing(font, "BRNG: " + to_string(get_bearing(AIRCRAFT_LAT, AIRCRAFT_LNG, VOR_LAT, VOR_LNG, AIRCRAFT_HEADING)) + "\260", 20); // use ascii value for the degrees symbol
    reqd_bearing.setFillColor(sf::Color::Black);
    reqd_bearing.setPosition(sf::Vector2f(5.f, 40.f));

    sf::Text turn_angle(font, "Turn: " + std::to_string(get_turn_angle(get_bearing(AIRCRAFT_LAT, AIRCRAFT_LNG, VOR_LAT, VOR_LNG, AIRCRAFT_HEADING), AIRCRAFT_HEADING)) + "\260", 20); // use ascii value for the degrees symbol
    turn_angle.setFillColor(sf::Color::Black);
    turn_angle.setPosition(sf::Vector2f(5.f, 60.f));

    sf::RectangleShape hdg_left_btn(sf::Vector2f(20,20));
    hdg_left_btn.setPosition(sf::Vector2f(110.f, 23.f));
    hdg_left_btn.setFillColor(sf::Color::Black);
    sf::Text hdg_left_text(font, "<", 21);
    hdg_left_text.setFillColor(sf::Color::White);
    hdg_left_text.setPosition(sf::Vector2f(113.f, 20.f));

    sf::RectangleShape hdg_right_btn(sf::Vector2f(20,20));
    hdg_right_btn.setPosition(sf::Vector2f(135.f, 23.f));
    hdg_right_btn.setFillColor(sf::Color::Black);
    sf::Text hdg_right_text(font, ">", 21);
    hdg_right_text.setFillColor(sf::Color::White);
    hdg_right_text.setPosition(sf::Vector2f(138.f, 20.f));

    sf::CircleShape compass(250.f);
    compass.setFillColor(sf::Color::Transparent);
    compass.setOutlineThickness(5.f);
    compass.setOutlineColor(sf::Color::Black);
    float compass_radius = 250.f;
    compass.setOrigin({compass_radius, compass_radius});
    compass.setPosition(sf::Vector2f(400.f, 325.f));

    sf::CircleShape heading_arrow(20.f, 3);
    heading_arrow.setFillColor(sf::Color::Black);
    heading_arrow.setPosition(sf::Vector2f(378.f, 95.f));

    sf::RenderTexture compass_texture({800,800});
    compass_texture.clear(sf::Color::Transparent);
    compass_texture.draw(compass);

    sf::Vector2f center(400.f, 325.f);
    float radius = 250.f;
    const int numMarks = 36;   // one every 10 degrees
    const std::size_t vertexCount = numMarks * 2;

    sf::VertexArray compassMarks(sf::PrimitiveType::Lines, vertexCount);

    for (int i = 0; i < numMarks; ++i) { // add the ticks onto the compass, better than doing it by hand!
        float angle = i * 360.f / numMarks * (3.14159265f / 180.f);
        sf::Vector2f p1 = center + sf::Vector2f(std::cos(angle)*(radius-10), std::sin(angle)*(radius-10));
        sf::Vector2f p2 = center + sf::Vector2f(std::cos(angle)*radius, std::sin(angle)*radius);
        compassMarks[i*2 + 0].position = p1;
        compassMarks[i*2 + 1].position = p2;
        compassMarks[i*2 + 0].color = sf::Color::Black;
        compassMarks[i*2 + 1].color = sf::Color::Black;
    }
    compass_texture.draw(compassMarks);
 
    std::vector<sf::Text> cardinalLabels;
    cardinalLabels.reserve(4);

    const std::vector<std::pair<float, std::string>> cardinals = {{
        {   0.f, "N" },
        {  90.f, "E" },
        { 180.f, "S" },
        { 270.f, "W" }
    }};

    for (auto& [deg, labelStr] : cardinals)
    {
        sf::Text label(font, labelStr, 18);
        label.setFillColor(sf::Color::Black);

        sf::FloatRect lb = label.getLocalBounds();
        label.setOrigin({
            lb.position.x + lb.size.x / 2.f,
            lb.position.y + lb.size.y / 2.f
        });

        float rad = get_radians(deg);
        float dist = radius;
        label.setPosition({
            center.x + std::cos(rad) * dist,
            center.y + std::sin(rad) * dist
        });
        label.setOrigin({compass_radius, compass_radius});
        cardinalLabels.push_back(label);
        compass_texture.draw(label);
    }

    compass_texture.draw(compassMarks);
    
    compass_texture.display();
    sf::Sprite compassSprite(compass_texture.getTexture());
    compassSprite.setOrigin({compass_radius, compass_radius});
    compassSprite.setPosition(sf::Vector2f(800 / 2.f, 600 / 2.f));
    
    while (window.isOpen()){
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()){
                window.close();
            }
            
            if (AIRCRAFT_HEADING >= 360){ // if the heading is 360 or above, reset to 0 to avoid going too high like 380 degrees
                AIRCRAFT_HEADING = 0;
            }

            if (AIRCRAFT_HEADING <= 0){ 
                AIRCRAFT_HEADING = 360;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){ // if the user presses the left arrow

                AIRCRAFT_HEADING = AIRCRAFT_HEADING - 1; // decrease the heading

                current_heading.setString("HDG: " + to_string(AIRCRAFT_HEADING) + "\260"); // update current heading value.
                turn_angle.setString("Turn: " + std::to_string(get_turn_angle(get_bearing(AIRCRAFT_LAT, AIRCRAFT_LNG, VOR_LAT, VOR_LNG, AIRCRAFT_HEADING), AIRCRAFT_HEADING)) + "\260"); // update the turn direction
                hdg_left_text.setFillColor(sf::Color::Red); // when the button is pressed, set the arrow icon thing to red, to show that its being used
                compassSprite.setRotation(sf::degrees(AIRCRAFT_HEADING));


            } else {
                hdg_left_text.setFillColor(sf::Color::White); // reset to white when not pressed
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)){ // if the user presses the left arrow

                if (AIRCRAFT_HEADING == 360){ // if the heading is 360 or above, reset to 0 to avoid going too high like 380 degrees
                    AIRCRAFT_HEADING = 0;
                }
                AIRCRAFT_HEADING = AIRCRAFT_HEADING + 1; // increase the heading

                current_heading.setString("HDG: " + to_string(AIRCRAFT_HEADING) + "\260"); // update current heading value.
                turn_angle.setString("Turn: " + std::to_string(get_turn_angle(get_bearing(AIRCRAFT_LAT, AIRCRAFT_LNG, VOR_LAT, VOR_LNG, AIRCRAFT_HEADING), AIRCRAFT_HEADING)) + "\260"); // update the turn direction
                hdg_right_text.setFillColor(sf::Color::Red); // when the button is pressed, set the arrow icon thing to red, to show that its being used
                compassSprite.setRotation(sf::degrees(AIRCRAFT_HEADING));
            } else {
                hdg_right_text.setFillColor(sf::Color::White); // reset to white when not pressed
            }

        }

        window.clear(sf::Color::White); // set bg colour to white

        window.draw(text);
        window.draw(current_heading);
        window.draw(reqd_bearing);
        window.draw(turn_angle);
        window.draw(hdg_left_btn);
        window.draw(hdg_left_text);
        window.draw(hdg_right_btn);
        window.draw(hdg_right_text);
        window.draw(heading_arrow);
        window.draw(compassSprite);


        window.display(); 
    }

    return 0;
}
