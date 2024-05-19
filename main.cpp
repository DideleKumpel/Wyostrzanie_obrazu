#include <iostream>
#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;

class obraz{

    vector<vector<double>> rownania;
    vector<vector<double>> wyostrzony;
    void utworzenieRownan();
    int przelicznik(int x, int y);
    void EleminacjaGausa();  //rozwiazywanie utworzonych rownan metoda eleminacji gausa
    void wyswietl();
public:
    int w, h, roz; //szerokosc, wysokosc i poziom rozmycia
    vector<vector<double>> zablurowana;
    void Wykonaj();
};

int main() {
    vector<obraz> dane;
    int a, b, c;
    int ileMacierzy=0;
    while(true) {
        obraz pomocnicza;
        cin >> a >> b >> c;
        if(a!=0 and b!=0 and c!=0){
            ileMacierzy++;
            pomocnicza.w=a;
            pomocnicza.h=b;
            pomocnicza.roz=c;
            vector<vector<double>> tymczasowy(a+1, (vector<double> (b+1)));
            for (int j = 1; j <=b; j++) {
                for (int i = 1; i <= a; i++) {
                    double d;
                    cin >> d;
                    tymczasowy[i][j] = d;

                }
            }
            pomocnicza.zablurowana=tymczasowy;
            dane.push_back(pomocnicza);
        }else break;
    }
    for(int i=0; i<ileMacierzy; i++)
        dane[i].Wykonaj();
    return 0;
}

void obraz::utworzenieRownan(){
    vector<vector<double>> RownanPomocnicze(w*h+1, (vector<double> (w*h+1)));
    int IleDodanych;
    for(int i=1;i<=w;i++)
    {
        for(int j=1;j<=h;j++)
        {
            IleDodanych=0;
            for(int k=1;k<=w;k++)
            {
                for(int l=1;l<=h;l++)
                {
                    if(abs(k-i)+abs(l-j)<=roz)
                    {
                        RownanPomocnicze[przelicznik(i, j)][przelicznik(k, l)]=1;  //zapisujey rownania w formie [nr rownania w macierzy zablurowanej][niewiadome wchodzace skladtego urwnania(=1)]
                        IleDodanych++;
                    }
                }
            }
            RownanPomocnicze[przelicznik(i, j)].push_back(IleDodanych * zablurowana[i][j]);// numer w macierzy po pomnorzeniu przez ilosc elementow czyli wynik rownania
        }
    }
    rownania=RownanPomocnicze;
}

int obraz::przelicznik(int x, int y) {
    return (y - 1) * w + x; //przeliczanie pozycji 2 wymiarowej tablicy na jeden wymiar
}

void obraz::EleminacjaGausa(){
    for(int i=1; i<=w*h; i++){  //petla dla kazdej kolumny
        int wiersz=-1;
        if(rownania[i][i]==0) { //pozniej bedziemy dielic przez ta liczbe nalzey sprawdzic czy bedzie tam zero jesli zero nalezy
            for (int j = i; j <= w * h; j++) {
                if (rownania[j][i] != 0) {  //sprawdza gdzie w kolumnie powyzej przekatnej nie ma zera
                    wiersz = j;
                    break;
                }
            }
            if (wiersz == -1) // Jesli nie znaleziono zero
                continue;
            if (wiersz != i)   //jesli znaleziono zamien kolejnosca te wiersze
                for (int j = 1; j <= w * h + 1; j++)
                    swap(rownania[i][j], rownania[wiersz][j]);
        }
        for(int j = i+1 ; j <= w * h; j++){ //petla odejmujaca wiersze macierzy od siebie aby kolumna byla wyzerowana
            double stala = rownania[j][i]/rownania[i][i];
            for(int k=i; k<= w*h+1; k++)
                rownania[j][k] -= stala*rownania[i][k];
        }
    }
    vector<vector<double>> wyostrzonyPomocniczy (w+1, (vector<double> (h+1)));
    for(int i = w*h; i>=1; i--){
        for(int j=i+1; j<w*h+1; j++)
            rownania[i][w*h+1] -= rownania[j][w*h+1] * rownania[i][j];
        int x = i % w;        //wyliczanie pozycji wyniku w macierzy z wyostzronym obrazem
        if (x == 0) x = w;
        int y = ((i - x) / w)+1;
        wyostrzonyPomocniczy[x][y] = rownania[i][w * h + 1] /= rownania[i][i]; //zapisanie wyniku do wyostrzinej macierzy
    }
    wyostrzony=wyostrzonyPomocniczy;
}

void obraz::wyswietl() {
    for(int i = 1; i <= h; ++i){
        for(int j = 1; j <= w; ++j){
            cout << setw(8) << setprecision(2) << fixed << wyostrzony[j][i] << " ";
        }
        cout << '\n';
    }
    cout << '\n';
}

void obraz::Wykonaj(){
    utworzenieRownan();
    EleminacjaGausa();
    wyswietl();
}