#include <windows.h>    ///< the best library
#include <tchar.h>      ///< для макроса _T и _TEXT

/// --- CXX library --------------------------------
#include <iostream>
#include <sstream>
#include <string>
/// --- CXX library --------------------------------

/// Конфигурационный файл
#include "config/config.h"

#define IDC_LABEL1  1025        ///< ID label
HWND MSLabel;


LRESULT CALLBACK main_win_proc (HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI thread_func ();

/**
 * @brief Основная функция
 * @param hInstance дескриптор текущего экземпляра приложения
 * @param nWinMode  управляет отображением окна
 * @return 0
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nWinMode) {
    FreeConsole();          ///< Избавляемся от консоли
    WNDCLASS wcl;

    /// #define _T(x)       __T(x)
    /// #define _TEXT(x)    __T(x)
    /// _T и _TEXT  __  аналогичны
    wcl.lpszClassName   =   _T(URVS_RGZ23_PROJECT_NAME);        ///< имя класса окна
    wcl.lpfnWndProc     =   main_win_proc;                      ///< указатель на процедуру окна
    wcl.hInstance       =   hInstance;                          ///< дескриптор экземпляра, содержащего процедуру окна для класса
    wcl.lpszMenuName    =   NULL;                               ///< имя ресурса меню класса
    wcl.hIcon           =   LoadIcon(NULL, IDI_APPLICATION);    ///< TODO: Добавить иконку
    wcl.hCursor         =   LoadCursor(NULL, IDC_ARROW);        ///< маркер курсора класса
    wcl.cbClsExtra      =   0;
    wcl.cbWndExtra      =   0;

    wcl.hbrBackground   =   reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1); ///< Маркер фоновой кисти класса
    wcl.style           =   CS_HREDRAW | CS_VREDRAW;                    ///< стиль класса
    /// CS_VREDRAW | CS_HREDRAW :
    /// осуществляет перерисовку окна при перемещении или изменении высоты и ширины окна

    RegisterClass(&wcl);

    HDC hDCScreen = GetDC(NULL);        ///< Извлекает дескриптор контекста дисплея
    int _app_width  = 320;              ///< Ширина application
    int _app_height = 180;              ///< Высота application

    /// Создаем главное окно и отображание его
    HWND hwnd = CreateWindow(
        _T(URVS_RGZ23_PROJECT_NAME),                                ///< указатель на зарегистрированное имя класса
        _T("RGZ23: current minute"),                                ///< указатель на имя окна
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,   ///< стиль окна
        (GetDeviceCaps(hDCScreen, HORZRES) - _app_width ) / 2,      ///< горизонтальная позиция окна
        (GetDeviceCaps(hDCScreen, VERTRES) - _app_height) / 2,      ///< вертикальная позиция окна
        _app_width,                                                 ///< ширина окна
        _app_height,                                                ///< высота окна
        NULL,                                                       ///< дескриптор родительского или окна владельца
        NULL,                                                       ///< дескриптор меню или ID дочернего окна
        hInstance,                                                  ///< дескриптор экземпляра приложения
        NULL);                                                      ///< указатель на данные создания окна

    ShowWindow(hwnd, nWinMode);
    UpdateWindow(hwnd);

    MSG msg;                                    ///< Сообщение
    while (GetMessage(&msg, NULL, 0, 0)) {      ///< Цикл обработки событий
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return static_cast<int>(msg.wParam);
}

/**
 * @brief      Функция обработки сообщений для главного окна
 * @param _hw  Дескриптор процедуры окна, которая получила сообщение
 * @param _msg Сообщение
 * @param _wp  Дополнительная информация о сообщении
 * @param _lp  Дополнительная информация о сообщении
 * @return     Результат обработки сообщения и зависит от сообщения
 */
LRESULT CALLBACK main_win_proc (HWND _hw, UINT _msg, WPARAM _wp, LPARAM _lp) {
    switch (_msg) {
        case WM_CREATE:             /// Отправляется, когда приложение запрашивает создание окна
        {
            RECT rt;
            GetClientRect(_hw, &rt);
            int static_width  = rt.right;
            int static_height = rt.bottom;
            int _border_textbox = 2;

            HINSTANCE hIns = reinterpret_cast<LPCREATESTRUCT>(_lp)->hInstance;
            /// STATIC - обозначает простое текстовое поле, окно или прямоугольник, используемый для надписей
            /// BUTTON - обозначает маленькое прямоугольное дочернее окно, которое представляет собой кнопку
            /// EDIT   - обозначает прямоугольное дочернее окно, внутри которого пользователь может напечатать текст с клавиатуры
            MSLabel = CreateWindow(
                _T("static"),                           ///< указатель на зарегистрированное имя класса
                _T("label1"),                           ///< указатель на имя окна
                WS_CHILD | WS_VISIBLE,                  ///< стиль окна
                _border_textbox,                        ///< горизонтальная позиция окна
                _border_textbox,                        ///< вертикальная позиция окна
                static_width  - 2 * _border_textbox,    ///< ширина окна
                static_height - 2 * _border_textbox,    ///< высота окна
                _hw,                                    ///< дескриптор родительского окна
                reinterpret_cast<HMENU>(IDC_LABEL1),    ///< ID дочернего окна
                hIns,                                   ///< дескриптор экземпляра приложения
                NULL);                                  ///< указатель на данные создания окна

            HFONT font_mono = reinterpret_cast<HFONT>(GetStockObject(OEM_FIXED_FONT));    /// Меняем шрифт на моноширный
            SendDlgItemMessage(_hw, IDC_LABEL1, WM_SETFONT, reinterpret_cast<WPARAM>(font_mono), TRUE);

            DWORD IDThread;
            /// Поток с вычисление текущей минуты
            HANDLE hThread = CreateThread(
                NULL,                                                   ///< дескриптор защиты
                0,                                                      ///< начальный размер стека
                reinterpret_cast<LPTHREAD_START_ROUTINE>(thread_func),  ///< функция потока
                NULL,                                                   ///< параметр потока
                0,                                                      ///< опции создания
                &IDThread);                                             ///< идентификатор потока

            /// Закрываем дескриптор открытого объекта
            CloseHandle(hThread);
            return 0;
        }
        case WM_CTLCOLORSTATIC:     /// Статический элемент управления
        {
            DWORD CtrlID = GetDlgCtrlID(reinterpret_cast<HWND>(_lp));
            HDC hdcStatic = reinterpret_cast<HDC>(_wp);
            if (CtrlID == IDC_LABEL1) {
                SetTextColor(hdcStatic, RGB(30, 200, 30));
                SetBkColor(hdcStatic, BLACK_BRUSH);
                return reinterpret_cast<LRESULT>(CreateSolidBrush(BLACK_BRUSH));
            }
            return 0;
        }
        case WM_DESTROY:            /// Отправляется при разрушении окна
        {
            PostQuitMessage(0);     ///< Пользователь закрыл окно
            return 0;               ///< Программа может завершаться
        }

        default: return DefWindowProc(_hw, _msg, _wp, _lp);
    }
}

/**
 * @brief Функция, запускаемая в рамках создаваемого потока
 *
 */
DWORD WINAPI thread_func () {
    /// Дескриптор экземпляра динамической библиотеки
    HINSTANCE hinstLib = LoadLibrary(_T("time_info.dll"));

    /// Если мы получим дескриптор
    if (hinstLib != NULL) {
        /// Поток для входного текста
        std::ostringstream _ostream;
        _ostream << "PM-92 | Glushko Vladislav | rgz-23" << '\n';
        typedef size_t (*get_time_minute_)();
        get_time_minute_ ifconnected =
            reinterpret_cast<get_time_minute_>(GetProcAddress(hinstLib, "get_time_minute"));

        if(ifconnected != NULL) {
            std::size_t _m = ifconnected();
            _ostream << "current minute: " << _m << '\n';
        } else {
            std::ostringstream _ostream_err;
            _ostream_err << "get_time_minute() - not found in time_info.dll" << '\n';

            /// Устанавливаем текст
            SetWindowTextA(MSLabel, _ostream_err.str().c_str());
            constexpr unsigned long _err_not_func = { 3 };
            return _err_not_func;
        }
        SetWindowTextA(MSLabel, _ostream.str().c_str());

        /// Освобождаем загруженный модуль
        FreeLibrary(hinstLib);
    } else {
        std::ostringstream _ostream_err;
        _ostream_err << "failed to load time_info.dll" << '\n';

        SetWindowTextA(MSLabel, _ostream_err.str().c_str());
        constexpr unsigned long _err_import_dll = { 2 };
        return _err_import_dll;
    }
    return 0;
}