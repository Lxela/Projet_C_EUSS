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
    char acquisition[10] = "stop"; // bien ici ??????
    
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
            entry = buf-'0';
            memcpy(buf, &buffer[4], 1);
            buf[1] = '\0';
            n = buf-'0';
            ret = setEntry(&entry, &n);
            break;
        case 'E':
            strcpy(mess, "AE0");
            memcpy(buf, &buffer[2], 2);
            buf[2] = '\0';
            entry = buf-'0';
            ret = readEntry(&entry);
            strcat(mess, ret);
            strcat(mess, "Z");
            break;
        case 'C':
            strcpy(mess,"AC0");
            sprintf(buf, "%.2f", avg);
            strcat(mess, buf);
            strcat(mess, "Z");
            break;
        case 'M':
            //to get the differents datas from the message
            memcpy(subbuf, &buffer[2], 1);      // c'est le v
            subbuf[2] = '\0';
           
            memcpy(subbuf2, &buffer[3], 2);     // c'est le temps
            subbuf2[2] = '\0';
           
            memcpy(subbuf3, &buffer[5],2);      // c'est le num
            subbuf3[1] = '\0';
           
           
            printf("acquisition = %s\n", acquisition);
 
            //to start the acquisition of the temperature
            if (strcmp(subbuf,"1") == 0) {
                printf("\nAcquisition started witch %s seconds of interval.\n",subbuf2);
                printf("We calculate median with %s data\n\n. ",subbuf3);
                int n = subbuf3-'0';
                getAverage(tabtemperature, &n);  // tabtemperature est le tableau qui a les temperature
           
            }
            //stop the acquisition of the temperature
            else {
                strcpy(mess,"AM0Z");
                strcpy(acquisition,"stop");
                printf("mess = %s\n", mess);
            }
            printf("acquisition = %s\n", acquisition);
            break;
        default:
            printf("invalid order\n");
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

int setEntry(int *val, int *entry) {
    int ret;
 
    pinMode(entry,OUTPUT);
    digitalWrite(entry,val);
 
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
