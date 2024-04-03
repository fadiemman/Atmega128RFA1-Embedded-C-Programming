#ifndef SES_BUTTON_H_
#define SES_BUTTON_H_



/* FUNCTION PROTOTYPES *******************************************************/

// definition type of  function pointers
typedef void (*pButtonCallback)();


// sets rotary call back if rotary button is pressed
void button_setRotaryButtonCallback(pButtonCallback callback);

// sets joystick call back if joystick button is pressed
void button_setJoystickButtonCallback(pButtonCallback callback);
/**
 * Initializes rotary encoder and joystick button
 */
void button_init(bool debouncing);

/** 
 * Get the state of the joystick button.
 */
bool button_isJoystickPressed(void);

/** 
 * Get the state of the rotary button.
 */
bool button_isRotaryPressed(void);

void button_checkState();




#endif /* SES_BUTTON_H_ */
