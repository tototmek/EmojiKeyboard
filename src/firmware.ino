#include <BleKeyboard.h>

#define COLUMN_COUNT 5
#define COLUMN_0 5
#define COLUMN_1 18
#define COLUMN_2 19
#define COLUMN_3 22
#define COLUMN_4 23

#define ROW_COUNT 4
#define ROW_0 27
#define ROW_1 14
#define ROW_2 12
#define ROW_3 13

#define COLUMNS_TIMES_ROWS 20

int COLUMNS[5] = {COLUMN_0, COLUMN_1, COLUMN_2, COLUMN_3, COLUMN_4};
int ROWS[4] = {ROW_0, ROW_1, ROW_2, ROW_3};
bool prevState[COLUMNS_TIMES_ROWS];

BleKeyboard bleKeyboard("Emoji Keyboard", "Tomek Zebrowski", 100);
char *emojiCodes[] = {"2764", "1f624", "1f928", "270c", "1f525",
                      "1f975", "1f633", "1f97a", "1f449", "1f4a6", 
                      "1f976", "1f629", "1f610", "1f448", "2728", 
                      "1f922", "1f913", "1f614", "1f44a", "1f4af"};

void setup() {
  pinMode(COLUMN_0, OUTPUT);
  pinMode(COLUMN_1, OUTPUT);
  pinMode(COLUMN_2, OUTPUT);
  pinMode(COLUMN_3, OUTPUT);
  pinMode(COLUMN_4, OUTPUT);
  pinMode(ROW_0, INPUT_PULLDOWN);
  pinMode(ROW_1, INPUT_PULLDOWN);
  pinMode(ROW_2, INPUT_PULLDOWN);
  pinMode(ROW_3, INPUT_PULLDOWN);
  bleKeyboard.setDelay(1);
  bleKeyboard.begin();
}

void loop() {
  scan_keys();
  delay(20);
}

void scan_keys() {
  if(!bleKeyboard.isConnected()) return;
  for (int x=0; x<COLUMN_COUNT; x++) {
    digitalWrite(COLUMNS[x], HIGH);
    delay(1);
    for (int y=0; y<ROW_COUNT; y++) {
      if (digitalRead(ROWS[y])) {
        if (!prevState[x + COLUMN_COUNT * y]) {
            writeUnicode(emojiCodes[x + COLUMN_COUNT * y]);
            if (x==0 && y==0) writeUnicode("fe0f"); // A bit weird, solution for red heart emoji (u+2764 u+fe0f)
            prevState[x + COLUMN_COUNT * y] = true;
            delay(90); // To avoid weird situations when two keys are pressed simultanousely
        }
      } else {
        prevState[x + COLUMN_COUNT * y] = false;
      }
    }
    digitalWrite(COLUMNS[x], LOW);
  } 
}

void writeUnicode(char* code) {
    bleKeyboard.press(KEY_LEFT_CTRL);
    bleKeyboard.press(KEY_LEFT_SHIFT);
    bleKeyboard.press('u');
    bleKeyboard.releaseAll();
    bleKeyboard.print(code);
    bleKeyboard.write(KEY_RETURN);
}
