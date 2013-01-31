/**
 * Pennyworth - A new smarthome protocol.
 * Copyright (C) 2012  Dream-Crusher Labs LLC
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
/*
 * CommandInterface.cpp
 *
 *  Created on: Jun 22, 2012
 *      Author: jmonk
 */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string>
#include <iostream>
#include <fstream>
//#include <utility>
#include <algorithm>
#include "StringVar.h"

#include "CommandInterface.h"
#include "Util.h"
#include "Server.h"

#include "Image.h"
#include "Filter.h"
#include "MaskFilter.h"
#include "Invert.h"
#include "InvertChannel.h"
#include "HistogramEqualize.h"

using namespace std;

namespace ImgProc {

typedef void(*funcListener)(vector<string>* args, CommandInterface* interface);

void echoFunc(vector<string>* args, CommandInterface* interface);
void quitFunc(vector<string>* args, CommandInterface* interface);
void saveFunc(vector<string>* args, CommandInterface* interface);
void loadFunc(vector<string>* args, CommandInterface* interface);
void neighborFunc(vector<string>* args, CommandInterface* interface);
void filterFunc(vector<string>* args, CommandInterface* interface);
void invertFunc(vector<string>* args, CommandInterface* interface);
void copyFunc(vector<string>* args, CommandInterface* interface);
void histoFunc(vector<string>* args, CommandInterface* interface);
/*
void listDevices(vector<string>* args);
void selFunc(vector<string>* args);
void rcommFunc(vector<string>* args);
void listFields(vector<string>* args);
void setField(vector<string>* args);
void printField(vector<string>* args);
void printDevice(vector<string>* args);
void listComms(vector<string>* args);
void delComm(vector<string>* args);
void newComm(vector<string>* args);
*/

#define NFUNCS 12

pair<string*, funcListener> *(functions[NFUNCS]) = {
	new pair<string*, funcListener>(new string("echo"), echoFunc),
	new pair<string*, funcListener>(new string("quit"), quitFunc),
	new pair<string*, funcListener>(new string("exit"), quitFunc),
	new pair<string*, funcListener>(new string("load"), loadFunc),
	new pair<string*, funcListener>(new string("save"), saveFunc),
	new pair<string*, funcListener>(new string("mask"), neighborFunc),
	new pair<string*, funcListener>(new string("invert"), invertFunc),
	new pair<string*, funcListener>(new string("filter"), filterFunc),
	new pair<string*, funcListener>(new string("copy"), copyFunc),
	new pair<string*, funcListener>(new string("histoeq"), histoFunc),
    /*
	new pair<string*, funcListener>(new string("list"), listDevices),
	new pair<string*, funcListener>(new string("select"), selFunc),
	new pair<string*, funcListener>(new string("rcomm"), rcommFunc),
	new pair<string*, funcListener>(new string("fields"), listFields),
	new pair<string*, funcListener>(new string("set"), setField),
	new pair<string*, funcListener>(new string("print"), printField),
	new pair<string*, funcListener>(new string("printd"), printDevice),
	new pair<string*, funcListener>(new string("comms"), listComms),
	new pair<string*, funcListener>(new string("close"), delComm),
	new pair<string*, funcListener>(new string("open"), newComm)
    */
};

CommandInterface* instance;
char* print_buffer;
int print_index;
int buffer_length;

#include <stdarg.h>

void copy_to_buffer(char* string, int length) {
	for (int i = 0; (i < length) && (print_index < buffer_length - 1); i++) {
		print_buffer[print_index++] = string[i];
	}
	print_buffer[print_index++] = '\0';
}

int print(const char* format, ...) {
	char buffer[512];
	int ret = 0;
	va_list args;
	va_start(args, format);

	if (print_buffer != NULL) {
		ret = vsprintf(buffer, format, args);
		copy_to_buffer(buffer, ret);
	} else {
		ret = vprintf(format, args);
	}

	return ret;
}

void commandHandleGlobal(char* line) {
	instance->commandHandle(line);
}

CommandInterface::CommandInterface(const char* prompt) {
	instance = this;
	rl_callback_handler_install(prompt, (rl_vcpfunc_t*) commandHandleGlobal);
    Server::getServer()->addListener(0, this);
}

CommandInterface::CommandInterface(int argc, char* argv[]) {
	instance = this;
    for (int i = 1; i < argc; i++) {
        char s[30];
        sprintf(s, "arg%d", i - 1);
        std::string str(s);
        std::string value(argv[i]);
       
        setVar(str, new StringVar(value));
    }
}

CommandInterface::~CommandInterface() {
	rl_callback_handler_remove();
    for (std::map<std::string,Variable*>::iterator it=vars.begin(); it!=vars.end(); ++it) {
        delete it->second;
    }
}

void CommandInterface::run() {
	rl_callback_read_char();
}

void CommandInterface::runScript() {
    std::string var0("arg0");
    StringVar* file = dynamic_cast<StringVar*>(getVar(var0));
    if (file == NULL) {
        printf("Problem getting filename...\n");
        exit(-1);
    }
    string filename = file->str;
    if (string::npos == filename.find(".i")) {
        filename = filename + ".i";
    }
    std::ifstream f(filename.c_str());
    if (!f.good()) {
        f.open(("scripts/" + filename).c_str());
    }
    char line[256];

    while (f.good()) {
        f.getline(line, 256);
        commandHandle(line);
    }
}

void CommandInterface::setVar(std::string var, Variable* value) {
    if (vars.find(var) != vars.end()) {
        delete vars[var];
    }
    vars[var] = value;
}

Variable* CommandInterface::getVar(std::string var) {
    return vars[var];
}

bool CommandInterface::hasVar(std::string var) {
    return vars.find(var) != vars.end();
}

void CommandInterface::commandHandle(char* line, char* output, int out_len) {
	print_buffer = output;
	print_index = 0;
	buffer_length = out_len;

	string l(line);
	if (l.size() > 0) {
		vector<string>* args = Util::split(l, ' ');
		int i;
		for (i = 0; i < NFUNCS; i++) {
			if (*(functions[i]->first) == (*args)[0]) {
				functions[i]->second(args, this);
				break;
			}
		}
		delete args;
		if (i == NFUNCS) {
			print("Invalid Command: %s %d\n", line, strlen(line));
			for (unsigned int i = 0; i < strlen(line); i++) {
				printf("%c-", line[i]);
			}
		}
	}
	print_buffer = NULL;
}

void CommandInterface::commandHandle(char* line) {
	string l(line);
	if (l.size() > 0) {
		vector<string>* args = Util::split(l, ' ');
		int i;
		for (i = 0; i < NFUNCS; i++) {
			if (*(functions[i]->first) == (*args)[0]) {
				functions[i]->second(args, this);
				break;
			}
		}
		delete args;
		if (i == NFUNCS) {
			print("Invalid Command: %s\n", line);
		}
		add_history(line);
	}
}

void CommandInterface::setPromptI(char* prompt) {
	rl_set_prompt(prompt);
}

void echoFunc(vector<string>* args, CommandInterface* interface) {
    for (unsigned int i = 1; i < args->size(); i++) {
        string var = (*args)[i];
        if (interface->hasVar(var)) {
            StringVar* varstr = (dynamic_cast<StringVar*>(interface->getVar(var)));
            if (varstr == NULL) {
                cout << var << " is not a string" << endl;
                return;
            } else {
                var = varstr->str;
            }
        }
        cout << var << endl;
    }
}

void quitFunc(vector<string>* args, CommandInterface* interface) {
    Server::getServer()->stop();
}

void loadFunc(vector<string>* args, CommandInterface* interface) {
    if (args->size() < 3) {
        cout << "Usage: " << (*args)[0] << " filename varname" << endl;
        cout << "\tLoads an image from target file" << endl;
        return;
    }
    string filename = (*args)[1];
    if (interface->hasVar(filename)) {
        StringVar* var = (dynamic_cast<StringVar*>(interface->getVar(filename)));
        if (var == NULL) {
            cout << (*args)[1] << " is not a string" << endl;
            return;
        } else {
            filename = var->str;
        }
    }
    string varname = (*args)[2];

    cout << "Loading " << filename << " into " << varname << endl;
    interface->setVar(varname, new Image(filename.c_str()));
}

void saveFunc(vector<string>* args, CommandInterface* interface) {
    if (args->size() < 3) {
        cout << "Usage: " << (*args)[0] << " filename varname" << endl;
        cout << "\tSaves an image into target file" << endl;
        return;
    }
    string filename = (*args)[1];
    if (interface->hasVar(filename)) {
        StringVar* var = (dynamic_cast<StringVar*>(interface->getVar(filename)));
        if (var == NULL) {
            cout << (*args)[1] << " is not a string" << endl;
            return;
        } else {
            filename = var->str;
        }
    }
    string varname = (*args)[2];

    Image* image = dynamic_cast<Image*>(interface->getVar(varname));
    if (image == NULL) {
        cout << varname << " is not an image" << endl;
        return;
    } else {
        cout << "Saving " << varname << " as " << filename << endl;
        image->save(filename.c_str());
    }
}

void neighborFunc(vector<string>* args, CommandInterface* interface) {
    if (args->size() < 11) {
        cout << "Usage: " << (*args)[0] << " varname S1 S2 S3 S4 S5 S6 S7 S8 S9" << endl;
        cout << "\tDefines a 3x3 mask filter" << endl;
        cout << "\tWhere Neighborhood mask is as follows" << endl;
        cout << "\t----------" << endl;
        cout << "\t|S1|S2|S3|" << endl;
        cout << "\t----------" << endl;
        cout << "\t|S4|S5|S6|" << endl;
        cout << "\t----------" << endl;
        cout << "\t|S7|S8|S9|" << endl;
        cout << "\t----------" << endl;
        return;
    }
    string varname = (*args)[1];
    float mask[3][3];
    int inOffset = 1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            mask[i][j] = ::atof((*args)[++inOffset].c_str()); 
            //cout << "mask[" << i << "][" << j << "] is " << mask[i][j] << endl;
        }
    }

    interface->setVar(varname, new MaskFilter(mask));
    cout << "Created mask in " << varname << endl;
}

void filterFunc(vector<string>* args, CommandInterface* interface) {
    if (args->size() < 4) {
        cout << "Usage: " << (*args)[0] << " filter source dest" << endl;
        cout << "\tApplies filter on source and stores the resulting image in dest" << endl;
        return;
    }
    Filter* filter = dynamic_cast<Filter*>(interface->getVar((*args)[1]));
    if (filter == NULL) {
        cout << (*args)[1] << " is not a filter" << endl;
        return;
    }
    Image* source = dynamic_cast<Image*>(interface->getVar((*args)[2]));
    if (source == NULL) {
        cout << (*args)[2] << " is not an image" << endl;
        return;
    }
    Image* dest = dynamic_cast<Image*>(interface->getVar((*args)[3]));
    if (dest == NULL) {
        dest = new Image(*source);
        interface->setVar((*args)[3], dest);
    }
    filter->process(source, dest);
    cout << "Applying " << (*args)[1] << " filter" << endl;
}

void invertFunc(vector<string>* args, CommandInterface* interface) {
    if (args->size() < 3) {
        cout << "Usage: " << (*args)[0] << " varname type" << endl;
        cout << "\tDefines an inversion filter" << endl;
        cout << "\tValid types:" << endl;
        cout << "\t\tGray" << endl;
        cout << "\t\tRed" << endl;
        cout << "\t\tGreen" << endl;
        cout << "\t\tBlue" << endl;
        cout << "\t\tAlpha" << endl << endl;
        cout << "\tNote: Gray will convert resulting image to grayscale" << endl;
        return;
    }
    string varname = (*args)[1];
    string type = (*args)[2];
    if (interface->hasVar(type)) {
        StringVar* typestr = (dynamic_cast<StringVar*>(interface->getVar(type)));
        if (typestr == NULL) {
            cout << type << " is not a string" << endl;
            return;
        } else {
            type = typestr->str;
        }
    }

    std::transform(type.begin(), type.end(), type.begin(), ::tolower);
    int channel = GRAY;
    if (type.compare("gray") == 0) {
        channel = GRAY;
    } else if (type.compare("red") == 0) {
        channel = RED;
    } else if (type.compare("green") == 0) {
        channel = GREEN;
    } else if (type.compare("blue") == 0) {
        channel = BLUE;
    } else if (type.compare("alpha") == 0) {
        channel = ALPHA;
    } else {
        cout << type << " is not a valid type" << endl;
        return;
    }

    interface->setVar(varname, new InvertChannel(channel));
    cout << "Created " << type << " inverter in " << varname << endl;
}

void histoFunc(vector<string>* args, CommandInterface* interface) {
    if (args->size() < 3) {
        cout << "Usage: " << (*args)[0] << " varname type" << endl;
        cout << "\tDefines a histograme equalization filter" << endl;
        cout << "\tValid types:" << endl;
        cout << "\t\tGray" << endl;
        cout << "\t\tRed" << endl;
        cout << "\t\tGreen" << endl;
        cout << "\t\tBlue" << endl;
        cout << "\t\tAlpha" << endl << endl;
        cout << "\tNote: Gray will convert resulting image to grayscale" << endl;
        return;
    }
    string varname = (*args)[1];
    string type = (*args)[2];
    if (interface->hasVar(type)) {
        StringVar* typestr = (dynamic_cast<StringVar*>(interface->getVar(type)));
        if (typestr == NULL) {
            cout << type << " is not a string" << endl;
            return;
        } else {
            type = typestr->str;
        }
    }

    std::transform(type.begin(), type.end(), type.begin(), ::tolower);
    int channel = GRAY;
    if (type.compare("gray") == 0) {
        channel = GRAY;
    } else if (type.compare("red") == 0) {
        channel = RED;
    } else if (type.compare("green") == 0) {
        channel = GREEN;
    } else if (type.compare("blue") == 0) {
        channel = BLUE;
    } else if (type.compare("alpha") == 0) {
        channel = ALPHA;
    } else {
        cout << type << " is not a valid type" << endl;
        return;
    }

    interface->setVar(varname, new HistogramEqualizer(channel));
    cout << "Created " << type << " histogram equalizer in " << varname << endl;
}


void copyFunc(vector<string>* args, CommandInterface* interface) {
    if (args->size() < 3) {
        cout << "Usage: " << (*args)[0] << " source dest" << endl;
        cout << "\tMakes copy of source in dest" << endl;
        return;
    }
    Image* source = dynamic_cast<Image*>(interface->getVar((*args)[1]));
    if (source == NULL) {
        cout << (*args)[1] << " is not an image" << endl;
        return;
    }
    Image* dest = new Image(*source);
    interface->setVar((*args)[2], dest);
}


/*

   unsigned int selected = -1;

   void listDevices(vector<string>* args) {
   print("Devices: \n");

   for (unsigned int i = 1; i < maxDeviceId; i++) {
   if (Device::getDevice(i) != NULL) {
   if (i == selected) {
   print("-->\t%d\t%s\t\t%s\n", i, Device::getDevice(i)->getCName().c_str(), Device::getDevice(i)->getName().c_str());
   } else {
   print("\t%d\t%s\t\t%s\n", i, Device::getDevice(i)->getCName().c_str(), Device::getDevice(i)->getName().c_str());
   }
   }
   }
   }

   void quitFunc(vector<string>* args) {
   exit(0);
   }

   char buf[10];

   void selFunc(vector<string>* args) {
   if (args->size() > 1) {
   selected = atoi((*args)[1].c_str());
   }
   sprintf(buf, "D(%d)->", selected);
   CommandInterface::setPrompt(buf);
   }

   void rcommFunc(vector<string>* args) {
   Device* device = Device::getDevice(selected);
   if (device != NULL) {
   Communicator* comm = device->getComm();
   StartPacket start;
   comm->sendPacket(&start);
   } else {
   print("Error: No Device Selected\n");
   }
   }

   void listFields(vector<string>* args) {
   Device* device = Device::getDevice(selected);
   if (device != NULL) {
   print("Fields:\n");
   for (unsigned int i = 0; i < device->getMaxField(); i++) {
   if (device->hasField(i)) {
   Field* field = device->getField(i);
   print("\t%d\t%s\t%s\n", i, field->getName().c_str(), typeStrings[(int)field->getType()].c_str());
   }
   }
   } else {
   print("Error: No Device Selected\n");
   }
   }

   void setField(vector<string>* args) {
   Device* device = Device::getDevice(selected);
   if (device != NULL) {
   if (args->size() > 2) {
   Field* field = device->getField(atoi((*args)[1].c_str()));
   if (field != NULL) {
   if (field->isWritable()) {
   field->setRealString((*args)[2]);
   } else {
   print("Error: Field is Read-only\n");
   }
   } else {
   print("Error: Invalid Field %d\n", atoi((*args)[1].c_str()));
}
} else {
    print("Usage: set <field id> <value>\n");
}
} else {
    print("Error: No Device Selected\n");
}
}

void printDevice(vector<string>* args) {
    Device* device = Device::getDevice(selected);
    if (device == NULL) {
        if (args->size() > 1) {
            device = Device::getDevice(atoi((*args)[1].c_str()));
        }
        if (device == NULL) {
            print("Error: No Device Selected\n");
            return;
        }
    }
    print("Device ID: %d\n", device->getId());
    print("Device: %s\t\tCommon Name: %s\n", device->getName().c_str(), device->getCName().c_str());
    print("Room: %d\t X: %lf\tY: %lf\n", device->getRoom()->getId(), device->getX(), device->getY());
}

void printField(vector<string>* args) {
    Device* device = Device::getDevice(selected);
    if (device != NULL) {
        if (args->size() > 1) {
            Field* field = device->getField(atoi((*args)[1].c_str()));

            if (field != NULL) {
                print("Field: %d - %s:\t%s\n", field->getId(), typeStrings[(int)field->getType()].c_str(), field->getName().c_str());
                print("%s\t", field->isWritable()?"Writable":"Read-Only");
                if (field->isVolatile()) {
                    print("Volatile\t");
                }
                print("\n");
                switch (field->getType()) {
                    case BOOL:
                        print("Value: %s\n", ((BooleanField*)field)->getBool()?"True":"False");
                        break;
                    case INTEGER:
                        print("Value: %d\n", ((IntegerField*)field)->getInt());
                        break;
                    case FLOAT:
                        print("Value: %lf\n", ((FloatField*)field)->getFloat());
                        break;
                    case FIXED:
                        print("Value: %lf\n", ((FixedField*)field)->getFloat());
                        print("One Reference: %d\n", ((FixedField*)field)->getOne());
                        break;
                    case STRING:
                        print("Value: %s\n", ((StringField*)field)->getValue().c_str());
                        break;
                }
            } else {
                print("Error: Invalid Field %d\n", atoi((*args)[1].c_str()));
            }
        } else {
            print("Usage: print <field id>\n");
        }
    } else {
        print("Error: No Device Selected\n");
    }
}

void listComms(vector<string>* args) {
    print("Active Comms:\n");
    for (unsigned int i = 0; i < comms.size(); i++) {
        print("%d\t-\t%s\n", i + 1, comms[i]->toString().c_str());
    }
}

void delComm(vector<string>* args) {
    if (args->size() > 1) {
        int id = atoi((*args)[1].c_str());
        printf("Removing %d\n", id);
        printf("Location %p\n", comms[id - 1]);
        comms[id - 1]->remove();
    }
}

void newComm(vector<string>* args) {
    if (args->size() > 1) {
        new SerialComm((char *)(*args)[1].c_str());
    }
}
*/

} /* namespace dvs */
