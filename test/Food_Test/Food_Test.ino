//attaching trigger and echo pins on HC-SR04
const int foodTrig = 14;
const int foodEcho = 15;


//define volume variable and container dimension variables
float food_grams; //mass of food (g)
float distance_away = 5.5; //distance sensor is away from container
float radius = 6.3; //radius of container space in cm
float height = 23.3; //height of container in cm
float density = 0.82844; //density of pinto beans (g/mL; according to nutrition facts there are 49g per 1/4 cup)

void setup() {
  //configure pins to output and input
  pinMode(foodTrig, OUTPUT);
  pinMode(foodEcho, INPUT);
  
  // initialize serial communications
  Serial.begin(9600);

}

float get_mass(){
  //declare mass, volume, distance, and timeSignal
  float gram_val, mL_val, distance;
  double timeSignal;
  
  //prepare clean trig pulse with writing low for 5us
  digitalWrite(foodTrig, LOW);
  delayMicroseconds(5);

  //datasheet requires at least 10us of HIGH pulse
  digitalWrite(foodTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(foodTrig, LOW);

  //read length of HIGH pulse in microseconds
  timeSignal = pulseIn(foodEcho, HIGH);

  //use time to calculate distance in cm (speed of sound ~340m/s = 0.034cm/us, halve time taken to get one length)
  distance = timeSignal * 0.034 / 2;

  //V = L*pi*(r^2)
  mL_val = (height + distance_away - distance) * (3.14159 * radius * radius);
  gram_val = mL_val * density;

  //print mass and level of food from top
  //Serial.print("Mass (g) = ");
  //Serial.print(mL_val * density);
  //Serial.print("\t Distance (cm.) = ");
  //Serial.println((distance - distance_away));

  return gram_val;
}

void loop() {
  //call mass function (NOTE: takes at least 15us)
  food_grams = get_mass();

  //datasheet recommends at least 60ms measuring period. For sake of test will include delay for 2s
  delay(2000);

}

