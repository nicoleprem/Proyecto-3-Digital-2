//Cristopher Sagastume
//Nicole Prem
//***************************************************************************************************************************************
/* Librería para el uso de la pantalla ILI9341 en modo 8 bits
   Basado en el código de martinayotte - https://www.stm32duino.com/viewtopic.php?t=637
   Adaptación, migración y creación de nuevas funciones: Pablo Mazariegos y José Morales
   Con ayuda de: José Guerra
   IE3027: Electrónica Digital 2 - 2019
*/
//***************************************************************************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "bitmaps.h"
#include "font.h"
#include "lcd_registers.h"

#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
//***************************************************************************************************************************************
// Functions Prototypes
//***************************************************************************************************************************************
void LCD_Init(void);
void LCD_CMD(uint8_t cmd);
void LCD_DATA(uint8_t data);
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_Clear(unsigned int c);
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void LCD_Print(String text, int x, int y, int fontSize, int color, int background);

void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[], int columns, int index, char flip, char offset);

const int SW1 = PA_3;
const int SW2 = PA_6;
const int SW3 = PA_2;
const int SW4 = PA_7;
const int SW5 = PA_4;//abajo
const int SW6 = PA_5; //arriba
char flagJuego = 0;
char flag = 0;
int PDD = 0;
int PDD2 = 0;
int SW1State = 0;
int SW2State = 0;
int SW3State = 0;
int SW4State = 0;
int SW5State = 0;
int SW6State = 0;
int v = 0;
int count = 0;
int count2 = 0;
int flag2 = true;
int flag3 = true;
int vida = 120;
int vida2 = 120;
//int daño = 30;
int r = 180;

extern uint8_t fondo[];
extern uint8_t ryu_img[];
extern uint8_t zangief_img[];
extern uint8_t ryuS[];
extern uint8_t suelo[];
extern uint8_t ryuP[];
extern uint8_t zangief[];
extern uint8_t zangiefP[];
//extern uint8_t Inicio[];

//***************************************************************************************************************************************
// Inicialización
//***************************************************************************************************************************************
void setup() {
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);
  pinMode(SW4, INPUT_PULLUP);
  pinMode(SW5, INPUT_PULLUP);
  pinMode(SW6, INPUT_PULLUP);
  SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
  Serial.begin(9600);
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial.println("Inicio");
  LCD_Init();
  LCD_Clear(0x00);

  //FillRect(0, 0, 319, 206, 0x421b);
  String text1 = "Super Mario World!";
  //LCD_Print(text1, 20, 100, 2, 0xffff, 0x421b);
  //LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);

  //LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);

  //LCD_Bitmap(0, 0, 320, 240, Inicio);
  //delay(3000);


}
//***************************************************************************************************************************************
// Loop Infinito
//***************************************************************************************************************************************
void loop() {
  LCD_Bitmap(0, 0, 320, 240, fondo); //imprime el fondo
  count = 0;
  count2 = 0;
  flag2 = true;
  flag3 = false;

  while (flag2 == true) {
    Serial.println("de nuevo");
    flag = 0;
    SW1State = digitalRead(SW1);
    SW2State = digitalRead(SW2);
    SW3State = digitalRead(SW3);
    SW4State = digitalRead(SW4);
    SW5State = digitalRead(SW5);
    SW6State = digitalRead(SW6);
    if (SW3State == LOW) {
      while (flag == 0) {
        SW3State = digitalRead(SW3);
        SW4State = digitalRead(SW4);
        if (SW3State == HIGH) {
          flag = 1;
        }
      }
      //Escoger personaje 1
      Serial.println("izquierda-arriba");
      delay(100);
      switch (count) {
        case 0:
          LCD_Bitmap(40, 39, 56, 79, zangief_img);
          count = 1;
          break;
        case 1:
          LCD_Bitmap(40, 39, 56, 79, ryu_img);
          count--;
          break;


      }
    }
    //escoger personaje 2
    else if (SW4State == LOW) {
      while (flag == 0) {
        SW3State = digitalRead(SW3);
        SW4State = digitalRead(SW4);
        if (SW4State == HIGH) {
          flag = 1;
        }
      }
      Serial.println("izquierda-abajo");
      delay(100);
      switch (count2) {
        case 0:
          LCD_Bitmap(224, 39, 56, 79, zangief_img);
          count2 = 1;
          break;
        case 1:
          LCD_Bitmap(224, 39, 56, 79, ryu_img);
          count2--;
          break;

      }
    }


    else if (SW1State == LOW) {
      while (flag == 0) {
        SW1State = digitalRead(SW1);
        SW2State = digitalRead(SW2);
        if (SW1State == HIGH) {
          flag = 1;
        }
      }
      Serial.println("Derecha-arriba");
      delay(100);
      switch (count) {
        case 0:
          LCD_Bitmap(40, 39, 56, 79, zangief_img);
          count++;
          break;
        case 1 :
          LCD_Bitmap(40, 39, 56, 79, ryu_img);
          count = 0;
          break;

      }
    }
    else if (SW2State == LOW) {
      while (flag == 0) {
        SW1State = digitalRead(SW1);
        SW2State = digitalRead(SW2);
        if (SW2State == HIGH) {
          flag = 1;
        }
      }
      Serial.println("derecha-abajo");
      delay(100);
      switch (count2) {
        case 0:
          LCD_Bitmap(224, 39, 56, 79, zangief_img);
          count2++;
          break;
        case 1:
          LCD_Bitmap(224, 39, 56, 79, ryu_img);
          count2 = 0;
          break;

      }
    }
    if (SW5State == LOW) {
      while (flag == 0) {
        SW5State = digitalRead(SW5);
        SW6State = digitalRead(SW6);
        if (SW5State == HIGH) {
          flag = 1;
        }
      }
      PDD = 1;
    }
    else if (SW6State == LOW) {
      while (flag == 0) {
        SW5State = digitalRead(SW5);
        SW6State = digitalRead(SW6);
        if (SW6State == HIGH) {
          flag = 1;
        }
      }
      PDD2 = 1;
    }
    //Si ambos jugadores escogen su peleador, se van a la otra pantalla
    if (PDD == 1 and PDD2 == 1) {
      PDD = 0;
      PDD2 = 0;
      flag3 = true;
      flag2 = false;
    }
  }
  LCD_Clear(0x00);
  LCD_Bitmap(0, 200, 320, 40, suelo);
  vida = 120;
  vida2 = 120;
  v = 0;
  r = 180;

  while (flag3 == true) {

    if (vida == 0) {
      Serial.println("terminó");
      flag2 = true;
      flag3 = false;

    }
    for (int x = 0; x < 340; x++) {
      FillRect(vida + 6, 10, 60 , 10, 0x0000);
      //FillRect(195, 10, 120, 10, 0x0000);
      FillRect(5, 10, vida, 10, 0xFFE0);
      FillRect(195, 10, vida2, 10, 0xFFE0);
      Serial.println(v);
      Serial.println(r);
      Serial.println((r - (v + 52)));
      Serial.println(vida);
      Serial.println(x);
      int anim2 = (x / 10) % 2;
      LCD_Sprite(r, 130, 40, 70, zangief, 2, anim2, 1, 0); //movimiento del jugador
      V_line(r-1, 130, 70, 0x00);
      //int anim = (x / 30) % 2;
      LCD_Sprite(v, 125, 29, 72, ryuS, 2, anim2, 0, 0);
      //ciclo del juego
      flag = 0;
      SW5State = digitalRead(SW5);
      SW6State = digitalRead(SW6);
      SW1State = digitalRead(SW1);
      SW2State = digitalRead(SW2);
      //antirebote del jugador 1
      if (SW1State == LOW) {
        while (flag == 0) {
          SW1State = digitalRead(SW1);
          SW2State = digitalRead(SW2);
          if (SW1State == HIGH) {
            flag = 1;
          }
        }
        v++;
        v++;
        v++;
        v++;
        v++;
        v++;
        v++;
        v++;
        if (v == 340) {
          v--;
          v--;
          v--;
          v--;
          v--;
          v--;
          v--;
          v--;
        }
        LCD_Sprite(v, 125, 29, 72, ryuS, 2, 0, 0, 0);
        delay(300);
        LCD_Sprite(v, 125, 29, 72, ryuS, 2, 0, 0, 0);
        for (int z = 0; z == v; z++) {
          V_line(z, 125, 200, 0xFFFF);
        }
      }

      //antirebote del jugador 1
      if (SW5State == LOW) {
        while (flag == 0) {
          SW6State = digitalRead(SW5);
          SW5State = digitalRead(SW6);
          if (SW5State == HIGH) {
            flag = 1;
          }
        }

        LCD_Sprite(v, 125, 52, 70, ryuP, 2, 0, 0, 0);
        delay(120);
        LCD_Sprite(v, 125, 52, 70, ryuP, 2, 1, 0, 0);
        for (int x = 29; x <= 50; x++) {
          V_line(v-1, 125, 150, 0x0000);
          //delay(15);
        }
        delay(15);
        if ((r - (v + 52)) == 0) {


          for (int l = 0; l <= 29; l++) {
            vida--;
            if (vida == 0) {
              Serial.println("terminó");
              flag2 = true;
              flag3 = false;
              x = 335;
            }
          }


        }
      }

      //antirebote jugador 2
      else if (SW6State == LOW) {
        while (flag == 0) {
          SW5State = digitalRead(SW5);
          SW6State = digitalRead(SW6);
          if (SW6State == HIGH) {
            flag = 1;
          }
        }
        //aumento de contador jugador 2 y encendido de leds

        LCD_Sprite(80, 130, 45, 66, zangiefP, 2, 0, 1, 0);
        delay(300);
        LCD_Sprite(80, 130, 45, 66, zangiefP, 2, 1, 1, 0);
        for (int x = 120; x <= 130; x++) {
          V_line(x-1, 130, 150, 0x0000);
          //delay(15);
        }
      }
    }
    Serial.println("salio");
  }
}
//***************************************************************************************************************************************
// Función para inicializar LCD
//***************************************************************************************************************************************
void LCD_Init(void) {
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RD, OUTPUT);
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(DPINS[i], OUTPUT);
  }
  //****************************************
  // Secuencia de Inicialización
  //****************************************
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_RST, HIGH);
  delay(5);
  digitalWrite(LCD_RST, LOW);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  delay(150);
  digitalWrite(LCD_CS, LOW);
  //****************************************
  LCD_CMD(0xE9);  // SETPANELRELATED
  LCD_DATA(0x20);
  //****************************************
  LCD_CMD(0x11); // Exit Sleep SLEEP OUT (SLPOUT)
  delay(100);
  //****************************************
  LCD_CMD(0xD1);    // (SETVCOM)
  LCD_DATA(0x00);
  LCD_DATA(0x71);
  LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0xD0);   // (SETPOWER)
  LCD_DATA(0x07);
  LCD_DATA(0x01);
  LCD_DATA(0x08);
  //****************************************
  LCD_CMD(0x36);  // (MEMORYACCESS)
  LCD_DATA(0x40 | 0x80 | 0x20 | 0x08); // LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0x3A); // Set_pixel_format (PIXELFORMAT)
  LCD_DATA(0x05); // color setings, 05h - 16bit pixel, 11h - 3bit pixel
  //****************************************
  LCD_CMD(0xC1);    // (POWERCONTROL2)
  LCD_DATA(0x10);
  LCD_DATA(0x10);
  LCD_DATA(0x02);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC0); // Set Default Gamma (POWERCONTROL1)
  LCD_DATA(0x00);
  LCD_DATA(0x35);
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC5); // Set Frame Rate (VCOMCONTROL1)
  LCD_DATA(0x04); // 72Hz
  //****************************************
  LCD_CMD(0xD2); // Power Settings  (SETPWRNORMAL)
  LCD_DATA(0x01);
  LCD_DATA(0x44);
  //****************************************
  LCD_CMD(0xC8); //Set Gamma  (GAMMASET)
  LCD_DATA(0x04);
  LCD_DATA(0x67);
  LCD_DATA(0x35);
  LCD_DATA(0x04);
  LCD_DATA(0x08);
  LCD_DATA(0x06);
  LCD_DATA(0x24);
  LCD_DATA(0x01);
  LCD_DATA(0x37);
  LCD_DATA(0x40);
  LCD_DATA(0x03);
  LCD_DATA(0x10);
  LCD_DATA(0x08);
  LCD_DATA(0x80);
  LCD_DATA(0x00);
  //****************************************
  LCD_CMD(0x2A); // Set_column_address 320px (CASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x3F);
  //****************************************
  LCD_CMD(0x2B); // Set_page_address 480px (PASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0xE0);
  //  LCD_DATA(0x8F);
  LCD_CMD(0x29); //display on
  LCD_CMD(0x2C); //display on

  LCD_CMD(ILI9341_INVOFF); //Invert Off
  delay(120);
  LCD_CMD(ILI9341_SLPOUT);    //Exit Sleep
  delay(120);
  LCD_CMD(ILI9341_DISPON);    //Display on
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar comandos a la LCD - parámetro (comando)
//***************************************************************************************************************************************
void LCD_CMD(uint8_t cmd) {
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = cmd;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar datos a la LCD - parámetro (dato)
//***************************************************************************************************************************************
void LCD_DATA(uint8_t data) {
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = data;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para definir rango de direcciones de memoria con las cuales se trabajara (se define una ventana)
//***************************************************************************************************************************************
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  LCD_CMD(0x2a); // Set_column_address 4 parameters
  LCD_DATA(x1 >> 8);
  LCD_DATA(x1);
  LCD_DATA(x2 >> 8);
  LCD_DATA(x2);
  LCD_CMD(0x2b); // Set_page_address 4 parameters
  LCD_DATA(y1 >> 8);
  LCD_DATA(y1);
  LCD_DATA(y2 >> 8);
  LCD_DATA(y2);
  LCD_CMD(0x2c); // Write_memory_start
}
//***************************************************************************************************************************************
// Función para borrar la pantalla - parámetros (color)
//***************************************************************************************************************************************
void LCD_Clear(unsigned int c) {
  unsigned int x, y;
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  SetWindows(0, 0, 319, 239); // 479, 319);
  for (x = 0; x < 320; x++)
    for (y = 0; y < 240; y++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c);
    }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea horizontal - parámetros ( coordenada x, cordenada y, longitud, color)
//***************************************************************************************************************************************
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  SetWindows(x, y, l, y);
  j = l;// * 2;
  for (i = 0; i < l; i++) {
    LCD_DATA(c >> 8);
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada y, longitud, color)
//***************************************************************************************************************************************
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + y;
  SetWindows(x, y, x, l);
  j = l; //* 2;
  for (i = 1; i <= j; i++) {
    LCD_DATA(c >> 8);
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  H_line(x  , y  , w, c);
  H_line(x  , y + h, w, c);
  V_line(x  , y  , h, c);
  V_line(x + w, y  , h, c);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo relleno - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  unsigned int i;
  for (i = 0; i < h; i++) {
    H_line(x  , y  , w, c);
    H_line(x  , y + i, w, c);
  }
}
//***************************************************************************************************************************************
// Función para dibujar texto - parámetros ( texto, coordenada x, cordenada y, color, background)
//***************************************************************************************************************************************
void LCD_Print(String text, int x, int y, int fontSize, int color, int background) {
  int fontXSize ;
  int fontYSize ;

  if (fontSize == 1) {
    fontXSize = fontXSizeSmal ;
    fontYSize = fontYSizeSmal ;
  }
  if (fontSize == 2) {
    fontXSize = fontXSizeBig ;
    fontYSize = fontYSizeBig ;
  }

  char charInput ;
  int cLength = text.length();
  Serial.println(cLength, DEC);
  int charDec ;
  int c ;
  int charHex ;
  char char_array[cLength + 1];
  text.toCharArray(char_array, cLength + 1) ;
  for (int i = 0; i < cLength ; i++) {
    charInput = char_array[i];
    Serial.println(char_array[i]);
    charDec = int(charInput);
    digitalWrite(LCD_CS, LOW);
    SetWindows(x + (i * fontXSize), y, x + (i * fontXSize) + fontXSize - 1, y + fontYSize );
    long charHex1 ;
    for ( int n = 0 ; n < fontYSize ; n++ ) {
      if (fontSize == 1) {
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontYSize) + n);
      }
      if (fontSize == 2) {
        charHex1 = pgm_read_word_near(bigFont + ((charDec - 32) * fontYSize) + n);
      }
      for (int t = 1; t < fontXSize + 1 ; t++) {
        if (( charHex1 & (1 << (fontXSize - t))) > 0 ) {
          c = color ;
        } else {
          c = background ;
        }
        LCD_DATA(c >> 8);
        LCD_DATA(c);
      }
    }
    digitalWrite(LCD_CS, HIGH);
  }
}
//***************************************************************************************************************************************
// Función para dibujar una imagen a partir de un arreglo de colores (Bitmap) Formato (Color 16bit R 5bits G 6bits B 5bits)
//***************************************************************************************************************************************
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);

  unsigned int x2, y2;
  x2 = x + width;
  y2 = y + height;
  SetWindows(x, y, x2 - 1, y2 - 1);
  unsigned int k = 0;
  unsigned int i, j;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k + 1]);
      //LCD_DATA(bitmap[k]);
      k = k + 2;
    }
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una imagen sprite - los parámetros columns = número de imagenes en el sprite, index = cual desplegar, flip = darle vuelta
//***************************************************************************************************************************************
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[], int columns, int index, char flip, char offset) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);

  unsigned int x2, y2;
  x2 =   x + width;
  y2 =    y + height;
  SetWindows(x, y, x2 - 1, y2 - 1);
  int k = 0;
  int ancho = ((width * columns));
  if (flip) {
    for (int j = 0; j < height; j++) {
      k = (j * (ancho) + index * width - 1 - offset) * 2;
      k = k + width * 2;
      for (int i = 0; i < width; i++) {
        LCD_DATA(bitmap[k]);
        LCD_DATA(bitmap[k + 1]);
        k = k - 2;
      }
    }
  } else {
    for (int j = 0; j < height; j++) {
      k = (j * (ancho) + index * width + 1 + offset) * 2;
      for (int i = 0; i < width; i++) {
        LCD_DATA(bitmap[k]);
        LCD_DATA(bitmap[k + 1]);
        k = k + 2;
      }
    }


  }
  digitalWrite(LCD_CS, HIGH);
}
