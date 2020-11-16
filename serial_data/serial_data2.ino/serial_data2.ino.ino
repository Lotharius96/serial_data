double count_speed = 0;
double pos_init = 0;
unsigned long sample_time = 0;
const int signal1 = 2;
const int signal2 = 5;
double angular_speed = 0;
double pos_motor = 0;
unsigned long time_ref, time_back = 0; //tiempo ahorita, tiempo atras
unsigned int flag_giro1 = HIGH;
unsigned int flag_giro2 = LOW;
int flag_time,state = 0;
double y=0;

void setup() {
  cli();
  const int pinNumber = 11;
  const int pin_giro1 = 7;
  const int pin_giro2 = 8;
  pinMode(pinNumber, OUTPUT);
  pinMode(pin_giro1, OUTPUT);
  pinMode(pin_giro2, OUTPUT);
  pinMode(signal1, INPUT); //señal1
  pinMode(signal2, INPUT); //señal2


  TCCR2B = TCCR2B & B11111000 | B00000101; //220 Mhz
  //Disable global interrupts
  //pin11->
  /*
     TCCR1A = 0; //Register set to 0
     TCCR1B = 0; //Register set to 0
     TCNT1 = 0;

     OCR1A = 15999; //Counter for 1KHz interrupt 16*10^6/1000-1 no prescaler
     TCCR1B |= (1 << WGM12); //CTC mode
     TCCR1B |= (1 << CS10); //No prescaler
     TIMSK1 |= (1 << OCIE1A); //Compare interrupt mode
  */
  sei();

  //Enable global interrupts

  Serial.begin(115200);
  //Interrupcion//

  attachInterrupt(digitalPinToInterrupt(signal1), contaSignal, RISING); //EN RISING EDGE

}


void loop()
{

  const int pinNumber = 11;
  const int pin_giro1 = 7;
  const int pin_giro2 = 8;
  int pwmVal =255;
  flag_giro1 = (time_back < 3500) ? HIGH : LOW;
  flag_giro2 = (time_back < 3500) ? LOW : HIGH;
  digitalWrite(pin_giro1, flag_giro1);
  digitalWrite(pin_giro2, flag_giro2);


  analogWrite(pinNumber, pwmVal);

  //Aqui comienza la parte del control
  //time_back=time_ref;
  delay(3); //retardo d eun milisegundo
  //time_back++;
  if (time_back <= 7000) {

    time_back++;
  } else {
    time_back = 0;

  }
 

  double pos_tmp = pos_motor;
  //sample_time = time_ref;
  pos_motor = ConvertPosition();
  
  if (pos_motor != pos_tmp) {
    sample_time=time_ref;
    time_ref=millis();
    //if((time_ref-sample_time)!=0){
    ConvertSpeed(pos_motor, pos_tmp, time_ref, sample_time);
    if(angular_speed <20 && angular_speed>-20){
      y=0.904*y+0.09564*angular_speed;
      
   }else{
     y=y;
    }
   
   // y=0.904*y+0.09564*angular_speed;
     
     Serial.print((time_ref)/10.0,2);
     Serial.print("\t");
     Serial.println(y, 3);
    //}
  }
  //Convierte la señal
 // y=0.904*y+0.09564*angular_speed; //aplica el filtro

  //Serial.print(time_back / 10.0, 2);
  //Serial.print("\t");
 
 // Serial.print("\t");
 // Serial.println(angular_speed, 3);
  //Cambio el sentido de giro


  /*
    Serial.print(time_ref/10.0,2);
    Serial.print("\t");
    Serial.println(pos_motor,3);
  */
  //delay(2000);


  // digitalWrite(pinNumber,0);
  // delay(400);
  //pwmVal+=10;
  //En esta funcion

  /// //obtiene la velocidad por segundo
  /*
    if(time==1000){
        speed=()

    }
  */

  ///Serial print time in milliseconds

  //delay(100);


  //analogWrite(pinNumber,0);
  /*digitalWrite(11,HIGH);

    Serial.print(millis()/10.0,2);
    //Serial.print("\t");
    //Serial.println(digitalRead(2),3);
    delay(500);
    digitalWrite(11,LOW);
    delay(1500);
  */
  //count_speed=0;

}

//Filtro digital
/*
  double digital_LPfilter(){


  }
*/

double ConvertPosition() {
  double base_mpos, mpos = 0;
  int k = (flag_time == HIGH) ? -1 : 1;
  base_mpos = (count_speed / 360);


  //mpos=base_mpos;


  if (count_speed / 75 < 1) {
    mpos = 2 * 3.1416 * (4.8 * base_mpos);
  } else {
    count_speed = 0;


  }
  if (state == 1) {
    pos_init = mpos;

  }

  mpos=pos_init+k*mpos;
  
  return mpos;
}

void ConvertSpeed(double pos1, double pos2, unsigned long t1, unsigned long t2) {
  //double angular_spt = 0;
  if((t1-t2)!=0){
  angular_speed = (pos1 - pos2) * 1000 / (t1 - t2);
  } 
  
  //return angular_spt;

}

/*
  ISR(TIMER1_COMPA_vect){

    //cuenta milisegundos

        double pos_tmp=pos_motor;
        unsigned long time_tmp=sample_time;
        //delay(1);

        //sample_time++;




        pos_motor=ConvertPosition(time_ref);
        if(pos_motor!=pos_tmp){
          //time_ref=millis();
          unsigned long time_tmp=sample_time;
          angular_speed=ConvertSpeed(pos_motor,pos_tmp,time_ref,time_tmp);
        }
       // flag_giro1=(tau<5000)?~flag_giro1:flag_giro1;
       // flag_giro2=(tau<5000)?~flag_giro2:flag_giro2;


        //angular_speed=ConvertSpeed(pos_motor,pos_tmp,time_ref,time_tmp);
  }

*/

void contaSignal() {
  int flag_tmp=flag_time;
  if (digitalRead(signal1) == HIGH) {
    count_speed++; //cuenta cada pulso

 
    //sample_time = time_ref;
    //time_ref = millis();
    flag_time = digitalRead(signal2);
    
    //time_ref=0;
    // sample_time=millis();
  }


  //  delay(1);

}
