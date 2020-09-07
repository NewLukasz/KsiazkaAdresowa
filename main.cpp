#include <iostream>
#include <windows.h>
#include <conio.h>
#include <iomanip>
#include <fstream>
#include <vector>


using namespace std;

struct Adresat {
    string imie="",nazwisko="",email="",adres="";
    int id,numerTelefonu;
};

struct Uzytkownik{
    int idUzytkownika;
    string nazwa,haslo;
};

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

Adresat dodajKontakt(vector<Adresat> kontakty) {

    cout<<"Dodawanie nowego kontaktu."<<endl;
    Adresat kontaktDoDodania;
    cin.sync();
    cout<<"Podaj imie: ";
    getline(cin,kontaktDoDodania.imie);

    //cin.ignore();
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

    if(kontakty.size()>0) {
        kontaktDoDodania.id=kontakty[kontakty.size()-1].id+1;
    } else kontaktDoDodania.id=1;

    cout<<"Kontakt zostal dodany prawidlowo"<<endl;

    system("pause");

    return kontaktDoDodania;
}

void wyswietlanieWszystkichKontaktow(vector <Adresat> kontakty) {
    if(kontakty.size()>0) {
        for (int i=0; i<=kontakty.size()-1; i++) {
            if(kontakty[i].id!=0) {
                if(kontakty[i].nazwisko!="usunieto") {
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
    } else cout<<"Brak kontaktow do wyswietlenia."<<endl;

}

void wyszukajPoImieniu(vector<Adresat> kontakty) {
    if(kontakty.size()>0) {
        string imie;
        int iloscWyswietlen=0;
        cout<<"Wyszukiwane imie: ";
        getline(cin,imie);
        for(int i=0; i<=kontakty.size(); i++) {
            if(imie==kontakty[i].imie) {
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

void wyszukajPoNazwisku(vector<Adresat> kontakty) {
    if(kontakty.size()>0) {
        string nazwisko;
        int iloscWyswietlen=0;
        cout<<"Wyszukiwane nazwisko: ";
        getline(cin,nazwisko);
        for(int i=0; i<=kontakty.size(); i++) {
            if(nazwisko==kontakty[i].nazwisko) {
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


void zapiszDoPlikuKontakty(vector <Adresat> kontakty) {

    fstream plik;
    plik.open("ListaKontaktow.txt",ios::out);

    for (int i=0; i<=kontakty.size()-1; i++) {
        plik<<kontakty[i].id;
        plik<<"|";
        plik<<kontakty[i].imie;
        plik<<"|";
        plik<<kontakty[i].nazwisko;
        plik<<"|";
        plik<<kontakty[i].email;
        plik<<"|";
        plik<<kontakty[i].numerTelefonu;
        plik<<"|";
        plik<<kontakty[i].adres;
        plik<<"|";
        plik<<endl;
    }
    plik.close();
    cout<<"\nPlik z lista kontaktow zostal pomyslnie zaktualizowany.\n";
    system("pause");
}

vector <Adresat> odczytajKontaktyZPliku() {
    vector <Adresat> kontakty;
    fstream plik;
    plik.open("ListaKontaktow.txt",ios::in);
    if(plik.good()==true) {
        string liniaZDanymi;
        int iloscKontaktow=0;
        int i=1;
        int const ILOSC_KOLUMN=6;
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
            string imie(liniaZDanymi,tablicaIndeksowPodzialow[0]+1,tablicaIndeksowPodzialow[1]-tablicaIndeksowPodzialow[0]-1);
            string nazwisko(liniaZDanymi,tablicaIndeksowPodzialow[1]+1,tablicaIndeksowPodzialow[2]-tablicaIndeksowPodzialow[1]-1);
            string email(liniaZDanymi,tablicaIndeksowPodzialow[2]+1,tablicaIndeksowPodzialow[3]-tablicaIndeksowPodzialow[2]-1);
            string numerString(liniaZDanymi,tablicaIndeksowPodzialow[3]+1,tablicaIndeksowPodzialow[4]-tablicaIndeksowPodzialow[3]-1);
            int numer=atoi(numerString.c_str());
            string adres(liniaZDanymi,tablicaIndeksowPodzialow[4]+1,tablicaIndeksowPodzialow[5]-tablicaIndeksowPodzialow[4]-1);

            Adresat kontaktDoPrzesylaniaDanych;
            kontaktDoPrzesylaniaDanych.id=id;
            kontaktDoPrzesylaniaDanych.imie=imie;
            kontaktDoPrzesylaniaDanych.nazwisko=nazwisko;
            kontaktDoPrzesylaniaDanych.email=email;
            kontaktDoPrzesylaniaDanych.numerTelefonu=numer;
            kontaktDoPrzesylaniaDanych.adres=adres;

            kontakty.push_back(kontaktDoPrzesylaniaDanych);
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

void usunAdresata(vector <Adresat> &kontakty) {
    if(kontakty.size()>0) {
        system("cls");
        int numerIDAdresataDoUsuniecia;
        wyswietlanieWszystkichKontaktow(kontakty);
        cout<<"Podaj numer ID adresata, ktory ma zostac usuniety: ";
        cin>>numerIDAdresataDoUsuniecia;
        cout<<"Czy jestes pewnie ze chcesz usunac adresata z przypisanym "<<numerIDAdresataDoUsuniecia<<" numerem ID? Jezeli tak wcisnij t"<<endl;
        char wybor;
        wybor=_getch();
        if(wybor=='t') {
            if(kontakty.size()==1) {
                cout<<"Nie mozna usunac kontaktu. Dokonaj edycji."<<endl;
                system("pause");
            } else {
                for(int i=0; i<kontakty.size(); i++) {
                    if(kontakty[i].id==numerIDAdresataDoUsuniecia) {
                        kontakty.erase(kontakty.begin()+i);
                    }
                }
                cout<<"Kontakt zostal usuniety"<<endl;
                system("pause");
            }
        } else cout<<"Kontakt nie zostal usuniety."<<endl;
    } else {
        cout<<"Brak kontaktow na Twojej liscie. Rozpocznij dodawanie kontaktow"<<endl;
        system("pause");
    }


    zapiszDoPlikuKontakty(kontakty);
}

void modyfikujKontakt(vector <Adresat> &kontakty) {
    if(kontakty.size()>0) {
        system("cls");
        int numerIDDoModyfikacji=0;
        wyswietlanieWszystkichKontaktow(kontakty);
        cout<<"Podaj ID kontaktu do modyfikacji: ";
        cin>>numerIDDoModyfikacji;
        system("cls");
        cout<<"ID modyfikowanego kontaktu to: "<<numerIDDoModyfikacji<<endl;
        cout<<"1. Zmien imie "<<endl;
        cout<<"2. Zmien nazwisko "<<endl;
        cout<<"3. Zmien numer telefonu "<<endl;
        cout<<"4. Zmien e-mail "<<endl;
        cout<<"5. Zmien adres "<<endl;
        cout<<"6. Powrot do menu"<<endl;

        for(int i=0; i<kontakty.size(); i++) {
            if(numerIDDoModyfikacji==kontakty[i].id) {
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
            }
        }

    } else {
        cout<<"Brak kontaktow do modyfikacji. Rozpocznij dodawanie kontaktow."<<endl;
        system("pause");
    }
    zapiszDoPlikuKontakty(kontakty);
}

void wyswietlMenuUzytkownika() {
    vector <Adresat> kontakty=odczytajKontaktyZPliku();
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
        cout<<"9. Zakoncz"<<endl;

        wybor=_getch();
        if(wybor=='1') {
            system("cls");
            kontakty.push_back(dodajKontakt(kontakty));
            zapiszDoPlikuKontakty(kontakty);
        } else if(wybor=='2') {
            system("cls");
            wyszukajPoImieniu(kontakty);
        } else if(wybor=='3') {
            system("cls");
            wyszukajPoNazwisku(kontakty);
        } else if(wybor=='4') {
            system("cls");
            wyswietlanieWszystkichKontaktow(kontakty);
            system("pause");
        } else if(wybor=='5') {
            usunAdresata(kontakty);
        } else if(wybor=='6') {
            modyfikujKontakt(kontakty);
        } else if(wybor=='9') {
            zapiszDoPlikuKontakty(kontakty);
            exit(0);
        }
    }
}

void zapiszDoPlikuUzytkownicy(vector <Uzytkownik> uzytkownicy) {

    fstream plik;
    plik.open("ListaUzytkownikow.txt",ios::out);

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
    plik.open("ListaUzytkownikow.txt",ios::in);
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
        cout<<"Brak uzytkownikow - rozpocznij dodawanie"<<endl;
        return uzytkownicy;
    }
}


int main() {

    char wybor;
    while(1) {
        system("cls");

        cout<<"1. Logowanie"<<endl;
        cout<<"2. Rejestracja"<<endl;
        cout<<"3. Zamknij program"<<endl;

        wybor=_getch();
        if(wybor=='1') {
            wyswietlMenuUzytkownika();
        } else if(wybor=='2') {
            system("cls");

            cout<<"Proces rejestracji..."<<endl;



            vector <Uzytkownik> uzytkownicy=odczytajUzytkownikowZPliku();
            Uzytkownik uzytkownikDoDodania;

            cout<<"Podaj nazwe uzytkownika: ";
            cin>>uzytkownikDoDodania.nazwa;
            cout<<"Podaj haslo uzytkownika: ";
            cin>>uzytkownikDoDodania.haslo;
            uzytkownikDoDodania.idUzytkownika=uzytkownicy.size()+1;

            uzytkownicy.push_back(uzytkownikDoDodania);

            cout<<"Nazwa to: "<<uzytkownicy[uzytkownicy.size()-1].nazwa<<endl;
            cout<<"Haslo to: "<<uzytkownicy[uzytkownicy.size()-1].haslo<<endl;
            cout<<"IdUzytkownika to: "<<uzytkownicy[uzytkownicy.size()-1].idUzytkownika<<endl;

            zapiszDoPlikuUzytkownicy(uzytkownicy);

            //system("pause");

        }  else if(wybor=='3') {
            exit(0);
        }
    }
}
