#include <SoftwareSerial.h> 
#include <avr/interrupt.h>

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <float.h>


float tabtemperature[100];
int i = 0;
int n;
int counter = 0;
float avg = 0.00;
int current = 0;

bool start = false; // aquisition false au démarrage

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);


  // initialize Timer1
  cli(); // disable global interrupts
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B

  // set compare match register to desired timer count:
  OCR1A = 15624;

  // turn on CTC mode:
  TCCR1B |= (1 << WGM12);

  // Set CS10 and CS12 bits for 1024 prescaler:
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);

  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);

  // enable global interrupts:
  sei();

}

ISR(TIMER1_COMPA_vect) {

  if (start == true) {
    randomSeed(analogRead(0));
    tabtemperature[i] = random(0.0, 30.0);
    
    if (i == 99) {
      i = 0;
      current = 0;
    } else {
      i++;
      current++;
      counter++;
    }

    if (counter == n) {
      avg = getAverage(tabtemperature, n);
      Serial.println("avg : ");
      Serial.println(avg);
      counter = 0;
    }

  }

}

//(# timer counts + 1) = (target time) / (timer resolution)
//(# timer counts + 1) = (1 s) / (6.4e-5 s)
//(# timer counts + 1) = 15625
//(# timer counts) = 15625 - 1 = 15624

void loop() {

  char acquisition[10] = "stop";
  char mess[10];
  String buffer;
  char val[4];
  char subbuf[10];
  char subbuf2[10];
  char subbuf3[10];
  char buf[10];
  int entry;
  int ret;
  int i = 0;

  buffer = Serial.readString();
  
  char order = buffer[1];
  if (buffer != NULL) {
    Serial.print("I received something: ");
    Serial.println("message : ");
    Serial.println(buffer);

    switch (order) {
    case 'S':

      memcpy(buf, &buffer[2], 2);
      buf[2] = '\0';
      entry = atoi(buf);
      if (buffer[0] != 'A' || buffer[5] != 'Z' ) {
        strcpy(mess, "AS1Z");
      } else if ((entry >= 0 && entry <= 13) || (buffer[4] == 0 || buffer[4] == 1) || buffer[1] == "S") {
        strcpy(mess, "AS0Z");
        memcpy(buf, &buffer[4], 1);
        buf[1] = '\0';
        n = atoi(buf);
        ret = setEntry( &entry, &n);
      } else {
        strcpy(mess, "AS2Z");
      }
      Serial.print("mess : ");
      Serial.println(mess);
      break;

    case 'E':

      memcpy(buf, &buffer[2], 2);
      buf[2] = '\0';
      entry = atoi(buf);

      if (buffer[0] != 'A' || buffer[4] != 'Z'  ) {
        strcpy(mess, "AS1Z");
      } else if (entry >= 0 && entry <= 13 || buffer[1] == 'E') {
        strcpy(mess, "AE0");
        ret = readEntry(&entry);
        if (ret == 0) {
          strcat(mess, "0");
        } else {
          strcat(mess, "1");
        }
        strcat(mess, "Z");

      } else {
        strcpy(mess, "AS2Z");
      }
      Serial.print("mess : ");
      Serial.println(mess);
      break;

    case 'C':

      if (buffer[0] != 'A' || buffer[2] != 'Z') {
        strcpy(mess, "AS1Z");
      } else if (buffer[1] != 'C') {
        strcpy(mess, "AS2Z");
      } else {
        strcpy(mess, "AC0");
        dtostrf(avg, 4, 1, subbuf);

        if (avg < 10) {
          dtostrf(avg, 4, 2, subbuf);
        } else {
          dtostrf(avg, 4, 1, subbuf);
        }
        strcat(mess, subbuf);
        strcat(mess, "Z");
      }

      Serial.print("mess : ");
      Serial.println(mess);
      break;

    case 'M':
    
      //to get the differents datas from the message
      memcpy(subbuf, &buffer[2], 1); // c'est le v
      subbuf[1] = '\0';

      memcpy(subbuf2, &buffer[3], 2); // c'est le temps
      subbuf2[2] = '\0';

      memcpy(subbuf3, &buffer[5], 1); // c'est le num
      subbuf3[1] = '\0';

      if (buffer[0] != 'A' || buffer[6] != 'Z') {
        strcpy(mess, "AS1Z");
        Serial.println("rentré dans permeir");
      } else if ( (buffer[2] == '0' || buffer[2] == '1') && (atoi(subbuf2) >= 1 && atoi(subbuf2) <= 20) && (atoi(subbuf3) >= 1 && atoi(subbuf3) <= 9) ) {
        Serial.println("rentré dans deuxiemr");
        //to start the acquisition of the temperature
      if (strcmp(subbuf, "1") == 0) {
        // disable global interrupts:
        cli();
        // set compare match register to desired timer count:
        int timercount = 15624 * atoi(subbuf2);
        OCR1A = timercount;

        // enable global interrupts:
        sei();

        Serial.print("Acquisition started with an interval in seconds of : ");
        Serial.println(subbuf2);
        Serial.print("We calculate median with a number of data of : ");
        Serial.println(subbuf3);
        n = atoi(subbuf3);
        strcpy(acquisition, "start");
        start = true;
        strcpy(mess, "AS0Z");

      }
      //stop the acquisition of the temperature
      else {
        strcpy(mess, "AM0Z");
        strcpy(acquisition, "stop");
        start = false;
        
        counter = 0;
      }
      }
      else {
        strcpy(mess, "AS2Z");
      }

      Serial.print("mess = ");
        Serial.println(mess);
      Serial.print("acquisition is now = ");
      Serial.println(acquisition);
      break;
      
    default:
      Serial.println("invalid order \n");
      strcpy(mess, "AS1Z");
      Serial.println(mess);
    }

  }

}

int setEntry(int *entry, int *val) {
  int ret = 0;
  bool state;
  if ( *val == 0) {
    state = LOW;
  } else if ( *val == 1) {
    state = HIGH;
  }

  pinMode(entry, OUTPUT);
  digitalWrite( *entry, state);

  return ret;
}

int readEntry(int *entry) {

  pinMode(entry, INPUT);
  return digitalRead(*entry);

}

//to get the average of the temperature
float getAverage(float *tab, int n) {
  float ret;
  int count = 0;
  float sum = 0.0;
  int i = current - n;

  if (current - n < 0) {
    i = 100 - current - n;
  }

  while (count < n) {
    if (i > 99) {
      i = 0;
    }
    sum = sum + tab[i];
    count++;
    i++;
  }
  ret = sum / count;
  return ret;
}
