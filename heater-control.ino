
#define POTPIN 14
#define PWRPIN 15
#define VALVEPIN 16

#define OPEN LOW
#define CLOSE HIGH

int potMin = 1;
int potMax = 1024;
int curPot = potMin;
int prevPot = curPot;
int val = curPot;
int diff = 0;
int prevDiff = diff;
int spread = 10;
int timeMax = 3800;

void setup()
{                
  Serial.begin(38400);

  pinMode(PWRPIN, OUTPUT);
  pinMode(VALVEPIN, OUTPUT);

  digitalWrite(PWRPIN, CLOSE);
  digitalWrite(VALVEPIN, CLOSE);

  closeValve(timeMax);
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

    closeValve(timeMax);

    val = curPot;

    openValve();


  } else if (diff >= spread) {
    prevDiff = diff;
  }
    
  delay(150);
}

void openValve() {
  
  int timeVal = getTime();

  Serial.print("Opening valve for ms: ");
  Serial.println(timeVal);

  digitalWrite(PWRPIN, OPEN);
  digitalWrite(VALVEPIN, OPEN);
  delay(timeVal);
  digitalWrite(PWRPIN, CLOSE);
  digitalWrite(VALVEPIN, CLOSE);
}

void closeValve(int val) {
  Serial.print("Closing valve, ms: "); Serial.println(val);

  digitalWrite(VALVEPIN, OPEN);
  delay(val);
  digitalWrite(VALVEPIN, CLOSE);
}

// integer value for valve open time (delay)
int getTime() {

  float y = logit(val);

  Serial.print("x:"); Serial.println(val);
  Serial.print("y:"); Serial.println(y);

  return (int) y;
}


// tweak at: https://www.desmos.com/calculator/wfzzwx7bnb
float logit(float x) {
  float a = 0.00;
  float b = potMax;
  float c = 0.00099;

  float result = (timeMax / 2) + ((1/c) * log10((x-a) / (b-a-x)));

  if (result < 0) {
    return 0.00;
  } else if (result > 3800) {
    return 3800.00;
  } else {
    return result;
  }
}

