#include <BasicLinearAlgebra.h>
#define WHEEL_RADIUS 2
#define BASELINE 5
#define irInputA 12
#define irInputB 11

//matrix method with simulation
int interruptA[24]= {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}; //IR encoder A input
int interruptB[24]= {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}; //IR encoder B input
int idx = 0;
int count_A_encoder = 0;
int count_B_encoder = 0;

double circumference = 2*PI*WHEEL_RADIUS;
double tick_circumference = circumference * (75.8/360);
double delta_theta = tick_circumference/BASELINE;


Matrix<4,4> rotation_A    = {  cos(delta_theta), -sin(delta_theta),  0,  0,
                              sin(delta_theta),  cos(delta_theta),  0,  BASELINE/2,
                              0,  0,  1,  0,
                              0,  0,  0,  1};

Matrix<4,4> rotation_B    = {  cos(-delta_theta), -sin(-delta_theta),  0,  0,
                              sin(-delta_theta),  cos(-delta_theta),  0,  -BASELINE/2,
                              0,  0,  1,  0,
                              0,  0,  0,  1};

Matrix<4,4> translation_A = {  1,  0,  0,  0,
                               0,  1,  0,  -BASELINE,
                               0,  0,  1,  0,
                               0,  0,  0,  1};

Matrix<4,4> translation_B = {  1,  0,  0,  0,
                               0,  1,  0,  BASELINE,
                               0,  0,  1,  0,
                               0,  0,  0,  1};

Matrix<4,4> transformation_A = translation_A * rotation_A;
Matrix<4,4> transformation_B = translation_B * rotation_B;

Matrix<4,4> m = {  1,  0,  0,  0,
                   0,  1,  0,  0,
                   0,  0,  1,  0,
                   0,  0,  0,  1};

void setup()
{
  Serial.begin(9600);
  /*
  pinMode(irInputA, RISING);
  pinMode(irInputB, RISING);
  attachInterrupt(digitalPinToInterrupt(irInputB), checkB, CHANGE); // four interrupts per spin
  attachInterrupt(digitalPinToInterrupt(irInputA), checkA, CHANGE); // four interrupts per spin
  */
}

void loop(){
  Serial.print("theta_Z: ");
  Serial.print(acos(m(0, 0)));
  Serial.print(" , x: ");
  Serial.print(m(0, 3));
  Serial.print(" , y: ");
  Serial.println(m(1, 3)); 
  
  checkA();
  checkB();
  //updatePos();
  idx+=1;
  if(idx ==24){
    idx = 0;
  }
}


void checkA(){
  if(interruptA[idx] != 0){
    count_A_encoder++;
    if(count_A_encoder == 2){
      count_A_encoder = 0;
      
      m = m * transformation_A;
    }
  }
}

void checkB(){
  if(interruptB[idx] != 0){
    count_B_encoder++;
    if(count_B_encoder == 2){
      count_B_encoder = 0;
 
      m = m * transformation_B;
    }
  }
}
