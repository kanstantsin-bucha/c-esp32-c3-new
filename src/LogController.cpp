#include <Print.h>
#include <stdio.h>
#include <Arduino.h>
#include "LogController.h"
#include "HardwareSerial.h"

#define LogControllerMaxLogLength 10000
#define LogControllerLogCutLength 1000
#define LogControllerMaxLineLenght 80
#define LogControllerMaxFormatStringLength 64

HardwareSerial *serial = nullptr;

void LogController::begin(HardwareSerial &printingSerial, unsigned long baud)
{
    serial = &printingSerial;

    if (serial == nullptr) { return; }
    serial->begin(baud);
}

void LogController::formatWithList(const char *format, va_list arg) {    
    char loc_buf[LogControllerMaxFormatStringLength];
    char * temp = loc_buf;
    va_list copy;
    va_copy(copy, arg);
    size_t len = vsnprintf(NULL, 0, format, arg);
    va_end(copy);
    if(len >= sizeof(loc_buf)){
        temp = new char[len+1];
        if(temp == NULL) {
            lit(F("[log] failed to print a format - no memory allocated"));
            return;
        }
    }
    len = vsnprintf(temp, len+1, format, arg);
    cStr(temp);
    if(len >= sizeof(loc_buf)){
        delete[] temp;
    }
}

void LogController::cStr(const char *str) {
    cStr(str, true);
}
void LogController::cStr(const char *str, bool wrapString) {
    if (disabled || serial == nullptr) {
        return;
    }
    // if (wrapString && addTimeMark) {
    //     int8_t size = 8 * sizeof(unsigned long);
    //     char buf[size + 2];
    //     buf[size+1] = 0;
    //     ultoa(millis(), buf, 10);
    //     serial->print(buf);
    //     serial->print(timeDelimiter);
    //     logStorage += buf;
    //     logStorage += timeDelimiter;
    // }
    // if (wrapString) { serial->print(logPrefix); }
    serial->print(str);
    // if (wrapString) { serial->print(logSuffix); }

    unsigned int lengthBefore = logStorage.length();
    
    // if (wrapString) { logStorage += logPrefix; }
    // logStorage += str;
    // if (wrapString) { logStorage += logSuffix; }

    // unsigned int lengthAfter = logStorage.length();
    // afterNewLineCounts += lengthAfter - lengthBefore;
    // bool shouldPutNewLine = afterNewLineCounts > LogControllerMaxLineLenght;
    // if (shouldPutNewLine) {
    //     logStorage += newLine;
    //     serial->print(newLine);
    //     afterNewLineCounts = 0;
    // }
    // if (lengthAfter > LogControllerMaxLogLength) {
    //     logStorage = logStorage.substring(LogControllerLogCutLength);
    // }
}
void LogController::nL() {
    if (disabled) {
        return;
    }
    cStr(newLine, false);
}
void LogController::ardStr(String string) {
    if (disabled) {
        return;
    }
    cStr(string.c_str());
}
void LogController::stdStr(std::string string) {
    if (disabled) {
        return;
    }
    char cstr[string.length() + 1];
        string.copy(cstr, string.length() + 1);
    cstr[string.length()] = '\0';
    cStr(cstr);
}
void LogController::lit(const __FlashStringHelper *ifsh) {
    cStr(reinterpret_cast<const char *>(ifsh)); 
}
void LogController::litFormat(const __FlashStringHelper *ifsh, ...) {
    if (disabled) {
        return;
    }
    const char *formatString = reinterpret_cast<const char *>(ifsh);
    va_list arg;
    va_start(arg, ifsh);
    formatWithList(formatString, arg);
    va_end(arg);
}
void LogController::format(const char *format, ...) {
    if (disabled) {
        return;
    }
    va_list arg;
    va_start(arg, format);
    formatWithList(format, arg);
    va_end(arg);
}

LogController lg = LogController();