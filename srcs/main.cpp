#include "../includes/tinky-winkey.h"

int main(void)
{
    SC_HANDLE SCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (SCManager == NULL)
    {
        exit(ERROR);
    }
    
}