#include "../includes/tinky-winkey.h"

int main(void)
{
    SC_HANDLE Manager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
}