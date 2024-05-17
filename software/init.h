#ifndef INIT_H
#define INIT_H

// constants 
const int shiftAddr1 = 0x70; //i2c address for shift light
const int shiftAddr2 = 0x71; //i2c address for shift light
const float tireCirc = 78.6f; //inches
float tireDiam = 25.0f; //inches
float gearratio[6] = {3.46,4.2, 2.49, 1.66, 1.24, 1.0}; //final drive, 1st, 2nd, 3rd, 4th, 5th

int redline = 7000;
int ledCount = 48; // # LEDs in whole strip

// Temp LUT - hex, degC
short tempLUT[39][39] = {
    { 325, 150 },{ 362, 145 },{ 404, 140 },{ 451, 135 },
    { 505, 130 },{ 565, 125 },{ 634, 120 },{ 710, 115 },
    { 796, 110 },{ 892, 105 },{ 1000, 100 },{ 1118, 95 },
    { 1249, 90 },{ 1391, 85 },{ 1544, 80 },{ 1708, 75 },
    { 1882, 70 },{ 2063, 65 },{ 2248, 60 },{ 2435, 55 },
    { 2621, 50 },{ 2803, 45 },{ 2976, 40 },{ 3138, 35 },
    { 3287, 30 },{ 3421, 25 },{ 3540, 20 },{ 3644, 15 },
    { 3732, 10 },{ 3807,  5 },{ 3869,  0 },{ 3920, -5 },
    { 3961, -10 },{ 3993, -15 },{ 4018, -20 },{ 4038, -25 },
    { 4053, -30 },{ 4065, -35 },{ 4074, -40 }};

// global variables
bool g_alarms = false;
unsigned long g_seconds = 0;
float g_vspeed = 0;
int g_rpm = 0;
int g_gear = 0;
bool g_sampleDataFlag = false;
bool g_isLogging = false;
int g_menustate = 0;

float gps_time;         // UTC time
int gps_hours;
int gps_mins;
float gps_secs;
//char validity,ns,ew;// RMC data status A = Data Valid; V = Data Not valid;
float gps_lat;     //
float gps_long;    //
float gps_head;      // heading in degrees derived from previous & current location
int gps_sats;     // number of satellites used
float gps_alt;     //
int gps_fix;            // 0 = no fix;  1 = fix;  2=differential fix
//string cardinal;
float gps_kph;


struct sensorstruct sensor[8];
struct sensoraccel gforce;
struct sensoraccel gforcepk;

#endif