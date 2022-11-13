#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00004";

int VRx1 = A1;
int VRy1 = A0;
int SW1 = 2;

int x1Position = 0;
int y1Position = 0;
int SW_state_1 = 0;
int mapX1 = 0;
int mapY1 = 0;

int VRx2 = A3;
int VRy2 = A2;
int SW2 = 3;

int x2Position = 0;
int y2Position = 0;
int SW_state_2 = 0;
int mapX2 = 0;
int mapY2 = 0;

void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  Serial.begin(9600);

  pinMode(VRx1, INPUT);
  pinMode(VRy1, INPUT);
  pinMode(SW1, INPUT_PULLUP);

  pinMode(VRx2, INPUT);
  pinMode(VRy2, INPUT);
  pinMode(SW2, INPUT_PULLUP);

}

void loop() {
  char pos[] = {'D', 'f'};
  x1Position = analogRead(VRx1);
  y1Position = analogRead(VRy1);
  SW_state_1 = digitalRead(SW1);
  mapX1 = map(x1Position, 0, 1023, -512, 512);
  mapY1 = map(y1Position, 0, 1023, -512, 512);

  x2Position = analogRead(VRx2);
  y2Position = analogRead(VRy2);
  SW_state_2 = digitalRead(SW2);
  mapX2 = map(x2Position, 0, 1023, -512, 512);
  mapY2 = map(y2Position, 0, 1023, -512, 512);

  if (SW_state_1)
  {
    if (mapY1 <= 350 and mapY1 > 20)
    {
      pos[0] = 'M';
    }
    else if (mapY1 < 513 and mapY1 > 350)
    {
      pos[0] = 'F';
    }

    else
    {
      pos[0] = 'D';
    }

  }
  else
  {
    pos[0] = 'S';
  }

  if (SW_state_2)
  {
    if (mapY2 < 513 and mapY2 > 20)
    {
      pos[1] = 'u';
    }
     else if (mapY2 <= -20 and mapY2 > -513)
    {
      pos[1] = 'd';
    }

    else
    {
      pos[1] = 'x';
    }

  }
  else 
  {
    pos[1] = 'm';
  }


  Serial.print("X1: ");
  Serial.print(mapX1);
  Serial.print(" | Y1: ");
  Serial.print(mapY1);
  Serial.print(" | Button: ");
  Serial.print(SW_state_1);

  Serial.print(" | X2: ");
  Serial.print(mapX2);
  Serial.print(" | Y2: ");
  Serial.print(mapY2);
  Serial.print(" | Button: ");
  Serial.print(SW_state_2);

  Serial.print(" | Pos: ");
  Serial.println(pos);

  radio.write(&pos, sizeof(pos));
  delay(100);

}
