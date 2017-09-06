//---------------------------------------------------------------------------


#pragma hdrstop

#include "curves_class.h"
#include<stdlib.h>
#include<string>
#include <iostream>

#include <XMLDoc.hpp>
#include <XMLIntf.hpp>
#include <ComObj.hpp>

#include <xmldoc.hpp>
#include <msxmldom.hpp>

using namespace std;

//---------------------------------------------------------------------------

#pragma package(smart_init)

#define MAX_AMOUNT_CURVES 128
#define MAX_AMOUNT_POINTS 256

bool extractSerializedData(AnsiString * serializedImage, int ** outputVector, unsigned int length);

typedef struct
{
    float x;
    float y;
}TCurvePoint;

// ------------------------------- TCurve -------------------------------

class TCurve
{
    public:
        TCurve();
        TCurve(const TCurve& copy_from);
        TCurve& operator=(const TCurve& copy_from);
        ~TCurve();

        TCurvePoint ** getPoints();
        bool addPoint(float x, float y);
        bool removePoint(int pointIndex);
        int pointsCount();
        char * getName();
        bool setName(char * newName);

        int enabled;

    private:
        TCurvePoint ** points;
        int amountOfPoints;
        char * name;
};

TCurve::~TCurve()
{
    // TODO: complete the destructor
    
}

char * TCurve::getName()
{
    return name;
}

bool TCurve::setName(char * newName)
{
    name = (char *)malloc(sizeof(char) * (strlen(newName) +1));

    if (name == NULL)
        return false;

    strcpy(name, newName);
    return true;
}

TCurve::TCurve()
{
    amountOfPoints = 0;
    points = (TCurvePoint **)malloc(sizeof(TCurvePoint *) * MAX_AMOUNT_POINTS);
    setName("No name");
    enabled = true;
}

TCurvePoint ** TCurve::getPoints()
{
    return points;
}

bool TCurve::addPoint(float x, float y)
{
    if(amountOfPoints +1 >= MAX_AMOUNT_POINTS)
        return false;

    points[amountOfPoints] = (TCurvePoint *)malloc(sizeof(TCurvePoint));
    points[amountOfPoints]->x = x;
    points[amountOfPoints]->y = y;

    if(points[amountOfPoints] == NULL)
        return false;

    amountOfPoints ++;
    return true;
}

bool TCurve::removePoint(int pointIndex)
{
    // Check that the index is within the valid range
    if(pointIndex > amountOfPoints-1 || pointIndex < 0)
        return false;

    for(int i = pointIndex; i < amountOfPoints - 1; i++)
    {
        points[i] = points[i+1];
    }
    amountOfPoints --;
    return true;
}

int TCurve::pointsCount()
{
    return amountOfPoints;
}

class TAxis
{
    public:
        TAxis()
        {
            startPoint.x = 0;
            startPoint.y = 0;
            endPoint.x = 0;
            endPoint.y = 0;
            isLogarithmic = false;
            min = 0;
            max = 0;
        }
        TAxis(const TAxis& copy_from);
        TAxis& operator=(const TAxis& copy_from);
        ~TAxis()
        {}

        TCurvePoint startPoint;
        TCurvePoint endPoint;
        float min, max;
        bool isLogarithmic;
        float logarithmicBase;
};

// ------------------------------- TCurves -------------------------------

class TCurves
{
    public:
        TCurves();
        TCurves(const TCurves& copy_from);
        TCurves& operator=(const TCurves& copy_from);
        ~TCurves();

        TCurve * getCurve(int curveIndex);
        TCurve ** getCurves();
        int curvesCount();
        bool addNewCurve();
        bool removeCurve(int curveIndex);
        TAxis ** axes;

        // File save / recall part
        bool saveToXMLFile(string filename);
        bool loadFromXMLFile(string filename);

        // Image part
        string imageFilename;
        int imageWidth;
        int imageHeight;
        int * serializedImage;
        int serializedImageSize;
        int * trimValues;


    private:
        TCurve ** curves;
        int amountOfCurves;
};

TCurves::~TCurves()
{
    // Free all curves
    for(int i=0; i<amountOfCurves; i++)
    {
        for(int j=0; j < curves[i]->pointsCount(); j++)
        {
            free(curves[i]->getPoints()[j]);
        }
        free(curves[i]);
    }

    // Free the curves pointers
    free(curves);

}

TCurves::TCurves()
{
    // Allocate memory for the array of pointers
    curves = (TCurve **)malloc(sizeof(TCurve *) * MAX_AMOUNT_CURVES);

    // Initialize the counter of curves
    amountOfCurves = 0;

    axes = (TAxis **)malloc(sizeof(TAxis *) * 2);
    axes[0] =  new TAxis();
    axes[1] =  new TAxis();

    // Serialized image to be saved
    imageWidth = -1;
    imageHeight = -1;
    serializedImage = NULL;
    serializedImageSize = -1;
    trimValues = (int *)malloc(sizeof(int) * 4);
    trimValues[0] = 0;
    trimValues[1] = 0;
    trimValues[2] = 100;
    trimValues[3] = 100;
}

int TCurves::curvesCount()
{
    return amountOfCurves;
}

TCurve * TCurves::getCurve(int curveIndex)
{
    if (curveIndex >= amountOfCurves)
        return NULL;
    else
        return curves[curveIndex];
}

TCurve ** TCurves::getCurves()
{
    return curves;
}

bool TCurves::addNewCurve()
{
    if(amountOfCurves == MAX_AMOUNT_CURVES-1)
        return false;

    curves[amountOfCurves] = new TCurve();
    if(curves[amountOfCurves] == NULL)
        return false;

    amountOfCurves ++;
    return true;
}

bool TCurves::removeCurve(int curveIndex)
{
    // Check that such curve exists
    if(curveIndex > amountOfCurves-1)
        return false;

    // Free the curve memory
    delete curves[curveIndex];

    // Move all pointers back to rearrange the array
    amountOfCurves --;
    for(int i=curveIndex; i <amountOfCurves; i++)
    {
        curves[i] = curves[i+1];
    }

    return true;
}

bool TCurves::saveToXMLFile(string filename)
{
    try
    {
        TXMLDocument * _document = new TXMLDocument("");
	    _document->Active = true;
        _document->Version = "1.0";
        _document->Encoding = "utf-8";

        AnsiString * _formatString = new AnsiString();
        _di_IXMLNode _point, _child;
    	_document->DocumentElement = _document->CreateNode("captured_data", ntElement, "");

        // Header with general info
        _di_IXMLNode _header = _document->DocumentElement->AddChild("global", -1);
        _header->AddChild("image_filename", -1)->SetText(imageFilename.c_str());
        _header->AddChild("image_width", -1)->SetText((AnsiString)imageWidth);
        _header->AddChild("image_height", -1)->SetText((AnsiString)imageHeight);
        _header->AddChild("image_bin_size", -1)->SetText((AnsiString)serializedImageSize);
        if(serializedImageSize > 0)
        {
            AnsiString * _serializedImage = new AnsiString();
            _serializedImage->printf("%d", serializedImage[0]);
            for (int i=1; i<serializedImageSize; i++)
            {
                _serializedImage->cat_printf(";%d", serializedImage[i]);
            }
            _header->AddChild("image_bin", -1)->SetText(_serializedImage->c_str());
        }
        _formatString->printf("%d;%d;%d;%d", trimValues[0], trimValues[1], trimValues[2], trimValues[3]);
        _header->AddChild("image_trim_values", -1)->SetText(_formatString->c_str());

        _point = _header->AddChild("axis_x", -1);
            _child = _point->AddChild("start_point", -1);
            _formatString->sprintf("%.8f", axes[0]->startPoint.x);
            _child->AddChild("x", -1)->SetText(_formatString->c_str());
            _formatString->sprintf("%.8f", axes[0]->startPoint.y);
            _child->AddChild("y", -1)->SetText(_formatString->c_str());
            _child = _point->AddChild("end_point", -1);
            _formatString->sprintf("%.8f", axes[0]->endPoint.x);
            _child->AddChild("x", -1)->SetText(_formatString->c_str());
            _formatString->sprintf("%.8f", axes[0]->endPoint.y);
            _child->AddChild("y", -1)->SetText(_formatString->c_str());
            _formatString->sprintf("%d", (int)(axes[0]->isLogarithmic));
            _point->AddChild("logarithmic", -1)->SetText(_formatString->c_str());
            _formatString->sprintf("%.8f", axes[0]->logarithmicBase);
            _point->AddChild("logarithmic_base", -1)->SetText(_formatString->c_str());
            _formatString->sprintf("%.8f", axes[0]->min);
            _point->AddChild("min", -1)->SetText(_formatString->c_str());
            _formatString->sprintf("%.8f", axes[0]->max);
            _point->AddChild("max", -1)->SetText(_formatString->c_str());
        _point = _header->AddChild("axis_y", -1);
            _child = _point->AddChild("start_point", -1);
            _formatString->sprintf("%.8f", axes[1]->startPoint.x);
            _child->AddChild("x", -1)->SetText(_formatString->c_str());
            _formatString->sprintf("%.8f", axes[1]->startPoint.y);
            _child->AddChild("y", -1)->SetText(_formatString->c_str());
            _child = _point->AddChild("end_point", -1);
            _formatString->sprintf("%.8f", axes[1]->endPoint.x);
            _child->AddChild("x", -1)->SetText(_formatString->c_str());
            _formatString->sprintf("%.8f", axes[1]->endPoint.y);
            _child->AddChild("y", -1)->SetText(_formatString->c_str());
            _formatString->sprintf("%d", (int)(axes[1]->isLogarithmic));
            _point->AddChild("logarithmic", -1)->SetText(_formatString->c_str());
            _formatString->sprintf("%.8f", axes[1]->logarithmicBase);
            _point->AddChild("logarithmic_base", -1)->SetText(_formatString->c_str());
            _formatString->sprintf("%.8f", axes[1]->min);
            _point->AddChild("min", -1)->SetText(_formatString->c_str());
            _formatString->sprintf("%.8f", axes[1]->max);
            _point->AddChild("max", -1)->SetText(_formatString->c_str());

        _formatString->printf("%d", amountOfCurves);
        _header->AddChild("curves_count", -1)->SetText(_formatString->c_str());

        // All curves data
        _di_IXMLNode _curvesData = _document->DocumentElement->AddChild("curves", -1);
        for(int i=0; i<amountOfCurves; i++)
        {
            _di_IXMLNode _curveNode = _curvesData->AddChild("curve", -1);
            _curveNode->AddChild("name", -1)->SetText(curves[i]->getName());
            _curveNode->AddChild("points_count", -1)->SetText((AnsiString)(curves[i]->pointsCount()));
            _di_IXMLNode _curvePoints = _curveNode->AddChild("points", -1);

            for(int j=0; j < curves[i]->pointsCount(); j++)
            {
                _point = _curvePoints->AddChild("point", -1);
                _point->AddChild("x",-1);
                _formatString->sprintf("%.8f", curves[i]->getPoints()[j]->x);
                (_point->ChildNodes->Get(0))->SetText(_formatString->c_str());
                _point->AddChild("y",-1);
                _formatString->sprintf("%.8f", curves[i]->getPoints()[j]->y);
                (_point->ChildNodes->Get(1))->SetText(_formatString->c_str());
            }
        }

        /*
    	document->DocumentElement->Attributes["attrName"] = "attrValue";
    	_di_IXMLNode nodeElement = document->DocumentElement->AddChild
    		("ThisElementHasText", -1);
    	nodeElement->Text = "Inner text.";
    	_di_IXMLNode nodeCData = document->CreateNode("any characters here",
    		ntCData, "");
    	document->DocumentElement->ChildNodes->Add(nodeCData);
    	_di_IXMLNode nodeText = document->CreateNode("This is a text node.",
    		ntText, "");
        */
    	//_document->DocumentElement->ChildNodes->Add(nodeText);

    	_document->SaveToFile(filename.c_str());
    }
    catch (const Exception& e)
    {
        return false;
    }
    return true;
}


double toDouble(std::string s){
    std::replace(s.begin(), s.end(), ',', '.');
    return std::atof(s.c_str());
}

bool TCurves::loadFromXMLFile(string filename)
{
    // Load XML file from
    try
    {
        _di_IXMLDocument _document = LoadXMLDocument(filename.c_str());
        if(_document->IsEmptyDoc())
        {
            return False;
        }

        _di_IXMLNode _rootElement = _document->ChildNodes->FindNode("captured_data");
        if(_rootElement == NULL)
            return False;

        // Load header
        _di_IXMLNode _global = _rootElement->ChildNodes->FindNode("global");
        imageFilename = ((AnsiString)_global->ChildNodes->FindNode("image_filename")->Text).c_str();
        imageWidth = toDouble(((AnsiString)(_global->ChildNodes->FindNode("image_width")->Text)).c_str());
        imageHeight = toDouble(((AnsiString)(_global->ChildNodes->FindNode("image_height")->Text)).c_str());
        serializedImageSize = toDouble(((AnsiString)(_global->ChildNodes->FindNode("image_bin_size")->Text)).c_str());

        // Try to read and parse the binary image
        if (!extractSerializedData(&((AnsiString)(_global->ChildNodes->FindNode("image_bin")->Text)),
                                    &serializedImage, serializedImageSize))
        {
            return False;
        }

        // Try to read and parse the image trim values
        if (!extractSerializedData(&((AnsiString)(_global->ChildNodes->FindNode("image_trim_values")->Text)),
                                    &trimValues, 4))
        {
            return False;
        }

        // Axes information
        axes[0]->startPoint.x = toDouble(((AnsiString)(_global->ChildNodes->FindNode("axis_x")
                                    ->ChildNodes->FindNode("start_point")
                                    ->ChildNodes->FindNode("x")->Text)).c_str());
        axes[0]->startPoint.y = toDouble(((AnsiString)(_global->ChildNodes->FindNode("axis_x")
                                    ->ChildNodes->FindNode("start_point")
                                    ->ChildNodes->FindNode("y")->Text)).c_str());
        axes[0]->endPoint.x = toDouble(((AnsiString)(_global->ChildNodes->FindNode("axis_x")
                                    ->ChildNodes->FindNode("end_point")
                                    ->ChildNodes->FindNode("x")->Text)).c_str());
        axes[0]->endPoint.y = toDouble(((AnsiString)(_global->ChildNodes->FindNode("axis_x")
                                    ->ChildNodes->FindNode("end_point")
                                    ->ChildNodes->FindNode("y")->Text)).c_str());
        axes[1]->startPoint.x = toDouble(((AnsiString)(_global->ChildNodes->FindNode("axis_y")
                                    ->ChildNodes->FindNode("start_point")
                                    ->ChildNodes->FindNode("x")->Text)).c_str());
        axes[1]->startPoint.y = toDouble(((AnsiString)(_global->ChildNodes->FindNode("axis_y")
                                    ->ChildNodes->FindNode("start_point")
                                    ->ChildNodes->FindNode("y")->Text)).c_str());
        axes[1]->endPoint.x = toDouble(((AnsiString)(_global->ChildNodes->FindNode("axis_y")
                                    ->ChildNodes->FindNode("end_point")
                                    ->ChildNodes->FindNode("x")->Text)).c_str());
        axes[1]->endPoint.y = toDouble(((AnsiString)(_global->ChildNodes->FindNode("axis_y")
                                    ->ChildNodes->FindNode("end_point")
                                    ->ChildNodes->FindNode("y")->Text)).c_str());
        axes[0]->min = toDouble(((AnsiString)(_global->ChildNodes->FindNode("axis_x")
                                ->ChildNodes->FindNode("min")->Text)).c_str());
        axes[0]->max = toDouble(((AnsiString)(_global->ChildNodes->FindNode("axis_x")
                                ->ChildNodes->FindNode("max")->Text)).c_str());
        axes[1]->min = toDouble(((AnsiString)(_global->ChildNodes->FindNode("axis_y")
                                ->ChildNodes->FindNode("min")->Text)).c_str());
        axes[1]->max = toDouble(((AnsiString)(_global->ChildNodes->FindNode("axis_y")
                                ->ChildNodes->FindNode("max")->Text)).c_str());
        axes[0]->logarithmicBase = toDouble(((AnsiString)(_global->ChildNodes->FindNode("axis_x")
                                            ->ChildNodes->FindNode("logarithmic_base")->Text)).c_str());
        axes[1]->logarithmicBase = toDouble(((AnsiString)(_global->ChildNodes->FindNode("axis_y")
                                            ->ChildNodes->FindNode("logarithmic_base")->Text)).c_str());
        axes[0]->isLogarithmic = (bool)toDouble(((AnsiString)(_global->ChildNodes->FindNode("axis_x")
                                          ->ChildNodes->FindNode("logarithmic")->Text)).c_str());
        axes[1]->isLogarithmic = (bool)toDouble(((AnsiString)(_global->ChildNodes->FindNode("axis_y")
                                          ->ChildNodes->FindNode("logarithmic")->Text)).c_str());

        // Curves
        int _expectedAmountOfCurves = (int) toDouble(((AnsiString)(_global->ChildNodes->FindNode("curves_count")
                                          ->Text)).c_str());
        if (_expectedAmountOfCurves == 0)
        {
            return True;
        }
        _di_IXMLNode _curves = _rootElement->ChildNodes->FindNode("curves");
        for(int i=0; i < _curves->ChildNodes->Count; i++)
        {
            addNewCurve();
            getCurve(i)->setName(((AnsiString)_curves->ChildNodes->Get(i)->ChildNodes->FindNode("name")->Text).c_str());
            int _expectedAmountOfPoints = atoi(((AnsiString)(_curves->ChildNodes->Get(i)->ChildNodes->FindNode("points_count")->Text)).c_str());
            if (_expectedAmountOfPoints > 0)
            {
                _di_IXMLNode _points = _curves->ChildNodes->Get(i)->ChildNodes->FindNode("points");
                for(int j=0; j < _points->ChildNodes->Count; j++)
                {
                    float _x = toDouble(((AnsiString)(_points->ChildNodes->Get(j)->ChildNodes->FindNode("x")->Text)).c_str());
                    float _y = toDouble(((AnsiString)(_points->ChildNodes->Get(j)->ChildNodes->FindNode("y")->Text)).c_str());
                    getCurve(i)->addPoint(_x, _y);
                }
            }
        }
        return true;
    }
    catch (const Exception& e)
    {
        return false;
    }
}

bool extractSerializedData(AnsiString * serializedData, int ** outputVector, unsigned int length)
{
    if(serializedData == NULL || length == 0)
        return False;


    (*outputVector) = (int *)malloc(sizeof(int) * length);
    char * _data = serializedData->c_str();
    char _newNumberString[50];
    unsigned int _counter = 0, _startPosition = 0, _endPosition = 0, i;
    while(_counter < length)
    {
        _startPosition = _endPosition;
        for(; _data[_endPosition] != ';' && _endPosition < (unsigned int)serializedData->Length(); _endPosition++);

        //copy from start to end to _newNumberString
        for(i=0; i<(_endPosition - _startPosition); i++)
        {
            _newNumberString[i] = _data[i + _startPosition];
        }
        _newNumberString[i] = '\0';
        _endPosition++;

        (*outputVector)[_counter] = atoi(_newNumberString);
        _counter ++;
    }


    return True;

    // Much slower version:
    /*
    std::string s = serializedData->c_str();
    std::string token = "";
    (*outputVector) = (int *)malloc(sizeof(int) * length);
    for(int i=0; i < length; i++)
    {
        token = s.substr(0, s.find(";"));
        s.erase(0, s.find(";") + 1);
        (*outputVector)[i] = atoi(token.c_str());
    }
    return True;
    */
}
