/* File: ObstacleAvoidingRobot.ino
   Two-wheel Obstacle-Avoiding Rover with Tilt Safety
*/

#include <NewPing.h>
#include "I2Cdev.h"
#include "MPU6050.h"

// ===== User Config =====
#define TRIG_PIN       8   // HC-SR04 trigger
#define ECHO_PIN       9   // HC-SR04 echo
#define DIST_THRESHOLD 20  // cm

#define MOTOR_LEFT_IN1   2
#define MOTOR_LEFT_IN2   3
#define MOTOR_LEFT_PWM   5
#define MOTOR_RIGHT_IN1  4
#define MOTOR_RIGHT_IN2  7
#define MOTOR_RIGHT_PWM  6
#define MOTOR_SPEED     180  // 0–255

#define REVERSE_TIME     400  // ms
#define TURN_TIME       600   // ms

#define TILT_LIMIT      30.0  // degrees

// ===== Globals =====
NewPing sonar(TRIG_PIN, ECHO_PIN, 200); // max distance 200cm
MPU6050 imu;
bool     tiltAlarm = false;

void setup() {
  // Motor pins
  pinMode(MOTOR_LEFT_IN1,  OUTPUT);
  pinMode(MOTOR_LEFT_IN2,  OUTPUT);
  pinMode(MOTOR_LEFT_PWM,  OUTPUT);
  pinMode(MOTOR_RIGHT_IN1, OUTPUT);
  pinMode(MOTOR_RIGHT_IN2, OUTPUT);
  pinMode(MOTOR_RIGHT_PWM, OUTPUT);

  // Initialize IMU
  Wire.begin();
  imu.initialize();
  if (!imu.testConnection()) {
    // IMU failed
    while (1);
  }

  // Random seed
  randomSeed(analogRead(A0));
}

void loop() {
  if (tiltAlarm) {
    stopMotors();
    return;
  }
  if (isTiltExceeded()) {
    tiltAlarm = true;
    stopMotors();
    return;
  }

  delay(50); // sensor pacing

  // Measure distance
  unsigned int dist = sonar.ping_cm();
  if (dist > 0 && dist < DIST_THRESHOLD) {
    avoidObstacle();
  } else {
    moveForward();
  }
}

// Drive forward at fixed speed
void moveForward() {
  digitalWrite(MOTOR_LEFT_IN1, HIGH);
  digitalWrite(MOTOR_LEFT_IN2, LOW);
  analogWrite(MOTOR_LEFT_PWM, MOTOR_SPEED);

  digitalWrite(MOTOR_RIGHT_IN1, HIGH);
  digitalWrite(MOTOR_RIGHT_IN2, LOW);
  analogWrite(MOTOR_RIGHT_PWM, MOTOR_SPEED);
}

// Stop both motors
void stopMotors() {
  analogWrite(MOTOR_LEFT_PWM,  0);
  analogWrite(MOTOR_RIGHT_PWM, 0);
}

// Reverse, turn randomly, then resume
void avoidObstacle() {
  stopMotors();
  delay(100);

  // Reverse
  digitalWrite(MOTOR_LEFT_IN1, LOW);
  digitalWrite(MOTOR_LEFT_IN2, HIGH);
  digitalWrite(MOTOR_RIGHT_IN1, LOW);
  digitalWrite(MOTOR_RIGHT_IN2, HIGH);
  analogWrite(MOTOR_LEFT_PWM, MOTOR_SPEED);
  analogWrite(MOTOR_RIGHT_PWM, MOTOR_SPEED);
  delay(REVERSE_TIME);

  // Random turn
  bool turnLeft = random(0, 2);
  if (turnLeft) {
    // Left wheel backward, right wheel forward
    digitalWrite(MOTOR_LEFT_IN1, LOW);
    digitalWrite(MOTOR_LEFT_IN2, HIGH);
    digitalWrite(MOTOR_RIGHT_IN1, HIGH);
    digitalWrite(MOTOR_RIGHT_IN2, LOW);
  } else {
    // Right turn
    digitalWrite(MOTOR_LEFT_IN1, HIGH);
    digitalWrite(MOTOR_LEFT_IN2, LOW);
    digitalWrite(MOTOR_RIGHT_IN1, LOW);
    digitalWrite(MOTOR_RIGHT_IN2, HIGH);
  }
  analogWrite(MOTOR_LEFT_PWM, MOTOR_SPEED);
  analogWrite(MOTOR_RIGHT_PWM, MOTOR_SPEED);
  delay(TURN_TIME);

  stopMotors();
}

// Check IMU for excessive tilt
bool isTiltExceeded() {
  int16_t ax, ay, az;
  imu.getAcceleration(&ax, &ay, &az);
  // Convert to g
  float xg = ax / 16384.0;
  float yg = ay / 16384.0;
  float zg = az / 16384.0;
  // Compute tilt angles
  float roll  = atan2(yg, zg) * 57.3;
  float pitch = atan2(-xg, sqrt(yg*yg + zg*zg)) * 57.3;
  return (abs(roll) > TILT_LIMIT || abs(pitch) > TILT_LIMIT);
}
