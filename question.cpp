#include "LogEntry.h"



std::string entry_format::formattostring()
{
    char buffer[BUFF_SIZE_256] = { 0 };
    const int b = reset.is_success ? 1 : 0;
    UPD_SPRINTF(buffer, BUFF_SIZE_256, "time: %s;mode: %s; version: %s; result: %d", reset.entry_time.c_str(), reset.entry_mode.c_str(), reset.version.c_str(), b); 
    std::string output(buffer);
    return output;
}

int Log::insert(std::string new_line)
{
    std::vector<std::string>lines;
    read_out(lines);

    lines.push_back(new_line);
    if (lines.size() > MAX_LINE)
    {
        lines.erase(lines.begin());
    }

    write_back(lines);
    return 1;
}

int Log::read_out(std::vector<std::string> &lines)
{
    fstream fin(file_name.get(), ios::in);
    if (!fin.is_open())
    {
        UPD_LOG(ERR_LOG, "open file %s fail", file_name.get());
    }
    std::string line;
    while (!fin.eof())
    {
        getline(fin, line);
        if (is_blank_line(line))
        {
            continue;
        }
        lines.push_back(line);
    }
    fin.close();
    return 0;
}


int Log::write_back(std::vector<std::string> &lines)
{
    fstream fout(file_name.get(), ios::out);
    if (fout.is_open())
    {
        for (auto iter = lines.begin(); iter != lines.end(); iter++)
        {
            fout << *iter << endl;
        }
        fout.close();
    }
    return 1;
}

bool is_blank_line(std::string &line)
{
    if (line.size() == 0)
    {
        return true;
    }
    else
    {
        int i = 0;
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



void file_name_release(char *file_name)
{
    delete [] file_name;
    cout << "delte invoke" << endl;
    return;
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


int redirect_log()
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
        log_dir = LOG_DIR_QCOM;
    }
    else
    {
        log_dir = LOG_DIR_HISI;
    }
    if (UPD_ERR == check_dir_exist(log_dir))
    {
        UPD_LOG(ERR_LOG ,"check_dir_exist  fail");
        return  UPD_ERR;
    }
    return UPD_OK;
}


std::unique_ptr<format> make_string(const char* type)
{
    entry a;
    a.entry_mode = "002";
    a.entry_time = "12001";
    a.is_success = true;
    a.version = "abcdefghi";
    std::unique_ptr<format>ret(new entry_format(a)); 
    return ret;
}
template<class T>
std::unique_ptr<format> make_string(T type)
{
    if(*type == "reset")
    {
        return std::unique_ptr<format>(new entry_format(a));
    }
    else
    {
        return NULL;
    }
}

int record_entry_to_file(const char* f_name)
{
    char *log_dir = NULL;
    if(UPD_ERR == redirect_log())
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

    std::unique_ptr<format> ptr = make_string();
    std::string new_line = ptr->formattostring();
    if(access(p_file.get(), F_OK) != 0)
    {
        UPD_LOG(INFO_LOG,"file %s is not exist", p_file.get());
        //create file and write
        fstream fout(p_file.get(), ios::out);
        if (fout.is_open())
        {
            fout<<new_line<<endl;
            fout.close();
        }
    }
    else
    {
        Log log(p_file);
        log.insert(new_line);
    }
    if(-1 == chmod(p_file.get(), BUG_REPORT_MODE))
    {
        UPD_LOG(ERR_LOG, "chmod file [%s] fail", p_file.get());
    }
    return UPD_OK;
}
