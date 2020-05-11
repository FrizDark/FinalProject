//
// Created by Dmitriy Steshenko on 28.04.2020.
//

#ifndef FINALPROJECTCPP_MYSTRING_H
#define FINALPROJECTCPP_MYSTRING_H

#include "IncludeLib.h"
using namespace std;

struct MyString{
    int length = 0;
    char* str = nullptr;
    void setMyString(char s[]){
        length = strlen(s);
        str = new char[length + 1];
        strcpy(str, s);
    }
    void delMyString(){
        length = 0;
        delete str;
    }
    void toFirstUpper(){
        str[0] = toupper(str[0]);
    }
    void toAllLower(){
        for (int i = 0; i < length; i++)
        {
            str[i] = tolower(str[i]);
        }
    }
    void oneSpace(){
        int j = 0;
        for (int i = 0; i < length; i++)
        {
            if (!isspace(str[i]))
            {
                str[j] = str[i]; j++;
            }
            else if (isspace(str[i]) && !isspace(str[i - 1]))
            {
                str[j] = str[i]; j++;
            }
        }

        str[j] = '\0';
        length = j;
    }


};

#endif //FINALPROJECTCPP_MYSTRING_H

