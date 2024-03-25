#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string>
#include <tchar.h>
#include <TlHelp32.h>
#include <psapi.h>
#include <Windows.h>
#include <thread>
#include <vector>

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


LPDWORD CreeareMapari(const wchar_t* mapName, size_t size, HANDLE &hMapFile_1) {

    HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, mapName);
    if (hMapFile == NULL) {
        printf("Eroare la CreateFileMapping\n");
        CloseHandle(hMapFile);
        return 0;
    }

    LPDWORD mappedData = reinterpret_cast<LPDWORD>(MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, size));
    if (mappedData == NULL) {
        //memset(mappedData, 0, sizeof(DWORD) * size);
        printf("Eroare la MapViewOfFile\n");
        CloseHandle(hMapFile);
        return 0;
    }
    
    for (size_t i = 0; i < size / sizeof(DWORD); ++i) {
        mappedData[i] = 0xFFFFFFFF;
    }
    
    hMapFile_1 = hMapFile;
    return mappedData;
}



void make_sold_donations() {
  
    const wchar_t* soldFilePath = L"C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\sold.txt";
    const wchar_t* donationsFilePath = L"C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\donations.txt";


    HANDLE hSoldFile = CreateFile(soldFilePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hSoldFile == INVALID_HANDLE_VALUE) {
        printf("Eroare creere sold.txt\n");
        return;
    }

   
    DWORD initialValue = 0;
    DWORD bytesWritten;
    if (!WriteFile(hSoldFile, &initialValue, sizeof(initialValue), &bytesWritten, NULL)) {
        printf("Eroare scriere in sold.txt\n");
        CloseHandle(hSoldFile);
        return;
    }

 
    CloseHandle(hSoldFile);

   
    HANDLE hDonationsFile = CreateFile(donationsFilePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hDonationsFile == INVALID_HANDLE_VALUE) {
        printf("Eroare creere donations.txt");
        return;
    }

  
    if (!WriteFile(hDonationsFile, &initialValue, sizeof(initialValue), &bytesWritten, NULL)) {
        printf("Eroare scriere in donations.txt");
        CloseHandle(hDonationsFile);
        return;
    }


    CloseHandle(hDonationsFile);

    printf("sold.txt si donations.txt au fost creeate cu succes!\n");
}

bool StartProcess(const wstring& executablePath, DWORD timeoutInSeconds) {
   
    LPWSTR lpCommandLine = NULL;


    LPSECURITY_ATTRIBUTES lpProcessAttributes = NULL;
    LPSECURITY_ATTRIBUTES lpThreadAttributes = NULL;

 
    BOOL bInheritHandles = FALSE;

  
    DWORD dwCreationFlags = 0;


    LPVOID lpEnvironment = NULL;

  
    LPCTSTR lpCurrentDirectory = NULL;


    STARTUPINFO startupInfo;
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);

    
    PROCESS_INFORMATION processInfo;
    ZeroMemory(&processInfo, sizeof(processInfo));


    LPCTSTR lpApplicationName = executablePath.c_str();

  
    if (CreateProcess(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes,
        bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory,
        &startupInfo, &processInfo))
    {
      
        CloseHandle(processInfo.hThread);

      
        DWORD result = WaitForSingleObject(processInfo.hProcess, timeoutInSeconds * 1000);

      
        if (result == WAIT_OBJECT_0) {
         
            CloseHandle(processInfo.hProcess);
            return true;
        }
        else if (result == WAIT_TIMEOUT) {
        
            printf("Timeout atins de catre %s\n", executablePath);
            
        }
        else {
           
            DWORD error = GetLastError();
            printf("Eroare in timpul asteptarii pt proces, cod eroare: %u\n",error);
          
        }

        
        CloseHandle(processInfo.hProcess);
    }
    else
    {
       
        DWORD error = GetLastError();

        printf("Eroare pt a porni procesul %s , cod eroare: %u\n", executablePath, error);
       
    }

    return false;
}


void ReadAndDisplayFile(const wchar_t* filePath) {

    HANDLE hFile = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {

        printf("Error opening file in ReadAndDisplayFile!\n");
        
        return;
    }

    
    char buffer[1024];
    DWORD bytesRead;

    while (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0) {
       
        printf("%.*s", bytesRead, buffer);
    }


    CloseHandle(hFile);
}

int main()
{
    const wchar_t* path1 = L"C:\\Facultate";
    const wchar_t* path2 = L"C:\\Facultate\\CSSO";
    const wchar_t* path3 = L"C:\\Facultate\\CSSO\\Week4";
    const wchar_t* path4 = L"C:\\Facultate\\CSSO\\Week4\\Reports";
    const wchar_t* path5 = L"C:\\Facultate\\CSSO\\Week4\\Reports\\Summary";
    //const wchar_t* path6 = L"C:\\Facultate\\CSSO\\Laboratoare\\Week2\\Rezultate";



    if (CreeareDirector1(path1) &&
        CreeareDirector1(path2) &&
        CreeareDirector1(path3) &&
        CreeareDirector1(path4) &&
        CreeareDirector1(path5)
        ) {
        printf("Toate directoarele au fost create.\n");
    }
    else {
        printf("Toate directoarele au fost create deja.\n");
    }

    HANDLE marketShelves_HANDLE;
    HANDLE marketValability_HANDLE;
    HANDLE productPrices_HANDLE;

    size_t arraySize = 10000 * sizeof(DWORD);
    
    LPDWORD marketShelves = CreeareMapari(L"MarketShelves", arraySize, marketShelves_HANDLE);
    
    if (marketShelves == 0) {
        cout << marketShelves << '\n';
        UnmapViewOfFile(marketShelves);
        return 1;
    }
    


    LPDWORD marketValability = CreeareMapari(L"MarketValability", arraySize, marketValability_HANDLE);
    if (marketValability == 0) {
        cout << marketShelves << '\n';
        UnmapViewOfFile(marketValability);
        UnmapViewOfFile(marketShelves);
        return 1;
    }


    LPDWORD productPrices = CreeareMapari(L"ProductPrices", arraySize, productPrices_HANDLE);
    if (productPrices == 0) {
        cout << marketShelves << '\n';
        UnmapViewOfFile(marketShelves);
        UnmapViewOfFile(marketValability);
        UnmapViewOfFile(productPrices);
        return 1;
    }

    make_sold_donations();

    vector<wstring> executablePaths = {
        L"C:\\Users\\Robert\\Desktop\\VSCODE\\deposit\\x64\\Debug\\deposit.exe",
        L"C:\\Users\\Robert\\Desktop\\VSCODE\\sold\\x64\\Debug\\sold.exe",
        L"C:\\Users\\Robert\\Desktop\\VSCODE\\donate\\x64\\Debug\\donate.exe"
    };

   
    DWORD timeoutInSeconds = 60;

  
    vector<thread> threads;

    HANDLE hMutex = CreateMutex(NULL, FALSE, L"LogsMutex");
    if (hMutex == NULL) {
        printf("Nu s-a creeat mutexul pt logs!\n");
        return 1;
    }



  
    for (const auto& path : executablePaths) {
        threads.emplace_back([&path, timeoutInSeconds]() {
            if (StartProcess(path, timeoutInSeconds)) {
                printf("Procesul %u a finalizat cu succes\n", path);
                
            }
            else {
                printf("Procesul %u NU a finalizat cu succes\n", path);
            }
            });
    }

    for (auto& thread : threads) {
        thread.join();
    }
  

    const wchar_t* errorsFilePath = L"C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\errors.txt";
    const wchar_t* soldFilePath = L"C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\sold.txt";
    const wchar_t* donationsFilePath = L"C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\donations.txt";

   
    HANDLE hErrorsFile = CreateFile(errorsFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hErrorsFile == INVALID_HANDLE_VALUE) {
        
        ReadAndDisplayFile(soldFilePath);
        ReadAndDisplayFile(donationsFilePath);
    }
    else {
     
        CloseHandle(hErrorsFile);
        ReadAndDisplayFile(errorsFilePath);
    }


    UnmapViewOfFile(marketShelves);
    UnmapViewOfFile(marketValability);
    UnmapViewOfFile(productPrices);
    CloseHandle(marketShelves_HANDLE);
    CloseHandle(marketValability_HANDLE);
    CloseHandle(productPrices_HANDLE);
    CloseHandle(hMutex);
    

 
    return 0;


}
