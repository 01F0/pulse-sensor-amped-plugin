#ifndef SERIAL_READER_H
#define SERIAL_READER_H
#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <stdbool.h>

#define BPM_TAG 'B'
#define SIGNAL_TAG 'S'
#define IBI_TAG 'Q'
#define MAX_BPM 210

bool open_com_port(const char* comPort);

bool close_com_port();

bool read_serial_data(int *data, char tag);

#endif