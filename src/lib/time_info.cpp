#include <windows.h>

/**
 * @brief  Функция определеят текущую минуту
 * @return Возвращает положительно число
 */
extern "C" __declspec(dllexport) size_t get_time_minute () {
    SYSTEMTIME _cur_time;
    GetSystemTime(&_cur_time);
    return _cur_time.wMinute;
}