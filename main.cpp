#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <set>
using namespace std;

vector<string> DFA_Stari;
vector<string> DFA_Stari_Finale;
unordered_map<int,unordered_map<char,vector<int>>> NFA_Tranzitii;
unordered_map<string,unordered_map<char,string>> DFA_Tranzitii;
unordered_map<string,bool> Closed_list;
unordered_map<string,int> String_Translate;
bool Zero_Needed = false;

int Numar_stari, Numar_litere, Numar_stari_finale, Numar_tranzitii, x, z, Starea_initiala;
vector <int> NFA_Starile, NFA_Stari_finale;
char y;
vector <char> Literele;

void NFA_to_DFA(set<int> Stare, string Cuvant_stare){
    set<int> concatenate = {};
    string special_string = "";

    DFA_Stari.push_back(Cuvant_stare);              // Add the new node into the pile

    for (int Stare_finala : NFA_Stari_finale){              // Check to see if it's also a final node
        if (Stare.find(Stare_finala) != Stare.end()){
            DFA_Stari_Finale.push_back(Cuvant_stare);
            break;
        }
    }

    for (char Litera : Literele) {                                          // Iterate through all letters
        for (int Elem : Stare){                                 // Concatenate all the states found by that letter
            concatenate.insert(NFA_Tranzitii[Elem][Litera].begin(), NFA_Tranzitii[Elem][Litera].end());
        }
        for (int Elem : concatenate){                       // Create special string
            special_string += to_string(Elem) + "-";
        }

        if (special_string == "") {              // Check to see if it's empty
            special_string = "0";
            Zero_Needed = true;
        }
        DFA_Tranzitii[Cuvant_stare][Litera] = special_string;        // Put it in the transition unordered_map

        if (!Closed_list[special_string] && special_string != "0"){       // Check to see if it's unique
            Closed_list[special_string] = true;
            NFA_to_DFA(concatenate, special_string);
        }

        special_string = "";                         // Reset
        concatenate.clear();
    }
}

int main() {
    fstream input("Input.txt");

                                    //  INPUT
    input >> Numar_stari;
    for (int i = 0; i < Numar_stari; i++){
        input >> x;
        NFA_Starile.push_back(x);
    }

    input >> Numar_litere;
    for (int i = 0; i < Numar_litere; i++){
        input >> y;
        Literele.push_back(y);
    }

    input >> Starea_initiala;

    input >> Numar_stari_finale;
    for (int i = 0; i < Numar_stari_finale; i++){
        input >> x;
        NFA_Stari_finale.push_back(x);
    }

    input >> Numar_tranzitii;
    for (int i = 0; i < Numar_tranzitii; i++) {
        input >> x >> y >> z;
        NFA_Tranzitii[x][y].push_back(z);
    }

    //  Function call

    NFA_to_DFA({Starea_initiala}, to_string(Starea_initiala));

    for (int i = 0; i <= DFA_Stari.size(); i++){
        String_Translate[DFA_Stari[i]] = i + 1;
    }

    // Check if you need a new state (0)

    if (Zero_Needed){
        DFA_Stari.push_back("0");
        for (char aux : Literele){
            DFA_Tranzitii["0"][aux] = "0";
        }
    }

                                    // OUTPUT
    cout << "DFA" << endl;
    cout << "Nr. stari: " << DFA_Stari.size() << endl;
    cout << "Stari: ";
    for (string stare : DFA_Stari) {
        cout << String_Translate[stare] << " ";
    }
    cout << endl << "Nr. litere: " << Numar_litere << endl;
    cout << "Literele: ";
    for (char aux : Literele){
        cout << aux << " ";
    }
    cout << endl << "Starea initiala: " << Starea_initiala << endl;
    cout << "Nr. stari finale: " << DFA_Stari_Finale.size() << endl;
    cout << "Starile finale: ";
    for (string stare : DFA_Stari_Finale){
        cout << String_Translate[stare] << " ";
    }
    cout << endl << "Tranzitii:" << endl;
    for (string stare : DFA_Stari) {
        for (pair<char,string> road : DFA_Tranzitii[stare]) {
            if (road.second != ""){
                cout << String_Translate[stare] << " " << road.first << " " << String_Translate[road.second];
            }else{
                cout << String_Translate[stare] << " " << road.first << " " << "0";
            }
            cout << endl;
        }
    }
    return 0;
}