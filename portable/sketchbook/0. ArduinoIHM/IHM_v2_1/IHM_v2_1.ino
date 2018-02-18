/**
    Interface graphique Arduino pour la gestion en temps réel de commande tout où rien,
    PWM et servomoteur.
  
    @author :     LOÏC BEAUFILS<lowstation@hotmail.fr>
    @author :     Grégory Fromain <gregoryfromain@gmail.com>
    
    @since : 	  26/11/11
    @license :    http://creativecommons.org/licenses/by-nc/3.0/
    @link :       http://www.pobot.org/Une-interface-IHM-pour-Arduino.html
    @version :    2.1
    
    Cette version n'est pas totalement optimisée !
*/

// LIBRAIRIE
#include <Servo.h>

//DECLARATION
int RESU;

char longueur_trame = 0;

Servo mon_entree13;
Servo mon_entree12;
Servo mon_entree11;
Servo mon_entree10;
Servo mon_entree9;
Servo mon_entree8;
Servo mon_entree7;
Servo mon_entree6;
Servo mon_entree5;
Servo mon_entree4;
Servo mon_entree3;
Servo mon_entree2;
char Res[6];
char ReceptionDestruction;
int read0 = 0;
int read1 = 0;
int read2 = 0;
int read3 = 0;
int read4 = 0;
int read5 = 0;
int tension0 = 0;
int tension1 = 0;
int tension2 = 0;
int tension3 = 0;
int tension4 = 0;
int tension5 = 0;

// SETUP D'INTITIALISATION
void setup()   
{                
  Serial.begin(115200);
  
  for( byte i = 2; i<=13; i++)
  {
    pinMode(i,OUTPUT);
  }
}

// LOOP : BOUCLE DU PROGRAMME
void loop()                     
{
  
  if (Serial.available()>0)                           //SI CARACTERE DANS LA FILE D'ATTENTE
  {
    while (Serial.available()>0)                      //LECTURE DU MESSAGE
    { 
      if(longueur_trame<=5)
      {
        Res[longueur_trame]=Serial.read();  
        longueur_trame++;
      }
      else
      {
        ReceptionDestruction=Serial.read(); 
      }
      delay(2);                                       // pause pour laisser le temps au Serial.available de recevoir les caractères
    }
  
  
    if(Res[1] == 'A')							                    // MESSAGE $AD001 PERMET D'ACTIVER EN 5V LA SORTIE 13.
    {
      switch (Res[2]) {
        case 'D':
          mon_entree13.detach();
          break;
          
        case 'C':
          mon_entree12.detach();
          break;
          
        case 'B':
          mon_entree11.detach();
          break;
          
        case 'A':
          mon_entree10.detach();
          break;
          
        case '9':
          mon_entree9.detach();
          break;
          
        case '8':
          mon_entree8.detach();
          break;
          
        case '7':
          mon_entree7.detach();
          break;
          
        case '6':
          mon_entree6.detach();
          break;
          
        case '5':
          mon_entree5.detach();
          break;
          
        case '4':
          mon_entree4.detach();
          break;
          
        case '3':
          mon_entree3.detach();
          break;
          
        case '2':
          mon_entree2.detach();
          break;
      }   
      activation_sortie( Res );   
    }
  
    
    if((Res[0]=='$')&&(Res[1]=='B'))                  //MESSAGE $BBXXX PERMET DE COMMANDER LA SORTIE 11,10,9,6,5,3 EN PWM
    {    
      switch (Res[2])
      {
          case 'B':
            mon_entree11.detach();
            break;
            
          case 'A':
            mon_entree10.detach();
            break;
            
          case '9':
            mon_entree9.detach();
            break;
            
          case '6':
            mon_entree6.detach();
            break;
          
          case '5':
            mon_entree5.detach();
            break;
          
          case '3':
            mon_entree3.detach();
            break;
      }
      
     activation_sortie( Res ); 
    }
    
    
    if((Res[0]=='$')&&(Res[1]=='D'))                  // LES COMMANDES SERVOS.
    {
      RESU = ascii_vers_int(Res[3], Res[4], Res[5]);
      char num_pin = numero_pin(Res[2]);
      switch (Res[2])
      {
      
        case 'D':
          mon_entree13.attach(num_pin);
          mon_entree13.write(RESU);
          break;
       
        case 'C':
          mon_entree12.attach(num_pin);
          mon_entree12.write(RESU);
          break;
          
        case 'B':
          mon_entree11.attach(num_pin);
          mon_entree11.write(RESU);
          break;
          
        case 'A':
          mon_entree10.attach(num_pin);
          mon_entree10.write(RESU);
          break;
          
        case '9':
          mon_entree9.attach(num_pin);
          mon_entree9.write(RESU);
          break;
          
        case '8':
          mon_entree8.attach(num_pin);
          mon_entree8.write(RESU);
          break;
          
        case '7':
          mon_entree7.attach(num_pin);
          mon_entree7.write(RESU);
          break;
          
        case '6':
          mon_entree6.attach(num_pin);
          mon_entree6.write(RESU);
          break;
          
        case '5':
          mon_entree5.attach(num_pin);
          mon_entree5.write(RESU);
          break;
          
        case '4':
          mon_entree4.attach(num_pin);
          mon_entree4.write(RESU);
          break;
          
        case '3':
          mon_entree3.attach(num_pin);
          mon_entree3.write(RESU);
          break;
          
        case '2':
          mon_entree2.attach(num_pin);
          mon_entree2.write(RESU);
          break;       
      }
    
     activation_sortie( Res );  
    }
    
        if((Res[0]=='$')&&(Res[1]=='C'))                  // LES COMMANDES SERVOS.
    {
      RESU = ascii_vers_int(Res[3], Res[4], Res[5]);
      char num_pin = numero_pin(Res[2]);
      switch (Res[2])
      {
      
        case '0':      
          if(Res[5]=='1')
            read0=1;
          if(Res[5]=='0')
          {
            read0=0;
            input0("$C0000");
          }
          input0(Res);
          break;
       
        case '1':
          if(Res[5]=='1')
            read1=1;
          if(Res[5]=='0')
          {
            read1=0;
            input0("$C1000");
          }
          input1(Res);
          break;
          
        case '2':
          if(Res[5]=='1')
            read2=1;
          if(Res[5]=='0')
          {
            read2=0;
            input0("$C2000");
          }
          input2(Res);
          break;
          
        case '3':
          if(Res[5]=='1')
            read3=1;
          if(Res[5]=='0')
          {
            read3=0;
            input0("$C3000");
          }
          input3(Res);
          break;
          
        case '4':
          if(Res[5]=='1')
            read4=1;
          if(Res[5]=='0')
          {
            read4=0;
            input0("$C4000");
          }
          input4(Res);
          break;
          
        case '5':
          if(Res[5]=='1')
            read5=1;
          if(Res[5]=='0')
          {
            read5=0;
            input0("$C5000");
          }
          input5(Res);
          break;
          
        case '6':
            read0=0;            
            read1=0;
            read2=0;
            read3=0;
            read4=0;
            read5=0;
          break;
        
      }
    }
    longueur_trame=0;      
  }
if(read0==1)
{
  input0("$CA001");
}
if(read1==1)
{
  input1("$C1001");
}
if(read2==1)
{
  input2("$C2001");
}
if(read3==1)
{
  input3("$C3001");
}
if(read4==1)
{
  input4("$C4001");
}
if(read5==1)
{
  input5("$C5001");
}
}

/**
Permet de retrouver le numero de la pin en fonction d'un hexa en ascii
  @param char hexa en ascii
  @return char correspondant au numero de la broche
*/
char numero_pin (char val_ascii)
{
  char val_retour;
  switch (Res[2])
  {
    case 'D':
      val_retour = 13;
      break;
 
    case 'C':
      val_retour = 12;
      break;
      
    case 'B':
      val_retour = 11;
      break;
      
    case 'A':
      val_retour = 10;
      break;
            
    default:
      val_retour = val_ascii-48;
  }
  return val_retour;
}

/**
Permet de pouvoir réutiliser la sortie en numerique simple.
  @param char[6] la trame complete
*/
void activation_sortie ( char trame[6] )
{
  char led_pin = numero_pin(trame[2]);                 // On recupere la valeur décimal du numero de broche
  switch (trame[1])
    {
      case 'A':
        if(trame[5]=='1')
          digitalWrite(led_pin, HIGH); 
        if(trame[5]=='0')
          digitalWrite(led_pin, LOW); 
        break;
   
      case 'B':
        RESU = ascii_vers_int(trame[3], trame[4], trame[5]);//((trame[3]-48)*100) + ((trame[4]-48)*10) + trame[5]-48;
        analogWrite(led_pin, RESU);
        break;
    }
    transfert(trame);
}

/**
Permet de faire la conversion entre 3 char et un ascii 
  @param char chiffre des centaines
  @param char chiffre dixaines
  @param char chiffre des unitées
  @return int le nombre entier
*/
int ascii_vers_int( char centaine, char dixaine, char unite)
{
  int conversion = ((centaine-48)*100) + ((dixaine-48)*10) + unite-48;
  return ( conversion );
}

void transfert( char frame[6] )
{
Serial.print(frame[0]);
Serial.print(frame[1]);
Serial.print("=");
Serial.print(frame[2]);
Serial.print(frame[3]);
Serial.print(frame[4]);
Serial.print(frame[5]);
Serial.print("\n");
}

void input0( char trame[6] )
{
  tension0 = analogRead(0);
  delay(20);
  tension0 = map(tension0, 0 , 1023, 0, 500);
  
  Serial.print(trame[0]);
  Serial.print(trame[1]);
  Serial.print("=");
  Serial.print(trame[2]);
  
  if(tension0<10){
     Serial.print("00");
     Serial.print(tension0);
   }
   if(tension0<100 && tension0>=10){
     Serial.print("0");
     Serial.print(tension0);
   }
   if(tension0>=100){
     Serial.print(tension0);
   }
   Serial.print("\n");   
}

void input1( char trame[6] )
{
  tension1 = analogRead(1);
  delay(20);
  tension1 = map(tension1, 0 , 1023, 0, 500);
  
  Serial.print(trame[0]);
  Serial.print(trame[1]);
  Serial.print("=");
  Serial.print(trame[2]);
  
  if(tension1<10){
     Serial.print("00");
     Serial.print(tension1);
   }
   if(tension1<100 && tension1>=10){
     Serial.print("0");
     Serial.print(tension1);
   }
   if(tension1>=100){
     Serial.print(tension1);
   }
   Serial.print("\n");   
}

void input2( char trame[6] )
{
  tension2 = analogRead(2);
  delay(20);
  tension2 = map(tension2, 0 , 1023, 0, 500);
  
  Serial.print(trame[0]);
  Serial.print(trame[1]);
  Serial.print("=");
  Serial.print(trame[2]);
  
  if(tension2<10){
     Serial.print("00");
     Serial.print(tension2);
   }
   if(tension2<100 && tension2>=10){
     Serial.print("0");
     Serial.print(tension2);
   }
   if(tension2>=100){
     Serial.print(tension2);
   }
   Serial.print("\n");   
}

void input3( char trame[6] )
{
  tension3 = analogRead(3);
  delay(20);
  tension3 = map(tension3, 0 , 1023, 0, 500);
  
  Serial.print(trame[0]);
  Serial.print(trame[1]);
  Serial.print("=");
  Serial.print(trame[2]);
  
  if(tension3<10){
     Serial.print("00");
     Serial.print(tension3);
   }
   if(tension3<100 && tension3>=10){
     Serial.print("0");
     Serial.print(tension3);
   }
   if(tension3>=100){
     Serial.print(tension3);
   }
   Serial.print("\n");   
}

void input4( char trame[6] )
{
  tension4 = analogRead(4);
  delay(20);
  tension4 = map(tension4, 0 , 1023, 0, 500);
  
  Serial.print(trame[0]);
  Serial.print(trame[1]);
  Serial.print("=");
  Serial.print(trame[2]);
  
  if(tension4<10){
     Serial.print("00");
     Serial.print(tension4);
   }
   if(tension4<100 && tension4>=10){
     Serial.print("0");
     Serial.print(tension4);
   }
   if(tension4>=100){
     Serial.print(tension4);
   }
   Serial.print("\n");   
}

void input5( char trame[6] )
{
  tension5 = analogRead(5);
  delay(20);
  tension5 = map(tension5, 0 , 1023, 0, 500);
  
  Serial.print(trame[0]);
  Serial.print(trame[1]);
  Serial.print("=");
  Serial.print(trame[2]);
  
  if(tension5<10){
     Serial.print("00");
     Serial.print(tension5);
   }
   if(tension5<100 && tension5>=10){
     Serial.print("0");
     Serial.print(tension5);
   }
   if(tension5>=100){
     Serial.print(tension5);
   }
   Serial.print("\n");   
}
