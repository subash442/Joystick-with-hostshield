
/*
********************Author:: Subash Timilsina
********************
*/

#include <XBOXRECV.h>
#include <SPI.h>

USB Usb;
XBOXRECV Xbox(&Usb);

char GamebuttonA , GamebuttonB;
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

void setup() {
  Serial.begin(38400);  //uart0 communication
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nJoystick initialization failed.")); //string inside f act as constant in program memory rather than loading in ram
    while (1); //halt
  }
  Serial.print(F("\r\nIncoming data"));
}



void loop() {
  Usb.Task();     //to check usb state **running or not ** high speed usb or not **usb task changed if vbus is changed and communicate with max3421e
  if (Xbox.XboxReceiverConnected) //check if the xbox wireless receiver is connected or not
  { 
      if (Xbox.Xbox360Connected[0])   //if joystick is on otherside is on
      {
        if (Xbox.getButtonPress(L2) || Xbox.getButtonPress(R2))  //if data are available on left and right analog buttons
        { 
         Serial.print("L2: ");
          Lefttrigger = int(Xbox.getButtonPress(L2)*100.0/255);
          Serial.print(int(Lefttrigger));
          Serial.print("\tR2: ");
          Righttrigger = int(Xbox.getButtonPress(R2)*100.0/255);
          Serial.println(int(Righttrigger));
         Xbox.setRumbleOn(Xbox.getButtonPress(L2), Xbox.getButtonPress(R2));  //Set vibrations on motor @para( leftvalue, rightvalue ,controller)
        }

        if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500 || Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500 || Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500 || Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) 
        { //if sticks are moved //right and up 100 max and left and down 0
          if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500) 
          {
            Serial.print(F("LeftHatX: "));
            LeftstickX = Compute(Xbox.getAnalogHat(LeftHatX));
            Serial.print(int(LeftstickX)); 
            Serial.print("\t");
          }
          if (Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500)
          {
            Serial.print(F("LeftHatY: "));
             LeftstickY = Compute(Xbox.getAnalogHat(LeftHatY));
            Serial.print(int(LeftstickY));
            Serial.print("\t");
          }
          if (Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500)
          {
            Serial.print(F("RightHatX: "));
            RightstickX = Compute(Xbox.getAnalogHat(RightHatX));
            Serial.print(int(RightstickX));
            Serial.print("\t");
          }
          if (Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500)
          {
            Serial.print(F("RightHatY: "));
             RightstickY = Compute(Xbox.getAnalogHat(RightHatY));
            Serial.print(int(RightstickY));
          }
          Serial.println();
        }

        if (Xbox.getButtonPress(UP))
        {
          Xbox.setLedOn(LED1);
          GamebuttonB |= (1<<0);
          Serial.println(F("Up"));
          
        }
        if (Xbox.getButtonPress(DOWN))
        {
          Xbox.setLedOn(LED4);
          GamebuttonB |= (1<<2);
          Serial.println(F("Down"));
        }
        if (Xbox.getButtonPress(LEFT))
        {
          Xbox.setLedOn(LED3);
          Serial.println(F("Left"));
          GamebuttonB |= (1<<3);
        }
        if (Xbox.getButtonPress(RIGHT))
        {
          Xbox.setLedOn(LED2);
          GamebuttonB |= (1<<1);
          Serial.println(F("Right"));
        }

        if (Xbox.getButtonClick(START))
        {
          Xbox.setLedMode(ALTERNATING); //opposite led are on
          Serial.println(F("Start"));
          GamebuttonA |= (1<<6);
        }
        if (Xbox.getButtonClick(BACK))
        {
          Xbox.setLedBlink(ALL); //all light with certain delay
          Serial.println(F("Back"));
          GamebuttonB |= (1<<4);
        }
         if (Xbox.getButtonClick(XBOX)) 
        {
          Xbox.setLedMode(ROTATING);
          Serial.print(F("Xbox (Battery: "));   //rotating led level
          Serial.print(Xbox.getBatteryLevel()); // The battery level in the range 0-3
          Serial.println(F(")"));
        }
        
        if (Xbox.getButtonClick(L3))
        {
          Serial.println(F("L3"));
          GamebuttonB |= (1<<5);
        }
        if (Xbox.getButtonClick(R3))
        {
          Serial.println(F("R3"));
          GamebuttonA |= (1<<4);
        }
        if (Xbox.getButtonClick(L1))
        {
          Serial.println(F("L1"));
          GamebuttonB |= (1<<6);
        }
        if (Xbox.getButtonClick(R1))
        {
          Serial.println(F("R1"));
          GamebuttonA |= (1<<5);
        }
         
        if (Xbox.getButtonClick(SYNC))
        {
          Serial.println(F("Sync"));
          Xbox.disconnect();
        }

        if (Xbox.getButtonClick(A))
        {
          Serial.println(F("A"));
          GamebuttonA |= (1<<0);
        }
        if (Xbox.getButtonClick(B))
        {
          Serial.println(F("B"));
          GamebuttonA |= (1<<1);
        }
        if (Xbox.getButtonClick(X))
        {
          Serial.println(F("X"));
          GamebuttonA |= (1<<2);
        }
        if (Xbox.getButtonClick(Y))
        {
          Serial.println(F("Y"));
          GamebuttonA |= (1<<3);
        }
     }
  }
}
