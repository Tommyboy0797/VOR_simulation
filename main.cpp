#include <iostream> 
#include <vector> 
#include <string>
#include <algorithm>
#include <cmath>
using namespace std;

int AIRCRAFT_HEADING = 0;
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

int main(){
    
    cout << get_bearing(AIRCRAFT_LAT, AIRCRAFT_LNG, VOR_LAT, VOR_LNG, AIRCRAFT_HEADING);

    return 0;
}