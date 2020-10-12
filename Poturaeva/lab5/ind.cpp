#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>



class Vertex {
public:
    std::map<char, int> sons; 
    std::map<char, int> go; 
    std::pair<char, int> prev; 
    int suffLink; 
    bool isLeaf; 
    bool isNextJoker;
    int deep;
    std::vector<int> pos;

    Vertex() {
        prev.second = -1;
        prev.first = '}';
        isLeaf = false;
        suffLink = -1;
        deep = 0;
        isNextJoker = false;
    }
    Vertex(int prevInd, char prevChar) {
        isLeaf = false;
        suffLink = -1;
        prev.first = prevChar;
        prev.second = prevInd;
        deep = 0;
        isNextJoker = false;
    }
    void printInfo(int i) {
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


char notJoker;//не джокер
int strSize = 0;
std::vector<int> answer;
std::vector<int> jockersPos;//позиции джокера
char joker;
std::vector<Vertex> bohr;
void createBorh();
int getSuffLink(int ind);
int getLink(int ind, char c);
void increase(std::vector<int>& counterArr, int indInText, int j);//увеличить значения в индексах
void decrease(std::vector<int>& counterArr, int indInText);//уменьшить значения в индексах
void ahoKorasik(std::string& text, std::vector<int>& c);


int main() {
    std::string text;
    int size = 0;
    std::cin >> text;
    std::vector<int> c(text.length());
    createBorh();
    std::cout << "\nBohr info:\n----------------------\n";
    for (unsigned i = 0; i < bohr.size(); i++) {
        bohr[i].printInfo(i);
        std::cout << "----------------------\n";
    }
    std::cout << "\n\n";
    ahoKorasik(text, c);
    std::cout << "\nAutomat info:\n----------------------\n";
    for (unsigned i = 0; i < bohr.size(); i++) {
        bohr[i].printInfo(i);
        std::cout << "----------------------\n";
    }
}


void createBorh() {
    Vertex root;
    int deepCounter = 0;
    bool flag = false;
    int counter = 0;
    bohr.push_back(root);
    std::string buff;
    std::cout << "Type pattern:\n";
    std::cin >> buff;
    strSize = buff.length();
    std::cout << "Type joker:\n";
    std::cin >> joker;
    std::cout << "Type non joker:\n";
    std::cin >> notJoker;
    int curr = 0;
    std::cout << "Creating bohr...\n";
    for (unsigned int j = 0; j < buff.length(); j++) { //делим на подстроки
        std::cout << "Work with \'" << buff[j] << "\' symbol\n";
        if (buff[j] == joker) {//если джокер
            std::cout << "It's joker ";
            if (j == 0) {//первый символ
                std::cout << "and first symbol, skip symbol, push position of joker to non-joker arr.\n";
                jockersPos.push_back(j);//записываем индекс джокера
                counter = 0;
                deepCounter = 0;
                flag = true;
                continue;
            }
            if (flag) {//если пред джокер
                std::cout << "and previous symbol is joker too, so skip symbol, push position of joker to non-joker vertex.\n";
                jockersPos.push_back(j);
                deepCounter = 0;
                curr = 0;
                counter = 0;
                continue;
            }
            std::cout << " then previous vertex is leaf, sub-str added to bohr, push position of joker to non-joker vertex.\n";
            jockersPos.push_back(j);
            flag = true;
            bohr[curr].isLeaf = true;
            bohr[curr].pos.push_back(j - counter);
            if (bohr[curr].deep == 0) {
                bohr[curr].deep = counter;
            }
            counter = 0;
            curr = 0;
            continue;
        }
        flag = false;
        counter++;
        if (bohr[curr].sons.find(buff[j]) == bohr[curr].sons.end()) { //если не создавали еще ребро
            std::cout << "Symbol not joker, there is no edges by symbol \'" << buff[j] << "\', creating new vertex...\n";
            Vertex vert(curr, buff[j]);
            vert.deep = counter;
            bohr.push_back(vert);
            bohr[curr].sons[buff[j]] = bohr.size() - 1;

        }
        curr = bohr[curr].sons[buff[j]];
        std::cout << "Going to vertex by symbol \'" << buff[j] << "\'\n";
    }
    if (!flag) {//если послед не джокер
        if (bohr[curr].deep == 0) {
            bohr[curr].deep = counter;
        }
        bohr[curr].isLeaf = true;
        bohr[curr].pos.push_back(buff.length() - counter);
        std::cout << "Template over, then current vertex is leaf too.\n";
    }
    std::cout << "Bohr created!\n\n";
}


int getLink(int ind, char c) {//получение ссылки
    if (bohr[ind].go.find(c) == bohr[ind].go.end()) {//если есть переход
        std::cout << "There is no transitions by symbol\'" << c << "\'\n";
        if (bohr[ind].sons.find(c) != bohr[ind].sons.end()) {//если есть переход в боре от родителя
            std::cout << "But vertex have son by this symbol, add this edge to tranision map.\n";
            bohr[ind].go[c] = bohr[ind].sons[c];
        }
        else {
            if (ind == 0) {//если первый символ
                std::cout << "We are in ther root!\n";
                bohr[ind].go[c] = 0;
            }
            else {
                std::cout << "Trying to transit by the suffix link...\n";
                bohr[ind].go[c] = getLink(getSuffLink(ind), c);//пыьаемся получить суф ссылку
            }
        }
    }
    std::cout << "Transition by symbol \'" << c << "\' to " << bohr[ind].go[c] << " vertex.\n";
    return bohr[ind].go[c];//возвр состояние
}


int getSuffLink(int ind) {//получение суф ссылки
    std::cout << "Finding suffix link...\n";
    if (bohr[ind].suffLink == -1) {//если еще не определяли
        std::cout << "No suffix link for this vertex yet \n";
        if (ind == 0 || bohr[ind].prev.second == 0) {//корень или сын корня
            std::cout << "We are in the root or root-child, so suffix link is 0!\n";
            bohr[ind].suffLink = 0;
        }
        else {
            std::cout << "Trying to find suffix link by the transition to the previous vertex and check suffix link of it.\n";
            bohr[ind].suffLink = getLink(getSuffLink(bohr[ind].prev.second), bohr[ind].prev.first);//пытаемся рекурсивно перейти по ссылке
        }
    }
    std::cout << "Suffix link is " << bohr[ind].suffLink << ".\n";
    return bohr[ind].suffLink;//возвр суф ссылку
}


void ahoKorasik(std::string& text, std::vector<int>& counterArr) {//алгоритм
    int curr = 0;
    int tmp = 0;//количество подстрок
    for (unsigned int i = 0; i < bohr.size(); i++) {
        if (bohr[i].isLeaf) {
            for (int j = 0; j < bohr[i].pos.size(); j++) {
                tmp++;
            }
        }
    }
    std::cout << "Start algorithm...\nCurrent vertex is root.\n";
    for (unsigned int i = 0; i < text.length(); i++) { 
      
        std::cout << "\nTransition by symbol \'" << text[i] << "\' with index " << i << "\n";
        curr = getLink(curr, text[i]); //получение ссылки
        std::cout << "----------------------------\nEntry check...\n";
        if (curr == 0) {//если это корень
            if (text[i] == notJoker) {//и не джокер
                std::cout << "Found non jocker, decreasing elements in counter arr...\n";
                decrease(counterArr, i);//уменьшаем значение в индексе
            }
        }
        for (int j = curr; j != 0; j = getSuffLink(j)) { //пока не дошли до корня
            int indInText = i + 1 - bohr[j].deep;
            if (bohr[j].isLeaf) {
                std::cout << "Vertex is leaf, sub-str found, ";
                std::cout << "Increasing elements in counter arr...\n";
                increase(counterArr, indInText, j);//увеличиваем значение в индексе
                if (text[i] == notJoker) {//если не джокер
                    std::cout << "Found non jocker, decreasing elements in counter arr...\n";
                    decrease(counterArr, indInText);//уменьшаем
                }
            }
            else {
                if (text[i] == notJoker) {
                    std::cout << "Found non jocker, decreasing elements in counter arr...\n";
                    decrease(counterArr, indInText);
                }
            }
        }
        std::cout << "----------------------------\n";
    }
    std::cout << "Counter arr contain next elements(if elements = " << tmp << " template found):\n";
    for (unsigned int i = 0; i < counterArr.size(); i++) {
        std::cout << counterArr[i] << " ";
    }
    std::cout << "\n\nPositions of template are:\n";
    for (unsigned int i = 0; i < counterArr.size() - strSize + 1; i++) {
        if (counterArr[i] == tmp) {
            std::cout << i + 1 << "\n";
        }
    }

}


void decrease(std::vector<int>& counterArr, int indInText) {//уменьшение
    for (unsigned int k = 0; k < jockersPos.size(); k++) {
        if ((indInText - jockersPos[k] >= 0)) {
            std::cout << "\tDecreasing " << indInText - jockersPos[k] << " element by one, it is " << counterArr[indInText - jockersPos[k]] + 1 << " now." << "\n";
            counterArr[indInText - jockersPos[k]] -= 1;
        }
    }
}

void increase(std::vector<int>& counterArr, int indInText, int j) {//увеличение
    for (unsigned int k = 0; k < bohr[j].pos.size(); k++) {
        if (indInText - bohr[j].pos[k] >= 0) {
            std::cout << "\tIncreasing " << indInText - bohr[j].pos[k] << " element by one, it is " << counterArr[indInText - bohr[j].pos[k]] + 1 << " now." << "\n";
            counterArr[indInText - bohr[j].pos[k]] += 1;
        }
    }
}

