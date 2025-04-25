// Storage.h
#ifndef STORAGE_H
#define STORAGE_H

#include "main.h"

extern const char* ssidFile;

bool loadHtmlFile(const char* filename, String& content);
bool loadWiFiConfig();
bool saveWiFiConfig();

#endif