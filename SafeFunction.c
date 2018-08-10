
#ifdef __cplusplus
extern "C" {
#endif


// function return value, UPD_OK : process ok ; UPD_ERR : process with error
#define UPD_OK                                  (0)
#define UPD_ERR                                 (-1)
#define UPD_WRONG_PARA                          (0x1)

// only for code chapter describe
#define UPD_CHAPTER(x)                          1

#if UPD_CHAPTER("delay time")
#define UPD_DELAY_1                             1
#define UPD_DELAY_30                            30
#define UPD_DELAY_50                            50
#define UPD_DELAY_100                           100
#define UPD_DELAY_200                           200
#define UPD_DELAY_250                           250
#define UPD_DELAY_500                           500
#define UPD_DELAY_750                           750
#define UPD_DELAY_1000                          1000
#define UPD_DELAY_1500                          1500
#endif // UPD_CHAPTER("delay time")

#if UPD_CHAPTER("times")
#define UPD_TIMES_3                             3
#define UPD_TIMES_10                            10
#define UPD_TIMES_20                            20
#define UPD_TIMES_50                            50
#endif // UPD_CHAPTER("times")

#if UPD_CHAPTER("array")
#define NULL_CHAR_LEN                           (1)                 // char length of '\0', only suggest
#define BUFF_SIZE_16                            16
#define BUFF_SIZE_32                            32
#define BUFF_SIZE_64                            64
#define BUFF_SIZE_128                           128
#define BUFF_SIZE_256                           256
#define BUFF_SIZE_1024                          1024
#define ARRAY_ITEM_NUM(array)                   (sizeof(array) / sizeof(array[0]))
#endif // UPD_CHAPTER("array")

#if UPD_CHAPTER("pointer")
#define PTR_IS_NULL(ptr)                        ((ptr) == NULL)     // only suggest
#define PTR_IS_VALID(ptr)                       ((ptr) != NULL)     // only suggest

#define SAFE_DEL_PTR(x)\
if (PTR_IS_VALID(x))\
{\
    delete(x);\
    (x) = NULL;\
}

#define SAFE_DEL_ARRAY(x) \
if (PTR_IS_VALID(x))\
{\
    delete[](x);\
    (x) = NULL;\
}

#define SAFE_FREE_PTR(x)\
if (PTR_IS_VALID(x))\
{\
    free(x);\
    (x) = NULL;\
}
#endif // UPD_CHAPTER("pointer")

#if UPD_CHAPTER("UPD_CHK")

/*  1) CHK          - check condition, if ture, return
    2) LOG          - check condition, if ture, print log
    3) RETURN       - return ret;
    4) RETURN_VOID  - return;
    5) GOTO         - check condition, if ture, goto label
*/
#define UPD_CHK_LOG_RETURN(condition, ret, format, args...)\
do {\
    if (condition)\
    {\
        UPD_LOG(ERR_LOG, format" ret-value 0x%x.", ##args, ret);\
        return ret;\
    }\
} while(0)

#define UPD_CHK_LOG_RETURN_VOID(condition, format, args...)\
do {\
    if (condition)\
    {\
        UPD_LOG(ERR_LOG, format, ##args);\
        return;\
    }\
} while(0)

#define UPD_CHK_LOG(condition, log_type, format, args...)\
do {\
    if (condition)\
    {\
        UPD_LOG(log_type, format, ##args);\
    }\
} while(0)

#define UPD_CHK_RETURN(condition, ret)\
do {\
    if (condition)\
    {\
        return ret;\
    }\
} while(0)

#define UPD_CHK_RETURN_VOID(condition)\
do {\
    if (condition)\
    {\
        return;\
    }\
} while(0)

#define UPD_CHK_LOG_GOTO(condition, label, format, args...)\
do {\
    if (condition)\
    {\
        UPD_LOG(ERR_LOG, format, ##args);\
        goto label;\
    }\
} while(0)
#endif // UPD_CHAPTER("UPD_CHK")

#if UPD_CHAPTER("safe function with return value")
#define UPD_MEMSET_RET_VALUE(ret_value, dest, dest_max, value, count)\
{\
    ret_value = memset_s(dest, dest_max, value, count);\
    if (ret_value != UPD_OK)\
    {\
        UPD_LOG(ERR_LOG, "memset_s fail, dest %s, dest_max %d, value '%c', count %d, ret-value 0x%x.", dest, dest_max, value, count, ret_value);\
    }\
}

#define UPD_MEMCPY_RET_VALUE(ret_value, dest, dest_max, src, count)\
{\
    ret_value = memcpy_s(dest, dest_max, src, count);\
    if (ret_value != UPD_OK)\
    {\
        UPD_LOG(ERR_LOG, "memcpy_s fail, dest %s, dest_max %d, src %s, count %d, ret-value 0x%x.", dest, dest_max, src, count, ret_value);\
    }\
}

#define UPD_MEMMOVE_RET_VALUE(ret_value, dest, dest_max, src, count)\
{\
    ret_value = memmove_s(dest, dest_max, src, count);\
    if (ret_value != UPD_OK)\
    {\
        UPD_LOG(ERR_LOG, "memmove_s fail, dest %s, dest_max %d, src %s, count %d, ret-value 0x%x.", dest, dest_max, src, count, ret_value);\
    }\
}

#define UPD_STRCPY_RET_VALUE(ret_value, dest_str, dest_max, src_str)\
{\
    ret_value = strcpy_s(dest_str, dest_max, src_str);\
    if (ret_value != UPD_OK)\
    {\
        UPD_LOG(ERR_LOG, "strcpy_s fail, dest_str %s, dest_max %d, src_str %s, ret-value 0x%x.", dest_str, dest_max, src_str, ret_value);\
    }\
}

#define UPD_STRNCPY_RET_VALUE(ret_value, dest_str, dest_max, src_str, count)\
{\
    ret_value = strncpy_s(dest_str, dest_max, src_str, count);\
    if (ret_value != UPD_OK)\
    {\
        UPD_LOG(ERR_LOG, "strncpy_s fail, dest_str %s, dest_max %d, src_str %s, count %d, ret-value 0x%x.", dest_str, dest_max, src_str, count, ret_value);\
    }\
}

#define UPD_STRCAT_RET_VALUE(ret_value, dest_str, dest_max, src_str)\
{\
    ret_value = strcat_s(dest_str, dest_max, src_str);\
    if (ret_value != UPD_OK)\
    {\
        UPD_LOG(ERR_LOG, "strcat_s fail, dest_str %s, dest_max %d, src_str %s, ret-value 0x%x.", dest_str, dest_max, src_str, ret_value);\
    }\
}

#define UPD_STRNCAT_RET_VALUE(ret_value, dest_str, dest_max, src_str, count)\
{\
    ret_value = strncat_s(dest_str, dest_max, src_str, count);\
    if (ret_value != UPD_OK)\
    {\
        UPD_LOG(ERR_LOG, "strncat_s fail, dest_str %s, dest_max %d, src_str %s, count %d, ret-value 0x%x.", dest_str, dest_max, src_str, count, ret_value);\
    }\
}

#define UPD_SPRINTF_RET_LEN(ret_len, dest_str, dest_max, format...)\
{\
    ret_len = sprintf_s(dest_str, dest_max, format);\
    if (ret_len == UPD_ERR)\
    {\
        UPD_LOG(ERR_LOG, "sprintf_s fail, dest_str %s, dest_max %d.", dest_str, dest_max);\
    }\
}

#define UPD_SNPRINTF_RET_LEN(ret_len, dest_str, dest_max, count, format...)\
{\
    ret_len = snprintf_s(dest_str, dest_max, count, format);\
    if (ret_len < 0)\
    {\
        UPD_LOG(ERR_LOG, "snprintf_s fail, dest_str %s, dest_max %d, count %d, ret_len %d.", dest_str, dest_max, count, ret_len);\
    }\
}

#define UPD_VSPRINTF_RET_LEN(ret_len, dest_str, dest_max, format, arg_list)\
{\
    ret_len = vsprintf_s(dest_str, dest_max, format, arg_list);\
    if (ret_len == UPD_ERR)\
    {\
        UPD_LOG(ERR_LOG, "vsprintf_s fail, dest_str %s, dest_max %d.", dest_str, dest_max);\
    }\
}

#define UPD_VSNPRINTF_RET_LEN(ret_len, dest_str, dest_max, count, format, arg_list)\
{\
    ret_len = vsnprintf_s(dest_str, dest_max, count, format, arg_list);\
    if (ret_len == UPD_ERR)\
    {\
        UPD_LOG(ERR_LOG, "vsnprintf_s fail, dest_str %s, dest_max %d, count %d.", dest_str, dest_max, count);\
    }\
}

#define UPD_SCANF_RET_VALUE(ret_value, format...)\
{\
    ret_value = scanf_s(format);\
    if (ret_value == UPD_ERR)\
    {\
        UPD_LOG(ERR_LOG, "scanf_s fail.");\
    }\
}
#endif // UPD_CHAPTER("safe function with return value")

#if UPD_CHAPTER("safe function without return value")
#define UPD_MEMSET(dest, dest_max, value, count)\
{\
    if (memset_s(dest, dest_max, value, count) != UPD_OK)\
    {\
        UPD_LOG(ERR_LOG, "memset_s fail, dest %s, dest_max %d, value '%c', count %d.", dest, dest_max, value, count);\
    }\
}

#define UPD_MEMCPY(dest, dest_max, src, count)\
{\
    if (memcpy_s(dest, dest_max, src, count) != UPD_OK)\
    {\
        UPD_LOG(ERR_LOG, "memcpy_s fail, dest %s, dest_max %d, src %s, count %d.", dest, dest_max, src, count);\
    }\
}

#define UPD_MEMMOVE(dest, dest_max, src, count)\
{\
    if (memmove_s(dest, dest_max, src, count) != UPD_OK)\
    {\
        UPD_LOG(ERR_LOG, "memmove_s fail, dest %s, dest_max %d, src %s, count %d.", dest, dest_max, src, count);\
    }\
}

#define UPD_STRCPY(dest_str, dest_max, src_str)\
{\
    if (strcpy_s(dest_str, dest_max, src_str) != UPD_OK)\
    {\
        UPD_LOG(ERR_LOG, "strcpy_s fail, dest_str %s, dest_max %d, src_str %s.", dest_str, dest_max, src_str);\
    }\
}

#define UPD_STRNCPY(dest_str, dest_max, src_str, count)\
{\
    if (strncpy_s(dest_str, dest_max, src_str, count) != UPD_OK)\
    {\
        UPD_LOG(ERR_LOG, "strncpy_s fail, dest_str %s, dest_max %d, src_str %s, count %d.", dest_str, dest_max, src_str, count);\
    }\
}

#define UPD_STRCAT(dest_str, dest_max, src_str)\
{\
    if (strcat_s(dest_str, dest_max, src_str) != UPD_OK)\
    {\
        UPD_LOG(ERR_LOG, "strcat_s fail, dest_str %s, dest_max %d, src_str %s.", dest_str, dest_max, src_str);\
    }\
}

#define UPD_STRNCAT(dest_str, dest_max, src_str, count)\
{\
    if (strncat_s(dest_str, dest_max, src_str, count) != UPD_OK)\
    {\
        UPD_LOG(ERR_LOG, "strncat_s fail, dest_str %s, dest_max %d, src_str %s, count %d.", dest_str, dest_max, src_str, count);\
    }\
}

#define UPD_SPRINTF(dest_str, dest_max, format...)\
{\
    if (sprintf_s(dest_str, dest_max, format) == UPD_ERR)\
    {\
        UPD_LOG(ERR_LOG, "sprintf_s fail, dest_str %s, dest_max %d.", dest_str, dest_max);\
    }\
}

#define UPD_SNPRINTF(dest_str, dest_max, count, format...)\
{\
    if (snprintf_s(dest_str, dest_max, count, format) == UPD_ERR)\
    {\
        UPD_LOG(ERR_LOG, "snprintf_s fail, dest_str %s, dest_max %d, count %d.", dest_str, dest_max, count);\
    }\
}

#define UPD_VSPRINTF(dest_str, dest_max, format, arg_list)\
{\
    if (vsprintf_s(dest_str, dest_max, format, arg_list) == UPD_ERR)\
    {\
        UPD_LOG(ERR_LOG, "vsprintf_s fail, dest_str %s, dest_max %d.", dest_str, dest_max);\
    }\
}

#define UPD_VSNPRINTF(dest_str, dest_max, count, format, arg_list)\
{\
    if (vsnprintf_s(dest_str, dest_max, count, format, arg_list) == UPD_ERR)\
    {\
        UPD_LOG(ERR_LOG, "vsnprintf_s fail, dest_str %s, dest_max %d, count %d.", dest_str, dest_max, count);\
    }\
}

#define UPD_SCANF(format...)\
{\
    if (scanf_s(format) == UPD_ERR)\
    {\
        UPD_LOG(ERR_LOG, "scanf_s fail.");\
    }\
}
#endif // UPD_CHAPTER("safe function without return value")

#if UPD_CHAPTER("UPD_ZERO") // only suggest

// UPD_ZERO_OBJ:    for struct or variable object
// UPD_ZERO_ARRAY:  only for array
// UPD_ZERO_BY_LEN: for pointer with known length
#define UPD_ZERO_OBJ(obj)\
        memset_s(&(obj), sizeof(obj), 0, sizeof(obj))

#define UPD_ZERO_ARRAY(array)\
        memset_s((array), sizeof(array), 0, sizeof(array))

#define UPD_ZERO_BY_LEN(ptr, len)\
        memset_s((ptr), (len), 0, (len))
#endif // UPD_CHAPTER("UPD_ZERO")

#define UPD_LOG(log_type, format, args...) do {recovery_print((char*)"%s:%s,line=%d:" format"\n", log_type, __func__, __LINE__, ##args);} while(0)

int recovery_print(char* fmt, ...)
{
    time_t now;
    char datetime[BUFF_SIZE_1024] = {'\0'};
    int ret = 0;
    va_list ap;
    struct timeval t;
    gettimeofday(&t, NULL);
    struct tm *ptm = localtime(&t.tv_sec);

    (void)memset_s( &ap , sizeof( va_list ), 0 , sizeof( va_list ) ) ;
    if( NULL == ptm )
    {
        printf( "%s,line:%d,NULL == ptm\n" , __func__ , __LINE__ ) ;
        return ( ret ) ;
    }
    (void)snprintf_s(datetime , sizeof( datetime ) , sizeof( datetime )-1,  "[%.4d-%.2d-%.2d %.2d:%.2d:%.2d %.3d]" ,
                        ptm->tm_year+1900 ,     // year
                        ptm->tm_mon+1,          // month
                        ptm->tm_mday,           // date
                        ptm->tm_hour,           // hour
                        ptm->tm_min,            // minute
                        ptm->tm_sec,            // second
                        (int)t.tv_usec/1000) ;  // mili second
    printf("%s ", datetime);
    va_start(ap, fmt);
    ret = vfprintf(stdout, fmt, ap);
    va_end(ap);
    return (ret);
}


#ifdef __cplusplus
}
#endif

#endif
