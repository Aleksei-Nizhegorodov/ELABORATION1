#include <iostream> 
#include <fstream> 
#include <string.h> 
#include <stdio.h> 
#include <vector> 
#include <iomanip>
#include <fixed/fixed.h>
#include <pde_solvers/pde_solvers.h>
using namespace std;


/// @brief иницилизация класса PipeCalculation 
class PipeCalculation {
/// @brief 
private:
    /// @brief структура данных о пременных трубопровода
    struct PipeData {
        double L; //Длина трубопровода
        double speed; //Средняя скорость движения партий
        int num_segments; //Число разбиений трубопровода

        PipeData() {
            L = 200;
            speed = 50;
            num_segments = 9;
        }
    };

    /// @brief структура данных о пременных в трубопроводе
    struct CalcData {
        int T;    //T - период моделирования  
        double ro_n; // Начальная плотность нефти в трубе 
        double sera_n; // Начальное содержание серы в нефти
        int num_layers; //Кол-во слоев
        double dt, dx; // Шаги по времени, координате

        CalcData(PipeData& iniPipe) {
            T = 5;
            ro_n = 850;
            sera_n = 0.1;
            dx = iniPipe.L / (iniPipe.num_segments - 1);
            dt = dx / iniPipe.speed;
            num_layers = (T / dt);
        }
    };

/// @brief 
public:
    PipeCalculation() {}

    /// @brief ф-ция вывода данных расчета
    /// @param iniCalc ссылка на данные в структуре CalcData
    /// @param current_layer текущего слоя 
    void output_fun(CalcData iniCalc, vector<vector<double>>& current_layer, int i) {
    
        if (i == 0) {
            ofstream outFile("output.csv");
            outFile << "время,координиата,плотность, сера" << "\n";
            // Записать значения текущего слоя в файл
            for (size_t j = 0; j < current_layer.size(); j++) {
                for (size_t k = 0; k < current_layer[j].size(); k++) {
                    outFile << i * iniCalc.dt << "," << j * iniCalc.dx << "," << current_layer[j][k] << "\n";
                }
            }
            outFile.close();
        }
        else {
            ofstream outFile("output.csv", ios::app);
            // Записать значения текущего слоя в файл
            for (size_t j = 0; j < current_layer.size(); j++) {
                for (size_t k = 0; k < current_layer[j].size(); k++) {
                    outFile << i * iniCalc.dt << "," << j * iniCalc.dx << "," << current_layer[j][k] << "\n";
                }
            }
            outFile.close();
        }
    }
/// @brief отдельная функция для метода характеристик
/// @param iniPipe объявление переменной iniPipe (Данные о параметрах трубопровода)
/// @param argument  нач.значения
/// @param current_layer вектор текущий слой 
/// @param previous_layer вектор предыущий слой
    void calculation(PipeData iniPipe, double argument, vector<double>& current_layer, vector<double>& previous_layer) {
    // Метод характеристик
    for (size_t i = 1; i < current_layer.size(); i++)
    {
        current_layer[i] = previous_layer[i - 1];
    }
    current_layer[0] = argument;
}
    /// @brief 
    void run() {
        setlocale(LC_ALL, "Russian");

        PipeData iniPipe;
        CalcData iniCalc(iniPipe);

        vector<double> ro_in(iniCalc.num_layers);
        ro_in = { 850, 870, 860, 860, 860,860, 860, 860, 860, 870 };

        vector<double> sera_in(iniCalc.num_layers);
        sera_in = { 0.01, 0.02, 0.03, 0.04, 0.05,0.06, 0.07, 0.08, 0.09, 0.01 };

        vector<double> double_array(iniPipe.num_segments);
        ring_buffer_t<vector<vector<double>>> buffer(2, { double_array, double_array });

        for (size_t i = 0; i < iniCalc.num_layers; i++) {
            for (size_t j = 0; j < buffer.current().size(); j++) {
                calculation(iniPipe, ro_in[i], buffer.current()[j], buffer.previous()[j]);
                calculation(iniPipe, sera_in[i], buffer.current()[j], buffer.previous()[j]);
            }
            buffer.advance(1);
            output_fun(iniCalc, buffer.current(), i);
        }
    }
};
///
/// @brief 
/// @return 
int main() {
    PipeCalculation pc;
    pc.run();
    return 0;
}