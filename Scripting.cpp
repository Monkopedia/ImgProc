#include "Scripting.h"
#include "StringVar.h"
#include "Filter.h"
#include "MaskFilter.h"
#include "Invert.h"
#include "InvertChannel.h"
#include "HistogramEqualize.h"
#include "ColorImage.h"

#include "Server.h"

#include "Image.h"

#include <algorithm>

using namespace std;
namespace ImgProc {

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
	new pair<string*, funcListener>(new string("colorcopy"), colorCopyFunc),
	new pair<string*, funcListener>(new string("histoeq"), histoFunc),
};

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

int stringToChannel(string type) {
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);
    int channel = -1;
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
    }
    return channel;
}

void printChannelTypes(ostream& cout) {
    cout << "\tValid types:" << endl;
    cout << "\t\tGray" << endl;
    cout << "\t\tRed" << endl;
    cout << "\t\tGreen" << endl;
    cout << "\t\tBlue" << endl;
    cout << "\t\tAlpha" << endl << endl;
}

void invertFunc(vector<string>* args, CommandInterface* interface) {
    if (args->size() < 3) {
        cout << "Usage: " << (*args)[0] << " varname type" << endl;
        cout << "\tDefines an inversion filter" << endl;
        printChannelTypes(cout);
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

    int channel = stringToChannel(type);
    if (channel == -1) {
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
        printChannelTypes(cout);
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

    int channel = stringToChannel(type);
    if (channel == -1) {
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

void colorCopyFunc(vector<string>* args, CommandInterface* interface) {
    if (args->size() < 3) {
        cout << "Usage: " << (*args)[0] << " source dest" << endl;
        cout << "\tMakes color organized copy of source in dest" << endl;
        return;
    }
    Image* source = dynamic_cast<Image*>(interface->getVar((*args)[1]));
    if (source == NULL) {
        cout << (*args)[1] << " is not an image" << endl;
        return;
    }
    Image* dest = new ColorImage(*source);
    interface->setVar((*args)[2], dest);
}

void rotateFunc(vector<string>* args, CommandInterface* interface) {
    if (args->size() < 4) {
        cout << "Usage: " << (*args)[0] << " amount source dest" << endl;
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
    /*
    Image* dest = dynamic_cast<Image*>(interface->getVar((*args)[3]));
    if (dest == NULL) {
        dest = new Image(*source);
        interface->setVar((*args)[3], dest);
    }
    */
    cout << "Applying " << (*args)[1] << " filter" << endl;
}


}
