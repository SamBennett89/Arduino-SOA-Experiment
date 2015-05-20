int button = 3;
int startbutton = 4;
int buttoninterrupt = 1;
int led[] = {8, 9, 10};
int promptled = 11;
int duration_adaptation = 150;     // change this for delay between button press and flash in adaptation phase
int debouncetime = 200;
int duration_ON_OFF = 10;        
int synch_delay = 740;             // change this for delay between flashes in snych task (make sure duration_ON_OFF + synch delay = delay that you want)            
int b = 0;
int i = 0;
int t = 0;
char buffer[5];
boolean expphase = 0;
boolean buttonstate = 1;
boolean startbuttonstate = 1;
unsigned long starttime = 0;
volatile unsigned long ledtime = 0;
volatile unsigned long buttontime = 0;
volatile long elapsedtime = 0;

void setup(){
  pinMode(button, INPUT_PULLUP);
  pinMode(startbutton, INPUT_PULLUP);
  for(i = 0; i < 3; i++){
    pinMode(led[i], OUTPUT);
  }
  pinMode(promptled, OUTPUT);
  Serial.begin(9600);
}

void loop(){
  while(t <= 20){                                            // change this for number of trials in pretest phase
    detachInterrupt(1);
    startbuttonstate = digitalRead(startbutton);
    if(startbuttonstate == 0 && expphase == 0){
      Serial.print("PRE:");
      Serial.print(t+1);
      Serial.println(":");
      synch_phase();
      expphase = 0;
      Serial.println();
      Serial.println();
      detachInterrupt(1);
      t++;
    }
  }
  digitalWrite(promptled, HIGH);
  delay(10000);
  digitalWrite(promptled, LOW);
  while(t > 20 && t <= 40){                                // change this for number of trials in post-test phase (make sure t > ... is equal to t in pre test phase then double this number for t <= ...)
    startbuttonstate = digitalRead(startbutton);
    if(startbuttonstate == 0 && expphase == 0){
      Serial.print("POST:");
      Serial.print(t+1);
      Serial.println(":");
      Serial.println("adapt");
      adaptation_phase();
      expphase = 1;
      Serial.println();
      Serial.println("synch");
      delay(2000);                                        // delay between adaptation and synch phase in posttest
      synch_phase();
      expphase = 0;
      detachInterrupt(1);
      t++;
    }
  }
}

void buttonpress() {
   if((millis() - buttontime) > debouncetime){
    buttontime = millis();
    elapsedtime = buttontime - ledtime;
    if(buttontime == ledtime){
      b = b; 
    }
    else if(elapsedtime < 350){
      Serial.print(elapsedtime);
      Serial.print(";\t");
    }
    else{
      Serial.print(elapsedtime - 750);
      Serial.print(";\t");
    }
  }
}

void adaptation_phase(){
  b = 0;
  buttontime = 0;
  ledtime = 0;
  elapsedtime = 0;
  while(b < 10){                                              // change for number of button presses in adaptation phase
    buttonstate = digitalRead(button);
    if(buttonstate == 0){
      buttontime = millis();
      elapsedtime = buttontime - starttime;
      if(elapsedtime > 1000){
        delay(duration_adaptation);
        digitalWrite(led[1], HIGH);
        delay(duration_ON_OFF);
        digitalWrite(led[1], LOW);
        starttime = millis();
        delay(debouncetime);
        b++; 
      }
      else{
        Serial.print(elapsedtime);
        Serial.print(";\t");
        delay(duration_adaptation);
        digitalWrite(led[1], HIGH);
        delay(duration_ON_OFF);
        digitalWrite(led[1], LOW);
        starttime = millis();
        delay(debouncetime);
        b++;
      }      
    }
  }  
  while(b == 10){                                        // make sure this is qual to number of button presses in adaptation phase
    for(i = 0; i <3; i++){
      digitalWrite(led[i], HIGH);
    }
    delay(duration_ON_OFF);
    for(i = 0; i <3; i++){
      digitalWrite(led[i], LOW);
    }
    digitalWrite(promptled, HIGH);
    delay(2000);
    digitalWrite(promptled, LOW);
    b++;
  }
}

void synch_phase(){
  b = 0;
  elapsedtime = 0;
  buttontime = 0;
  ledtime = millis();
  attachInterrupt(1, buttonpress, FALLING);
  for(b = 0; b < 10; b++){                                  // change b < ... for number of flashes in synch phase
    digitalWrite(led[1], HIGH);
    ledtime = millis();
    delay(duration_ON_OFF);
    digitalWrite(led[1], LOW);
    delay(synch_delay/2);
    elapsedtime = buttontime - ledtime;
    if(elapsedtime < -500){
      Serial.print("-\t");
    }
    delay(synch_delay/2);
  }
  for(i = 0; i <3; i++){
      digitalWrite(led[i], HIGH);
    }
  delay(duration_ON_OFF);
  for(i = 0; i <3; i++){
    digitalWrite(led[i], LOW);
  }
  delay(debouncetime);
}


