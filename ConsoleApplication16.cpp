
#include <iostream> 
#include <fstream> 
#include <string.h> 
#include <stdio.h> 
#include <vector> 
using namespace std;

/// @brief Структура данных параметров трубопровода
struct pipe_data {
    double L; //Длина трубопровода
    double speed; //Средняя скрость движения партий
    int num_segmetations;    //Число разбиений трубопровода
     
};

/// @brief Структура данных для расчета 
struct calc_data {
    int T;    //T - период моделирования  
    double ro_n;  // Начальная плотность нефти в трубе 
    double sera_n; // Начальное содержание серы в нефти
    int num_layers; //Кол-во слоев
    double dt, dx; // Шаги по времени, координате
};

void Pipe(pipe_data& iniPipe) {
    iniPipe.L = 200;
    iniPipe.speed = 50;
    iniPipe.num_segmetations = 9;

    cout << "Длина трубопровода:                        " << iniPipe.L << endl;
    cout << "Значение средней скрости движения партий:  " << iniPipe.speed << endl;
    cout << "Число разбеений трубопровода:              " << iniPipe.num_segmetations << endl;

};

void Calc(calc_data& iniCalc, pipe_data& iniPipe) {
    iniCalc.T = 5;
    iniCalc.ro_n = 850;
    iniCalc.sera_n = 0.1;

    iniCalc.num_layers = (iniCalc.T / iniCalc.dt);
    iniCalc.dx = iniPipe.L / (iniPipe.num_segmetations - 1);
    iniCalc.dt = iniCalc.dx / iniPipe.speed;


    cout << "Время моделирования:                      " << iniCalc.T << endl;
    cout << "Значение плотности в начале:              " << iniCalc.ro_n << endl;
    cout << "Значение серы в начале:                   " << iniCalc.sera_n << endl;
    cout << "Число слоев:                              " << iniCalc.num_layers << endl;
    cout << "Шаг по координате:                        " << iniCalc.ro_n << endl;
    cout << "Шаг по времени:                           " << iniCalc.sera_n << endl;


};

int main()
{
    setlocale(LC_ALL, "Russian"); // Корректный вывод руского текста

    std::ofstream Overview("output.csv");// Открываем файл для записи пармаетров расчета

    // Проверяем, открылся ли файл
    if (!Overview.is_open()) {
        std::cerr << "Не удалось открыть файл." << std::endl;
        return 1;
    }

    Overview << "Время,Координата,Плотность,Содержание серы" << std::endl; // Записываем заголовки колонок
    
    pipe_data iniPipe; //объявление переменной iniPipe (хранение данных о трубопроводе) типа pipe_data
    calc_data iniCalc;  //объявление переменной iniCalc типа calc_data

    Pipe(iniPipe); // это вызов функции Pipe с аргументом iniPipe. Функция Pipe принимает ссылку на структуру pipe_data и использует эту ссылку для изменения значений полей структуры
    Calc(iniCalc, iniPipe); //это вызов функции Calc с аргументом iniCalc и iniPipe.Функция Calc принимает ссылку на структуру calc_data и использует эту ссылку для изменения значений полей структуры


    vector<double> ro_in(iniCalc.num_layers); // Вектор плотностей нефти входных партий
    ro_in = { 850, 870, 860, 860, 860,860, 860, 860, 860, 860 };

    vector <double> ro_0(iniPipe.num_segmetations); // Начальный слой по плотности
    vector<double> ro_1(iniPipe.num_segmetations);   // Следующий слой по плотности


    vector<double> sera_in(iniCalc.num_layers); // Вектор содержания серы в нефти входных партий
    sera_in = { 0.01, 0.02, 0.03, 0.04, 0.05,0.06, 0.07, 0.08, 0.09, 0.01 };

    vector <double> sera_0(iniPipe.num_segmetations); // Начальный слой по сере
    vector<double> sera_1(iniPipe.num_segmetations); // Следующий слой по сере

    // Предполагаем, что в начальный момент времени всю трубу заполняют  нефть с плотностью ro_n
    for (int k = 0; k < iniPipe.num_segmetations; k++) {
        ro_0[k] = iniCalc.ro_n;
        sera_0[k] = iniCalc.sera_n;
        cout << ro_0[k] << endl;
        cout << sera_0[k] << endl;
        Overview << 0 << "," << iniCalc.dx * k << "," << iniCalc.ro_n << "," << iniCalc.sera_n << endl;
    }
    // Рассчёт слоев 
    for (int j = 0; j < iniCalc.num_layers; j++) {
        //cout << "Слой " << j + 1 << endl;
        ro_1[0] = ro_in[j];
        sera_1[0] = sera_in[j];
        cout << ro_1[0] << endl;
        cout << sera_1[0] << endl;


        Overview << iniCalc.dt * (j + 1) << "," << 0 << "," << ro_1[0] << "," << sera_1[0] << endl; // Запись  в файл

        // Слудующий слой
        for (int i = 1; i < iniPipe.num_segmetations; i++) {
            ro_1[i] = ro_0[i - 1];
            sera_1[i] = sera_0[i - 1];
            Overview << iniCalc.dt * (j + 1) << "," << iniCalc.dx * i << "," << ro_1[i] << "," << sera_1[i] << endl;
        }
        ro_0 = ro_1;
        sera_0 = sera_1;
    };
};