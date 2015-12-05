/***
 * Millenium Library - Arduino Workshop
 * 
 * TITLE:        Example #6
 * DESCRIPTION:  Reading input.
 * AUTHOR:       Dario Schor (schor@ieee.org)
 * DATE:         December 5, 2015
 *
 * Implements the game SIMON. The game is played between
 * SIMON (the Arduino) and the user. You start at level 1 and
 * work your way up to MAX_SEQUENCE levels. 
 * 
 * On each level, SIMON will press a sequence of keys corresponding
 * to the level number. These are displayed to the screen with 
 * short delays in between so that the user can try memorizing the 
 * sequence. Then, the user has to press the same keys in order. 
 * If they get them right, they advance to the next level. Else, 
 * the game is over and the user lost. 
 *
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

// Max number of levels (sequences) in the game
#define MAX_SEQUENCE    10

// Each button has a different resistance value
#define MAPPING_KEY_RIGHT       50
#define MAPPING_KEY_UP          200
#define MAPPING_KEY_DOWN        400
#define MAPPING_KEY_LEFT        600
#define MAPPING_KEY_SELECT      800

// Constants to use for each key
#define KEY_RIGHT       0
#define KEY_UP          1
#define KEY_DOWN        2
#define KEY_LEFT        3
#define KEY_SELECT      4

// State machine labels
#define STATE_START      0
#define STATE_TURN_SIMON 1
#define STATE_TURN_USER  2
#define STATE_WINNER     3
#define STATE_LOSER      4

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
int sequence[MAX_SEQUENCE];
int level = 1;

// Analog value corresponding to each key pressed
int keyMapping[NUM_KEYS] = {
  MAPPING_KEY_RIGHT, 
  MAPPING_KEY_UP, 
  MAPPING_KEY_DOWN, 
  MAPPING_KEY_LEFT, 
  MAPPING_KEY_SELECT 
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
  lcd.clear();

  // Serial debugging
  Serial.begin(9600);

  // Set initial state
  state = STATE_START;

  // Initialize sequence
  for(int i = 0; i < MAX_SEQUENCE; i++)
  {
    sequence[i] = INVALID_KEY;
  }
} // End setup()

/***
 * loop()
 * Description:  Implements a state machine for the game SIMON.
 **/
void loop()
{
  int keyPressed = INVALID_KEY;
  int count      = 0;

  Serial.print("STATE: ");
  Serial.println(state);
  
  switch(state)
  {
    case STATE_START:
    {
      // Initialize all the levels (pre-compute full sequence)
      for(count = 0; count < MAX_SEQUENCE; count++)
      {
        sequence[count] = random(0, NUM_KEYS);
        Serial.println(sequence[count]);
      }
      
      lcd.setCursor(0,0);
      lcd.print("Press SELECT to ");
      lcd.setCursor(0,1);
      lcd.print("start the game. ");
      while(keyPressed != KEY_SELECT)
      {
        keyPressed = readKey();
      }

      // Reset to the first level.
      level = 1;

      // Not it is Simon's turn
      state = STATE_TURN_SIMON;
      
      break;
    }

    case STATE_TURN_SIMON:
    {
      // Print a countdown to the next level.
      lcd.clear();
      lcd.print("New level in ");
      lcd.setCursor(0,1);
      lcd.print("3...");
      delay(500);
      lcd.print("2...");
      delay(500);
      lcd.print("1...");
      delay(500);

      /************************************
       * TODO - Implement Simon's turn.
       ***********************************/

      // Once SIMON finished playing, it is the user's turn.
      state = STATE_TURN_USER;
      break;
    }

    case STATE_TURN_USER:
    {
      // Update the LCD info
      updateLcdForLevel(1, INVALID_KEY);
      delay(100);

      /************************************
       * TODO - Implement the User's turn.
       ***********************************/
      break;
    }// End STATE_TURN_USER

    case STATE_WINNER:
    {
      lcd.setCursor(0,0);
      lcd.print("YOU WIN!!!      ");
      lcd.setCursor(0,1);
      lcd.print("                ");
      delay(5000);
      state = STATE_START;
      break;
    } // End STATE_WINNER

    case STATE_LOSER:
    {
      lcd.setCursor(0,0);
      lcd.print("YOU LOSE.       ");
      lcd.setCursor(0,1);
      lcd.print("                ");
      delay(5000);
      state = STATE_START;
      break;
    } // End STATE_LOSER
    
  } // End switch
} // End loop()

/**
 *  Function:     updateLcdForLevel()
 *  Description:  Updates LCD display for the current level.
 *  
 *  The screen should be divided into parts as follows:
 *  --------------------------------------
 *  | L:<levelNum>   T:<currTurn>        |
 *  |------------------------------------|
 *  | 1:<currKey>                        |
 *  --------------------------------------
 *  
 *  Where:
 *    <levelNum> - is the current level taken from the 
 *                 global variable.
 *    <currTurn> - String "YOURS" if it is the user's turn
 *                 or "SIMON" if it is Simon's turn.
 *                 This is obtained from the current state
 *                 global variable.
 *    <currKey>  - String representation (eg, "RIGHT") for 
 *                 the current key presed.
 *  
 *  Parameters:   seqCount - Seq count within a turn
 *                key - Key associated with that sequence count
 *  Return:       None.
 */
void updateLcdForLevel(int seqCount, int key)
{
  // Print level on top row
  lcd.setCursor(0,0);
  lcd.print("L:");
  lcd.print(level);
  lcd.print("      ");

  // Print whose turn it is
  lcd.setCursor(6,0);
  lcd.print("T:");
  if(state == STATE_TURN_SIMON)
  {
    lcd.print("SIMON   ");
  }
  else
  {
    lcd.print("YOURS   ");
  }

  // Print curr sequence & key
  lcd.setCursor(0,1);
  lcd.print(seqCount);
  lcd.print(":");
  if(key != INVALID_KEY)
  {
    lcd.print(msgs[key]);
  }
  lcd.print("        ");
} // End updateLcdForLevel()

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

  // Wait for user to press a key
  while(key >= 1000)
  {
    key = analogRead(READ_BUTTON);
    delay(5);
  }

  // Wait for user to release the key
  temp = analogRead(READ_BUTTON);
  while(temp <= 1000)
  {
    temp = analogRead(READ_BUTTON);
    delay(5);
  }

  // Find which key was pressed
  for(temp = 0; temp < NUM_KEYS; temp++)
  {
    if(key < keyMapping[temp])
    {
       return temp; 
    }
  }

  // No key found
  return INVALID_KEY;
} // End readKey()




