/*
                          -:MULTIPLE CHANNELS NRF24L01 RECEIVER:-
        Here we creat multiple channels in NRF24L01 module for stnderd communictaion.
        This is an experimental project for drone, because drone receive can identify
        the different joystick values which comming from the transmitter.
       
*/

#include<SPI.h>
#include<nRF24L01.h>
#include<RF24.h>
#include<RF24Network.h>
#include<Servo.h>

RF24 radio(7, 8);               //Assign CE and CSN pins
RF24Network network(radio);     //Include the radio in the network
const uint16_t this_node1 = 01;  //Address of 1st node in octal form(04, 031 etc)
const uint16_t this_node2 = 04; //Address of 2nd node in octal form

//Now we have to craet esc objects to control brushless motors
   Servo ESC1;
   Servo ESC2;
   Servo ESC3;
   Servo ESC4;
   

void setup() {
  // put your setup code here, to run once:

//Now we have to caliberate the all 4 ESCs
  ESC1.attach(5,1000,2000);
  ESC2.attach(6,1000,2000);
  ESC3.attach(9,1000,2000);
  ESC4.attach(10,1000,2000);

  SPI.begin();//Start the SPI communication
  radio.begin();//Start the radio communication
  radio.setDataRate(RF24_2MBPS);
  Serial.begin(57600);

}

void loop() {
  // put your main code here, to run repeatedly:
  network.update();//if there is any update in network

////------Receive the data from firs joystick of first channel------------\\\\\\\\\\\\\\\\

  network.begin(10, this_node1);//Set channel and address
  while(network.available()){//is there any incoming data
    RF24NetworkHeader header1;
    unsigned long incomingdata1;
    network.read(header1, &incomingdata1, sizeof(incomingdata1));//read the available data
    ESC1.write(incomingdata1);//send the received pwm signal to the ESC
    ESC2.write(incomingdata1);
    ESC3.write(incomingdata1);
    ESC4.write(incomingdata1);
    Serial.println(incomingdata1);

  network.update();

//////------------Receive the data from second joystick of second channel-------\\\\\\\\\\\\\

 network.begin(20, this_node2);//Set channel and node address
  RF24NetworkHeader header2;
  unsigned long incomingdata2;
  network.read(header2, &incomingdata2, sizeof(incomingdata2));
  
  ////If the pwm value of 2nd joystick is less than 126 then drone move forward
    if(incomingdata2 < 126){
   
   int FRONT_MOVE = incomingdata1 - 55; //Now we have to minus the 55pwm value from the present pwm value 
     
     ESC1.write(FRONT_MOVE);//send the calculated pwm for front  movement
    ESC2.write(FRONT_MOVE);
    ESC3.write(incomingdata1);
    ESC4.write(incomingdata1);
    Serial.println(FRONT_MOVE);
    Serial.println(incomingdata1);
 }

////////If the pwm value of 2nd joystick is greater than 130 then drone move backward direction
     if(incomingdata2 > 130){
      int BACK_MOVE = incomingdata1 - 55;//Now we have to minus the 55pwm value from the present pwm value 
    ESC1.write(incomingdata1);//send the calculated pwm for front  movement
    ESC2.write(incomingdata1);
    ESC3.write(BACK_MOVE);
    ESC4.write(BACK_MOVE);
    Serial.println(BACK_MOVE);
    Serial.println(incomingdata1);
     }
  
  }

}
