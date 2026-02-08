#include <LiquidCrystal.h>

LiquidCrystal lcd(11, 12, 4, 5, 6, 7);

int high_score = 0;
bool button_pressed = false;
int LED1 = 8;
int LED2 = 9;
int LED3 = 10;  // New trap LED on pin 10

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);  // Setup for new trap LED
  randomSeed(analogRead(5));
  lcd.begin(16, 2);
  lcd.print("WHACK-A-MOLE");
  delay(4000);
  lcd.clear();
}

void loop() {
  bool game_over = false;
  int mistakes = 0;
  int score = 0;
  int LED_num;

  // Display high score at the start
  lcd.clear();
  lcd.print("High Score: ");
  lcd.print(high_score);
  delay(1000);
  
  lcd.clear();
  for (int i = 3; i >= 1; i--) {
    lcd.print("New Game in: ");
    lcd.print(i);
    delay(1000);
    lcd.clear();
  }

  // Display the score continuously so the LCD is always updated
  lcd.clear();
  lcd.print("Score: ");
  lcd.print(score);
  lcd.setCursor(0, 1);  // Move to the second line for mistakes display
  lcd.print("Mistakes: ");
  lcd.print(mistakes);

  while (!game_over) {
    LED_num = random(1, 4);  // Randomly select between 1, 2, or 3
    light_LED(LED_num);  // Turn on the chosen LED
    delay(500); // LED stays on for 0.75 second (25% faster than 1 second)

    // Check if the trap LED was lit
    if (LED_num == 3) {  
      if (button_pressed) {
        mistakes++;
      } else {
        score++;
      }
    } else {  // Normal LEDs
      if (button_pressed) {
        score++;
      } else {
        mistakes++;
      }
    }

    // Reset button_pressed for next round
    button_pressed = false;

    // Update the LCD with new score and mistakes
    lcd.setCursor(7, 0);  // Update score on the first line
    lcd.print(score);

    lcd.setCursor(10, 1);  // Update mistakes on the second line
    lcd.print(mistakes);

    // Game over condition if mistakes are too high
    if (mistakes >= 3) {
      game_over = true;
      lcd.clear();
      lcd.print("GAME OVER");
      delay(1000);
      lcd.clear();
      lcd.print("Final Score: ");
      lcd.print(score);
      if (score > high_score) {
        high_score = score;
      }
      delay(1500);
    }
  }
}

void buttonPressed() {
  // Debounce the button press
  static unsigned long lastInterrupttime = 0;
  unsigned long currentInterrupttime = millis();
  if (currentInterrupttime - lastInterrupttime > 250) {
    button_pressed = true;
    lastInterrupttime = currentInterrupttime;
  }
}

void light_LED(int LED_num) {
  if (LED_num == 1) {
    attachInterrupt(digitalPinToInterrupt(2), buttonPressed, RISING);
    digitalWrite(LED1, HIGH);
    delay(500);  // LED stays on for 0.75 second
    digitalWrite(LED1, LOW);
    detachInterrupt(digitalPinToInterrupt(2));
  } 
  else if (LED_num == 2) {
    attachInterrupt(digitalPinToInterrupt(3), buttonPressed, RISING);
    digitalWrite(LED2, HIGH);
    delay(500);  // LED stays on for 0.75 second
    digitalWrite(LED2, LOW);
    detachInterrupt(digitalPinToInterrupt(3));
  }
  else if (LED_num == 3) { // Trap LED
    attachInterrupt(digitalPinToInterrupt(2), buttonPressed, RISING);
    attachInterrupt(digitalPinToInterrupt(3), buttonPressed, RISING);
    digitalWrite(LED3, HIGH);
    delay(500);  // LED stays on for 0.75 second
    digitalWrite(LED3, LOW);
    detachInterrupt(digitalPinToInterrupt(2));
    detachInterrupt(digitalPinToInterrupt(3));
  }
}