
/*
********************Author:: Subash Timilsina
********************
*/

#include <XBOXRECV.h>
#include <SPI.h>

USB Usb;
XBOXRECV Xbox(&Usb);

const char STARTBYTE = 127;
char GamebuttonA , GamebuttonB, previousA,previousB;
char Righttrigger , Lefttrigger;
char LeftstickY , RightstickY , LeftstickX , RightstickX;

int Compute(long int x)
{
  if (x < -32767) x = -32767;
   return ((x+32767)/65534.0)*100;
}

void reset_data()
{
  GamebuttonA = 0 , GamebuttonB = 0 , Righttrigger = 0 , Lefttrigger = 0 ,
  LeftstickY=50 , RightstickY=50 , LeftstickX=50 , RightstickX=50 ; 
}

void send_data()
{
  Serial.print((STARTBYTE));
  Serial.print((GamebuttonA));
  Serial.print((GamebuttonB));
  Serial.print((Righttrigger));
  Serial.print((Lefttrigger));
  Serial.print((LeftstickX));
  Serial.print((LeftstickY));
  Serial.print((RightstickX));
  Serial.print((RightstickY));
}

void setup() {
  Serial.begin(38400);  //uart0 communication
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nJoystick initialization failed.")); //string inside f act as constant in program memory rather than loading in ram
    while (1); //halt
  }
}



void loop() {
  Usb.Task();     //to check usb state **running or not ** high speed usb or not **usb task changed if vbus is changed and communicate with max3421e
  reset_data();
  if (Xbox.XboxReceiverConnected) //check if the xbox wireless receiver is connected or not
  { 
    reset_data();
      if (Xbox.Xbox360Connected[0])   //if joystick is on otherside is on
      {
        reset_data();
        if (Xbox.getButtonPress(L2) || Xbox.getButtonPress(R2))  //if data are available on left and right analog buttons
        { 
          Lefttrigger = int(Xbox.getButtonPress(L2)*100.0/255);
          Righttrigger = int(Xbox.getButtonPress(R2)*100.0/255);
         //Xbox.setRumbleOn(Xbox.getButtonPress(L2), Xbox.getButtonPress(R2));  //Set vibrations on motor @para( leftvalue, rightvalue ,controller)
        }

        if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500 || Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500 || Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500 || Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) 
        { //if sticks are moved //right and up 100 max and left and down 0
          if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500) 
          {
            LeftstickX = Compute(Xbox.getAnalogHat(LeftHatX));
          }
          if (Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500)
          {
             LeftstickY = Compute(Xbox.getAnalogHat(LeftHatY));
          }
          if (Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500)
          {
            RightstickX = Compute(Xbox.getAnalogHat(RightHatX));
          }
          if (Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500)
          {
             RightstickY = Compute(Xbox.getAnalogHat(RightHatY));
          }
        }
      
        if (Xbox.getButtonPress(UP))
        {
          Xbox.setLedOn(LED1);
          GamebuttonB |= (1<<0);
        }
       if (Xbox.getButtonPress(DOWN))
        {  
          Xbox.setLedOn(LED4);
          GamebuttonB |= (1<<2);
        }
        if (Xbox.getButtonPress(LEFT))
        {
          Xbox.setLedOn(LED3);
          GamebuttonB |= (1<<3);
        }
        if (Xbox.getButtonPress(RIGHT))
        {
          Xbox.setLedOn(LED2);
          GamebuttonB |= (1<<1);
        }

        if (Xbox.getButtonClick(START))
        {
          Xbox.setLedMode(ALTERNATING); //opposite led are on
          GamebuttonA |= (1<<6);
        }
        if (Xbox.getButtonClick(BACK))
        {
          Xbox.setLedBlink(ALL); //all light with certain delay
          GamebuttonB |= (1<<6);
        }
         if (Xbox.getButtonClick(XBOX)) 
        {
          Xbox.setLedMode(ROTATING);
        }
        
        if (Xbox.getButtonClick(L3))
        {
          GamebuttonB |= (1<<4);
        }
        if (Xbox.getButtonClick(R3))
        {
          GamebuttonA |= (1<<4);
        }
        if (Xbox.getButtonClick(L1))
        {
          GamebuttonB |= (1<<5);
        }
        if (Xbox.getButtonClick(R1))
        {
          GamebuttonA |= (1<<5);
        }
         
        if (Xbox.getButtonClick(SYNC))
        {
          Xbox.disconnect();
        }

        if (Xbox.getButtonClick(A))
        {
          GamebuttonA |= (1<<0);
        }
        if (Xbox.getButtonClick(B))
        {
          GamebuttonA |= (1<<1);
        }
        if (Xbox.getButtonClick(X))
        {
          GamebuttonA |= (1<<2);
        }
        if (Xbox.getButtonClick(Y))
        {
          GamebuttonA |= (1<<3);
        }
       send_data();
     }
  }
}
