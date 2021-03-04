//attaching trigger and echo pins on HC-SR04
const int waterTrig = 16;
const int waterEcho = 17;


//define volume variable and container dimension variables
float water_mL = 0; //volume in container (mL)
float distance_away = 3.1; //distance sensor is away from container in cm
float radius = 3; //radius of container space in cm (an average was calculated between the top and bottom radii of the testing cup)
float height = 13.6; //internal height of container in cm

void setup() {
  //configure pins to output and input
  pinMode(waterTrig, OUTPUT);
  pinMode(waterEcho, INPUT);
  
  // initialize serial communications
  Serial.begin(9600);

}

float get_volume(){
  //declare mL, distance, and timeSignal
  float mL_val, distance;
  double timeSignal;
  
  //prepare clean trig pulse with writing low for 5us
  digitalWrite(waterTrig, LOW);
  delayMicroseconds(5);

  //datasheet requires at least 10us of HIGH pulse
  digitalWrite(waterTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(waterTrig, LOW);

  //read length of HIGH pulse in microseconds
  timeSignal = pulseIn(waterEcho, HIGH);

  //use time to calculate distance in cm (speed of sound ~340m/s = 0.034cm/us, halve time taken to get one length)
  distance = timeSignal * 0.034 / 2;

  //V = L*pi*(r^2)
  mL_val = (height + distance_away - distance) * (3.14159 * radius * radius);

  //print volume in container and level of water from top
  //Serial.print("Volume (mL) = ");
  //Serial.print(mL);
  //Serial.print("\t Distance from top (cm.) = ");
  //Serial.println(distance - distance_away); 

  return mL_val;
}

void loop() {
  //can report mL (NOTE: get_volume takes at least 15us)
  water_mL = get_volume();
  //datasheet recommends at least 60ms measuring period. For sake of test will include delay for 2s
  //delay(2000);

}

