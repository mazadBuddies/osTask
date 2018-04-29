/* 
 * File:   main.cpp
 * Author: bodey & Hashem
 *
 * Created on April 28, 2018, 8:24 PM
 */


#include <bits/stdc++.h>

#define notFound    "NULL"
#define sep         "=>"
#define rep(vec)    for(int i=0;i<(vec).size();i++)
using namespace std;
typedef string type;
typedef vector<type> vstr;
typedef map<string, string> shellMP;
shellMP mp;
#define shellFile   "shellMapping.txt"
#define filepathhelp "help_and_manual.txt"
vector<string> strVec ;
vector<string> strVecManual;

void printToFile();
vector<string> readFromFile(string filePath);
string copy(string str, int length, int start);
void Addcommand();
void printMenu();
void getManual();
void startCommand();
string getOsName();
void setCommandAsMap(vstr comnds);
char* toChar(string str);
int haveParam(type command);
bool isWindows();
bool isLinux();

type getMapByIndex(map<string, string> mp, int index){
    shellMP::iterator it = mp.begin();
    while(index--)it++;
    return it->first;
}

void Menu();
template <class T>
void lsVec(vector<T> vec){
    for(int i=0;i<vec.size();i++)
        cout<<vec[i]<<endl;
}//end of lsVec

template <class T>
void lsVecWithLineNumber(vector<T> vec){
    for(int i=0;i<vec.size();i++)
        cout<<i+1<<" - "<<vec[i]<<endl;
}//end of lsVec

 void lsMp(map<string , string> maps){
     int i=0;
     map<string , string>::iterator mpIt = maps.begin();
     while(mpIt != maps.end()){
         cout<<i+1<<" - "<<mpIt->first<<endl;
         ++mpIt;
         i++;
     }
 }
string copy(string str, int length, int start){
    char str2 [length+1];
    str.copy(str2,length, start);
    str2[length]='\0';
    return str2;
}

type getFirstWord(type str){
    return ((int)str.find(" ") != -1)?str.substr(0, (int)str.find(" ")):str;
}

void Addcommand(){
    cout<<"ENTER COMMAND LIKE THIS 'DOS=>TERMINAL'";
    getchar();
    string line;
    getline(cin, line);
    cout<<line;
    if((int )line.find(sep)!=-1){
        strVec.push_back(line);
        printToFile();
        strVec = readFromFile(shellFile);
        setCommandAsMap(strVec);
    }else{
       cout<<"NOT VALID COMMAND";
    }
}

void printMenu(){
    cout<<"1-Start Command\n2-Help And Manual\n3-Add command\n4-Delete Command\n5-Exit\n";
}

void getManual(){
    strVecManual = readFromFile(filepathhelp);
    lsVec(strVecManual);
}

void startCommand(){
    system("clear");
    lsMp(mp);
    int i;
    cin>>i;
    if(i <= strVec.size()){
        type fullCommand = getFirstWord(mp[getMapByIndex(mp, i-1)]);
        int param = haveParam(getMapByIndex(mp, i-1));
        if(param > 0){
            cout<<"enter "<<param<<" parameters \n";
            type paramStr;
            while(param--){
                cout<<">";
                cin>>paramStr;
                fullCommand+= " " + paramStr;
            }//end of while 
        }//end of if
        
        system(toChar(fullCommand));
    }else{
        cout<<"NOT FOUND\n";
    }
}

int haveParam(type command){
    int miusCnt = 0;
    for(int i = 0;i < command.length();i++)
        if(command[i] == '-')
            miusCnt++;
    return miusCnt;
}

void deleteCommand(){
    lsVecWithLineNumber(strVec);
    int i;
    cin>>i;
    if(i<= (int)strVec.size()){
        strVec.erase(strVec.begin()+(i-1));
        printToFile();
        strVec = readFromFile(shellFile);
        setCommandAsMap(strVec);
    }else{
        cout<<"NOT VALID INDEX";
    }
    
}
void Menu(){
   bool flag = false;
   while (flag!=true){
        printMenu();
        int i;
        cin >> i;
        switch(i){
            case 1 : startCommand();
                break;
            case 2 : getManual();
                break;
            case 3 : Addcommand();
                break;
            case 4: deleteCommand();
                break;
            case 5 : flag=true;
                break;
            default: cout << "Your choice is wrong choose again!";
                break;
        }//end of switch 
    }//end of while
}//end of function

char* toChar(string str){
    char* str2 = new char [str.length()+1];
    strcpy(str2, str.c_str());
    return str2;
}

vector<string> readFromFile(string filePath)
{
    type ln;
    vstr cmnds;
    ifstream myfile (filePath);
    if (myfile.is_open()){
      while ( getline (myfile,ln) ){
        cmnds.push_back(ln);
      }
      myfile.close();
    }
    else cmnds.push_back(notFound);
 return cmnds;
}

string getOsName(){
    #ifdef _WIN32
        return "Windows 32-bit";
    #elif _WIN64
        return "Windows 64-bit";
    #elif __unix || __unix__
        return "Unix";
    #elif __APPLE__ || __MACH__
        return "Mac OSX";
    #elif __linux__
        return "Linux";
    #elif __FreeBSD__
        return "FreeBSD";
    #else
        return "Other";
    #endif
}

void setCommandAsMap(vstr comnds){
    type dosCommand, terCommand;
    for(int i=0; i<comnds.size(); i++){
        int index = (int)comnds[i].find(sep);
        string strDos , strTer;
       if(index != -1){
           strDos = copy(comnds[i], index, 0);
           strTer = copy(comnds[i], comnds[i].length()-(strDos.length()+2), strDos.length()+2);
           if(isLinux())
               mp[strDos] = strTer;
           else if(isWindows())
               mp[strTer] = strDos;
       }//end of if
    }//end of for
}//end of function setCommandAsMap

void printToFile(){
    FILE* fptr = fopen(shellFile, "w");
    if(strVec.size()!=0){
        fprintf(fptr,"%s\n",toChar(strVec[0]));
        fclose(fptr);
        FILE* fptr = fopen(shellFile, "a");
        for(int i=1; i<strVec.size();i++)
            fprintf(fptr,"%s\n",toChar(strVec[i]));
        fclose(fptr);
    }//end of if
}//end of function

bool isGood(vector<string> cmnds){
    return (cmnds[0] != notFound);
}

bool isWindows(){
    return (getOsName()[0] == 'W');
}

bool isLinux(){
    return (getOsName()[0] == 'L' || getOsName()[0] == 'U');
}

int main(int argc, char** argv) {
    strVec = readFromFile(shellFile);
    setCommandAsMap(strVec);
    Menu();
    return 0;
}