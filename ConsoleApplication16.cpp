#include <iostream> 
#include <fstream> 
#include <string.h> 
#include <stdio.h> 
#include <vector> 
#include <iomanip>
#include <fixed/fixed.h>
#include <pde_solvers/pde_solvers.h>
using namespace std;

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


/// @brief Ф-ция вывода значений расчета в .csv формат
/// @param iniCalc предача значений из структуры CalcData
/// @param current_layer ссылка на текущий слой
void output_fun(const CalcData& iniCalc, const vector<vector<double>>& current_layer, int i) {
    if (i == 0) {
        ofstream outFile("output.csv");
        outFile << "время,координиата,плотность, сера" << "\n";
        for (size_t j = 0; j < current_layer.size(); j++) {
            for (size_t k = 0; k < current_layer[j].size(); k++) {
                outFile << i * iniCalc.dt << "," << j * iniCalc.dx << "," << current_layer[j][k] << "\n";
            }
        }
        outFile.close();
    }
    else {
        ofstream outFile("output.csv", ios::app);
        for (size_t j = 0; j < current_layer.size(); j++) {
            for (size_t k = 0; k < current_layer[j].size(); k++) {
                outFile << i * iniCalc.dt << "," << j * iniCalc.dx << "," << current_layer[j][k] << "\n";
            }
        }
        outFile.close();
    }
}

/// @brief иницилизация класса PipeCalculation (реализация решения методом характеристик)
class PipeCalculation {

/// @brief Передача экзепляров структур
private:
    PipeData pipe_data_;
    CalcData calc_data_;
    vector<double> ro_profile_;
    vector<double> sera_profile_;

public:
    PipeCalculation(const PipeData& pd, const CalcData& cd, const vector<double>& ro_prof, const vector<double>& sera_prof)
        : pipe_data_(pd),
        calc_data_(cd),
        ro_profile_(ro_prof),
        sera_profile_(sera_prof) {}


/// @brief отдельная функция для метода характеристик
/// @param iniPipe объявление переменной iniPipe (Данные о параметрах трубопровода)
/// @param argument  нач.значения
/// @param current_layer вектор текущий слой 
/// @param previous_layer вектор предыущий слой
    void calculation(PipeData iniPipe, double argument, vector<double>& current_layer, vector<double>& previous_layer) {
        //метод хар-к
        for (size_t i = 1; i < current_layer.size(); i++)
            current_layer[i] = previous_layer[i - 1];
        current_layer[0] = argument;
    }

    /// @brief ф-ция расчета методом характеристик с использованием buffer
    void characteristic_method() {
        setlocale(LC_ALL, "Russian");

        PipeData iniPipe;
        CalcData iniCalc(iniPipe);

        vector<double> double_array(pipe_data_.num_segments);
        ring_buffer_t<vector<vector<double>>> buffer(2, { double_array, double_array });

        for (size_t i = 0; i < calc_data_.num_layers; i++) {
            for (size_t j = 0; j < buffer.current().size(); j++) {
                calculation(pipe_data_, ro_profile_[i], buffer.current()[j], buffer.previous()[j]);
                calculation(pipe_data_, sera_profile_[i], buffer.current()[j], buffer.previous()[j]);
            }
            buffer.advance(1);
            output_fun(calc_data_, buffer.current(), i);
        }
    }
};

/// @brief ф-ция мейн
/// @return 
int main() {
    PipeData iniPipe;
    CalcData iniCalc(iniPipe);

    // Профили входящих значений
    vector<double> roProfile = { 850,   870,   860,   860,   860,   860,   860,   860,   860,   870 };
    vector<double> seraProfile = { 0.01,   0.02,   0.03,   0.04,   0.05,   0.06,   0.07,   0.08,   0.09,   0.01 };

    PipeCalculation pc(iniPipe, iniCalc, roProfile, seraProfile);
    pc.characteristic_method();
    return  0;
}