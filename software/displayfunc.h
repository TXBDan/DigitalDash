#ifndef DISPLAYFUNC_H
#define DISPLAYFUNC_H

#include "mbed.h"
#include "TextLCD.h"
#include "structdefs.h"
#include "Adafruit_LEDBackpack.h"
#include "MMA7455.h"
#include "mcp3208.h" // ADC
#include "nmea.h"

extern TextLCD_I2C lcd;
extern Adafruit_24bargraph shiftlight1; // first shift light
extern Adafruit_24bargraph shiftlight2; // second shift l ight
extern MMA7455 accel; 
extern MCP3208 adc; 
extern NMEA nmea;

extern int rpminc; //temporary

extern sensorstruct sensor[];
extern sensoraccel gforce;
extern sensoraccel gforcepk;

extern short tempLUT[][39];

extern float tireDiam; //inches
extern float gearratio[];

extern int redline;
extern int ledCount;
extern int maxscaleRPM;

extern bool g_alarms;
extern bool g_isLogging;
extern int g_menustate;
extern float g_vspeed;
extern int g_rpm;
extern int g_gear;

extern float gps_lat;     //
extern float gps_long;    //
extern float gps_head;      // heading in degrees derived from previous & current location
extern int gps_sats;     // number of satellites used
extern float gps_alt;     //
extern int gps_fix;   
extern float gps_kph;


void DisplayLine(int line, char msg[]);
void DisplayDash();
void PrintBigNum(int number, int col);
void DisplayShiftLight(int startRPM, int maxRPM);
void GetAccelData();
void GetGPSData();
void GetADCData();
void CalcGear();
float ToTemperature(unsigned short adcIn);
float ToPressure(unsigned short data);
float ToPercent(unsigned short data);
float ConvCtoF(float cels);
float ConvKPHtoMPH(float kph);
int map(int val, int in_min, int in_max, int out_min, int out_max);



#endif