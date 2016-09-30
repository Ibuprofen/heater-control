
#define POTPIN 14
#define PWRPIN 15
#define VALVEPIN 16

#define OPEN LOW
#define CLOSE HIGH

int potMin = 1;
int potMax = 1023;
int curPot = potMin;
int prevPot = curPot;
int val = curPot;
int diff = 0;
int prevDiff = diff;
int spread = 10;
int timeMax = 4000;

void setup()
{                
  Serial.begin(38400);

  pinMode(PWRPIN, OUTPUT);
  pinMode(VALVEPIN, OUTPUT);

  digitalWrite(PWRPIN, CLOSE);
  digitalWrite(VALVEPIN, CLOSE);

}

void loop()                     
{
  prevPot = curPot;
  curPot = analogRead(POTPIN);
  // ive never seen this return 0, prevent division by 0
  if (curPot == 0) curPot = 1;
  diff = abs(curPot - prevPot);

  // knob stopped turning && last diff was a change && current pot value is different than valve
  if (diff < spread && prevDiff && abs(curPot - val) >= spread) {

    Serial.print("pot is: ");
    Serial.println(curPot);

    // this is what we'll set the "value" to
    val = curPot;

    // todo smarts
    closeValve();
    openValveToVal();


  } else if (diff >= spread) {
    prevDiff = diff;
  }
    
  delay(150);

  
  
}

void openValveToVal() {

  // get percentage of turn of the knob
  float turnPerc = float(val) / float(potMax);

  Serial.print("val: "); Serial.println(val);
  Serial.print("potMax: "); Serial.println(potMax);
  Serial.print("turnPerc: "); Serial.println(turnPerc);
  Serial.print("time float: "); Serial.println(timeMax * turnPerc);
  
  int timeVal = abs(timeMax * turnPerc);

  Serial.print("Opening valve for ms: ");
  Serial.println(timeVal);

  digitalWrite(PWRPIN, OPEN);
  digitalWrite(VALVEPIN, OPEN);
  delay(timeVal);
  digitalWrite(PWRPIN, CLOSE);
  digitalWrite(VALVEPIN, CLOSE);
  

}

void openValve() {
  digitalWrite(PWRPIN, OPEN);
  digitalWrite(VALVEPIN, OPEN);
  delay(timeMax);
  digitalWrite(PWRPIN, CLOSE);
  digitalWrite(VALVEPIN, CLOSE);
}

void closeValve() {
  Serial.println("Closing valve");
  
  digitalWrite(VALVEPIN, OPEN);
  delay(timeMax);
  digitalWrite(VALVEPIN, CLOSE);
}
