
// pin numbers
const int buttonPin[]       = {2, 3, 4};
const int ledPin[]          = {8, 9, 10};
const int promptLedPin      = 11;
const int SCRPin            = 12;

// order of conditions
int condition[]             = {1, 0, 2, 0, 2, 1, 1, 2, 0};

// prediciton accuracy arrays
boolean accuracyHistory[]   = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
boolean accuracy[][15]      = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1}, 
                                {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1}, 
                                {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1} };

// delay times
int durationAdaptation      = 150;
int durationOnOff           = 10;
int durationDebounce        = 200;
int durationTrialEnd        = 10000;
int promptRange             = 1000;
int promptMinimum           = 1000;

// number of trials per cond / button presses per trial
int numTrials               = 15;
int numButtonPress          = 5;

// timers
unsigned long lastButtonTime    = 0;
unsigned long elapsedTime       = 0;

// counters
int b  = 0;
int t  = 0;
int c = 0;
int id = 0;

// array identifier
int acc   = 0;

// buttonstate checkers
boolean buttonState        = true;
int buttonNumber           = 0;
int buttonNumberIncorrect  = 0;

void setup() {
  for(int i = 0; i < 3; i++){
    pinMode(buttonPin[i], INPUT_PULLUP);
    pinMode(ledPin[i], OUTPUT); 
  }
  pinMode(promptLedPin, OUTPUT);
  pinMode(SCRPin, OUTPUT);
  Serial.begin(9600);
}


void loop() {
  for(c = 0; c < 9; c++){
    trial();
  }
}

void trial() {
  t = 0;
  while(t < numTrials){
    b = 0;
    while(b <= numButtonPress){
      buttonRead();
    }
    while(b == numButtonPress + 1){
      digitalWrite(promptLedPin, HIGH);
      delay(promptMinimum + random(promptRange));
      digitalWrite(promptLedPin, LOW);
      b++; 
    }
  t++;
  id++;  
  }
  while(t == numTrials){
    for(int i = 0; i < 5; i++){
      delay(durationTrialEnd/10);
      digitalWrite(promptLedPin, HIGH);
      delay(durationTrialEnd/10);
      digitalWrite(promptLedPin, LOW);
    }
    for(int i = 0; i < 15; i++){
      accuracyHistory[i] = 0;
    }
    t++;  
  }
 t = 0; 
}

void buttonRead() {
  for(buttonNumber = 0; buttonNumber < 3; buttonNumber++) {
    buttonState = digitalRead(buttonPin[buttonNumber]);
    if(buttonState == false){
      buttonPress();
    } 
  }
}

void buttonPress() {
  printTime();
  lastButtonTime = millis();
  if(b == 0){
    Serial.print(id+1);
    Serial.print(",");
    Serial.print(t+1); 
    Serial.print(",");
    Serial.print(condition[c]+1);
    Serial.print(",");
  }
  if(b < numButtonPress){
    delay(durationAdaptation);
    for(int i = 0; i < 3; i++){
      digitalWrite(ledPin[i], HIGH);
    }
    delay(durationOnOff);
    for(int i = 0; i < 3; i++){
      digitalWrite(ledPin[i], LOW);
    }
    delay(durationDebounce);
  }
  if(b == numButtonPress){
    selectAcc();
    if(accuracy[condition[c]][acc] == 1){
      Serial.println(accuracy[condition[c]][acc]);
      digitalWrite(SCRPin, HIGH);
      digitalWrite(ledPin[buttonNumber], HIGH);
      delay(durationOnOff);
      digitalWrite(ledPin[buttonNumber], LOW);
      delay(durationDebounce);
      digitalWrite(SCRPin, LOW);
      accuracyHistory[acc] = 1; 
    }
    if(accuracy[condition[c]][acc] == 0) {
      do{
        buttonNumberIncorrect = random(3);
      }
      while(buttonNumberIncorrect == buttonNumber);
      Serial.println(accuracy[condition[c]][acc]);
      digitalWrite(SCRPin, HIGH);
      digitalWrite(ledPin[buttonNumberIncorrect], HIGH);
      delay(durationOnOff);
      digitalWrite(ledPin[buttonNumberIncorrect], LOW);
      delay(durationDebounce);
      digitalWrite(SCRPin, LOW);
      accuracyHistory[acc] = 1; 
    }
  }
  b++;
}

void selectAcc() {
  do{
    acc = random(15);
  }
  while(accuracyHistory[acc] == 1);
}

void printTime(){
  if(b > 0){
    elapsedTime = millis() - lastButtonTime;
    Serial.print(elapsedTime);
    Serial.print(","); 
  }
}
