
#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <Servo.h>

Servo needleServo;

const int SERVO_PIN = 9;

// ===== Servo Angles (change to match your wheel) =====
const int servoAngles[10] = {
  180,   // Purple
  160,  // Cyan
  140,  // Orange
  120,  // Pink
  100,  // Olive Green
  80, // Dark Blue
  60, // Yellow
  40, // Gray
  20, // Sky Blue
  0  // Red
};

const char *colorNames[10] = {
  "Purple","Cyan","Orange","Pink","Olive",
  "DarkBlue","Yellow","Gray","SkyBlue","Red"
};

// ===== YOUR CALIBRATION VALUES =====
const int refRGB[10][3] = {
  {148,60,46},
  {93,99,61},
  {179,40,35},
  {175,41,38},
  {138,74,42},
  {117,81,56},
  {170,50,34},
  {133,73,48},
  {106,89,59},
  {162,51,41}
};

Adafruit_TCS34725 tcs(
TCS34725_INTEGRATIONTIME_50MS,
TCS34725_GAIN_4X);

int lastColor=-1;

void setup() {
  Serial.begin(9600);

  needleServo.attach(SERVO_PIN);
  needleServo.write(90);

  if(!tcs.begin()){
    Serial.println("Sensor not found");
    while(1);
  }

  Serial.println("Ready");
}

void loop() {

  long sr=0,sg=0,sb=0;

  for(int i=0;i<5;i++){
    uint16_t r,g,b,c;
    tcs.getRawData(&r,&g,&b,&c);

    float sum=r+g+b;
    if(sum==0) sum=1;

    sr+=(r*255.0)/sum;
    sg+=(g*255.0)/sum;
    sb+=(b*255.0)/sum;

    delay(20);
  }

  int R=sr/5;
  int G=sg/5;
  int B=sb/5;

  Serial.print("RGB : ");
  Serial.print(R);
  Serial.print(",");
  Serial.print(G);
  Serial.print(",");
  Serial.println(B);

  int best=0;
  long bestDist=999999;

  for(int i=0;i<10;i++){
    long dr=R-refRGB[i][0];
    long dg=G-refRGB[i][1];
    long db=B-refRGB[i][2];
    long d=dr*dr+dg*dg+db*db;

    if(d<bestDist){
      bestDist=d;
      best=i;
    }
  }

  if(best!=lastColor){
    needleServo.write(servoAngles[best]);
    Serial.print("Detected : ");
    Serial.println(colorNames[best]);
    lastColor=best;
  }

  delay(3000);
}
