 #include "pitches.h"


const int NUM_LEDS = 3;
const int speakerPin = 10;

int buttonPin[] = {3, 2, 4};
int ledPin[] = {13, 8, 9};
int tones[] = {NOTE_C4, NOTE_A4, NOTE_G3};

int buttonState[] = {HIGH, HIGH, HIGH};
int lastButtonState[] = {LOW, LOW, LOW};
boolean buttonOn[] = {false, false};

int sequence[] = {0, 1, 0, 2, 2, 1, 0};
int max_level = 7;
int level = 1;

void setup() {
  randomSeed(analogRead(0));
  
  // initialize the LED pin as an output:
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(ledPin[i], OUTPUT);      
    pinMode(buttonPin[i], INPUT);     
  }
  
  Serial.begin(9600);
}

void generateSequence() {
  for (int i = 0; i < max_level; i++) {
    int randLed = random(0, 3);
    sequence[i] = randLed;
  }
}


void loop(){

  boolean wrong = false;
  generateSequence();

  while (!wrong && level <= max_level) {
    playSequence(level);
    wrong = inputFromUser(level);
    level++;
  }
  
  if (wrong) {
    playLoosingSequence();
  }
  else {
    playWinningSequence();
  }
  level = 1;
}
 
void playSequence(int level) {
  for (int i = 0; i < level; i++) {
    int ledNum = sequence[i];
    tone(speakerPin, tones[ledNum]);
    digitalWrite(ledPin[ledNum], HIGH);
    delay(500 - 30 * level);
    digitalWrite(ledPin[ledNum], LOW);
    noTone(speakerPin);
    delay(200  - 10 * level); 
  }
}

boolean inputFromUser(int level) {
  boolean wrong = false;
  int seqIndex = 0;
  
  while (!wrong && seqIndex < level) {
   // read the state of the pushbutton value:
    for (int i = 0; i < NUM_LEDS; i++) {
      buttonState[i] = digitalRead(buttonPin[i]); 
      if (buttonState[i] == HIGH) {
        if (i == sequence[seqIndex]) {
          play(ledPin[i], tones[i]);
          seqIndex++;
        }
        else {
          wrong = true;
          Serial.println("WRONG");
        }
      }
    }
  }
  return wrong;
}

void play(int ledNum, int toneValue) {
  tone(speakerPin, toneValue);
  digitalWrite(ledNum, HIGH);
  delay(500);
  digitalWrite(ledNum, LOW);
  noTone(speakerPin);
  delay(500);
}

void playWinningSequence() {
  int mc_melody[] = {           NOTE_C4, NOTE_C4, NOTE_C4, NOTE_G3, NOTE_A3, NOTE_A3, NOTE_G3, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4, NOTE_G3, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_G3, NOTE_A3, NOTE_A3, NOTE_G3, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4};
  int mc_noteDurations[] = {    4,       4,       4,       4,       4,       4,       2,       4,       4,       4,       4,       2,       4,       4,       4,       4,       4,       4,       4,       2,       4,       4,       4,       4,       2  };

  for (int thisNote = 0; thisNote < 25; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/mc_noteDurations[thisNote];
    tone(speakerPin, mc_melody[thisNote],noteDuration);

    int randLed = random(0, 3);
    digitalWrite(ledPin[randLed], HIGH);


    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    digitalWrite(ledPin[randLed], LOW);
    // stop the tone playing:
    noTone(speakerPin);
  }
  
  for (int t = 0; t < 5; t++) {
    for (int i = 0; i < level; i++) {
      digitalWrite(ledPin[sequence[i]], HIGH);
      delay(100);
      digitalWrite(ledPin[sequence[i]], LOW);
      delay(200);
    }
  }
}

void playLoosingSequence() {
  for (int t = 0; t < 5; t++) {
    for (int i = 0; i < level; i++) {
      digitalWrite(ledPin[sequence[i]], HIGH);
    }
    delay(100);
    for (int i = 0; i < level; i++) {
      digitalWrite(ledPin[sequence[i]], LOW);
    }
    delay(100);
  }
}



