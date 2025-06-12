# Obstacle-Avoiding Robot

**Files**  
- `README.md`  
- `ObstacleAvoidingRobot.ino`

## 1. Concept  
A two-wheel differential drive rover that roams autonomously, uses an HC-SR04 ultrasonic sensor to detect obstacles ahead, and an MPU-6050 IMU to monitor tilt (prevent tipping). On obstacle detection it stops, reverses, turns randomly left or right, then resumes. If it tilts beyond a safety angle, it immediately halts.

**Key Behaviors**  
1. **Forward Patrol** at constant speed.  
2. **Obstacle Avoidance**:  
   - If distance < 20 cm ➔ Stop ➔ Reverse ➔ Turn (random) ➔ Resume.  
3. **Tilt Safety**:  
   - If roll or pitch > 30° ➔ Emergency stop.

## 2. Bill of Materials  
- 1 × Arduino Uno (or Nano)  
- 1 × L298N dual H-bridge motor driver  
- 2 × DC gear motors + wheels  
- 1 × HC-SR04 ultrasonic distance sensor  
- 1 × MPU-6050 6-axis IMU  
- Chassis, caster wheel, battery pack (7.4 V Li-ion or 6×AA)  
- Jumper wires, solderless breadboard (optional)  

## 3. Wiring Diagram

    Arduino Uno                L298N Motor Driver         Motor A  
    ┌──────────────┐           ┌────────────────┐        ┌─────┐
    │ D2  ─────► IN1 │──►      │ IN1            │──►    │ M+  │
    │ D3  ─────► IN2 │──►      │ IN2            │──►    │ M-  │
    │ D5 (PWM)► ENA │──►       │ ENA            │
    └──────────────┘           └────────────────┘        └─────┘

    Arduino Uno                L298N Motor Driver         Motor B  
    ┌──────────────┐           ┌────────────────┐        ┌─────┐
    │ D4  ─────► IN3 │──►      │ IN3            │──►    │ M+  │
    │ D7  ─────► IN4 │──►      │ IN4            │──►    │ M-  │
    │ D6 (PWM)► ENB │──►       │ ENB            │
    └──────────────┘           └────────────────┘        └─────┘

    HC-SR04:
    ┌──────────────┐
    │ D8 ──► Trig  │
    │ D9 ──► Echo  │
    └──────────────┘

    MPU-6050 (I²C):
    ┌──────────────┐
    │ A4 ──► SDA   │
    │ A5 ──► SCL   │
    │ 5V ──► VCC   │
    │ GND─► GND    │
    └──────────────┘

> **Power:** Drive motors from battery pack; power Arduino and sensors from 5 V regulator.

## 4. Software Setup  
1. In Arduino IDE install libraries:  
   - `NewPing` by Tim Eckel  
   - `I2Cdev` & `MPU6050` (Jeff Rowberg’s I2CdevLib)  
2. Create project folder `ObstacleAvoidingRobot` and add both files.  
3. Open `ObstacleAvoidingRobot.ino` in Arduino IDE.  
4. Select **Arduino Uno** (or Nano) and correct COM port.

## 5. Operation  
1. Power on. Robot begins forward at `MOTOR_SPEED`.  
2. If obstacle < `DIST_THRESHOLD` cm:  
   - Stops ➔ reverses `REVERSE_TIME` ms ➔ random turn left/right for `TURN_TIME` ms ➔ resumes.  
3. If tilt > `TILT_LIMIT` ° in any axis:  
   - Immediate full stop. Re-enable by power-cycle.  

---

Say **continua** to move on to project #7!  
