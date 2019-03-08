#include "CNetMessage.h"

CNetMessage::CNetMessage() {
	reset();
}

void CNetMessage::reset() {
	for (int i = 0; i < 256; i++)
		buffer[i] = 0;
	state = EMPTY;
}

void CNetMessage::finish() {
	if (state == READING)
		state = FULL;
}

int CNetMessage::NumToLoad() {
	if (state == EMPTY)
		return 255;
	else
		return 0;
}

int CNetMessage::NumToUnLoad() {
	if (state == FULL)
		return strlen(buffer) + 1;
	else
		return 0;
}

void CNetMessage::LoadBytes(charbuf& inputbuffer, int n) {
	for (int i = 0; i < n; i++)
		buffer[i] = inputbuffer[i];
	state = READING;
}

void CNetMessage::UnLoadBytes(charbuf& destbuffer) {
	for (int i = 0; i < this->NumToUnLoad(); i++)
		destbuffer[i] = buffer[i];
	reset();
}