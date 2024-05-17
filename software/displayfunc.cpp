#include "displayfunc.h"


short errlist[8];
short errnum = 0;


/* Centered bar graph printer 
   data: data to graph
   min: data range min
   max: data range max
   col: LCD column to start bargraph print
   row: LCD row to start bargraph print 
   length: length (chars) of bar graph */
void PrintCenterBarGraph( float data, float min, float max, int col, int row, int length ){
    
    float blocks;
    float remainder = 0.0f;
    int wholeblocks = 0;
    
    if(length%2 != 0)   //if length is odd, make even
        length--;
        
    length = length/2; // divide in two for negative and positive
    blocks = data*length / (max - min); // decimal count of blocks in graph

    if(blocks >= 0){ // Positive side
    
        wholeblocks = (int)blocks;
        remainder = blocks - wholeblocks;
        
        for(int i=0; i < wholeblocks; i++){ //whole blocks
            lcd.locate(col+length+i,row); 
            lcd.putc(4);    // full block
        }
        if( wholeblocks < length ){  //only write if there is room
            lcd.locate(col+length+wholeblocks,row); 
            if( (remainder >= 0.000f) && (remainder < 0.199f) ){
                lcd.putc('-');  //nothing
            }
            if( (remainder >= 0.200f) && (remainder < 0.399f) ){
                lcd.putc(0);    // 1 bar
            }
            if( (remainder >= 0.400f) && (remainder < 0.599f) ){
                lcd.putc(1);    // 2 bars
            }
            if( (remainder >= 0.600f) && (remainder < 0.799f) ){
                lcd.putc(2);    // 3 bars
            }
            if( (remainder >= 0.800f) && (remainder <= 0.999f) ){
                lcd.putc(3);    // 4 bars
            }
        }
        if( (wholeblocks+1) < length){      // empty space?
            for(int i=0; i < (length-(wholeblocks+1)); i++){
                lcd.locate(col+length+wholeblocks+1+i,row); 
                lcd.putc('-');    
            }
        }
    } // end of positive
    else{ // negative side
    
        blocks = abs(blocks);
        wholeblocks = (int)blocks;
        
        for(int i=0; i < length; i++){ //whole blocks
            lcd.locate(col+length-1-i,row); 
            if(i < wholeblocks){
                lcd.putc(255);//(4);    // full block
            }
            else{
                lcd.putc('-');    
            }
        } 
    }// end negative side
}


/* Bar graph printer 
   data: data to graph
   min: data range min
   max: data range max
   col: LCD column to start bargraph print
   row: LCD row to start bargraph print 
   length: length (chars) of bar graph */
void PrintBarGraph( float data, float min, float max, int col, int row, int length){
    
    float blocks;
    float remainder = 0.0f;
    int wholeblocks = 0;
    
    data = abs(data); // all data graphs positive 
    
    if (data >= max)
        data = max - min;
    else if (data >= min)
        data = data - min;
    else
        data = 0;
        
    blocks = data*length / (max - min); // decimal count of blocks in graph
    wholeblocks = (int)blocks;
    remainder = blocks - wholeblocks;

    for(int i=0; i < wholeblocks; i++){ //whole blocks
        lcd.locate(col+i,row); 
        lcd.putc(4);    // full block
    }
    if( wholeblocks < length ){  //only write if there is room
        lcd.locate(col+wholeblocks,row); 
        if( (remainder >= 0.000f) && (remainder < 0.199f) ){
            lcd.putc('-');  //nothing
        }
        if( (remainder >= 0.200f) && (remainder < 0.399f) ){
            lcd.putc(0);    // 1 bar
        }
        if( (remainder >= 0.400f) && (remainder < 0.599f) ){
            lcd.putc(1);    // 2 bars
        }
        if( (remainder >= 0.600f) && (remainder < 0.799f) ){
            lcd.putc(2);    // 3 bars
        }
        if( (remainder >= 0.800f) && (remainder <= 0.999f) ){
            lcd.putc(3);    // 4 bars
        }
    }
    if( (wholeblocks+1) < length){      // empty space?
        for(int i=0; i < (length-(wholeblocks+1)); i++){
            lcd.locate(col+wholeblocks+1+i,row); 
            lcd.putc('-');    
        }
    }
}


/*                    */
void PrintBigNum(int number, int col){
    //5 top, 6 bot, 7 full
    switch(number){
        
     case 0:
        lcd.locate(col,   0);lcd.putc(7);
        lcd.locate(col+1, 0);lcd.putc(5);
        lcd.locate(col+2, 0);lcd.putc(7);
        
        lcd.locate(col,   1);lcd.putc(7);
        lcd.locate(col+1, 1);lcd.putc(' ');
        lcd.locate(col+2, 1);lcd.putc(7);
        
        lcd.locate(col,   2);lcd.putc(7);
        lcd.locate(col+1, 2);lcd.putc(' ');
        lcd.locate(col+2, 2);lcd.putc(7);
        
        lcd.locate(col,   3);lcd.putc(7);
        lcd.locate(col+1, 3);lcd.putc(6);
        lcd.locate(col+2, 3);lcd.putc(7);
        break;
 
    case 1:
        lcd.locate(col,   0);lcd.putc(6);
        lcd.locate(col+1, 0);lcd.putc(7);
        lcd.locate(col+2, 0);lcd.putc(' ');
        
        lcd.locate(col,   1);lcd.putc(' ');
        lcd.locate(col+1, 1);lcd.putc(7);
        lcd.locate(col+2, 1);lcd.putc(' ');
        
        lcd.locate(col,   2);lcd.putc(' ');
        lcd.locate(col+1, 2);lcd.putc(7);
        lcd.locate(col+2, 2);lcd.putc(' ');
        
        lcd.locate(col,   3);lcd.putc(6);
        lcd.locate(col+1, 3);lcd.putc(7);
        lcd.locate(col+2, 3);lcd.putc(6);
        break;
    
    case 2:
        lcd.locate(col,   0);lcd.putc(5);
        lcd.locate(col+1, 0);lcd.putc(5);
        lcd.locate(col+2, 0);lcd.putc(7);
        
        lcd.locate(col,   1);lcd.putc(6);
        lcd.locate(col+1, 1);lcd.putc(6);
        lcd.locate(col+2, 1);lcd.putc(7);
        
        lcd.locate(col,   2);lcd.putc(7);
        lcd.locate(col+1, 2);lcd.putc(' ');
        lcd.locate(col+2, 2);lcd.putc(' ');
        
        lcd.locate(col,   3);lcd.putc(7);
        lcd.locate(col+1, 3);lcd.putc(6);
        lcd.locate(col+2, 3);lcd.putc(6);
        break;
        
    case 3:
        lcd.locate(col,   0);lcd.putc(5);
        lcd.locate(col+1, 0);lcd.putc(5);
        lcd.locate(col+2, 0);lcd.putc(7);
        
        lcd.locate(col,   1);lcd.putc(6);
        lcd.locate(col+1, 1);lcd.putc(6);
        lcd.locate(col+2, 1);lcd.putc(7);
        
        lcd.locate(col,   2);lcd.putc(' ');
        lcd.locate(col+1, 2);lcd.putc(' ');
        lcd.locate(col+2, 2);lcd.putc(7);
        
        lcd.locate(col,   3);lcd.putc(6);
        lcd.locate(col+1, 3);lcd.putc(6);
        lcd.locate(col+2, 3);lcd.putc(7);
        break;
    
    case 4:
        lcd.locate(col,   0);lcd.putc(7);
        lcd.locate(col+1, 0);lcd.putc(' ');
        lcd.locate(col+2, 0);lcd.putc(7);
        
        lcd.locate(col,   1);lcd.putc(7);
        lcd.locate(col+1, 1);lcd.putc(6);
        lcd.locate(col+2, 1);lcd.putc(7);
        
        lcd.locate(col,   2);lcd.putc(' ');
        lcd.locate(col+1, 2);lcd.putc(' ');
        lcd.locate(col+2, 2);lcd.putc(7);
        
        lcd.locate(col,   3);lcd.putc(' ');
        lcd.locate(col+1, 3);lcd.putc(' ');
        lcd.locate(col+2, 3);lcd.putc(7);
        break;
    case 5:
        lcd.locate(col,   0);lcd.putc(7);
        lcd.locate(col+1, 0);lcd.putc(5);
        lcd.locate(col+2, 0);lcd.putc(5);
        
        lcd.locate(col,   1);lcd.putc(7);
        lcd.locate(col+1, 1);lcd.putc(6);
        lcd.locate(col+2, 1);lcd.putc(6);
        
        lcd.locate(col,   2);lcd.putc(' ');
        lcd.locate(col+1, 2);lcd.putc(' ');
        lcd.locate(col+2, 2);lcd.putc(7);
        
        lcd.locate(col,   3);lcd.putc(6);
        lcd.locate(col+1, 3);lcd.putc(6);
        lcd.locate(col+2, 3);lcd.putc(7);
        break;
    case 6://5 top, 6 bot, 7 full
        lcd.locate(col,   0);lcd.putc(7);
        lcd.locate(col+1, 0);lcd.putc(5);
        lcd.locate(col+2, 0);lcd.putc(5);
        
        lcd.locate(col,   1);lcd.putc(7);
        lcd.locate(col+1, 1);lcd.putc(6);
        lcd.locate(col+2, 1);lcd.putc(6);
        
        lcd.locate(col,   2);lcd.putc(7);
        lcd.locate(col+1, 2);lcd.putc(' ');
        lcd.locate(col+2, 2);lcd.putc(7);
        
        lcd.locate(col,   3);lcd.putc(7);
        lcd.locate(col+1, 3);lcd.putc(6);
        lcd.locate(col+2, 3);lcd.putc(7);
        break;  
    case 7:
        lcd.locate(col,   0);lcd.putc(5);
        lcd.locate(col+1, 0);lcd.putc(5);
        lcd.locate(col+2, 0);lcd.putc(7);
        
        lcd.locate(col,   1);lcd.putc(' ');
        lcd.locate(col+1, 1);lcd.putc(' ');
        lcd.locate(col+2, 1);lcd.putc(7);
        
        lcd.locate(col,   2);lcd.putc(' ');
        lcd.locate(col+1, 2);lcd.putc(' ');
        lcd.locate(col+2, 2);lcd.putc(7);
        
        lcd.locate(col,   3);lcd.putc(' ');
        lcd.locate(col+1, 3);lcd.putc(' ');
        lcd.locate(col+2, 3);lcd.putc(7);
        break;  
    case 8://5 top, 6 bot, 7 full
        lcd.locate(col,   0);lcd.putc(7);
        lcd.locate(col+1, 0);lcd.putc(5);
        lcd.locate(col+2, 0);lcd.putc(7);
        
        lcd.locate(col,   1);lcd.putc(7);
        lcd.locate(col+1, 1);lcd.putc(6);
        lcd.locate(col+2, 1);lcd.putc(7);
        
        lcd.locate(col,   2);lcd.putc(7);
        lcd.locate(col+1, 2);lcd.putc(' ');
        lcd.locate(col+2, 2);lcd.putc(7);
        
        lcd.locate(col,   3);lcd.putc(7);
        lcd.locate(col+1, 3);lcd.putc(6);
        lcd.locate(col+2, 3);lcd.putc(7);
        break;  
    case 9:
        lcd.locate(col,   0);lcd.putc(7);
        lcd.locate(col+1, 0);lcd.putc(5);
        lcd.locate(col+2, 0);lcd.putc(7);
        
        lcd.locate(col,   1);lcd.putc(7);
        lcd.locate(col+1, 1);lcd.putc(6);
        lcd.locate(col+2, 1);lcd.putc(7);
        
        lcd.locate(col,   2);lcd.putc(' ');
        lcd.locate(col+1, 2);lcd.putc(' ');
        lcd.locate(col+2, 2);lcd.putc(7);
        
        lcd.locate(col,   3);lcd.putc(' ');
        lcd.locate(col+1, 3);lcd.putc(' ');
        lcd.locate(col+2, 3);lcd.putc(7);
        break;     
    default:
        lcd.locate(col,   0);lcd.putc(' ');
        lcd.locate(col+1, 0);lcd.putc(' ');
        lcd.locate(col+2, 0);lcd.putc(' ');
        
        lcd.locate(col,   1);lcd.putc(6);
        lcd.locate(col+1, 1);lcd.putc(6);
        lcd.locate(col+2, 1);lcd.putc(6);
        
        lcd.locate(col,   2);lcd.putc(' ');
        lcd.locate(col+1, 2);lcd.putc(' ');
        lcd.locate(col+2, 2);lcd.putc(' ');
        
        lcd.locate(col,   3);lcd.putc(' ');
        lcd.locate(col+1, 3);lcd.putc(' ');
        lcd.locate(col+2, 3);lcd.putc(' ');
        break;
    
    
    }
}


void DisplayLine(int line, char msg[]){
    lcd.locate(0, line);
    lcd.printf("%s",msg);  
}



/* LCD Display */
void DisplayDash(){
    
    if(g_alarms == true){ // we have an alarm
    
        char str[3][21];
        snprintf(str[0], 21, "                    ");
        snprintf(str[1], 21, "                    ");
        snprintf(str[2], 21, "                    ");
    
        for(int i=0; i<errnum; i++){ //loop over errlist
            if( sensor[errlist[i]].alarmstate == 'H' ){
                snprintf(str[i], 21, "%s is HIGH!:%5.1f!", sensor[errlist[i]].name, sensor[errlist[i]].value);
            }
            if( sensor[errlist[i]].alarmstate == 'L' ){
                snprintf(str[i], 21, "%s is LOW!: %5.1f!", sensor[errlist[i]].name, sensor[errlist[i]].value);
            }
        }
        DisplayLine(0," DANGER TO MANIFOLD ");   //printf(" DANGER TO MANIFOLD \n");
        DisplayLine(1,str[0]);                   //printf("%s\n",str[0]);
        DisplayLine(2,str[1]);                   //printf("%s\n",str[1]);
        DisplayLine(3,str[2]);                   //printf("%s\n",str[2]);
    }
    else{ // all is good
        
        switch(g_menustate){
            
            case 0: // Main Display
                PrintBigNum(g_gear,0); //number, starting column
                lcd.locate(3, 0);
                lcd.printf(" SPD:%3d RPM:%4d",(int)g_vspeed,g_rpm);
                lcd.locate(3, 1);
                lcd.printf(" %s:%3d", sensor[0].name, (int)sensor[0].value);
                PrintBarGraph(sensor[0].value, sensor[0].graphmin, sensor[0].graphmax,13,1,7); // data, min, max, col, row, length
                lcd.locate(3, 2);
                lcd.printf(" %s:%3d",  sensor[1].name, (int)sensor[1].value);
                PrintBarGraph(sensor[1].value, sensor[1].graphmin, sensor[1].graphmax,13,2,7);
                lcd.locate(3, 3);
                lcd.printf(" %s:%3d",  sensor[4].name, (int)sensor[4].value);
                PrintBarGraph(sensor[4].value, sensor[4].graphmin, sensor[4].graphmax,13,3,7);
                break;
                
            case 1: // Speedo Display
                PrintBigNum((int)g_vspeed/10,0); //tens digit
                PrintBigNum((int)g_vspeed%10,4); //ones digit
                lcd.locate(8, 0);
                lcd.printf("RPM:%4d    ",g_rpm);
                lcd.locate(8, 1);
                lcd.printf("WT:%3d", (int)sensor[0].value);
                PrintBarGraph(sensor[0].value, sensor[0].graphmin, sensor[0].graphmax,14,1,6); // data, min, max, col, row, length
                lcd.locate(8, 2);
                lcd.printf("OT:%3d", (int)sensor[1].value);
                PrintBarGraph(sensor[1].value, sensor[1].graphmin, sensor[1].graphmax,14,2,6);
                lcd.locate(8, 3);
                lcd.printf("OP:%3d", (int)sensor[4].value);
                PrintBarGraph(sensor[4].value, sensor[4].graphmin, sensor[4].graphmax,14,3,6);
                break;
                
            case 2: // Sensors
                lcd.locate(0, 0);
                lcd.printf("%s%5.1f%s%5.1f", sensor[0].name, sensor[0].value, sensor[4].name, sensor[4].value);
                lcd.locate(0, 1);
                lcd.printf("%s%5.1f%s%5.1f", sensor[1].name, sensor[1].value, sensor[5].name, sensor[5].value);
                lcd.locate(0, 2);
                lcd.printf("%s%5.1f%s%5.1f", sensor[2].name, sensor[2].value, sensor[6].name, sensor[6].value);
                lcd.locate(0, 3);
                lcd.printf("%s%5.1f%s%5.1f", sensor[3].name, sensor[3].value, sensor[7].name, sensor[7].value);
                break;
                
            case 3: // Min/Max
                lcd.locate(0, 0);
                lcd.printf("%s:%5.1f %3d %3d ", sensor[0].name, sensor[0].value, (int)sensor[0].valuemin, (int)sensor[0].valuemax);
                lcd.locate(0, 1);
                lcd.printf("%s:%5.1f %3d %3d ", sensor[1].name, sensor[1].value, (int)sensor[1].valuemin, (int)sensor[1].valuemax);
                lcd.locate(0, 2);
                lcd.printf("%s:%5.1f %3d %3d ", sensor[4].name, sensor[4].value, (int)sensor[4].valuemin, (int)sensor[4].valuemax);
                lcd.locate(0, 3);
                lcd.printf("%s:%5.1f %3d %3d ", sensor[5].name, sensor[5].value, (int)sensor[5].valuemin, (int)sensor[5].valuemax);
                break;
        
            case 4: // G-Force
                DisplayLine(0, "     G-FORCE     PK ");
                lcd.locate(0, 1);
                lcd.printf("X:%5.2f", gforce.x);
                PrintBarGraph(gforce.x,0,1.3,7,1,9); // data, min, max, col, row, length
                lcd.locate(16, 1);
                lcd.printf("%4.2f", gforcepk.x);
                
                lcd.locate(0, 2);
                lcd.printf("Y:%5.2f", gforce.y);
                PrintBarGraph(gforce.y,0,1.3,7,2,9); // data, min, max, col, row, length
                lcd.locate(16, 2);
                lcd.printf("%4.2f", gforcepk.y);
                
                lcd.locate(0, 3);
                lcd.printf("Z:%5.2f", gforce.z);
                PrintBarGraph(gforce.z,0,1.3,7,3,9); // data, min, max, col, row, length
                lcd.locate(16, 3);
                lcd.printf("%4.2f", gforcepk.z);
                break;

            case 5: // GPS
                DisplayLine(0, "      GPS DATA      ");
                lcd.locate(0, 1);
                lcd.printf("FIX : %d   SATS: %2d  ", gps_fix, gps_sats );
                lcd.locate(0, 2);
                lcd.printf("LAT : %10.6f    ", gps_lat);
                lcd.locate(0, 3);
                lcd.printf("LONG: %10.6f    ", gps_long);
                break;
                
            /*case 5: // ADC
                lcd.locate(0, 0);
                lcd.printf("CH0:%4u  CH4:%4u  ", sensor[0].rawvalue, sensor[4].rawvalue);
                lcd.locate(0, 1);
                lcd.printf("CH1:%4u  CH5:%4u  ", sensor[1].rawvalue, sensor[5].rawvalue);
                lcd.locate(0, 2);
                lcd.printf("CH2:%4u  CH6:%4u  ", sensor[2].rawvalue, sensor[6].rawvalue);
                lcd.locate(0, 3);
                lcd.printf("CH3:%4u  CH7:%4u  ", sensor[3].rawvalue, sensor[7].rawvalue);
                break;*/
                
            case 6: // Setup
                DisplayLine(0, "       SETUP        ");
                lcd.locate(0, 1);
                lcd.printf("RECORD:%s",  g_isLogging ? "  ON          ":" OFF          ");
                lcd.locate(0, 2);
                lcd.printf("                    ");
                lcd.locate(0, 3);
                lcd.printf("                    ");
                break;
        } //end switch
    } // end no warning
}


/* Shift Light */
void DisplayShiftLight(int startRPM, int maxRPM) {
    
    if(g_alarms){
        for (int thisLed = 0; thisLed < ledCount/2; thisLed++) {   
            shiftlight1.setBar(thisLed, LED_RED);
            shiftlight2.setBar(thisLed, LED_RED);
        }
        shiftlight1.blinkRate(1); // options are 1 (faster),2, or 3 (slower)
        shiftlight2.blinkRate(1); // options are 1 (faster),2, or 3 (slower)   
    }
    else{
        int shift1Level = 0;
        int shift2Level = 0;
        
        // map the result to a range from 0 to the number of LEDs:
        int ledLevel = map(g_rpm, startRPM, maxRPM, 0, ledCount);
        
        if(ledLevel <= 24){
            shift1Level = ledLevel;
            shift2Level = 0;
        }else{
            shift1Level = 24;
            shift2Level = ledLevel - 24;
        }
        //clear all LEDs
        for (int thisLed = 0; thisLed < ledCount/2; thisLed++) {   
            shiftlight1.setBar(thisLed, LED_OFF);
            shiftlight2.setBar(thisLed, LED_OFF);
        }
        //set LEDs
        if ( g_rpm > maxRPM-200){ // solid red if close to redline
            for (int thisLed = 0; thisLed < ledCount/2; thisLed++) {   
                shiftlight1.setBar(thisLed, LED_RED);
                shiftlight2.setBar(thisLed, LED_RED);
            }
            shiftlight1.blinkRate(0); // options are 0 (off), 1 (faster),2, or 3 (slower)
            shiftlight2.blinkRate(0); // options are 0 (off), 1 (faster),2, or 3 (slower)
        }
        else{       //normal operation
            // loop over first half of the LED array: (first two LED bars)
            for (int thisLed = 0; thisLed < ledCount/2; thisLed++) {
                if (thisLed < shift1Level) shiftlight1.setBar(thisLed, LED_GREEN);
            }
            // loop from 1/2 to 3/4 (second two LED bars)
            for (int thisLed = 0; thisLed < ledCount/4; thisLed++) {  
                if (thisLed < shift2Level) shiftlight2.setBar(thisLed, LED_YELLOW);
            }
            // loop over last 1/4th of leds
            for (int thisLed = ledCount/4; thisLed < ledCount/2; thisLed++) { 
                if (thisLed < shift2Level) shiftlight2.setBar(thisLed, LED_RED);
            }
            shiftlight1.blinkRate(0); //turn off blinking
            shiftlight2.blinkRate(0); //turn off blinking
        } 
    }
    shiftlight1.writeDisplay();
    shiftlight2.writeDisplay();
}


/* Read Accerometer */
void GetAccelData(){
    
    static float oldx, oldy, oldz;
    
    int val[3]={0,0,0};
    if (!accel.read(val[0], val[1], val[2])) {
        printf("Failed to read accelerometer data!\n");
    }
    gforce.x = val[0]/64.0f; //scales to Gs
    gforce.y = val[1]/64.0f;
    gforce.z = val[2]/64.0f;
    
    gforce.x = oldx+0.38587f*(gforce.x-oldx); //LP filter at 1hz for 10hz sample rate
    gforce.y = oldy+0.38587f*(gforce.y-oldy);
    gforce.z = oldz+0.38587f*(gforce.z-oldz);
    
    oldx = gforce.x;
    oldy = gforce.y;
    oldz = gforce.z;
    
    if(abs(gforce.x) > gforcepk.x)
        gforcepk.x = abs(gforce.x);
        
    if(abs(gforce.y) > gforcepk.y)
        gforcepk.y = abs(gforce.y);
        
    if(abs(gforce.z) > gforcepk.z)
        gforcepk.z = abs(gforce.z);
}


/* Get GPS Data */
void GetGPSData(){
    
    if (nmea.isdataready()) {
        //GGA
        gps_lat = nmea.getLatitude();
        gps_long = nmea.getLongitude();
        gps_fix = nmea.getFix();
        gps_sats = nmea.getSatellites();
        //RMC
        gps_alt = nmea.getAltitude();
        gps_kph = nmea.getSpeed();
        gps_head = nmea.getBearing();
    }     
}


/* Get ADC Data */
void GetADCData(){
    
    g_alarms = false;
    errnum = 0;
    
    for(int i=0; i<8; i++){
        sensor[i].rawvalue = adc.read_input(i); //adcbuff[i]; 
        //printf("Chan %d: %u\n",i,sensor[i].rawvalue);

        //convert to units by type
        if(sensor[i].type == 'T'){        // if sensor type is Temperature                  
            sensor[i].value = ConvCtoF(ToTemperature(sensor[i].rawvalue));             
        }
        else if(sensor[i].type == 'P'){    // if sensor type is Pressure             
            sensor[i].value = ToPressure(sensor[i].rawvalue);                   
        }
        else if(sensor[i].type == '%'){    // if sensor type is Percent             
            sensor[i].value = ToPercent(sensor[i].rawvalue);                   
        }
        else{                  
            sensor[i].value = 5.0f*sensor[i].rawvalue/4096.0f;   // voltage reading        
        }
        //printf("sensor:%d value:%f\n",i,sensor[i].value);
        
        //set mins and maxes
        if(sensor[i].value > sensor[i].valuemax)
            sensor[i].valuemax = sensor[i].value;
            
        if(sensor[i].value < sensor[i].valuemin)
            sensor[i].valuemin = sensor[i].value;        
        
        /*  //check for alarm conditions
        if( sensor[i].value > sensor[i].alarmmax ){
            sensor[i].alarmstate = 'H';  
            g_alarms = true;
            if(errnum < 3){ //only three LCD rows to fill with alarms
                errlist[errnum] = i; //build list of channels with alarms on
                errnum++;
            }
        }
        else if( sensor[i].value < sensor[i].alarmmin ){
            sensor[i].alarmstate = 'L';
            g_alarms = true;  
            if(errnum < 3){ //only three LCD rows to fill with alarms
                errlist[errnum] = i; //build list of channels with alarms on
                errnum++;
            }
        }
        else{
            sensor[i].alarmstate = ' ';
        } */
        //wait_ms(1);
    } // end of for
}


/* Calculate current gear */
void CalcGear(){
    
    if(g_vspeed <= 0){
        g_gear = 0;
    }else{
        float ratio = (g_rpm*tireDiam) / (g_vspeed*336.0f*gearratio[0]);
        //1st gear?
        if( (ratio >= gearratio[1]-0.5f) && (ratio <= gearratio[1]+0.5f) ){
            g_gear = 1;    
        }//2nd gear?
        else if( (ratio >= gearratio[2]-0.5f) && (ratio <= gearratio[2]+0.5f) ){
            g_gear = 2;
        }//3rd gear?
        else if( (ratio >= gearratio[3]-0.3f) && (ratio <= gearratio[3]+0.3f) ){
            g_gear = 3;
        //4th gear?
        }else if( (ratio >= gearratio[4]-0.1f) && (ratio <= gearratio[4]+0.1f) ){
            g_gear = 4;
        //5th gear?
        }else if( (ratio >= gearratio[5]-0.1f) && (ratio <= gearratio[5]+0.1f) ){
            g_gear = 5;  
        }else
            g_gear = 0; 
    }
}


/* Temp Sensor Look Up Table 
*  Returns Temp in C
*/
float ToTemperature(unsigned short adcIn) {

    short lowerI = 0;
    short upperI = 0;
    for(short i=0; i<39; i++) {
        if(tempLUT[i][0] <= adcIn ){
             lowerI = i;
        }
        else{
            upperI = i;
            break;
        }       
    }
    return tempLUT[lowerI][1] + (float)( tempLUT[upperI][1] - tempLUT[lowerI][1] )*( adcIn - tempLUT[lowerI][0] )/(float)( tempLUT[upperI][0] - tempLUT[lowerI][0] ); 
}

/* converts voltage to pressure (PSI) for AEM 30-2131-100 sensor */
float ToPressure(unsigned short data) {
    return 125.0f*data/4096-12.5f;
}

float ToPercent(unsigned short data){
    return data*100.0f/4096.0f;   // % 0-100. may need scale/offset adjustment    
}

/* converts degrees C to F */
float ConvCtoF(float cels) {
    return cels*9.0f/5.0f+32.0f;
}


/* converts KPH to MPH */
float ConvKPHtoMPH(float kph) {
    return kph*0.62137f;
}


/*val: the number to map
in_min: the lower bound of the value's current range
in_max: the upper bound of the value's current range
out_min: the lower bound of the value's target range
out_max: the upper bound of the value's target range  */
int map(int val, int in_min, int in_max, int out_min, int out_max) {
  return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}