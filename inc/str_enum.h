/*
 * str_enum.h
 *
 *  Created on: 29 лист. 2016
 *      Author: root
 */


#pragma once


#define AWESOME_MAKE_ENUM(name, ...) enum class name { __VA_ARGS__, __COUNT}; \
inline std::string ToString(name value){ \
std::string enumName = #name; \
std::string str = #__VA_ARGS__; \
int len = str.length(); \
std::vector<std::string> strings; \
std::ostringstream temp; \
for(int i = 0; i < len; i ++) { \
if(isspace(str[i])) continue; \
        else if(str[i] == ',') { \
        strings.push_back(temp.str()); \
        temp.str(std::string());\
        } \
        else temp<< str[i]; \
} \
strings.push_back(temp.str()); \
return strings[static_cast<int>(value)]; } \

