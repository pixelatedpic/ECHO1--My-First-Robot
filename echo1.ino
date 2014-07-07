//Ultrasonic bot ECHO1 by Cosmicflu & PixelatedPIC
//PIN configurations
//L293D PIN2->>ARDUINO PIN17
//L293D PIN7->>ARDUINO PIN5
//L293D PIN10->>ARDUINO PIN11
//L293D PIN15->>ARDUINO PIN12
//LEFT MOTOR+ > L293D PIN3
//LEFT MOTOR- > L293D PIN6
//RIGHT MOTOR+ > L293D PIN11
//RIGHT MOTOR- > L293D PIN14

#include <Servo.h> 
Servo myservo;
int servopin = 9;


int sensor_in = A0;
int distance = 0;
long sensor_val;
int vperinch = 9.77;
int threshold =7; //inch
long anVolt, inches, cm;
int sum=0;//Create sum variable so it can be averaged
int avgrange=50;//Quantity of values to average (sample size)
int cur_pos=90;

//h bridge var
int cw_rw  = 5;
int ccw_rw = 6;
int cw_lw   = 3;
int ccw_lw  = 11;
int spd  = 10;
int sp = 0;
int stopspeedy =0;
int speedy  = 200;
int speedy1 = 200;
byte byteRead;
long cur_posy;
int speed = 0;

int GLOBALVAR = 0;

void setup(){
  Serial.begin(9600);
  pinMode(cw_rw, OUTPUT);
  pinMode(ccw_rw, OUTPUT);
  pinMode(cw_lw, OUTPUT);
  pinMode(ccw_lw, OUTPUT);
  pinMode(10, OUTPUT);
  //pinMode(redled, OUTPUT);
  myservo.attach(servopin);
  myservo.write(90);
}

void loop(){ 
 

  double clean_angle=180;
  clean_angle = sensor_cleancheck();

  if ( GLOBALVAR == 0 ) {
    Serial.print("GLOBVAR 3 is: ");
    Serial.println(GLOBALVAR);
    int casetest = 0;
    if (clean_angle > 90) { casetest = 1; }
    if (clean_angle < 90) { casetest = 2; }
    if (clean_angle == 90) { casetest = 3; }
    Serial.print("casetest is: ");
    Serial.println(casetest);
    switch (casetest) {
      case 1:
          Serial.println("Case turn Right . ");
         goReverse(speedy);
         delay(100);
         turnright();
         
           delay(200);
          stopEngine();
          break;
      case 2:
          Serial.println("Case turn Left . ");
         goReverse(speedy);
         delay(100);
           turnleft();
           delay(200);
          stopEngine();
          break;
      case 3:
          Serial.println("Case go Forward . ");
          //analogWrite(10,255);
          goForward();
          break;
    }
  }
  else if ( GLOBALVAR == 2 ) {

    Serial.println(" (180 turn) . ");
    goReverse(speedy1);
    delay(60);
    stopEngine();
    //delay(50);
    //go180();
    //delay(50);
    //stopEngine();
  }
}

double sensor_cleancheck(){
  sensor_val = readsonic();// read the ultrasonic
  Serial.print("sensor_val:: ");
  Serial.println(sensor_val);
  if (sensor_val > threshold) { // means clean
    int angle = cur_pos;
    cur_pos = 90;
    myservo.write(cur_pos); //go back to facing forward again
    GLOBALVAR = 0;
;
    return angle;
  }
  else if (sensor_val <= threshold) { //not clean
    cur_pos = 150;
    myservo.write(cur_pos);
    GLOBALVAR = 1;
    stopEngine();
    sensor_val = readsonic();

    if (sensor_val <= threshold) { //still not clean
      cur_pos = 90;
      myservo.write(cur_pos);
      GLOBALVAR = 1;
      stopEngine();
      sensor_val = readsonic();

      if (sensor_val <= threshold) { //still not clean
        cur_pos = 30;
        myservo.write(cur_pos);
        GLOBALVAR = 2;
        stopEngine();

          return cur_pos;
      }
      return cur_pos;
    }
    return cur_pos;
  }

}  


// function to read the ultrafucking sensor and average the value
int readsonic(){
  for(int i = 0; i < avgrange ; i++) {
    anVolt = analogRead(sensor_in)/2;
    sum += anVolt;
    //delay(10);
  }  
  inches = sum/avgrange;
  sum = 0;
  return inches;
}

void go180() {//take full speed and turn 180

    analogWrite(cw_rw,0);//1
    analogWrite(cw_lw,0);//4  // check the sensors
    delay(1);
    analogWrite(ccw_rw,140);//1
    analogWrite(ccw_lw,140);//4  // check the sensors  
    delay(300); 
    //return;    
}

void goReverse(int speedy1)//take full speed to go forward
{
    
    analogWrite(cw_rw,0);//tesr
    analogWrite(ccw_lw,0);//test
    delay(1);
    analogWrite(cw_lw,85);//3
    analogWrite(ccw_rw,85);//2
    delay(60);
    return;
              
}
void goForward()//take full speed to go reverse
{
    analogWrite(cw_lw,0);//test
    analogWrite(ccw_rw,0);//test
    delay(1);
    analogWrite(cw_rw,81);//1
    analogWrite(ccw_lw,85);//4
    return;
}
//if less thann 90 degree

void turnleft(){
     analogWrite(cw_rw,0);//1
    analogWrite(cw_lw,0);//4  // check the sensors
    delay(1);
    analogWrite(ccw_rw,85);//1
    analogWrite(ccw_lw,85);//4  // check the sensors   
    delay(100);
    return;
    }

void turnright(){
  
     analogWrite(ccw_rw,0);//1
     analogWrite(ccw_lw,0);//4  // check the sensors
     delay(1);
     analogWrite(cw_rw,85);//1
     analogWrite(cw_lw,85);//4  // check the sensors
     delay(100);
     return;

    }
             
void stopEngine()//stop
    {
      digitalWrite(cw_rw,0);//tesr
      digitalWrite(ccw_lw,0);//test
      digitalWrite(cw_lw,0);//test
      digitalWrite(ccw_rw,0);//test
    }
