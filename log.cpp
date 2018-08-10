#include <time.h>

#include "LogEntry.h"
#include "hwbootloader.h"
#include "oeminfo.h"

std::string ResetFormat::FormatString()
{
    char buffer[BUFF_SIZE_256] = { 0 };
    const char* result = (reset.is_success == -1) ? "fail" : "success";
    UPD_SPRINTF(buffer, BUFF_SIZE_256, "time: %s; mode: %s; version: %s; result: %s", reset.entry_time.c_str(), reset.mode.c_str(), reset.version.c_str(), result); 
    return buffer;
}

std::string SDFormat::FormatString()
{
    char buffer[BUFF_SIZE_256] = { 0 };
    const int result = sd.is_success ? 1 : 0;
    UPD_SPRINTF(buffer, 256, "time: %s; mode: %s; result: %d", sd.entry_time.c_str(), sd.mode.c_str(), result);
    return buffer;
}



int Log::insert(std::string new_line)
{
    std::vector<std::string>lines;
    if(UPD_ERR == read_out(lines))
    {
        UPD_LOG(ERR_LOG, "read_out fail");
        return UPD_ERR;
    }
    lines.push_back(new_line);
    if (lines.size() > MAX_LINE)
    {
        lines.erase(lines.begin());
    }
    if(UPD_ERR == write_in(lines))
    {
        UPD_LOG(ERR_LOG, "write_in fail");
        return UPD_ERR;
    }
    return UPD_OK;
}

int Log::read_out(std::vector<std::string> &lines)
{
    std::fstream fin(file_name.get(), std::ios::in);
    if (!fin.is_open())
    {
        UPD_LOG(ERR_LOG, "open file %s fail", file_name.get());
        return UPD_ERR;
    }
    std::string line;
    while (!fin.eof())
    {
        std::getline(fin, line);
        if (is_blank_line(line))
        {
            continue;
        }
        lines.push_back(line);
    }
    fin.close();
    return UPD_OK;
}


int Log::write_in(std::vector<std::string> &lines)
{
    std::fstream fout(file_name.get(), std::ios::out);
    if (!fout.is_open())
    {
        UPD_LOG(ERR_LOG, "open file %s fail", file_name.get());
        return UPD_ERR;
    }
    for (auto iter = lines.begin(); iter != lines.end(); iter++)
    {
        fout << *iter << std::endl;
    }
    fout.close();
    return UPD_OK;
}

bool is_blank_line(std::string &line)
{
    if (line.size() == 0)
    {
        return true;
    }
    else
    {
        unsigned long i = 0;
        while (i < line.size())
        {
            if (line[i++] != ' ')
            {
                return false;
            }
        }
        return true;
    }
}

int check_dir_exist(const char* log_dir)
{
    int ret = 0;
    if ( access( log_dir, F_OK ) != 0 )
    {
        ret = mkdir( log_dir, BUG_REPORT_MODE );
        if( 0 == ret)
        {
            UPD_LOG(INFO_LOG, "create log dir[%s] success", log_dir);
            if(-1 == chmod(log_dir, BUG_REPORT_MODE))
            {
                UPD_LOG(ERR_LOG, "chmod dir[%s] fail", log_dir);
            }
            return UPD_OK;
        }
        else
        {
            UPD_LOG (ERR_LOG,"create log dir[%s] fail", log_dir);
            return UPD_ERR;
        }
    }
    else
    {
        UPD_LOG(INFO_LOG, "log dir[%s] exist", log_dir);
        if(-1 == chmod(log_dir, BUG_REPORT_MODE))
        {
            UPD_LOG(ERR_LOG, "chmod dir[%s] fail", log_dir);
        }
        return UPD_OK;
    }
}


int redirect_log(char **log_dir)
{
    if (0 != hw_ensure_path_mounted("/splash2"))
    {
        UPD_LOG(INFO_LOG, "mount  /splash2  fail");
    }
    if (0 != hw_ensure_path_mounted("/log"))
    {
        UPD_LOG(INFO_LOG, "mount  /log  fail");
    }
    if (isMountpointMounted("/log") == 0)
    {
        *log_dir = LOG_DIR_QCOM;
    }
    else
    {
        *log_dir = LOG_DIR_HISI;
    }
    if (UPD_ERR == check_dir_exist(*log_dir))
    {
        UPD_LOG(ERR_LOG ,"check_dir_exist  fail");
        return  UPD_ERR;
    }
    return UPD_OK;
}

void file_name_release(char *file_name)
{
    delete [] file_name;
    return;
}

std::string  get_current_time()
{
    struct tm t = {0};
    time_t  now = {0};
    time(&now);
    localtime_r(&now, &t);
    char buffer[BUFF_SIZE_32] = {0};
    UPD_SPRINTF(buffer, BUFF_SIZE_32, "%d%.2d%.2d%.2d%.2d%.2d",t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour + 8, t.tm_min, t.tm_sec);
    return buffer;
}

std::string fetch_base_version()
{
    unsigned char temp[BUFF_SIZE_256 + 1] = {0};
    char buffer[BUFF_SIZE_256 + 1] = {0};
    if( 0 !=  rmt_oeminfo_read( OEMINFO_BASE_VERSION , BUFF_SIZE_256,  temp) )
    {
        UPD_SPRINTF(buffer,BUFF_SIZE_256 + 1, "fail");
    }
    else
    {
        UPD_MEMCPY(buffer, BUFF_SIZE_256 + 1, temp, BUFF_SIZE_256);
    }
    return buffer;
}
