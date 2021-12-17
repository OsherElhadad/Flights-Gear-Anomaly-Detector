/*
 * run2.cpp
 *
 *  Created on: 8 áãöî× 2019
 *      Author: Eli
 */

#include <iostream>
#include <fstream>
#include "commands.h"
#include "CLI.h"

using namespace std;


class STDtest:public DefaultIO{
    ifstream in;
    ofstream out;
public:
    STDtest(string inputFile,string outputFile){
        in.open(inputFile);
        out.open(outputFile);
    }
    virtual string read(){
        string s;
        in>>s;
        return s;
    }
    virtual void write(string text){
        out<<text;
    }

    virtual void write(float f){
        out<<f;
    }

    virtual void read(float* f){
        in>>*f;
    }

    void close(){
        if(in.is_open())
            in.close();
        if(out.is_open())
            out.close();
    }
    ~STDtest(){
        close();
    }
};

void check(string outputFile,string expectedOutputFile){
    size_t chk[]={31,62,63,74,75,86,87,98,99,110,111};
    ifstream st(outputFile);
    ifstream ex(expectedOutputFile);
    size_t i=1,j=0;
    string lst,lex;
    while(!st.eof() && !ex.eof()){
        getline(st,lst);
        getline(ex,lex);
        if(i<13 && lst.compare(lex)!=0){ // 12
            cout<<"line "<<i<<" expected: "<<lex<<" you got "<<lst<<endl;
            cout<<"wrong output (-1)"<<endl;
        }else
        if(j<11 && i==chk[j]){
            if(lst.compare(lex)!=0){ // 88
                cout<<"line "<<i<<" expected: "<<lex<<" you got "<<lst<<endl;
                cout<<"wrong output (-8)"<<endl;
            }
            j++;
        }
        i++;
    }
    if(j<11)
        cout<<"wrong output size (-"<<(11-j)*8<<")"<<endl;
    st.close();
    ex.close();
}

//small test
int main(){
    STDtest std("input.txt","output.txt");
    CLI cli(&std);
    cli.start();
    std.close();
    check("output.txt","expectedOutput.txt");
    cout<<"done"<<endl;
    return 0;
}