typedef enum
{
  STOP=0,
  FORWARD=1,
  BACKWARD=2,
  LEFT=3,
  RIGHT=4
} TDirection;
volatile TDirection dir = STOP;

/*
 * Vincent's configuration constants
 */

// Number of ticks per revolution from the 
// wheel encoder.

#define COUNTS_PER_REV      195

// Wheel circumference in cm.
// We will use this to calculate forward/backward distance traveled 
// by taking revs * WHEEL_CIRC

#define WHEEL_CIRC          20.42

// Motor control pins. You need to adjust these till
// Vincent moves in the correct direction
#define LF                  6   // Left forward pin
#define LR                  5   // Left reverse pin
#define RF                  10  // Right forward pin
#define RR                  11  // Right reverse pin


/*
 *    Vincent's State Variables
 */
// PI, for calculating turn circumference
#define PI 3.141592654

// Vincent's length and breadth in cm
#define VINCENT_LENGTH   16
#define VINCENT_BREADTH  6

// Vincent's diagonal. We compute and store this once
// since it is expensive to compute and really doesn't change.
float vincentDiagonal = 0.0;

// Vincent's turning circumference, calculated once
float vincentCirc = 0.0;


// Store the ticks from Vincent's left and
// right encoders for moving forward and backwards.
volatile unsigned long leftForwardTicks; 
volatile unsigned long rightForwardTicks;
volatile unsigned long leftReverseTicks;
volatile unsigned long rightReverseTicks;

// Left and right encoders ticks for turning
volatile unsigned long leftForwardTicksTurns; 
volatile unsigned long rightForwardTicksTurns;
volatile unsigned long leftReverseTicksTurns;
volatile unsigned long rightReverseTicksTurns;

// Store the revolutions on Vincent's left
// and right wheels
volatile unsigned long leftRevs;
volatile unsigned long rightRevs;

// Forward and backward distance traveled
volatile unsigned long forwardDist;
volatile unsigned long reverseDist;

// Variables to keep track of whether we have moved a commanded distance
unsigned long deltaDist;
unsigned long newDist;
unsigned long deltaTicks;
unsigned long targetTicks;


/*
 * Setup and start codes for external interrupts and 
 * pullup resistors.
 * 
 */
// Enable pull up resistors on pins 2 and 3
void enablePullups()
{
  // Use bare-metal to enable the pull-up resistors on pins
  // 2 and 3. These are pins PD2 and PD3 respectively.
  // We set bits 2 and 3 in DDRD to 0 to make them inputs. 
  DDRD &= 0b11110011;
  PIND |= 0b00001100;
}

// Functions to be called by INT0 and INT1 ISRs.
// we assume that when dir is FORWARD and dir is BACKWARD the number of clicks 
// by the left and right encoders is similar, and we update forwardDist and reverseDist 
// only in leftISR, and not in rightISR
void leftISR()
{
  if (dir == FORWARD) {
    leftForwardTicks++;
    forwardDist = (unsigned long) ((float) leftForwardTicks / COUNTS_PER_REV * WHEEL_CIRC);
  }
  else if (dir == BACKWARD) {
    leftReverseTicks++;
    reverseDist = (unsigned long) ((float) leftReverseTicks / COUNTS_PER_REV * WHEEL_CIRC);
  }
  else if (dir == LEFT) 
    leftReverseTicksTurns++;
  else if (dir == RIGHT) 
    leftForwardTicksTurns++;


  //  Serial.print("LEFT: ");
  //  Serial.println((float) leftTicks / COUNTS_PER_REV * WHEEL_CIRC);
}

void rightISR()
{
  if (dir == FORWARD) 
    rightForwardTicks++;
  else if (dir == BACKWARD)
    rightReverseTicks++;
  else if (dir == LEFT)
    rightForwardTicksTurns++;
  else if (dir == RIGHT)
    rightReverseTicksTurns++;

  // Serial.print("RIGHT: ");
  // Serial.println((float) rightTicks / COUNTS_PER_REV * WHEEL_CIRC);
}

// Set up the external interrupt pins INT0 and INT1
// for falling edge triggered. Use bare-metal.
void setupEINT()
{
  // Use bare-metal to configure pins 2 and 3 to be
  // falling edge triggered. Remember to enable
  // the INT0 and INT1 interrupts.
  cli();
  EICRA |= 0b00001010;
  EIMSK |= 0b00000011;
  sei();
}

// Implement the external interrupt ISRs below.
// INT0 ISR should call leftISR while INT1 ISR
// should call rightISR.
ISR(INT0_vect) {
  leftISR();
}

ISR(INT1_vect) {
  rightISR();
}

// Set up the serial connection. For now we are using 
// Arduino Wiring, you will replace this later
// with bare-metal code.
void setupSerial()
{
  // To replace later with bare-metal.
//  Serial.begin(9600); // alr called in loop
}

// Start the serial connection. For now we are using
// Arduino wiring and this function is empty. We will
// replace this later with bare-metal code.

void startSerial()
{
  // Empty for now. To be replaced with bare-metal code
  // later on.

}

// Convert percentages to PWM values
int pwmVal(float speed)
{
  if(speed < 0.0)
    speed = 0;

  if(speed > 100.0)
    speed = 100.0;

  return (int) ((speed/100.0) * 255.0);
}

// Move Vincent forward "dist" cm at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// move forward at half speed.
// Specifying a distance of 0 means Vincent will
// continue moving forward indefinitely.

void forward(float dist, float speed)
{
  // Code to tell us how far to move 
  if (dist==0)
    deltaDist = 999999;
  else 
    deltaDist = dist;

  newDist = forwardDist + deltaDist;  
  
  dir = FORWARD;

  int val = pwmVal(speed);

  // For now we will ignore dist and move
  // forward indefinitely. We will fix this
  // in Week 9.

  // LF = Left forward pin, LR = Left reverse pin
  // RF = Right forward pin, RR = Right reverse pin
  // This will be replaced later with bare-metal code.

  analogWrite(LF, val);
  analogWrite(RF, val);
  analogWrite(LR,0);
  analogWrite(RR, 0);
}

// Reverse Vincent "dist" cm at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// reverse at half speed.
// Specifying a distance of 0 means Vincent will
// continue reversing indefinitely.

void reverse(float dist, float speed)
{
  // code to tell us how har to move
  if(dist == 0)
     deltaDist = 999999;
   else
     deltaDist = dist;

    newDist = reverseDist + deltaDist;

    dir = BACKWARD;

  int val = pwmVal(speed);

  // For now we will ignore dist and 
  // reverse indefinitely. We will fix this
  // in Week 9.

  // LF = Left forward pin, LR = Left reverse pin
  // RF = Right forward pin, RR = Right reverse pin
  // This will be replaced later with bare-metal code.
  analogWrite(LR, val);
  analogWrite(RR, val);
  analogWrite(LF, 0);
  analogWrite(RF, 0);
}

unsigned long computeDeltaTicks(float ang)
{
  //we will assume that angular distance moved = linear distance moved in one wheel
  //revolution. This is probably incorrect but simplifies calculation.
  //# of wheel revs to make one full 360 turn is vincentCirc / WHEEL_CIRC
  //This is for 360 degrees. For ang drgrees it will be (ang * vincentCirc)/(360* WHEEL_CIRC)
  //To convert to ticks, we multiply by COUNTS_PER_REV.

 unsigned long ticks = (unsigned long)((ang * vincentCirc * COUNTS_PER_REV)/(360.0* WHEEL_CIRC));

 return ticks;
}

// Turn Vincent left "ang" degrees at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// turn left at half speed.
// Specifying an angle of 0 degrees will cause Vincent to
// turn left indefinitely.
void left(float ang, float speed)
{
  dir = LEFT;

  if(ang == 0)
    deltaTicks = 9999999;
  else
  {
    deltaTicks = computeDeltaTicks(ang);
  }

  targetTicks = leftReverseTicksTurns + deltaTicks;
  
  int val = pwmVal(speed);

  // For now we will ignore ang. We will fix this in Week 9.
  // We will also replace this code with bare-metal later.
  // To turn left we reverse the left wheel and move
  // the right wheel forward.
  analogWrite(LR, val);
  analogWrite(RF, val);
  analogWrite(LF, 0);
  analogWrite(RR, 0);
}

// Turn Vincent right "ang" degrees at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// turn left at half speed.
// Specifying an angle of 0 degrees will cause Vincent to
// turn right indefinitely.
void right(float ang, float speed)
{
  dir = RIGHT;

if(ang == 0)
  deltaTicks = 9999999;
  else
  {
    deltaTicks = computeDeltaTicks(ang);
  }

  targetTicks = rightReverseTicksTurns + deltaTicks;
  
  int val = pwmVal(speed);

  // For now we will ignore ang. We will fix this in Week 9.
  // We will also replace this code with bare-metal later.
  // To turn right we reverse the right wheel and move
  // the left wheel forward.
  analogWrite(RR, val);
  analogWrite(LF, val);
  analogWrite(LR, 0);
  analogWrite(RF, 0);
}

// Stop Vincent. To replace with bare-metal code later.
void stop()
{
  dir = STOP;

  analogWrite(LF, 0);
  analogWrite(LR, 0);
  analogWrite(RF, 0);
  analogWrite(RR, 0);
}

void sendStatus() {
  // Implement code to send back a packet containing key
  // information like leftTicks, rightTicks, leftRevs, rightRevs
  // forwardDist and reverseDist
  // Use the params array to store this information, and set the
  // packetType and command files accordingly, then use sendResponse
  // to send out the packet. See sendMessage on how to use sendResponse.
  //
  Serial.println("------- VINCENT STATUS REPORT -------\n");
  Serial.print("Left Forward Ticks:\t\t"); Serial.println(leftForwardTicks);
  Serial.print("Right Forward Ticks:\t\t"); Serial.println(rightForwardTicks);
  Serial.print("Left Reverse Ticks:\t\t"); Serial.println(leftReverseTicks);
  Serial.print("Right Reverse Ticks:\t\t"); Serial.println(rightReverseTicks);
  Serial.print("Left Forward Ticks Turns:\t"); Serial.println(leftForwardTicksTurns);
  Serial.print("Right Forward Ticks Turns:\t"); Serial.println(rightForwardTicksTurns);
  Serial.print("Left Reverse Ticks Turns:\t"); Serial.println(leftReverseTicksTurns);
  Serial.print("Right Reverse Ticks Turns:\t"); Serial.println(rightReverseTicksTurns);
  Serial.print("Forward Distance:\t\t"); Serial.println(forwardDist);
  Serial.print("Reverse Distance:\t\t"); Serial.println(reverseDist);
  Serial.println("\n---------------------------------------\n");
  }

/*
 * Vincent's setup and run codes
 * 
 */

// Clears all our counters
void clearCounters()
{
  leftForwardTicks=0;
  rightForwardTicks=0;
  leftReverseTicks=0;
  rightReverseTicks=0;
  leftForwardTicksTurns = 0; 
  rightForwardTicksTurns = 0;
  leftReverseTicksTurns = 0;
  rightReverseTicksTurns = 0;

  leftRevs=0;
  rightRevs=0;
  forwardDist=0;
  reverseDist=0; 
}

// Clears one particular counter
void clearOneCounter(int which)
{
  clearCounters();
}
// Intialize Vincet's internal states

void initializeState()
{
  clearCounters();
}

String cmd;
volatile int dist,speed;
bool flag = false;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  vincentDiagonal = sqrt((VINCENT_LENGTH * VINCENT_LENGTH) + (VINCENT_BREADTH * VINCENT_BREADTH));
  vincentCirc = PI * vincentDiagonal;

  cli();
  setupEINT();
  //setupSerial();
  //startSerial();
//  setupMotors();
//  startMotors();
  enablePullups();
  initializeState();
  sei();
}

void loop() {
  cmdFromPi();
  comToAr();
  if(deltaDist > 0)
      {
        if(dir == FORWARD)
        {
          if(forwardDist > newDist)
          {
              deltaDist = 0;
              newDist = 0;
              stop();
          }
        }
        else
             if(dir == STOP)
             {
                  deltaDist = 0;
                  newDist = 0;
                  stop();
             }
      }

      if(deltaTicks>0)
      {
        if(dir==LEFT){
          if(leftReverseTicksTurns>=targetTicks){
            deltaTicks=0;
            targetTicks=0;
            stop();
          }
        }
        else
          if(dir==STOP)
          {
            deltaTicks=0;
            targetTicks=0;
            stop();
          }
        }    
}
void cmdFromPi(){
        if (Serial.available() > 0) {
                flag = true;
                // read the incoming byte:
                String incomingByte = Serial.readString();
                String temp = incomingByte.substring(0,1);
                Serial.println(temp);
                cmd = temp;
                if(temp == "S" || temp =="s" ||temp =="G" || temp =="g"){
                  return;
                }
                int j=2,k=2;
                for(int i=0;i<2;i++){
                  while(1){
                    if(incomingByte.charAt(j) == ','){
                      break;
                    }
                    j = 1 + j;
                  }
                  temp = incomingByte.substring(k,j);
                  int num = temp.toInt();
                  if(i == 0){
                    dist = num;
                  }else if(i == 1){
                    speed = num;
                  }
                  Serial.println(num);
                  j = j + 1;
                  k=j;
                }
                
                //Serial.println(incomingByte);
        }

}

void comToAr(){
  if(flag == true){
    if(cmd == "s" || cmd == "S"){
        stop();
    }else if(cmd == "G" || cmd == "g" ){
        sendStatus();
    }else if(cmd == "F" || cmd == "f"){
        forward((float) dist, (float) speed);
    }else if(cmd == "B" || cmd == "b"){
        reverse((float) dist, (float) speed);
    }else if(cmd == "R" || cmd == "r"){
        right((float) dist, (float) speed);
    }else if(cmd == "L" || cmd == "l"){
       left((float) dist, (float) speed);
    }
    flag = false;
  }
}

