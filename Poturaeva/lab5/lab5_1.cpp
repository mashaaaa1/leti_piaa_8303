#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>


class Vertex {
public:
    std::map<char, int> sons; //map of sons
    std::map<char, int> go; //map of transition
    std::pair<char, int> prev; //information about parent vertex
    int suffLink; //suffix link
    bool isLeaf; //obvious
    int number; //number of str
    int deep; //deep in bohr

    Vertex() {
        prev.second = -1;
        prev.first = '}';
        isLeaf = false;
        suffLink = -1;
        deep = 0;
        number = 0;
    }
    Vertex(int prevInd, char prevChar) {
        isLeaf = false;
        suffLink = -1;
        prev.first = prevChar;
        prev.second = prevInd;
        deep = 0;
        number = 0;
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


std::vector<std::pair<int, int>> answer; //first int - number, 2 - position
std::vector<Vertex> bohr;
std::vector<std::string> forOut;
void createBohr(int size);
int getSuffLink(int ind);
int getLink(int ind, char c);
void ahoKorasik(std::string& text);
int compare(std::pair<int, int> a, std::pair<int, int>  b);


int main() {
    std::string text;
    int size = 0;
    std::cin >> text;
    std::cin >> size;
    createBohr(size);
    std::cout << "\nBohr info:\n----------------------\n";
    for (unsigned i = 0; i < bohr.size(); i++) {
        bohr[i].printInfo(i);
        std::cout << "----------------------\n";
    }
    
    std::cout << "\n\n";
    ahoKorasik(text);
    std::cout << "\nAutomat info:\n----------------------\n";
    for (unsigned i = 0; i < bohr.size(); i++) {
        bohr[i].printInfo(i);
        std::cout << "----------------------\n";
    }
    sort(answer.begin(), answer.end(), compare);
    std::cout << "Answer is:\n";
    for (unsigned i = 0; i < answer.size(); i++) {
        std::cout << "Position in the text " << answer[i].second << ", number of template is " << answer[i].first + 1 << "\n";
    }
    
}


void createBohr(int size) {
    std::cout << "Creating bohr...\n";
    Vertex root;
    bohr.push_back(root);
    for (int i = 0; i < size; i++) { //for each template
        std::string buff;
        std::cin >> buff;
        forOut.push_back(buff);
        int curr = 0;
        std::cout << "Add \"" << buff << "\" template in bohr...\n";
        // system("pause");
        for (unsigned int j = 0; j < buff.length(); j++) { //for each symbol in template
            std::cout << "Work with \'" << buff[j] << "\' symbol\n";
            if (bohr[curr].sons.find(buff[j]) == bohr[curr].sons.end()) { //if can't go 
                std::cout << "There is no edges by symbol \'" << buff[j] << "\', creating new vertex...\n";
                Vertex vert(curr, buff[j]); //create new vertex
                bohr.push_back(vert);
                bohr[curr].sons[buff[j]] = bohr.size() - 1; //add information about child vertex
            }
            curr = bohr[curr].sons[buff[j]]; // move to vertex
            std::cout << "Going to vertex by symbol \'" << buff[j] << "\'\n";
        }
        std::cout << "Template added to bohr, current vertex is leaf.\n";
        bohr[curr].isLeaf = true;
        bohr[curr].number = i;
        bohr[curr].deep = buff.length();
    }
}


int getSuffLink(int ind) {
    std::cout << "Finding suffix link...\n";
    if (bohr[ind].suffLink == -1) {
        std::cout << "No suffix link for this vertex yet \n";
        if (ind == 0 || bohr[ind].prev.second == 0) {
            std::cout << "We are in the root or root-child, so suffix link is 0!\n";
            bohr[ind].suffLink = 0;
        }
        else {
            std::cout << "Trying to find suffix link by the transition to the previous vertex and check suffix link of it.\n";
            bohr[ind].suffLink = getLink(getSuffLink(bohr[ind].prev.second), bohr[ind].prev.first);
        }
    }
    std::cout << "Suffix link is " << bohr[ind].suffLink << ".\n";
    return bohr[ind].suffLink;
}


int getLink(int ind, char c) {
    if (bohr[ind].go.find(c) == bohr[ind].go.end()) {
        std::cout << "There is no transions by symbol\'" << c << "\'\n";
        if (bohr[ind].sons.find(c) != bohr[ind].sons.end()) {
            std::cout << "But vertex have son by this symbol, add this edge to tranision map.\n";
            bohr[ind].go[c] = bohr[ind].sons[c];
        }
        else {
            if (ind == 0) {
                std::cout << "We are in ther root!\n";
                bohr[ind].go[c] = 0;
            }
            else {
                std::cout << "Trying to transit by the suffix link...\n";
                bohr[ind].go[c] = getLink(getSuffLink(ind), c);
            }
        }
    }
    std::cout << "Transition by symbol \'" << c << "\' to " << bohr[ind].go[c] << " vertex.\n";
    return bohr[ind].go[c];
}


void ahoKorasik(std::string& text) {
    int curr = 0;
    std::cout << "Start algorithm...\nCurrent vertex is root.\n";
    for (unsigned int i = 0; i < text.length(); i++) { //for each symbol of text
        std::cout << "\nTransition by symbol \'" << text[i] << "\' with index " << i << "\n";
        curr = getLink(curr, text[i]); //make transition
        std::cout << "----------------------------\nEntry check...\n";
        for (int j = curr; j != 0; j = getSuffLink(j)) { //entry check
            if (bohr[j].isLeaf) {
                std::cout << "Checking vertex is leaf, template found!\n";
                std::pair<int, int> res(bohr[j].number, i + 2 - bohr[j].deep);
                answer.push_back(res);
                std::cout << "Template is: \n";
                std::cout << forOut[res.first] << "\n";
            }
        }
        std::cout << "----------------------------\n";
    }
}


int compare(std::pair<int, int> a, std::pair<int, int>  b) {
    if (a.second == b.second) {
        return a.first < b.first;
    }
    else {
        return a.second < b.second;
    }
}