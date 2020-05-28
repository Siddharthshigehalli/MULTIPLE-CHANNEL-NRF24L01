/*
                          -:MULTIPLE CHANNELS NRF24L01 TRANSMITTER:-
        Here we creat multiple channels in NRF24L01 module for stnderd communictaion.
        This is an experimental project for drone, because drone receive can identify
        the different joystick values which comming from the transmitter.
       
*/

#include<SPI.h>
#include<nRF24L01.h>
#include<RF24.h>
#include<RF24Network.h>

RF24 radio(7, 8);               //Assign CE and CSN pins
RF24Network network(radio);     //Include the radio in the network
const uint16_t this_node1 = 00;  //Address of 1st node in octal form(04, 031 etc)
const uint16_t  node01 = 01; //Address of 2nd node in octal form
const uint16_t node02 = 02;
const uint16_t this_node2 = 04;

void setup() {
  // put your setup code here, to run once:
  SPI.begin();          //Start the SPI communication
  radio.begin();        //Start the radio communication
  radio.setDataRate(RF24_2MBPS);

}

void loop() {
  // put your main code here, to run repeatedly:
  network.update();                                                         //If there is any update in network
 ///---Transmite the data of first joystick from first channel---------\\\\\\\\\\\\
 
  network.begin(10, this_node1);//chnannel and node address
  unsigned long Xaxis = analogRead(A6);                                       //read the analog value of first joystick
  unsigned long First_joystick = map(Xaxis,0,1023,0,255);                     //map the value from 0 to 255 pwm signal along the joystck value from 0 to 1023
  RF24NetworkHeader header1(node01);                                           // Address where the data is going
  bool ok1 = network.write(header1, &First_joystick, sizeof(First_joystick)); // Send the data

  network.update();//If there is any update in network
  ///---Transmite the data of Second joystick from Second channel---------\\\\\\\\\\\\
 
  network.begin(20, this_node2);//chnannel and node address
  unsigned long X1axis = analogRead(A4);                                       //read the analog value of second joystick
  unsigned long Second_joystick = map(X1axis,0,1023,0,255);                     //map the value from 0 to 255 pwm signal along the joystck value from 0 to 1023
  RF24NetworkHeader header2(node02);                                           // Address where the data is going
  bool ok2 = network.write(header2, &Second_joystick, sizeof(Second_joystick)); // Send the data

}
