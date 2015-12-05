/***
 * Millenium Library - Arduino Workshop
 * 
 * TITLE:        Example #3
 * DESCRIPTION:  Reading input.
 * AUTHOR:       Dario Schor (schor@ieee.org)
 * DATE:         December 5, 2015
 *
 * Track the last 4 keys pressed and display them on the display. 
 * The example uses a state machine to read one key per loop iteration. 
 * The display is updated on every iteration. 
 * 
 * Use arrays to store the keys pressed. Since we are also combining it with
 * functions, we now can translate some of the key presses and abstract the 
 * details of the hardware implementation.
 * 
 * Example: 
 *  1. On startup, no keys are pressed, so the display shows empty strings.
 *       ---------------------
 *       | 1:      | 2:      |
 *       ---------------------
 *       | 3:      | 4:      |
 *       ---------------------
 *  2. The first key pressed will be recorded in Q1. For example, 
 *     if the user presses LEFT (in this example)
 *       ---------------------
 *       | 1:LEFT  | 2:      |
 *       ---------------------
 *       | 3:      | 4:      |
 *       ---------------------
 *  3. The second key pressed will be recorded in Q2. For example, 
 *     if the user presses RIGHT (in this example)
 *       ---------------------
 *       | 1:LEFT  | 2:RIGHT |
 *       ---------------------
 *       | 3:      | 4:      |
 *       ---------------------
 *  3. The third key pressed will be recorded in Q3. For example, 
 *     if the user presses UP (in this example)
 *       ---------------------
 *       | 1:LEFT  | 2:RIGHT |
 *       ---------------------
 *       | 3:UP    | 4:      |
 *       ---------------------
 *  4. The fourth key pressed will be recorded in Q4. For example, 
 *     if the user presses UP (in this example)
 *       ---------------------
 *       | 1:LEFT  | 2:RIGHT |
 *       ---------------------
 *       | 3:UP    | 4:UP    |
 *       ---------------------       
 *  5. The fifth key pressed will be override whatever is in Q1.
 *     For example, press RIGHT=50. 
 *       ---------------------
 *       | 1:RIGHT | 2:RIGHT |
 *       ---------------------
 *       | 3:UP    | 4:UP    |
 *       ---------------------
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
int keys[4] = {INVALID_KEY, INVALID_KEY, INVALID_KEY, INVALID_KEY};

// Analog value corresponding to each key pressed
int keyMapping[NUM_KEYS] = {
  KEY_RIGHT, 
  KEY_UP, 
  KEY_DOWN, 
  KEY_LEFT, 
  KEY_SELECT 
  };

// Define the message to display for each button pressed
char msgs[NUM_KEYS][6] = {
  "RIGHT",
  "UP   ",
  "DOWN ",
  "LEFT ",
  "SEL  "
  };

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
  state = 0;

  // Update display with initial settings.
  updateDisplay();
}

/***
 * loop()
 * Description:  Read a key and print a msg to the LCD.
 **/
void loop()
{
  // Read a key pressed
  keys[state] = readKey();

  // Advance to the next state.
  state = (state + 1) % 4;

  // Update display with current readings
  updateDisplay();
  
  // Print the state to the serial log for debugging
  Serial.print(state);
}

/**
 *  Function:     readKey()
 *  Description:  Read a key and return the value read.
 *  Parameters:   None
 *  Return:       Returns value of key pressed.
 */
int readKey()
{
  // Read default value for the key
  int key  = analogRead(READ_BUTTON);
  int temp = 0; // Used to wait for the key to be released
  
  while(key >= 1000)
  {
    key = analogRead(READ_BUTTON);
    delay(10);
  }

  // Wait to release the key
  temp = analogRead(READ_BUTTON);
  while(temp <= 1000)
  {
    temp = analogRead(READ_BUTTON);
    delay(10);
  }

  return convertToBtnId(key);
}

/**
 * Function:      updateDisplay()
 * Description:   Updates values for all 4 quadrants.
 * Parameters:    None.
 * Return:        None.
 */
void updateDisplay()
{
  // Update quadrant 1
  lcd.setCursor(0,0);
  lcd.print("1:      ");
  lcd.setCursor(2,0);
  if(INVALID_KEY != keys[0])
  {
    lcd.print(msgs[keys[0]]);
  }
  
  // Update quadrant 2
  lcd.setCursor(8,0);
  lcd.print("2:      ");
  lcd.setCursor(10,0);
  if(INVALID_KEY != keys[1])
  {
    lcd.print(msgs[keys[1]]);
  }

  // Update quadrant 3
  lcd.setCursor(0,1);
  lcd.print("3:      ");
  lcd.setCursor(2,1);
  if(INVALID_KEY != keys[2])
  {
    lcd.print(msgs[keys[2]]);
  }

  // Update quadrant 4
  lcd.setCursor(8,1);
  lcd.print("4:      ");
  lcd.setCursor(10,1);
  if(INVALID_KEY != keys[3])
  {
    lcd.print(msgs[keys[3]]);
  }
}

/**
 * Function:      convertToBtnId()
 * Description:   Convert the button pressed to the
 *                string LEFT, RIGHT, UP, DOWN, or SEL.
 * Parameters:    None.
 * Return:        Int representing key pressed (index into array).
 */
int convertToBtnId(int key)
{
  int index;
  
  // Find which key was pressed
  for(index = 0; index < NUM_KEYS; index++)
  {
    if(key < keyMapping[index])
    {
       return index; 
    }
  }
   
  // No key found
  return INVALID_KEY;  
}

