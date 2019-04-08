#define WHEEL_RADIUS 2
#define BASELINE 5
#define irInputA 12
#define irInputB 11

//analytical method with simulation
//this code emulates the robot travelling in straight line indefinitely.
//has support for IR interrupts with some simple commenting, current setup is for test/simulation
int interruptA[24]= {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}; //IR encoder A input
int interruptB[24]= {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}; //IR encoder B input

double x_loc;
double y_loc;
double z_theta;

int idx = 0;
int count_A_encoder = 0;
int count_B_encoder = 0;

double circumference = 2*PI*WHEEL_RADIUS;
double tick_circumference = circumference * (75.8/360);
double delta_theta = tick_circumference/BASELINE;
double delta_x = (BASELINE/2)*sin(delta_theta);
double delta_y = (BASELINE/2)*(1-cos(delta_theta));
double x_0 = 0;
double y_0 = 0;

void setup() {
  Serial.begin(9600);
  /*
  pinMode(irInputA, RISING);
  pinMode(irInputB, RISING);
  attachInterrupt(digitalPinToInterrupt(irInputB), checkB, CHANGE); // four interrupts per spin
  attachInterrupt(digitalPinToInterrupt(irInputA), checkA, CHANGE); // four interrupts per spin
  */
}

void loop() {
  checkA();
  checkB();
  //updatePos();
  idx+=1;
  if(idx ==24){
    idx = 0;
  }
  Serial.print("X: ");
  Serial.print(x_loc);
  Serial.print(" , Y:");
  Serial.println(y_loc);
}

void checkA(){
  if(interruptA[idx] != 0){
    count_A_encoder++;
    if(count_A_encoder == 2){
      count_A_encoder = 0;

      z_theta += delta_theta;
      x_0 = (delta_x * cos(delta_theta)) + (delta_y * sin(delta_theta));
      y_0 = (delta_x * sin(delta_theta)) + (delta_y * cos(delta_theta));
      x_loc += x_0;
      y_loc -= y_0;
    }
  }
}

void checkB(){
  if(interruptB[idx] != 0){
    count_B_encoder++;
    if(count_B_encoder == 2){
      count_B_encoder = 0;

      z_theta -= delta_theta;
      x_0 = (delta_x * cos(delta_theta)) + (delta_y * sin(delta_theta));
      y_0 = (delta_x * sin(delta_theta)) + (delta_y * cos(delta_theta));
      x_loc += x_0;
      y_loc += y_0;
    }
  }
}
