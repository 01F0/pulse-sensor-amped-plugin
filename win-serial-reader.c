#include "win-serial-reader.h"

HANDLE comPortFile;

bool open_com_port(const char* comPort)
{
	comPortFile = CreateFileA(comPort,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (INVALID_HANDLE_VALUE == comPortFile) {
		return false;
	}

	DCB dcbPort;
	memset(&dcbPort, 0, sizeof(dcbPort));
	dcbPort.DCBlength = sizeof(dcbPort);

	if (!GetCommState(comPortFile, &dcbPort)) {
		return false;
	}

	if (!BuildCommDCB(L"baud=115200 parity=n data=8 stop=1", &dcbPort)) {
		return false;
	}

	if (!SetCommState(comPortFile, &dcbPort)) {
		return false;
	}

	COMMTIMEOUTS timeoutSettings;
	timeoutSettings.ReadIntervalTimeout = 1;
	timeoutSettings.ReadTotalTimeoutMultiplier = 1;
	timeoutSettings.ReadTotalTimeoutConstant = 1;
	timeoutSettings.WriteTotalTimeoutMultiplier = 1;
	timeoutSettings.WriteTotalTimeoutConstant = 1;

	if (!SetCommTimeouts(comPortFile, &timeoutSettings)) {
		return false;
	}

	return true;
}

bool close_com_port()
{
	return CloseHandle(comPortFile);
}

bool read_serial_data(volatile int *data, char tag)
{
	char readBuffer[1];
	DWORD numberOfBytesRead;
	int charIndexCounter = 0;
	char tempBuffer[1024];

	bool foundStart = false;

	do {

		if (!ReadFile(comPortFile, readBuffer, sizeof(readBuffer), &numberOfBytesRead, NULL)) {
			return false;
		}

		if (readBuffer[0] == tag) {
			foundStart = true;
			continue;
		}

		if (readBuffer == NULL) {
			continue;
		}
		if (numberOfBytesRead == 0) {
			continue;
		}

		if (foundStart) {

			if (readBuffer[0] == 0x0D) {
				tempBuffer[charIndexCounter] = '\0';

				*data = atoi(tempBuffer);

				return true;
			}

			tempBuffer[charIndexCounter] = *readBuffer;

			charIndexCounter++;
		}

	} while (TRUE);

	return false;
}


