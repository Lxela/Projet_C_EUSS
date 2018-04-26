#include "DHT.h"
#include <SoftwareSerial.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
 

#define DHTPIN 2
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

int temperature;
float tabtemperature[100];
int i=0;

//int temps = 0;
String message;

// ------ var function millis-------

unsigned long previousMillis=0 ;
unsigned long interval = 30000;




void setup() {
  // put your setup code here, to run once:

  pinMode(DHTPIN,INPUT);
  Serial.begin(9600);
  dht.begin();
  
  
}




void loop() {
    char acquisition[10] = "stop";
    
    char* mess = malloc(10*sizeof(char));
    char* acqu_state = malloc(10*sizeof(char));
    acqu_state = "stop"; //initialisation
       
    String buffer; //= buffer[1];
    char val[4];
    char subbuf[10];
    char subbuf2[10];
    char subbuf3[10];
    char buf[10];
    int entry;
    int n;
    int ret;
    float avg;
    char test;


              
    buffer = Serial.readString();
    char order = buffer[1];
    if(buffer != NULL){          
        Serial.print("I received something: ");
        Serial.println("message : ");
        Serial.println(buffer);

    switch(order) {
        case 'S':
            
            strcpy(mess, "AS0Z");
            memcpy(buf, &buffer[2], 2);
            buf[2] = '\0';            
            entry = atoi(buf);
            Serial.print("entry = ");
            Serial.println(entry);
            memcpy(buf, &buffer[4], 1);
            buf[1] = '\0';
            n = atoi(buf);
            Serial.print("n = ");
            Serial.println(n);
            ret = setEntry(&entry, &n);
              break;
              
        case 'E':
            Serial.println("rentré dans case E");
            strcpy(mess, "AE0");
            memcpy(buf, &buffer[2], 2);
            buf[2] = '\0';
            entry = atoi(buf);
            ret = readEntry(&entry);
            Serial.print("ret : ");
            Serial.println(ret);
            test = char(&ret);
            strcat(mess, &test);
            strcat(mess, "Z");
            Serial.print("mess : ");
            Serial.println(mess);
            break;
            
        case 'C':
            Serial.println("rentré dans case c");
            strcpy(mess,"AC0");
            sprintf(buf, "%.2f", avg);
            if (avg < 10) 
            strcat(buf, "0");
            strcat(mess, buf);
            strcat(mess, "Z");
            Serial.println("sort du case c");
            break;
            
        case 'M':
            Serial.println("rentré dans case m");
            //to get the differents datas from the message
            memcpy(subbuf, &buffer[2], 1);      // c'est le v
            subbuf[1] = '\0';
           
            memcpy(subbuf2, &buffer[3], 2);     // c'est le temps
            subbuf2[2] = '\0';
           
            memcpy(subbuf3, &buffer[5], 1);      // c'est le num
            subbuf3[1] = '\0';
            
            //Serial.print("acquisition = ");
            //Serial.print(acquisition);
            
            //to start the acquisition of the temperature
            if (strcmp(subbuf,"1") == 0) {
                
                
                Serial.print("Acquisition started with an interval in seconds of : ");
                Serial.println(subbuf2);
                Serial.print("We calculate median with a number of data of : ");
                Serial.println(subbuf3);
                int n = atoi(subbuf3);
                avg = getAverage(tabtemperature, &n);  // tabtemperature est le tableau qui a les temperature
                strcpy(acquisition,"start");
                
            }
            //stop the acquisition of the temperature
            else {
                strcpy(mess,"AM0Z");
                strcpy(acquisition,"stop");
                Serial.print("mess = ");
                Serial.println(mess);
            }
            Serial.print("acquisition is now = ");
            Serial.println(acquisition);
            break;
        default:
            Serial.println("invalid order \n");
            strcpy(mess,"erreuuuur");
 
    }


  }


//------------ recuperation temperature----------
  
   temperature = (int)dht.readTemperature();
  i++;
  tabtemperature[i] = temperature;
  if (i==99){
    i=0;
    }
  
   
  
  if (isnan(temperature) ) {
      Serial.println("Impossible de lire le capteur DHT");
      return;
    }
  Serial.println(temperature);
  delay(500); // a changer selon demande
}



int setEntry(int *entry, int *val) {
    int ret = 0;
    bool state;
    if (*val == 0){
      state = LOW;} 
      
      else if(*val==1){
        state = HIGH;}

    pinMode(entry,OUTPUT);
    digitalWrite(*entry,state);
    
    return ret;
}
 
int readEntry (int *entry) {
    int ret;
    pinMode(entry,INPUT);
    return digitalRead(entry);
}



 
//to get the average of the temperature
float getAverage(float *tab, int *n) {
    float ret;
    int count = 0;
    float sum = 0.0;
    int i;
   
    for (i=0;i < n; i++){
        if (tab[i] != 0){
            sum = sum + tab[i];
            count++;
        }
    }
    ret = sum/count;
    printf("average = %f\n", ret);
   
    return ret;
}
