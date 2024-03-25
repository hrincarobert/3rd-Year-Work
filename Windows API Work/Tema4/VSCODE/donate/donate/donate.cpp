#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string>
#include <tchar.h>
#include <TlHelp32.h>
#include <psapi.h>
#include <vector>
#include <windows.h>
#include <windows.h>
#include <string>
#include <sstream>


using namespace std;

const char* shelvesFileName = "MarketShelves";
const char* valabilityFileName = "MarketValability";
const char* pricesFileName = "ProductPrices";
const char* soldDirectory = "C:\\Users\\Robert\\Desktop\\sold";
const char* errorsFileName = "C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\errors.txt";
const char* logsFileName = "C:\\Facultate\\CSSO\\Week4\\Reports\\logs.txt";
const char* donationsFileName = "C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\donations.txt";

const int MAX_PATH_LEN = 260;
const int MAX_LINE_LEN = 256;


wchar_t* ConvertToWideString(const char* narrowStr) {
    int wideStrLen = MultiByteToWideChar(CP_UTF8, 0, narrowStr, -1, nullptr, 0);
    wchar_t* wideStr = new wchar_t[wideStrLen];
    MultiByteToWideChar(CP_UTF8, 0, narrowStr, -1, wideStr, wideStrLen);
    return wideStr;
}

LPDWORD OpenAndMapFile(LPCWSTR fileName) {



    HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, fileName);
    if (hMapFile == NULL) {
        cout << "EROARE LA DESCHIDERE DIN SOLD MAPPING" << endl;
        CloseHandle(hMapFile);
        return nullptr;
    }

    LPDWORD data = reinterpret_cast<LPDWORD>(MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0));
    if (data == nullptr) {

        CloseHandle(hMapFile);
        return nullptr;
    }
    CloseHandle(hMapFile);
    return data;
}

void WriteToFile(const char* fileName, const char* content) {
    HANDLE hFile = CreateFileA(fileName, FILE_APPEND_DATA, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD bytesWritten;
        WriteFile(hFile, content, static_cast<DWORD>(strlen(content)), &bytesWritten, NULL);
        CloseHandle(hFile);
    }
}

string ReadFileContent(const wstring& filename) {
    HANDLE hFile = CreateFile(filename.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Eroare la deschidere fisier in ReadFileContent!\n");

        return "";
    }

    const DWORD bufferSize = 50000;
    char buffer[bufferSize];
    string fileContent;

    while (true) {
        DWORD bytesRead;
        if (ReadFile(hFile, buffer, bufferSize - 1, &bytesRead, NULL)) {
            if (bytesRead == 0) {
                break;
            }

            buffer[bytesRead] = '\0';
            fileContent += buffer;
        }
        else {
            printf("Eroare la citire fisier din ReadFileContent!\n");
            fileContent = "";
            break;
        }
    }

    CloseHandle(hFile);
    return fileContent;
}


vector<string> ExtractLines(const string& content) {
    vector<string> lines;
    istringstream contentStream(content);
    string line;

    while (getline(contentStream, line, '\n')) {
        lines.push_back(line);
    }

    return lines;
}

void ProcessDonations(LPDWORD shelves, LPDWORD valability, LPDWORD prices) {
    
    size_t size = 10000;
    HANDLE hExistingMutex = OpenMutex(SYNCHRONIZE, FALSE, L"LogsMutex");
    for (size_t i = 0; i < size / sizeof(DWORD); ++i) {
        //cout << valability[i] << ' ';
        if (valability[i] == 0)
        {
            

            DWORD new_value = valability[i] + prices[i];

            char update[MAX_LINE_LEN];
            sprintf_s(update, MAX_LINE_LEN, "%u\n", new_value);
            WriteToFile(donationsFileName, update);

            WaitForSingleObject(hExistingMutex, INFINITE);

            char logContent[MAX_LINE_LEN];
            sprintf_s(logContent, MAX_LINE_LEN, "Produsul %u a fost donat.\n", i);

            WriteToFile(logsFileName, logContent);

            ReleaseMutex(hExistingMutex);
    
            CloseHandle(hExistingMutex);




            valability[i] = 0xFFFFFFFF;
            prices[i] = 0xFFFFFFFF;
            
            for (size_t j = 0; j < size / sizeof(DWORD); ++j)
            {
                if (shelves[j] == i)
                    shelves[j] = 0xFFFFFFFF;
            }

        }
        else if (valability[i] > 0 && valability[i] < 0xFFFFFFFF) {
            valability[i] = valability[i] - 1;
        }
        
    }
    
}



int main() {




    LPDWORD shelves = OpenAndMapFile(L"MarketShelves");
    LPDWORD valability = OpenAndMapFile(L"MarketValability");
    LPDWORD prices = OpenAndMapFile(L"ProductPrices");


    ProcessDonations(shelves, valability, prices);



    return 0;
}
