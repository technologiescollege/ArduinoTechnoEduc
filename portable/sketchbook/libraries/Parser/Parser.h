#ifndef Parser_h
#define Parser_h

#include <arduino.h>
#include <Ethernet.h>

class Parser
{
  public:
    void readStream(EthernetClient,char*);
    byte parsing(char*);
    char* getName(byte);
    char* getValue(char*);
	void free(char*);
    Parser(void);
	char myreadstring[255]={""};
  private:
    char *nomparam [16];
    char *valeurparam [16];
};





#endif
