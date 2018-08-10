#ifndef _LOG_ENTRY_H
#define _LOG_ENTRY_H

#include <fstream>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "hwroots.h"


#define LOG_DIR_Q    "/log/recovery/"
#define LOG_DIR_H   "/splash2/recovery/"
#define MAX_LINE              50



bool is_blank_line(std::string &line);
int check_dir_exist(const char* log_dir);
int redirect_log(char **log_dir);
void file_name_release(char *file_name);
std::string  get_current_time();
std::string fetch_base_version();
std::string get_entry_info();




typedef struct
{
    std::string    entry_time;
    std::string    mode;
    std::string    version;
    int    is_success;
}ResetRecord;

typedef struct
{
    std::string    entry_time;
    std::string    mode;
    bool    is_success;
}SDRecord;




class Format
{
public:
    virtual std::string FormatString() = 0;
    virtual ~Format() {};
};


class  ResetFormat:public Format
{
public:
    ResetFormat(ResetRecord _t):reset(_t){}
    virtual std::string FormatString();
private:
    ResetRecord  reset;
};

class  SDFormat :public Format
{
public:
    SDFormat(SDRecord _t):sd(_t){}
    virtual std::string FormatString();
private:
    SDRecord  sd;
};


template<typename T>
std::unique_ptr<Format> make_ptr(T obj)
{
    UPD_LOG(ERR_LOG, "Unrecognized type, please check if the type exists");
    return  NULL;
}

//Specialize for reset
template<>
inline std::unique_ptr<Format> make_ptr<ResetRecord>(ResetRecord obj)
{
    return std::unique_ptr<Format>(new ResetFormat(obj));
}

//Specialize for sd
template<>
inline std::unique_ptr<Format> make_ptr<SDRecord>(SDRecord obj)
{
    return std::unique_ptr<Format>(new SDFormat(obj));
}


class Log
{
public:
    Log(std::shared_ptr<char>name) :file_name(name) {};
    int insert(std::string str);
private:
    int read_out(std::vector<std::string> &lines);
    int write_in(std::vector<std::string> &lines);
    std::shared_ptr<char>file_name;
};





template<typename T>
int record_entry_to_file(const char* f_name, T obj)
{
    char *log_dir = NULL;
    if(UPD_ERR == redirect_log(&log_dir))
    {
        UPD_LOG(ERR_LOG ,"redirect log fail");
        return UPD_ERR;
    }
    //combine log_dir & file_name
    int size = strlen(log_dir) + strlen(f_name) + 1;
    std::shared_ptr<char>p_file(new char[size], file_name_release);
    UPD_MEMSET(p_file.get(), size, 0, size);
    UPD_STRCPY(p_file.get(), size, log_dir);
    UPD_STRCAT(p_file.get(), size, f_name);
    UPD_LOG(INFO_LOG, "Absolute path of the file is %s\n",p_file.get());

    std::unique_ptr<Format> ptr = make_ptr(obj);
    if(NULL == ptr)
    {
        UPD_LOG(ERR_LOG ,"ptr is NULL");
        return UPD_ERR;
    }
    std::string new_line = ptr->FormatString();
    if(access(p_file.get(), F_OK) != 0)
    {
        UPD_LOG(INFO_LOG,"file %s is not exist", p_file.get());
        std::fstream fout(p_file.get(), std::ios::out);
        if (!fout.is_open())
        {
            UPD_LOG(ERR_LOG, "Open file %s fail", p_file.get());
            return UPD_ERR;
        }
        else
        {
            fout << new_line <<std::endl;
            fout.close();
        }
    }
    else
    {
        Log log(p_file);
        if(UPD_ERR == log.insert(new_line))
        {
            UPD_LOG(ERR_LOG, "insert new_line to log fail");
            return UPD_ERR;
        }
    }
    if(-1 == chmod(p_file.get(), BUG_REPORT_MODE))
    {
        UPD_LOG(ERR_LOG, "chmod file [%s] fail", p_file.get());
    }
    return UPD_OK;
}

#endif
