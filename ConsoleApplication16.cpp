
#include <iostream> 
#include <fstream> 
#include <string.h> 
#include <stdio.h> 
#include <vector> 
using namespace std;
/// @brief struct of pipe one 
struct pipe_data {
    double L;
    double V;
    int num_segmetations;    //Число разбиений трубопровода
   
    
};
/// @brief struct of calculation pipe data
struct calc_data {
    int T;    //T - период моделирования  
    double ro_n;  // Начальная плотность нефти в трубе 
    double sera_n; // Начальное содержание серы в нефти
};

int main()
{
    pipe_data pipe_info = { 200, 50, 9 };
    calc_data calc_info = {5, 850, 0.1 };


    /// Корректный вывод руского текста
    setlocale(LC_ALL, "Russian");

    double dx = pipe_info.L / (pipe_info.num_segmetations - 1);
    double dt = dx / pipe_info.V;     //условие Куранта 
    int num_layers = (calc_info.T / dt);  // Число слоев
    
  

    // Открываем файл для записи
    std::ofstream Overview("output.csv");
    // Проверяем, открылся ли файл
    if (!Overview.is_open()) {
        std::cerr << "Не удалось открыть файл." << std::endl;
        return 1;
    }
    ///// Записываем заголовки колонок
    Overview << "Время,Координата,Плотность,Содержание серы" << std::endl;

    vector<double> ro_in(num_layers); // Вектор плотностей нефти входных партий
    ro_in = { 850, 870, 860, 860, 860,860, 860, 860, 860, 860 };

    vector <double> ro_0(pipe_info.num_segmetations); // Начальный слой по плотности
    vector<double> ro_1(pipe_info.num_segmetations);   // Следующий слой по плотности


    vector<double> sera_in(num_layers);//// Начальный слой по сере
    sera_in = { 0.01, 0.02, 0.03, 0.04, 0.05,0.06, 0.07, 0.08, 0.09, 0.01 };

    vector <double> sera_0(pipe_info.num_segmetations); // Начальный слой по сере
    vector<double> sera_1(pipe_info.num_segmetations); // Следующий слой по сере
     
    // Предполагаем, что в начальный момент времени всю трубу заполняют  нефть с плотностью ro_n
    for (int k = 0; k < pipe_info.num_segmetations; k++) {
        ro_0[k] = calc_info.ro_n;
        sera_0[k] =  calc_info.sera_n;
        cout << ro_0[k] << endl;
        cout << sera_0[k] << endl;
        Overview << 0 << "," << dx * k << "," << calc_info.ro_n << "," << calc_info.sera_n << endl;
    }
    // Рассчёт слоев 
    for (int j = 0; j < num_layers; j++) {
        //cout << "Слой " << j + 1 << endl;
        ro_1[0] = ro_in[j];
        sera_1[0] = sera_in[j];
        cout << ro_1[0] << endl;
        cout << sera_1[0] << endl;
        // Запись  в файл
        Overview << dt * (j + 1) << "," << 0 << "," << ro_1[0] << "," << sera_1[0] << endl;

        // Слудующий слой
        for (int i = 1; i < pipe_info.num_segmetations; i++) {
            ro_1[i] = ro_0[i - 1];
            sera_1[i] = sera_0[i - 1];
            Overview << dt * (j + 1) << "," << dx * i << "," << ro_1[i] << "," << sera_1[i] <<  endl;
        }
        ro_0 = ro_1;
        sera_0 = sera_1;
       
    }
}