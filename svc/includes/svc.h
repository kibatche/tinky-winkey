#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdlib.h>
#include "consts.h"

void installSvc(SC_HANDLE SCManager);
void startSvc(SC_HANDLE SCManager);
void stopSvc(SC_HANDLE SCManager);
void deleteSvc(SC_HANDLE SCManager);

int print_err(void);