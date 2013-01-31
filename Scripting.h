#ifndef __SCRIPTING_H__
#define __SCRIPTING_H__

#include "CommandInterface.h"
#include <string>
#include <vector>

namespace ImgProc {

typedef void(*funcListener)(std::vector<std::string>* args, ImgProc::CommandInterface* interface);

void echoFunc(std::vector<std::string>* args, ImgProc::CommandInterface* interface);
void quitFunc(std::vector<std::string>* args, ImgProc::CommandInterface* interface);
void saveFunc(std::vector<std::string>* args, ImgProc::CommandInterface* interface);
void loadFunc(std::vector<std::string>* args, ImgProc::CommandInterface* interface);
void neighborFunc(std::vector<std::string>* args, ImgProc::CommandInterface* interface);
void filterFunc(std::vector<std::string>* args, ImgProc::CommandInterface* interface);
void invertFunc(std::vector<std::string>* args, ImgProc::CommandInterface* interface);
void copyFunc(std::vector<std::string>* args, ImgProc::CommandInterface* interface);
void colorCopyFunc(std::vector<std::string>* args, ImgProc::CommandInterface* interface);
void histoFunc(std::vector<std::string>* args, ImgProc::CommandInterface* interface);

#define NFUNCS 12
extern std::pair<std::string*, funcListener> *(functions[NFUNCS]);

}

#endif //__SCRIPTING_H__