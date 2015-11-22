/* 
	Stampede

	This sketch is used to control an array of relays with the purpose of
	choosing between stomp pedals.

	The setup uses 10 switches
	- 8 for enabling/disabling stomps and memory selection
	- 1 for switching between select/edit mode
	- 1 for bypass (bypassing all pedals)

	There are two modes, select and edit which are controlled by the edit switch.
	- In select mode pressing one of the select switches will load the combination 
	  saved under that switch.
	- In edit mode pressing one of the select switches will enable/disable the respective
	  stomp pedal. A prolonged press of the switch will store the current combination in the 
	  respective memory position.

	To each select switch/pedal corresponds a LED that shows whether the pedal is enabled or not.
	Also the edit and bypass switches have their respective LEDs to show if they are enabled.

*/

#include <EEPROM.h>
#include <stdint.h>

/* The output ports of the stomps*/
#define STOMP_1 12
#define STOMP_2 10
#define STOMP_3 8
#define STOMP_4 6

/* The input ports of the switches*/
#define BTN_1 13
#define BTN_2 11
#define BTN_3 9
#define BTN_4 7

/* Edit button */
#define BTN_FN 5

/* Edit LED */
#define FN_LED 4

#define STOMP_COUNT 4
#define READ_REPEAT 2

#define FN_SWITCH 0
#define FN_EDIT 1

int stomps[] = { STOMP_1, STOMP_2, STOMP_3, STOMP_4 };
int stomp_buttons[] = { BTN_1, BTN_2, BTN_3, BTN_4 };
int current_function = FN_EDIT;

int current_values[STOMP_COUNT] = { 1, 1, 1, 1 };

void setup()
{

  /* add setup code here */
	pinMode(STOMP_1, OUTPUT);
	pinMode(STOMP_2, OUTPUT);
	pinMode(STOMP_3, OUTPUT);
	pinMode(STOMP_4, OUTPUT);
	pinMode(FN_LED, OUTPUT);

	pinMode(BTN_1, INPUT_PULLUP);
	pinMode(BTN_2, INPUT_PULLUP);
	pinMode(BTN_3, INPUT_PULLUP);
	pinMode(BTN_4, INPUT_PULLUP);
	pinMode(BTN_FN, INPUT_PULLUP);


	for (int i = 0; i < STOMP_COUNT; ++i){
		digitalWrite(stomps[i], HIGH);
	}
}

int button_pressing[STOMP_COUNT] = { 0, 0, 0, 0 };

void loop()
{
	int change = 0;

	int fn_val = digitalRead(BTN_FN);

	if (fn_val == 0){
		current_function = !current_function & 1;
		change = 1;
	}

	int button_values[STOMP_COUNT] = { 0, 0, 0, 0 };

	for (int j = 0; j < READ_REPEAT; ++j){
		for (int i = 0; i < STOMP_COUNT; ++i){
			button_values[i] += digitalRead(stomp_buttons[i]);
		}
	}

	for (int i = 0; i < STOMP_COUNT; ++i){
		if (button_values[i] == 0){
			++button_pressing[i];
			if (current_function == FN_SWITCH){
				fn_switch(i, current_values);
			}
			else if (button_pressing[i] > 5){
				fn_store(i, current_values);
				change = 1;
			}
		}
		else if (button_pressing[i] > 0 && current_function == FN_EDIT){
			fn_edit(i, current_values);
			change = 1;
		}
		else{
			button_pressing[i] = 0;
		}
	}

	update(current_values);
	if (change)
		for (int i = 0; i < STOMP_COUNT; ++i){
			button_pressing[i] = 0;
		}
		delay(200);
}

void fn_switch(int val, int * current_values){
	for (int i = 0; i < STOMP_COUNT; ++i){
		current_values[i] = EEPROM.read((val * STOMP_COUNT) + i);
	}
}

void fn_edit(int val, int * current_values){
	current_values[val] = !current_values[val] & 1;
}

void fn_store(int val, int * current_values){
	for (int i = 0; i < STOMP_COUNT; ++i){
		EEPROM.write((val * STOMP_COUNT) + i, (byte)current_values[i]);
	}
	switch_of_leds();
	for (int i = 0; i < 5; ++i){
		digitalWrite(stomps[val], HIGH);
		delay(50);
		digitalWrite(stomps[val], LOW);
		delay(50);
	}
}

void update(int vals[]){
	for (int i = 0; i < STOMP_COUNT; ++i){
		digitalWrite(stomps[i], vals[i] ? HIGH : LOW);
	}
	digitalWrite(FN_LED, current_function ? HIGH : LOW);
}

void switch_of_leds(){
	for (int i = 0; i < STOMP_COUNT; ++i){
		digitalWrite(stomps[i], LOW);
	}
}