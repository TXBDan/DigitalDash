#ifndef STRUCTDEFS_H
#define STRUCTDEFS_H


struct sensorstruct{
    char type;
    char name[6];
    unsigned short rawvalue;
    float value;
    float valuemax;
    float valuemin;
    int alarmmax;
    int alarmmin;
    int graphmax;
    int graphmin;
    char  alarmstate;
};

struct sensoraccel{
    float x;
    float y;
    float z;
};



#endif