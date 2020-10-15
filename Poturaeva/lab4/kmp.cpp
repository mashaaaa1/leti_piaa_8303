#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
using namespace std;


void printInterInfo(std::string& splice, std::vector<size_t> vect, int n) {//вывод информации о префикс функции строки
    std::cout << "\nState of prefix function:  " << std::endl;
    for (int i = 0; i <= n; i++)
        std::cout << splice[i] << " ";//вывод символов строки
    std::cout << std::endl;
    for (int i = 0; i <= n; i++)
        std::cout << vect[i] << " ";//вывод значений префикс-функции
    std::cout << std::endl<<std::endl;
}


void prefixFunction(std::string& p, std::vector<size_t>& pi) {
    std::cout << "Start prefix function:" << std::endl;
    int n = 1;
    size_t j = 0;
    pi[0] = 0;

    for (size_t i = 1; i < p.size();)//проходим по склееной строке
    {
        // поиск какой префикс-суффикс можно расширить
        if (p[j] == p[i]) {//если можно увеличить
           
            pi[i] = j + 1;
            j++;
            i++;
            std::cout << "p[i] == p[j],  j++, i++,  pi[i] = " << j << " ,i=" << i;
            printInterInfo(p, pi, n);
            n++;
        }
        else {//если нельзя
            if (j == 0) {//это первый символ
                pi[i] = 0;
                i++;
                std::cout << "p[i] != p[j],  prefix[i] = 0, i=1, k=0";
                printInterInfo(p, pi, n);
                n++;

            }
            else {//если не первый символ
                j = pi[j - 1];
                std::cout << "p[i] != p[j], i= "<<i<<", j= "<<j;
                printInterInfo(p, pi, n);
            }
        }
        

        }


}

void KMP(std::string& t, std::string& p, size_t flowCount) {//функция для поиска шаблонов в тексте

    vector<size_t> res;//результат
    vector<size_t> pi(p.size());//значения префикс функции
    prefixFunction(p, pi);//считаем префикс функцию
    cout << endl;

    size_t j, start, end;
    if (t.length() / flowCount < p.length())
        flowCount = t.length() / p.length();//максимальное количесвто потоков

    for (size_t f = 0; f < flowCount; f++) {//обрабатываем текст по потокам

        cout << "Flow " << f + 1 << ": ";
        j = 0;
        size_t r = t.length() % flowCount;
        size_t flowLen = t.length() / flowCount;//длина потока
        if (r > 0)
            flowLen++;
        start = f * flowLen;//выделяем начальную позицию для текущего потока
        end = start + flowLen;//выделяем конечную позицию для текущего потока
        end += p.length() - 1;//прибавляем шаблон
        bool isFound = false;
        if (end > t.size())//если больше текста
            end = t.size();
        for (size_t s = start; s < end; s++)//вывод потока
            cout << t[s];
        cout << endl << endl;

        for (size_t i = start; i < end;) {//проходим по потоку
            for (size_t k = start; k < i; k++) {
                cout << t[k];
            }
            cout << " (" << t[i] << ")";
            for (size_t k = i + 1; k < end; k++) {
                cout << t[k];
            }
            cout << endl;
            string shift = "";
            for (size_t k = 0; k < i - start - j + 1; k++) {
                shift += " ";
            }


            cout << shift;
            for (size_t k = 0; k < p.size(); k++) {
                if (k == j) {
                    cout << "(" << p[k] << ")";
                }
                else cout << p[k];
            }

            if (t[i] == p[j]) {//если нашли одинаковые символы
                cout << endl << endl;
                i++;
                j++;
                if (j == p.size()) {//нашли вхождение
                    cout << "Occurrence of the sample" << endl;
                    cout << "Index: ";
                    cout << i - p.size() << endl << endl;
                    res.push_back(i - p.size());//помещаем в результат
                    isFound = true;
                    j = pi[j - 1];//откатываемся
                }
            }
            else {//если символы не совпали
                cout << " - symbols don't match." << endl << endl;
                if (j == 0) {//если первый символ
                    i++;
                }
                else {//если не первый
                    j = pi[j - 1];//откат
                }
            }
        }
        cout << endl;
    }
    if (res.empty()) {
        cout << "Not found" << endl;
        cout << -1 << endl;
        return;
    }

    cout << "\nResult:" << endl;
    string sep = "";
    for (auto& el : res) {
        cout << sep << el;
        sep = ",";
    }
    cout << endl;

}


// в начале вводится шаблон, затем
// текст и число "потоков"
int main()
{
    string p, t;
    size_t flowCount;
    
    cin >> p;//шаблон
    cin >> t;//текст
    cin >> flowCount;//потоки
    KMP(t, p, flowCount);
    return 0;
}
