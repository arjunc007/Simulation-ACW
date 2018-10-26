#pragma once
#include<fstream>

#define LOG(x) {	\
std::ofstream fout("output.txt", std::ofstream::app);	\
fout << x <<"\n";	\
fout.close(); \
}

#define CLEARLOG	\
std::ofstream fout("output.txt", std::ofstream::out | std::ofstream::trunc);	\
fout.close();	\
