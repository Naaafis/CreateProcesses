#include <windows.h>
#include <stdio.h>
#include <stdlib.h>


BOOL PrintFileContents(char* filename){
    // //your solution here!
    HANDLE hFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE){
        printf("Failed to open file %s and get handle because of %d \n", filename, GetLastError());
        return FALSE;
    }
    DWORD dwFileSize = GetFileSize(hFile, NULL);
    if (dwFileSize == INVALID_FILE_SIZE){
        printf("Failed to get file size because of %d \n", GetLastError());
        return FALSE;
    }
    char* buffer = (char*)malloc(dwFileSize + 1);
    // read the file into the buffer, dwFileSize + 1 because we need to null terminate the string later
    if (buffer == NULL){
        printf("Failed to allocate memory for file buffer because of %d \n", GetLastError());
        return FALSE;
    }
    DWORD dwBytesRead = 0;
    if (!ReadFile(hFile, buffer, dwFileSize, &dwBytesRead, NULL)){
        printf("Failed to read file into buffer because of %d \n", GetLastError());
        return FALSE;
    }
    // null terminate the string
    buffer[dwFileSize] = '\0';
    // printf("read %d bytes from file %s", dwBytesRead, filename);
    printf("%s", buffer);
    free(buffer);
    CloseHandle(hFile);
    return TRUE;
}