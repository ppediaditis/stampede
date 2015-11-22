# stampede
Arduino based stomp pedal switcher

	The purpose of this arduino design is to control an array of relays with the result of
	choosing between combinations of stomp pedals.

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
