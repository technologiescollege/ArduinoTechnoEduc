#include <Servo.h>
#include <EEPROM.h>
#include <Wire.h>
#include <IRremote_TechnoZone51.h>
#include <LiquidCrystal_I2C_TechnoZone51.h>
#include <DS1307new.h>

//*********************************************************
//  L'interpréteur de commande pour la carte Arduino Mega *
//  Auteur : www.technozone51.fr ( Grosse Christophe )    *
//  Licence : LGPL                                        *
//  Version 2.8                               du 10/11/14 *
//  Pour Compilation avec Arduino 1.05                    *
//*********************************************************

#define __mega_ver 1          //Commenter cette ligne pour compilation sur UNO, DECIMILLIA, FREEDUINO ...

#if defined(__mega_ver)
#define first_pin_eeprom 0    //Adresse de départ de la table de brochage dans l'EEPROM
#define last_pin_eeprom 70    //Adresse de fin de la table de brochage dans l'EEPROM
#define first_name_eeprom 71  //Adresse de départ de la chaine d'Identification de la maquette ( 6 caractêres de long )
#define autorun 77            //Adresse en EEPROM du flag autorun. Si il vaut 1 alors les programmes dans les slots activés sont exécutés à chaque reset de la carte
#define lcd_connected 78      //Adresse en EEPROM du flag lcd_connected. Si il vaut 1 alors cela indique qu'il y a un LCD de connecté via la prise I2C
#define lcd_i2c_add 79        //Adresse en EEPROM I2C de l'écran LCD           
#define run 80               //Adresse en EEPROM des flags RUNNING qui spécifie quelle slot de mémoire doit être interprété sur 33 octets
                              //run[79+slot] = 0 : Le programme dans le slot n° slot ne doit pas être interprété
                              //run[79+slot] = 1 : Le programme dans le slot n° slot doit être interprété 
#define name_slot 113        //Adresse en EEPROM du nom des programme stocké sur 33 * 8 =  264 octets
                             //name_slot[113+(n°slot-1)*8+n°caractere] pour accéder au n°caractère du nom du slot n°slot 

#define lcd_col 377           //Adresse en EEPROM du nombre de colonnes du LCD
#define lcd_row 378           //Adresse en EEPROM du nombre de Ligne du LCD
#define baud_rate 379         //Adresse en EEPROM de la vitesse de communication série

#define first_opcode 410     //Adresse de départ du slot mémoire interne EEPROM de l'arduino
//Adresse de la premiere mémoire externe BANK0
#define MEM_ADDR 0x50


#else

#define first_pin_eeprom 0    //Adresse de départ de la table de brochage dans l'EEPROM
#define last_pin_eeprom 13    //Adresse de fin de la table de brochage dans l'EEPROM
#define first_name_eeprom 30  //Adresse de départ de la chaine d'Identification de la maquette ( 6 caractêres de long )
#define autorun 37           //Adresse en EEPROM du flag autorun. Si il vaut 1 alors les programmes dans les slots activés sont exécutés à chaque reset de la carte
#define lcd_connected 38      //Adresse en EEPROM du flag lcd_connected. Si il vaut 1 alors cela indique qu'il y a un LCD de connecté via la prise I2C
#define lcd_i2c_add 39        //Adresse en EEPROM I2C de l'écran LCD           
#define run 40               //Adresse en EEPROM des flags RUNNING qui spécifie quelle slot de mémoire doit être interprété sur 33 octets
                              //run[39+slot] = 0 : Le programme dans le slot n° slot ne doit pas être interprété
                              //run[39+slot] = 1 : Le programme dans le slot n° slot doit être interprété 
#define name_slot 49         //Adresse en EEPROM du nom des programme stocké sur 9 * 8 =  72 octets
                             //name_slot[113+(n°slot-1)*8+n°caractere] pour accéder au n°caractère du nom du slot n°slot 
#define lcd_col 130          //Adresse en EEPROM du nombre de colonnes du LCD
#define lcd_row 131           //Adresse en EEPROM du nombre de Ligne du LCD

#define first_opcode 132     //Adresse de départ du slot mémoire interne EEPROM de l'arduino
#define max_slot 8           //Numéro du dernier slot disponible


//Adresse de la premiere mémoire externe BANK0
#define MEM_ADDR 0x50

#endif


//Les variables globales utilisées par le programme
#if defined(__mega_ver)
boolean debug=0;         //Mettre a 1 pour cession de débugage sinon à 0
#endif

uint8_t hor;            //Pour la lecture horaire
int incomingByte = 0;	// for incoming serial data
boolean time_out;       // Flag de time_out
int no=0;               // Stockage du numéro de broche
int n1;
int n2;
int n3;
int n4;
int n5;
int n6;
int n7;
int n8;
int n9;
int n10;
int n11;
int n12;
int n13;
int value=0;            //Stockage d'une valeur
int digit_u=0;          //stockage du digit des unités
int digit_d=0;          //stockage du digit des dizaines
int digit_c=0;          //stockage du digit des centaines
unsigned long time_d;    //Utilisé pour gérer le time out, stocke le début du chronométrage
unsigned long time_f;    //Utilisé pour gérer le time out, stocke la fin du cronométrage
//word programme_size;   //Utilisé pour stocker la longueur du programme compilé à charger en EEPROM
byte slot_p;             //Slot utilisée pour programmer le programme (commande F et B)
word pc;                 //program counter : utilisé comme pointeur sur l'OPCODE en cours d'interprétation

#if defined(__mega_ver)
 word pcs[33];            //Program counter des différents slots n°1 à ,n°33
 int variable[26];       //Un tableau contenant les 26 variables utilisés par le programme en cours d'interprétation.
 int variables[828];     //Stockage des 26*33=828 variables des différents slots n°1 à °33
 unsigned long intempo[33];//Flag du temps de pause restant pour un programme qui exécute une temporisation
 //Déclaration des servomoteurs susceptibles d'être connectés
 Servo myservo14;
 Servo myservo15;
 Servo myservo16;
 Servo myservo17;
 Servo myservo18;
 Servo myservo19;
 Servo myservo22;
 Servo myservo23;
 Servo myservo24;
 Servo myservo25;
 Servo myservo26;
 Servo myservo27;
#else
word pcs[9];            //Program counter des différents slots n°1 à ,n°9
int variable[26];       //Un tableau contenant les 26 variables utilisés par le programme en cours d'interprétation.
int variables[234];     //Stockage des 26*9=234 variables des différents slots n°1 à °9
unsigned long intempo[9];//Flag du temps de pause restant pour un programme qui exécute une temporisation
Servo myservo4;
Servo myservo5;
Servo myservo6;
Servo myservo7;
Servo myservo8;
Servo myservo9;
Servo myservo10;
Servo myservo11;
Servo myservo12;
Servo myservo13;
#endif

//Déclaration de l'écran LCD
LiquidCrystal_I2C lcd(EEPROM.read(lcd_i2c_add),EEPROM.read(lcd_col),EEPROM.read(lcd_row));  // set the LCD address to 0x27 for a 16 chars and 2 line display

//Déclaration de la broche utilisée pour le récepteur infrarouge
int RECV_PIN = 11; //Broche par défaut du récepteur Infrarouge
IRrecv irrecv(RECV_PIN);
decode_results results;

//Routine génération d'un son sur une des broches 
void buzz(int targetpin,long frequency,long length){
 long delayvalue=1000000/frequency/2; //calcul le delay entre deux transitions
 long numcycles=frequency*length/1000; //Calculm du nombre de cycles 
 for (long i=0;i<numcycles;i++){
   digitalWrite(targetpin,HIGH);
   delayMicroseconds(delayvalue);
   digitalWrite(targetpin,LOW);
   delayMicroseconds(delayvalue);
 }  
}
//Routine ecriture mémoire EEPROM Externe
void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, byte data ) {
  int rdata = data;
  Wire.beginTransmission(deviceaddress);
  Wire.write((byte)(eeaddress >> 8)); // MSB
  Wire.write((byte)(eeaddress & 0xFF)); // LSB
  Wire.write(rdata);
  Wire.endTransmission();
}

//Calcule le nombre de jours écoulés depuis le 01/03/00
unsigned long g(byte j,byte m2, byte y2) {
 byte m=(m2 +9)%12;
 unsigned long y=(y2+2000)-m/10;
 return 365*y+y/4-y/100+y/400+(m*306+5)/10+(j-1); 
}  

//Calcule le nombre de jours qui séparent deux dates
unsigned long diff_date(byte j1,byte m1, byte y1, byte j2, byte m2, byte y2) {
  return g(j2,m2,y2)-g(j1,m1,y1);
}  

//Variable horaire h3=h2-h1;
void duree(byte h1,byte h2,byte h3){
 byte retenu=0;
 byte a;
 byte b;
 //Calcul de la différence des secondes
 RTC.getRAM(h2*7+5,&hor,1);
 a=hor;
 RTC.getRAM(h1*7+5,&hor,1);
 b=hor;
 if (b>a) 
 {
   hor=a+60-b;
   retenu=1;
 } else
 {
   hor=a-b;
   retenu=0;
 }
 RTC.setRAM(h3*7+5,&hor,1);
 //Calcul de la différence des minutes
 RTC.getRAM(h2*7+4,&hor,1);
 a=hor;
 RTC.getRAM(h1*7+4,&hor,1);
 b=hor+retenu;
 if (b>a) 
 {
   hor=a+60-b;
   retenu=1;
 } else
 {
   hor=a-b;
   retenu=0;
 }
 RTC.setRAM(h3*7+4,&hor,1);
 //Calcul de la différence des heures
 RTC.getRAM(h2*7+3,&hor,1);
 a=hor;
 RTC.getRAM(h1*7+3,&hor,1);
 b=hor+retenu;
 if (b>a) 
 {
   hor=a+24-b;
   retenu=1;
 } else
 {
   hor=a-b;
   retenu=0;
 }
 RTC.setRAM(h3*7+3,&hor,1);
 //Calcul de la différence de jours
 byte j1,j2,m1,m2,a1,a2;
 RTC.getRAM(h1*7,&hor,1);
 j1=hor;
 RTC.getRAM(h1*7+1,&hor,1);
 m1=hor;
 RTC.getRAM(h1*7+2,&hor,1);
 a1=hor;
 RTC.getRAM(h2*7,&hor,1);
 j2=hor;
 RTC.getRAM(h2*7+1,&hor,1);
 m2=hor;
 RTC.getRAM(h2*7+2,&hor,1);
 a2=hor;
 hor=(byte)diff_date(j1,m1,a1,j2,m2,a2)-retenu;
 RTC.setRAM(h3*7,&hor,1);
 hor=0;
 RTC.setRAM(h3*7+1,&hor,1);
 RTC.setRAM(h3*7+2,&hor,1);
 RTC.setRAM(h3*7+6,&hor,1);
}  

//Routine lecture mémoire EEPROM Externe
byte i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress ) {
  byte rdata = 0xFF;
  Wire.beginTransmission(deviceaddress);
  Wire.write((byte)(eeaddress >> 8)); // MSB
  Wire.write((byte)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress,1);
  if (Wire.available()) {
   rdata = Wire.read();
  }
/*  Serial.print("Lecture octet ");
  Serial.print(eeaddress,DEC);
  Serial.print(" = ");
  Serial.println(rdata,DEC);
  delay(1000); */
  return rdata;
}

//Routine qui renvoie la bank de mémoire correspondant à un slot mémoire
byte get_bank(unsigned int slot) {
  if (slot<9) { return 0; }
  if ((slot>8)&&(slot<17)) {return 1;}
  if ((slot>16)&&(slot<25)) {return 2;}
  if (slot>24) {return 3;}
 }

//Routine  de lecture du programme en mémoire EEPROM Interne ou Externe
byte lire_eeprom(unsigned int slot, unsigned int eeaddress ) {
  if (slot==0) {
  //Lecture de la mémoire EEPROM interne de l'Arduino
  return EEPROM.read(eeaddress);
  } else {
  //Lecture de la mémoire EEPROM externe pointé par <target>
  return i2c_eeprom_read_byte(MEM_ADDR+get_bank(slot), eeaddress);
  } 
}

//Routine de conversion et d'affichage de la température en °C
void affiche_celsius(int valeur) {
 float temperature=valeur*-0.525641025+116.0897436;
 if (temperature>=0) {
   //On affiche le signe +
   lcd.print("+");
   }
   lcd.print(temperature);
   lcd.write(223);
   lcd.print('C');
}

//Routine de conversion et d'affichage de la luminosité en Lux
void affiche_lux(int valeur) {
  int lux;
  switch (valeur) {
   case 0 : lux=1517; 
            break; 
   case 1 : lux=1477; 
            break;
   case 2 : lux=1438; 
            break;
   case 3 : lux=1399; 
            break;
   case 4 : lux=1362; 
            break;
   case 5 : lux=1326; 
            break;
   case 6 : lux=1291; 
            break;
   case 7 : lux=1256; 
            break;
   case 8 : lux=1223; 
            break;
   case 9 : lux=1190; 
            break;
   case 10 : lux=1159; 
            break;
   case 11 : lux=1128; 
            break;
   case 12 : lux=1098; 
            break;
   case 13 : lux=1069; 
            break;
   case 14 : lux=1040; 
            break;
   case 15 : lux=1013; 
            break;
   case 16 : lux=986; 
            break;
   case 17 : lux=960; 
            break;
   case 18 : lux=934; 
            break;
   case 19 : lux=909; 
            break;
   case 20 : lux=885; 
            break;
   case 21 : lux=861; 
            break;
   case 22 : lux=839; 
            break;
   case 23 : lux=816; 
            break;
   case 24 : lux=795; 
            break;
   case 25 : lux=773; 
            break;
   case 26 : lux=753; 
            break;
   case 27 : lux=733; 
            break;
   case 28 : lux=713; 
            break;
   case 29 : lux=694; 
            break;
   case 30 : lux=676; 
            break;
   case 31 : lux=658; 
            break;
   case 32 : lux=640; 
            break;
   case 33 : lux=623; 
            break;
   case 34 : lux=607; 
            break;
   case 35 : lux=591; 
            break;
   case 36 : lux=575; 
            break;
   case 37 : lux=560; 
            break;
   case 38 : lux=545; 
            break;
   case 39 : lux=530; 
            break;
   case 40 : lux=516; 
            break;
   case 41 : lux=502; 
            break;
   case 42 : lux=489; 
            break;
   case 43 : lux=476; 
            break;
   case 44 : lux=463; 
            break;
   case 45 : lux=451; 
            break;
   case 46 : lux=439; 
            break;
   case 47 : lux=427; 
            break;
   case 48 : lux=416; 
            break;
   case 49 : lux=405; 
            break;
   case 50 : lux=394; 
            break;
   case 51 : lux=384; 
            break;
   case 52 : lux=374; 
            break;
   case 53 : lux=364; 
            break;
   case 54 : lux=354; 
            break;
   case 55 : lux=345; 
            break;
   case 56 : lux=335; 
            break;
   case 57 : lux=326; 
            break;
   case 58 : lux=318; 
            break;
   case 59 : lux=309; 
            break;
   case 60 : lux=301;   
            break;
   case 61 : lux=293; 
            break;
   case 62 : lux=285; 
            break;
   case 63 : lux=278; 
            break;
   case 64 : lux=270; 
            break;
   case 65 : lux=263; 
            break;
   case 66 : lux=256; 
            break;
   case 67 : lux=249; 
            break;
   case 68 : lux=243; 
            break;
   case 69 : lux=236; 
            break;
   case 70 : lux=230;   
            break;
   case 71 : lux=224; 
            break;
   case 72 : lux=218; 
            break;
   case 73 : lux=212; 
            break;
   case 74 : lux=206; 
            break;
   case 75 : lux=201; 
            break;
   case 76 : lux=196; 
            break;
   case 77 : lux=190; 
            break;
   case 78 : lux=185; 
            break;
   case 79 : lux=180; 
            break;
   case 80 : lux=176;   
            break;
   case 81 : lux=171; 
            break;
   case 82 : lux=166; 
            break;
   case 83 : lux=162; 
            break;
   case 84 : lux=158; 
            break;
   case 85 : lux=153; 
            break;
   case 86 : lux=149; 
            break;
   case 87 : lux=145; 
            break;
   case 88 : lux=142; 
            break;
   case 89 : lux=138; 
            break;
   case 90 : lux=134;   
            break;
   case 91 : lux=131; 
            break;
   case 92 : lux=127; 
            break;
   case 93 : lux=124; 
            break;
   case 94 : lux=120; 
            break;
   case 95 : lux=117; 
            break;
   case 96 : lux=114; 
            break;
   case 97 : lux=111; 
            break;
   case 98 : lux=108; 
            break;
   case 99 : lux=105; 
            break;
   case 100 : lux=102;   
            break;
   case 101 : lux=100; 
            break;
   case 102 : lux=97; 
            break;
   case 103 : lux=94; 
            break;
   case 104 : lux=92; 
            break;
   case 105 : lux=90; 
            break;
   case 106 : lux=87; 
            break;
   case 107 : lux=85; 
            break;
   case 108 : lux=83; 
            break;
   case 109 : lux=80; 
            break;
   case 110 : lux=78; 
            break;
   case 111 : lux=76; 
            break;
   case 112 : lux=74; 
            break;
   case 113 : lux=72; 
            break;
   case 114 : lux=70; 
            break;
   case 115 : lux=68; 
            break;
   case 116 : lux=67; 
            break;
   case 117 : lux=65; 
            break;
   case 118 : lux=63; 
            break;
   case 119 : lux=61; 
            break;
   case 120 : lux=60; 
            break;
   case 121 : lux=58; 
            break;
   case 122 : lux=57; 
            break;
   case 123 : lux=55; 
            break;
   case 124 : lux=54; 
            break;
   case 125 : lux=52; 
            break;
   case 126 : lux=51; 
            break;
   case 127 : lux=49; 
            break;
   case 128 : lux=48; 
            break;
   case 129 : lux=47; 
            break;
   case 130 : lux=46; 
            break;
   case 131 : lux=44; 
            break;
   case 132 : lux=43; 
            break;
   case 133 : lux=42; 
            break;
   case 134 : lux=41; 
            break;
   case 135 : lux=40; 
            break;
   case 136 : lux=39; 
            break;
   case 137 : lux=38; 
            break;
   case 138 : lux=37; 
            break;
   case 139 : lux=36; 
            break;
   case 140 : lux=35; 
            break;
   case 141 : lux=34; 
            break;
   case 142 : lux=33; 
            break;
   case 143 : lux=32; 
            break;
   case 144 : lux=31; 
            break;
   case 145 : lux=30; 
            break;
   case 146 : lux=30; 
            break;
   case 147 : lux=29; 
            break;
   case 148 : lux=28; 
            break;
   case 149 : lux=27; 
            break;
   case 150 : lux=27; 
            break;
   case 151 : lux=26; 
            break;
   case 152 : lux=25; 
            break;
   case 153 : lux=25; 
            break;
   case 154 : lux=24; 
            break;
   case 155 : lux=23; 
            break;
   case 156 : lux=23; 
            break;
   case 157 : lux=22; 
            break;
   case 158 : lux=21; 
            break;
   case 159 : lux=21; 
            break;
   case 160 : lux=20;   
            break;
   case 161 : lux=20; 
            break;
   case 162 : lux=19; 
            break;
   case 163 : lux=19; 
            break;
   case 164 : lux=18; 
            break;
   case 165 : lux=18; 
            break;
   case 166 : lux=17; 
            break;
   case 167 : lux=17; 
            break;
   case 168 : lux=16; 
            break;
   case 169 : lux=16; 
            break;
   case 170 : lux=16;   
            break;
   case 171 : lux=15; 
            break;
   case 172 : lux=15; 
            break;
   case 173 : lux=14; 
            break;
   case 174 : lux=14; 
            break;
   case 175 : lux=14; 
            break;
   case 176 : lux=13; 
            break;
   case 177 : lux=13; 
            break;
   case 178 : lux=13; 
            break;
   case 179 : lux=12; 
            break;
   case 180 : lux=12;   
            break;
   case 181 : lux=12; 
            break;
   case 182 : lux=11; 
            break;
   case 183 : lux=11; 
            break;
   case 184 : lux=11; 
            break;
   case 185 : lux=10; 
            break;
   case 186 : lux=10; 
            break;
   case 187 : lux=10; 
            break;
   case 188 : lux=10; 
            break;
   case 189 : lux=9; 
            break;
   case 190 : lux=9;   
            break;
   case 191 : lux=9; 
            break;
   case 192 : lux=9; 
            break;
   case 193 : lux=8; 
            break;
   case 194 : lux=8; 
            break;
   case 195 : lux=8; 
            break;
   case 196 : lux=8; 
            break;
   case 197 : lux=7; 
            break;
   case 198 : lux=7; 
            break;
   case 199 : lux=7; 
            break;
   case 200 : lux=7;   
            break;
   case 201 : lux=7; 
            break;
   case 202 : lux=7; 
            break;
   case 203 : lux=6; 
            break;
   case 204 : lux=6; 
            break;
   case 205 : lux=6; 
            break;
   case 206 : lux=6; 
            break;
   case 207 : lux=6; 
            break;
   case 208 : lux=6; 
            break;
   case 209 : lux=5; 
            break;
   case 210 : lux=5; 
            break;
   case 211 : lux=5; 
            break;
   case 212 : lux=5; 
            break;
   case 213 : lux=5; 
            break;
   case 214 : lux=5; 
            break;
   case 215 : lux=5; 
            break;
   case 216 : lux=4; 
            break;
   case 217 : lux=4; 
            break;
   case 218 : lux=4; 
            break;
   case 219 : lux=4; 
            break;
   case 220 : lux=4; 
            break;
   case 221 : lux=4; 
            break;
   case 222 : lux=4; 
            break;
   case 223 : lux=4; 
            break;
   case 224 : lux=4; 
            break;
   case 225 : lux=4; 
            break;
   case 226 : lux=3; 
            break;
   case 227 : lux=3; 
            break;
   case 228 : lux=3; 
            break;
   case 229 : lux=3; 
            break;
   case 230 : lux=3; 
            break;
   case 231 : lux=3; 
            break;
   case 232 : lux=3; 
            break;
   case 233 : lux=3; 
            break;
   case 234 : lux=3; 
            break;
   case 235 : lux=3; 
            break;
   case 236 : lux=3; 
            break;
   case 237 : lux=3; 
            break;
   case 238 : lux=2; 
            break;
   case 239 : lux=2; 
            break;
   case 240 : lux=2; 
            break;
   case 241 : lux=2; 
            break;
   case 242 : lux=2; 
            break;
   case 243 : lux=2; 
            break;
   case 244 : lux=2; 
            break;
   case 245 : lux=2; 
            break;
   case 246 : lux=2; 
            break;
   case 247 : lux=2; 
            break;
   case 248 : lux=2; 
            break;
   case 249 : lux=2; 
            break;
   case 250 : lux=2; 
            break;
   case 251 : lux=2; 
            break;
   case 252 : lux=2; 
            break;
   case 253 : lux=2; 
            break;
   case 254 : lux=2; 
            break;
   case 255 : lux=2; 
            break;
  }
 if (lux>=0) {
   //On affiche le signe +
   lcd.print("+");
   }
   lcd.print(lux);
   lcd.print(" Lx");
}  

//Renvoie la chaîne d'identification de la maquette ainsi que le numéro de version du firmware
void identification() {
  Serial.print("MODEL NAME : ");
  n1=EEPROM.read(first_name_eeprom);
  n2=EEPROM.read(first_name_eeprom+1);
  n3=EEPROM.read(first_name_eeprom+2);
  n4=EEPROM.read(first_name_eeprom+3);
  n5=EEPROM.read(first_name_eeprom+4);
  n6=EEPROM.read(first_name_eeprom+5);
  Serial.write(n1);
  Serial.write(n2);
  Serial.write(n3);
  Serial.write(n4);
  Serial.write(n5);
  Serial.write(n6);
  Serial.println("");
  //Serial.println("VERSION : 2.7");
}  

char ith(int value) {
 switch(value) {
  case 32 : return ' ';
            break;
  case 48 : return '0';
            break;
  case 49 : return '1';
            break;
  case 50 : return '2';
            break;
  case 51 : return '3';
            break;
  case 52 : return '4';
            break;
  case 53 : return '5';
            break;
  case 54 : return '6';
            break;
  case 55 : return '7';
            break;
  case 56 : return '8';
            break;
  case 57 : return '9';
            break;
  case 65 : return 'A';
            break;
  case 66 : return 'B';
            break;
  case 67 : return 'C';
            break;
  case 68 : return 'D';
            break;
  case 69 : return 'E';
            break;
  case 70 : return 'F';
            break;

 } 
}
//Fonction qui attends de recevoir un octet sur le port série tout en gérant le time_out
//Si le flag time_out=true à l'entrée de la fonction, celle-ci ne fera rien sinon, elle se mettra a attendre un octet
//Sur le port série durant un maximum de 100ms. Si au bout de ce délai, uncun octet ne s'est présenté sur le port série, alors
//Le flag time_out sera mis à true et la fonction renvoie -1. Si un octet s'est présenté avant les 100ms , la fonction le renvoie 
//en argument.
int lire_octet() {
   time_d=millis();
   while(Serial.available()==0 && !time_out) {
      time_f=millis();
      if ((time_f-time_d)>100) {time_out=true;}
      }
   if (!time_out) { return Serial.read(); } else { return -1; }
}

word get_first_pc(unsigned int no) {
#if defined(__mega_ver)
 switch (no) {
 case 0 : return first_opcode;
          break;
 case 1 : return 0;
          break;
 case 2 : return 4096;
          break;
 case 3 : return 8192; 
          break;
 case 4 : return 12288;
          break;
 case 5 : return 16384;
          break;
 case 6 : return 20480;
          break;
 case 7 : return 24576;
          break;
 case 8 : return 28672;
          break;
 case 9 : return 0;
          break;
 case 10 : return 4096;
          break;
 case 11 : return 8192; 
          break;
 case 12 : return 12288;
          break;
 case 13 : return 16384;
          break;
 case 14 : return 20480;
          break;
 case 15 : return 24576;
          break;
 case 16 : return 28672;
          break;
 case 17 : return 0;
          break;
 case 18 : return 4096;
          break;
 case 19 : return 8192; 
          break;
 case 20 : return 12288;
          break;
 case 21 : return 16384;
          break;
 case 22 : return 20480;
          break;
 case 23 : return 24576;
          break;
 case 24 : return 28672;
          break;
 case 25 : return 0;
          break;
 case 26 : return 4096;
          break;
 case 27 : return 8192; 
          break;
 case 28 : return 12288;
          break;
 case 29 : return 16384;
          break;
 case 30 : return 20480;
          break;
 case 31 : return 24576;
          break;
 case 32 : return 28672;
          break;
  }
#else
 switch (no) {
 case 0 : return first_opcode;
          break;
 case 1 : return 0;
          break;
 case 2 : return 4096;
          break;
 case 3 : return 8192; 
          break;
 case 4 : return 12288;
          break;
 case 5 : return 16384;
          break;
 case 6 : return 20480;
          break;
 case 7 : return 24576;
          break;
 case 8 : return 28672;
          break;
 }         
#endif
}
//****************************************************************************************************//
//Lance l'interpréteur qui exécute les programmes stockés en EEPROM en analysant les différents OPCODE//
//****************************************************************************************************//
void interpreteur() {
  //Initialisation des PC's pour tous les slots mémoire et remise a zéro de tous les flags intempo
#if defined(__mega_ver)
  if (debug) {Serial.println("Entrée interpréteur");}
#endif  
  for (int i=0;i<33;i++) { 
    pcs[i]=get_first_pc(i);
    intempo[i]=0;
    }
//  #if defined(__mega_ver)  
//  if (debug) {Serial.println("Initialisation des PCS effectuée");
//             for (int i=0;i<33;i++) {
//               Serial.print("PC[");
//               Serial.print(i,DEC);
//               Serial.print("]=");
//               Serial.println(pcs[i],DEC);
//              }
//             }
//  #endif
  //On vide le tampon de réception série avant d'entrer dans l'exécution 
  while (Serial.available()>0) {
   value=Serial.read(); 
  }
  boolean fin=false;    //Flag pour savoir si on a atteint la fin du programme
  
  while (!fin) {
   if (Serial.available()>0) {fin=true;} //L'interpréteur s'arrète dès qu'un octet est reçu sur le port série de la carte 
//   #if defined(__mega_ver)
//   if (debug) {Serial.println("Entrée boucle infinie");} 
//   #endif
   for (int slot=0;slot<33;slot++)
   {
//     #if defined(__mega_ver)
//     if (debug) {
//       Serial.print("Sheduller sur le slot ");
//       Serial.println(slot,DEC);
//       }
//     #endif  
     //Pour chaque slot vérifier si il faut l'interpréter
     if (EEPROM.read(run+slot)==1) {
      //On interprète le programme dans ce slot
      //On restaure le PC correspondant ainsi que le tableau des variables
      pc=pcs[slot];
      for (int i=0;i<26;i++) {
        variable[i]=variables[slot*26+i];
      }
      #if defined(__mega_ver)
      if (debug) {
        Serial.print("Execution programme en slot memoire no ");
        Serial.println(slot,DEC);
      }
      #endif
  if (Serial.available()>0) {fin=true;} //L'interpréteur s'arrète dès qu'un octet est reçu sur le port série de la carte
  #if defined(__mega_ver)
  if (debug) {
    delay(500);
    Serial.println("*********************************************"); 
    Serial.print("Interpretation opcode ");
    Serial.print(lire_eeprom(slot,pc),DEC);
    Serial.print(";");
    Serial.print(lire_eeprom(slot,pc+1),DEC);
    Serial.print(";");
    Serial.print(lire_eeprom(slot,pc+2),DEC);
    Serial.print(";");
    Serial.print(lire_eeprom(slot,pc+3),DEC);
    Serial.print(";");
    Serial.print(lire_eeprom(slot,pc+4),DEC);
    Serial.print(";");
    Serial.print(lire_eeprom(slot,pc+5),DEC);
    Serial.print(" a l'adresse ");
    Serial.println(pc,DEC);
  }
  #endif
  if (intempo[slot]==0) 
  {  
  switch (lire_eeprom(slot,pc)) {
     case 0 : //ne rien faire
              #if defined(__mega_ver)
              if (debug) { Serial.println("NOP"); }
              #endif 
              pc=pc+1;
              break;
     case 1 : //affecte une valeur à une variable 1MVV
//              #if defined(__mega_ver)
//              if (debug) { 
//                Serial.write(lire_eeprom(slot,pc+1));
//                Serial.print("=");
//                Serial.println(lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3),DEC);
//              }
//              #endif 
              variable[lire_eeprom(slot,pc+1)-65]=lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3);
              pc=pc+4;
              break;
     case 2 : //incrémente une variable 2M
//             #if defined(__mega_ver)
//             if (debug) { 
//                Serial.write(lire_eeprom(slot,pc+1));
//                Serial.print("=");
//                Serial.write(lire_eeprom(slot,pc+1));
//                Serial.println("+1");
//              }
//             #endif 
              variable[lire_eeprom(slot,pc+1)-65]=variable[lire_eeprom(slot,pc+1)-65]+1;
              pc=pc+2;
              break;
     case 3 : //Décrémente une variable 3M
//              #if defined(__mega_ver)
//              if (debug) { 
//                Serial.write(lire_eeprom(slot,pc+1));
//                Serial.print("=");
//                Serial.write(lire_eeprom(slot,pc+1));
//                Serial.println("-1");
//              }
//              #endif             
              variable[lire_eeprom(slot,pc+1)-65]=variable[lire_eeprom(slot,pc+1)-65]-1;
              pc=pc+2;
              break;
     case 4 : //Lecture d'une entrée Analogique et stockage du résultat dans une variable 4BM
//              #if defined(__mega_ver)
//              if (debug) { 
//                Serial.print("lire l'entree analogique ");
//                Serial.print(lire_eeprom(slot,pc+1),DEC);
//                Serial.print(" et stocke le resulat dans la variable ");
//                Serial.write(lire_eeprom(slot,pc+2));
//                Serial.println("");
//              }
//              #endif 
              variable[lire_eeprom(slot,pc+2)-65]=analogRead(lire_eeprom(slot,pc+1))/4;
              pc=pc+3;
              break;
     case 5 : //Sortie PWM d'une valeur contenu dans une variable 5BM
//              #if defined(__mega_ver)
//              if (debug) { 
//                        Serial.print("Sortie PWM ");
//                        Serial.print(lire_eeprom(slot,pc+1),DEC);
//                        Serial.print(" reglee avec le contenu de la variable ");
//                        Serial.write(lire_eeprom(slot,pc+2)); 
//                        Serial.println("");
//                      }
//              #endif        
              analogWrite(lire_eeprom(slot,pc+1),variable[lire_eeprom(slot,pc+2)-65]); 
              pc=pc+3;
              break;
     case 6 : //Sortie PWM d'une valeur directe 6BV
//              #if defined(__mega_ver)
//              if (debug) { 
//                        Serial.print("Sortie PWM ");
//                        Serial.print(lire_eeprom(slot,pc+1),DEC);
//                        Serial.print(" reglee sur ");
//                        Serial.println(lire_eeprom(slot,pc+2),DEC); 
//                      }
//              #endif        
              analogWrite(lire_eeprom(slot,pc+1),lire_eeprom(slot,pc+2)); 
              pc=pc+3;
              break;
     case 7 : //Sortie numérique sur une broche 7BV
              if (lire_eeprom(slot,pc+2)==0) {
                      digitalWrite(lire_eeprom(slot,pc+1), LOW); //On met la broche à 0 V
//                      #if defined(__mega_ver)
//                      if (debug) { 
//                        Serial.print("Sortie Numerique ");
//                        Serial.print(lire_eeprom(slot,pc+1),DEC);
//                        Serial.println(" a 0 Volts"); 
//                      }
//                     #endif 
                    } 
                    else
                    {
                      digitalWrite(lire_eeprom(slot,pc+1), HIGH); //On met la broche à 5 V
//                      #if defined(__mega_ver)
//                      if (debug) { 
//                        Serial.print("Sortie Numerique ");
//                        Serial.print(lire_eeprom(slot,pc+1),DEC);
//                        Serial.println(" a 1 Volts"); 
//                      }
//                     #endif 
                    } // if (value==0)
              pc=pc+3;
              break;
     case 8 : //Commande servo moteur avec valeur directe 8BV
//              #if defined(__mega_ver)
//              if (debug) { 
//                        Serial.print("Sortie SERVO ");
//                        Serial.print(lire_eeprom(slot,pc+1),DEC);
//                        Serial.print(" reglee sur ");
//                        Serial.println(lire_eeprom(slot,pc+2),DEC); 
//                      }
//              #endif
              #if defined(__mega_ver)        
              switch(lire_eeprom(slot,pc+1)) {
                    case 14 :
                            myservo14.write(lire_eeprom(slot,pc+2));
                            break;
                    case 15 :
                            myservo15.write(lire_eeprom(slot,pc+2));
                            break;
                    case 16 :
                            myservo16.write(lire_eeprom(slot,pc+2));
                            break;
                    case 17 :
                            myservo17.write(lire_eeprom(slot,pc+2));
                            break;
                    case 18 :
                            myservo18.write(lire_eeprom(slot,pc+2));
                            break;
                    case 19 :
                            myservo19.write(lire_eeprom(slot,pc+2));
                            break;
                    case 22 :
                            myservo22.write(lire_eeprom(slot,pc+2));
                            break;
                    case 23 :
                            myservo23.write(lire_eeprom(slot,pc+2));
                            break;
                    case 24 :
                            myservo24.write(lire_eeprom(slot,pc+2));
                            break;
                    case 25 :
                            myservo25.write(lire_eeprom(slot,pc+2));
                            break;        
                    case 26 :
                            myservo26.write(lire_eeprom(slot,pc+2));
                            break;        
                    case 27 :
                            myservo27.write(lire_eeprom(slot,pc+2));
                            break;        
                            
                     }//switch broche
                   #else
                   switch(lire_eeprom(slot,pc+1)) {
                    case 4 :
                            myservo4.write(lire_eeprom(slot,pc+2));
                            break;
                    case 5 :
                            myservo5.write(lire_eeprom(slot,pc+2));
                            break;
                    case 6 :
                            myservo6.write(lire_eeprom(slot,pc+2));
                            break;
                    case 7 :
                            myservo7.write(lire_eeprom(slot,pc+2));
                            break;
                    case 8 :
                            myservo8.write(lire_eeprom(slot,pc+2));
                            break;
                    case 9 :
                            myservo9.write(lire_eeprom(slot,pc+2));
                            break;
                    case 10 :
                            myservo10.write(lire_eeprom(slot,pc+2));
                            break;
                    case 11 :
                            myservo11.write(lire_eeprom(slot,pc+2));
                            break;
                    case 12 :
                            myservo12.write(lire_eeprom(slot,pc+2));
                            break;
                    case 13 :
                            myservo13.write(lire_eeprom(slot,pc+2));
                            break;        
                     }//switch broche
                   #endif  
                   pc=pc+3;
                   break;  
     case 9 : //Commande servo moteur avec valeur contenu dans une variable 9BM
//              #if defined(__mega_ver)
//              if (debug) { 
//                        Serial.print("Sortie SERVO ");
//                        Serial.print(lire_eeprom(slot,pc+1),DEC);
//                        Serial.print(" reglee avec le contenu de la variable ");
//                        Serial.write(lire_eeprom(slot,pc+2)); 
//                        Serial.println("");
//                      }
//              #endif
              #if defined(__mega_ver)        
              switch(lire_eeprom(slot,pc+1)) {
                    case 14 :
                            myservo14.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 15 :
                            myservo15.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 16 :
                            myservo16.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 17 :
                            myservo17.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 18 :
                            myservo18.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 19 :
                            myservo19.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 22 :
                            myservo22.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 23 :
                            myservo23.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 24 :
                            myservo24.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 25 :
                            myservo25.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;        
                    case 26 :
                            myservo26.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;        
                    case 27 :
                            myservo27.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;        
                            
                     }//switch broche
                   #else
                   switch(lire_eeprom(slot,pc+1)) {
                    case 4 :
                            myservo4.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 5 :
                            myservo5.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 6 :
                            myservo6.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 7 :
                            myservo7.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 8 :
                            myservo8.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 9 :
                            myservo9.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 10 :
                            myservo10.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 11 :
                            myservo11.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 12 :
                            myservo12.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 13 :
                            myservo13.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;        
                     }//switch broche 
                   #endif  
                   pc=pc+3;
                   break;  
     case 10 : //Lire une valeur numérique sur une broche et la comparer à une valeur. Sauter de l'offset indiqué si oui 10BVAA
//              #if defined(__mega_ver)
//              if (debug) {
//               Serial.print("Lire l'entree numerique ");
//               Serial.print(lire_eeprom(slot,pc+1),DEC);
//               Serial.print(" et la comparer a ");
//               Serial.println(lire_eeprom(slot,pc+2),DEC);
//              }
//              #endif
              value = digitalRead(lire_eeprom(slot,pc+1)); //On lit l'état de la broche et on mets le résultat dans value
              if (value==lire_eeprom(slot,pc+2))
              {
               //La condition est remplie, on saute de l'offset indiquée
//               #if defined(__mega_ver)
//               if (debug) {
//                Serial.print("Le test est positif, on saute de ");
//                Serial.print(lire_eeprom(slot,pc+3)*256+lire_eeprom(slot,pc+4),DEC);
//                Serial.println(" Octets"); 
//               }
//               #endif
               no=lire_eeprom(slot,pc+3)*256+lire_eeprom(slot,pc+4);
               pc=pc+no+4;
              } else
              {
               //La condition n'est pas remplie, on éxécute le token suivant;
//               #if defined(__mega_ver)
//               if (debug) {
//                Serial.println("Le test est negatif, on execute l'instruction suivante");
//               }
//               #endif
              pc=pc+5; 
              }
              break;
     case 11 : //Tester si une variable = une valeur de référence 11MVVAA
//             #if defined(__mega_ver)
//             if (debug) { 
//                        Serial.print("Tester si ");
//                        Serial.print(lire_eeprom(slot,pc+1),DEC);
//                        Serial.print(" = ");
//                        Serial.println(lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3),DEC); 
//                      }
//              #endif        
              value = variable[lire_eeprom(slot,pc+1)-65]; //On lit le contenu de la variable dans value
              if (value==(lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3)))
              {
               //La condition est remplie, on saute de l'offset indiquée
//               #if defined(__mega_ver)
//               if (debug) {
//                Serial.print("Le test est positif, on saute de ");
//                Serial.print(lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5),DEC);
//                Serial.println(" Octets"); 
//               }
//               #endif
               no=lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5);
               pc=pc+no+5;
              } else
              {
//               #if defined(__mega_ver)
//               if (debug) {
//                Serial.println("Le test est negatif, on execute l'instruction suivante");
//               }
//               #endif
               
               //La condition n'est pas remplie, on éxécute le token suivant;
              pc=pc+6; 
              }
              break;
     case 12 : //Tester si une variable < une valeur de référence 12MVVAA
//             #if defined(__mega_ver)
//             if (debug) { 
//                        Serial.print("Tester si ");
//                        Serial.write(lire_eeprom(slot,pc+1));
//                        Serial.print(" < ");
//                        Serial.println(lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3),DEC); 
//                      }
//              #endif        
              value = variable[lire_eeprom(slot,pc+1)-65]; //On lit le contenu de la variable dans value
//              #if defined(__mega_ver)
//             if (debug) { 
//                        Serial.print("Tester si ");
//                        Serial.print(value);
//                        Serial.print(" < ");
//                        Serial.println(lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3),DEC); 
//                      }
//              #endif        
              if (value<(lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3)))
              {
               //La condition est remplie, on saute de l'offset indiquée
               #if defined(__mega_ver)
               if (debug) {
                Serial.print("Le test est positif, on saute de ");
                Serial.print(lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5),DEC);
                Serial.println(" Octets"); 
               }
               #endif
               no=lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5);
               pc=pc+no+5;
              } else
              {
               //La condition n'est pas remplie, on éxécute le token suivant;
               #if defined(__mega_ver)
               if (debug) {
                Serial.println("Le test est negatif, on execute l'instruction suivante");
               }
              #endif 
              pc=pc+6; 
              }
              break;
     case 13 : //Tester si une variable > une valeur de référence 13MVVAA
//              #if defined(__mega_ver)
//              if (debug) { 
//                        Serial.print("Tester si ");
//                        Serial.print(lire_eeprom(slot,pc+1),DEC);
//                        Serial.print(" > ");
//                        Serial.println(lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3),DEC); 
//                      }
//              #endif        
              value = variable[lire_eeprom(slot,pc+1)-65]; //On lit le contenu de la variable dans value
              if (value>(lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3)))
              {
               //La condition est remplie, on saute de l'offset indiquée
//               #if defined(__mega_ver)
//               if (debug) {
//                Serial.print("Le test est positif, on saute de ");
//                Serial.print(lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5),DEC);
//                Serial.println(" Octets"); 
//               }
//               #endif
               no=lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5);
               pc=pc+no+5;
              } else
              {
               //La condition n'est pas remplie, on éxécute le token suivant;
//               #if defined(__mega_ver)
//               if (debug) {
//                Serial.println("Le test est negatif, on execute l'instruction suivante");
//               }
//               #endif
              pc=pc+6; 
              }
              break;
      case 14 : //Tester si une variable <= une valeur de référence 14MVVAA
//              #if defined(__mega_ver)
//              if (debug) { 
//                        Serial.print("Tester si ");
//                        Serial.print(lire_eeprom(slot,pc+1),DEC);
//                        Serial.print(" <= ");
//                        Serial.println(lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3),DEC); 
//                      }
//              #endif        
              value = variable[lire_eeprom(slot,pc+1)-65]; //On lit le contenu de la variable dans value
              if (value<=(lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3)))
              {
               //La condition est remplie, on saute de l'offset indiquée
//               #if defined(__mega_ver)
//               if (debug) {
//                Serial.print("Le test est positif, on saute de ");
//                Serial.print(lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5),DEC);
//                Serial.println(" Octets"); 
//               }
//               #endif
               no=lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5);
               pc=pc+no+5;
              } else
              {
               //La condition n'est pas remplie, on éxécute le token suivant;
//               #if defined(__mega_ver)
//               if (debug) {
//                Serial.println("Le test est negatif, on execute l'instruction suivante");
//               }
//               #endif
              pc=pc+6; 
              }
              break;
       case 15 : //Tester si une variable >= une valeur de référence 15MVVAA
//              #if defined(__mega_ver)
//              if (debug) { 
//                        Serial.print("Tester si ");
//                        Serial.print(lire_eeprom(slot,pc+1),DEC);
//                        Serial.print(" >= ");
//                        Serial.println(lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3),DEC); 
//                      }
//              #endif        
              value = variable[lire_eeprom(slot,pc+1)-65]; //On lit le contenu de la variable dans value
              if (value>=(lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3)))
              {
               //La condition est remplie, on saute de l'offset indiquée
//               #if defined(__mega_ver)
//               if (debug) {
//                Serial.print("Le test est positif, on saute de ");
//                Serial.print(lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5),DEC);
//                Serial.println(" Octets"); 
//               }
//               #endif
               no=lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5);
               pc=pc+no+5;
              } else
              {
               //La condition n'est pas remplie, on éxécute le token suivant;
//               #if defined(__mega_ver)
//               if (debug) {
//                Serial.println("Le test est negatif, on execute l'instruction suivante");
//               }
//               #endif
              pc=pc+6;
              }
              break;
       case 16 : //Tester si une variable <> une valeur de référence 16MVVAA
//              #if defined(__mega_ver)
//              if (debug) { 
//                        Serial.print("Tester si ");
//                        Serial.print(lire_eeprom(slot,pc+1),DEC);
//                        Serial.print(" <> ");
//                        Serial.println(lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3),DEC); 
//                      }
//                #endif      
              value = variable[lire_eeprom(slot,pc+1)-65]; //On lit le contenu de la variable dans value
              if (value!=(lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3)))
              {
               //La condition est remplie, on saute de l'offset indiquée
//               #if defined(__mega_ver)
//               if (debug) {
//                Serial.print("Le test est positif, on saute de ");
//                Serial.print(lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5),DEC);
//                Serial.println(" Octets"); 
//               }
//               #endif
               no=lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5);
               pc=pc+no+5;
              } 
              else
              {
               //La condition n'est pas remplie, on éxécute le token suivant;
//               #if defined(__mega_ver)
//               if (debug) {
//                Serial.println("Le test est negatif, on execute l'instruction suivante");
//               }
//               #endif
              pc=pc+6; 
              }
              break;
     case 17 : //Tempo en millisecondes 17VV
              intempo[slot]=millis()+(unsigned int)(lire_eeprom(slot,pc+1)*256+lire_eeprom(slot,pc+2));  //On configure le flag intempo pour spécifié que l'on est dans une boucle d'attente
              //delay(lire_eeprom(slot,pc+1)*256+lire_eeprom(slot,pc+2)); //Temporisation en millisecondes
              #if defined(__mega_ver)
              if (debug) { 
                        Serial.print("Temporisation de ");
                        Serial.print((unsigned int)(lire_eeprom(slot,pc+1)*256+lire_eeprom(slot,pc+2)),DEC);
                        Serial.println(" ms"); 
                      }
              #endif        
              pc=pc+3;
              break;
     case 18 : //Saut à une adresse absolue 18AA
              #if defined(__mega_ver)
              if (debug) { 
                        Serial.print("Saut a l'adresse absolue ");
                        Serial.println(lire_eeprom(slot,pc+1)*256+lire_eeprom(slot,pc+2),DEC);
                        }
              #endif          
              no=lire_eeprom(slot,pc+1)*256+lire_eeprom(slot,pc+2); //Adresse du saut
              if (slot==0) {
                //Saut absolue en mémoire EEPROM interne 
                pc=first_opcode+no-1;
              } else {
                //Saut absolue en mémoire EEPROM externe
                pc=get_first_pc(slot)+no-1;
              }  
              break;
     case 19 : //Lit l'entrée de la télécommande et la compare à une chaine de caractêre de 8 caractêres de long. Sauter de l'offset indiqué si oui 19VVVVVVVVAA 
//                 #if defined(__mega_ver)
//                 if (debug) { 
//                        Serial.println("Lecture telecommande ");
//                      }
//                 #endif     
                 if (irrecv.decode(&results)) {
                 //Une touche a été appuyée on stocke dans la chaine s le code complété avec des espaces sur une longueur de 8 caractere
                 String s = String(long(results.value),HEX);
                 s.toUpperCase(); //On converti en majuscule
                 String s2=String(' ');
                 while (s.length()<8) {
                   //On ajoute un espace à la chaine de caractêre
                   s.concat(s2); //On ajoute un espace
                    } 
//                 #if defined(__mega_ver)
//                 if (debug) { 
//                        Serial.print("La touche  ");
//                        Serial.print(s);
//                        Serial.println(" a ete appuyee");
//                      }
//                 #endif     
                 //On récupère la chaine servant de comparaison dans l'opcode
                 String s3=(String)ith(lire_eeprom(slot,pc+1))+(String)ith(lire_eeprom(slot,pc+2))+(String)ith(lire_eeprom(slot,pc+3))+(String)ith(lire_eeprom(slot,pc+4))+(String)ith(lire_eeprom(slot,pc+5))+(String)ith(lire_eeprom(slot,pc+6))+(String)ith(lire_eeprom(slot,pc+7))+(String)ith(lire_eeprom(slot,pc+8));
//               String s3=String(ith(lire_eeprom(slot,pc+1)));
//                 delay(100);
                 s3.toUpperCase();
//                 #if defined(__mega_ver)
//                 if (debug) { 
//                        Serial.print("On la compare a la chaine  ");
//                        Serial.println(s3);
//                      }
//                 #endif     
                 if (s.equals(s3)) {
                  //La condition est remplie, on saute de l'offset indiquée
                  no=lire_eeprom(slot,pc+9)*256+lire_eeprom(slot,pc+10);
//                  #if defined(__mega_ver)
//                  if (debug) { 
//                        Serial.print("Le test est positif, on saute de ");
//                        Serial.print(no,DEC);
//                        Serial.println(" Octets"); 
//                      }
//                  #endif    
                  pc=pc+no+10;
                 } else {
                   //La condition n'est pas remplie, on éxécute le token suivant
//                   #if defined(__mega_ver)
//                   if (debug) { 
//                        Serial.println("Le test est negatif, on execute la commande suivante");
//                        }
//                   #endif     
                   pc=pc+11;    
                 }
                 } else {
                 //Aucune touche n'a été appuyée
//                 #if defined(__mega_ver)
//                 if (debug) { 
//                        Serial.println("Aucune touche n'a ete appuyee, on compare la chaine a -1 ");
//                      }
//                 #endif     
                 if (lire_eeprom(slot,pc+1)==45 & lire_eeprom(slot,pc+2)==49) {
                   //la condition est remplie, on saute de l'offset indiqué
                  no=lire_eeprom(slot,pc+9)*256+lire_eeprom(slot,pc+10);
//                  #if defined(__mega_ver)
//                  if (debug) { 
//                        Serial.print("Le test est positif, on saute de ");
//                        Serial.print(no,DEC);
//                        Serial.println(" Octets"); 
//                      }
//                   #endif   
                   pc=pc+no+10;
                 } else {
                   //La condition n'est pas remplie, on éxécute le token suivant
//                   #if defined(__mega_ver)
//                   if (debug) { 
//                        Serial.println("Le test est negatif, on execute la commande suivante");
//                        }
//                    #endif    
                   pc=pc+11;
                 } 
                 }  
                break;
     case 20 : //Vidange buffet télécommande
//              #if defined(__mega_ver)
//              if (debug) { 
//                        Serial.println("Vidange du buffet de la telecommande");
//                        }
//              #endif          
              irrecv.resume(); // Receive the next value
  //            delay(200);     
              pc=pc+1; 
              break;           
     case 21 : //Effacer l'écran LCD : 21
//              #if defined(__mega_ver)
//              if (debug) { 
//                        Serial.println("Effacer l'écran LCD");
//                        }
//              #endif          
              lcd.clear();
              pc=pc+1; 
              break;           
     case 22 : //Positionne le curseur de l'écran LCD à la position (c,r) : 22cr
//              #if defined(__mega_ver)
//              if (debug) { 
//                        Serial.print("Positionne le curseur du LCD à (");
//                        Serial.print(lire_eeprom(slot,pc+1),DEC);
//                        Serial.print(",");
//                        Serial.print(lire_eeprom(slot,pc+2),DEC);
//                        Serial.println(")"); 
//                      }
//              #endif        
              lcd.setCursor(lire_eeprom(slot,pc+1),lire_eeprom(slot,pc+2));        
              pc=pc+3;
              break;
     case 23 : //Ecrire à la position courante du curseur la chaine sss...ss sur l'écran LCD : 23sss..ss|
//              #if defined(__mega_ver)
//              if (debug) { 
//                        Serial.println("Ecrire une chaine de caractère sur le LCD"); 
//                      }
//              #endif        
              n3=1;        
              do
                { 
                  n2 = lire_eeprom(slot,pc+n3);
                  if (n2!=124) {lcd.write(n2);}
                  n3++;
                } while (n2!=124);
              pc=pc+n3;
              break;
     case 24 : //Positionne le curseur de l'écran LCD à la position (c,r) et écris la valeur de la mémoire m : 24mcr
//              #if defined(__mega_ver)
//              if (debug) { 
//                        Serial.print("Ecrire la mémoire ");
//                        Serial.print(lire_eeprom(slot,pc+1),DEC);
//                        Serial.print(" à (");
//                        Serial.print(lire_eeprom(slot,pc+2),DEC);
//                        Serial.print(",");
//                        Serial.print(lire_eeprom(slot,pc+3),DEC);
//                        Serial.println(") sur le LCD"); 
//                      }
//              #endif        
              lcd.setCursor(lire_eeprom(slot,pc+2),lire_eeprom(slot,pc+3));
              lcd.print("   ");
              lcd.setCursor(lire_eeprom(slot,pc+2),lire_eeprom(slot,pc+3));
              value = variable[lire_eeprom(slot,pc+1)-65]; //On lit le contenu de la variable dans value
              lcd.print(value);
              pc=pc+4;
              break;
      case 25 : //Positionne le curseur de l'écran LCD à la position (c,r) et écris la valeur v : 25vcr
//              #if defined(__mega_ver)
//              if (debug) { 
//                        Serial.print("Ecrire la valeur ");
//                        Serial.print(lire_eeprom(slot,pc+1),DEC);
//                        Serial.print(" à (");
//                        Serial.print(lire_eeprom(slot,pc+2),DEC);
//                        Serial.print(",");
//                        Serial.print(lire_eeprom(slot,pc+3),DEC);
//                        Serial.println(") sur le LCD"); 
//                      }
//              #endif        
              lcd.setCursor(lire_eeprom(slot,pc+2),lire_eeprom(slot,pc+3));
              lcd.print("   ");
              lcd.setCursor(lire_eeprom(slot,pc+2),lire_eeprom(slot,pc+3));
              value = lire_eeprom(slot,pc+1); //On lit la valeur à écrire
              lcd.print(value);
              pc=pc+4;
              break;        
      case 26 : //Ecrire la date de la variable horaire n°h au format JJ/MM/AA à la position courante du curseur sur l'écran LCD : 26h
//              #if defined(__mega_ver)
//              if (debug) { 
//                        Serial.print("Ecrire la date de la variable horaire n° ");
//                        Serial.print(lire_eeprom(slot,pc+1),DEC);
//                        Serial.println(") sur le LCD"); 
//                      }
//              #endif        
              value = lire_eeprom(slot,pc+1); //On lit h
              RTC.getRAM(value*7,&hor,1);  //Lire Jour
              lcd.write((hor/10)+48);
              lcd.write((hor%10)+48);
              lcd.write('/');
              RTC.getRAM(value*7+1,&hor,1); //Lire mois
              lcd.write((hor/10)+48);
              lcd.write((hor%10)+48);
              lcd.write('/');
              RTC.getRAM(value*7+2,&hor,1); //Lire année
              lcd.write((hor/10)+48);         
              lcd.write((hor%10)+48);
              pc=pc+2;
              break;        
      case 27 : //Ecrire l'heure de la variable horaire n°h au format hh:mm:ss à la position courante du curseur sur l'écran LCD : 27h
//              #if defined(__mega_ver)
//              if (debug) { 
//                        Serial.print("Ecrire l'heure de la variable horaire n° ");
//                        Serial.print(lire_eeprom(slot,pc+1),DEC);
//                        Serial.println(") sur le LCD"); 
//                      }
//              #endif        
              value = lire_eeprom(slot,pc+1); //On lit h
              RTC.getRAM(value*7+3,&hor,1);   //Lire l'heure
              lcd.write((hor/10)+48);
              lcd.write((hor%10)+48);
              lcd.write(':');
              RTC.getRAM(value*7+4,&hor,1);   //Lire les minutes
              lcd.write((hor/10)+48);
              lcd.write((hor%10)+48);
              lcd.write(':');
              RTC.getRAM(value*7+5,&hor,1);  //Lire les secondes
              lcd.write((hor/10)+48);
              lcd.write((hor%10)+48);
              pc=pc+2;
              break;        
      case 28 : //Lire la date et l'heure courante de l'horloge temps réel et les copier dans la variable horaire n°h : 28h
//              #if defined(__mega_ver)
//              if (debug) { 
//                        Serial.print("Lire date et heure et copier dans la variable horaire n° ");
//                        Serial.println(lire_eeprom(slot,pc+1),DEC);
//                        }
//              #endif          
              value = lire_eeprom(slot,pc+1); //On lit h
              RTC.getTime();
              hor=RTC.day;
              RTC.setRAM(value*7,&hor,1);
              hor=RTC.month;
              RTC.setRAM(value*7+1,&hor,1);
              hor=RTC.year-2000;
              RTC.setRAM(value*7+2,&hor,1);
              hor=RTC.hour;
              RTC.setRAM(value*7+3,&hor,1);
              hor=RTC.minute;
              RTC.setRAM(value*7+4,&hor,1);
              hor=RTC.second;
              RTC.setRAM(value*7+5,&hor,1);
              hor=RTC.dow;
              RTC.setRAM(value*7+6,&hor,1);
              pc=pc+2;
              break;
     case 29: //Copie la partie x de la variable horaire n°h dans la variable v : 29hxv
//              #if defined(__mega_ver)
//              if (debug) { 
//                        Serial.print("Copie la partie ");
//                        Serial.print(lire_eeprom(slot,pc+2));
//                        Serial.print(" de la variable horaire n°");
//                        Serial.print(lire_eeprom(slot,pc+1),DEC);
//                        Serial.print(" dans la varaible ");
//                        Serial.println(lire_eeprom(slot,pc+3));
//                        }
//              #endif          
              n4=lire_eeprom(slot,pc+1); //On lit h
              n2=lire_eeprom(slot,pc+2);      //On lit x
              switch (n2) {
                          case 'J' : 
                             n5=0;
                             break;
                          case 'M' :
                             n5=1;
                             break;
                          case 'A' :
                             n5=2; 
                             break;
                          case 'h' :
                             n5=3;
                             break;
                          case 'm' :
                             n5=4;
                             break;
                          case 's' :
                             n5=5;
                             break;
                          case 'o' :
                             n5=6;
                             break;   
              } //fin switch
              RTC.getRAM(n4*7+n5,&hor,1);
              variable[lire_eeprom(slot,pc+3)-65]=hor;
              pc=pc+4;
              break;
     case 30: //Calcule la durée séparant les variables horaire h1 et h2 et stocke le résultat dans h3 ( h3=h2-h1): 30h1h2h3
//              #if defined(__mega_ver)
//              if (debug) { 
//                        Serial.print("Calcule la durée séparant h");
//                        Serial.print(lire_eeprom(slot,pc+1),DEC);
//                        Serial.print(" et h");
//                        Serial.print(lire_eeprom(slot,pc+2),DEC);
//                        Serial.print(" , stocke le résultat dans h");
//                        Serial.println(lire_eeprom(slot,pc+3));
//                        }
//              #endif          
              n2=lire_eeprom(slot,pc+1);       //On lit h1
              n3=lire_eeprom(slot,pc+2);      //On lit h2
              n4=lire_eeprom(slot,pc+3);      //On lit h3              
              duree(n2,n3,n4);
              pc=pc+4;
              break;         
     case 31 : //Positionne le curseur de l'écran LCD à la position (c,r) et écris la valeur de la température dans la mémoire m : 31mcr
//              #if defined(__mega_ver)
//              if (debug) { 
//                        Serial.print("Ecrire la temperature ");
//                        Serial.print(lire_eeprom(slot,pc+1),DEC);
//                        Serial.print(" à (");
//                        Serial.print(lire_eeprom(slot,pc+2),DEC);
//                        Serial.print(",");
//                        Serial.print(lire_eeprom(slot,pc+3),DEC);
//                        Serial.println(") sur le LCD"); 
//                      }
//              #endif        
              lcd.setCursor(lire_eeprom(slot,pc+2),lire_eeprom(slot,pc+3));
              lcd.print("         ");
              lcd.setCursor(lire_eeprom(slot,pc+2),lire_eeprom(slot,pc+3));
              value = variable[lire_eeprom(slot,pc+1)-65]; //On lit le contenu de la variable dans value
              affiche_celsius(value);
              pc=pc+4;
              break;
      case 32 : //Positionne le curseur de l'écran LCD à la position (c,r) et écris la valeur de la tempérarture contenu v : 32vcr
//              #if defined(__mega_ver)
//              if (debug) { 
//                        Serial.print("Ecrire la temperature ");
//                        Serial.print(lire_eeprom(slot,pc+1),DEC);
//                        Serial.print(" à (");
//                        Serial.print(lire_eeprom(slot,pc+2),DEC);
//                        Serial.print(",");
//                        Serial.print(lire_eeprom(slot,pc+3),DEC);
//                        Serial.println(") sur le LCD"); 
//                      }
//              #endif        
              lcd.setCursor(lire_eeprom(slot,pc+2),lire_eeprom(slot,pc+3));
              lcd.print("         ");
              lcd.setCursor(lire_eeprom(slot,pc+2),lire_eeprom(slot,pc+3));
              value = lire_eeprom(slot,pc+1); //On lit la valeur à écrire
              affiche_celsius(value);
              pc=pc+4;
              break;
     case 33 : //Positionne le curseur de l'écran LCD à la position (c,r) et écris la valeur de la luminosite dans la mémoire m : 33mcr
//              #if defined(__mega_ver)
//              if (debug) { 
//                        Serial.print("Ecrire la luminosite ");
//                        Serial.print(lire_eeprom(slot,pc+1),DEC);
//                        Serial.print(" à (");
//                        Serial.print(lire_eeprom(slot,pc+2),DEC);
//                        Serial.print(",");
//                        Serial.print(lire_eeprom(slot,pc+3),DEC);
//                        Serial.println(") sur le LCD"); 
//                      }
//              #endif        
              lcd.setCursor(lire_eeprom(slot,pc+2),lire_eeprom(slot,pc+3));
              lcd.print("         ");
              lcd.setCursor(lire_eeprom(slot,pc+2),lire_eeprom(slot,pc+3));
              value = variable[lire_eeprom(slot,pc+1)-65]; //On lit le contenu de la variable dans value
              affiche_lux(value);
              pc=pc+4;
              break;
      case 34 : //Positionne le curseur de l'écran LCD à la position (c,r) et écris la valeur de la luminosit contenu v : 34vcr
//              #if defined(__mega_ver)
//              if (debug) { 
//                        Serial.print("Ecrire la luminosite ");
//                        Serial.print(lire_eeprom(slot,pc+1),DEC);
//                        Serial.print(" à (");
//                        Serial.print(lire_eeprom(slot,pc+2),DEC);
//                        Serial.print(",");
//                        Serial.print(lire_eeprom(slot,pc+3),DEC);
//                        Serial.println(") sur le LCD"); 
//                      }
//              #endif        
              lcd.setCursor(lire_eeprom(slot,pc+2),lire_eeprom(slot,pc+3));
              lcd.print("         ");
              lcd.setCursor(lire_eeprom(slot,pc+2),lire_eeprom(slot,pc+3));
              value = lire_eeprom(slot,pc+1); //On lit la valeur à écrire
              affiche_lux(value);
              pc=pc+4;
              break;         
      case 35 : //Produit un son sur la broche b de fréquence ff et de durée dd : 35bffdd
              //long freq2=lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3);
              //long dur2=lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5);
              #if defined(__mega_ver)
              if (debug) { 
                        Serial.print("Produire un son de fréquence ");
                        Serial.print(lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3),DEC);
                        Serial.print(" et de durée ");
                        Serial.print(lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5),DEC);
                        Serial.print(" sur la broche ");
                        Serial.print(lire_eeprom(slot,pc+1),DEC);
                        Serial.println("."); 
                      }
              #endif 
              //Produire le son ! en mode préemptif ( suspends l'éxécution des programmes en parallèle durant l'exécution du son
              buzz(lire_eeprom(slot,pc+1),lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3),lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5));
              pc=pc+6;
              break;         
     case 36 : //Mesure distance ultrasonore sur un capteur HC-SR04 connecté à T( trigger ) et D ( dist ou echo ) et stocke la valeur dans la mémoire M 36TDM
              n1=lire_eeprom(slot,pc+1);
              n2=lire_eeprom(slot,pc+2);
              n3=lire_eeprom(slot,pc+3);
              #if defined(__mega_ver)
              if (debug) { 
                Serial.print("lire distance ultrason (");
                Serial.print(n1);
                Serial.print(";");
                Serial.print(n2);
                Serial.print(" et stocke le resulat dans la variable ");
                Serial.write(n3);
                Serial.println("");
              }
              #endif 
              //Impulsion de trigger
              digitalWrite(n1,HIGH);
              delayMicroseconds(1000);
              digitalWrite(n1,LOW);
              //Lecture de l'eccho
              value=255;
              value=(pulseIn(n2,HIGH)/2)/29.1+2;  //On lit l'impulsion du capteur et on calcule la distance
              #if defined(__mega_ver)
              if (debug) { 
                     Serial.print("Distance mesuree =");
                     Serial.print(value,DEC);
                     Serial.println("");
              }
              #endif
              if (value>255) { value=255; } //Obstacle hors de portée 
              variable[n3-65]=value;
              pc=pc+4;
              break;              
     case 37 : //Tester entre le contenu de deux mémoires 37mntOO
//             #if defined(__mega_ver)
//             if (debug) { 
//                        Serial.print("Tester si ");
//                        Serial.print(lire_eeprom(slot,pc+1),DEC);
//                        Serial.print(" = ");
//                        Serial.println(lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3),DEC); 
//                      }
//              #endif        
              n1 = variable[lire_eeprom(slot,pc+1)-65]; //On lit le contenu de la variable n°1
              n2 = variable[lire_eeprom(slot,pc+2)-65]; //On lit le contenu de la variable n°2
              n3 = lire_eeprom(slot,pc+3); //On lit le test a effectuer
                                           //'=' : =
                                           //'>' : >
                                           //'<' : <
                                           //'!' : <>
                                           //'s' : >=
                                           //'i' : <=
              value=0;                             
              switch (n3) {
                case 61: 
                 //test =
                 if (n1==n2) {value=1;} else {value=0;}
                 break;
                case 62: 
                 //test >
                 if (n1>n2) {value=1;} else {value=0;}
                 break;
                case 60: 
                 //test <
                 if (n1<n2) {value=1;} else {value=0;}
                 break;
                case 33: 
                 //test <>
                 if (n1!=n2) {value=1;} else {value=0;}
                 break;
                case 115: 
                 //test >=
                 if (n1>=n2) {value=1;} else {value=0;}
                 break;
                case 105: 
                 //test <=
                 if (n1<=n2) {value=1;} else {value=0;}
                 break; 
              }                
              if (value==1)
              {
               //La condition est remplie, on saute de l'offset indiquée
//               #if defined(__mega_ver)
//               if (debug) {
//                Serial.print("Le test est positif, on saute de ");
//                Serial.print(lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5),DEC);
//                Serial.println(" Octets"); 
//               }
//               #endif
               no=lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5);
               pc=pc+no+5;
              } else
              {
//               #if defined(__mega_ver)
//               if (debug) {
//                Serial.println("Le test est negatif, on execute l'instruction suivante");
//               }
//               #endif
               
               //La condition n'est pas remplie, on éxécute le token suivant;
              pc=pc+6; 
              }
              break;

     case 38 : //Effectuer une opération arithmétique entre deux mémoire et stocker le résulstat dans une mémoire 38lmpn
//             #if defined(__mega_ver)
//             if (debug) { 
//                        Serial.print("Tester si ");
//                        Serial.print(lire_eeprom(slot,pc+1),DEC);
//                        Serial.print(" = ");
//                        Serial.println(lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3),DEC); 
//                      }
//              #endif        
              n1 = variable[lire_eeprom(slot,pc+2)-65]; //On lit le contenu de la variable n°1
              n2 = variable[lire_eeprom(slot,pc+4)-65]; //On lit le contenu de la variable n°2
              n3 = lire_eeprom(slot,pc+3); //On lit l'opération arithmétique à effectuer
                                           //'+' : addition
                                           //'-' : soustraction
                                           //'*' : multiplication
                                           //'/' : division
              value=0;                             
              switch (n3) {
                case 43: 
                 //Opération +
                 value=n1+n2;
                 break;
                case 45: 
                 //Opération -
                 value=n1-n2;
                 break;
                case 42: 
                 //Opération *
                 value=n1*n2;
                 break;
                case 47: 
                 //Opération /
                 value=n1/n2;
                 break;
              }           
              //On sauve le résultat dans la variable adéquate
              variable[lire_eeprom(slot,pc+1)-65]=value;
              pc=pc+5; 
              break;
     case 40 : //Ecrire n octets b1...bn vers un composant I2C d'adresse a : 40anb1...bn
//             #if defined(__mega_ver)
//             if (debug) { 
//                      }
//              #endif        
              n1 = lire_eeprom(slot,pc+1); //On lit l'adresse du composant I2C
              n2 = lire_eeprom(slot,pc+2); //On lit le nombre d'octet à envoyer
              Wire.beginTransmission(n1);  //démarre la communication avec le composant I2C
              for (byte i=1;i<=n2;i++){
                 //On envoie la donnée n°i
                 n3 = lire_eeprom(slot,pc+2+i); //On lit un un octet à envoyer
                 Wire.write(n3);
                 }
              Wire.endTransmission();      //Stoppe la communication avec le composant I2C
              pc=pc+3+n2; 
              break;
     case 41 : //Lire un octet ( du registre courant ) d'un composant I2C d'adresse a et le comparer à la valeur v : 41avoo
//             #if defined(__mega_ver)
//             if (debug) { 
//                      }
//              #endif        
              n1 = lire_eeprom(slot,pc+1); //On lit l'adresse du composant I2C
              Wire.requestFrom(n1,1);  //On demande la lecture de 1 octet sur le composant I2C
              delay(1);
              value=Wire.read();  //On lit la valeur du registre courant
              delay(1);
              Wire.endTransmission();      //Stoppe la communication avec le composant I2C
              //On compare la valeur du registre et la valeur de référence
              if (value==lire_eeprom(slot,pc+2))
              {
               //La condition est remplie, on saute de l'offset indiquée
//               #if defined(__mega_ver)
//               if (debug) {
//                Serial.print("Le test est positif, on saute de ");
//                Serial.print(lire_eeprom(slot,pc+3)*256+lire_eeprom(slot,pc+4),DEC);
//                Serial.println(" Octets"); 
//               }
//               #endif
               no=lire_eeprom(slot,pc+3)*256+lire_eeprom(slot,pc+4);
               pc=pc+no+4;
              } else
              {
               //La condition n'est pas remplie, on éxécute le token suivant;
//               #if defined(__mega_ver)
//               if (debug) {
//                Serial.println("Le test est negatif, on execute l'instruction suivante");
//               }
//               #endif
              pc=pc+5; 
              }
              break;              
     case 42 : //Ecrire 1 octets contenu dans la variable m vers le registre r d'un composant I2C d'adresse a : 42arm
//             #if defined(__mega_ver)
//             if (debug) { 
//                      }
//              #endif        
              n1 = lire_eeprom(slot,pc+1); //On lit l'adresse du composant I2C
              n2 = lire_eeprom(slot,pc+2); //On lit le numéro de registre 
              Wire.beginTransmission(n1);  //démarre la communication avec le composant I2C
              Wire.write(n2); //ecrire le numéro de registre
              Wire.write(variable[lire_eeprom(slot,pc+3)-65]);
              Wire.endTransmission();      //Stoppe la communication avec le composant I2C
              pc=pc+4; 
              break;
       
     case 255 : //Arrêt du programme dans le slot courant
              #if defined(__mega_ver)
              if (debug) { 
                        Serial.print("Arret du programme dans le slot ");
                        Serial.println(slot,DEC);
                        }
              #endif          
              //Serial.print("PROGRAM ENDING SLOT");
              //Serial.println(slot,DEC);
              break;    
          } //switch
     } else
     {
       //On est dans une temporisation, on teste si on doit en sortir
       #if defined(__mega_ver)
       if (debug) { Serial.println("On est dans la temporisation"); }
       #endif
      if (intempo[slot]<millis()) { 
       intempo[slot]=0;
       #if defined(__mega_ver)
       if (debug) { 
         Serial.println("On sort de la temporisation");
          }
       #endif   
      } //if (intempo[slot]<millis())   
     } //if intempo[slot]=0 ...   
          //On sauvegarde le PC actuel ainsi que les variables
          pcs[slot]=pc;
          for (int i=0;i<26;i++) {
          variables[slot*26+i]=variable[i];
           }
       } //if EEPROM.read(run+slot)==1       
   } //Boucle For sur les slots
//  Serial.print("On reboucle avec fin =");  
//  Serial.print(fin);
}//while   
 #if defined(__mega_ver)
  if (debug) {Serial.println("Sortie interpréteur");}
#endif  
}

void fixbaudrate() {

   switch (EEPROM.read(baud_rate)) {
     case 0 : //300 baud
              Serial.begin(300);
              break;
     case 1 : //600 baud
              Serial.begin(9600);
              break;
     case 2 : //1200 baud
              Serial.begin(1200);
              break;
     case 3 : //2400 baud
              Serial.begin(2400);
              break;
     case 4 : //4800 baud
              Serial.begin(4800);
              break;
     case 5 : //9600 baud
              Serial.begin(9600);
              break;
     case 6 : //14400 baud
              Serial.begin(14400);
              break;
     case 7 : //19200 baud
              Serial.begin(19200);
              break;
     case 8 : //28800 baud
              Serial.begin(28800);
              break;
     case 9 : //38400 baud
              Serial.begin(38400);
              break;
     case 10 : //57600 baud
              Serial.begin(57600);
              break;
     case 11 : //115200 baud
              Serial.begin(115200);
              break;
     default : //9600 baud
              Serial.begin(9600);
              break;          
    } //switch
    //Serial.begin(9600);
}  

// The setup() method runs once, when the sketch starts
void setup()   { 
  //On lit la mémoire eeprom pour déterminer le réglage de chaque broche
  for (int i=first_pin_eeprom;i<=last_pin_eeprom;i++) {
   switch (EEPROM.read(i)) {
     case 1 : //C'est une sortie numérique
              pinMode(i,OUTPUT);
              digitalWrite(i,LOW);
              break;
     case 2 : //C'est une Entrée numérique
              pinMode(i,INPUT);
              break;
     case 3 : //C'est une Entrée analogique
              break;
     case 4 : //C'est une sortie PWM
              pinMode(i, OUTPUT);
              analogWrite(i,0);      //Duty cycle=0 ( toujours éteint )
              break;
     case 5 : //C'est une sortie servo-moteur
              #if defined(__mega_ver) 
              switch (i) {
                case 14 : myservo14.attach(14);
                         break;
                case 15 : myservo15.attach(15);
                         break;
                case 16 : myservo16.attach(16);
                         break;
                case 17 : myservo17.attach(17);
                         break;
                case 18 : myservo18.attach(18);
                         break;
                case 19 : myservo19.attach(19);
                         break;
                case 22 : myservo22.attach(22);
                         break;
                case 23 : myservo23.attach(23);
                         break;
                case 24 : myservo24.attach(24);
                         break;
                case 25 : myservo25.attach(25);
                         break;
                case 26 : myservo26.attach(26);
                         break;  
                case 27 : myservo27.attach(27);
                         break;           
              } //switch (i)
              #else
              switch (i) {
                case 4 : myservo4.attach(4);
                         break;
                case 5 : myservo5.attach(5);
                         break;
                case 6 : myservo6.attach(6);
                         break;
                case 7 : myservo7.attach(7);
                         break;
                case 8 : myservo8.attach(8);
                         break;
                case 9 : myservo9.attach(9);
                         break;
                case 10 : myservo10.attach(10);
                         break;
                case 11 : myservo11.attach(11);
                         break;
                case 12 : myservo12.attach(12);
                         break;
                case 13 : myservo13.attach(13);
                         break;
                } //switch (i)
              #endif
              break;  
     case 6 : //C'est une entrée port Infrarouge
              pinMode(i, INPUT);
              IRrecv irrecv(i);
              irrecv.enableIRIn(); // Démarre le récepteur sur la broche i
              break;
   } //switch eeprom.read
  }//for
  //On initialise le port série et le port I2C
  fixbaudrate();
  //Serial.begin(115200);
  Wire.begin();
   //On envoie le nom du modèle de la maquette
  identification(); 
  //On initialise le flag time_out
  time_out=false;
  if (EEPROM.read(lcd_connected)==1) {
      lcd.init();                      // initialize the lcd 
      lcd.backlight();
      lcd.print("  Powered by");
      lcd.setCursor(0,1);
      lcd.print("TECHNOZONE51(c)");
  }
  if (EEPROM.read(autorun)==1) {interpreteur();} //On lance l'interpreteur si l'autorun est mis à 1 
}

// the loop() method runs over and over again,
// as long as the Arduino has power
void loop()                     
{
        if (time_out) {
          //Le temps imparti a été dépassé
          Serial.flush();
          Serial.println("TIME OUT");
          time_out=false;
        }
        //On teste si on reçoit un Octet
        if (Serial.available() > 0) {
		// read the incoming byte:
		incomingByte = Serial.read();
                
                if (incomingByte==90) {
                  //On reçoit le caractère Z qui force le mode à 0 et réinitialise la carte
                  Serial.flush();
                  setup();
                }

                if (incomingByte==65) {
                  //On reçoit le caractère A pour lire sur une entrée Analogique en mode 8bits
                  //On attend le paramètre 1
                  digit_d = lire_octet()-48;
                  //On attend le paramètre 2
                  digit_u = lire_octet()-48;
                  if (!time_out) {
                   //On calcule le numéro de broche à lire
                   no=digit_d*10+digit_u;
                   value = analogRead(no)/4;  //On lit l'état de la broche et on mets le résultat dans value
                   Serial.print("VALUE=");
                   Serial.println(value, DEC); //On renvoie le résultat de la lecture de la broche sur le port série
                   }
                 }

                if (incomingByte==97) {
                  //On reçoit le caractère a pour lire sur une entrée Analogique en mode 10bits
                  //On attend le paramètre 1
                  digit_d = lire_octet()-48;
                  //On attend le paramètre 2
                  digit_u = lire_octet()-48;
                  if (!time_out) {
                   //On calcule le numéro de broche à lire
                   no=digit_d*10+digit_u;
                   value = analogRead(no);  //On lit l'état de la broche et on mets le résultat dans value
                   Serial.print("VALUE=");
                   Serial.println(value, DEC); //On renvoie le résultat de la lecture de la broche sur le port série
                   }
                 }

                if (incomingByte==81) {
                  //On reçoit le caractère Q pour lire la distance d'un capteur ultra-son
                  //On attend le paramètre 1
                  digit_d = lire_octet()-48;
                  //On attend le paramètre 2
                  digit_u = lire_octet()-48;
                  //Lecture paramètre 3
                  n2=lire_octet()-48;
                  //Lecture paramètre 4
                  n3=lire_octet()-48;
                  if (!time_out) {
                   //On calcule le numéro de broche de trigger et on envoie une impulsion
                   no=digit_d*10+digit_u;
                   digitalWrite(no,HIGH);
                   delayMicroseconds(1000);
                   digitalWrite(no,LOW);
                   //On calcul la broche de l'impulsion a lire
                   no=n2*10+n3;
                   value = (pulseIn(no,HIGH)/2)/29.1+2;  //On lit l'impulsion du capteur et on calcule la distance
                   if (value>255) { value=255; }
                   Serial.print("VALUE=");
                   Serial.println(value, DEC); //On renvoie le résultat de la lecture de la broche sur le port série
                   }
                 }


                if (incomingByte==66) {
                  //On reçoit le caractère B pour écrire en EEPROM UN OPCODE à l'adresse indexée par pc
                  //On attend le paramètre 1 ( l'opcode )
                  digit_u = lire_octet();
                  if (!time_out) {
                   if (slot_p==0) {
                     EEPROM.write(pc,digit_u); //Ecriture de l'opcode dans l'EEPROM interne de l'arduino
                     delay(5);
                     }
                     else {
                     i2c_eeprom_write_byte(MEM_ADDR+get_bank(slot_p), pc ,digit_u); //Ecriture de l'opcode dans une mémoire externe I2C
                     delay(5);
                     }
                   pc=pc+1; //On incrémente pc
                   Serial.print("DONE B");
                   Serial.println(digit_u, DEC); //On renvoie la confirmation
                   }
                 }
                #if defined(__mega_ver)
                if (incomingByte==68) {
                  //On reçoit le caractère D pour activer ou non la cession de debuggage
                  //On attend le paramètre 1 ( enable flag )
                  digit_u = lire_octet()-48;
                  if (!time_out) {
                   debug = digit_u;  
                  }
                   Serial.print("DONE D");
                   Serial.println(digit_u, DEC); //On renvoie la confirmation
                  }
                #endif
                if (incomingByte==76) {
                  //On reçoit le caractère L pour une commande de l'écran LCD
                  //On attend la sous commande
                  n1 = lire_octet();
                  if (!time_out) {
                    //On interprète la sous commande
                    if (n1==67) {
                      //Commande LC : efface l'écran LCD
                      lcd.clear();
                    }
                    if (n1==83) {
                      //Commande LSxccraaa : Active ou désactive le LCD et fixe ces caractéristiques
                      //x=0 : LCD non connecté
                      //x=1 : LCD connecté
                      //cc : Nombre de colonnes
                      //r : Nombre de lignes
                      //aaa : adresse I2C du lcd connecté
                      //Lecture paramètre x
                      n2=lire_octet()-48;
                      //Lecture paramètre c
                      n3=lire_octet()-48;
                      //Lecture paramètre c
                      n4=lire_octet()-48;
                      //Lecture paramètre r
                      n5=lire_octet()-48;
                      //Lecture paramètre a
                      n6=lire_octet()-48;
                      //Lecture paramètre a
                      n7=lire_octet()-48;
                      //Lecture paramètre a
                      n8=lire_octet()-48;
                      if (n2==1 && !time_out)
                      {
                        //On active le flag du LCD
                        EEPROM.write(lcd_connected,1);
                        EEPROM.write(lcd_col,n3*10+n4);
                        EEPROM.write(lcd_row,n5);
                        EEPROM.write(lcd_i2c_add,n6*100+n7*10+n8);
                      } else
                      {
                        //On désactive le flag du LCD
                        EEPROM.write(lcd_connected,0);
                        //On ne touche pas aux autres paramètres
                      }  
                    }
                    if (n1==71) {
                      //Commande LGCCR : Positionne le curseur de l'écran LCD sur (CC,R) 
                      //On attend le paramètre 1
                      n2 = lire_octet()-48;
                      //On attend le paramètre 2
                      n3 = lire_octet()-48;
                      //On attend le paramètre 3
                      n4 = lire_octet()-48;
                      //On calcul le numéro de colone  
                      value=n2*10+n3;
                      //On Positionne le curseur à l'endroit voulu
                      if (!time_out) {lcd.setCursor(value, n4);}
                    }
                    if (n1=='D') {
                      //Commande LDh : Ecrire la date contenue dans la variable horaire h à la position courante du curseur sur l'écran LCD 
                      //On attend le paramètre h
                      n2 = lire_octet()-48;
                      if (!time_out) 
                       {
                        RTC.getRAM(n2*7,&hor,1);  //Lire Jour
                        lcd.write((hor/10)+48);
                        lcd.write((hor%10)+48);
                        lcd.write('/');
                        RTC.getRAM(n2*7+1,&hor,1); //Lire mois
                        lcd.write((hor/10)+48);
                        lcd.write((hor%10)+48);
                        lcd.write('/');
                        RTC.getRAM(n2*7+2,&hor,1); //Lire année
                        lcd.write((hor/10)+48);         
                        lcd.write((hor%10)+48);
                       }
                    }                      
                    if (n1=='H') {
                      //Commande LHh : Ecrire l'heure contenue dans la variable horaire h à la position courante du curseur sur l'écran LCD 
                      //On attend le paramètre h
                      n2 = lire_octet()-48;
                      if (!time_out) 
                       {
                        RTC.getRAM(n2*7+3,&hor,1);   //Lire l'heure
                        lcd.write((hor/10)+48);
                        lcd.write((hor%10)+48);
                        lcd.write(':');
                        RTC.getRAM(n2*7+4,&hor,1);   //Lire les minutes
                        lcd.write((hor/10)+48);
                        lcd.write((hor%10)+48);
                        lcd.write(':');
                        RTC.getRAM(n2*7+5,&hor,1);  //Lire les secondes
                        lcd.write((hor/10)+48);
                        lcd.write((hor%10)+48);
                       }
                    }                      
                    if (n1==87) {
                      //Commande LW......| : Ecrit sur l'écran LCD les caractères suivants jusqu'au terminateur |
                      do
                       { 
                        n2 = lire_octet();
                        if (!time_out && n2!=124) {lcd.write(n2);}
                       } while (n2!=124&&!time_out);
                    }  
                    if (n1==86) {
                      //Commande LVvvvccr : Ecrire la valeur vvv sur l'écran LCD à la position (cc,r) 
                      //On lit la valeur a écrire (centaine)
                      digit_c=lire_octet()-48;
                      //On lit la valeur a écrire (dizaine)
                      digit_d=lire_octet()-48;
                      //On lit la valeur a écrire (unite)
                      digit_u=lire_octet()-48;
                      //On lit la colonne ( dizaine )
                      n3=lire_octet()-48;
                      //On lit la colonne ( unité )
                      n4=lire_octet()-48;
                      //On lit la ligne
                      n5=lire_octet()-48;
                      value=digit_c*100+digit_d*10+digit_u;
                      if (!time_out) 
                      {
                       lcd.setCursor(n3*10+n4,n5);
                       lcd.print("   ");
                       lcd.setCursor(n3*10+n4,n5);
                       lcd.print(value);
                      }
                    }
                   if (n1==84) {
                      //Commande LTvvvccr : Ecrire la température correspondant à la valeur vvv sur l'écran LCD à la position (cc,r)
                      //On écrit la température au format +TT,T° 
                      //On lit la valeur a écrire (centaine)
                      digit_c=lire_octet()-48;
                      //On lit la valeur a écrire (dizaine)
                      digit_d=lire_octet()-48;
                      //On lit la valeur a écrire (unite)
                      digit_u=lire_octet()-48;
                      //On lit la colonne ( dizaine )
                      n3=lire_octet()-48;
                      //On lit la colonne ( unité )
                      n4=lire_octet()-48;
                      //On lit la ligne
                      n5=lire_octet()-48;
                      value=digit_c*100+digit_d*10+digit_u;
                      if (!time_out) 
                      {
                       lcd.setCursor(n3*10+n4,n5);
                       lcd.print("         ");
                       lcd.setCursor(n3*10+n4,n5);
                       //Conversion en °C de la valeur value
                       affiche_celsius(value);
                       //lcd.print(value);
                      }
                    }
                   
                   if (n1==76) {
                      //Commande LLvvvccr : Ecrire la luminosité en Lux correspondant à la valeur vvv sur l'écran LCD à la position (cc,r)
                      //On écrit la luminosité au format +LLLL Lx° 
                      //On lit la valeur a écrire (centaine)
                      digit_c=lire_octet()-48;
                      //On lit la valeur a écrire (dizaine)
                      digit_d=lire_octet()-48;
                      //On lit la valeur a écrire (unite)
                      digit_u=lire_octet()-48;
                      //On lit la colonne ( dizaine )
                      n3=lire_octet()-48;
                      //On lit la colonne ( unité )
                      n4=lire_octet()-48;
                      //On lit la ligne
                      n5=lire_octet()-48;
                      value=digit_c*100+digit_d*10+digit_u;
                      if (!time_out) 
                      {
                       lcd.setCursor(n3*10+n4,n5);
                       lcd.print("         ");
                       lcd.setCursor(n3*10+n4,n5);
                       //Conversion en Lux de la valeur value
                       affiche_lux(value);
                       //lcd.print(value);
                      }
                    } 
                  }
                  
                  if (!time_out) {
                  Serial.print("DONE L");
                  Serial.write(n1);
                  Serial.println("");
                  }
                 }
                
                if (incomingByte==77) {
                  //On reçoit le caractère M pour une écriture sur une sortie PWM
                  //On attend le paramètre 1
                  n1 = lire_octet()-48;
                  //On attend le paramètre 2
                  n2 = lire_octet()-48;
                  //On attend le paramètre 3
                  n3 = lire_octet()-48;
                  //On attend le paramètre 4
                  n4 = lire_octet()-48;
                  //On attend le paramètre 5
                  n5 = lire_octet()-48;
                  if (!time_out) {
                  //On calcul le numéro de broche  
                  no=n1*10+n2;
                  //On calcule le duty cycle
                  value=n3*100+n4*10+n5;
                  //sorti sur la broche PWM
                  analogWrite(no,value);      //Duty cycle=value sur la broche no
                  Serial.print("DONE M");
                  Serial.write(n1+48);
                  Serial.write(n2+48);
                  Serial.write(n3+48);
                  Serial.write(n4+48);
                  Serial.write(n5+48);
                  Serial.println("");
                  }
                 }
                
                if (incomingByte==78) {
                  //On reçoit le caractère N pour enregistrer l'identification de la maquette
                  //On attend le paramètre 1
                  n1 = lire_octet();
                  //On attend le paramètre 2
                  n2 = lire_octet();
                  //On attend le paramètre 3
                  n3 = lire_octet();
                  //On attend le paramètre 4
                  n4 = lire_octet();
                  //On attend le paramètre 5
                  n5 = lire_octet();
                  //On attend le paramètre 6
                  n6 = lire_octet();
                  if (!time_out) {
                  //On enregistre l'ID dans l'EEPROM           
                  EEPROM.write(first_name_eeprom,n1);
                  EEPROM.write(first_name_eeprom+1,n2);
                  EEPROM.write(first_name_eeprom+2,n3);
                  EEPROM.write(first_name_eeprom+3,n4);
                  EEPROM.write(first_name_eeprom+4,n5);
                  EEPROM.write(first_name_eeprom+5,n6);
                  Serial.print("DONE N");
                  Serial.write(n1);
                  Serial.write(n2);
                  Serial.write(n3);
                  Serial.write(n4);
                  Serial.write(n5);
                  Serial.write(n6);
                  Serial.println("");
                  }
                 }

                if (incomingByte==80) {
                  //On reçoit le caractère P pour activer ou non la résistance de pullup sur une broche
                  //On attend le paramètre 1
                  digit_d = lire_octet()-48;
                  //On attend le paramètre 2
                  digit_u = lire_octet()-48;
                   //On attend le paramètre 3
                   value = lire_octet()-48;
                   //On peut écrire sur la broche
                   if (!time_out) {
                     //On calcule le numéro de broche sur lequel est connecté le servo moteur
                     no=digit_d*10+digit_u;
                     if (value==0) {
                      digitalWrite(no, LOW); //On met la broche à 0 V
                      } 
                      else
                      {
                      digitalWrite(no, HIGH); //On met la broche à 5 V
                      } // if (value==0)
                   Serial.print("DONE P");
                   Serial.print(digit_d,DEC);
                   Serial.print(digit_u,DEC);
                   Serial.println(value,DEC);
                   }
                 }

                if (incomingByte==82) {
                  //On reçoit le caractère R pour lire sur une entrée numérique
                  //On attend le paramètre 1
                  digit_d = lire_octet()-48;
                  //On attend le paramètre 2
                  digit_u = lire_octet()-48;
                  //On calcule le numéro de broche à lire
                  no=digit_d*10+digit_u;
                  if (!time_out) {
                    //On calcule le numéro de broche à lire
                    no=digit_d*10+digit_u;
                    value = digitalRead(no); //On lit l'état de la broche et on mets le résultat dans value
                    Serial.print("VALUE=");
                    Serial.println(value, DEC); //On renvoie le résultat de la lecture de la broche sur le port série
                    }
                 }

                 if (incomingByte==73) {
                  //On reçoit le caractère I pour lire sur l'entrée infrarouge
                  //On attend le paramètre 1
                  digit_d = lire_octet()-48;
                  //On attend le paramètre 2
                  digit_u = lire_octet()-48;
                  if (!time_out) {
                    if (irrecv.decode(&results)) {
                         String s=String(long(results.value), HEX);
                         s.toUpperCase();
                         Serial.print("VALUE=");
                         Serial.println(s); //On renvoie le résultat de la lecture de la broche sur le port série
//                         delay(100);
//                        irrecv.resume(); // Receive the next value
                     } else {Serial.println("VALUE=-1");}
                   }
                 }

                 if (incomingByte==74) {
                  //On reçoit le caractère J pour effacer le tampon de la télécommande
                  irrecv.resume(); // Receive the next value
                  Serial.println("DONE J");                
                 }

                if (incomingByte==84) {
                  //On reçoit le caractère T pour activer ou non l'autorun
                  //On attend le paramètre 1
                  value = lire_octet()-48;
                  //On peut écrire sur la broche
                  if (value==0) {
                     //On désactive l'autorun
                     EEPROM.write(autorun,0); 
                    } 
                    else
                    {
                      //On active l'autorun
                      EEPROM.write(autorun,1); 
                    } // if (value==0)
                   Serial.print("DONE T");
                   Serial.println(value,DEC);
                 }
                
                if (incomingByte==87) {
                  //On reçoit le caractère W pour l'écriture sur une sortie numérique
                  //On attend le paramètre 1
                  digit_d = lire_octet()-48;
                  //On attend le paramètre 2
                  digit_u = lire_octet()-48;
                  //On attend le paramètre 3
                  value = lire_octet()-48;
                  //On peut écrire sur la broche
                  if (!time_out) {
                    //On calcule le numéro de broche sur lequel est connecté le servo moteur
                    no=digit_d*10+digit_u;
                    if (value==0) {
                      digitalWrite(no, LOW); //On met la broche à 0 V
                    } 
                    else
                    {
                      digitalWrite(no, HIGH); //On met la broche à 5 V
                    } // if (value==0)
                   Serial.print("DONE W");
                   Serial.print(digit_d,DEC);
                   Serial.print(digit_u,DEC);
                   Serial.println(value,DEC);
                   }
                 }
                
                if (incomingByte==69) {
                  //On reçoit le caractère E pour l'écriture en EEPROM
                  //On attend le paramètre 1
                  digit_d = lire_octet()-48;
                  //On attend le paramètre 2
                  digit_u = lire_octet()-48;
                  //On attend le paramètre 3
                  value = lire_octet()-48;
                  if (!time_out) {
                   //On peut écrire dans l'eeprom
                   //On calcule le numéro de broche
                   no=digit_d*10+digit_u; 
                   EEPROM.write(no,value); //Ecriture du status dans l'EEPROM
                   Serial.print("DONE E");
                   Serial.print(digit_d,DEC);
                   Serial.print(digit_u,DEC);
                   Serial.println(value,DEC);
                   }
                 }  

                if (incomingByte==70) {
                  //On reçoit le caractère F pour initialiser le compteur pc pour l'écriture d'un programme compilé en EEPROM
                  //On attend le paramètre 1 codé sur deux octets // La cible 0=EEPROM interne slot0 , 1 =EEPROM bank 0 slot1, 2 = EEPROM bank 0 slot2 ...
                  digit_d=lire_octet()-48;   //Lecture chiffre des dizaines
                  digit_u = lire_octet()-48; //Lecture chiffre des unités
                  slot_p=digit_d*10+digit_u; //Calcul du slot à programmer
                  if (!time_out) {
                     pc=get_first_pc(slot_p);  //On initialise le PC sur le premier octet d'une mémoire externe en fonction du slot
                     // EEPROM.write(target,digit_u); //On sauve la Mémoire cible du programme dans l'eeprom
                     Serial.print("DONE F");
                     Serial.print(digit_d,DEC);
                     Serial.println(digit_u,DEC);
                    }
                 }  
                
                if (incomingByte==71) {
                  //On reçoit le caractère G pour initialiser le flag run d'un slot contenant un programme compilé en EEPROM
                  //On attend le paramètre 1 codé sur deux octets // La cible 0=EEPROM interne slot0 , 1 =EEPROM bank 0 slot1, 2 = EEPROM bank 0 slot2 ...
                  digit_d=lire_octet()-48;   //Lecture chiffre des dizaines
                  digit_u = lire_octet()-48; //Lecture chiffre des unités
                  value=lire_octet()-48;     //Lecture de la valeur 0 (désactivation slot) ou 1 ( activation slot ) 
                  slot_p=digit_d*10+digit_u; //Calcul du slot à activer ou non
                  if (!time_out) {
                     EEPROM.write(run+slot_p,value); //On sauve le flag run du slot dans l'eeprom interne
                     Serial.print("DONE G");
                     Serial.print(digit_d,DEC);
                     Serial.print(digit_u,DEC);
                     Serial.println(value,DEC);
                   }  
                 }  
                
                if (incomingByte==98) {
                  //On reçoit le caractère b pour modifier la vitesse du port série
                  //On attend le paramètre 1 codé sur un octet qui indique le baudrate
                  // 0 = 300 baud
                  // 1 = 9600 baud
                  // 2 = 1200 baud
                  // 3 = 2400 baud
                  // 4 = 4800 baud
                  // 5 = 9600 baud
                  // 6 = 14400 baud
                  // 7 = 19200 baud
                  // 8 = 28800 baud
                  // 9 = 38400 baud
                  // 10 = 57600 baud
                  // 11 = 115200 baud
                  digit_d=lire_octet()-48;   //Lecture chiffre des dizaines
                  digit_u = lire_octet()-48; //Lecture chiffre des unités
                  value=digit_d*10+digit_u;   //calcul du parametre baudrate
                  if (value>11) {value=7;}
                  if (!time_out) {
                     EEPROM.write(baud_rate,value); //On sauve le parametre baud_rate en EEPROM interne
                     Serial.print("DONE b");
                     Serial.print(digit_d,DEC);
                     Serial.println(digit_u,DEC);
                     fixbaudrate();    //On modifie la vitesse de la liaison série
                   }  
                 }
                 
                if (incomingByte==72) {
                  //On reçoit le caractère H pour sauver le nom du programme d'un slot
                  //format : HSSNNNNNNNN
                  digit_d=lire_octet()-48;   //Lecture chiffre des dizaines
                  digit_u = lire_octet()-48; //Lecture chiffre des unités
                  n1=lire_octet();     //Lecture du nom du programme dans le slot 
                  n2=lire_octet();     //Lecture du nom du programme dans le slot 
                  n3=lire_octet();     //Lecture du nom du programme dans le slot 
                  n4=lire_octet();     //Lecture du nom du programme dans le slot 
                  n5=lire_octet();     //Lecture du nom du programme dans le slot 
                  n6=lire_octet();     //Lecture du nom du programme dans le slot 
                  n7=lire_octet();     //Lecture du nom du programme dans le slot 
                  n8=lire_octet();     //Lecture du nom du programme dans le slot 
                  slot_p=digit_d*10+digit_u; //Calcul du slot à activer ou non
                  if (!time_out) {
                     EEPROM.write(name_slot+slot_p*8,n1); //On sauve le nom du programme dans le slot
                     EEPROM.write(name_slot+slot_p*8+1,n2); //On sauve le nom du programme dans le slot
                     EEPROM.write(name_slot+slot_p*8+2,n3); //On sauve le nom du programme dans le slot
                     EEPROM.write(name_slot+slot_p*8+3,n4); //On sauve le nom du programme dans le slot
                     EEPROM.write(name_slot+slot_p*8+4,n5); //On sauve le nom du programme dans le slot
                     EEPROM.write(name_slot+slot_p*8+5,n6); //On sauve le nom du programme dans le slot
                     EEPROM.write(name_slot+slot_p*8+6,n7); //On sauve le nom du programme dans le slot
                     EEPROM.write(name_slot+slot_p*8+7,n8); //On sauve le nom du programme dans le slot
                     
                     Serial.print("DONE H");
                     Serial.print(digit_d,DEC);
                     Serial.print(digit_u,DEC);
                     Serial.write(n1);
                     Serial.write(n2);
                     Serial.write(n3);
                     Serial.write(n4);
                     Serial.write(n5);
                     Serial.write(n6);
                     Serial.write(n7);
                     Serial.write(n8);
                     Serial.println("");
                   }  
                 }

                if (incomingByte==79) {
                  //On reçoit le caractère O pour produire un son sur la broche spécifié
                  //format : Obbffffdddd
                  digit_d=lire_octet()-48;   //Lecture chiffre des dizaines
                  digit_u = lire_octet()-48; //Lecture chiffre des unités
                  n1=lire_octet()-48;     //Lecture fréquence millier 
                  n2=lire_octet()-48;     //Lecture fréquence centaine 
                  n3=lire_octet()-48;     //Lecture fréquence dizaine 
                  n4=lire_octet()-48;     //Lecture fréquence unite  
                  n5=lire_octet()-48;     //Lecture duree millier 
                  n6=lire_octet()-48;     //Lecture duree centaine 
                  n7=lire_octet()-48;     //Lecture duree dizaine 
                  n8=lire_octet()-48;     //Lecture duree unite 
                  slot_p=digit_d*10+digit_u; //Calcul du slot à activer ou non
                  long freq=1000*n1+100*n2+10*n3+n4;
                  long dur=1000*n5+100*n6+10*n7+n8;
                  if (!time_out) {
                     buzz(slot_p,freq,dur);
                     Serial.print("DONE O");
                     Serial.print(digit_d,DEC);
                     Serial.print(digit_u,DEC);
                     Serial.print(n1,DEC);
                     Serial.print(n2,DEC);
                     Serial.print(n3,DEC);
                     Serial.print(n4,DEC);
                     Serial.print(n5,DEC);
                     Serial.print(n6,DEC);
                     Serial.print(n7,DEC);
                     Serial.print(n8,DEC);
                     Serial.println("");
                   }  
                 }
                 
                if (incomingByte==75) {
                  //On reçoit le caractère K pour lire le nom du programme d'un slot
                  //format : KSS
                  //Renvoie : Value=NNNNNNNNS
                  digit_d=lire_octet()-48;   //Lecture chiffre des dizaines
                  digit_u = lire_octet()-48; //Lecture chiffre des unités
                  slot_p=digit_d*10+digit_u; //Calcul du slot à activer ou non
                  if (!time_out) {
                     n1=EEPROM.read(name_slot+slot_p*8);  //On lit le nom du programme dans le slot
                     n2=EEPROM.read(name_slot+slot_p*8+1);  //On lit le nom du programme dans le slot
                     n3=EEPROM.read(name_slot+slot_p*8+2);  //On lit le nom du programme dans le slot
                     n4=EEPROM.read(name_slot+slot_p*8+3);  //On lit le nom du programme dans le slot
                     n5=EEPROM.read(name_slot+slot_p*8+4);  //On lit le nom du programme dans le slot
                     n6=EEPROM.read(name_slot+slot_p*8+5);  //On lit le nom du programme dans le slot
                     n7=EEPROM.read(name_slot+slot_p*8+6);  //On lit le nom du programme dans le slot
                     n8=EEPROM.read(name_slot+slot_p*8+7);  //On lit le nom du programme dans le slot
                     value=EEPROM.read(run+slot_p);  //Lire l'état du programme ( activé ou non )
                     Serial.print("VALUE=");
                     Serial.write(n1);
                     Serial.write(n2);
                     Serial.write(n3);
                     Serial.write(n4);
                     Serial.write(n5);
                     Serial.write(n6);
                     Serial.write(n7);
                     Serial.write(n8);
                     Serial.print(":");
                     Serial.println(value,DEC);
                   }  
                 }
                 
                if (incomingByte==83) {
                  //On reçoit le caractère S pour la commande d'un servomoteur
                  //On attend le paramètre 1
                  digit_d = lire_octet()-48;
                  //On attend le paramètre 2
                  digit_u = lire_octet()-48;
                  //On calcule le numéro de broche sur lequel est connecté le servo moteur
                  no=digit_d*10+digit_u;
                  //On attend le paramètre 3
                  digit_c = lire_octet()-48;
                  //On attend le paramètre 4
                  digit_d = lire_octet()-48;
                  //On attend le paramètre 5
                  digit_u = lire_octet()-48;
                  if (!time_out) {
                  //On calcule la valeur de la position
                  value=digit_c*100+digit_d*10+digit_u;
                  #if defined(__mega_ver)
                   switch(no) {
                    case 14 :
                            myservo14.write(value);
                            break;
                    case 15 :
                            myservo15.write(value);
                            break;
                    case 16 :
                            myservo16.write(value);
                            break;
                    case 17 :
                            myservo17.write(value);
                            break;
                    case 18 :
                            myservo18.write(value);
                            break;
                    case 19 :
                            myservo19.write(value);
                            break;
                    case 22 :
                            myservo22.write(value);
                            break;
                    case 23 :
                            myservo23.write(value);
                            break;
                    case 24 :
                            myservo24.write(value);
                            break;
                    case 25 :
                            myservo25.write(value);
                            break;
                    case 26 :
                            myservo26.write(value);
                            break;
                    case 27 :
                            myservo27.write(value);
                            break;        
                   }  
                   #else
                   switch(no) {
                    case 4 :
                            myservo4.write(value);
                            break;
                    case 5 :
                            myservo5.write(value);
                            break;
                    case 6 :
                            myservo6.write(value);
                            break;
                    case 7 :
                            myservo7.write(value);
                            break;
                    case 8 :
                            myservo8.write(value);
                            break;
                    case 9 :
                            myservo9.write(value);
                            break;
                    case 10 :
                            myservo10.write(value);
                            break;
                    case 11 :
                            myservo11.write(value);
                            break;
                    case 12 :
                            myservo12.write(value);
                            break;
                    case 13 :
                            myservo13.write(value);
                            break;
                    } 
                   #endif
                   //On envoie l'accusé de réception
                   Serial.println("DONE S");
                  } 
                }
                                
                if (incomingByte==86) {
                  //On reçoit le caractère V : on renvoit la version du modèle
                  identification();
                }
                
                if (incomingByte==88) {
                  //On reçoit le caractère X : on interprète le programme compilé stocké en EEPROM
                  Serial.println("PROGRAM RUNNING");
                  interpreteur();
                }

                if (incomingByte==105) {
                  //On reçoit le caractère i : Commande de gestion I2C
                  //On attends la sous-commande
                  n1 = lire_octet();
                  if (!time_out) {
                    //On interprète la sous-commande R
                    if (n1=='R') {
                      //Commande iRAAANN : Provoquer la lecture de NN octets sur le composant I2C d'adresse 7bit AAA
                      n2 = lire_octet()-48; //Lecture A2
                      n3 = lire_octet()-48; //Lecture A1
                      n4 = lire_octet()-48; //Lecture A0
                      n5 = lire_octet()-48; //Lecture N1
                      n6 = lire_octet()-48; //lecture N0
                      n7 = n2*100+n3*10+n4; //Calcul de l'adresse I2C du composant
                      n8 = n5*10+n6;        //Calcul du nombre d'octets à lire
                      if (!time_out) {
                       //On exécute la commande
                       Wire.requestFrom(n7,n8);
                       Serial.print("VALUE=");
                       for (byte i=1;i<n8;i++) {
                         delay(1);
                         n9=Wire.read();
                         Serial.print(n9,DEC);
                         Serial.print(",");
                       }
                       delay(1);
                       n9=Wire.read();
                       Wire.endTransmission();
                       Serial.println(n9,DEC);
                       delay(1);
                      } 
                    } //Fin sous commande R
                 //On interprète la sous-commande W
                    if (n1=='W') {
                      //Commande iWAAANNDDD... : Provoquer l'écriture de NN octets (DDD) sur le composant I2C d'adresse 7bit AAA
                      n2 = lire_octet()-48; //Lecture A2
                      n3 = lire_octet()-48; //Lecture A1
                      n4 = lire_octet()-48; //Lecture A0
                      n5 = lire_octet()-48; //Lecture N1
                      n6 = lire_octet()-48; //lecture N0
                      n7 = n2*100+n3*10+n4; //Calcul de l'adresse I2C du composant
                      n8 = n5*10+n6;        //Calcul du nombre d'octets à lire
                      if (!time_out) {
                       //On exécute la commande
                       Wire.beginTransmission(n7);  //démarre la communication avec le composant I2C
                       for (byte i=1;i<=n8;i++){
                         //On envoie la donnée n°i
                         n2 = lire_octet()-48; //Lecture D2
                         n3 = lire_octet()-48; //Lecture D1
                         n4 = lire_octet()-48; //Lecture D0
                         n7 = n2*100+n3*10+n4; //Calcul de la donnée à transmettre
                         delay(1);
                         Wire.write(n7);
                       }
                       delay(1);
                       Wire.endTransmission();      //Stoppe la communication avec le composant I2C
                       Serial.println("DONE iW");
                      } 
                    } //Fin sous commande W   
                 //On interprète la sous-commande w
                    if (n1=='w') {
                      //Commande iwAAANNDDD... : Provoquer l'écriture de NN octets (DDD) sur le composant I2C d'adresse 7bit AAA et
                      //garde la communication ouverte
                      n2 = lire_octet()-48; //Lecture A2
                      n3 = lire_octet()-48; //Lecture A1
                      n4 = lire_octet()-48; //Lecture A0
                      n5 = lire_octet()-48; //Lecture N1
                      n6 = lire_octet()-48; //lecture N0
                      n7 = n2*100+n3*10+n4; //Calcul de l'adresse I2C du composant
                      n8 = n5*10+n6;        //Calcul du nombre d'octets à lire
                      if (!time_out) {
                       //On exécute la commande
                       Wire.beginTransmission(n7);  //démarre la communication avec le composant I2C
                       for (byte i=1;i<=n8;i++){
                         //On envoie la donnée n°i
                         n2 = lire_octet()-48; //Lecture D2
                         n3 = lire_octet()-48; //Lecture D1
                         n4 = lire_octet()-48; //Lecture D0
                         n7 = n2*100+n3*10+n4; //Calcul de la donnée à transmettre
                         Wire.write(n7);
                       }
                       Wire.endTransmission(false);      //Garde la communication avec le composant I2C ouverte restart
                       Serial.println("DONE iw");
                      } 
                    } //Fin sous commande w   
                    
                  } //!time out
                }//Fin de commande I2C

 
                if (incomingByte==89) {
                  //On reçoit le caractère Y : Commande de gestion horaire
                  //On attends la sous-commande
                  n1 = lire_octet();
                  if (!time_out) {
                    //On interprète la sous-commande
                    if (n1=='C') {
                      //Commande YCh1h2h3 : Calcul la durée séparant les date h2 et h1 et stocke le résultat dans h3
                      n2 = lire_octet()-48; //Lecture h1
                      n3 = lire_octet()-48; //Lecture h2
                      n4 = lire_octet()-48; //Lecture h3
                      if (!time_out) {
                       duree(n2,n3,n4); 
                       Serial.println("DONE YC");
                      } 
                    }
                    if (n1==83) {
                      //Commande YSjjmmaahhmmss : mise a l'heure et démarrage de l'horloge
                      n2 = lire_octet()-48; //Lecture j
                      n3 = lire_octet()-48; //Lecture j
                      n4 = lire_octet()-48; //Lecture m
                      n5 = lire_octet()-48; //Lecture m
                      n6 = lire_octet()-48; //Lecture a
                      n7 = lire_octet()-48; //Lecture a
                      n8 = lire_octet()-48; //Lecture h
                      n9 = lire_octet()-48; //Lecture h
                      n10 = lire_octet()-48; //Lecture m
                      n11 = lire_octet()-48; //Lecture m
                      n12 = lire_octet()-48; //Lecture s
                      n13 = lire_octet()-48; //Lecture s
                      if (!time_out) {
                       RTC.stopClock();
                       RTC.fillByYMD(2000+n6*10+n7,n4*10+n5,n2*10+n3);
                       RTC.fillByHMS(n8*10+n9,n10*10+n11,n12*10+n13);
                       RTC.setTime();
                       RTC.startClock();
                       Serial.println("DONE YS");
                      } 
                    }
                    if (n1==86) {
                      //Commande YVhx : Renvoie la partie x de la variable horaire n°h 
                      n2 = lire_octet()-48; //Lecture h
                      n4 = lire_octet();    //Lecture x
                      if (!time_out) 
                      {
                        //On effectue la copie
                        n5=6;  //Par défaut on renvoie le jour de la semaine 
                        switch (n4) {
                          case 'J' : 
                             n5=0;
                             break;
                          case 'M' :
                             n5=1;
                             break;
                          case 'A' :
                             n5=2; 
                             break;
                          case 'h' :
                             n5=3;
                             break;
                          case 'm' :
                             n5=4;
                             break;
                          case 's' :
                             n5=5;
                             break;
                          case 'o' :
                             n5=6;
                             break;   
                        }
                        RTC.getRAM(n2*7+n5,&hor,1);
                        Serial.print("VALUE=");
                        Serial.println(hor,DEC);
                      }
                    }
                    if (n1==82) {
                      //Commande YRh : Lecture de la date et l'heure et stockage dans la mémoire horaire h ( 0 à 7 )
                      n2 = lire_octet()-48; //Lecture h
                      if (!time_out) {
                      RTC.getTime();
                      hor=RTC.day;
                      RTC.setRAM(n2*7,&hor,1);
                      hor=RTC.month;
                      RTC.setRAM(n2*7+1,&hor,1);
                      hor=RTC.year-2000;
                      RTC.setRAM(n2*7+2,&hor,1);
                      hor=RTC.hour;
                      RTC.setRAM(n2*7+3,&hor,1);
                      hor=RTC.minute;
                      RTC.setRAM(n2*7+4,&hor,1);
                      hor=RTC.second;
                      RTC.setRAM(n2*7+5,&hor,1);
                      hor=RTC.dow;
                      RTC.setRAM(n2*7+6,&hor,1);
                      Serial.print("VALUE=");
                      if (RTC.day < 10)                    // correct date if necessary
                      {
                        Serial.print("0");
                        Serial.print(RTC.day, DEC);
                      }else
                      {
                        Serial.print(RTC.day, DEC);
                      }
                      Serial.print("/");
                      if (RTC.month < 10)                   // correct month if necessary
                      {
                        Serial.print("0");
                        Serial.print(RTC.month, DEC);
                      }
                      else
                      {
                        Serial.print(RTC.month, DEC);
                      }
                      Serial.print("/");
                      Serial.print(RTC.year-2000, DEC);          // Year need not to be changed
                      Serial.print(" ");
                      if (RTC.hour < 10)                    // correct hour if necessary
                      {
                        Serial.print("0");
                        Serial.print(RTC.hour, DEC);
                      } 
                      else
                      {
                        Serial.print(RTC.hour, DEC);
                      }
                      Serial.print(":");
                      if (RTC.minute < 10)                  // correct minute if necessary
                      {
                        Serial.print("0");
                        Serial.print(RTC.minute, DEC);
                      }
                      else
                      {
                        Serial.print(RTC.minute, DEC);
                      }
                      Serial.print(":");
                      if (RTC.second < 10)                  // correct second if necessary
                      {
                        Serial.print("0");
                        Serial.print(RTC.second, DEC);
                      }
                      else
                      {
                        Serial.print(RTC.second, DEC);
                      }
                      Serial.print(" ");
                      Serial.println(RTC.dow,DEC);
                      }//timeout
                    }//sous commande 
                }//Fin de commande horaire
                
            }               
	}
}
