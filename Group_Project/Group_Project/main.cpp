#include <iostream>
#include "station_query.h"
#include "menu.h"

#ifdef _WIN32
#include <windows.h>
#include <clocale>
#include <io.h>     
#include <fcntl.h>  
#endif

int main() {

#ifdef _WIN32
    // 把控制台输入,输出的代码页都锁定为UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    //让C++标准库也锁定UTF-8编码
    std::setlocale(LC_ALL, ".UTF8");


#endif

    try {
        StationQuery stationQuery("Data/Station.csv",
            "Data/Edge.csv");

        MenuSystem menu(stationQuery);
        menu.run();
    }
    catch (const std::exception& e) {
        std::cerr << "程序异常: " << e.what() << std::endl;
    }

    return 0;
}