#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;
bool czydopliku = false;
ofstream wynik;
std::stringstream ss;
string kodyH[256];

// deklaracja wêzla drzewa binarnego
//----------------------------------

struct wezel
{
  wezel * rodzic, * lewo, * prawo;
  char znak;
  int wartosc;
};

// Funkcja rekurencyjne przechodzenia drzewa binarnego
//----------------------------------------------------

void inorder(wezel * w, char c[], int lenc)
{
  if(!(w->lewo))
  {
    ss.str( std::string() ); ss.clear();
    if(w->znak == '\n') cout << "\\n" << ": "; else
    cout << w->znak << ": ";
    for(int i = 0; i < lenc; i++){
        cout << c[i];
        ss << c[i];
    }
    kodyH[w->znak] = ss.str();
    cout << endl;
  }
  else
  {
    c[lenc] = '0'; inorder(w->lewo,c,lenc + 1);
    c[lenc] = '1'; inorder(w->prawo,c,lenc + 1);
  }
}

void znajdz2najmniejsze(std::vector<wezel*> &myvector, wezel * &w1, wezel * &w2, int &i1, int &i2){
    wezel * w;
    w1 = myvector[0]; i1 = 0;
    w2 = myvector[1]; i2 = 1;
    //cout << "first w1->znak: " << w1->znak << " w1->wartosc: " << w1->wartosc << endl;
    //cout << "first w2->znak: " << w2->znak << " w2->wartosc: " << w2->wartosc << endl;
    if(w1->wartosc > w2->wartosc){
        wezel * x;
        int ix;
        x = w1; w1 = w2; w2 = x;
        ix = i1; i1 = i2; i2 = ix;
    }
    //for (w = myvector->front()+2; w != myvector->back(); ++w){
    for( int i = 2; i < myvector.size(); i++ ){
        w = myvector[i];
        //cout << "w->wartosc:" << w->wartosc << endl;
        if(w->wartosc < w2->wartosc){
            w2 = w;
            i2 = i;
            if(w1->wartosc > w2->wartosc){
                wezel * x;
                int ix;
                x = w1; w1 = w2; w2 = x;
                ix = i1; i1 = i2; i2 = ix;
            }
        }
    }

    if(i1 > i2){
        int ix;
        ix = i1; i1 = i2; i2 = ix;
    }
    //cout << "Najmn w1->znak: " << w1->znak << " w1->wartosc: " << w1->wartosc << endl;
    //cout << "Najmn w2->znak: " << w2->znak << " w2->wartosc: " << w2->wartosc << endl;
}

//******************************************************
//**   Przyk³adowa implementacja algorytmu Huffmana   **
//******************************************************

int main()
{
    cout << "Program wczytuje tekst z pliku lub z klawiatury i koduje go za pomoca algorytmu Huffmana." << endl;
    wynik.open("wynik.txt");
    string text;
    int n = 0;
    std::vector < wezel* > tab;
    while(1){
        //cout << "Gdzie wypisac wyniki? 1 - na ekran 2 - do pliku" << endl;
        //cin >> n;
        //if(n==2) czydopliku = true;
        //n = 0;

        cout << "Skad pobrac dane? 1-wczytaj z pliku 2-pozwol wpisac z klawiatury" << endl;
        cin >> n;
        if(n==1){
            //czyZpliku = true;
            ifstream dane;
            dane.open("text.txt");

            if( !dane.good() ){
                cout << "Nie znaleziono pliku text.txt" << endl;
                continue;
            }

            std::string wiersz;
            std::getline( dane, wiersz );
            ss.str( std::string() ); ss.clear();
            ss << wiersz;
            while( std::getline( dane, wiersz ) ){
                ss << "\n" << wiersz;
            }
            text = ss.str();
            ss.str( std::string() ); ss.clear();
            cout << "Wczytalem nastepujacy tekst z pliku text.txt:" << endl;
            cout << text << endl;
            dane.close();
        }else{
            cout << "Wpisz tekst do zakodowania: " << endl;
            cin.clear();
            std::cin.sync();
            getline(cin,text);
        }
        break;
    }

    // Zliczamy liczbê wyst¹pieñ ka¿dego znaku
    int ccount[256];
    int ileWszystkichZnakow = 0;
    for(int i = 0; i < 256; i++) ccount[i] = 0;
    for(int i = text.size() - 1; i >= 0; i--) ccount[text[i]]++;
    wezel * w;
    wezel * w1, * w2;
    for(int i = 0; i < 256; i++)
        if(ccount[i]){
            w = new wezel;
            w->rodzic = w->lewo = w->prawo = NULL;
            w->znak = (char) i;
            w->wartosc = ccount[i];
            ileWszystkichZnakow += ccount[i];
            tab.push_back(w);
        }

    // wyœwietlamy znaki wraz z ich czêstoœciami w wierszu
    cout << endl;
    cout << "Znak: Ilosc wystapien" << endl;
    for( int i = 0; i < tab.size(); i++ ){
        char znaczek = tab[ i ]->znak;
        if(znaczek == '\n') cout << "\\n" << ": " << tab[ i ]->wartosc << endl; else
        cout << tab[ i ]->znak << ": " << tab[ i ]->wartosc << endl; //<< "(" << int(double(tab[ i ]->wartosc/ileWszystkichZnakow)*100) << "%)" << std::endl;
    }

    // tworzymy drzewo binarne Huffmana
    int i1 = 0;
    int i2 = 0;
    int j = 0;
    cout << endl;
    cout << "Uzyskane kody dla poszczegolnych znakow:" << endl;
    while(tab.size() > 1){
        /*for( int i = 0; i < tab.size(); i++ ){
            w = tab[i];
            cout << w->znak << "(" << w->wartosc << ") ";
        }
        cout << endl;*/
        znajdz2najmniejsze(tab, w1, w2, i1, i2); // na liscie wyszukujemy dwa najmniejsze elementy
        // z wêzlów zawartych w w1 i w2 tworzymy nowy wezel
        //cout << "Najmn2 w1->znak: " << w1->znak << " w1->wartosc: " << w1->wartosc << endl;
        //cout << "Najmn2 w2->znak: " << w2->znak << " w2->wartosc: " << w2->wartosc << endl;
        w = new wezel;
        w->rodzic = NULL;
        w->lewo  = w1;
        w->prawo = w2;
        //j++;
        //w->znak = (char) '0'+j;
        w->lewo->rodzic = w->prawo->rodzic = w;
        w->wartosc = w1->wartosc + w2->wartosc;
        //cout << w1->znak << "(" << w1->wartosc << ")+" << w2->znak << "(" << w2->wartosc << ") = " << w->znak << "(" << w->wartosc << ")" << endl;
        tab.push_back(w);
        //cout << "tab[i1]->znak: " << tab[i1]->znak << " tab[i1]->wartosc: " << tab[i1]->wartosc << endl;
        tab.erase(tab.begin()+i2);
        tab.erase(tab.begin()+i1);
        //if (it != tab.end()) { tab.erase(it); }
        //it = std::find(tab.begin(), tab.end(), w2);
        //if (it != tab.end()) { tab.erase(it); }
        //tab.push_back(w);
    }
    // wywolujemy rekurencyjna funkcje inorder, która
    // przejdzie przez cale drzewko wypisujac kody Huffmana
    // poszczególnych lisci - znaków
    char tabb[256];
    inorder(tab[0],tabb,0);
    cout << endl;
    cout << "Wczytany tekst w formie zakodowanej:" << endl;
    for(int i = 0; i < text.size(); i++){
        cout << kodyH[text[i]];
        wynik << kodyH[text[i]];
    }
    cout << endl;
    cout << "Wyjscie programu zapisano rowniez do pliku: wynik.txt" << endl;

    wynik.close();
    int blablabla = 0;
    cin >> blablabla;
    return 0;
}
