#ifndef SERIAL_READER_H
#define SERIAL_READER_H
#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <stdbool.h>
#include <stdlib.h>

#define BPM_TAG 'B'
#define SIGNAL_TAG 'S'
#define IBI_TAG 'Q'
#define MAX_BPM 210

HANDLE open_com_port(const char* comPort);

bool close_com_port(HANDLE comPortFile);

bool read_serial_data(HANDLE comPortFile, volatile int *data, char tag);

#endif