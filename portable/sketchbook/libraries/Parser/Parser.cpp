/*
  Parse.cpp - Library for Parsing Get and Post on requests.
  Created by Professor J and Professor R, March 20, 2018.
  Released into the public domain.
*/

#include <arduino.h>
#include <Parser.h>


Parser::Parser()
{

}

void Parser::readStream(EthernetClient client,char* myreadstring) {
  char c = 0; //carctère de lecture
  char tempo[255];
  bool currentLineIsBlank = true;
  int n = 1;
  int flag = 0;

  for (int k = 0; k < 50; k++) tempo[k] = 0;
  c = client.read(); //lecture du premier caractère;
  if (c != (-1)) {
    if ( c == 'G') {
      flag = 1;
      tempo[0] = c;
      while (c != '\n') {
        c = client.read(); //lecture du caractère
        delay(5);
        tempo[n] = c;
        delay(5);
        n++;
      }
    }
    else {
      tempo[0] = '¤';
      tempo[1] = '?';
      while (client.available() > 0) {
        c = client.read();
        if (c == '\n' && currentLineIsBlank) {
          while (c != '\r') { //doute
            c = client.read(); //lecture du caractère

            delay(5);
            if (c == (-1))
              break;
            tempo[n + 1] = c;
            delay(5);
            n++;
          }
        }
        else if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
  }

  if (!flag)n++;

  for (int j = 0; j < n; j++)
    myreadstring[j] = tempo[j] ;

}

byte Parser::parsing(char *str1) {
  byte i = 0, k ;
  int n = 0;
  char *pch; //pointeur de travail
  char *couple [16];  // tableau de 16 pointeurs
  char str[128];
  for (n = 0; n < 128; n++) str[n] = 0;
  for (n = 0; n < 16; n++) {
    couple[n] = NULL;
    nomparam[n] = NULL;
    valeurparam[n] = NULL;
  }

  strcpy(str, str1);
  // comptage de couple
  pch = strchr(str, '=');
  while (pch != NULL)
  {
    i++;
    pch = strchr(pch + 1, '=');
  }

  // parsing niveau 1
  pch = strtok (str, "?"); // nettoyage/////////////

  for (k = 0; k < i; k++)
  {
    if ( k != (i - 1))
      pch = strtok (NULL, "&");
    else
      pch = strtok (NULL, " ");
    couple[k] = pch;
  }


  // Parsing niveau 2
  for (k = 0; k < i; k++)
  {
    nomparam[k] = strtok (couple[k], "=");
    valeurparam[k] = strtok (NULL, "=");
  }

  return i; //renvoi le nombre de param
}

char* Parser::getName(byte drapeau) {
  return nomparam[drapeau];
}

char* Parser::getValue(char* str) {
  byte i = 0;

  while (i < 16) {
    if ( strcmp (str, nomparam[i]) == 0)
      return valeurparam[i];
    i++;
  }
  return "inconnu";
}

void Parser::free(char *myreadstring){
	for(byte l=0; l< 255;l++)myreadstring[l]="";
	for(byte k=0;k<16;k++)
	{
		nomparam[k]="";
		valeurparam[k]="";
	}
}