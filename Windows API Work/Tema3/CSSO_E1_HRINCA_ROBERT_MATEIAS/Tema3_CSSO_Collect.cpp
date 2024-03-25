#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string>
#include <tchar.h>
#include <TlHelp32.h>
#include <psapi.h>

using namespace std;

bool CreeareDirector1(const wchar_t* path) {
    if (CreateDirectory(path, NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
        return true;
    }
    else {
        printf("Eroare la a creea directorul %s. Cod eroare: %d\n", path, GetLastError());
        return false;
    }
}


int info_procese() {

    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
   
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        printf("CreateToolhelp32Snapshot in info_procese failed.err = %d \n", GetLastError());
        return(-1);
    }
  
    pe32.dwSize = sizeof(PROCESSENTRY32);
    TCHAR infoBuffer[100000];  
    infoBuffer[0] = _T('\0');

    if (!Process32First(hProcessSnap, &pe32))
    {
        printf("Process32First in info_procese failed. err = %d \n", GetLastError());
        CloseHandle(hProcessSnap); 
        return(-1);
    }
    do
    {
        TCHAR buffer[256];
        _stprintf_s(buffer, _T("Process [%lu]: ParentProcess [%lu], Name: %s\n"), pe32.th32ProcessID, pe32.th32ParentProcessID, pe32.szExeFile);

        _tcscat_s(infoBuffer, 100000, buffer);
        _tcscat_s(infoBuffer, 100000, _T("\n"));
        //printf("ParentProcess [%d] :Process [%d]: %s \n", pe32.th32ParentProcessID, pe32.th32ProcessID, pe32.szExeFile);

    } while (Process32Next(hProcessSnap, &pe32)); 

    int bufferSize = WideCharToMultiByte(CP_ACP, 0, infoBuffer, -1, NULL, 0, NULL, NULL);
    char* asciiString = new char[bufferSize];
    WideCharToMultiByte(CP_ACP, 0, infoBuffer, -1, asciiString, bufferSize, NULL, NULL);

    LPCWSTR path_procese_txt = L"C:\\Facultate\\CSSO\\Laboratoare\\Week3\\ProcessInfo\\procese.txt";
    HANDLE fileHandle = CreateFile(path_procese_txt, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fileHandle != INVALID_HANDLE_VALUE) {
        DWORD dwWritten;
        WriteFile(fileHandle, asciiString, strlen(asciiString) * sizeof(char), &dwWritten, NULL);
    }
    
    CloseHandle(fileHandle);
    CloseHandle(hProcessSnap);
    delete[] asciiString;
    return 1;
}


int info_fire()
{
    HANDLE hProcessSnap;
    THREADENTRY32 te32;
    te32.dwSize = sizeof(THREADENTRY32);

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        printf("CreateToolhelp32Snapshot in info_fire failed.err = %d \n", GetLastError());
        return(-1);
    }

    
    TCHAR infoBuffer[200000];
    infoBuffer[0] = _T('\0');

    if (!Thread32First(hProcessSnap, &te32))
    {
        printf("Thread32First in info_fire failed. err = %d \n", GetLastError());
        CloseHandle(hProcessSnap);
        return(-1);
    }
    do
    {
        TCHAR buffer[256];
        _stprintf_s(buffer, _T("ThreadId [%lu] ,  OwnerProcessId [%lu]\n"), te32.th32ThreadID, te32.th32OwnerProcessID);

        _tcscat_s(infoBuffer, 200000, buffer);
        _tcscat_s(infoBuffer, 200000, _T("\n"));

    } while (Thread32Next(hProcessSnap, &te32));

    int bufferSize = WideCharToMultiByte(CP_ACP, 0, infoBuffer, -1, NULL, 0, NULL, NULL);
    char* asciiString = new char[bufferSize];
    WideCharToMultiByte(CP_ACP, 0, infoBuffer, -1, asciiString, bufferSize, NULL, NULL);

    LPCWSTR path_fire_txt = L"C:\\Facultate\\CSSO\\Laboratoare\\Week3\\ProcessInfo\\fire.txt";
    HANDLE fileHandle = CreateFile(path_fire_txt, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fileHandle != INVALID_HANDLE_VALUE) {
        DWORD dwWritten;
        WriteFile(fileHandle, asciiString, strlen(asciiString) * sizeof(char), &dwWritten, NULL);
    }

    CloseHandle(fileHandle);
    CloseHandle(hProcessSnap);
    delete[] asciiString;
    return 1;
}

int info_module() {


    TCHAR infoBuffer[10000];
    infoBuffer[0] = _T('\0');

    HMODULE hModules[1024];
    DWORD cbNeeded;
    if (EnumProcessModules(GetCurrentProcess(), hModules, sizeof(hModules), &cbNeeded)) {
        for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
            TCHAR szModule[MAX_PATH];
            if (GetModuleFileNameEx(GetCurrentProcess(), hModules[i], szModule, MAX_PATH)) {
                MODULEINFO moduleInfo;
                GetModuleInformation(GetCurrentProcess(), hModules[i], &moduleInfo, sizeof(MODULEINFO));

                TCHAR szExePath[MAX_PATH];
                if (GetModuleFileName(NULL, szExePath, MAX_PATH)) {

                    TCHAR buffer[5000];
                    _stprintf_s(buffer, _T("ModuleId: %p, ProcessId: %p, szModule: %s, szExePath: %s\n"),
                        hModules[i], GetCurrentProcess(), szModule, szExePath);

                    _tcscat_s(infoBuffer, 10000, buffer);
                    _tcscat_s(infoBuffer, 10000, _T("\n"));
                    
                }
                else
                {
                    printf("Eroare GetModuleFileName in info_module = %d \n", GetLastError());
                }
            }
            else
            {
                printf("Eroare GetModuleFileNameEx in info_module = %d \n", GetLastError());
            }
            
        }
    }
    else
    {
        printf("Eroare EnumProcessModules in info_module = %d \n", GetLastError());
    }

    int bufferSize = WideCharToMultiByte(CP_ACP, 0, infoBuffer, -1, NULL, 0, NULL, NULL);
    char* asciiString = new char[bufferSize];
    WideCharToMultiByte(CP_ACP, 0, infoBuffer, -1, asciiString, bufferSize, NULL, NULL);

    LPCWSTR path_module_txt = L"C:\\Facultate\\CSSO\\Laboratoare\\Week3\\ProcessInfo\\module_process.txt";
    HANDLE fileHandle = CreateFile(path_module_txt, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fileHandle != INVALID_HANDLE_VALUE) {
        DWORD dwWritten;
        WriteFile(fileHandle, asciiString, strlen(asciiString) * sizeof(char), &dwWritten, NULL);
    }

    CloseHandle(fileHandle);
    delete[] asciiString;
    return 1;
}

int GetFileContor(LPCTSTR filePath) {

    int contor = 0;
    HANDLE hFile = CreateFile(filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile != INVALID_HANDLE_VALUE) {
        char buffer[20000]; 
        DWORD bytesRead;

        while (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0) {
            for (DWORD i = 0; i < bytesRead; i++) {
                if (buffer[i] == '\n') {
                    contor++;
                }
            }
        }

        CloseHandle(hFile);
    }

    return contor;
}

void scrie_mapare() {

  
    int nr_module = GetFileContor(_T("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\ProcessInfo\\module_process.txt"));
    int nr_procese = GetFileContor(_T("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\ProcessInfo\\procese.txt"));
    int nr_fire = GetFileContor(_T("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\ProcessInfo\\fire.txt"));

    TCHAR summary[256];
    _stprintf_s(summary, _T("Module: %d\nProcese: %d\nFire: %d"), nr_module, nr_procese, nr_fire);

    HANDLE hMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 256, _T("cssoh3basicsync"));
    if (hMapping == NULL) {
        printf("Eroare CreateFileMapping in scrie_mapare = %d \n", GetLastError());
      
    }


    LPVOID pMapView = MapViewOfFile(hMapping, FILE_MAP_WRITE, 0, 0, 256);
    if (pMapView) {
        _tcscpy_s((LPTSTR)pMapView, 256, summary);
        UnmapViewOfFile(pMapView);
        printf("S-a scris in memory mapping cu numele �cssoh3basicsync�.\n ");
    }

    CloseHandle(hMapping);
}

int main()
{
    /*const wchar_t* path1 = L"C:\\Facultate";
    const wchar_t* path2 = L"C:\\Facultate\\CSSO";
    const wchar_t* path3 = L"C:\\Facultate\\CSSO\\Laboratoare";*/
    const wchar_t* path4 = L"C:\\Facultate\\CSSO\\Laboratoare\\Week3";
    const wchar_t* path5 = L"C:\\Facultate\\CSSO\\Laboratoare\\Week3\\ProcessInfo";
    //const wchar_t* path6 = L"C:\\Facultate\\CSSO\\Laboratoare\\Week2\\Rezultate";


    if (/*CreeareDirector1(path1) &&
        CreeareDirector1(path2) &&
        CreeareDirector1(path3) && */
        CreeareDirector1(path4) &&
        CreeareDirector1(path5) 
        ) {
        printf("Toate directoarele (Week3 si ProcessInfo) au fost create.\n");
    }
    else {
        printf("Toate directoarele au fost create deja.\n");
    }   

    info_procese();

    info_fire();

    info_module();

    scrie_mapare();

    return 0;
}