#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string>
#include <tchar.h>
#include <TlHelp32.h>
#include <psapi.h>
#include <wininet.h>
#include <vector>
#include <string>
#include <sstream>
#pragma comment(lib, "Wininet")

using namespace std;



wstring StringToTCHAR(const string& str) {
    int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    wstring buffer(size, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &buffer[0], size);
    return buffer;
}

wchar_t* ConvertToWideString(const char* narrowStr) {
    int wideStrLen = MultiByteToWideChar(CP_UTF8, 0, narrowStr, -1, nullptr, 0);
    wchar_t* wideStr = new wchar_t[wideStrLen];
    MultiByteToWideChar(CP_UTF8, 0, narrowStr, -1, wideStr, wideStrLen);
    return wideStr;
}

LPCWSTR StringToLPCWSTR(const string& str) {
    int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    wchar_t* buffer = new wchar_t[size];
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer, size);
    return buffer;
}

bool CreeareDirector1(const wchar_t* path) {
    if (CreateDirectory(path, NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
        return true;
    }
    else {
        printf("Eroare la a creea directorul %s. Cod eroare: %d\n", path, GetLastError());
        return false;
    }
}

void write_to_myconfig(const wchar_t* filePath, const char* data, DWORD dataSize) {

    HANDLE hFile = CreateFile(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD bytesWritten;
        WriteFile(hFile, data, dataSize, &bytesWritten, NULL);
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

int CreateTextFile(LPCWSTR fileName) {

    HANDLE hFile = CreateFile(
        fileName,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Eroare creeare fisier valoare!\n");
        return FALSE;
    }

    CloseHandle(hFile);

    return TRUE;
  
}

void AppendToFile(const TCHAR* filePath, const char* textToAppend) {
    HANDLE hFile = CreateFile(
        filePath,
        FILE_APPEND_DATA,
        0,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Eroare deschidere fisier in AppendToFile!\n");
        return;
    }

    SetFilePointer(hFile, 0, NULL, FILE_END);

   
    int length = MultiByteToWideChar(CP_UTF8, 0, textToAppend, -1, NULL, 0);
    if (length == 0) {
        printf("Eroare convertire wstrings in AppendToFile!\n");
        CloseHandle(hFile);
        return;
    }

   
    WCHAR* wideText = new WCHAR[length];
    MultiByteToWideChar(CP_UTF8, 0, textToAppend, -1, wideText, length);

    
    DWORD bytesWritten;
    WriteFile(hFile, wideText, length * sizeof(WCHAR), &bytesWritten, NULL);

    delete[] wideText;
    CloseHandle(hFile);

    if (bytesWritten == 0) {
        printf("Eroare!Nu s-a scris nimic in fisier din AppendToFile!\n");
    }
}

int main()
{
    /*const wchar_t* path1 = L"C:\\Facultate";
    const wchar_t* path2 = L"C:\\Facultate\\CSSO";
    const wchar_t* path3 = L"C:\\Facultate\\CSSO\\Laboratoare";*/
    const wchar_t* path4 = L"C:\\Facultate\\CSSO\\Week5";
    const wchar_t* path5 = L"C:\\Facultate\\CSSO\\Week5\\Downloads";
    //const wchar_t* path6 = L"C:\\Facultate\\CSSO\\Laboratoare\\Week2\\Rezultate";


    if (/*CreeareDirector1(path1) &&
        CreeareDirector1(path2) &&
        CreeareDirector1(path3) && */
        CreeareDirector1(path4) &&
        CreeareDirector1(path5)
        ) {
        printf("Toate directoarele (Week5 si Downloads) au fost create.\n");
    }
    else {
        printf("Toate directoarele au fost create deja.\n");
    }


    const wchar_t* url = L"http://cssohw.herokuapp.com/assignhomework/310910401ESL211041";

    HINTERNET hInternet = InternetOpen(L"Tema 5", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    
    if (hInternet) {
        
        HINTERNET hUrl = InternetOpenUrl(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (hUrl) {
            
            const int bufferSize = 10000;
            char buffer[bufferSize];
            DWORD bytesRead;

            
            while (InternetReadFile(hUrl, buffer, bufferSize, &bytesRead) && bytesRead > 0) {
                write_to_myconfig(L"C:\\Facultate\\CSSO\\Week5\\myconfig.txt", buffer, bytesRead);
            }
            
            char filePath_myconfig[500] = "C:\\Facultate\\CSSO\\Week5\\myconfig.txt";

            LPCWSTR filePath_wide = ConvertToWideString(filePath_myconfig);

            string content = ReadFileContent(filePath_wide);

            vector<string> lines = ExtractLines(content);

            size_t lineNumber = 0;

            HINTERNET hNet_server = InternetConnect(hInternet, _T("http://cssohw.herokuapp.com"), INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);

            if (hNet_server) {
                printf("Eroare InternetConnect!\n");
            }
            

            while (lineNumber < lines.size()) {


                string currentLine = lines[lineNumber];

                //cout << currentLine << endl;

                size_t colonPos = currentLine.find(':');

                string method = currentLine.substr(0, colonPos);
                string url = currentLine.substr(colonPos + 1);

                size_t dotComPos = url.find(".com");
                string afterDotCom = url.substr(dotComPos + 4);
                
                size_t slashPos = afterDotCom.find('/',1);
                string dohomework_type = afterDotCom.substr(0, slashPos);

                size_t firstSlashPos = afterDotCom.find('/');
                size_t secondSlashPos = afterDotCom.find('/', firstSlashPos + 1);

                string valoare = afterDotCom.substr(secondSlashPos + 1);

                LPCWSTR afterDotCom_LPCWSTR = StringToLPCWSTR(afterDotCom);

                string path_txt = "C:\\Facultate\\CSSO\\Week5\\Downloads\\";
                path_txt += valoare;
                path_txt += ".txt";



                //cout << afterDotCom << endl;


                if (dohomework_type == "/dohomework") {
                    if (method == "GET") {
                        HINTERNET open_req_get = HttpOpenRequest(hNet_server, L"GET", afterDotCom_LPCWSTR, L"HTTP/1.1", NULL, NULL, INTERNET_FLAG_RELOAD | INTERNET_FLAG_EXISTING_CONNECT, 0);

                        if (open_req_get) {
                            printf("Eroare HttpOpenRequest!\n");
                        }

                        bool send_req_get = HttpSendRequest(open_req_get, NULL, NULL, NULL, NULL);

                        if (send_req_get) {
                            printf("Eroare la HttpSendRequest!\n");
                        }

                        const int bufferSize = 100;
                        char buffer[bufferSize];
                        DWORD bytesRead;

                        LPCWSTR path_txt_w = StringToLPCWSTR(path_txt);
                        
                        

                        bool is_file_created = CreateTextFile(path_txt_w);

                        if (is_file_created == FALSE)
                        {
                            while (InternetReadFile(open_req_get, buffer, bufferSize, &bytesRead) && bytesRead > 0) {
                                cout << buffer << endl;
                                AppendToFile(path_txt_w, buffer);
                            }
                        }
                        else {
                            while (InternetReadFile(open_req_get, buffer, bufferSize, &bytesRead) && bytesRead > 0) {
                                cout << buffer << endl;
                                write_to_myconfig(path_txt_w, buffer, bytesRead);
                            }
                        }

                        

                        InternetCloseHandle(open_req_get);
                    }
                }
                
                delete[] afterDotCom_LPCWSTR;

                lineNumber++;
                
            }

            InternetCloseHandle(hNet_server);
            InternetCloseHandle(hUrl);
        }
        else {
            printf("Eroare InternetOpenUrl!\n");
        }

        InternetCloseHandle(hInternet);
    }
    else {
        printf("Eroare InternetOpen!\n");
    }

    

 
    

    return 0;



}