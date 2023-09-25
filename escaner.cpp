#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctype.h>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <iomanip>
#include <limits.h>
#include <stack>


using namespace std;

struct scanner {

  //Operadores y palabras reservadas
  string op = ",+-=></*%!";
  vector<string> keywords = { "for","bool","if","else","read","write","int", "str", "char","getpos","print","in","true","false"};

  //Leer el código
  string path = "lenguaje.txt";
  string line;
  ifstream myfile;
  
  scanner() {
        myfile.open(path);
  }

  char get_char(string line, int &index){
      char aux = line[index];
      index+=1;
      return aux;
  }
  
  char getnext_char(string line, int &index){
    index+=1;
    return line[index];
  }
  
  char peek_char(string line, int index){
    return line[index+1];
  }

  void throw_error(int line,int ch,string type){
    cout<<"Error of "<<type<<" found at ("<<line<<":"<<ch<<")"<<endl;
  }

  void debug(int line,int chi,string type,char ch){
    cout<<"DEBUG - "<< type << ch <<" in ("<< line<<":"<< chi <<")"<<endl;
  }

  void debug(int line,int chi,string type,string ch){
    cout<<"DEBUG - "<< type << ch <<" in ("<< line<<":"<< chi <<")"<<endl;
  }

  void scann(){
    
    int line_count = 1, token_col = 0;
    int index = 0;
    stack< pair<int,int> > delim;
    stack< pair<int,int> > delim1;
    stack< pair<int,int> > delim2;
    char ch;
    
    cout<< "INFO - start scanning"<<endl;
  
    while (getline (myfile, line)) {
      
      for(int i = 0; i< line.length();i++){
        
        if(line[i] == ' ') continue; //espacios
        
        //operadores aritméticos-booleanos
        if(op.find(line[i]) != std::string::npos) { 
          debug(line_count,i,"operator ",line[i]); 
        }
        //asignación
        if(line[i] == ':'){
          ch = peek_char(line, i);
          if(ch == '='){
            index+=1;
            debug(line_count,i,"assignation ",line[i]); 
          }
          else {
            throw_error(line_count,i,"assignation");
          }
        }
        
        //Comentarios
        if(line[i] == '#'){
          debug(line_count,i,"comment ",line[i]); 
          break;
        }

        //ids y keywords
        if(isalpha(line[i])){
          string word = ""; word+=line[i];
          vector<string>::iterator it;
          
          while(isalpha(peek_char(line,i)) || peek_char(line,i)=='_'){
            word+=getnext_char(line,i);
          }
          
          if (find(keywords.begin(), keywords.end(), word) != keywords.end())
            {debug(line_count,i,"keyword ", word);}
          else
            {debug(line_count,i,"identifyer ",word);}
        }
        //Números
        if(isdigit(line[i])){
          string number = "";number+=line[i];
          while(isdigit(peek_char(line,i))){
            number+=getnext_char(line,i);
          }
          
          if (number[0] == '0' && number.size() > 1)
              throw_error(line_count,i,"integer definition");
          else if (std::stoi(number) > 2147483647)
              throw_error(line_count,i,"integer limit");
          else
              debug(line_count,i,"integer ",number);
        }
        //Delimitadores
        if(line[i]=='{'){
          debug(line_count,i,"open delimiter ",line[i]);
          pair<int,int> aux(line_count,i);
          delim.push(aux);
        }
        else if(line[i]=='}'){
          if(delim.empty()){
            throw_error(line_count,i,"extra bracket");
          }
          else{
            debug(line_count,i,"close delimiter ",line[i]);
            delim.pop();
          }
        }
        
        if(line[i]=='['){
          debug(line_count,i,"open sqbracket ",line[i]);
          pair<int,int> aux(line_count,i);
          delim1.push(aux);
        }
        else if(line[i]==']'){
          if(delim1.empty()){
            throw_error(line_count,i,"extra sqbracket");
          }
          else{
            debug(line_count,i,"close sqbracket ",line[i]);
            delim1.pop();
          }
        }

        if(line[i]=='('){
          debug(line_count,i,"open parenthesis ",line[i]);
          pair<int,int> aux(line_count,i);
          delim2.push(aux);
        }
        else if(line[i]==')'){
          if(delim2.empty()){
            throw_error(line_count,i,"extra parenthesis");
          }
          else{
            debug(line_count,i,"close parenthesis ",line[i]);
            delim2.pop();
          }
        }
        
        //Cadenas
        if(line[i] == '"'){
          string str = "";
          str+=line[i];
          i+=1;
          str+=line[i];
          while(line[i] != '"'){
            str+=getnext_char(line,i);
            if(i>line.length()) {
              throw_error(line_count,i,"missing \"");
              break;
            }
          }
          debug(line_count,i,"string ",str);
        }
        
      }
      line_count++;
    }
    if(!delim.empty()){
      while(!delim.empty()){
        pair<int,int> top = delim.top();
        throw_error(top.first,top.second,"missing }");
        delim.pop();
      }
      
    }
    if(!delim1.empty()){
      while(!delim1.empty()){
        pair<int,int> top = delim1.top();
        throw_error(top.first,top.second,"missing ]");
        delim1.pop();
      }
    }
    if(!delim2.empty()){
      while(!delim2.empty()){
        pair<int,int> top = delim2.top();
        throw_error(top.first,top.second,"missing )");
        delim2.pop();
      }
    }
  }


};


int main() {

  scanner a;
  a.scann();
  
}
