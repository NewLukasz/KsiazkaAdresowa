#include <iostream>
#include <windows.h>
#include <conio.h>
#include <iomanip>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <cstdio>

using namespace std;

struct Adresat {
    string imie="",nazwisko="",email="",adres="";
    int id,numerTelefonu, idUzytkownika;
};

struct Uzytkownik {
    int idUzytkownika;
    string nazwa,haslo;
};

Adresat dodajKontakt(vector<Adresat> kontakty, int idZalogowanegoUzytkownika, int ostatniZajetyAdresID);
int okreslenieKolejnegoNumeruIdDlaKontaktuNaPodstawiePlikuTymczasowegoOrazPamieci(vector <Adresat> kontakty);
int weryfikacjaPoprawnosciNumeru();
string weryfikacjaPoprawnosciMaila();
void wyswietlanieWszystkichKontaktow(vector <Adresat> kontakty, int idZalogowanegoUzytkownika);
void wyszukajPoImieniu(vector<Adresat> kontakty, int idZalogowanegoUzytkownika);
void wyszukajPoNazwisku(vector<Adresat> kontakty, int idZalogowanegoUzytkownika);
void usunAdresata(vector <Adresat> &kontakty, int idZalogowanegoUzytkownika);
void modyfikujKontakt(vector <Adresat> &kontakty, int idZalogowanegoUzytkownika);

void zapiszDoPlikuKontakty(vector <Adresat> kontakty, int idZalogowanegoUzytkownika);
void odczytajKontantyZPlikuDlaUzytkownikaNiezalogowanegoIZapiszDoPlikuTymczasowego(int idAktualnieZalogowanegoUzytkownika);
void odczytajKontantyZPlikuDlaUzytkownikaZalogowanegoIZapiszDoPliku(int idAktualnieZalogowanegoUzytkownika);
void przeniesienieKontaktowZPlikuTymczasowego();
vector <Adresat> odczytajKontaktyZPlikuDlaUzytkownikaZalogowanego(int idAktualnieZalogowanegoUzytkownika);
int wyznaczIDUzytkownikaKtoryStworzylKontakt(string liniaZDanymi);
Adresat rozszyfrujDaneKontaktu(string liniaZDanymi);

vector <Uzytkownik> odczytajUzytkownikowZPliku();
void zapiszDoPlikuUzytkownicy(vector <Uzytkownik> uzytkownicy);
void zarejestrujUzytkownika();
void zmienHaslo(int idZalogowanegoUzytkownika, vector <Uzytkownik> uzytkownicy);

void uruchomMenuUzytkownika(int idZalogowanegoUzytkownika, vector <Uzytkownik> uzytkownicy);
void wyswietlMenuLogowania();
void logowanie();

int main() {
    wyswietlMenuLogowania();
}

Adresat dodajKontakt(vector<Adresat> kontakty, int idZalogowanegoUzytkownika, int ostatniZajetyAdresID) {
    cout<<"Dodawanie nowego kontaktu."<<endl;
    Adresat kontaktDoDodania;
    cin.sync();
    cout<<"Podaj imie: ";
    getline(cin,kontaktDoDodania.imie);
    cout<<"Podaj nazwisko: ";
    getline(cin,kontaktDoDodania.nazwisko);
    cout<<"Podaj email: ";
    cin.ignore();
    kontaktDoDodania.email=weryfikacjaPoprawnosciMaila();
    cout<<"Podaj numer telefonu: ";
    kontaktDoDodania.numerTelefonu=weryfikacjaPoprawnosciNumeru();
    cout<<"Podaj adres: ";
    cin.ignore();
    getline(cin,kontaktDoDodania.adres);
    kontaktDoDodania.id=ostatniZajetyAdresID+1;
    cout<<"Kontakt zostal dodany prawidlowo"<<endl;
    system("pause");
    kontaktDoDodania.idUzytkownika=idZalogowanegoUzytkownika;
    return kontaktDoDodania;
}
int okreslenieKolejnegoNumeruIdDlaKontaktuNaPodstawiePlikuTymczasowegoOrazPamieci(vector <Adresat> kontakty) {
    vector <int> numeryIDKontaktow;

    if(kontakty.size()>0) {
        for(int i=0; i<kontakty.size(); i++) {
            numeryIDKontaktow.push_back(kontakty[i].id);
        }
    }
    fstream plikTymczasowy;
    plikTymczasowy.open("Adresaci_tymczasowy.txt",ios::in);
    if(plikTymczasowy.good()==true) {
        string liniaZDanymi;
        int i=0;
        while(getline(plikTymczasowy,liniaZDanymi)) {
            int indeksWykryciaZnakuPodzialu=0;
            for(int k=0; k<liniaZDanymi.length(); k++) {
                if(liniaZDanymi[k]=='|') {
                    indeksWykryciaZnakuPodzialu=k;
                    string idString(liniaZDanymi,0,indeksWykryciaZnakuPodzialu);
                    int id = atoi(idString.c_str());
                    numeryIDKontaktow.push_back(id);
                    i++;
                    break;
                }
            }
        }
        plikTymczasowy.close();
    }

    if(numeryIDKontaktow.size()>0) {
        return *max_element(numeryIDKontaktow.begin(),numeryIDKontaktow.end());
    } else {
        return 0;
    }

}
int weryfikacjaPoprawnosciNumeru() {
    int poprawnyNumerTelefonu=0;
    while(!(cin>>poprawnyNumerTelefonu)) {
        cout<<"Podano nieprawidlowy numer.Sprobuj ponownie: ";
        cin.clear();
        cin.sync();
    }
    return poprawnyNumerTelefonu;
}
string weryfikacjaPoprawnosciMaila() {
    string weryfikowanyAdresMail;
    getline(cin,weryfikowanyAdresMail);
    while(1) {
        for(int i=0; i<weryfikowanyAdresMail.length(); i++) {
            if(weryfikowanyAdresMail[i]==' ') {
                cout<<"Podano nieprawidlowy adres e-mail(zawiera spacje). Sprobuj ponownie: ";
                getline(cin,weryfikowanyAdresMail);
                i=0;
                break;
            } else if(i==weryfikowanyAdresMail.length()-1) {
                return weryfikowanyAdresMail;
            }
        }
    }
}
void wyswietlanieWszystkichKontaktow(vector <Adresat> kontakty, int idZalogowanegoUzytkownika) {
    int iloscKontaktowDlaDanegoUzytkownika=0;
    for(int i=0; i<kontakty.size(); i++) {
        if(kontakty[i].idUzytkownika==idZalogowanegoUzytkownika) {
            iloscKontaktowDlaDanegoUzytkownika++;
        }
    }
    if(iloscKontaktowDlaDanegoUzytkownika>0) {
        for (int i=0; i<=kontakty.size()-1; i++) {
            if(kontakty[i].id!=0) {
                if(kontakty[i].nazwisko!="usunieto") {
                    if(kontakty[i].idUzytkownika==idZalogowanegoUzytkownika) {
                        cout<<"ID: "<<kontakty[i].id<<endl;
                        cout<<"Imie: "<<kontakty[i].imie<<endl;
                        cout<<"Nazwisko: "<<kontakty[i].nazwisko<<endl;
                        cout<<"Email: "<<kontakty[i].email<<endl;
                        cout<<"Numer telefonu: "<<kontakty[i].numerTelefonu<<endl;
                        cout<<"Adres: "<<kontakty[i].adres<<endl;
                        cout<<"_________________________________________________"<<endl;
                    }

                }
            }
        }
    } else cout<<"Brak kontaktow do wyswietlenia."<<endl;
}
void wyszukajPoImieniu(vector<Adresat> kontakty, int idZalogowanegoUzytkownika) {
    if(kontakty.size()>0) {
        string imie;
        int iloscWyswietlen=0;
        cout<<"Wyszukiwane imie: ";
        cin.sync();
        getline(cin,imie);
        for(int i=0; i<=kontakty.size(); i++) {
            if(imie==kontakty[i].imie&&kontakty[i].idUzytkownika==idZalogowanegoUzytkownika) {
                cout<<"ID: "<<kontakty[i].id<<endl;
                cout<<"Imie: "<<kontakty[i].imie<<endl;
                cout<<"Nazwisko: "<<kontakty[i].nazwisko<<endl;
                cout<<"Email: "<<kontakty[i].email<<endl;
                cout<<"Numer telefonu: "<<kontakty[i].numerTelefonu<<endl;
                cout<<"Adres: "<<kontakty[i].adres<<endl;
                cout<<"_________________________________________________"<<endl;
                iloscWyswietlen++;
            }
        }
        if(iloscWyswietlen==0) {
            system("cls");
            cout<<"Brak kontaktow dla slowa kluczowego: "<<imie<<endl;
        }
    } else cout<<"Brak kontaktow do wyswietlenia, rozpocznij dodawanie"<<endl;
    system("pause");
}
void wyszukajPoNazwisku(vector<Adresat> kontakty, int idZalogowanegoUzytkownika) {
    if(kontakty.size()>0) {
        string nazwisko;
        int iloscWyswietlen=0;
        cout<<"Wyszukiwane nazwisko: ";
        getline(cin,nazwisko);
        for(int i=0; i<=kontakty.size(); i++) {
            if(nazwisko==kontakty[i].nazwisko&&kontakty[i].idUzytkownika==idZalogowanegoUzytkownika) {
                cout<<"ID: "<<kontakty[i].id<<endl;
                cout<<"Imie: "<<kontakty[i].nazwisko<<endl;
                cout<<"Nazwisko: "<<kontakty[i].nazwisko<<endl;
                cout<<"Email: "<<kontakty[i].email<<endl;
                cout<<"Numer telefonu: "<<kontakty[i].numerTelefonu<<endl;
                cout<<"Adres: "<<kontakty[i].adres<<endl;
                cout<<"_________________________________________________"<<endl;
                iloscWyswietlen++;
            }
        }
        if(iloscWyswietlen==0) {
            system("cls");
            cout<<"Brak kontaktow dla slowa kluczowego: "<<nazwisko<<endl;
        }
    } else cout<<"Brak kontaktow do wyswietlenia, rozpocznij dodawanie"<<endl;
    system("pause");
}
void usunAdresata(vector <Adresat> &kontakty, int idZalogowanegoUzytkownika) {
    if(kontakty.size()>0) {
        system("cls");
        int numerIDAdresataDoUsuniecia;
        wyswietlanieWszystkichKontaktow(kontakty, idZalogowanegoUzytkownika);
        cout<<"Podaj numer ID adresata, ktory ma zostac usuniety: ";
        cin>>numerIDAdresataDoUsuniecia;
        if(kontakty.size()==1) {
            cout<<"Nie mozna usunac kontaktu. Dokonaj edycji."<<endl;
            system("pause");
        } else {
            for(int i=0; i<kontakty.size(); i++) {
                if(kontakty[i].id==numerIDAdresataDoUsuniecia) {
                    if(idZalogowanegoUzytkownika==kontakty[i].idUzytkownika) {
                        cout<<"Czy jestes pewnie ze chcesz usunac adresata z przypisanym "<<numerIDAdresataDoUsuniecia<<" numerem ID? Jezeli tak wcisnij t"<<endl;
                        char wybor;
                        wybor=_getch();
                        if(wybor=='t') {
                            kontakty.erase(kontakty.begin()+i);
                            cout<<"Kontakt zostal usuniety"<<endl;
                            system("pause");
                            zapiszDoPlikuKontakty(kontakty, idZalogowanegoUzytkownika);
                            break;
                        } else cout<<"Kontakt nie zostal usuniety"<<endl;
                    } else {
                        cout<<"Brak takiego kontaktu na Twojej liscie"<<endl;
                        system("pause");
                    }
                }
                if(i==kontakty.size()-1) {
                    cout<<"Nie ma na liscie kontaktu z numerem: "<<numerIDAdresataDoUsuniecia<<endl;
                    system("pause");
                    przeniesienieKontaktowZPlikuTymczasowego();
                    break;
                }
            }
        }
    } else {
        cout<<"Brak kontaktow na Twojej liscie. Rozpocznij dodawanie kontaktow"<<endl;
    }
}
void modyfikujKontakt(vector <Adresat> &kontakty, int idZalogowanegoUzytkownika) {
    if(kontakty.size()>0) {
        system("cls");
        int numerIDDoModyfikacji=0;
        wyswietlanieWszystkichKontaktow(kontakty, idZalogowanegoUzytkownika);
        cout<<"Podaj ID kontaktu do modyfikacji: ";
        cin>>numerIDDoModyfikacji;
        for(int i=0; i<kontakty.size(); i++) {
            if(numerIDDoModyfikacji==kontakty[i].id&&kontakty[i].idUzytkownika==idZalogowanegoUzytkownika) {
                system("cls");
                cout<<"ID modyfikowanego kontaktu to: "<<numerIDDoModyfikacji<<endl;
                cout<<"1. Zmien imie "<<endl;
                cout<<"2. Zmien nazwisko "<<endl;
                cout<<"3. Zmien numer telefonu "<<endl;
                cout<<"4. Zmien e-mail "<<endl;
                cout<<"5. Zmien adres "<<endl;
                cout<<"6. Powrot do menu"<<endl;
                char wybor;
                wybor=_getch();
                if(wybor=='1') {
                    string imie;
                    system("cls");
                    cout<<"Podaj imie: ";
                    cin>>imie;
                    kontakty[i].imie=imie;
                    cout<<"Dane zostaly zaktualizowane"<<endl;
                    system("pause");
                } else if(wybor=='2') {
                    string nazwisko;
                    system("cls");
                    cout<<"Podaj nazwisko: ";
                    cin>>nazwisko;
                    kontakty[i].nazwisko=nazwisko;
                    cout<<"Dane zostaly zaktualizowane"<<endl;
                    system("pause");
                } else if(wybor=='3') {
                    system("cls");
                    int numerTelefonu;
                    cout<<"Podaj numer telefonu: ";
                    cin>>numerTelefonu;
                    kontakty[i].numerTelefonu=numerTelefonu;
                    cout<<"Dane zostaly zaktualizowane"<<endl;
                    system("pause");
                } else if(wybor=='4') {
                    system("cls");
                    string email;
                    cout<<"Podaj email: ";
                    cin>>email;
                    kontakty[i].email=email;
                    cout<<"Dane zostaly zaktualizowane"<<endl;
                    system("pause");
                } else if(wybor=='5') {
                    system("cls");
                    cin.sync();
                    string adres;
                    cout<<"Podaj adres: ";
                    getline(cin,adres);
                    kontakty[i].adres=adres;
                    cout<<"Dane zostaly zaktualizowane"<<endl;
                    system("pause");
                }
            } else if(kontakty[i].idUzytkownika!=idZalogowanegoUzytkownika) {
                cout<<"Nie ma mozliwosci modyfikacji kontaktu"<<endl;
                break;
            }
        }
        zapiszDoPlikuKontakty(kontakty, idZalogowanegoUzytkownika);
    } else {
        system("cls");
        cout<<"Brak kontaktow do modyfikacji. Rozpocznij dodawanie kontaktow."<<endl;
        system("pause");
    }

}

void zapiszDoPlikuKontakty(vector <Adresat> kontakty, int idZalogowanegoUzytkownika) {

    odczytajKontantyZPlikuDlaUzytkownikaNiezalogowanegoIZapiszDoPlikuTymczasowego(idZalogowanegoUzytkownika);
    odczytajKontantyZPlikuDlaUzytkownikaZalogowanegoIZapiszDoPliku(idZalogowanegoUzytkownika);

    fstream plik;
    plik.open("Adresaci.txt",ios::out);

    for (int i=0; i<=kontakty.size()-1; i++) {
        plik<<kontakty[i].id<<"|";
        plik<<kontakty[i].idUzytkownika<<"|";
        plik<<kontakty[i].imie<<"|";
        plik<<kontakty[i].nazwisko<<"|";
        plik<<kontakty[i].email<<"|";
        plik<<kontakty[i].numerTelefonu<<"|";
        plik<<kontakty[i].adres<<"|";
        plik<<endl;
    }
    plik.close();
    cout<<"\nPlik z lista kontaktow zostal pomyslnie zaktualizowany.\n";
    system("pause");
    przeniesienieKontaktowZPlikuTymczasowego();
}
void odczytajKontantyZPlikuDlaUzytkownikaNiezalogowanegoIZapiszDoPlikuTymczasowego(int idAktualnieZalogowanegoUzytkownika) {
    vector <Adresat> kontakty;
    fstream plik;
    plik.open("Adresaci.txt",ios::in);
    if(plik.good()==true) {
        string liniaZDanymi;
        int i=1;
        while(getline(plik,liniaZDanymi)) {
            if(idAktualnieZalogowanegoUzytkownika!=wyznaczIDUzytkownikaKtoryStworzylKontakt(liniaZDanymi)) {
                kontakty.push_back(rozszyfrujDaneKontaktu(liniaZDanymi));
            }
            i++;
        }
    }
    plik.close();
    fstream plikTymczasowy;
    plikTymczasowy.open("Adresaci_tymczasowy.txt",ios::out);

    if(kontakty.size()>=1) {
        for (int i=0; i<=kontakty.size()-1; i++) {
            plikTymczasowy<<kontakty[i].id<<"|";
            plikTymczasowy<<kontakty[i].idUzytkownika<<"|";
            plikTymczasowy<<kontakty[i].imie<<"|";
            plikTymczasowy<<kontakty[i].nazwisko<<"|";
            plikTymczasowy<<kontakty[i].email<<"|";
            plikTymczasowy<<kontakty[i].numerTelefonu<<"|";
            plikTymczasowy<<kontakty[i].adres<<"|";
            plikTymczasowy<<endl;
        }
    }
    plikTymczasowy.close();
}
void odczytajKontantyZPlikuDlaUzytkownikaZalogowanegoIZapiszDoPliku(int idAktualnieZalogowanegoUzytkownika) {
    vector <Adresat> kontakty;
    fstream plik;
    plik.open("Adresaci.txt",ios::in);
    if(plik.good()==true) {
        string liniaZDanymi;
        int i=1;
        while(getline(plik,liniaZDanymi)) {
            if(idAktualnieZalogowanegoUzytkownika==wyznaczIDUzytkownikaKtoryStworzylKontakt(liniaZDanymi)) {
                kontakty.push_back(rozszyfrujDaneKontaktu(liniaZDanymi));
                i++;
            }
        }
        plik.close();
    }

    fstream plikTymczasowy;
    plikTymczasowy.open("Adresaci.txt",ios::out);

    if(kontakty.size()>=1) {
        for (int i=0; i<=kontakty.size()-1; i++) {
            plikTymczasowy<<kontakty[i].id<<"|";
            plikTymczasowy<<kontakty[i].idUzytkownika<<"|";
            plikTymczasowy<<kontakty[i].imie<<"|";
            plikTymczasowy<<kontakty[i].nazwisko<<"|";
            plikTymczasowy<<kontakty[i].email<<"|";
            plikTymczasowy<<kontakty[i].numerTelefonu<<"|";
            plikTymczasowy<<kontakty[i].adres<<"|";
            plikTymczasowy<<endl;
        }
    }
    plikTymczasowy.close();
}
void przeniesienieKontaktowZPlikuTymczasowego() {
    vector <Adresat> kontaktyDoPrzeniesienia;
    fstream plikTymczasowy;
    plikTymczasowy.open("Adresaci_tymczasowy.txt",ios::in);
    if(plikTymczasowy.good()==true) {
        string liniaZDanymi;
        int i=1;
        while(getline(plikTymczasowy,liniaZDanymi)) {
            kontaktyDoPrzeniesienia.push_back(rozszyfrujDaneKontaktu(liniaZDanymi));
            i++;
        }
        plikTymczasowy.close();
    }
    fstream plik;
    plik.open("Adresaci.txt",ios::out|ios::app);

    if(kontaktyDoPrzeniesienia.size()>=1) {
        for (int i=0; i<=kontaktyDoPrzeniesienia.size()-1; i++) {
            plik<<kontaktyDoPrzeniesienia[i].id<<"|";
            plik<<kontaktyDoPrzeniesienia[i].idUzytkownika<<"|";
            plik<<kontaktyDoPrzeniesienia[i].imie<<"|";
            plik<<kontaktyDoPrzeniesienia[i].nazwisko<<"|";
            plik<<kontaktyDoPrzeniesienia[i].email<<"|";
            plik<<kontaktyDoPrzeniesienia[i].numerTelefonu<<"|";
            plik<<kontaktyDoPrzeniesienia[i].adres<<"|";
            plik<<endl;
        }
    }
    plikTymczasowy.close();
    remove("Adresaci_tymczasowy.txt");
}
vector <Adresat> odczytajKontaktyZPlikuDlaUzytkownikaZalogowanego(int idAktualnieZalogowanegoUzytkownika) {
    vector <Adresat> kontakty;
    fstream plik;
    plik.open("Adresaci.txt",ios::in);
    if(plik.good()==true) {
        string liniaZDanymi;
        int i=1;
        while(getline(plik,liniaZDanymi)) {
            if(idAktualnieZalogowanegoUzytkownika==wyznaczIDUzytkownikaKtoryStworzylKontakt(liniaZDanymi)) {
                kontakty.push_back(rozszyfrujDaneKontaktu(liniaZDanymi));
            }

            i++;
        }
        plik.close();
        return kontakty;
    } else {


        cout<<"Brak kontaktow - rozpocznij dodawanie"<<endl;
        system("pause");
        return kontakty;
    }
}
int wyznaczIDUzytkownikaKtoryStworzylKontakt(string liniaZDanymi) {
    int const ILOSC_KOLUMN=2;
    int tablicaIndeksowPodzialow[ILOSC_KOLUMN];
    int indeksWykryciaZnakuPodzialu=0;
    for(int k=0; k<liniaZDanymi.length(); k++) {
        if(liniaZDanymi[k]=='|') {
            tablicaIndeksowPodzialow[indeksWykryciaZnakuPodzialu]=k;
            indeksWykryciaZnakuPodzialu++;
        }
    }
    string idZalogowanegoUzytkownikaString (liniaZDanymi,tablicaIndeksowPodzialow[0]+1,tablicaIndeksowPodzialow[1]-tablicaIndeksowPodzialow[0]-1);
    int idZalogowanegoUzytkownika = atoi(idZalogowanegoUzytkownikaString.c_str());
    string idString(liniaZDanymi,0,tablicaIndeksowPodzialow[0]);
    int id = atoi(idString.c_str());
    return idZalogowanegoUzytkownika;
}
Adresat rozszyfrujDaneKontaktu(string liniaZDanymi) {
    int const ILOSC_KOLUMN=7;
    int tablicaIndeksowPodzialow[ILOSC_KOLUMN];
    int indeksWykryciaZnakuPodzialu=0;
    for(int k=0; k<liniaZDanymi.length(); k++) {
        if(liniaZDanymi[k]=='|') {
            tablicaIndeksowPodzialow[indeksWykryciaZnakuPodzialu]=k;
            indeksWykryciaZnakuPodzialu++;
        }
    }



    string idString(liniaZDanymi,0,tablicaIndeksowPodzialow[0]);
    int id = atoi(idString.c_str());

    string idZalogowanegoUzytkownikaString (liniaZDanymi,tablicaIndeksowPodzialow[0]+1,tablicaIndeksowPodzialow[1]-tablicaIndeksowPodzialow[0]-1);
    int idZalogowanegoUzytkownika = atoi(idZalogowanegoUzytkownikaString.c_str());



    string imie(liniaZDanymi,tablicaIndeksowPodzialow[1]+1,tablicaIndeksowPodzialow[2]-tablicaIndeksowPodzialow[1]-1);

    string nazwisko(liniaZDanymi,tablicaIndeksowPodzialow[2]+1,tablicaIndeksowPodzialow[3]-tablicaIndeksowPodzialow[2]-1);

    string email(liniaZDanymi,tablicaIndeksowPodzialow[3]+1,tablicaIndeksowPodzialow[4]-tablicaIndeksowPodzialow[3]-1);

    string numerString(liniaZDanymi,tablicaIndeksowPodzialow[4]+1,tablicaIndeksowPodzialow[5]-tablicaIndeksowPodzialow[4]-1);

    int numer=atoi(numerString.c_str());
    string adres(liniaZDanymi,tablicaIndeksowPodzialow[5]+1,tablicaIndeksowPodzialow[6]-tablicaIndeksowPodzialow[5]-1);

    Adresat kontaktDoPrzesylaniaDanych;
    kontaktDoPrzesylaniaDanych.id=id;
    kontaktDoPrzesylaniaDanych.imie=imie;
    kontaktDoPrzesylaniaDanych.nazwisko=nazwisko;
    kontaktDoPrzesylaniaDanych.email=email;
    kontaktDoPrzesylaniaDanych.numerTelefonu=numer;
    kontaktDoPrzesylaniaDanych.adres=adres;
    kontaktDoPrzesylaniaDanych.idUzytkownika=idZalogowanegoUzytkownika;
    return kontaktDoPrzesylaniaDanych;
}

void zarejestrujUzytkownika() {
    vector <Uzytkownik> uzytkownicy=odczytajUzytkownikowZPliku();
    Uzytkownik uzytkownikDoDodania;
    cout<<"Podaj nazwe uzytkownika: ";
    cin>>uzytkownikDoDodania.nazwa;
    cout<<"Podaj haslo uzytkownika: ";
    cin>>uzytkownikDoDodania.haslo;
    uzytkownikDoDodania.idUzytkownika=uzytkownicy.size()+1;
    uzytkownicy.push_back(uzytkownikDoDodania);
    cout<<"Uzytkownik zostal zarejestrowany."<<endl;
    zapiszDoPlikuUzytkownicy(uzytkownicy);
}
void zmienHaslo(int idZalogowanegoUzytkownika, vector <Uzytkownik> uzytkownicy) {
    for(int i=0; i<uzytkownicy.size(); i++) {
        if(idZalogowanegoUzytkownika==uzytkownicy[i].idUzytkownika) {
            system("cls");
            cout<<"Podaj nowe haslo: ";
            string noweHaslo;
            cin>>noweHaslo;
            uzytkownicy[i].haslo=noweHaslo;
            cout<<"Haslo zostalo zmienione"<<endl;
            zapiszDoPlikuUzytkownicy(uzytkownicy);
            break;
        }
    }
}
void zapiszDoPlikuUzytkownicy(vector <Uzytkownik> uzytkownicy) {

    fstream plik;
    plik.open("Uzytkownicy.txt",ios::out);

    for (int i=0; i<=uzytkownicy.size()-1; i++) {
        plik<<uzytkownicy[i].idUzytkownika<<"|";
        plik<<uzytkownicy[i].nazwa<<"|";
        plik<<uzytkownicy[i].haslo<<"|"<<endl;
    }
    plik.close();
    cout<<"\nPlik z lista uzytkownikow zostal pomyslnie zaktualizowany.\n";
    system("pause");
}
vector <Uzytkownik> odczytajUzytkownikowZPliku() {
    vector <Uzytkownik> uzytkownicy;
    fstream plik;
    plik.open("Uzytkownicy.txt",ios::in);
    if(plik.good()==true) {
        string liniaZDanymi;
        int iloscUzytkownikow=0;
        int i=1;
        int const ILOSC_KOLUMN=3;
        int tablicaIndeksowPodzialow[ILOSC_KOLUMN];
        while(getline(plik,liniaZDanymi)) {
            int indeksWykryciaZnakuPodzialu=0;
            for(int k=0; k<liniaZDanymi.length(); k++) {
                if(liniaZDanymi[k]=='|') {
                    tablicaIndeksowPodzialow[indeksWykryciaZnakuPodzialu]=k;
                    indeksWykryciaZnakuPodzialu++;
                }
            }
            string idString(liniaZDanymi,0,tablicaIndeksowPodzialow[0]);
            int id = atoi(idString.c_str());
            string nazwa(liniaZDanymi,tablicaIndeksowPodzialow[0]+1,tablicaIndeksowPodzialow[1]-tablicaIndeksowPodzialow[0]-1);
            string haslo(liniaZDanymi,tablicaIndeksowPodzialow[1]+1,tablicaIndeksowPodzialow[2]-tablicaIndeksowPodzialow[1]-1);
            Uzytkownik uzytkownikDoPrzesylaniaDanych;
            uzytkownikDoPrzesylaniaDanych.idUzytkownika=id;
            uzytkownikDoPrzesylaniaDanych.nazwa=nazwa;
            uzytkownikDoPrzesylaniaDanych.haslo=haslo;
            uzytkownicy.push_back(uzytkownikDoPrzesylaniaDanych);
            i++;
        }
        plik.close();
        return uzytkownicy;
    } else {
        return uzytkownicy;
    }
}


void wyswietlMenuLogowania() {
    char wybor;
    while(1) {
        system("cls");

        cout<<"1. Logowanie"<<endl;
        cout<<"2. Rejestracja"<<endl;
        cout<<"3. Zamknij program"<<endl;

        wybor=_getch();
        if(wybor=='1') {
            logowanie();
        } else if(wybor=='2') {
            system("cls");
            zarejestrujUzytkownika();
        }  else if(wybor=='3') {
            exit(0);
        }
    }
}
void logowanie() {
    string podanaNazwa, podaneHaslo="";
    vector <Uzytkownik> uzytkownicy=odczytajUzytkownikowZPliku();
    system("cls");
    if(uzytkownicy.size()==0) {
        system("cls");
        cout<<"Brak zarejestrowanych uzytkownikow. Zarejestruj sie."<<endl;
        system("pause");
        wyswietlMenuLogowania();
    }
    cout<<"Podaj nazwe uzytkownika: ";
    cin>>podanaNazwa;

    for(int i=0; i<=uzytkownicy.size(); i++) {
        if(podanaNazwa==uzytkownicy[i].nazwa) {
            cout<<"Podaj haslo uzytkownika: ";
            cin>>podaneHaslo;
            if(podaneHaslo==uzytkownicy[i].haslo) {
                //odczytajKontantyZPlikuDlaUzytkownikaNiezalogowanegoIZapiszDoPlikuTymczasowego(uzytkownicy[i].idUzytkownika);
                //odczytajKontantyZPlikuDlaUzytkownikaZalogowanegoIZapiszDoPliku(uzytkownicy[i].idUzytkownika);
                uruchomMenuUzytkownika(uzytkownicy[i].idUzytkownika,uzytkownicy);
            } else cout<<"Podano nieprawidlowe haslo"<<endl;
        } else if(i==uzytkownicy.size()&&podaneHaslo=="") cout<<"Uzytkownik o nazwie "<<podanaNazwa<<" nie posiada jeszcze konta"<<endl;
    }
    system("pause");
}
void uruchomMenuUzytkownika(int idZalogowanegoUzytkownika, vector <Uzytkownik> uzytkownicy) {
    vector <Adresat> kontakty=odczytajKontaktyZPlikuDlaUzytkownikaZalogowanego(idZalogowanegoUzytkownika);
    int iloscKontakow=kontakty.size();
    char wybor;
    while(1) {
        system("cls");
        cout<<"1. Dodaj adresata"<<endl;
        cout<<"2. Wyszukaj po imieniu"<<endl;
        cout<<"3. Wyszukaj po nazwisku"<<endl;
        cout<<"4. Wyswietl liste wszystkich kontaktow"<<endl;
        cout<<"5. Usun adresata"<<endl;
        cout<<"6. Edytuj adresata"<<endl;
        cout<<"7. Zmien haslo"<<endl;
        cout<<"9. Wyloguj"<<endl;

        wybor=_getch();
        if(wybor=='1') {
            system("cls");
            kontakty.push_back(dodajKontakt(kontakty,idZalogowanegoUzytkownika, okreslenieKolejnegoNumeruIdDlaKontaktuNaPodstawiePlikuTymczasowegoOrazPamieci(kontakty)));
            zapiszDoPlikuKontakty(kontakty, idZalogowanegoUzytkownika);
        } else if(wybor=='2') {
            system("cls");
            wyszukajPoImieniu(kontakty, idZalogowanegoUzytkownika);
        } else if(wybor=='3') {
            system("cls");
            wyszukajPoNazwisku(kontakty, idZalogowanegoUzytkownika);
        } else if(wybor=='4') {
            system("cls");
            wyswietlanieWszystkichKontaktow(kontakty, idZalogowanegoUzytkownika);
            system("pause");
        } else if(wybor=='5') {
            usunAdresata(kontakty, idZalogowanegoUzytkownika);
        } else if(wybor=='6') {
            modyfikujKontakt(kontakty, idZalogowanegoUzytkownika);
        } else if(wybor=='7') {
            zmienHaslo(idZalogowanegoUzytkownika, uzytkownicy);
        } else if(wybor=='9') {
            wyswietlMenuLogowania();
            zapiszDoPlikuKontakty(kontakty, idZalogowanegoUzytkownika);

        }
    }
}
