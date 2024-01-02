#include "mbed.h"
Serial pc(SERIAL_TX, SERIAL_RX);

// Green LED
DigitalOut led1(LED1);
// Blue LED
DigitalOut led2(LED2);
// Red LED
DigitalOut led3(LED3);
// Initialise interrupt when user button pressed
InterruptIn button(USER_BUTTON);

Timeout button_debounce_timeout;
float debounce_time_interval = 0.3;

Ticker cycle_ticker;
float cycle_time_interval = 1;

// Sequence length
const int N = 4;
int button_press_count = 0;
int i = 0;
int j = 0;
int current_LED;
int initial_sequence[3] = {1,2,3};
int final_sequence[N];

void onButtonStopDebouncing(void);

void onButtonPress(void)
{
    final_sequence[button_press_count] = current_LED;
    button_press_count ++;
    button.rise(NULL);
    button_debounce_timeout.attach(onButtonStopDebouncing, debounce_time_interval);
}

void onButtonStopDebouncing(void)
{
    button.rise(onButtonPress);
}

void select_led(int l)
{
    current_LED = l;
    if (l==1) {
            led1 = true;
            led2 = false;
            led3 = false;
    }
    else if (l==2) {
            led1 = false;
            led2 = true;
            led3 = false;
    }
    else if (l==3) {
            led1 = false;
            led2 = false;
            led3 = true;
    }
}

void onCycleTicker(void)
{
    if (button_press_count < N){
        select_led(initial_sequence[i]);
        i = (i+1)%3;
    }
    else {
        select_led(final_sequence[j]);
        j = (j+1)%N;
    }
}

int main()
{
    button.rise(onButtonPress);
    cycle_ticker.attach(onCycleTicker, cycle_time_interval);
}