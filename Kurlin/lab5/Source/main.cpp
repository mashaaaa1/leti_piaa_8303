#include <iostream>
#include <string>
#include <vector>
#include <map>


struct Vertex
{
    std::map<char, int> next;   //Контейнер переходов по символу char в вершину с номером int
    std::map<char, int> go;     //массив переходов (запоминаем переходы в ленивой рекурсии), используемый для вычисления суффиксных ссылок
    bool isTerminal = false;    //Является ли терминальной (конечной) вершиной (на которой заканчивается паттерн)
    int prev;                   //Номер предыдущей вершины (родителя)
    char prevChar;              //Символ, по которому пришли в вершину
    int suffix;                 //Суффиксная ссылка
    int number;                 //Какой по счёту считанный паттерн заканчивается на этой вершине (только для вывода)
    int deep;                   //глубина в боре, равная длине строки-паттерна, заканчивающегося в этой терминальной вершине
};

int go(int index, char symb, std::vector<Vertex>& vertexArr);

void addString(const std::string& str, std::vector<Vertex>& vertexArr, int& count)
{
    std::cout << "Adding string \"" << str << "\" in the bohr\n";
    int current = 0;
    for (int i = 0; i < str.size(); i++) {
        std::cout << "\tCurrent symbol: \'" << str[i] << "\'\n";
        std::cout << "\tCurrent vertex: " << current << "\n";
        if (vertexArr[current].next.find(str[i]) == vertexArr[current].next.end()) {    //Если переход по символу не обнаружен и итератор указывает на конец мэпа
            std::cout << "\tWay through \'" << str[i] << "\' wasn't found. Adding new vertex with number " << vertexArr.size() << "\n";
            Vertex ver;                                                                 //Создаём новую вершину
            ver.suffix = -1;
            ver.prev = current;
            std::cout << "\t*previous vertex is " << current << ", the symbol of incoming path \'" << str[i] << "\'\n";
            ver.prevChar = str[i];
            vertexArr.push_back(ver);
            vertexArr[current].next[str[i]] = vertexArr.size() - 1;
        }
        else{
            std::cout << "The way through the symbol exist\n";
        }
        std::cout << std::endl;
        current = vertexArr[current].next[str[i]];      //Переход к следующей вершине
    }

    std::cout << "The number of this pattern is " << count + 1 << "\n";
    std::cout << "Vertex " << current << " is terminal, deep of the vertex is " << str.size() << "\n\n";
    vertexArr[current].number = ++count;                //Устанавливаем номер считанного паттерна,
    vertexArr[current].isTerminal = true;               //Терминальную вершину
    vertexArr[current].deep = str.size();               //Глубину
}


int getSuffix(int index, std::vector<Vertex>& vertexArr)    //Функция поиска суффиксной ссылки для вершины index
{
    std::cout << "\t\t\tGetting suffix-link from vertex " << index << "\n";
    if (vertexArr[index].suffix == -1) {                    //Если суффиксная ссылка ещё не была найдена
        if (index == 0 || vertexArr[index].prev == 0) {     //Если вершина - корень или сын корня
            vertexArr[index].suffix = 0;
            (index == 0) ? std::cout << "\t\t\tThis is root, suffix-link vertex = 0\n" : std::cout << "\t\t\tThis is a vertex with deep = 1, suffix-link = 0\n";
        }
        else {                                              //Рекурсивный поиск суфф. ссылки. Получаем ссылку родителя и выполняем
            std::cout << "\t\t\tFinding suffix-link from suffix of parent-vertex (" << vertexArr[index].prev << ") through " << vertexArr[index].prevChar << "\n";
            vertexArr[index].suffix = go(getSuffix(vertexArr[index].prev, vertexArr), vertexArr[index].prevChar, vertexArr);
        }                                                   //из неё переход по символу, по которому попали в вершину, для
    }                                                       //которой и ищется суфф. ссылка
    std::cout << "\t\t\tSuffix-link from vertex " << index << " is " << vertexArr[index].suffix << "\n\n";
    return vertexArr[index].suffix;
}


int go(int index, char symb, std::vector<Vertex>& vertexArr)      //Функция перехода из вершины index по символу symb. Если прямой переход
{                                                                 //невозможен, перейдёт по ссылке
    std::cout << "\t\t\t*Finding the way from " << index << " through \'" << symb << "\'\n";
    if (vertexArr[index].go.find(symb) == vertexArr[index].go.end()) {            //Если путь ещё не был найден
        if (vertexArr[index].next.find(symb) != vertexArr[index].next.end()) {    //Если найден прямой переход по символу в боре
            vertexArr[index].go[symb] = vertexArr[index].next[symb];              //Добавляем в контейнер возможных переходов
        }
        else {                                                                    //Если прямого перехода нет,  получаем суфф. ссылку
            if (index == 0)                                                       //и ищем переход из суффиксной ссылки по заданному символу
                std::cout << "\t\t\t*This is root\n";
            else
                std::cout << "\t\t\t*No straight path. Finding the way from suffix-link of this vertex through \'" << symb << "\'\n";
            vertexArr[index].go[symb] = (index == 0 ? 0 : go(getSuffix(index, vertexArr), symb, vertexArr));
        }
    }
    std::cout << "\t\t\t*Found way from " << index << " through \'" << symb << "\' is " << vertexArr[index].go[symb] << "\n";
    return vertexArr[index].go[symb];
}


void search(const std::string& text, std::vector<Vertex>& vertexArr, std::vector<std::pair<int, int>>& res, const std::vector<std::string>& patternArr)
{
    std::cout << "Searching begin\n";
    int curr = 0;
    for (int i = 0; i < text.size(); i++) {
        std::cout << "\tCurrent symbol is \'" << text[i] <<  "\' from text...\n";
        std::cout << "\tCurrent vertex is " << curr << "\n";
        curr = go(curr, text[i], vertexArr);
        std::cout << "\tAchieved vertex " << curr << "\n";
        std::cout << "\tFinding possible entrance with end suffix-links:\n";
        for (int tmp = curr; tmp != 0; tmp = getSuffix(tmp, vertexArr)) {
            std::cout << "\t\tCurrent suffix-link vertex: " << tmp <<"\n";
            if (vertexArr[tmp].isTerminal) {
                res.push_back(std::make_pair(i + 2 - vertexArr[tmp].deep, vertexArr[tmp].number));
                std::cout << "\t\tThe vertex is terminal (end suffix-link). The entrance found, index = " <<
                    i + 2 - vertexArr[tmp].deep << " (pattern = \"" << patternArr[vertexArr[tmp].number - 1] << "\")";
            } else
                std::cout << "\t\tIt's not terminal vertex, getting suffix-link from this vertex\n\n";
        }
        std::cout << "\t\tRoot is arrived, reading new symbol from the text\n";
        std::cout << "\t---------------------------------------------------------------------------------\n";
        std::cout << "\t---------------------------------------------------------------------------------\n";
    }
    std::cout << "-------------------------------------------------------------------------------------\n";
    std::cout << "-------------------------------------------------------------------------------------\n";
}


void printRes(const std::vector<std::pair<int, int>>& res, const std::vector<std::string>& patternArr, const std::string& text, std::string& textRest)
{
    std::vector<bool> cutStr(text.size());          //Индексы символов в строке, которые будут вырезаны
    for (int i = 0; i < res.size(); i++) {
        std::cout << res[i].first << " " << res[i].second << '\n';
        for (int j = 0; j < patternArr[res[i].second - 1].size(); j++)
            cutStr[res[i].first - 1 + j] = true;
    }

    for (int i = 0; i < cutStr.size(); i++){
        if (!cutStr[i])
            textRest.push_back(text[i]);            //Сохраняем только неудалённые символы
    }
}

void readPattern(std::vector<Vertex>& vertexArr, int& count, std::vector<std::string>& patternArr)
{
    Vertex root;
    root.prev = -1;
    root.suffix = -1;
    vertexArr.push_back(root);
    count = 0;

    int patternNumb;
    std::cout << "Enter pattern count:\n";
    std::cin >> patternNumb;

    for (int i = 0; i < patternNumb; i++) {
        std::cout << "Enter pattern for searching in:\n";
        std::string pattern;
        std::cin >> pattern;
        patternArr.push_back(pattern);
        addString(pattern, vertexArr, count);
    }
}

//Функция поиска максимального числа исходящих дуг из одной вершины бора
int findMaxSons(std::vector<Vertex> vertexArr)
{
    int max = vertexArr[0].next.size();

    for(int i = 1; i < vertexArr.size(); i++){
        if (vertexArr[i].next.size() > max)
            max = vertexArr[i].next.size();
    }
    return max;
}

void automatePrint(std::vector <Vertex> vertexArr)
{
    std::cout << "-------------------------------------------------------------------------------------\n";
    std::cout << "Total automate:\n";

    for (int i = 0; i < vertexArr.size(); i++){
        std::cout << "Connections from vertex " << i << ":\n";
        auto iter = vertexArr[i].go.begin();
        for (int j = 0; j < vertexArr[i].go.size(); j++){
            std::cout << "\t" << i << "  --" << iter->first << "->  " << iter->second << "\n";
            iter++;
        }
    }
}

int main() {
    std::cout << "-------------------------------------------------------------------------------------\n";
    std::cout << "Enter text:\n";
    std::string text, textRest;
    std::cin >> text;

    std::vector<Vertex> vertexArr;
    std::vector<std::string> patternArr;
    std::vector<std::pair<int, int>> res;   //<Индекс паттерна в тексте, номер паттерна>
    int count;

    readPattern(vertexArr, count, patternArr);
    search(text, vertexArr, res, patternArr);
    printRes(res, patternArr, text, textRest);

    std::cout << "Rest string from text after cutting patterns from it: " << textRest << "\n";

    int maxSonsCount = findMaxSons(vertexArr);
    std::cout << "Max count of sons: " << maxSonsCount << "\n\n";


    automatePrint(vertexArr);

    return 0;
}