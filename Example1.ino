/***
 * Millenium Library - Arduino Workshop
 * 
 * TITLE:        Example #1
 * DESCRIPTION:  Reading input.
 * AUTHOR:       Dario Schor (schor@ieee.org)
 * DATE:         December 5, 2015
 *
 * Track the last 4 keys pressed and display them on the display. 
 * The example uses a state machine to read one key per loop iteration. 
 * The display is updated on every iteration. 
 * 
 * Example: 
 *  1. On startup, no keys are pressed, so the display shows 1023 for 
 *     all keys. That's because 1023 is the value read from the analog 
 *     channel button reads when nothing is pressed. 
 *       ---------------------
 *       | 1:1023  | 2:1023  |
 *       ---------------------
 *       | 3:1023  | 4:1023  |
 *       ---------------------
 *  2. The first key pressed will be recorded in Q1. For example, 
 *     if the user presses LEFT=600 (in this example)
 *       ---------------------
 *       | 1:600   | 2:1023  |
 *       ---------------------
 *       | 3:1023  | 4:1023  |
 *       ---------------------
 *  3. The second key pressed will be recorded in Q2. For example, 
 *     if the user presses RIGHT=50 (in this example)
 *       ---------------------
 *       | 1:600   | 2:50    |
 *       ---------------------
 *       | 3:1023  | 4:1023  |
 *       ---------------------
 *  3. The third key pressed will be recorded in Q3. For example, 
 *     if the user presses UP=200 (in this example)
 *       ---------------------
 *       | 1:600   | 2:50    |
 *       ---------------------
 *       | 3:200   | 4:1023  |
 *       ---------------------
 *  4. The fourth key pressed will be recorded in Q4. For example, 
 *     if the user presses UP=200 (in this example)
 *       ---------------------
 *       | 1:600   | 2:50    |
 *       ---------------------
 *       | 3:200   | 4:200   |
 *       ---------------------       
 *  5. The fifth key pressed will be override whatever is in Q1.
 *     For example, press RIGHT=50. 
 *       ---------------------
 *       | 1:50    | 2:50    |
 *       ---------------------
 *       | 3:200   | 4:200   |
 *       ---------------------
 * 
 * In this example, the 4 keys are stored in separate variables. 
 * For example #2, the keys are stored in an array to reduce the 
 * code size.
 *
 * This example uses the LiquidCrystal librady from:
 * http://arduino.cc/en/Reference/LiquidCrystal
 *
 * The display has 2 rows of 16 characters:
 * 
 *     0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15   < COLS
 *   -----------------------------------------------------------------
 *   | QUADRANT #1                   | QUADRANT #2                   |  < ROW-0
 *   -----------------------------------------------------------------
 *   | QUADRANT #3                   | QUADRANT #4                   |  < ROW-1
 *   -----------------------------------------------------------------
 **/ 

#include <LiquidCrystal.h>

// Size of screen
#define NUM_ROWS        2
#define NUM_COLS        16

// Custom pinout for the shield used in the workshop
#define LCD_RS_PIN      8
#define LCD_RW_PIN      13
#define LCD_EN_PIN      9
#define LCD_D4_PIN      4
#define LCD_D5_PIN      5
#define LCD_D6_PIN      6
#define LCD_D7_PIN      7

// Analog 0 reads the input from the buttons
#define READ_BUTTON     0
#define NUM_KEYS        5
#define INVALID_KEY     -1

// Each button has a different resistance value
#define KEY_RIGHT       50
#define KEY_UP          200
#define KEY_DOWN        400
#define KEY_LEFT        600
#define KEY_SELECT      800

// Create instance of LiquidCrystal object
LiquidCrystal lcd(
  LCD_RS_PIN,   // Register Select
  LCD_RW_PIN,   // H-Read, L-Write
  LCD_EN_PIN,   // Enable (data strobe, active high)
  LCD_D4_PIN,   // Data pins
  LCD_D5_PIN, 
  LCD_D6_PIN, 
  LCD_D7_PIN
  );
  
int state = 0;  // Current state
int key1 = 1023;   // Keys pressed
int key2 = 1023;
int key3 = 1023;
int key4 = 1023;
int temp = 0;   // Temp variable

/***
 * setup()
 * Description:  Initialize the LCD.
 **/
void setup()
{
  // Clear the LCD screen
  lcd.clear(); 
  
  // Set the dimensions of the LCD screen.
  lcd.begin(NUM_COLS, NUM_ROWS);

  // Serial debugging
  Serial.begin(9600);

  // Set initial state
  state = 1;

  // Update quadrant 1
  lcd.setCursor(0,0);
  lcd.print("1:      ");
  lcd.setCursor(2,0);
  lcd.print(key1);

  /****************************************
   * Insert code to update quadrants 2-4.
   ****************************************/
}

/***
 * loop()
 * Description:  Read a key and print a msg to the LCD.
 **/
void loop()
{
  switch(state)
  {
    /**
     *  Read 1st key.
     */
    case 1:
    {
      // Read a key pressed
      key1 = analogRead(READ_BUTTON);
      while(key1 >= 1000)
      {
        key1 = analogRead(READ_BUTTON);
        delay(10);
      }

      // Wait to release the key
      /****************************************
       * Insert code to handle the key being released
       ****************************************/
      // Advanced to the next key
      state = 2;
      break;
    }
    
    /****************************************
     * Repeat for other states.
     ****************************************/
  }

  /****************************************
   * Update display
   ****************************************/
  
  // Print the state to the serial log for debugging
  Serial.print(state);
}

	
