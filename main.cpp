#include <iostream> 
#include <vector> 
#include <string>
#include <algorithm>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace std;

int AIRCRAFT_HEADING = 180;
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

float get_bearing(float lat1, float lng1, float lat2, float lng2, int hdg){

    lat1 = get_radians(lat1); // convert lat and lngs to radians 
    lat2 = get_radians(lat2);
    lng1 = get_radians(lng1);
    lng2 = get_radians(lng2);

    float lng_diff = lng2 - lng1; // get difference between longitudes must do 2 - 1

    float result = atan2(sin(lng_diff) * cos(lat2), cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(lng_diff));

    result = get_degrees(result);

    if (result < 0) result += 360.0f; // rationalise the result, if its a minus, add 360 to make it positive

    return result;
}

float get_turn_angle(float bearing, float heading){

    float turn_amount = bearing - heading;

    if (turn_amount > 180){ // rationalise the result
        turn_amount -= 360;
    } else if (turn_amount < -180) {
        turn_amount += 360;
    }

    return turn_amount;
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

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear(sf::Color::White); // set bg colour to white

        window.draw(text);
        window.draw(current_heading);
        window.draw(reqd_bearing);
        window.draw(turn_angle);

        window.display(); 
    }


    cout << get_bearing(AIRCRAFT_LAT, AIRCRAFT_LNG, VOR_LAT, VOR_LNG, AIRCRAFT_HEADING) << "\n";
    cout << "For present heading you must turn: " << get_turn_angle(get_bearing(AIRCRAFT_LAT, AIRCRAFT_LNG, VOR_LAT, VOR_LNG, AIRCRAFT_HEADING), AIRCRAFT_HEADING) << " degrees!" << endl;

    return 0;
}