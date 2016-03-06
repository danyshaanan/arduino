int hall, state, phase, alpha, i, j, led, b, button, program, p;
unsigned long int lastmillis;
#define LEDS 8
#define maxalpha 360
#define maxprogram 8

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);    //devled  
  pinMode(A3, INPUT);     //hall sensor
  pinMode(10, INPUT); digitalWrite(10, HIGH); //button with pullup
  pinMode(11, OUTPUT); digitalWrite(11, LOW); //v0 for button
  for(led=0;led<LEDS;++led) pinMode(led+2, OUTPUT);
  
  state = 0;
  lastmillis = 0;
  phase = 1000;
  button = 1;
  program = 1;
}

void loop() {
  if (!button && digitalRead(10)) program = program%maxprogram+1;
  button = digitalRead(10);
  
  hall = abs(analogRead(A3) - 1024/2);
  if (hall<12) state = 0;
  else if ((41<hall) and (state==0)) {
    state = 1;
    phase = millis() - lastmillis;
    lastmillis = millis();
  }
  digitalWrite(13, ((41<hall) or ((21<=hall) and (millis()/10%2))));

  alpha = min(maxalpha,(maxalpha*(millis()-lastmillis))/phase);
  for(led=0;led<LEDS;++led) digitalWrite(led+2,!writeLedAlphaProgram(led,alpha,program));
   
  Serial.print(abs((millis()/100%14)-7));
  Serial.print("\n");
}

int writeLedAlphaProgram(int led, int alpha, int program){
  int kit = (millis()/20%14-7); //kit
  //define maxprogram at the begining of script to fit number of options.
  //write bool values based on alpha (0-360), maxalpha (360), led (2-9) and LEDS (9). Also, use millis() somehow
  //change p for testing:
  p = program;
  if (1==p) return (2*((alpha+millis()/30)%maxalpha)/maxalpha + led/4)%2; //moving q chess
  if (2==p) return (7-((3*alpha*LEDS/maxalpha)%LEDS) <= led*0.9); //3 pyramids
  if (3==p) return ((kit == led) or (-kit == led)); //kit
  if (4==p) return ((kit == led) or (-kit == led) or (kit == 7-led) or (-kit == 7-led)); //double kit
  if (5==p) return (4*alpha/maxalpha + led/2)%2; //half chess
  
  if (6==p) return 1;
  if (7==p) return (abs((alpha+0)%maxalpha-180) < 45*abs(millis()%1000-500)/500); //pacman? set alpha-shift and check!
  if (8==p) return millis()/500%2;
  
  
 return !(millis()/100%3);
}
