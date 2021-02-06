// Assembler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include<map>
#include <iostream>
#include<fstream>
#include<string>
#include<sstream>
using namespace std;
class Sympol {
    map <string, string>SympolTable = { {"R0","0"} ,{"R1","1"} ,{"R2","2"} ,{"R3","3"},{"R4","4"} ,{"R5","5"} ,{"R6","6"} ,{"R7","7"} ,{"R8","8"}
  ,{"R9","9"} ,{"R10","9"} ,{"R11","11"} ,{"R12","12"} ,{"R13","13"} ,{"R14","14"} ,{"R15","15"},{"SCREEN","16384"} ,{"KBD","24576"},{"SP","0"}
  ,{"LCL","1"},{"ARG","2"},{"THIS","3"},{"THAT","4"} };
    int var_pos = 16;
public:
    bool is_exist(string symbol) {
        return(SympolTable.find(symbol) != SympolTable.end());
    }
    void add_label(string label, int count) {
        SympolTable.insert(pair<string, string>(label, to_string(count)));
    }
    void add_var(string var) {
        if (!is_exist(var)) {
            SympolTable.insert(pair<string, string>(var, to_string(var_pos)));
            var_pos++;
        }
        return;
    }
    string return_symbol_val(string sympol) {
        return SympolTable[sympol];
    }
    void label_tour(string name) {
        int count = 0;
        string line;
        ifstream infile;
        infile.open(name + ".asm");
        if (infile.is_open()) {
            while (getline(infile, line)) {
                if (line == "" || line[0] == '/')
                    continue;
                else {
                    line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
                    if (line[0] == '(') {
                        add_label(line.substr(1, line.size() - 2), count);
                        continue;
                    }
                    count++;
                    }
                }
            }
    }
};
class code {
public:
};
class ccode :public code{
    map <string, string> destination = { {"null","000"} ,{"M","001"} ,{"D","010"} ,{"MD","011"} ,{"A","100"} ,{"AM","101"} ,{"AD","110"} ,{"AMD","111"} };
    map <string, string>compute = { {"0","0101010"},{"1","0111111"},{"-1","0111010"},{"D","0001100"},{"A","0110000"},{"M","1110000"},{"!D","0001101"},{"!A","0110001"}, {"!M","1110001"},{"-D","0001111"},{"-A","0110011"},{"-M","1110011"},{"D+1","0011111"},{"A+1","0110111"},{"M+1","1110111"},{"D-1","0001110"},{"A-1","0110010"},{"M-1","1110010"},{"D+A","0000010"},{"D+M","1000010"},{"D-A","0010011"},{"D-M","1010011"},{"A-D","0000111"},{"M-D","1000111"},{"D&A","0000000"},{"D&M","1000000"},{"D|A","0010101"},{"D|M","1010101"} };
    map <string, string>jump = { {"null","000"},{"JGT","001"},{"JEQ","010"} ,{"JGE","011"} ,{"JLT","100"} ,{"JNE","101"} ,{"JLE","110"} ,{"JMP","111"} };
public:
    string to_code(string field,string type) {
        if (type == "dest")
            return destination[field];
        else if(type == "comp")
            return compute[field];
        else if(type=="jmp")
            return jump[field];
    }
};
class acode {
    string toBinary(int n)
    {
        string r ,complete_16="";
        int count;
        while (n != 0) { r = (n % 2 == 0 ? "0" : "1") + r; n /= 2; }
        count = r.size();
        while (count < r.capacity()) {
            count++;
            complete_16 += '0';
        }
        return (complete_16+r);
    }
    public:
        string to_code(int add){
            return toBinary(add);
        }
};
class instruction {
public:
    virtual string binary_instruction()=0;
    virtual void parser(string command, Sympol obj) {};
};
class Ainstruction :public instruction {
    string op_code = "0";
    int address;
public:
     void parser(string command,Sympol obj) override {
         if (obj.is_exist(command.substr(1)))
             address = stoi(obj.return_symbol_val(command.substr(1)));
         else
             address = stoi(command.substr(1));
    };
     string binary_instruction() override{
        acode obj;
        string add = obj.to_code(address);
        return op_code + add;
    };

    
};
class Cinstruction :public instruction {
    string op_code = "111";
    string dest;
    string comp;
    string jmp;
public:
       void parser(string command, Sympol obj) override {
           auto equal = command.find('=');
           auto semicolon = command.find(';');
           auto comment = command.find("//");
           if (comment == std::string::npos) {
               if (equal == std::string::npos) {
                   if (semicolon == std::string::npos) {
                       dest = "null";
                       jmp = "null";
                       comp = command;
                   }
                   else {
                       dest = "null";
                       jmp = command.substr(semicolon + 1);
                       comp = command.substr(0, semicolon);
                   }
               }
               else {
                   if (semicolon == std::string::npos) {
                       dest = command.substr(0, equal);
                       jmp = "null";
                       comp = command.substr(equal + 1);
                   }
                   else {
                       comp = command.substr(equal + 1, semicolon - equal);
                       jmp = command.substr(semicolon + 1);
                       dest = command.substr(0, equal);

                   }
               }
           }else{
               if (equal == std::string::npos) {
                   if (semicolon == std::string::npos) {
                       dest = "null";
                       jmp = "null";
                       comp = command.substr(0,comment-2);
                   }
                   else {
                       dest = "null";
                       jmp = command.substr(semicolon + 1,comment-2);
                       comp = command.substr(0, semicolon);
                   }
               }
               else {
                   if (semicolon == std::string::npos) {
                       dest = command.substr(0, equal);
                       jmp = "null";
                       comp = command.substr(equal + 1,comment-2);
                   }
                   else {
                       comp = command.substr(equal + 1, semicolon - equal);
                       jmp = command.substr(semicolon + 1,comment-2);
                       dest = command.substr(0, equal);

                   }
               }
           }
             
    };
        string binary_instruction() override {
            ccode obj;
            string cd = obj.to_code(dest,"dest");
            string cc = obj.to_code(comp, "comp");
            string cj = obj.to_code(jmp, "jmp");
            return (op_code + cc + cd + cj);
        };

};


int main()
{
    string name,line;
    Sympol symp;
    cin >> name;
    ifstream infile;
    ofstream outfile((name + ".hack"));
    symp.label_tour(name);

        infile.open(name + ".asm");
        line = "";
        while (getline(infile, line)) {
    
            if (line == "" || line[0] == '/')
                continue;
            else {
                line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
                if (line[0] == '@') {
                    if (!isdigit(line[1])) {
                        symp.add_var(line.substr(1));
                    }
                    Ainstruction obj;
                    obj.parser(line, symp);
                    outfile << obj.binary_instruction() << endl;

                }
                else if(line[0] != '('){
                    Cinstruction obj;
                    obj.parser(line, symp);
                    outfile << obj.binary_instruction() << endl;
                }

            }

        }
        infile.close();
        outfile.close();
    }
        



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
