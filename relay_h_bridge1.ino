// C++ code
//
int throttle = A0;
int forward = 10;
int reverse = 11;
int motor_contactor_1 = 12; // relay used to simulate large contactor
int motor_contactor_2 = 13;
int battery_voltage_1 = A5; // accessory battery voltage = 1k/3.8k ohm resistor

void no_wake_motor_active()	{
	int throttle_position = analogRead(throttle);
  
  // throttle and motor speed control
  if(throttle_position <= 472)	{
    digitalWrite(motor_contactor_1, LOW);
    digitalWrite(forward, LOW);
    digitalWrite(motor_contactor_2, HIGH);
    // converted to go forward with PWM for speed control
    int motor_speed = 255 - throttle_position / 2;
    analogWrite(reverse, motor_speed);
    // Serial.println(throttle_position);
  }
  else if(throttle_position >= 552)	{
    digitalWrite(motor_contactor_2, LOW);
    digitalWrite(reverse, LOW);
    digitalWrite(motor_contactor_1, HIGH);
    //converted to go reverse with PWM for speed control
    int motor_speed = throttle_position / 2;
    analogWrite(forward, motor_speed);
    // Serial.println(throttle_position);
  }
  else	{
    //center of pot is off or park/nurtal shifter stright up and down
    digitalWrite(motor_contactor_1, LOW);
    digitalWrite(motor_contactor_2, LOW);
    digitalWrite(reverse, LOW);
    digitalWrite(forward, LOW);
  }
}

void low_power_mode()	{ 
  // disconnect auxillary motor from power to protect engine start motor
  digitalWrite(motor_contactor_1, LOW);
  digitalWrite(motor_contactor_2, LOW);
  digitalWrite(forward, LOW);
  digitalWrite(reverse, LOW);
  Serial.println("Accessories have drained start battery to much");
  Serial.println("AI mechanic has disconnected from start battery to ensure engine will start");
}

void setup()
{
  pinMode(forward, OUTPUT);
  pinMode(reverse, OUTPUT);
  pinMode(motor_contactor_1, OUTPUT);
  pinMode(motor_contactor_2, OUTPUT);
  pinMode(throttle, INPUT);
  pinMode(battery_voltage_1, INPUT);
  Serial.begin(9600);
  
  // initial states to avoid irratic behavior at start up
  digitalWrite(motor_contactor_1, LOW);
  digitalWrite(motor_contactor_2, LOW);
  digitalWrite(forward, LOW);
  digitalWrite(reverse, LOW);
}

void loop()
{ 
  no_wake_motor_active(); // only used to make simulation work, it starts with 0V till dial is turned for some weird reason
  //Serial.println(throttle_position);
  // battery level management
  int battery_level_1 = analogRead(battery_voltage_1);
  float sensor_voltage = battery_level_1 * (5.0 / 1023.0);
  float voltage = sensor_voltage + sensor_voltage * 3.8; // reducing factor from resistors
  int battery_life = 0; // battery life in percentage
  if(voltage >= 12.7)	{
    Serial.println("100%");
    no_wake_motor_active();
  }
  else if(voltage < 12.7 && voltage >= 12.5)	{
    Serial.println("90%");
    no_wake_motor_active();
  }
  else if(voltage < 12.5 && voltage >= 12.42)	{
    Serial.println("80%");
    no_wake_motor_active();
  }
  else if(voltage < 12.42 && voltage >= 12.32)	{
    Serial.println("70%");
    no_wake_motor_active();
  }
  else if(voltage < 12.32 && voltage >= 12.2)	{
    Serial.println("60%");
  	low_power_mode();
  }
  else if(voltage < 12.2 && voltage >= 12.06)	{
    Serial.println("50%");
    low_power_mode();
  }
  else if(voltage < 12.06 && voltage >= 11.9)	{
    Serial.println("40%");
    low_power_mode();
  }
  else if(voltage < 11.9 && voltage >= 11.75)	{
    Serial.println("30%");
    low_power_mode();
  }
  else if(voltage < 11.75 && voltage >= 11.58)	{
    Serial.println("20%");
    low_power_mode();
  }
  else if(voltage < 11.58 && voltage >= 11.31)	{
    Serial.println("10%");
    low_power_mode();
  }
  else if(voltage < 11.31 && voltage >= 10.5)	{
  	Serial.println("Batteries low, start engine or plug in to charge");
    low_power_mode();
  }
  else	{
    Serial.println("Service batteries");
    low_power_mode();
  }
  
  // AI mechanic basic demonstration
  if(voltage > 14.5)	{
    Serial.println("Alternator overcharging, have serviced");
  }
  
  //Serial.println(battery_level_1);
  //Serial.println(sensor_voltage);
  //Serial.println(voltage); // should match meter
  
  delay(250); // 1/4 sec delay to allow serial monitor to be readable
 
}