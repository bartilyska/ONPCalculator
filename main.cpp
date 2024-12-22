#include <iostream>
#include <string.h>
#define ROZMIAR 11
#define INF 1000000000
using namespace std;
class Wezel
{
public:
    char* info;
    Wezel* nast;
    Wezel(const char* dana): nast(nullptr)
    {
        info = new char[strlen(dana) + 1];
        strcpy(info, dana);
    };
    ~Wezel()
    {
        delete[] info;
    }
};
class Lista
{
    Wezel* glowa;
    Wezel* ogon;
public:
    Lista()
    {
        glowa = nullptr;
        ogon = nullptr;
    }
    void dodajWezelStart(char* dana)
    {
        Wezel* nowy = new Wezel(dana);
        if (glowa == nullptr)
        {
             glowa = nowy;
             ogon = nowy;
        }
        else
        {
            nowy->nast = glowa;
            glowa = nowy;
        }
    }
    void dodajWezelKoniec(char* dana)
    {
        Wezel* nowy = new Wezel(dana);
        if (glowa == nullptr)
        {
            glowa = nowy;
            ogon = nowy;
        }
        else
        {
            ogon->nast=nowy;
            ogon=nowy;
        }
    }
    void usunGlowe()
    {
        if (glowa == nullptr)
            return;
        Wezel* tmp = glowa;
        glowa = glowa->nast;
        delete tmp;
    }
    void wyswietl() const
    {
        Wezel* tmp = glowa;
        while (tmp != nullptr)
        {
            printf("%s ",tmp->info);
            tmp = tmp->nast;
        }
        printf("\n");
    }
    char* zwrocPierwszy() const
    {
        if (glowa != nullptr)
            return glowa->info;
        else return 0;
    }
    bool Pusty() const
    {
        if(glowa==nullptr)
            return true;
        else return false;
    }
    ~Lista()
    {
        while (glowa != nullptr)
        {
            Wezel* tmp = glowa;
            glowa = tmp->nast;
            delete tmp;
        }
    }
};
int priority(char * oper)
{
    if(oper[0]==',')
        return 0;
    else if(oper[0]=='+' || oper[0]=='-')
        return 1;
    else if(oper[0]=='*' || oper[0]=='/')
        return 2;
    else
        return 3;
}
char * intToChar(int liczba)
{
    int dl=snprintf(nullptr,0,"%d",liczba);
    char * odp= new char[dl+1];
    snprintf(odp,dl+1,"%d",liczba);
    return odp;
}
void obliczenia(Lista& onp);
void test()
{
    Lista onp; //kolejka
    Lista operatory; //stos
    char* wejscie= new char[ROZMIAR];
    while (scanf("%s",wejscie)==1)
    {
        if (wejscie[0]=='.')
            break;
        if(wejscie[0]>='0' && wejscie[0]<='9')
            onp.dodajWezelKoniec(wejscie);
        else if (wejscie[0]=='(')
            operatory.dodajWezelStart(wejscie);
        else if (wejscie[0]==')')
        {
            while(!operatory.Pusty())
            {
                char *token=new char[strlen(operatory.zwrocPierwszy())+1];
                strcpy(token,operatory.zwrocPierwszy());
                operatory.usunGlowe();
                if(token[0]=='(')
                {
                    delete[] token;
                    break;
                }
                else
                {
                    int liczprzecinki=1;
                    if(token[0]!=',')
                    {
                        if(strcmp(token,"MAX")==0 || strcmp(token,"MIN")==0)
                            token=strcat(token,"1"); //jezeli to min lub max to dodaj 1
                        onp.dodajWezelKoniec(token);
                    }
                    while(strcmp(token,",")==0) //jezeli przecinki to policz
                    {
                        liczprzecinki++;
                        delete[] token;
                        token=new char[strlen(operatory.zwrocPierwszy())+1];
                        strcpy(token,operatory.zwrocPierwszy());
                        operatory.usunGlowe();
                    }
                    if(liczprzecinki>1) // jak byl przecinek to bedziemy wyrzucac funkcje
                    {
                        char *op=new char[strlen(operatory.zwrocPierwszy()) + 1];
                        strcpy(op,operatory.zwrocPierwszy());
                        if(strcmp(op,"MAX")==0 || strcmp(op,"MIN")==0 || strcmp(op,"IF")==0)
                        {
                            operatory.usunGlowe();
                            char * doklej = intToChar(liczprzecinki);
                            if(strcmp(op,"IF")!=0) //do ifa nie doklejaj
                            {
                                op=strcat(op,doklej);
                            }
                            onp.dodajWezelKoniec(op);
                            delete[] doklej;
                            delete[] op;
                            delete[] token;
                            liczprzecinki=1;
                            break;
                        }
                        delete[] op;
                    }
                }
                delete[] token;
            }
        }
        else
        {
            int priorytetIn=priority(wejscie); //priorytet wchodzacego
            while(!operatory.Pusty())
            {
                char* top = new char[strlen(operatory.zwrocPierwszy()) + 1];
                strcpy(top, operatory.zwrocPierwszy());
                operatory.usunGlowe();
                if(strcmp(top,"(")==0 || priority(top)<priorytetIn ||
                (strcmp(wejscie,",")==0 && strcmp(top,",")==0) ||(priority(wejscie)==3 && priority(top)==3))
                { // wtedy wchodzacy idzie od razu na wyjscie (top wraca na swoje miejsce)
                    operatory.dodajWezelStart(top);
                    delete[] top;
                    break;
                }
                if(strcmp(top,"MAX")==0 || strcmp(top,"MIN")==0) //doklej 1 jezeli to max lub min
                    top=strcat(top,"1");
                onp.dodajWezelKoniec(top);
                delete[] top;
            }
            operatory.dodajWezelStart(wejscie);
        }
    }
    delete[] wejscie;
    while(!operatory.Pusty())
    {
        char *op=new char[strlen(operatory.zwrocPierwszy()) + 1];
        strcpy(op,operatory.zwrocPierwszy());
        operatory.usunGlowe();
        if(strcmp(op,"MAX")==0 || strcmp(op,"MIN")==0)
            op=strcat(op,"1");
        onp.dodajWezelKoniec(op);
        delete[] op;
    }
    onp.wyswietl();
    obliczenia(onp);
}
void obliczenia(Lista& onp)
{
    Lista liczby;
    bool czydziel0=0;
    while(!onp.Pusty())
    {
        char *token=new char[strlen(onp.zwrocPierwszy())+1];
        strcpy(token,onp.zwrocPierwszy());
        onp.usunGlowe();
        if(token[0]>='0' && token[0]<='9')
            liczby.dodajWezelStart(token);
        else
        {
            printf("%s ",token);
            liczby.wyswietl();
            char *op3=new char[ROZMIAR];
            char *op2=new char[ROZMIAR];
            char *op1=new char[ROZMIAR];
            if(token[0]=='+' || token[0]=='-' || token[0]=='/' || token[0]=='*')
            {
                strcpy(op2,liczby.zwrocPierwszy());
                liczby.usunGlowe();
                strcpy(op1,liczby.zwrocPierwszy());
                liczby.usunGlowe();
                int operand2=atoi(op2);
                int operand1=atoi(op1);
                int wynik;
                if(token[0]=='+')
                    wynik=operand1+operand2;
                else if(token[0]=='-')
                    wynik=operand1-operand2;
                else if(token[0]=='*')
                    wynik=operand1*operand2;
                else
                {
                    if(operand2==0)
                    {
                        printf("ERROR\n");
                        czydziel0=1;
                        delete[]op3;
                        delete[]op2;
                        delete[]op1;
                        delete[]token;
                        break;
                    }
                    else
                        wynik=operand1/operand2;
                }
                char *wyn=intToChar(wynik);
                liczby.dodajWezelStart(wyn);
                delete[]wyn;
            }
            else if(strcmp(token,"N")==0)
            {
                strcpy(op1,liczby.zwrocPierwszy());
                liczby.usunGlowe();
                int operand1=atoi(op1);
                operand1*=(-1);
                char *wyn=intToChar(operand1);
                liczby.dodajWezelStart(wyn);
                delete[] wyn;
            }
            else if(strcmp(token,"IF")==0)
            {
                strcpy(op3,liczby.zwrocPierwszy());
                liczby.usunGlowe();
                strcpy(op2,liczby.zwrocPierwszy());
                liczby.usunGlowe();
                strcpy(op1,liczby.zwrocPierwszy());
                liczby.usunGlowe();
                int operand1=atoi(op1);
                if(operand1>0)
                    liczby.dodajWezelStart(op2);
                else liczby.dodajWezelStart(op3);
            }
            else
            {
                char * przes= token+3; //przesuwa wsk o 3
                int ile=atoi(przes);
                if(token[1]=='A')//max
                {
                    int maxi=-INF;
                    for(int i=0;i<ile;i++)
                    {
                        strcpy(op1,liczby.zwrocPierwszy());
                        liczby.usunGlowe();
                        int operand=atoi(op1);
                        if(operand>maxi)
                            maxi=operand;
                    }
                    char *wyn=intToChar(maxi);
                    liczby.dodajWezelStart(wyn);
                    delete[] wyn;
                }
                else//min
                {
                    int mini=INF;
                    for(int i=0;i<ile;i++)
                    {
                        strcpy(op1,liczby.zwrocPierwszy());
                        liczby.usunGlowe();
                        int operand=atoi(op1);
                        if(operand<mini)
                            mini=operand;
                    }
                    char *wyn=intToChar(mini);
                    liczby.dodajWezelStart(wyn);
                    delete[] wyn;
                }
            }
            delete[]op3;
            delete[]op2;
            delete[]op1;
        }
        delete[]token;
    }
    if(czydziel0==0)
        liczby.wyswietl();
}
int main()
{
    int testy;
    scanf("%d",&testy);
    while(testy--)
    {
        test();
    }
    return 0;
}
