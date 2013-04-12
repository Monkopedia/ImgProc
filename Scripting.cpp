#include "Scripting.h"
#include "StringVar.h"
#include "Filter.h"
#include "MaskFilter.h"
#include "Invert.h"
#include "InvertChannel.h"
#include "MedianFilter.h"
#include "SobelFilter.h"
#include "LaplacianFilter.h"
#include "ErodeFilter.h"
#include "DilationFilter.h"
#include "ThresholdFilter.h"
#include "HistogramEqualize.h"
#include "ColorImage.h"
#include "Area.h"
#include "Eigens.h"
#include "Perimeter.h"
#include "MomentInvariant.h"

#include "Server.h"

#include "Image.h"

#include <algorithm>

#include "Rotation.h"
#include "RotatedImage.h"
#include "Transform.h"
#include "TransformImage.h"

using namespace std;
namespace ImgProc {

pair<string*, funcListener> *(functions[NFUNCS]) = {
	new pair<string*, funcListener>(new string("echo"), echoFunc),
	new pair<string*, funcListener>(new string("print"), printFunc),
	new pair<string*, funcListener>(new string("quit"), quitFunc),
	new pair<string*, funcListener>(new string("exit"), quitFunc),
	new pair<string*, funcListener>(new string("load"), loadFunc),
	new pair<string*, funcListener>(new string("save"), saveFunc),
	new pair<string*, funcListener>(new string("mask"), neighborFunc),
	new pair<string*, funcListener>(new string("invert"), invertFunc),
	new pair<string*, funcListener>(new string("median"), medianFunc),
	new pair<string*, funcListener>(new string("sobel"), sobelFunc),
	new pair<string*, funcListener>(new string("laplace"), laplaceFunc),
	new pair<string*, funcListener>(new string("erode"), erodeFunc),
	new pair<string*, funcListener>(new string("dilate"), dilateFunc),
	new pair<string*, funcListener>(new string("threshold"), thresholdFunc),
	new pair<string*, funcListener>(new string("filter"), filterFunc),
	new pair<string*, funcListener>(new string("copy"), copyFunc),
	new pair<string*, funcListener>(new string("colorcopy"), colorCopyFunc),
	new pair<string*, funcListener>(new string("histoeq"), histoFunc),
	new pair<string*, funcListener>(new string("rotate"), rotateFunc),
	new pair<string*, funcListener>(new string("deftrans"), defTransFunc),
	new pair<string*, funcListener>(new string("combinetrans"), combineTransFunc),
	new pair<string*, funcListener>(new string("transform"), transformFunc),
	new pair<string*, funcListener>(new string("area"), areaFunc),
	new pair<string*, funcListener>(new string("eigen"), eigenFunc),
	new pair<string*, funcListener>(new string("moment"), miFunc),
	new pair<string*, funcListener>(new string("perimeter"), perimeterFunc),
	new pair<string*, funcListener>(new string("descriptor"), descriptorFunc),
};

string getString(string str, CommandInterface* interface) {
    if (interface->getVar(str)) {
        StringVar* filter = dynamic_cast<StringVar*>(interface->getVar(str));
        if (filter == NULL) {
            cout << str << " is not a string" << endl;
        } else {
            str = filter->str;
        }
    }
    return str;
}

void echoFunc(vector<string>* args, CommandInterface* interface) {
    for (unsigned int i = 1; i < args->size(); i++) {
        string var = getString((*args)[i], interface);
        cout << var << endl;
    }
}

void printFunc(vector<string>* args, CommandInterface* interface) {
    for (unsigned int i = 1; i < args->size(); i++) {
        Variable* var = interface->getVar((*args)[i]);
        if (var != NULL) {
            cout << var->toString() << endl;
        } else {
            cout << (*args)[i] << " is not assigned" << endl;
        }
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
    string filename = getString((*args)[1], interface);
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
    string filename = getString((*args)[1], interface);
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
    string type = getString((*args)[2], interface);

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
    string type = getString((*args)[2], interface);

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
        cout << "\tRotates source by amount and stores the resulting image in dest" << endl;
        return;
    }
    string amount = getString((*args)[1], interface);
    Image* source = dynamic_cast<Image*>(interface->getVar((*args)[2]));
    if (source == NULL) {
        cout << (*args)[2] << " is not an image" << endl;
        return;
    }
    Rotation rot(atof(amount.c_str()));
    interface->setVar((*args)[3], new RotatedImage(rot, *source));
    cout << "Applying " << amount << " rotation" << endl;
}

void defTransFunc(vector<string>* args, CommandInterface* interface) {
    if (args->size() < 5) {
        cout << "Usage: " << (*args)[0] << " varname amount x y" << endl;
        cout << "\tCreates a transform" << endl;
        return;
    }
    string var = (*args)[1];
    string amount = getString((*args)[2], interface);
    string x = getString((*args)[3], interface);
    string y = getString((*args)[4], interface);

    Transform* trans = new Transform(atof(amount.c_str()), atof(x.c_str()), atof(y.c_str()));
    interface->setVar(var, trans);
    cout << "Creating transform rot: " << amount << " off: " << x << " " << y << endl;
}

void transformFunc(vector<string>* args, CommandInterface* interface) {
    if (args->size() < 4) {
        cout << "Usage: " << (*args)[0] << " varname source dest" << endl;
        cout << "\tTransform source using varname and stores the resulting image in dest" << endl;
        return;
    }
    Image* source = dynamic_cast<Image*>(interface->getVar((*args)[2]));
    if (source == NULL) {
        cout << (*args)[2] << " is not an image" << endl;
        return;
    }
    Transform* transform = dynamic_cast<Transform*>(interface->getVar((*args)[1]));
    if (transform == NULL) {
        cout << (*args)[1] << " is not a transform" << endl;
        return;
    }
    interface->setVar((*args)[3], new TransformImage(*transform, *source));
    cout << "Applying transform" << endl;
}

void combineTransFunc(vector<string>* args, CommandInterface* interface) {
    if (args->size() < 3) {
        cout << "Usage: " << (*args)[0] << " varname first second" << endl;
        cout << "\tSets varname to be first * second" << endl;
        return;
    }
    string var = (*args)[1];
    Transform* transform = dynamic_cast<Transform*>(interface->getVar((*args)[2]));
    if (transform == NULL) {
        cout << (*args)[2] << " is not a transform" << endl;
        return;
    }
    Transform* transform2 = dynamic_cast<Transform*>(interface->getVar((*args)[3]));
    if (transform == NULL) {
        cout << (*args)[2] << " is not a transform" << endl;
        return;
    }
    interface->setVar(var, new Transform((*transform) * (*transform2)));
}

void medianFunc(vector<string>* args, CommandInterface* interface) {
    if (args->size() < 3) {
        cout << "Usage: " << (*args)[0] << " varname type" << endl;
        cout << "\tDefines a median filter" << endl;
        printChannelTypes(cout);
        cout << "\tNote: Gray will convert resulting image to grayscale" << endl;
        return;
    }
    string varname = (*args)[1];
    string type = getString((*args)[2], interface);

    int channel = stringToChannel(type);
    if (channel == -1) {
        cout << type << " is not a valid type" << endl;
        return;
    }

    interface->setVar(varname, new MedianFilter(channel));
    cout << "Created " << type << " median filter in " << varname << endl;
}

void sobelFunc(vector<string>* args, CommandInterface* interface) {
    if (args->size() < 3) {
        cout << "Usage: " << (*args)[0] << " varname type" << endl;
        cout << "\tDefines a sobel filter" << endl;
        printChannelTypes(cout);
        cout << "\tNote: Gray will convert resulting image to grayscale" << endl;
        return;
    }
    string varname = (*args)[1];
    string type = getString((*args)[2], interface);

    int channel = stringToChannel(type);
    if (channel == -1) {
        cout << type << " is not a valid type" << endl;
        return;
    }

    interface->setVar(varname, new SobelFilter(channel));
    cout << "Created " << type << " sobel filter in " << varname << endl;
}

void laplaceFunc(vector<string>* args, CommandInterface* interface) {
    if (args->size() < 3) {
        cout << "Usage: " << (*args)[0] << " varname type" << endl;
        cout << "\tDefines a laplace filter" << endl;
        printChannelTypes(cout);
        cout << "\tNote: Gray will convert resulting image to grayscale" << endl;
        return;
    }
    string varname = (*args)[1];
    string type = getString((*args)[2], interface);

    int channel = stringToChannel(type);
    if (channel == -1) {
        cout << type << " is not a valid type" << endl;
        return;
    }

    interface->setVar(varname, new LaplacianFilter(channel));
    cout << "Created " << type << " laplacian filter in " << varname << endl;
}

void erodeFunc(vector<string>* args, CommandInterface* interface) {
    if (args->size() < 3) {
        cout << "Usage: " << (*args)[0] << " varname type" << endl;
        cout << "\tDefines a erode filter" << endl;
        printChannelTypes(cout);
        cout << "\tNote: Gray will convert resulting image to grayscale" << endl;
        return;
    }
    string varname = (*args)[1];
    string type = getString((*args)[2], interface);

    int channel = stringToChannel(type);
    if (channel == -1) {
        cout << type << " is not a valid type" << endl;
        return;
    }

    interface->setVar(varname, new ErodeFilter(channel));
    cout << "Created " << type << " erode filter in " << varname << endl;
}

void dilateFunc(vector<string>* args, CommandInterface* interface) {
    if (args->size() < 3) {
        cout << "Usage: " << (*args)[0] << " varname type" << endl;
        cout << "\tDefines a dilate filter" << endl;
        printChannelTypes(cout);
        cout << "\tNote: Gray will convert resulting image to grayscale" << endl;
        return;
    }
    string varname = (*args)[1];
    string type = getString((*args)[2], interface);

    int channel = stringToChannel(type);
    if (channel == -1) {
        cout << type << " is not a valid type" << endl;
        return;
    }

    interface->setVar(varname, new DilationFilter(channel));
    cout << "Created " << type << " dilate filter in " << varname << endl;
}

void thresholdFunc(vector<string>* args, CommandInterface* interface) {
    if (args->size() < 4) {
        cout << "Usage: " << (*args)[0] << " varname type threshold" << endl;
        cout << "\tDefines a threshold filter" << endl;
        printChannelTypes(cout);
        cout << "\tNote: Gray will convert resulting image to grayscale" << endl;
        return;
    }
    string varname = (*args)[1];
    string type = getString((*args)[2], interface);

    int channel = stringToChannel(type);
    if (channel == -1) {
        cout << type << " is not a valid type" << endl;
        return;
    }

    string threshold = getString((*args)[3], interface);
    float thresh = atof(threshold.c_str());

    interface->setVar(varname, new ThresholdFilter(channel, thresh));
    cout << "Created " << type << " threshold filter in " << varname << endl;
}

void areaFunc(vector<string>* args, CommandInterface* interface) {
    if (args->size() < 3) {
        cout << "Usage: " << (*args)[0] << " varname type" << endl;
        cout << "\tDefines a area calculator" << endl;
        printChannelTypes(cout);
        cout << "\tNote: Gray will convert resulting image to grayscale" << endl;
        return;
    }
    string varname = (*args)[1];
    string type = getString((*args)[2], interface);

    int channel = stringToChannel(type);
    if (channel == -1) {
        cout << type << " is not a valid type" << endl;
        return;
    }

    interface->setVar(varname, new Area(channel));
    cout << "Created " << type << " area calculator" << endl;
}

void perimeterFunc(vector<string>* args, CommandInterface* interface) {
    if (args->size() < 3) {
        cout << "Usage: " << (*args)[0] << " varname type" << endl;
        cout << "\tDefines a perimeter calculator" << endl;
        printChannelTypes(cout);
        cout << "\tNote: Gray will convert resulting image to grayscale" << endl;
        return;
    }
    string varname = (*args)[1];
    string type = getString((*args)[2], interface);

    int channel = stringToChannel(type);
    if (channel == -1) {
        cout << type << " is not a valid type" << endl;
        return;
    }

    interface->setVar(varname, new Perimeter(channel));
    cout << "Created " << type << " perimeter calculator" << endl;
}



void descriptorFunc(vector<string>* args, CommandInterface* interface) {
    if (args->size() < 4) {
        cout << "Usage: " << (*args)[0] << " descriptor source dest" << endl;
        cout << "\tCalculates descriptor from source and stores the resulting in dest" << endl;
        return;
    }
    Descriptor* filter = dynamic_cast<Descriptor*>(interface->getVar((*args)[1]));
    if (filter == NULL) {
        cout << (*args)[1] << " is not a filter" << endl;
        return;
    }
    Image* source = dynamic_cast<Image*>(interface->getVar((*args)[2]));
    if (source == NULL) {
        cout << (*args)[2] << " is not an image" << endl;
        return;
    }
    Variable* dest = dynamic_cast<Variable*>(interface->getVar((*args)[3]));
    if (dest == NULL) {
        dest = new StringVar();
        interface->setVar((*args)[3], dest);
    }
    filter->process(source, dest);
    cout << "Calculating " << (*args)[1] << " descriptor" << endl;
}

void miFunc(vector<string>* args, CommandInterface* interface) {
    if (args->size() < 3) {
        cout << "Usage: " << (*args)[0] << " varname which" << endl;
        cout << "\tDefines a moment invariant calculator" << endl;
        return;
    }
    string varname = (*args)[1];
    string type = getString((*args)[2], interface);

    int channel = atoi(type.c_str());

    interface->setVar(varname, new MomentInvariant(channel));
    cout << "Created " << channel << " moment invariant calculator" << endl;
}

void eigenFunc(vector<string>* args, CommandInterface* interface) {
    if (args->size() < 2) {
        cout << "Usage: " << (*args)[0] << " varname" << endl;
        cout << "\tDefines a eigen calculator" << endl;
        return;
    }
    string varname = (*args)[1];

    interface->setVar(varname, new Eigens());
    cout << "Created  eigen calculator" << endl;
}






}
