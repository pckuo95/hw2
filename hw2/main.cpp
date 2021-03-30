#include "mbed.h"
#include "TextLCD.h"    // haven't include

#define NSAMPLE 200
#define FS 200 // sampling rate

// Control buttons
DigitalIn buttonDown(D10);
DigitalIn buttonUp(D11);
DigitalIn buttonSel(D12);

// Display uLCD
TextLCD lcd(D2, D3, D4, D5, D6, D13); // RS, E, DB4-DB7

// DAC output
AnalogOut Aout(D7);

//ADC input
AnalogIn Ain(A0);

// ADC array
float sample[NSAMPLE];
int cnt_sample = 0;
int index_sample = 0;

int freqState(int state, int buttonDown, int buttonUp);
void generateWave(int freq);
void outputSample();

int main()
{
    int freq = 10;  // initial frequency 10Hz
    int state = 0;  // select frequency
    int enable = 0;
    int buttonD = 0;
    int buttonU = 0;
    int buttonS = 0;
    int pressCnt = 0;
    int print_EN = 0;

    // initial display
    lcd.locate(0,0);
    lcd.printf("Press Button to \n");
    lcd.locate(0,1);
    lcd.printf("Select frequency\n");

    while(true) {
        if (buttonDown == 1 || buttonUp == 1 || buttonSel == 1) {
            if (buttonDown == 1) {
                buttonD = 0;
                pressCnt++;
                if (pressCnt == 1) {
                    buttonD = 1;
                    pressCnt = 0;
                    ThisThread::sleep_for(250ms);
                }
            }
            else if (buttonUp == 1) {
                buttonU = 0;
                pressCnt++;
                if (pressCnt == 1) {
                    buttonU = 1;
                    pressCnt = 0;
                    ThisThread::sleep_for(250ms);
                }
            }
            else if (buttonSel == 1) {
                buttonS = 0;
                pressCnt++;
                if (pressCnt == 1) {
                    buttonS = 1;
                    ThisThread::sleep_for(250ms);
                }
            }
            if (buttonD == 1 || buttonU == 1) {
                state = freqState(state, buttonD, buttonU);
        
                switch (state) {
                    case 1:     freq = 5;
                        break;
                    case 2:     freq = 10;
                        break;
                    case 3:     freq = 20;
                        break;
                    case 4:     freq = 50;
                        break;
                    default:    freq = 1;
                        break;
                }
                lcd.locate(0,0);
                lcd.printf("Sel Freq = %2d Hz\n", freq);
            }
        
            if (buttonS) {
                enable = !enable;
            }
            if (enable) {
                lcd.locate(0,0);
                lcd.printf("Sel Freq = %2d Hz\n", freq);
                lcd.locate(0,1);
                lcd.printf("   Output ON!   \n");
                print_EN = 1;
            } else {
                lcd.locate(0,1);
                lcd.printf("   Output OFF   \n");
            }
        }
        else {
            buttonS = 0;
            buttonU = 0;
            buttonD = 0;
            pressCnt = 0;
        }
   
        if (enable) {
            generateWave(freq);
        } else {
            cnt_sample = 0;
            index_sample = 0;
            if (print_EN == 1) {
                outputSample();     // print result to screen
                print_EN = 0;       // only print once
            }
        }
    }
    return 0;
}


// define 5 diff freq for this machine
int freqState(int state, int buttonDown, int buttonUp)
{
    if (state < 4 && buttonUp) {
        state++;
    } 
    else if (state > 0 && buttonDown){
        state--;
    }
    return state;
}


// As studentID = 107012045, S = 0
// generate wave via DAC and collect wave via ADC
void generateWave(int freq)
{
    double i;
    int j;

    // 1Hz
    if (freq == 50){
        for(i = 1, j = 0; i > 0; i = i - 0.05) {
            Aout = i;
            cnt_sample++;
            if (cnt_sample == 5 && index_sample < NSAMPLE) {
                cnt_sample = 0;
                sample[index_sample] = Ain;
                index_sample++;
            }
            ThisThread::sleep_for(1ms); // 50Hz => 1/50/20=0.001
        }
        
    } else if (freq == 5) {
        for(i = 1; i > 0; i = i-0.005) {
            Aout = i;
            cnt_sample++;
            if (cnt_sample == 5 && index_sample < NSAMPLE) {
                cnt_sample = 0;
                sample[index_sample] = Ain;
                index_sample++;
            }
            ThisThread::sleep_for(1ms); // 5Hz => 1/5/100=0.002
        }
    } else if (freq == 10) {
        for(i = 1; i > 0; i = i-0.01) {
            Aout = i;
            cnt_sample++;
            if (cnt_sample == 5 && index_sample < NSAMPLE) {
                cnt_sample = 0;
                sample[index_sample] = Ain;
                index_sample++;
            }
            ThisThread::sleep_for(1ms); // 10Hz => 1/10/100=0.001
        }
    } else if (freq == 20) {
        for(i = 1; i > 0; i = i-0.02) {
            Aout = i;
            cnt_sample++;
            if (cnt_sample == 5 && index_sample < NSAMPLE) {
                cnt_sample = 0;
                sample[index_sample] = Ain;
                index_sample++;
            }
            ThisThread::sleep_for(1ms); // 20Hz => 1/20/50=0.001
        }
    } else {
        for(i = 1; i > 0; i = i-0.001) {
            Aout = i;
            cnt_sample++;
            if (cnt_sample == 5 && index_sample < NSAMPLE) {
                cnt_sample = 0;
                sample[index_sample] = Ain;
                index_sample++;
            }
            ThisThread::sleep_for(1ms);    // 1Hz => 1/100=0.01
        }
    }
 
}

// print the sample array to screen
void outputSample()
{
    int i;
    for (i = 0; i < NSAMPLE; i++) {
        printf("%5f\r\n", sample[i]);
        ThisThread::sleep_for(100ms);
    }
    for (i = 0; i < NSAMPLE; i++) {
        sample[i] = 0;
    }
}