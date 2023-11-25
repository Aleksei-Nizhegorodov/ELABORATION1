#include <iostream> 
#include <fstream> 
#include <string.h> 
#include <stdio.h> 
#include <vector> 
using namespace std;

struct pipe {
    double L;
    double V;
    int n;    /// Число разбиений трубопровода
    int T;    ///T - период моделирования  
    double ro_n;  // Начальная плотность нефти в трубе 
    double sera_n; // Начальное содержание серы в нефти
    
};

int main()
{
    pipe one = { 200, 50, 9, 5, 850, 0.1 };


    /// Корректный вывод руского текста
    setlocale(LC_ALL, "Russian");

    double dx = one.L / (one.n - 1);
    double dt = dx / one.V;     //условие Куранта 
    int N = (one.T / dt);  // Число слоев
    
  

    // Открываем файл для записи
    std::ofstream Overview("output.csv");
    // Проверяем, открылся ли файл
    if (!Overview.is_open()) {
        std::cerr << "Не удалось открыть файл." << std::endl;
        return 1;
    }
    ///// Записываем заголовки колонок
    Overview << "Время,Координата,Плотность,Содержание серы" << std::endl;

    vector<double> ro_in(N); // Вектор плотностей нефти входных партий
    ro_in = { 850, 870, 860, 860, 860,860, 860, 860, 860, 860 };

    vector <double> ro_0(one.n); // Начальный слой по плотности
    vector<double> ro_1(one.n);   // Следующий слой по плотности


    vector<double> sera_in(N);//// Начальный слой по сере
    sera_in = { 0.01, 0.02, 0.03, 0.04, 0.05,0.06, 0.07, 0.08, 0.09, 0.01 };

    vector <double> sera_0(one.n); // Начальный слой по сере
    vector<double> sera_1(one.n); // Следующий слой по сере
     
    // Предполагаем, что в начальный момент времени всю трубу заполняют  нефть с плотностью ro_n
    for (int k = 0; k < one.n; k++) {
        ro_0[k] = one.ro_n;
        sera_0[k] = one.sera_n;
        cout << ro_0[k] << endl;
        cout << sera_0[k] << endl;
        Overview << 0 << "," << dx * k << "," << one.ro_n << "," << one.sera_n << endl;
    }
    // Рассчёт слоев 
    for (int j = 0; j < N; j++) {
        //cout << "Слой " << j + 1 << endl;
        ro_1[0] = ro_in[j];
        sera_1[0] = sera_in[j];
        cout << ro_1[0] << endl;
        cout << sera_1[0] << endl;
        // Запись  в файл
        Overview << dt * (j + 1) << "," << 0 << "," << ro_1[0] << "," << sera_1[0] << endl;

        // Слудующий слой
        for (int i = 1; i < one.n; i++) {
            ro_1[i] = ro_0[i - 1];
            sera_1[i] = sera_0[i - 1];
            Overview << dt * (j + 1) << "," << dx * i << "," << ro_1[i] << "," << sera_1[i] <<  endl;
        }
        ro_0 = ro_1;
        sera_0 = sera_1;
       
    }
}