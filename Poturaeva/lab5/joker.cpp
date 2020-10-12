#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>



class Vertex {
public:
    std::map<char, int> sons; //для бора
    std::map<char, int> go; //для автомата
    std::pair<char, int> prev; //предок
    int suffLink; //суф ссылка
    bool isLeaf; //конец слова
    int deep; //глубина
    std::vector<int> pos; //индекс

    Vertex() {//конструктор
        prev.second = -1;
        prev.first = '}';
        isLeaf = false;
        suffLink = -1;
        deep = 0;
    }
    Vertex(int prevInd, char prevChar) {//конструктор с вершиной
        isLeaf = false;
        suffLink = -1;
        prev.first = prevChar;
        prev.second = prevInd;
        deep = 0;
    }
    void printInfo(int i) {//вывод информации
        std::cout << "Vertex number " << i << "\n";
        if (prev.second != -1) {
            std::cout << "Number of parent vertex: " << prev.second << " symbol of edge: " << prev.first << "\n";
        }
        else {
            std::cout << "It is root\n";
        }
        if (isLeaf) {
            std::cout << "Vertex is leaf.\n";
        }
        else {
            std::cout << "Kids of vertex are:\n";
            for (auto it = sons.begin(); it != sons.end(); it++) {
                std::cout << "Vertex number " << it->second << " by edge with symbol " << it->first << "\n";
            }
        }
        if (suffLink != -1) {
            std::cout << "Suffix link is " << suffLink << "\n";
        }
        else {
            std::cout << "No suffix link yet\n";
        }
        if (!go.empty()) {
            std::cout << "Transitions from vertex are:\n";
            for (auto it = go.begin(); it != go.end(); it++) {
                std::cout << "Vertex number " << it->second << " by edge with symbol " << it->first << "\n";
            }
        }
    }
};


std::vector<int> answer;//ответ
int strSize = 0;
char joker;//джокер
std::vector<Vertex> bohr;//бор
void createBorh();
int getSuffLink(int ind);
int getLink(int ind, char c);
void ahoKorasik(std::string& text, std::vector<int>& counterArr);


int main() {
    std::string text;
    int size = 0;
    std::cin >> text;//текст
    std::vector<int> counterArr(text.length());
    createBorh();//создание бора
    std::cout << "\nBohr info:\n----------------------\n";
    for (unsigned i = 0; i < bohr.size(); i++) {
        bohr[i].printInfo(i);
        std::cout << "----------------------\n";
    }
    std::cout << "\n\n";
    ahoKorasik(text, counterArr);//запуск алгоритма
    std::cout << "\nAutomat info:\n----------------------\n";
    for (unsigned i = 0; i < bohr.size(); i++) {
        bohr[i].printInfo(i);
        std::cout << "----------------------\n";
    }
}


void createBorh() {//создание бора
    Vertex root;
    bool isPrevJoker = false;//был джокер
    int counter = 0;
    bohr.push_back(root);//корень в бор
    std::string buff;//шаблон
    std::cout << "Type pattern:\n";
    std::cin >> buff;
    strSize = buff.length();//длина шаблона
    std::cout << "Type joker:\n";
    std::cin >> joker;//джокер
    std::cout << "Creating bohr...\n";
    int curr = 0;
    for (unsigned int j = 0; j < buff.length(); j++) { //разделяем на подстроки
        std::cout << "Work with \'" << buff[j] << "\' symbol\n";
        if (buff[j] == joker) {//если джокер
            std::cout << "It's joker ";
            if (j == 0) {//первый символ
                std::cout << "and first symbol, skip symbol.\n";
                counter = 0;
                isPrevJoker = true;
                continue;
            }
            if (isPrevJoker) {//если был джокер
                std::cout << "and previous symbol is joker too, so skip symbol.\n";
                curr = 0;
                counter = 0;
                continue;
            }
            std::cout << " then previous vertex is leaf, sub-str added to bohr.\n";
            isPrevJoker = true;
            bohr[curr].isLeaf = true;
            bohr[curr].pos.push_back(j - counter);//позиция в шаблоне
            if (bohr[curr].deep == 0) {
                bohr[curr].deep = counter;
            }
            counter = 0;
            curr = 0;
            continue;
        }
        isPrevJoker = false;//не джокер
        counter++;//увеличиваем длину подстроки
        if (bohr[curr].sons.find(buff[j]) == bohr[curr].sons.end()) { //если нет вершины в боре 
            std::cout << "Symbol not joker, there is no edges by symbol \'" << buff[j] << "\', creating new vertex...\n";
            Vertex vert(curr, buff[j]);//создаем вершину
            vert.deep = counter; ///////
            bohr.push_back(vert);
            bohr[curr].sons[buff[j]] = bohr.size() - 1;//куда можем перейти
        }
        curr = bohr[curr].sons[buff[j]];//переход в след вершину
        std::cout << "Going to vertex by symbol \'" << buff[j] << "\'\n";
    }
    if (!isPrevJoker) {//если не джокер
        if (bohr[curr].deep == 0) {
            bohr[curr].deep = counter;
        }
        bohr[curr].isLeaf = true;
        bohr[curr].pos.push_back(buff.length() - counter);//индекс последнего символа
        std::cout << "Template over, then current vertex is leaf too.\n";
    }
    std::cout << "Bohr created!\n\n";
}


int getLink(int ind, char c) {//получение ссылки
    if (bohr[ind].go.find(c) == bohr[ind].go.end()) {//если переходили уже 
        std::cout << "There is no transitions by symbol\'" << c << "\'\n";
        if (bohr[ind].sons.find(c) != bohr[ind].sons.end()) {//если имеет сына по этому символу
            std::cout << "But vertex have son by this symbol, add this edge to tranision map.\n";
            bohr[ind].go[c] = bohr[ind].sons[c];
        }
        else {
            if (ind == 0) {//если мы в корне
                std::cout << "We are in ther root!\n";
                bohr[ind].go[c] = 0;//кольцо
            }
            else {//если еще не переходили
                std::cout << "Trying to transit by the suffix link...\n";
                bohr[ind].go[c] = getLink(getSuffLink(ind), c);//пытаемся перейти по суф ссылке
            }
        }
    }
    std::cout << "Transition by symbol \'" << c << "\' to " << bohr[ind].go[c] << " vertex.\n";
    return bohr[ind].go[c];//возвр ссылку
}


int getSuffLink(int ind) {//получение суф ссылки
    std::cout << "Finding suffix link...\n";
    if (bohr[ind].suffLink == -1) {//еще не определяли ссылку
        std::cout << "No suffix link for this vertex yet \n";
        if (ind == 0 || bohr[ind].prev.second == 0) {//корень или сын корня
            std::cout << "We are in the root or root-child, so suffix link is 0!\n";
            bohr[ind].suffLink = 0;
        }
        else {
            std::cout << "Trying to find suffix link by the transition to the previous vertex and check suffix link of it.\n";
            bohr[ind].suffLink = getLink(getSuffLink(bohr[ind].prev.second), bohr[ind].prev.first);//рекурсивно переходим к вершине предку и пытаемся перейти по ссылке
        }
    }
    std::cout << "Suffix link is " << bohr[ind].suffLink << ".\n";
    return bohr[ind].suffLink;//возвр суффиксальную ссылку
}


void ahoKorasik(std::string& text, std::vector<int>& counterArr) {//алгоритм
    int curr = 0;
    int numOfSubStr = 0;//номер подстроки
    for (unsigned int i = 0; i < bohr.size(); i++) {//подсчет подстрок
        if (bohr[i].isLeaf) {
            for (int j = 0; j < bohr[i].pos.size(); j++) {
                numOfSubStr++;
            }
        }
    }
    std::cout << "Start algorithm...\nCurrent vertex is root.\n";
    for (unsigned int i = 0; i < text.length(); i++) { //проходимся по тексту
        std::cout << "\nTransition by symbol \'" << text[i] << "\' with index " << i << "\n";
        curr = getLink(curr, text[i]); //куда можем перейти по символу
        std::cout << "----------------------------\nEntry check...\n";
        for (int j = curr; j != 0; j = getSuffLink(j)) { //пока не дойдем до корня
            if (bohr[j].isLeaf) {//если в конце шаблона
                int indInText = i + 1 - bohr[j].deep;//индекс в тексте
                std::cout << "Vertex is leaf, sub-str found, increasing counter arr...\n";
                for (unsigned int k = 0; k < bohr[j].pos.size(); k++) {
                    if (indInText - bohr[j].pos[k] >= 0) {//если в пределах текста
                        std::cout << "Increasing " << indInText - bohr[j].pos[k] << " element by one, it is " << counterArr[indInText - bohr[j].pos[k]] + 1 << " now." << "\n";
                        counterArr[indInText - bohr[j].pos[k]] += 1;//увеличиваем значение вхождений в этом индексе
                    }
                }
            }
        }
        std::cout << "----------------------------\n";
    }
    std::cout << "Counter arr contain next elements(if elements = " << numOfSubStr << " template found):\n";
    for (unsigned int i = 0; i < counterArr.size(); i++) {
        std::cout << counterArr[i] << " ";
    }
    std::cout << "\n\nPositions of template are:\n";
    for (unsigned int i = 0; i < counterArr.size() - strSize + 1; i++) {
        if (counterArr[i] == numOfSubStr) {
            std::cout << i + 1 << "\n";
        }
    }

}

