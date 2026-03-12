/* -------------------------------------------------------------------------- */
/* FILENAME:
   AUTHOR:      Daniele Aimo (d.aimo@arduino.cc / maidnl74@gmail.com)
   DATE:        20230801
   REVISION:    0.0.1
   DESCRIPTION:
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;

#define EXP_TYPE_DIGITAL 1
#define EXP_TYPE_ANALOG 2

int getUserNumber(bool &good) {
  string usr_string;

  cin >> usr_string;
  bool all_digits = true;
  for (int i = 0; i < usr_string.size(); i++) {
    if (!isdigit(static_cast<unsigned char>(usr_string[i]))) {
      all_digits = false;
      cout << ">> ERROR: Unrecognized input! (not a number)" << endl;
      break;
    }
  }

  if (!all_digits) {
    good = false;
    return -1;
  } else {
    good = true;
    return atoi(usr_string.c_str());
  }
}

int print_single_char_on_file(FILE *fp, unsigned char ch, bool comma = true) {
  int rv = 0;
  if (fp != nullptr) {
    rv = 4;
    fprintf(fp, "0x%02X", ch);
    if (comma) {
      fprintf(fp, ", ");
      rv++;
    }
  }
  return rv;
}

static const char fileOutDefault[] = "fwUpdate.h";
static const char fileOutAnalog[] = "fwUpdateAnalog.h";
static const char fileOutDigital[] = "fwUpdateDigital.h";

static const unsigned char OA_header[] =
    "const unsigned char opta_analog_fw_update[] = {";
static const unsigned char OD_header[] =
    "const unsigned char opta_digital_fw_update[] = {";

bool write_update(const char *filename, unsigned char e_type, unsigned char M,
                  unsigned char m, unsigned char r, unsigned char *fw,
                  uint32_t fw_size) {
  bool rv = true;
  FILE *fp = fopen(filename, "w");
  int printed = 0;

  if (fp == nullptr) {
    cout << "ERROR Unable to open output file!" << endl;
    rv = false;
    goto EXIT_WRITE_UPDATE_AND_CLOSE;
  }

  if (e_type == EXP_TYPE_ANALOG) {
    /* print declaration of array containing the FW update */
    fprintf(fp, "%s\n", OA_header);
  } else if (e_type == EXP_TYPE_DIGITAL) {
    fprintf(fp, "%s\n", OD_header);
  } else {
    cout << "ERROR Unrecognized option" << endl;
    rv = false;
    goto EXIT_WRITE_UPDATE;
  }

  if (fw == nullptr) {
    cout << "ERROR FW is missing" << endl;
    rv = false;
    goto EXIT_WRITE_UPDATE;
  }

  for (int i = 0; i < fw_size; i++) {
    if (i == fw_size - 1) {
      printed += print_single_char_on_file(fp, fw[i], false);
    } else {
      printed += print_single_char_on_file(fp, fw[i]);
    }
    if (printed > 0 && (printed % 80) == 0) {
      fprintf(fp, "\n");
    }
  }
  /* the fw is finished, put a newline */
  fprintf(fp, ",\n");

  print_single_char_on_file(fp, e_type);
  print_single_char_on_file(fp, M);
  print_single_char_on_file(fp, m);
  print_single_char_on_file(fp, r, false);
  fprintf(fp, " };\n");
EXIT_WRITE_UPDATE_AND_CLOSE:
  fclose(fp);
EXIT_WRITE_UPDATE:
  return rv;
}

int main(int argc, char *argv[]) {
  unsigned char exp_type = 0;
  unsigned char fw_version_major = 0;
  unsigned char fw_version_minor = 0;
  unsigned char fw_version_release = 0;
  bool finished = false;
  char *outfilename = (char *)fileOutDefault;
  string filename;
  uint32_t fsize = 0;
  uint32_t oa_size = 0;
  uint32_t od_size = 0;
  unsigned char *oa_fcontent = nullptr;
  unsigned char *od_fcontent = nullptr;

  cout << "*** Opta Fw Update Maker ***" << endl;
  cout << endl;
  cout << "This program packs an Expansions fw into .h files" << endl;
  cout << "NOTE: Both expansions (analog and digital) fw update are requested"
       << endl;
  cout << endl;

  while (!finished) {

    bool good = false;
    /*
     * GET THE EXPANSION TYPE
     * */
    if (oa_fcontent == nullptr && od_fcontent == nullptr) {
      cout << "Please select the type of expansion:" << endl;
      cout << " 1. OPTA DIGITAL " << endl;
      cout << " 2. OPTA ANALOG " << endl;
      cout << ":> ";

      exp_type = (unsigned char)getUserNumber(good);
      if (!good) {
        continue;
      }
    } else {
      if (oa_fcontent == nullptr) {
        exp_type = 2;
      } else if (od_fcontent == nullptr) {
        exp_type = 1;
      } else {
        cout << "ERROR: Unexpected status!" << endl;
        finished = true;
        continue;
      }
    }

    if (exp_type == EXP_TYPE_ANALOG) {
      if (oa_fcontent == nullptr) {
        cout << "\n--> *** ANALOG *** expansion type selected" << endl;
      }
    } else if (exp_type == EXP_TYPE_DIGITAL) {
      if (od_fcontent == nullptr) {
        cout << "\n--> *** DIGITAL *** expansion type selected" << endl;
      }
    } else {
      cout << ">> ERROR unrecognized option!" << endl;
      continue;
    }

  GET_MAJOR:
    /*
     * getting the version
     */
    cout << "Please Enter the Major number of the expansion FW release:"
         << endl;
    cout << ":> ";

    good = false;
    fw_version_major = (unsigned char)getUserNumber(good);
    if (!good) {
      goto GET_MAJOR;
    }
  GET_MINOR:
    cout << "Please Enter the Minor number of the expansion FW release:"
         << endl;
    cout << ":> ";

    good = false;
    fw_version_minor = (unsigned char)getUserNumber(good);
    if (!good) {
      goto GET_MINOR;
    }
  GET_RELEASE:
    cout << "Please Enter the Release number of the expansion FW release:"
         << endl;
    cout << ":> ";

    good = false;
    fw_version_release = (unsigned char)getUserNumber(good);
    if (!good) {
      goto GET_RELEASE;
    }

    cout << "You entered the FW version: " << (int)fw_version_major << "."
         << (int)fw_version_minor << "." << (int)fw_version_release << endl;

  GET_BINARY:

    cout << "Please Enter the path the FW (bin format) is located" << endl;
    cout << ":> ";
    cin >> filename;

    struct stat info;
    if (stat(filename.c_str(), &info) != 0) {
      cout << "ERROR Unable to stat file! (Did you enter the correct path?)"
           << endl;
      goto GET_BINARY;
    }

    FILE *fp = fopen(filename.c_str(), "r+b");
    if (fp == nullptr) {
      cout << "ERROR Unable to open the file " << filename << endl;
      goto GET_BINARY;
    }

    fsize = info.st_size;

    if (exp_type == EXP_TYPE_ANALOG) {

      oa_fcontent = new unsigned char[fsize];
      if (oa_fcontent == NULL) {
        cout << "ERROR Unable to allocate memory... Exit" << endl;
        exit(1);
      }
      size_t blocks_read = fread(oa_fcontent, fsize, 1, fp);
      if (blocks_read != 1) {
        cout << "ERROR! Unable to read all the file" << endl;
        exit(2);
      }
      fclose(fp);
      oa_size = fsize;
      outfilename = (char *)fileOutAnalog;
      if (!write_update(fileOutAnalog, exp_type, fw_version_major,
                        fw_version_minor, fw_version_release, oa_fcontent,
                        oa_size)) {
        cout << "ERROR Unable to write update on file " << outfilename << endl;
        exit(3);
      }

    } else if (exp_type == EXP_TYPE_DIGITAL) {
      od_fcontent = new unsigned char[fsize];
      if (od_fcontent == NULL) {
        cout << "ERROR Unable to allocate memory... Exit" << endl;
        exit(4);
      }
      size_t blocks_read = fread(od_fcontent, fsize, 1, fp);
      if (blocks_read != 1) {
        cout << "ERROR! Unable to read all the file" << endl;
        exit(5);
      }
      fclose(fp);
      od_size = fsize;
      outfilename = (char *)fileOutDigital;
      if (!write_update(fileOutDigital, exp_type, fw_version_major,
                        fw_version_minor, fw_version_release, od_fcontent,
                        od_size)) {
        cout << "ERROR Unable to write update on file " << outfilename << endl;
        exit(6);
      }
    }
    if (oa_fcontent != nullptr && od_fcontent != nullptr) {

      delete[] oa_fcontent;
      oa_fcontent = nullptr;
      delete[] od_fcontent;
      od_fcontent = nullptr;
      // EXIT
      finished = true;
    }
  }

  cout << "\n**** REPORT: " << endl;

  cout << "+ ANALOG EXPANSION fw update saved on file:" << endl;
  cout << "  ---> " << fileOutAnalog << endl;
  cout << "  analog fw size: " << oa_size << endl << endl;
  cout << "+ DIGITAL EXPANSION fw update saved on file:" << endl;
  cout << "  ---> " << fileOutDigital << endl;
  cout << "  digital fw size: " << od_size << endl << endl;
}
