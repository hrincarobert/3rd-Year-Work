#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string>
#include <tchar.h>
#include <TlHelp32.h>
#include <psapi.h>

bool get_id(const char* nume_proces, DWORD& processId) {

    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        printf("Nu s-a putut creea snapshot in get_id: %d\n", GetLastError());
        return false;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hProcessSnap, &pe32)) {
        do {
            if (_stricmp(pe32.szExeFile, nume_proces) == 0) {
                processId = pe32.th32ProcessID;
                CloseHandle(hProcessSnap);
                return true;
            }
        } while (Process32Next(hProcessSnap, &pe32));
    }

    CloseHandle(hProcessSnap);
    return false;
}

bool kill_process(const char* nume_proces) {

    DWORD processId;
    if (get_id(nume_proces, processId)) {
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processId);
        if (hProcess != NULL) {
            if (TerminateProcess(hProcess, 0)) {
                printf("Procesul a fost terminat cu succes\n");
                CloseHandle(hProcess);
                return true;
            }
            else {
                printf("Nu s-a putut termina procesul: %d\n", GetLastError());
                CloseHandle(hProcess);
                return false;
            }
        }
        else {
            printf("Nu s-a putut deschide procesul: %d\n", GetLastError());
            return false;
        }
    }
    else {
        printf("Nu s-a gasit procesul: %d\n", GetLastError());
        return false;
    }
}

int main() {

    const char* nume_proces = "Tema3_CSSO_Filter.exe";

    if (kill_process(nume_proces)==0)
        printf("Nu s-a putut termina procesul");
    

    return 0;
}
