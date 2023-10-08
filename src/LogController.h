#pragma once

#include <WString.h>
#include <cstdarg>
#include <string>

class Print;
class HardwareSerial;
class __FlashStringHelper;
class String;

struct LogController
{
    bool disabled = true;
    bool addTimeMark = false;
    void begin(HardwareSerial& printingSerial, unsigned long baud);
    void cStr(const char* str);
    void cStr(const char* str, bool wrapString);
    void nL();
    void ardStr(String string);
    void stdStr(std::string string);
    void lit(const __FlashStringHelper* ifsh);
    void litFormat(const __FlashStringHelper* ifsh, ...);
    void format(const char* format, ...);

private:
    String logStorage = String();
    Print* print;
    const char* logPrefix = " [";
    const char* logSuffix = "]\r\n";
    const char* timeDelimiter = ": ";
    const char* newLine = "\r\n";
    uint16_t afterNewLineCounts = 0;
    void formatWithList(const char* format, va_list arg);
};

extern LogController lg;

#define Las(str) lg.ardStr(str)
#define Lss(str) lg.stdStr(str)
#define Lcs(c_str) lg.cStr(c_str)
#define Llf(string_literal, ...) lg.litFormat(F(string_literal), __VA_ARGS__)
#define Lcf(cStr, ...) lg.format(cStr, __VA_ARGS__)
#define Ll(string_literal) lg.lit(F(string_literal))
#define Lnl() lg.nL()