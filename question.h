#ifndef _LOG_ENTRY_H
#define _LOG_ENTRY_H

#include <fstream>
#include <vector>
#include <string>
#include "hwrecovery_common_utility.h"



#define LOG_DIR_QCOM    "log/recovery/"
#define LOG_DIR_HISI    "splash2/recovery/"
#define LOG_NAME        "reset_entry"
#define MAX_LINE        50



typedef struct
{
    std::string entry_time;
    std::string entry_mode;
    std::string version;
    bool        is_success;
}entry;


class format
{
public:
    virtual std::string formattostring() = 0;
    virtual ~format() {};
};

class  entry_format:public format
{
public:
    entry_format(entry _t):reset(_t){}
    virtual std::string formattostring();
private:
    entry  reset;
};


class Log
{
public:
    Log(std::shared_ptr<char>name) :file_name(name) {};
    int insert(std::string str);
private:
    int read_out(std::vector<std::string> &lines);
    int write_back(std::vector<std::string> &lines);
    std::shared_ptr<char>file_name;
};
