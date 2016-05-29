#include "win-serial-reader.h"

HANDLE open_com_port(const char* comPort) {
	HANDLE comPortFile;
	comPortFile = CreateFileA(comPort,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (INVALID_HANDLE_VALUE == comPortFile) {
		return NULL;
	}

	DCB dcbPort;
	memset(&dcbPort, 0, sizeof(dcbPort));
	dcbPort.DCBlength = sizeof(dcbPort);

	if (!GetCommState(comPortFile, &dcbPort)) {
		return NULL;
	}

	if (!BuildCommDCB(L"baud=115200 parity=n data=8 stop=1", &dcbPort)) {
		return NULL;
	}

	if (!SetCommState(comPortFile, &dcbPort)) {
		return NULL;
	}

	COMMTIMEOUTS timeoutSettings;
	timeoutSettings.ReadIntervalTimeout = 1;
	timeoutSettings.ReadTotalTimeoutMultiplier = 1;
	timeoutSettings.ReadTotalTimeoutConstant = 1;
	timeoutSettings.WriteTotalTimeoutMultiplier = 1;
	timeoutSettings.WriteTotalTimeoutConstant = 1;

	if (!SetCommTimeouts(comPortFile, &timeoutSettings)) {
		return NULL;
	}

	return comPortFile;
}

bool close_com_port(HANDLE comPortFile) {
	return CloseHandle(comPortFile);
}

bool read_serial_data(HANDLE comPortFile, volatile int *data, char tag) {
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


