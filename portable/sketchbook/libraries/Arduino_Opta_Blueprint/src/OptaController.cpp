/* -------------------------------------------------------------------------- */
/* FILENAME:    Controller.cpp
   AUTHOR:      Daniele Aimo (d.aimo@arduino.cc)
   DATE:        20230801
   DESCRIPTION: Implementation file for class Controller to be used on OPTA to
                control the I2C and the address assignment procedure to other
                expansions used with OPTA
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#include "OptaBlue.h"
#include "OptaBluePrintCfg.h"
#include "OptaBlueProtocol.h"
#include <cstdint>
#include <stdint.h>
#ifdef ARDUINO_OPTA
#include "AnalogExpansion.h"
#include "DigitalCommonCfg.h"
#include "DigitalExpansion.h"
#include "DigitalMechExpansion.h"
#include "DigitalStSolidExpansion.h"
#include "OptaController.h"

// #define DEBUG_COMM_TIMEOUT

#define INC_WITH_MAX(x, M) (x = (++x >= M) ? 0 : x)
#define DEC_WITH_MAX(x, M) (x = (--x < 0) ? (M - 1) : x)

class ExpType {

private:
  makeExpansion_f make;
  bool startUpFuncCalled;
  int type;
  std::string product;
  static int next_expansion_type;

public:
  startUp_f startUp;
  ExpType() : make(nullptr), startUpFuncCalled(false), type(-1), startUp(nullptr) {}
  void setType(int t) { type = t; }
  int setType() {
    type = next_expansion_type;
    next_expansion_type++;
    return type;
  }
  Expansion *allocateExpansion() {
    if (make != nullptr) {
      return make();
    }
    return nullptr;
  }
  int getType() { return type; }
  void setMake(makeExpansion_f f) { make = f; }
  void setProduct(std::string s) { product = s; }
  bool isProduct(std::string s) { return (product == s); }
  void setStart(startUp_f f) { startUp = f; }
  std::string getProduct() {return product;}
  void enableStartUpCallback() {
    startUpFuncCalled = false;
  }
  void callStartUp(Controller *p) {
    if(startUpFuncCalled == false) {
      startUp(p);
      startUpFuncCalled = true;
    }
  }
};

int ExpType::next_expansion_type = EXPANSION_CUSTOM + 20;

// namespace Opta {

/* -------------------------------------------------------------------------- */
/* Controller constructor (performs some initialization)                      */
/* -------------------------------------------------------------------------- */

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* CONSTRUCTOR */
Controller::Controller()
    : rx_buffer{0}, tx_buffer{0}, rx_num(0),
      tmp_address(OPTA_CONTROLLER_FIRST_TEMPORARY_ADDRESS), tmp_num_of_exp(0),
      address(OPTA_CONTROLLER_FIRST_AVAILABLE_ADDRESS), num_of_exp(0),
      failed_i2c_comm(nullptr) {
  init_exp_type_list();      
  for (int i = 0; i < OPTA_CONTROLLER_MAX_EXPANSION_NUM; i++) {
    expansions[i] = nullptr;
  }
}

void Controller::init_exp_type_list() {
    ExpType et;

    et.setType((uint8_t)EXPANSION_DIGITAL_INVALID);
    et.setMake(DigitalExpansion::makeExpansion);
    et.setProduct(DigitalExpansion::getProduct());
    et.setStart(DigitalExpansion::startUp);

    exp_type_list.push_back(et);

    et.setType((uint8_t)EXPANSION_OPTA_DIGITAL_MEC);
    et.setMake(DigitalMechExpansion::makeExpansion);
    et.setProduct(DigitalMechExpansion::getProduct());
    et.setStart(DigitalExpansion::startUp);

    exp_type_list.push_back(et);;

    et.setType((uint8_t)EXPANSION_OPTA_DIGITAL_STS);
    et.setMake(DigitalStSolidExpansion::makeExpansion);
    et.setProduct(DigitalStSolidExpansion::getProduct());
    et.setStart(DigitalExpansion::startUp);

    exp_type_list.push_back(et);

    et.setType((uint8_t)EXPANSION_OPTA_ANALOG);
    et.setMake(AnalogExpansion::makeExpansion);
    et.setProduct(AnalogExpansion::getProduct());
    et.setStart(AnalogExpansion::startUp);

    exp_type_list.push_back(et);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

int Controller::getExpansionType(std::string pr) {
  int rv = -1;
  for (unsigned int i = 0; i < exp_type_list.size(); i++) {
    if (exp_type_list[i].isProduct(pr)) {
      rv = exp_type_list[i].getType();
      break;
    }
  }
  return rv;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */


int Controller::registerCustomExpansion(std::string pr, makeExpansion_f f,
                                        startUp_f su) {
  int rv = getExpansionType(pr);
  /* product not registered... add it */
  if (rv == -1) {
    ExpType et;
    et.setMake(f);
    et.setProduct(pr);
    et.setStart(su);
    /* automatically increment the type so that is ready for another 
       call*/
    rv = et.setType();
    exp_type_list.push_back(et);
  }

  /* looking for expansions registered after begin() (slower) */
  assign_custom_type_and_call_start_up();
  return rv;
}


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* DESTRUCTOR */
Controller::~Controller() {
  for (int i = 0; i < OPTA_CONTROLLER_MAX_EXPANSION_NUM; i++) {
    if (expansions[i] != nullptr) {
      delete expansions[i];
      expansions[i] = nullptr;
    }
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void Controller::updateRegs(Expansion &exp) {
  if (exp.getIndex() < OPTA_CONTROLLER_MAX_EXPANSION_NUM) {
    if (expansions[exp.getIndex()] != nullptr &&
        exp.getI2CAddress() == exp_add[exp.getIndex()] &&
        exp.getType() == exp_type[exp.getIndex()]) {
      expansions[exp.getIndex()]->updateRegs(exp);
    }
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void Controller::beginOdDefaults(uint8_t device, bool d[OPTA_DIGITAL_OUT_NUM],
                                 uint16_t timeout) {
  DigitalExpansion::setDefault(*this, device,
                               DigitalExpansion::calcDefault(d[0], d[1], d[2],
                                                             d[3], d[4], d[5],
                                                             d[6], d[7]),
                               timeout);
}
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint8_t Controller::send(int add, int device, unsigned int type, int n, int r) {
  if (device < OPTA_CONTROLLER_MAX_EXPANSION_NUM && type != EXPANSION_NOT_VALID) {
    if (type == exp_type[device] && add == exp_add[device]) {
      if (n > 0) {

#ifdef DEBUG_CONTROLLER_I2C_MSG
        Serial.print("CTRL TX: ");
        for (int i = 0; i < n; i++) {
          Serial.print(tx_buffer[i], HEX);
          Serial.print(" ");
        }
        Serial.println();
#endif
        _send(add, n, r);

        if (r > 0) {
          if (wait_for_device_answer(device, r, OPTA_CONTROLLER_WAIT_REQUEST_TIMEOUT)) {
            return SEND_RESULT_OK;
          }
          return SEND_RESULT_COMM_TIMEOUT;
        }
        return SEND_RESULT_OK;
      }
      return SEND_RESULT_NO_DATA_TO_TRANSMIT;
    }
    return SEND_RESULT_WRONG_EXPANSION_ATTRIBUTES;
  }

  return SEND_RESULT_WRONG_EXPANSION_INDEX;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void Controller::setTx(uint8_t value, uint8_t pos) {
  if (pos < OPTA_I2C_BUFFER_DIM) {
    tx_buffer[pos] = value;
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint8_t Controller::getRx(uint8_t pos) {
  if (pos < OPTA_I2C_BUFFER_DIM) {
    return rx_buffer[pos];
  }
  return 0;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

Expansion *Controller::getExpansionPtr(int device) {
  
  if (device < OPTA_CONTROLLER_MAX_EXPANSION_NUM) {
    if (expansions[device] != nullptr) {
      return expansions[device];
    } else {
      for (unsigned int i = 0; i < exp_type_list.size(); i++) {
        if (exp_type[device] == exp_type_list[i].getType()) {
          expansions[device] = exp_type_list[i].allocateExpansion();
          break;
        }
      }
      if (expansions[device] != nullptr) {
        expansions[device]->setIndex(device);
        expansions[device]->setType(exp_type[device]);
        expansions[device]->setI2CAddress(exp_add[device]);
        expansions[device]->setCtrl(this);
      }
    }
    return expansions[device];
  }
  return nullptr;
}

Expansion dummyExp;

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

Expansion &Controller::getExpansion(int device) {
  if (device < OPTA_CONTROLLER_MAX_EXPANSION_NUM) {
    Expansion *exp = getExpansionPtr(device);
    if (exp != nullptr) {
      return *exp;
    }
  }
  dummyExp.setController(this);
  return dummyExp;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint8_t Controller::getExpansionType(uint8_t i) {
  if (i < num_of_exp) {
    return exp_type[i];
  }
  return EXPANSION_NOT_VALID;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint8_t Controller::getExpansionNum() { return num_of_exp; }

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint8_t Controller::getExpansionI2Caddress(uint8_t i) {
  if (i < num_of_exp) {
    return exp_add[i];
  }
  return OPTA_DEFAULT_SLAVE_I2C_ADDRESS - 1;
}

/* ------------------------------------------------------------------------ */
/* Controller begin */
/* ------------------------------------------------------------------------ */
void Controller::begin() {

#ifdef DEBUG_SERIAL
  pinMode(LED_RESET, OUTPUT);
  Serial.begin(115200);
  delay(2000);
  Serial.println("[LOG]: Controller begin");
#endif
  /* initalize the controller detect pin as a input pullup */
  pinMode(OPTA_CONTROLLER_DETECT_PIN, INPUT_PULLUP);
  /* initialize the controller as I2C MASTER */
  Wire.begin();
  Wire.setClock(400000);

  _send(OPTA_CONTROLLER_FIRST_AVAILABLE_ADDRESS, msg_opta_reset(), 0);
  delay(OPTA_CONTROLLER_SETUP_INIT_DELAY);
  checkForExpansions();
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

// REVISED
int Controller::analogReadOpta(uint8_t device, uint8_t pin,
                               bool update /*= true*/) {
  DigitalExpansion *ptr = (DigitalExpansion *)getExpansionPtr(device);
  if (ptr != nullptr) {
    return ptr->analogRead(pin, update);
  }
  return -1;
  // TODO (MAYBE): add analog
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

// REVISED
bool Controller::getFwVersion(uint8_t device, uint8_t &major, uint8_t &minor,
                              uint8_t &release) {
  Expansion *ptr = getExpansionPtr(device);
  if (ptr != nullptr) {
    return ptr->getFwVersion(major, minor, release);
  }
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

// REVISED
PinStatus Controller::digitalReadOpta(uint8_t device, uint8_t pin,
                                      bool update) {
  DigitalExpansion *ptr = (DigitalExpansion *)getExpansionPtr(device);
  if (ptr != nullptr) {
    return ptr->digitalRead(pin, update);
  }

  // TODO (MAYBE): add analog

  return LOW;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

bool Controller::rebootExpansion(uint8_t device) {
  if (device < OPTA_CONTROLLER_MAX_EXPANSION_NUM) {
    _send(exp_add[device], msg_opta_reboot(), getExpectedAnsLen(ANS_LEN_REBOOT));

    if (wait_for_device_answer(OPTA_BLUE_UNDEFINED_DEVICE_NUMBER, 
                               getExpectedAnsLen(ANS_LEN_REBOOT), 
                               OPTA_CONTROLLER_WAIT_REQUEST_TIMEOUT)) {
      if (parse_opta_reboot()) {
        delay(OPTA_CONTROLLER_DELAY_AFTER_REBOOT);
        return true;
      }
    } else {
#ifdef DEBUG_COMM_TIMEOUT
      Serial.println("ERR C");
      for (int i = 0; i < ANS_REBOOT_LEN_CRC; i++) {
        Serial.print(rx_buffer[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
#endif
    }
  }
  delay(OPTA_CONTROLLER_DELAY_AFTER_REBOOT);
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

// REVISED
void Controller::digitalWriteOpta(uint8_t device, uint8_t pin, PinStatus st,
                                  bool update) {
  DigitalExpansion *ptr = (DigitalExpansion *)getExpansionPtr(device);
  if (ptr != nullptr) {
    return ptr->digitalWrite(pin, st, update);
  }

  // TODO (MAYBE): add analog
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

// REVISED
bool Controller::updateDigitalsOut(uint8_t device) {
  DigitalExpansion *ptr = (DigitalExpansion *)getExpansionPtr(device);
  if (ptr != nullptr) {
    ptr->updateDigitalOutputs();
    return true;
  }
  return false;
  // TODO (MAYBE): add analog
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

// REVISED
void Controller::updateDigitalsIn(uint8_t device) {
  DigitalExpansion *ptr = (DigitalExpansion *)getExpansionPtr(device);
  if (ptr != nullptr) {
    return ptr->updateDigitalInputs();
  }
  // TODO (MAYBE): add analog
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

// REVISED
void Controller::updateDigitals(uint8_t device) {
  updateDigitalsIn(device);
  updateDigitalsOut(device);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

// REVISED
void Controller::updateAnalogsIn(uint8_t device) {
  DigitalExpansion *ptr = (DigitalExpansion *)getExpansionPtr(device);
  if (ptr != nullptr) {
    return ptr->updateAnalogInputs();
  }
  // TODO (MAYBE): add analog
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

// REVISED
void Controller::updateAnalogs(uint8_t device) { updateAnalogsIn(device); }

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* function to be called periodically to update all the data to and from
   expansions */
void Controller::update() {

#if defined DEBUG_SERIAL
  static unsigned long int start_debug = millis();
  static bool flag = true;
  if (millis() - start_debug > 500) {
    start_debug = millis();
    if (flag) {
      digitalWrite(LED_RESET, HIGH);
      flag = false;
    } else {
      digitalWrite(LED_RESET, LOW);
      flag = true;
    }
  }
#endif

  static unsigned long int start = millis();
  if (millis() - start > OPTA_CONTROLLER_UPDATE_RATE) {
    start = millis();

    checkForExpansions();
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* this function checks the status of the detect pin and debounce it for LOW
   status i.e. it returns true if the PIN is low and remains LOW for a
   certain time */
bool Controller::is_detect_low() {
  PinStatus detect_st = digitalRead(OPTA_CONTROLLER_DETECT_PIN);

  if (detect_st == LOW) {
    int num = 0;

    while (detect_st == LOW && num < OPTA_CONTROLLER_DEBOUNCE_LOW_NUMBER) {
      detect_st = digitalRead(OPTA_CONTROLLER_DETECT_PIN);
      if (detect_st == HIGH) {
        break;
      }
      delay(OPTA_CONTROLLER_DEBOUNCE_LOW_TIME);
      num++;
    }
  }

  return (detect_st == LOW);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* this function checks the status of the detect pin and debounce it for LOW
   status i.e. it returns true if the PIN is low and remains LOW for a
   certain time */
bool Controller::is_detect_high() {
  PinStatus detect_st = digitalRead(OPTA_CONTROLLER_DETECT_PIN);

  if (detect_st == HIGH) {
    int num = 0;

    while (detect_st == HIGH && num < OPTA_CONTROLLER_DEBOUNCE_UP_NUMBER) {
      detect_st = digitalRead(OPTA_CONTROLLER_DETECT_PIN);
      if (detect_st == LOW) {
        break;
      }
      delay(OPTA_CONTROLLER_DEBOUNCE_UP_TIME);
      num++;
    }
  }

  return (detect_st == HIGH);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void Controller::assign_custom_type_and_call_start_up() {
   /* safe to call this in the registerCustomExpansion function because:
      - if the Controller.begin() function has been called then num_of_exp is
        different from 0
      - otherwise is 0 end the for loop does not work */
   for (int i = 0; i < num_of_exp; i++) {
      /* assign to expansion in position i a custom expansion type (if custom 
         expansion has been registered */
      if (exp_type[i] >= OPTA_CONTROLLER_CUSTOM_MIN_TYPE || exp_type[i] == EXPANSION_NOT_VALID) {
        _send(exp_add[i], msg_get_product_type(), getExpectedAnsLen(ANS_LEN_GET_PRODUCT_TYPE));
        if (wait_for_device_answer(OPTA_BLUE_UNDEFINED_DEVICE_NUMBER,
                                   getExpectedAnsLen(ANS_LEN_GET_PRODUCT_TYPE), OPTA_CONTROLLER_WAIT_REQUEST_TIMEOUT)) {
          /* return expansion not valid if product is not found*/
          exp_type[i] = parse_get_product();
          #if defined DEBUG_SERIAL && defined DEBUG_ASSIGN_ADDRESS_CONTROLLER
          Serial.println("EXPANSION " + String(i) + " is custom with type " + exp_type[i]);
          #endif
        }
      }

      /* call start up functions */
      if(exp_type[i] != EXPANSION_NOT_VALID) {
         for(unsigned int k = 0; k < exp_type_list.size(); k++) {
            if(exp_type[i] == exp_type_list[k].getType()) {
               /* start can be called only once after the assign address process
                  or after a registerCustomExpansion so it contains a flag 
                  to avoid multiple calls */
               exp_type_list[k].callStartUp(this);
            }
         }
      }
   }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* function that performs the actual assign address process
   it has to be called ciclycally in the loop to support hot-plug expansion
   attachment */
void Controller::checkForExpansions() {
  if (digitalRead(OPTA_CONTROLLER_DETECT_PIN) == HIGH) {
    return;
  }

  bool enter_while = is_detect_low();
  /* PAY ATTENTION:
     The condition to ENTER into the while loop is that the DETECT pin is
     LOW BUT the condition to exit is that the DETECT pin is HIGH this exit
     condition is performed using a break statemente within the while loop
     This way I am sure to catch the transition LOW --> HIGH in a correct
     way the enter_while is no more updated */

  /* * FIX Controller not re-assigning address when reset *
     the tmp_exp_add is filled like a circular queue so that in any number
     of temporary address can be used  but only the last 10 value are stored
     - this initialization is useful to exit as soon as we meet 0*/
  
  /* INITIALIZING ALL VARIABLE USED THEN IN THE FOLLOWING while loop WHERE
     ADDRESS are assigned */
  if (enter_while) {
    
    for (int i = 0; i < OPTA_CONTROLLER_MAX_EXPANSION_NUM; i++) {
      tmp_exp_add[i] = 0;
      tmp_exp_type[i] = 0;
    }
    num_of_exp = 0;
    tmp_num_of_exp = 0;
    /* the tmp_address is incremented automatically when an answer for the
       request get address and type is correctly received */
    tmp_address = OPTA_CONTROLLER_FIRST_TEMPORARY_ADDRESS;
    
    /* with this for loop a reset command is sent to every device (with temporary
       or final addresses )*/
    for(int i = OPTA_CONTROLLER_FIRST_AVAILABLE_ADDRESS; 
        i < OPTA_CONTROLLER_FIRST_AVAILABLE_ADDRESS + 2*OPTA_CONTROLLER_MAX_EXPANSION_NUM; 
        i++) {
      _send(i, msg_opta_reset(), 0);
    }

    delay(OPTA_CONTROLLER_SETUP_INIT_DELAY);
  }
  
  /* #################################
   * TEMPORARY ADDRESS ASSIGNMENT LOOP
   * ################################# */

  uint8_t temporary_address_sent = tmp_address;
  
  while (enter_while) {

#if defined DEBUG_SERIAL && defined DEBUG_ASSIGN_ADDRESS_CONTROLLER
    Serial.println("[LOG]:  - DETECT pin is LOW (expansions without address)");
    Serial.println("        - Sending SET temp address message 0x" + String(tmp_address,HEX));
#endif
    
    /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
     * 1. SENDING new temporary address to the device using DEFAULT address 
     *    (no answer is expected)
     * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
    temporary_address_sent = tmp_address;
    _send(OPTA_DEFAULT_SLAVE_I2C_ADDRESS, msg_set_address(tmp_address), 0);
    delay(OPTA_CONTROLLER_DELAY_AFTER_SET_ADDRESS);
    
    uint8_t attempts = 3;
    /* exit with break upon correct answer received */
    while(attempts > 0) {
      #if defined DEBUG_SERIAL && defined DEBUG_ASSIGN_ADDRESS_CONTROLLER
      Serial.println("        - Sending GET address message");
      #endif

      /* 2. SENDING request: get address and type to the device to ensure 
            proper address has been accepted  */

      _send(tmp_address, msg_get_address_and_type(), getExpectedAnsLen(ANS_LEN_ADDRESS_AND_TYPE));

      #if defined DEBUG_SERIAL && defined DEBUG_ASSIGN_ADDRESS_CONTROLLER
      Serial.print("        - Receiving answer from device: ");
      #endif

      /*  3. RECEIVING ANSWER */

      if (wait_for_device_answer(OPTA_BLUE_UNDEFINED_DEVICE_NUMBER, getExpectedAnsLen(ANS_LEN_ADDRESS_AND_TYPE), OPTA_CONTROLLER_TIMEOUT_FOR_SETUP_MESSAGE)) {
        /* when the address is correcly received as answer to the previous the
          parse_address_and_type function increase in a circular way
          tmp_num_of_exp so that tmp_exp_add array is filled in a circular way */
        if(parse_address_and_type(tmp_address)) {
          #ifdef USE_CONFIRM_RX_MESSAGE
          delay(5);
          _send(tmp_address, msg_confirm_rx_address(),0);
          #endif
          delay(10);
          #if defined DEBUG_SERIAL && defined DEBUG_ASSIGN_ADDRESS_CONTROLLER
          Serial.println("        GOT IT! ");
          #endif
          break;
        } 
        else {
          delay(20);
        }
       
      }
      else {
        #if defined DEBUG_SERIAL && defined DEBUG_ASSIGN_ADDRESS_CONTROLLER
        Serial.println("          TIMEOUT ");
        #endif 
        delay(20);
      }
      attempts--;
    }

    /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
     * 4. VERIFY if the detect is gone UP --> if it is high no more device
     *    without address are present and we can exit the loop
     * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

    if (is_detect_high()) {
      if(temporary_address_sent == (tmp_address - 1)) {
        break;
      }
    }
  } //while (enter_while)

/* printing found devices with temporary addresses */
#if defined DEBUG_SERIAL && defined DEBUG_ASSIGN_ADDRESS_CONTROLLER
    Serial.println("[LOG]:  - TRANSITION of detect from LOW to HIGH");
    Serial.print("TEMPORARY Number of expansions found ");
    Serial.println(tmp_num_of_exp);
    for (int i = 0; i < OPTA_CONTROLLER_MAX_EXPANSION_NUM; i++) {
      Serial.print("Expansion index ");
      Serial.print(i);
      Serial.print(" address ");
      Serial.print(tmp_exp_add[i], HEX);
      Serial.print(" type ");
      Serial.println(tmp_exp_type[i], HEX);
    }
    delay(10000);

#endif
  /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
     * At this point all expansions have been found and have a temporary
     * address
     * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
  
  /* initializing variable */
  address = OPTA_CONTROLLER_FIRST_AVAILABLE_ADDRESS;
  num_of_exp = 0;

  /* * FIX Controller not re-assigning address when reset *
     Since now the tmp_exp_add array is filled in circular way we need to
     find the index of the array with the maximum value (this is always
     the address of the expansion closest to the controller and the
     re-assignment of address must begin from this expansion that will
     take the first address and the first position in the array of
     expansion) */

  int initial_value = -1;
  uint8_t max_value = 0x00;
  for (int i = 0; i < OPTA_CONTROLLER_MAX_EXPANSION_NUM; i++) {
    if (tmp_exp_add[i] > max_value) {
      max_value = tmp_exp_add[i];
      initial_value = i;
    }
  }

  /* * FIX Controller not re-assigning address when reset *
     tmp_num_of_exp will be decreased in circular way (when it becomes
     lower than 0 it will be assigned to OPTA_CONTROLLER_MAX_EXPANSION_NUM
     - 1 using DEC_WITH_MAX macro that does that) */
  tmp_num_of_exp = initial_value;

  /* * FIX Controller not re-assigning address when reset *
     In case of unwanted expansion reset (btw this is fixed also in the
     expansion so that is a case should never happen) it can happen that
     some temporary addresses is not working properly, this is fine...
     just try and then skip the address, the attemps is used to to so
      */
  int attempts = 0;

  bool remain_in_while_loop = true;
  do {
    #if defined DEBUG_SERIAL && defined DEBUG_ASSIGN_ADDRESS_CONTROLLER
    Serial.print("Sending to address 0x" + String(tmp_exp_add[tmp_num_of_exp], HEX) );
    Serial.println(" new address 0x" + String(address,HEX) );
    #endif
    _send(tmp_exp_add[tmp_num_of_exp], msg_set_address(address), 0);

    delay(OPTA_CONTROLLER_DELAY_AFTER_SET_ADDRESS);

    _send(address, msg_get_address_and_type(), getExpectedAnsLen(ANS_LEN_ADDRESS_AND_TYPE));

      /* * FIX Controller not re-assigning address when reset *
      when the address is correcly received as answer to the previous the
      parse_address_and_type function decrease in a circular way
      (see DEC_WITH_MAS tmp_num_of_exp)
      after 3 failed attemps the address is skipped and then the
      tmp_num_of_exp is decreased in the else branch */

      if (wait_for_device_answer(OPTA_BLUE_UNDEFINED_DEVICE_NUMBER, getExpectedAnsLen(ANS_LEN_ADDRESS_AND_TYPE), OPTA_CONTROLLER_TIMEOUT_FOR_SETUP_MESSAGE)) {
        if (parse_address_and_type(address)) {
          remain_in_while_loop = (tmp_num_of_exp != initial_value);
        }
        attempts = 0;
      } else {
        attempts++;
        if (attempts > 2) {
          DEC_WITH_MAX(tmp_num_of_exp, OPTA_CONTROLLER_MAX_EXPANSION_NUM);
          
          remain_in_while_loop = (tmp_num_of_exp != initial_value);
        } else {
          remain_in_while_loop = true;
        }
      }
    } while (tmp_exp_add[tmp_num_of_exp] != 0 && remain_in_while_loop);

    /* give some time to analog to reset Analog Devices */
    delay(OPTA_CONTROLLER_DELAY_EXPANSION_RESET);

    /* reset the start up callback flag so that the callback can be called again*/
    for(unsigned int i = 0; i < exp_type_list.size(); i++) {
      exp_type_list[i].enableStartUpCallback();
    }    
    assign_custom_type_and_call_start_up();

#if defined DEBUG_SERIAL && defined DEBUG_ASSIGN_ADDRESS_CONTROLLER
    Serial.print("FINAL Number of expansions found ");
    Serial.println(num_of_exp);
    for (int i = 0; i < num_of_exp; i++) {
      Serial.print("Expansion index ");
      Serial.print(i);
      Serial.print(" address ");
      Serial.println(exp_add[i], HEX);
    }

    delay(1000);
#endif

    /* delete expansions (new expansions can be different from old ones) */
    for (int i = 0; i < OPTA_CONTROLLER_MAX_EXPANSION_NUM; i++) {
      if (expansions[i] != nullptr) {
        delete expansions[i];
        expansions[i] = nullptr;
      }
    }    

#if defined DEBUG_SERIAL && defined DEBUG_ASSIGN_ADDRESS_CONTROLLER
    Serial.println();
    Serial.println("[LOG]: ***** REASSING ADDRESSES FINISHED ***** ");
    Serial.print("number of expansions found ");
    Serial.println(num_of_exp);
    for (int i = 0; i < num_of_exp; i++) {
      Serial.print(i + 1);
      Serial.print(") address ");
      Serial.print(exp_add[i]);
      Serial.print(", type ");
      if (exp_type[i] == EXPANSION_NOT_VALID) {
        Serial.println("EXPANSION_NOT_VALID");
      } else if (exp_type[i] == EXPANSION_OPTA_DIGITAL_MEC ||
                 exp_type[i] == EXPANSION_OPTA_DIGITAL_STS) {
        Serial.println("EXPANSION_OPTA_DIGITAL");
      } else if(exp_type[i] == EXPANSION_OPTA_ANALOG )  {
        Serial.println("EXPANSION_OPTA_ANALOG");
      }
      else {
        Serial.println("expansion type custom " + String(exp_type[i]));
      }
    }
#endif
   
}

void Controller::setFailedCommCb(CommErr_f f) { failed_i2c_comm = f; }

/* #################################################################### */
/* -------------- message preparation functions ---------------------- */
/* #################################################################### */

uint8_t Controller::msg_opta_reboot() {
  tx_buffer[REBOOT_1_POS] = REBOOT_1_VALUE;
  tx_buffer[REBOOT_2_POS] = REBOOT_2_VALUE;
  return prepareSetMsg(tx_buffer, ARG_REBOOT, LEN_REBOOT);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* prepare the message in the tx buffer to get opta digital analog in */
uint8_t Controller::msg_opta_reset() {
  tx_buffer[BP_PAYLOAD_START_POS] = CONTROLLER_RESET_CODE;
  return prepareSetMsg(tx_buffer, ARG_CONTROLLER_RESET, LEN_CONTROLLER_RESET);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* prepare the message in the tx buffer to set an address to an expansion */
uint8_t Controller::msg_set_address(uint8_t add) {
  tx_buffer[BP_PAYLOAD_START_POS] = add; /* the value to be set */
  return prepareSetMsg(tx_buffer, ARG_ADDRESS, LEN_ADDRESS);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
 #ifdef USE_CONFIRM_RX_MESSAGE
uint8_t Controller::msg_confirm_rx_address() {
  tx_buffer[CONFIRM_ADDRESS_FIRST_POS] = CONFIRM_ADDRESS_FIRST_VALUE; 
  tx_buffer[CONFIRM_ADDRESS_SECOND_POS] = CONFIRM_ADDRESS_SECOND_VALUE; 

  
  return prepareSetMsg(tx_buffer, ARG_CONFIRM_ADDRESS_RX, LEN_CONFIRM_ADDRESS_RX);

}
#endif


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

uint8_t Controller::msg_get_product_type() {
  return prepareGetMsg(tx_buffer, ARG_GET_PRODUCT_TYPE, LEN_GET_PRODUCT_TYPE);
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* prepare the message in the tx buffer to get address and type from
 * expansion*/
uint8_t Controller::msg_get_address_and_type() {
  return prepareGetMsg(tx_buffer, ARG_ADDRESS_AND_TYPE, LEN_ADDRESS_AND_TYPE);
}

/* #######################################################################
 */
/* ---------------------- parse message functions ---------------------- */
/* #######################################################################
 */

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void Controller::resetRxBuffer() { memset(rx_buffer, 0, OPTA_I2C_BUFFER_DIM); }

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

int Controller::parse_get_product() {
  int rv = EXPANSION_NOT_VALID;

  if (checkAnsGetReceived(rx_buffer, 
                          ANS_ARG_GET_PRODUCT_TYPE, 
                          ANS_LEN_GET_PRODUCT_TYPE)) {
    
    std::string pr((const char *)(rx_buffer + ANS_GET_PRODUCT_SIZE_POS + 1),
                   rx_buffer[ANS_GET_PRODUCT_SIZE_POS]);
    
    rv = getExpansionType(pr);
  }
  return (rv == -1) ? EXPANSION_NOT_VALID : rv;
}

bool Controller::parse_opta_reboot() {
  if (checkAnsSetReceived(rx_buffer, ANS_ARG_REBOOT, ANS_LEN_REBOOT)) {

    if (rx_buffer[ANS_REBOOT_CODE_POS] == ANS_REBOOT_CODE) {
      return true;
    } else {
#ifdef DEBUG_COMM_TIMEOUT
      Serial.println("ERR A");
      for (int i = 0; i < ANS_REBOOT_LEN_CRC; i++) {
        Serial.print(rx_buffer[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
#endif
    }
  } else {
#ifdef DEBUG_COMM_TIMEOUT
    Serial.println("ERR B");
    for (int i = 0; i < ANS_REBOOT_LEN_CRC; i++) {
      Serial.print(rx_buffer[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
#endif
  }
  resetRxBuffer();
  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* parse the answer to the message get address and type from the slave */
bool Controller::parse_address_and_type(int slave_address) {
  if (checkAnsGetReceived(rx_buffer, ANS_ARG_ADDRESS_AND_TYPE,
                          ANS_LEN_ADDRESS_AND_TYPE)) {
#if defined DEBUG_SERIAL && defined DEBUG_MSG_PARSE_ADDRESS_AND_TYPE
    Serial.println("        - Received answer to get address request");
#endif

    if (rx_buffer[BP_PAYLOAD_START_POS] == slave_address) {
      if (slave_address == tmp_address) {
#if defined DEBUG_SERIAL && defined DEBUG_MSG_PARSE_ADDRESS_AND_TYPE
        Serial.print("        - Received address is CORRECT: 0x");
        Serial.println(rx_buffer[BP_PAYLOAD_START_POS], HEX);
#endif
        tmp_address++;

        tmp_exp_add[tmp_num_of_exp] = slave_address;
        tmp_exp_type[tmp_num_of_exp] = rx_buffer[BP_PAYLOAD_START_POS + 1];

#if defined DEBUG_SERIAL && defined DEBUG_MSG_PARSE_ADDRESS_AND_TYPE
        Serial.print("        - Type of device attached: ");
        if (tmp_exp_type[tmp_num_of_exp] == EXPANSION_NOT_VALID) {
          Serial.println("EXPANSION_NOT_VALID");
        } else if (tmp_exp_type[tmp_num_of_exp] == EXPANSION_OPTA_DIGITAL_MEC) {
          Serial.println("EXPANSION_OPTA_DIGITAL_MEC");
        } else if (tmp_exp_type[tmp_num_of_exp] == EXPANSION_OPTA_DIGITAL_STS) {
          Serial.println("EXPANSION_OPTA_DIGITAL_STS");
        } else if (tmp_exp_type[tmp_num_of_exp] == EXPANSION_OPTA_ANALOG) {
          Serial.println("EXPANSION_OPTA_ANALOG");
        }else {
          Serial.println("expansion code unknown!!!!!!");
        }
#endif

        INC_WITH_MAX(tmp_num_of_exp, OPTA_CONTROLLER_MAX_EXPANSION_NUM);
      } else if (slave_address == address) {
        if (num_of_exp < OPTA_CONTROLLER_MAX_EXPANSION_NUM) {
          exp_add[num_of_exp] = slave_address;
          exp_type[num_of_exp] = rx_buffer[BP_PAYLOAD_START_POS + 1];
          num_of_exp++;
        }
        DEC_WITH_MAX(tmp_num_of_exp, OPTA_CONTROLLER_MAX_EXPANSION_NUM);
        address++;
      }
    } else {
#if defined DEBUG_SERIAL && defined DEBUG_MSG_PARSE_ADDRESS_AND_TYPE
      Serial.print("        - Received address is WRONG: 0x");
      Serial.println(rx_buffer[BP_PAYLOAD_START_POS], HEX);
#endif
    }
    return true;
  }
  return false;
}

/* ##################################################################### */
/* ---------------------- send receive message functions --------------- */
/* ##################################################################### */

/* send to address add n bytes from tx_buffer
   if r is > 0 then it issues a request from the slave for r bytes */
void Controller::_send(int add, int n, int r) {
#if defined DEBUG_SERIAL && defined DEBUG_TX_CONTROLLER_ENABLE
  Serial.print("- TX to device 0x");
  Serial.print(add, HEX);
#endif

  Wire.beginTransmission(add);

/* Send */
#if defined DEBUG_SERIAL && defined DEBUG_TX_CONTROLLER_ENABLE
  Serial.print(" bytes: ");
#endif

  for (int i = 0; i < n && i < OPTA_I2C_BUFFER_DIM; i++) {

#if defined DEBUG_SERIAL && defined DEBUG_TX_CONTROLLER_ENABLE
    Serial.print("0x");
    Serial.print(tx_buffer[i], HEX);
    Serial.print(" ");
#endif

    Wire.write(tx_buffer[i]);
  }

  Wire.endTransmission();

#if defined DEBUG_SERIAL && defined DEBUG_TX_CONTROLLER_ENABLE
  Serial.println();
#endif

  /* Request */
  if (r > 0) {
#if defined DEBUG_SERIAL && defined DEBUG_TX_CONTROLLER_ENABLE
    Serial.print("-> REQUESTED ");
    Serial.print(r);
    Serial.println(" bytes");
#endif
    Wire.requestFrom(add, r);
  } else {
#if defined DEBUG_SERIAL && defined DEBUG_TX_CONTROLLER_ENABLE
    Serial.println();
#endif
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* when an answer is requested from expansion this function wait for an
   answer and put it into the rx buffer
   return true if an answer is available */
bool Controller::wait_for_device_answer(uint8_t device, uint8_t wait_for, uint16_t timeout) {
  uint8_t rx = 0;
  unsigned long int start = millis();
  while (millis() - start < timeout &&
         rx < wait_for) {
    while (Wire.available()) {
      uint8_t rec = Wire.read();
      if (rx < OPTA_I2C_BUFFER_DIM) {
        rx_buffer[rx++] = rec;
      }
    }
  }

  rx_num = rx;

  if (rx_num == wait_for) {
    return true;
  } else {
#ifdef DEBUG_COMM_TIMEOUT
    Serial.println("COMMUNICATION TIMEOUT");
    Serial.println("wait_for " + String(wait_for));
    Serial.println("rx_num " + String(rx_num));
#endif
  }

  /* call callback function if nothing has been received */
  if (failed_i2c_comm != nullptr &&
      device < OPTA_BLUE_UNDEFINED_DEVICE_NUMBER) {
    failed_i2c_comm(device, tx_buffer[BP_ARG_POS]);
  }

  return false;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void Controller::getFlashData(uint8_t device, uint8_t *buf, uint8_t &dbuf,
                              uint16_t &add) {
  if (device < OPTA_CONTROLLER_MAX_EXPANSION_NUM) {
    if (exp_type[device] == EXPANSION_OPTA_DIGITAL_MEC ||
        exp_type[device] == EXPANSION_OPTA_DIGITAL_STS ||
        exp_type[device] == EXPANSION_DIGITAL_INVALID ||
        exp_type[device] == EXPANSION_OPTA_ANALOG) {
      Expansion *ptr = (Expansion *)getExpansionPtr(device);
      if (ptr != nullptr) {
        return ptr->getFlashData(buf, dbuf, add);
      }
    }
  }
}

void Controller::setProductionData(uint8_t device, uint8_t *data,
                                   uint8_t dlen) {
  if (device < OPTA_CONTROLLER_MAX_EXPANSION_NUM) {
    if (exp_type[device] == EXPANSION_OPTA_DIGITAL_MEC ||
        exp_type[device] == EXPANSION_OPTA_DIGITAL_STS ||
        exp_type[device] == EXPANSION_DIGITAL_INVALID) {
      DigitalExpansion *ptr = (DigitalExpansion *)getExpansionPtr(device);
      if (ptr != nullptr) {
        return ptr->setProductData(data, dlen);
      }
    } else if (exp_type[device] == EXPANSION_OPTA_ANALOG) {
      AnalogExpansion *ptr = (AnalogExpansion *)getExpansionPtr(device);
      if (ptr != nullptr) {
        return ptr->setProductData(data, dlen);
      }
    }
  }
}
void Controller::setOdMechanical(uint8_t device) {
  if (device < OPTA_CONTROLLER_MAX_EXPANSION_NUM) {
    if (exp_type[device] == EXPANSION_OPTA_DIGITAL_MEC ||
        exp_type[device] == EXPANSION_OPTA_DIGITAL_STS ||
        exp_type[device] == EXPANSION_DIGITAL_INVALID) {
      DigitalExpansion *ptr = (DigitalExpansion *)getExpansionPtr(device);
      if (ptr != nullptr) {
        return ptr->setIsMechanical();
      }
    }
  }
}
void Controller::setOdStateSolid(uint8_t device) {
  if (device < OPTA_CONTROLLER_MAX_EXPANSION_NUM) {
    if (exp_type[device] == EXPANSION_OPTA_DIGITAL_MEC ||
        exp_type[device] == EXPANSION_OPTA_DIGITAL_STS ||
        exp_type[device] == EXPANSION_DIGITAL_INVALID) {
      DigitalExpansion *ptr = (DigitalExpansion *)getExpansionPtr(device);
      if (ptr != nullptr) {
        return ptr->setIsStateSolid();
      }
    }
  }
}

Controller OptaController;

//} // namespace Opta

#endif // ARDUINO_OPTA
