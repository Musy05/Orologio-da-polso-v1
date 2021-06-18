#include "TinyWireM.h"
#include <DS3232RTC.h>
#include <Tiny4kOLED.h>

char Orologio[20];
char Data[20];
char Temperatura[20];

uint8_t width = 128;
uint8_t height = 64;

int h = 0;
int k = 0;
int w = 0;

int vin = 00.0;
int R1 = 100000;
int R2 = 10000;

DS3232RTC myRTC(false);
void  bordo() {
  for (uint8_t y = 0; y < 8; y++) {
    oled.setCursor(0, y);
    oled.startData();
    for (uint8_t x = 0; x < 128; x += 2) {
      oled.sendData(0b10101010);
      oled.sendData(0b01010101);
    }
    oled.endData();
  }

  oled.setCursor(0, 0);
  oled.startData();
  oled.sendData(0b11111111);
  oled.repeatData(0b00000001, width - 2);
  oled.sendData(0b11111111);
  oled.endData();

  for (uint8_t y = 1; y < (height - 8) / 8; y++) {
    oled.setCursor(0, y);
    oled.startData();
    oled.sendData(0b11111111);
    oled.repeatData(0b00000000, width - 2);
    oled.sendData(0b11111111);
    oled.endData();
  }

  oled.setCursor(0, (height - 8) / 8);
  oled.startData();
  oled.sendData(0b11111111);
  oled.repeatData(0b10000000, width - 2);
  oled.sendData(0b11111111);
  oled.endData();
}

void setup(void) {
  myRTC.begin();
  oled.begin(width, height, sizeof(tiny4koled_init_128x64br), tiny4koled_init_128x64br);
  oled.setFont(FONT8X16);
  oled.clear();
  oled.on();
  pinMode(4, INPUT);
  pinMode(3, INPUT);
}

void loop(void) {
  if (digitalRead(4) == HIGH) {
    bordo();
    oled.on();
    for (int i = 0; i < 150; i++) {
      time_t myTime;
      myTime = RTC.get();
      tmElements_t tm;
      RTC.read(tm);
      sprintf(Orologio, "%02d:%02d:%02d", tm.Hour, tm.Minute, tm.Second);
      sprintf(Data, "%02d/%02d/%04d", tm.Day, tm.Month, tm.Year + 1970);
      sprintf(Temperatura, "Temp:%02dC", RTC.temperature() / 4);
      oled.setCursor(32, 1);
      oled.print(Orologio);
      oled.setCursor(26, 35);
      oled.print(Data);
      oled.setCursor(34, 69);
      oled.print(Temperatura);
      if (digitalRead(4) == HIGH) {h++;};
      if (h > 50) {
        oled.clear();
        time_t myTime;
        myTime = RTC.get();
        tmElements_t tm;
        RTC.read(tm);
        oled.print("Impostazioni");
        oled.setCursor(0, 69);
        oled.print("Imposta: ora");
        int ore = tm.Hour;
        int minuti = tm.Minute;
        int secondi = tm.Second;
        int giorni = tm.Day;
        int mesi = tm.Month;
        int anni = tm.Year + 1970;
        
        w=0;
        delay(500);

        while (w < 6) {
          while (k < 100) {
            if (w<3) {
              sprintf(Orologio, "%02d:%02d:%02d", ore, minuti, secondi);
              oled.setCursor(0, 35);
              oled.print(Orologio);
          } else {
              sprintf(Data, "%02d/%02d/%04d", giorni, mesi, anni);
              oled.setCursor(0, 35);
              oled.print(Data);
          };
            switch (w) {
              case 0 : if (digitalRead(4) == HIGH) {ore++; delay(500); k = 0;} else {k++;};
                       if (ore > 23) {ore = 0;};
              break;
              case 1 : oled.setCursor(72, 69);
                       oled.print("minuti");
                       if (digitalRead(4) == HIGH) {minuti++; delay(500); k = 0;} else {k++;};
                       if (minuti > 59) {minuti = 0;};
              break;
              case 2 : oled.setCursor(72, 69);
                       oled.print("secondi");
                       if (digitalRead(4) == HIGH) {secondi++; delay(500); k = 0;} else {k++;};
                       if (secondi > 59) {secondi = 0;};
              break;
              case 3 : oled.setCursor(72, 69);
                       oled.print("giorno ");
                       if (digitalRead(4) == HIGH) {giorni++; delay(500); k = 0;} else {k++;};
                       if (giorni > 31) {giorni = 1;};
              break;
              case 4 : oled.setCursor(72, 69);
                       oled.print("mese  ");
                       if (digitalRead(4) == HIGH) {mesi++; delay(500); k = 0;} else {k++;};
                       if (mesi > 12) {mesi = 1;};
              break;
              case 5 : oled.setCursor(72, 69);
                       oled.print("anno ");
                       if (digitalRead(4) == HIGH) {anni++; delay(500); k = 0;} else {k++;};
                       if ((anni > 2030) || (anni < 2021)) {anni = 2021;};
              break;
            }}
            k = 0;
            w++;
            };
        setTime(ore, minuti, secondi, giorni, mesi, anni);
        RTC.set(now());
        oled.clear();
        oled.setCursor(0, 0);
        oled.print("Salvataggio...");
        delay(2000);
        oled.clear();
        i = 150;
        h = 0;
    }}
   } else {
    oled.off();
  }
}
