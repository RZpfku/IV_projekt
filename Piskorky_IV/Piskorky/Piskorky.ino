#include <TFT_eSPI.h>  //knižnice
#include <SPI.h>
TFT_eSPI tft = TFT_eSPI();

int fromTop = 60;  //premenné
int fromLeft = 10;
int space = 5;
int boxSize = 35;
int playGround = 3 * boxSize + 2 * space;

int i = 0;
int j = 0;

bool player = 0;  //hráč 1 alebo 2 (bool má 2 hodnoty a sú len 2ja hráči)
int hraciePole[3][3] = { 0 };
char sign[2] = { 'x', 'o' };  // hráč 1 má x a hráč 2 má o

void setup(void) {

  tft.init();  // nadstavenie začiatočnej pozície na displeji
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Piskorky", 5, 12, 4);
  pinMode(0, INPUT_PULLUP);   // ľavý button
  pinMode(35, INPUT_PULLUP);  // pravý button

  tft.drawFastVLine(fromLeft + boxSize + space / 2, fromTop, playGround, TFT_WHITE);  //zobrazenie hracieho poľa
  tft.drawFastVLine(1 + fromLeft + boxSize + space / 2, fromTop, playGround, TFT_WHITE);
  tft.drawFastVLine(fromLeft + boxSize * 2 + space + space / 2, fromTop, playGround, TFT_WHITE);
  tft.drawFastVLine(1 + fromLeft + boxSize * 2 + space + space / 2, fromTop, playGround, TFT_WHITE);
  tft.drawFastHLine(fromLeft, fromTop + boxSize + space / 2, playGround, TFT_WHITE);
  tft.drawFastHLine(fromLeft, 1 + fromTop + boxSize + space / 2, playGround, TFT_WHITE);
  tft.drawFastHLine(fromLeft, fromTop + boxSize * 2 + space + space / 2, playGround, TFT_WHITE);
  tft.drawFastHLine(fromLeft, 1 + fromTop + boxSize * 2 + space + space / 2, playGround, TFT_WHITE);

  tft.drawRect((fromLeft + (i * boxSize) + (i * space)), (fromTop + (j * boxSize) + (j * space)), boxSize, boxSize, TFT_RED);  // zobrazenie červeného štvorčeka na prvom políčku
}

void loop() {

  if (digitalRead(0) == 0)  // čo sa stane ak sa stlačí ľavý button (zmazanie aktuálneho červeného políčka, vykreslenie nasledujúceho)
  {
    tft.drawRect((fromLeft + (i * boxSize) + (i * space)), (fromTop + (j * boxSize) + (j * space)), boxSize, boxSize, TFT_BLACK);
    i = i + 1;
    if (i > 2) {
      i = 0;
      j++;
    }
    if (j > 2)
      j = 0;
    tft.drawRect((fromLeft + (i * boxSize) + (i * space)), (fromTop + (j * boxSize) + (j * space)), boxSize, boxSize, TFT_RED);
    delay(300);
  }

  if (digitalRead(35) == 0)  // čo sa stane ak sa stlačí pravý button   (x,o podľa aktuálneho hráča)
  {
    tft.drawString(String(sign[player]), 10 + fromLeft + (i * boxSize) + (space * i), 6 + fromTop + (j * boxSize) + j * space, 4);
    if (player == 0)
      hraciePole[i][j] = 1;

    if (player == 1)
      hraciePole[i][j] = 4;

    player = !player;
    checkWiner();
    delay(300);
  }
}

void checkWiner()  //kontrola víťaza
{
  for (int m = 0; m < 3; m++) {
    int score = 0;
    int hscore = 0;
    for (int n = 0; n < 3; n++) {
      score = score + hraciePole[m][n];
      hscore = hscore + hraciePole[n][m];

      if (score == 3 || score == 12)  //kontrola vertikálna
      {
        tft.drawFastVLine(fromLeft + (m * space) + (boxSize * m) + boxSize / 2, fromTop, playGround, TFT_RED);
        tft.drawFastVLine(fromLeft + 1 + (m * space) + (boxSize * m) + boxSize / 2, fromTop, playGround, TFT_RED);
      }

      if (hscore == 3 || hscore == 12)  //kontrola horizontálna
      {
        tft.drawFastHLine(fromLeft, fromTop + (m * space) + (boxSize * m) + boxSize / 2, playGround, TFT_RED);
        tft.drawFastHLine(fromLeft, fromTop + 1 + (m * space) + (boxSize * m) + boxSize / 2, playGround, TFT_RED);
      }
    }

    if (hraciePole[0][0] + hraciePole[1][1] + hraciePole[2][2] == 3 || hraciePole[0][0] + hraciePole[2][2] + hraciePole[1][1] == 12) {  //kontorla diagonala 1
      tft.drawLine(fromLeft, fromTop, fromLeft + playGround, fromTop + playGround, TFT_RED);
    }

    if (hraciePole[0][2] + hraciePole[1][1] + hraciePole[2][0] == 3 || hraciePole[0][2] + hraciePole[2][0] + hraciePole[1][1] == 12)  //kontorla diagonala 2
    {
      tft.drawLine(fromLeft + playGround, fromTop, fromLeft, fromTop + playGround, TFT_RED);
    }
  }
}
