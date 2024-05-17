#include "mbed.h"
#include "MCIFileSystem.h"
#include "Adafruit_LEDBackpack.h"
#include "TextLCD.h"
#include "nmea.h" 
#include "MMA7455.h" //accelerometer
#include "mcp3208.h" // ADC
#include "structdefs.h"
#include "init.h"
#include "displayfunc.h"
 
DigitalOut led1(LED1); //active low   sampling/writing
DigitalOut led2(LED2); //active low   data logging
DigitalOut led3(LED3); //active high  RPM interrupt
DigitalOut led4(LED4); //active high  Speed interrupt
DigitalOut cs(p8);     // for SPI1

DigitalIn btn_rec(p16, PullUp);
DigitalIn btn_1(p17, PullUp);
DigitalIn btn_2(p18, PullUp);

InterruptIn rpmIntInput(p14);   // RPM pulse train
InterruptIn speedIntInput(p15); // vehicle speed pulse train

Timer rpmPulseTimer;        // counts time between pulses
Timer speedPulseTimer;      // counts time between pulses
Timer logtimer;             // for log Time
Timer logtimeouttimer;      // timer to timeout to disable logging

Ticker tenHzTicker;         //for 10hz sample rate
Ticker twohundredHzTicker;  //for button checking

NMEA nmea;                  // GPS parser
Serial pc(USBTX, USBRX);    // serial port over programming USB connection 
Serial gps(P4_22, P4_23);   // GPS receiver in Xbee socket
MCIFileSystem mcifs("mci"); // create SD card file system
SPI spi1(p11,p12,p13);      // SPI1, mosi, miso, sclk for ADC
//SPI spi2(p39,p38,p32);    // Unused
I2C i2c(p9, p10);           // SDA, SCL for shift light

MMA7455 accel(p9,p10);      // MMA7455 accelerometer
MCP3208 adc(spi1, p8);      // ADC, spi, CS
Adafruit_24bargraph shiftlight1 = Adafruit_24bargraph(&i2c); // first shift light
Adafruit_24bargraph shiftlight2 = Adafruit_24bargraph(&i2c); // second shift l ight
TextLCD_I2C lcd(&i2c, 0x40, TextLCD::LCD20x4); // I2C bus, PCF8574 Slaveaddress, LCD Type


void CalcSpeed_ISR() {          //triggered every speed signal rising edge 
    int speedPeriod = speedPulseTimer.read_us();    // Get time since last interrupt  
    if( speedPeriod > 490000 ) g_vspeed = 0.0f;
    else if( speedPeriod < 2450 ) g_vspeed = 200.0f;
    else g_vspeed = 6313.13f*tireCirc/speedPeriod;    //converts period in us to speed (MPH)
    speedPulseTimer.reset();  
}


void CalcRPM_ISR() {            //triggered every rpm signal rising edge
    float rpm;
    static float oldrpm;        //static to keep value around for next time
    int rpmPeriod = rpmPulseTimer.read_us();    // Get time since last interrupt
    if( rpmPeriod > 20000000 ) rpm = 0;
    else if( rpmPeriod < 2000 ) rpm = 10000;
    else rpm = 20000000/rpmPeriod;            //converts period in us to RPM
    rpm = oldrpm+0.1586f*(rpm-oldrpm); // LP filter tuned to 0.3hz based on 10hz sample rate
    oldrpm = rpm;
    g_rpm = (int)rpm;
    rpmPulseTimer.reset();                      // Reset timer and wait for next interrupt
}


void SampleData_ISR() {     //called every sample rate
    g_sampleDataFlag = 1;
}


void ButtonCheck_ISR(){           
    static uint16_t state_btn1 = 0; // current debounce status
    static uint16_t state_btn2 = 0;
    static uint16_t state_rec = 0; 
    
    state_btn1 = ( state_btn1 << 1 ) | btn_1 | 0xE000; //select values?
    if( state_btn1 == 0xF000 ){ 
        g_menustate--;
        if( g_menustate < 0 ) g_menustate=6;
    }
    
    state_btn2 = ( state_btn2 << 1 ) | btn_2 | 0xE000; //increment screens
    if( state_btn2 == 0xF000 ){ 
        g_menustate++;
        if( g_menustate > 6 ) g_menustate=0;
    }
    
    state_rec = ( state_rec << 1 ) | btn_rec | 0xE000; //toggle recording
    if( state_rec == 0xF000 ) {
        if(g_menustate == 3){ //if gforce screen
            gforcepk.x = 0;
            gforcepk.y = 0;
            gforcepk.z = 0;   
        }
        else
            g_isLogging = !g_isLogging;
    }
}


void gps_ISR(){     // GPS Buffer 
    char c = gps.getc(); //Read the value out of the UART buffer
    nmea.fusedata(c);
}



/************
 *** MAIN ***
 ************/
int main() {
    
    bool wasLogging = false;
    int fileNumber = 0;
    char fname[16];

    // set baud rate for UART over USB to PC
    pc.baud(115200);
    
    // config sensors
    sensor[0].type = 'T';
    strncpy(sensor[0].name, "WTR T", 5);sensor[0].name[5] = '\0';
    sensor[0].alarmmin = 30;
    sensor[0].alarmmax = 210;
    sensor[0].graphmin = 120;
    sensor[0].graphmax = 240;
    sensor[0].valuemin = 50000;
    sensor[0].valuemax = -50000;
    sensor[0].alarmstate = 'N';

    sensor[1].type = 'T';
    strncpy(sensor[1].name, "OIL T", 5);sensor[1].name[5] = '\0';
    sensor[1].alarmmin = 30;
    sensor[1].alarmmax = 230;
    sensor[1].graphmin = 140;
    sensor[1].graphmax = 280;
    sensor[1].valuemin = 50000;
    sensor[1].valuemax = -50000;
    sensor[1].alarmstate = 'N';

    sensor[2].type = 'X';
    strncpy(sensor[2].name, "CHAN3", 5);sensor[2].name[5] = '\0';
    sensor[2].alarmmin = 0;
    sensor[2].alarmmax = 4096;
    sensor[2].valuemin = 50000;
    sensor[2].valuemax = -50000;
    sensor[2].alarmstate = 'N';

    sensor[3].type = 'X';
    strncpy(sensor[3].name, "CHAN4", 5);sensor[3].name[5] = '\0';
    sensor[3].alarmmin = 0;
    sensor[3].alarmmax = 4096;
    sensor[3].valuemin = 50000;
    sensor[3].valuemax = -50000;
    sensor[3].alarmstate = 'N';
    
    sensor[4].type = 'P';
    strncpy(sensor[4].name, "OIL P", 5);sensor[4].name[5] = '\0';
    sensor[4].alarmmin = 10;
    sensor[4].alarmmax = 90;
    sensor[4].graphmin = 0;
    sensor[4].graphmax = 100;
    sensor[4].valuemin = 50000;
    sensor[4].valuemax = -50000;
    sensor[4].alarmstate = 'N';
    
    sensor[5].type = '%';
    strncpy(sensor[5].name, "  TPS", 5);sensor[5].name[5] = '\0';
    sensor[5].alarmmin = -10;
    sensor[5].alarmmax = 110;
    sensor[5].graphmin = 0;
    sensor[5].graphmax = 100;
    sensor[5].valuemin = 50000;
    sensor[5].valuemax = -50000;
    sensor[5].alarmstate = 'N';
    
    sensor[6].type = 'X';
    strncpy(sensor[6].name, "CHAN7", 5);sensor[6].name[5] = '\0';
    sensor[6].alarmmin = 0;
    sensor[6].alarmmax = 4096;
    sensor[6].valuemin = 50000;
    sensor[6].valuemax = -50000;
    sensor[6].alarmstate = 'N';
    
    sensor[7].type = 'X';
    strncpy(sensor[7].name, "CHAN8", 5);sensor[7].name[5] = '\0';
    sensor[7].alarmmin = 0;
    sensor[7].alarmmax = 4096;
    sensor[7].valuemin = 50000;
    sensor[7].valuemax = -50000;
    sensor[7].alarmstate = 'N';
                       
    // turn off LEDs
    led1 = 1;
    led2 = 1;
    led3 = 0;
    led4 = 0;
    
    /*** Setup i2c ***/
    i2c.frequency(1000000); //LCD, shiftlight, accelerometer (400k)
    
    /* Setup ADC and SPI bus */
    DisplayLine(0,"                    ");
    DisplayLine(1,"  Initializing...   ");
    DisplayLine(2,"  ADC               ");
    DisplayLine(3,"                    ");
    printf("Initializing ADC...\n");
    spi1.format(8,0);
    spi1.frequency(1000000);
    wait_ms(100);
    
    /*** Setup Shift Light ***/
    DisplayLine(0,"                    ");   
    DisplayLine(1,"  Initializing...   ");
    DisplayLine(2,"  Shift Light       ");
    DisplayLine(3,"                    ");
    printf("Initializing Shift Light...\n");
    shiftlight1.begin(shiftAddr1);
    shiftlight2.begin(shiftAddr2);
    wait_ms(100);
    
    /*** Setup Accelerometer */
    DisplayLine(0,"                    ");
    DisplayLine(1,"  Initializing...   ");
    DisplayLine(2,"  Accelerometer     ");
    DisplayLine(3,"                    ");
    printf("Initializing Accelerometer...\n");
    if(!accel.setRange(MMA7455::Range_2g)) {
        printf("Unable to set range for MMA7455!\n");
    }
    wait_ms(50);
    if (!accel.setMode(MMA7455::ModeMeasurement)) {
        printf("Unable to set mode for MMA7455!\n");
    }
    wait_ms(50);
    if (!accel.calibrate()) {
        printf("Failed to calibrate MMA7455!\n");
    }
    wait_ms(50);
    
    /*** Setup RPM pulse timer ***/
    //rpmIntInput.mode(PullDown);             // Set the pin to Pull Down mode. REMEMBER TO SWITCH
    rpmIntInput.rise(&CalcRPM_ISR);         // Set up the interrupt for rising edge
    rpmPulseTimer.start();                  // start the timer
    
    /*** Setup Speed pulse timer ***/
    //speedIntInput.mode(PullDown);           // Set the pin to Pull Down mode.
    speedIntInput.rise(&CalcSpeed_ISR);     // Set up the interrupt for rising edge
    speedPulseTimer.start();    
    
    // Setup the GPS
    DisplayLine(0,"                    ");
    DisplayLine(1,"  Initializing...   ");
    DisplayLine(2,"  GPS               ");
    DisplayLine(3,"                    ");
    printf("Initializing GPS...\n");
    gps.baud(115200);
    wait_ms(100);
    //gps.printf("$PMTK220,1000*1F\r\n"); //1hz
    gps.printf("$PMTK220,200*2C\r\n"); //5hz
    //gps.printf("$PMTK220,100*2F\r\n"); // 10hz
    wait_ms(100);
    gps.printf("$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n"); // GGA and RMC
    wait_ms(100);
    gps.printf("$PMTK313,1*2E\r\n"); //enable SBAS mode
    wait_ms(100);
    gps.printf("$PMTK301,2*2E\r\n"); //set DGPS/SBAS to WAAS mode
    wait_ms(100);
    gps.printf("$PMTK397,0*23\r\n"); //disable nav speed threshold
    wait_ms(100);
    gps.attach(&gps_ISR);
    wait_ms(100);
    
    /* Setup LCD */
    lcd.setUDC(0, (char *) udc_bar_1);    
    lcd.setUDC(1, (char *) udc_bar_2);    
    lcd.setUDC(2, (char *) udc_bar_3);   
    lcd.setUDC(3, (char *) udc_bar_4);   
    lcd.setUDC(4, (char *) udc_bar_5);  
    lcd.setUDC(5, (char *) udc_tophalf); 
    lcd.setUDC(6, (char *) udc_bothalf); 
    lcd.setUDC(7, (char *) udc_fullblock); //same as putc(255);
    lcd.cls();
    
    /*** SD Card Setup ***/
    DisplayLine(0,"                    ");
    DisplayLine(1,"  Initializing...   ");
    DisplayLine(2,"  SD Card           ");
    DisplayLine(3,"                    ");
    printf("Initializing SD Card...\n");
    wait_ms(100);
    FILE *fp;
    if(!mcifs.cardInserted()) {
        printf("Please insert a SD/MMC card\n");
        DisplayLine(0,"                    ");
        DisplayLine(1,"    NO SD CARD!!!   ");
        DisplayLine(2,"                    ");
        DisplayLine(3,"                    ");
        wait(3);
    }
    else{
        printf("Found SD/MMC card\n");
        DisplayLine(0,"                    ");
        DisplayLine(1,"SD Card OK          ");
        DisplayLine(2,"                    ");
        DisplayLine(3,"                    ");
        wait_ms(100);
    }
    
    /* Init Screen */
    DisplayLine(0,"                    ");   
    DisplayLine(1," Digital Dashboard  ");
    DisplayLine(2,"    V1.02 2015      ");
    DisplayLine(3,"                    ");
    printf("Digital Dashboard V1.02\n");
    wait_ms(750);
    lcd.cls();
    
    /*** Setup and Start Button Checking Ticker ***/
    twohundredHzTicker.attach(&ButtonCheck_ISR, 0.005); //buttons are checked every 5ms
    
    /*** Setup and Start Sampling Ticker ***/
    tenHzTicker.attach(&SampleData_ISR, 0.1); //10hz data is sampled once every 0.1s
    
    // MAIN LOOP //
    while(1) {
             
        if( g_rpm < 1000 ){
            logtimeouttimer.reset();
            logtimeouttimer.start();
        }     
             
        if( logtimeouttimer.read() > 10 ) //greater than 10s
            g_isLogging = false;
             
        if( g_sampleDataFlag == 1 ) { //time to sample!
            
            led1 = 0; //turn on
            
            /*** Get GPS data ***/
            GetGPSData();
                   
            /*** Sample 16 ADC Channels ***/ 
            GetADCData();
            
            /*** Get Accelerometer Data ***/
            GetAccelData();
               
            /*** Get Gear Selected ***/
            CalcGear();


            /*** Display Shift Light ***/
            DisplayShiftLight(3000, redline);

            /*** Display Dashboard ***/
            DisplayDash();
            
            
            /*** write to SD Card ***/
            if( g_isLogging ){    //printf("logging enabled\n\r");           
                if( mcifs.cardInserted() ){ //check for SD card              
                    if( wasLogging ){ //continuing to log  //printf("continuing to log\n\r");  
                                    //Time,Longi ,Lati  ,Altit,GPSu,X    ,Y    ,MPH  ,Heading,RPM ,Gear,WT   ,OT   ,OP   ,TPS\n");
                        fprintf(fp, "%9.3f,%10.6f,%10.6f,%8.2f,%10d,%6.3f,%6.3f,%5.1f,%7.1f,%4d,%4d,%5.1f,%5.1f,%5.1f,%3d\n",logtimer.read(),gps_long,gps_lat,gps_alt,1,gforce.x,gforce.y,g_vspeed,gps_head,g_rpm,g_gear,sensor[0].value, sensor[1].value, sensor[4].value, (int)sensor[5].value);                    
                    }
                    else{   //just enabled logging, setup file   //printf("open new file\n\r");
                        snprintf(fname,16,"/mci/datalog%04d.csv",fileNumber); // change to match your file name structure               
                        fp = fopen(fname,"r");        
                        while (fp) {    //search for latest file name
                          fclose(fp);
                          fileNumber++;
                          snprintf(fname,16,"/mci/datalog%04d.csv",fileNumber); //update filename to search
                          fp = fopen(fname,"r"); //checking existence of fname
                        }             
                        fp = fopen(fname, "w"); //current log file name
                        if (fp != NULL) {
                            fprintf(fp, "Time     ,Longitude ,Latitude  ,Altitude,GPS_update,X     ,Y     ,MPH  ,Heading,RPM ,Gear,WTR T,OIL T,OIL P,TPS\n");
                            //fprintf(fp, "12345.123,123.123456,123.123456,12345.12,         1,-1.123,-1.123,123.1,  127.1,1234,   1,123.1,123.1,123.1,123\n");
                            logtimer.reset();
                            logtimer.start();
                        } else {
                            printf("Failed to open log file\n");
                        }   
                    }
                }//else{ //SD card NOT inserted
                 //   lcd.printf("    NO SD CARD!!!   ");
                //}
            } // end of g_isLogging
            else if(wasLogging==true){
                 fclose(fp); //close file
            }
            
            wasLogging = g_isLogging; //save current state to previous   
            g_sampleDataFlag = 0;     
            led1 = 1;     //led off
        } //end of sampling, g_sampleDataFlag==1
        
    } //end of while(1)

} //end of main()


