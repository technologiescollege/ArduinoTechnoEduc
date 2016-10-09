#ifndef SERIAL_ALLBOT_H
#define SERIAL_ALLBOT_H

#include <arduino.h>
#include <ALLBOT.h>

typedef void (*command_callback)(int argc, int argv[]);

struct struct_callback {
	String name;
	command_callback cb;
};

class SerialALLBOT : public ALLBOT {
public:
	SerialALLBOT(int count);
	~SerialALLBOT();
	void update();
	void registerCommand(String name, command_callback cb);
private:
	void checkAndHandleCommand();
	void sendStatus();
	void sendCommandComplete();
	bool getDigitalPortStatus(int pin);
	int getAnalogPortStatus(int pin);
protected:
	struct_callback _callbacks[30];
	int _callBackCount;
};

#endif