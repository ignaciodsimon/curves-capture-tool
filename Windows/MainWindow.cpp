//---------------------------------------------------------------------------

#define SOFTWARE_NAME "Curvey"
#define SOFTWARE_VERSION "0.1a-09.2016"
#define SOFTWARE_AUTHOR "Joe Simon (c) 2016"
#define SOFTWARE_WEBPAGE "http://github.com/ignaciodsimon"

#include <vcl.h>
#pragma hdrstop

#include <Clipbrd.hpp>
#include <memory>

#include "MainWindow.h"
#include "curves_class.cpp"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TMainForm *MainForm;

TCurves * curvesData = NULL;

AnsiString * loadedFilename;
bool axesTypes[2] = {False, False};
bool imageIsLoaded = False;
bool settingAxisPoints[2] = {False, False};
bool capturingCurvePoints = False;
int curveBeingCapturedIndex = 0;
//float axisLimitsPoint[2][2][2];
TColor curveFaceColor;
TColor curveBorderColor;
//int trimValues[4] = {0, 0, 100, 100};
bool isTrimming = False;
TImage * originalImage;

/******/

void refreshAxesTypeDisplay();
void setAxisType(int axisIndex, bool isLogarithmic);
void drawLine(TImage * paintBox, float fromX,
              float fromY, float toX, float toY,
              float lineThickness, TColor lineColor);
void setImageZoom(int imageZoom);
void setZoomToMatchPanel();
void closeImage(void);
void setAxis(int axisIndex, TObject * Sender);
void refreshPaintBox();
bool tryParseFloatNumber(TEdit * source, bool allowNegative);
void clearPaintBox(TImage * paintBox);
void refreshCurvesList();
void addCurveToDisplayList(TCurve * newCurve);
void removeSelectedCurve();
int  findFirstSelectedItemInList(TListBox * list, bool deselectOthers);
void printCurve(TImage * image, TCurve * curve, TColor curveColor);
void selectLastItem(TListBox * list);
void captureSelectedCurve();
void setStatusText(AnsiString text);
void setStatusTextToFilename();
void toggleCurveEnabled();
int serializePicture(TCanvas * canvas, int width, int height, int ** serializedOutput);
void drawSquare(TImage * image, float left, float top, float right, float bottom, int lineWidth, TColor color);
void setImageTrimValues(int left, int top, int right, int bottom);
void getOnlyFilename(AnsiString * filenameWithPath, AnsiString ** onlyFilename);
void convertXYPointToValues(TCurvePoint * point, TCurvePoint * xAxisStart,
                            TCurvePoint * xAxisEnd, TCurvePoint * yAxisStart,
                            TCurvePoint * yAxisEnd, float * xMinScale,
                            float * xMaxScale, float * yMinScale, float * yMaxScale,
                            bool xIsLogarithmic, bool yIsLogarithmic, float xBase,
                            float yBase, float * xOut, float * yOut);

/******/

int serializePicture(TCanvas * canvas, int width, int height, int ** serializedOutput)
{
    int _count = 0;

    // Check the input matrix
    if(canvas == NULL)
        return -1;

    // Check the given sizes
    int _expectedLength;
    _expectedLength = width * height;
    if(!_expectedLength)
        return 0;

    // Scan and save
    (*serializedOutput) = (int *)malloc(sizeof(int) * _expectedLength);
    for(int i=0; i<width; i++)
    {
        for(int j=0; j<height; j++)
        {
            (*serializedOutput)[_count] = (int)(canvas->Pixels[i][j]);
            _count ++;
        }
    }

    return _count;
}

void refreshPaintBox()
{
    if(!imageIsLoaded)
    {
        return;
    }

    clearPaintBox(MainForm->PaintedImage);
    clearPaintBox(MainForm->TrimPaintImage);
    
    if(isTrimming)
    {
        // Show box according to int trimValues[4]
        drawSquare(MainForm->TrimPaintImage, (float)curvesData->trimValues[0]/100.0,
                                             (float)curvesData->trimValues[1]/100.0,
                                             (float)curvesData->trimValues[2]/100.0,
                                             (float)curvesData->trimValues[3]/100.0,
                                             3, (TColor) 0x000000);
        drawSquare(MainForm->TrimPaintImage, (float)curvesData->trimValues[0]/100.0,
                                             (float)curvesData->trimValues[1]/100.0,
                                             (float)curvesData->trimValues[2]/100.0,
                                             (float)curvesData->trimValues[3]/100.0,
                                             2, (TColor) 0x0000FF);

        return;
    }

    // Show axes lines if they are defined (not equal to zero) and the check box
    // is enabled
    if(MainForm->ViewAxes->Checked)
    {
        // X - axis
        if(MainForm->ShowCurveBorderCheckBox->Checked)
        {
            drawLine(MainForm->PaintedImage,
                     curvesData->axes[0]->startPoint.x,
                     curvesData->axes[0]->startPoint.y,
                     curvesData->axes[0]->endPoint.x,
                     curvesData->axes[0]->endPoint.y,
                     4, (TColor)0x000000);
        }
        drawLine(MainForm->PaintedImage,
                 curvesData->axes[0]->startPoint.x,
                 curvesData->axes[0]->startPoint.y,
                 curvesData->axes[0]->endPoint.x,
                 curvesData->axes[0]->endPoint.y,
                 2, (TColor)0x00FF00);

        // Y - axis
        if(MainForm->ShowCurveBorderCheckBox->Checked)
        {
            drawLine(MainForm->PaintedImage,
                     curvesData->axes[1]->startPoint.x,
                     curvesData->axes[1]->startPoint.y,
                     curvesData->axes[1]->endPoint.x,
                     curvesData->axes[1]->endPoint.y,
                     4, (TColor)0x000000);
        }
        drawLine(MainForm->PaintedImage,
                 curvesData->axes[1]->startPoint.x,
                 curvesData->axes[1]->startPoint.y,
                 curvesData->axes[1]->endPoint.x,
                 curvesData->axes[1]->endPoint.y,
                 2, (TColor)0x0000FF);
    }

    TColor _shadowedCurveColor = curveFaceColor - (TColor)0x505050;
    TColor _selectedCurveColor = (TColor)0xCCFFFF;
    // Draw only the curve that is being captured (to reduce image lag)
    if(capturingCurvePoints)
    {
        if(!MainForm->HideCurvesCheckBox->Checked)
        {
            // Print other curves on a different color
            for(int i=0; i < curvesData->curvesCount(); i++)
            {
                if(!(i == curveBeingCapturedIndex))
                    printCurve(MainForm->PaintedImage,
                               curvesData->getCurve(i),
                               _shadowedCurveColor);
            }
        }

        // Print curve being captured in main color
        printCurve(MainForm->PaintedImage,
                   curvesData->getCurve(curveBeingCapturedIndex),
                   _selectedCurveColor);
    }
    // Draw all curves in main color
    else
    {
        for(int i=0; i < curvesData->curvesCount(); i++)
        {
            if(MainForm->CurvesListBox->Selected[i])
            //if(i == findFirstSelectedItemInList(MainForm->CurvesListBox, False))
            {
                printCurve(MainForm->PaintedImage,
                           curvesData->getCurve(i),
                           _selectedCurveColor);
            }
            else
            {
                printCurve(MainForm->PaintedImage,
                           curvesData->getCurve(i),
                           curveFaceColor);
            }
        }
    }
}

void drawSquare(TImage * image, float left, float top, float right, float bottom, int lineWidth, TColor color)
{
        drawLine(image,
                     left,
                     top,
                     left,
                     bottom,
                     lineWidth, color);
        drawLine(image,
                     left,
                     top,
                     right,
                     top,
                     lineWidth, color);
        drawLine(image,
                     right,
                     top,
                     right,
                     bottom,
                     lineWidth, color);
        drawLine(image,
                     right,
                     bottom,
                     left,
                     bottom,
                     lineWidth, color);
}

void printCurve(TImage * image, TCurve * curve, TColor curveColor)
{
    if(image == NULL || curve == NULL)
        return;

    if(curve->pointsCount() == 0)
        return;

    // If there is only one point, show it as a short line
    if(curve->pointsCount() == 1)
    {
        if (MainForm->ShowCurveBorderCheckBox->Checked)
        {
            drawLine(image,
                     curve->getPoints()[0]->x,
                     curve->getPoints()[0]->y,
                     curve->getPoints()[0]->x + 0.001,
                     curve->getPoints()[0]->y + 0.000,
                     4, curveBorderColor);
        }
        drawLine(image,
                 curve->getPoints()[0]->x,
                 curve->getPoints()[0]->y,
                 curve->getPoints()[0]->x + 0.001,
                 curve->getPoints()[0]->y + 0.000,
                 2, curveColor);
        return;
    }

    // If there are several points, create lines normally
    for(int i=0; i < curve->pointsCount()-1; i++)
    {
        if (MainForm->ShowCurveBorderCheckBox->Checked)
        {
            drawLine(image,
                     curve->getPoints()[i]->x,
                     curve->getPoints()[i]->y,
                     curve->getPoints()[i+1]->x,
                     curve->getPoints()[i+1]->y,
                     4, curveBorderColor);
        }
        drawLine(image,
                 curve->getPoints()[i]->x,
                 curve->getPoints()[i]->y,
                 curve->getPoints()[i+1]->x,
                 curve->getPoints()[i+1]->y,
                 2, curveColor);
    }
}

void setAxisType(int axisIndex, bool isLogarithmic)
{
    axesTypes[axisIndex] = isLogarithmic;
}

void clearPaintBox(TImage * paintBox)
{
    paintBox->Picture->Bitmap = NULL;

    // The trick to make it transparent is to first fill it with an unused color
    paintBox->Canvas->Pen->Color = (TColor)0xFFFFFF;
    paintBox->Canvas->Pen->Width = 100000;
    paintBox->Canvas->Rectangle(0,0,paintBox->Width, paintBox->Height);
}

void drawLine(TImage * paintBox, float fromX,
              float fromY, float toX, float toY,
              float lineThickness, TColor lineColor)
{
//        double _color = ((double)rgb[0]) + ((double)(rgb[1]<<8)) + ((double)(rgb[2]<<16));

    /*
    MainForm->Image->Canvas->Pen->DefaultHandler(NULL);
    MainForm->Image->Canvas->Pen->Width = lineThickness;
    MainForm->Image->Canvas->Pen->Color = (TColor)_color;
    MainForm->Image->Canvas->MoveTo(fromX * ((float)paintBox->Width), fromY * ((float)paintBox->Height));
    MainForm->Image->Canvas->LineTo(toX * ((float)paintBox->Width), toY * ((float)paintBox->Height));
    */

    paintBox->Canvas->Pen->DefaultHandler(NULL);
    paintBox->Canvas->Pen->Width = lineThickness;
    paintBox->Canvas->Pen->Color = lineColor;
    paintBox->Canvas->MoveTo(fromX * ((float)paintBox->Width), fromY * ((float)paintBox->Height));
    paintBox->Canvas->LineTo(toX * ((float)paintBox->Width), toY * ((float)paintBox->Height));

    // This is to fix the transparency issue (left-bottom corner must be white)
    paintBox->Canvas->Pixels[0][paintBox->Height -1] = (TColor)0xFFFFFF;
}

void setImageZoom(int imageZoom)
{
    if (imageIsLoaded)
    {
        // Resize the image
        MainForm->Image->Width = MainForm->Image->Picture->Width * (((float)imageZoom) / 100.0);
        MainForm->Image->Height = MainForm->Image->Picture->Height * (((float)imageZoom) / 100.0);
    }
    else
    {
        // Just give it a small size
        MainForm->Image->Width = 10;
        MainForm->Image->Height = 10;
    }

    // Resize the image to show the trim lines
    MainForm->TrimPaintImage->Width = MainForm->Image->Width;
    MainForm->TrimPaintImage->Height = MainForm->Image->Height;

    // Resize the paint-box
//    MainForm->PaintedImage->Picture->Bitmap = NULL;
    MainForm->PaintedImage->Width = MainForm->Image->Width;
    MainForm->PaintedImage->Height = MainForm->Image->Height;

    // Show the zoom value
    AnsiString * newString = new AnsiString();
    newString->printf("%d %",imageZoom);
    MainForm->ZoomTextPanel->Caption = newString->c_str();

    refreshPaintBox();
}

// Gives the appropriate zoom for the image to match the panel
void setZoomToMatchPanel()
{
    if(imageIsLoaded)
    {
            // Move the slider to the minimum, so the changes forces the update
            MainForm->ZoomTrackBar->Position = MainForm->ZoomTrackBar->Min;

            // Set it to fit the width (most images are wider than taller)
            MainForm->ZoomTrackBar->Position =
                (int)(((float)MainForm->ImageBufferPanel->Width) /
                ((float)MainForm->Image->Picture->Width) * 99);

            // If it does not fit, adjust for height
            if(MainForm->Image->Height > MainForm->ImageBufferPanel->Height)
            {
                MainForm->ZoomTrackBar->Position =
                    (int)(((float)MainForm->ImageBufferPanel->Height) /
                    ((float)MainForm->Image->Picture->Height) * 99);
            }
    }
}

//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Exit1Click(TObject *Sender)
{
    MainForm->Close();    
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ZoomTrackBarChange(TObject *Sender)
{
    setImageZoom(ZoomTrackBar->Position);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
    MainForm->Caption = SOFTWARE_NAME + (AnsiString)" - " + SOFTWARE_AUTHOR + " - version: " + SOFTWARE_VERSION + " -- " + SOFTWARE_WEBPAGE;
    AboutTextLabel1->Caption = SOFTWARE_NAME + (AnsiString)"\nversion: " + SOFTWARE_VERSION;
    AboutTextLabel2->Caption = (AnsiString)SOFTWARE_AUTHOR;
    AboutTextLabel3->Caption = (AnsiString)SOFTWARE_WEBPAGE;

    curvesData = new TCurves();

    curveFaceColor = (TColor) 0xFF9966;
    curveBorderColor = (TColor) 0x000000;
    CurveFaceColorPanel->Color = curveFaceColor;
    CurveBorderColorPanel->Color = curveBorderColor;

    setImageZoom(ZoomTrackBar->Position);
    PageControl->ActivePage = TabSheetImage;

    loadedFilename = new AnsiString();

    TrimLeft->Value = curvesData->trimValues[0];
    TrimTop->Value = curvesData->trimValues[1];
    TrimRight->Value = curvesData->trimValues[2];
    TrimBottom->Value = curvesData->trimValues[3];

    curvesData->axes[0]->logarithmicBase = xAxisLogarithmicBase->Text.ToDouble();
    curvesData->axes[1]->logarithmicBase = xAxisLogarithmicBase->Text.ToDouble();

    originalImage = new TImage(this);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ZoomTextPanelDblClick(TObject *Sender)
{
    setZoomToMatchPanel();
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::LogarithmicXaxis1Click(TObject *Sender)
{
    ((TMenuItem *)Sender)->Checked = !((TMenuItem *)Sender)->Checked;
    setAxisType(0, ((TMenuItem *)Sender)->Checked);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::LogarithmicYaxis1Click(TObject *Sender)
{
    ((TMenuItem *)Sender)->Checked = !((TMenuItem *)Sender)->Checked;
    setAxisType(1, ((TMenuItem *)Sender)->Checked);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::LoadImageButtonClick(TObject *Sender)
{
    if(imageIsLoaded)
    {
        if(MessageBox(NULL, "An image is already loaded, override?", "",  MB_YESNO) == IDYES)
        {
            imageIsLoaded = False;
        }
    }

    if (!imageIsLoaded)
    {
        if(OpenPictureDialog->Execute())
        {
            setStatusText("Loading, please wait ...");
            
            imageIsLoaded = True;
            originalImage->Picture->LoadFromFile(OpenPictureDialog->FileName);
//            Image->Picture->LoadFromFile(OpenPictureDialog->FileName);
            Image->Picture = originalImage->Picture;

            curvesData->imageFilename = OpenPictureDialog->FileName.c_str();
            setZoomToMatchPanel();

            // Show the file name
            getOnlyFilename(&(OpenPictureDialog->FileName), &loadedFilename);
            setStatusTextToFilename();

            // Go to the page to create the axis
            //PageControl->ActivePage = TabSheetAxes;
        }
    }
}

void getOnlyFilename(AnsiString * filenameWithPath, AnsiString ** onlyFilename)
{
    if(filenameWithPath == NULL)
        return;

    char _onlyFilename[256];
    if((*onlyFilename) == NULL)
        (*onlyFilename) = new AnsiString();
    for (int i = filenameWithPath->Length()-1; i >= 0; i--)
    {
        if(filenameWithPath->c_str()[i] == '\\')
        {
            i++;
            int j;
            for(j=0; j <= (filenameWithPath->Length() - i -1); j++)
            {
                _onlyFilename[j] = (*filenameWithPath)[j+i+1];
            }
            _onlyFilename[j] = '\0';
            break;
        }
    }
    (*onlyFilename)->sprintf("%s", _onlyFilename);
}

void setStatusTextToFilename()
{
    AnsiString * _newCaption = new AnsiString();
    _newCaption->sprintf("%s  [%d x %d]",
                         loadedFilename->c_str(),
                         MainForm->Image->Picture->Width,
                         MainForm->Image->Picture->Height);
    setStatusText((AnsiString)(_newCaption->c_str()));
}

void setStatusText(AnsiString text)
{
    MainForm->PictureFilenameLabel->Caption = text.c_str();
    MainForm->Refresh();
}


//---------------------------------------------------------------------------

void closeImage(void)
{
    if (imageIsLoaded)
    {
        imageIsLoaded = False;
        setStatusText("-- No file loaded --");
        MainForm->Image->Picture = NULL;

        // Resize the image
        MainForm->Image->Width = 10;
        MainForm->Image->Height = 10;

        // Resize the paint-box
        MainForm->PaintedImage->Width = MainForm->Image->Width;
        MainForm->PaintedImage->Height = MainForm->Image->Height;

        curvesData->imageHeight = -1;
        curvesData->imageWidth = -1;
        free(curvesData->serializedImage);
        curvesData->serializedImageSize = -1;

        curvesData->imageFilename = "--no-image-loaded--";
    }
}

void __fastcall TMainForm::CloseImageButtonClick(TObject *Sender)
{
    if (imageIsLoaded)
    {
        if (MessageBox(NULL, "Remove loaded image from buffer?", "",  MB_YESNO) == IDYES)
        {
            closeImage();
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::LoadFromClipboardButtonClick(TObject *Sender)
{
    TClipboard *cb = Clipboard();
    if (cb->HasFormat(CF_BITMAP))
    {
        if(imageIsLoaded)
        {
            if(MessageBox(NULL, "An image is already loaded, override?", "",  MB_YESNO) == IDYES)
            {
                imageIsLoaded = False;
            }
        }
        if (!imageIsLoaded)
        {
            setStatusText("Loading, please wait ...");

            originalImage->Picture->LoadFromClipboardFormat(CF_BITMAP, cb->GetAsHandle(CF_BITMAP), 0);
            Image->Picture = originalImage->Picture;
            //Image->Picture->LoadFromClipboardFormat(CF_BITMAP, cb->GetAsHandle(CF_BITMAP), 0);

            Image->Width = Image->Picture->Width;
            Image->Height = Image->Picture->Height;
            imageIsLoaded = True;
            setZoomToMatchPanel();

            curvesData->imageFilename = "--image-from-clipboard--";

            // Go to the page to create the axis
            //PageControl->ActivePage = TabSheetAxes;

            loadedFilename->sprintf("Image loaded from clipboard");
            setStatusTextToFilename();
            //setStatusText("Image loaded from clipboard");
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::N1001Click(TObject *Sender)
{
    ZoomTrackBar->Position = 100;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Fitwindow1Click(TObject *Sender)
{
    setZoomToMatchPanel();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::N501Click(TObject *Sender)
{
    ZoomTrackBar->Position = 50;
}
//---------------------------------------------------------------------------

void setAxis(int axisIndex, TObject * Sender)
{
    bool _otherAxisIsBeingSet = False;
    for(int i=0; i<2; i++)
    {
        if(settingAxisPoints[i])
        {
            _otherAxisIsBeingSet = True;
        }
    }

    if(!imageIsLoaded)
    {
        ShowMessage("Load an image before creating axes!");
        return;
    }
    if(settingAxisPoints[axisIndex])
    {
        // Just finished setting points for this axis
        ((TButton *)Sender)->Caption = "Set axis limit points";
        ((TButton *)Sender)->Font->Style = TFontStyles();
        settingAxisPoints[axisIndex] = False;
        setStatusTextToFilename();
    }
    else
    {
        if (!_otherAxisIsBeingSet)
        {
            // Started the setting
            ((TButton *)Sender)->Caption = "Done";
            ((TButton *)Sender)->Font->Style = TFontStyles() << fsBold;
            settingAxisPoints[axisIndex] = True;
            setStatusText("[Click to set: left->start, right->end]");
        }
    }
}

void __fastcall TMainForm::xAxisSetButtonClick(TObject *Sender)
{
    if(imageIsLoaded)
        setAxis(0, Sender);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::yAxisSetButtonClick(TObject *Sender)
{
    if(imageIsLoaded)
        setAxis(1, Sender);
}
//---------------------------------------------------------------------------

void updateAxisLabels()
{
    // Update label for x-axis or y-axis
    AnsiString * _newString = new AnsiString();

    _newString->printf("Start: (%.2f; %5.2f) [%%]\nEnd: (%.2f; %.2f) [%%]",
                       100.0 * curvesData->axes[0]->startPoint.x,
                       100.0 * curvesData->axes[0]->startPoint.y,
                       100.0 * curvesData->axes[0]->endPoint.x,
                       100.0 * curvesData->axes[0]->endPoint.y);
    MainForm->xAxisSetLabel->Caption = _newString->c_str();
    _newString->printf("Start: (%.2f; %5.2f) [%%]\nEnd: (%.2f; %.2f) [%%]",
                       100.0 * curvesData->axes[1]->startPoint.x,
                       100.0 * curvesData->axes[1]->startPoint.y,
                       100.0 * curvesData->axes[1]->endPoint.x,
                       100.0 * curvesData->axes[1]->endPoint.y);
    MainForm->yAxisSetLabel->Caption = _newString->c_str();
}

void __fastcall TMainForm::PaintedImageMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(!imageIsLoaded)
    {
        return;
    }

    // Calculate the relative clicked point
    float xPos = ((float)Mouse->CursorPos.x - (float)PaintedImage->ClientOrigin.x) / (float)PaintedImage->Width;
    float yPos = ((float)Mouse->CursorPos.y - (float)PaintedImage->ClientOrigin.y) / (float)PaintedImage->Height;

    // If any axis is being set, capture points
    if (settingAxisPoints[0] || settingAxisPoints[1])
    {
        int _pointIndex = -1;
        if(Button == mbLeft)
        {
            // It is capturing the initial point
            _pointIndex = 0;
        }
        if(Button == mbRight)
        {
            // It is capturing the final point
            _pointIndex = 1;
        }
        if(_pointIndex == -1)
        {
            return;
        }

        // Get which axis is being set
        int _axisIndex;
        if (settingAxisPoints[0])
        {
            _axisIndex = 0;
        }
        if (settingAxisPoints[1])
        {
            _axisIndex = 1;
        }

        // Write the axis values to the curves data object
        if(_pointIndex == 0)
        {
            curvesData->axes[_axisIndex]->startPoint.x = xPos;
            curvesData->axes[_axisIndex]->startPoint.y = yPos;
        }
        if(_pointIndex == 1)
        {
            curvesData->axes[_axisIndex]->endPoint.x = xPos;
            curvesData->axes[_axisIndex]->endPoint.y = yPos;
        }

        updateAxisLabels();
        refreshPaintBox();
    }

    // If curve points are being captured, use the click data
    if(capturingCurvePoints)
    {
        if (curveBeingCapturedIndex <= curvesData->curvesCount()-1)
        {
            if(Button == mbLeft)
            {
                curvesData->getCurve(curveBeingCapturedIndex)->addPoint(xPos, yPos);
            }
            if(Button == mbRight)
            {
                curvesData->getCurve(curveBeingCapturedIndex)
                    ->removePoint(curvesData
                        ->getCurve(curveBeingCapturedIndex)->pointsCount()-1);
            }
            if((Button == mbLeft) || (Button == mbRight))
            {
                refreshPaintBox();
                refreshCurvesList();
            }
        }
    }
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::xAxisLogarithmicCheckBoxClick(TObject *Sender)
{
    axesTypes[0] = xAxisLogarithmicCheckBox->Checked;
    curvesData->axes[0]->isLogarithmic = xAxisLogarithmicCheckBox->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::yAxisLogarithmicCheckBoxClick(TObject *Sender)
{
    axesTypes[1] = yAxisLogarithmicCheckBox->Checked;
    curvesData->axes[1]->isLogarithmic = yAxisLogarithmicCheckBox->Checked;
}
//---------------------------------------------------------------------------

bool tryParseFloatNumber(TEdit * source, bool allowNegative)
{
    try
    {
        // Try to parse it
        float _parsedNumber = (float)source->Text.ToDouble();

        // If no negative values are allowed, make it positive
        if ((_parsedNumber < 0) && !allowNegative)
        {
            source->Text = - _parsedNumber;
        }
        else
        {
            //AnsiString * _newText = new AnsiString();
            //_newText->printf("%.4f", _parsedNumber);
            //source->Text = _newText->c_str();
        }
    }
    catch (const Exception& e)
    {
        // If the text can not be parsed, delete it
        source->Text = "";
        return False;
    }
    return True;
}

void __fastcall TMainForm::xAxisLogarithmicBaseChange(TObject *Sender)
{
    if(tryParseFloatNumber((TEdit *) Sender, False))
    {
        curvesData->axes[0]->logarithmicBase = (float)(((TEdit *)Sender)->Text.ToDouble());
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::yAxisLogarithmicBaseChange(TObject *Sender)
{
    if(tryParseFloatNumber((TEdit *) Sender, False))
    {
        curvesData->axes[1]->logarithmicBase = (float)(((TEdit *)Sender)->Text.ToDouble());
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::yAxisMinValueChange(TObject *Sender)
{
    if(tryParseFloatNumber((TEdit *) Sender, True))
    {
        curvesData->axes[1]->min = (float)((TEdit *) Sender)->Text.ToDouble();
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::yAxisMaxValueChange(TObject *Sender)
{
    if(tryParseFloatNumber((TEdit *) Sender, True))
    {
        curvesData->axes[1]->max = (float)((TEdit *) Sender)->Text.ToDouble();
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::xAxisMaxValueChange(TObject *Sender)
{
    if(tryParseFloatNumber((TEdit *) Sender, True))
    {
        curvesData->axes[0]->max = (float)((TEdit *) Sender)->Text.ToDouble();
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::xAxisMinValueChange(TObject *Sender)
{
    if(tryParseFloatNumber((TEdit *) Sender, True))
    {
        curvesData->axes[0]->min = (float)((TEdit *) Sender)->Text.ToDouble();
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ViewAxesClick(TObject *Sender)
{
    refreshPaintBox();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ScrollBox1Resize(TObject *Sender)
{
    refreshPaintBox();
}
//---------------------------------------------------------------------------

AnsiString * formatStringFromCurve(int curveIndex, TCurve * curve)
{
    AnsiString * _newString = new AnsiString();
    AnsiString * _enabledString = new AnsiString();
    if(curve->enabled)
    {
        _enabledString->sprintf("+");
    }
    else
    {
        _enabledString->sprintf("--");
    }

    _newString->printf("[%2.d][%s] <%s> (%d)", curveIndex +1,
                                                 _enabledString->c_str(),
                                                 curve->getName(),
                                                 curve->pointsCount());
    return _newString;
}

void addCurveToDisplayList(TListBox * listBox, TCurve * newCurve)
{
    listBox->Items->Add(formatStringFromCurve(listBox->Items->Count, newCurve)->c_str());
}

void refreshCurvesList()
{
    AnsiString * _testString = new AnsiString();

    if(MainForm->CurvesListBox->Items->Count > curvesData->curvesCount())
    {
        MainForm->CurvesListBox->Items->Clear();
    }

    for(int i=0; i<curvesData->curvesCount(); i++)
    {
        // If the item does not exist
        if(i >= MainForm->CurvesListBox->Items->Count)
        {
            addCurveToDisplayList(MainForm->CurvesListBox, curvesData->getCurve(i));
        }
        else
        {
            // Check if the data needs to be updated and do so then
            _testString = formatStringFromCurve(i, curvesData->getCurve(i));
            if ((*_testString) != MainForm->CurvesListBox->Items[0][i] )
            {
                MainForm->CurvesListBox->Items->Delete(i);
                MainForm->CurvesListBox->Items->Insert(i, _testString->c_str());
            }
        }
    }

    MainForm->CapturedCurvesTitle->Caption = "  Captured curves (" +
        (AnsiString)MainForm->CurvesListBox->Items->Count + ")  ";
}

void __fastcall TMainForm::AddCurveButtonClick(TObject *Sender)
{
    if(imageIsLoaded && !capturingCurvePoints)
    {
        curvesData->addNewCurve();
        refreshCurvesList();
        selectLastItem(MainForm->CurvesListBox);
    }
}
//---------------------------------------------------------------------------

void selectLastItem(TListBox * list)
{
    if (list == NULL)
    {
        return;
    }

    // Leave only the last item selected
    for (int i=0; i < list->Items->Count; i++)
    {
        if (i == list->Items->Count -1)
        {
            list->Selected[i] = True;
        }
        else
        {
            list->Selected[i] = False;
        }
    }
}

void renameSelectedCurve()
{
    if(capturingCurvePoints)
        return;

    if(curvesData->curvesCount() == 0)
        return;

    for(int _index = 0; _index < MainForm->CurvesListBox->Items->Count; _index++)
    {
        if(MainForm->CurvesListBox->Selected[_index])
        {
            // Ask user for new name
            AnsiString _newName = InputBox("Set name for curve #" + (AnsiString)(_index + 1), "",
                                           curvesData->getCurve(_index)->getName() );
            // If changed, update
            if (_newName != curvesData->getCurve(_index)->getName())
            {
                curvesData->getCurve(_index)->setName(_newName.c_str());
                refreshCurvesList();
            }
        }
    }
}

void __fastcall TMainForm::CurvesListBoxDblClick(TObject *Sender)
{
    if(imageIsLoaded)
        captureSelectedCurve();
}
//---------------------------------------------------------------------------

void toggleCurveEnabled()
{
    if(capturingCurvePoints)
        return;

    if(curvesData->curvesCount() == 0)
        return;

    for(int i = 0; i < MainForm->CurvesListBox->Items->Count; i++)
    {
        if(MainForm->CurvesListBox->Selected[i])
        {
            curvesData->getCurve(i)->enabled = !curvesData->getCurve(i)->enabled;
        }
    }
}

void removeSelectedCurve()
{
    if(capturingCurvePoints)
        return;

    if(curvesData->curvesCount() == 0)
        return;

    int _index, _deletedCurvesCount = 0;

    for(_index = 0; _index < MainForm->CurvesListBox->Items->Count; _index++)
    {
        if(MainForm->CurvesListBox->Selected[_index])
        {
            curvesData->removeCurve(_index - _deletedCurvesCount);
            _deletedCurvesCount ++;
        }
    }
    refreshCurvesList();
    refreshPaintBox();
}

void __fastcall TMainForm::RemoveCurveButtonClick(TObject *Sender)
{
    if(imageIsLoaded && !capturingCurvePoints)
    {
        removeSelectedCurve();
        selectLastItem(MainForm->CurvesListBox);
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Delete1Click(TObject *Sender)
{
    if(imageIsLoaded)
        removeSelectedCurve();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Rename1Click(TObject *Sender)
{
    if(imageIsLoaded)
        renameSelectedCurve();
}
//---------------------------------------------------------------------------

int findFirstSelectedItemInList(TListBox * list, bool deselectOthers)
{
    int _firstSelected = -1;
    for(int i=0; i<list->Items->Count; i++)
    {
        if (list->Selected[i])
        {
            _firstSelected = i;
            if(!deselectOthers)
            {
                return _firstSelected;
            }
            else
            {
                for(int j=i+1; j<list->Items->Count; j++)
                {
                    list->Selected[j] = False;
                }
                break;
            }
        }
    }

    return _firstSelected;
}

void captureSelectedCurve()
{
    // If there are no curves add an empty one
    if(curvesData->curvesCount() == 0)
    {
        return;
        /*
        curvesData->addNewCurve();
        refreshCurvesList();
        MainForm->CurvesListBox->Selected[0] = True;
        */
    }

    // Start capturing
    if (!capturingCurvePoints)
    {
        curveBeingCapturedIndex = findFirstSelectedItemInList(MainForm->CurvesListBox, True);
        if(curveBeingCapturedIndex < 0)
        {
            return;
            /*
            curvesData->addNewCurve();
            refreshCurvesList();
            curveBeingCapturedIndex = MainForm->CurvesListBox->Items->Count - 1;
            */
        }
        MainForm->CurvesListBox->Selected[curveBeingCapturedIndex] = True;

        MainForm->CapturePointsButton->Caption = "Done";
        MainForm->CurvesListBox->Enabled = False;
    }
    // Finish capturing
    else
    {
        MainForm->CurvesListBox->Selected[curveBeingCapturedIndex] = True;
        MainForm->CapturePointsButton->Caption = "Capture selected curve";
    }
    capturingCurvePoints = !capturingCurvePoints;
    MainForm->CurvesListBox->Enabled = !capturingCurvePoints;
    refreshPaintBox();
}

void __fastcall TMainForm::CapturePointsButtonClick(TObject *Sender)
{
    if(imageIsLoaded)
        captureSelectedCurve();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CaptureCurveMenuItemClick(TObject *Sender)
{
    if(imageIsLoaded & !capturingCurvePoints)
        captureSelectedCurve();
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::Addnew1Click(TObject *Sender)
{
    if(imageIsLoaded && !capturingCurvePoints)
    {
        curvesData->addNewCurve();
        refreshCurvesList();
    }
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::HideCurvesCheckBoxClick(TObject *Sender)
{
    refreshPaintBox();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CurvesListBoxClick(TObject *Sender)
{
    refreshPaintBox();    
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CurveFaceColorPanelClick(TObject *Sender)
{
    ColorDialog->Color = CurveFaceColorPanel->Color;
    if(ColorDialog->Execute())
    {
        CurveFaceColorPanel->Color = ColorDialog->Color;
        curveFaceColor = ColorDialog->Color;
        refreshPaintBox();
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CurveBorderColorPanelClick(TObject *Sender)
{
    ColorDialog->Color = CurveBorderColorPanel->Color;
    if(ColorDialog->Execute())
    {
        CurveBorderColorPanel->Color = ColorDialog->Color;
        curveBorderColor = ColorDialog->Color;
        refreshPaintBox();
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ShowCurveBorderCheckBoxClick(TObject *Sender)
{
    refreshPaintBox();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Selectall1Click(TObject *Sender)
{
    for(int i=0; i<CurvesListBox->Items->Count; i++)
    {
        MainForm->CurvesListBox->Selected[i] = True;
    }
    refreshPaintBox();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Selectnone1Click(TObject *Sender)
{
    for(int i=0; i<CurvesListBox->Items->Count; i++)
    {
        MainForm->CurvesListBox->Selected[i] = False;
    }
    refreshPaintBox();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Toggleenabled1Click(TObject *Sender)
{
    if(imageIsLoaded)
    {
        toggleCurveEnabled();
        refreshCurvesList();
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SaveXMLButtonClick(TObject *Sender)
{
    // If there are any curves or an image loaded, save
    if(curvesData->curvesCount() > 0 || imageIsLoaded)
    {
        if(SaveDialog->Execute())
        {
            bool _imageIsTrimmed = True;
            if(curvesData->trimValues[0] == 0 && curvesData->trimValues[1] == 0 &&
               curvesData->trimValues[2] == 100 && curvesData->trimValues[3] == 100)
            {
                _imageIsTrimmed = False;
            }

            setStatusText("Saving, please wait ...");
            if(_imageIsTrimmed && MessageBox(NULL, "Save only trimmed part of image?", "",  MB_YESNO) == IDYES)
            {
                // Serialize TRIMMED image and save to file
                curvesData->serializedImageSize = serializePicture(Image->Canvas,
                                                                   Image->Picture->Width,
                                                                   Image->Picture->Height,
                                                                   &(curvesData->serializedImage));
                // Save the image size to be able to reconstruct the image correctly
                curvesData->imageWidth = Image->Picture->Width;
                curvesData->imageHeight = Image->Picture->Height;
            }
            else
            {
                // Serialize FULL image and save to file
                curvesData->serializedImageSize = serializePicture(originalImage->Canvas,
                                                                   originalImage->Picture->Width,
                                                                   originalImage->Picture->Height,
                                                                   &(curvesData->serializedImage));
                // Save the image size to be able to reconstruct the image correctly
                curvesData->imageWidth = originalImage->Picture->Width;
                curvesData->imageHeight = originalImage->Picture->Height;
            }

            if(!curvesData->saveToXMLFile(SaveDialog->FileName.c_str()))
            {
                ShowMessage("ERROR: could not save the data to an XML file!");
            }
            setStatusTextToFilename();
            //setStatusText("Saved!");
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::LoadXMLButtonClick(TObject *Sender)
{
    if(isTrimming)
        return;

    if(imageIsLoaded)
    {
        if(MessageBox(NULL, "An image is already loaded, override?", "",  MB_YESNO) == IDYES)
        {
            imageIsLoaded = False;
        }
    }

    if(imageIsLoaded)
        return;    

    if(OpenDialog->Execute())
    {
        closeImage();
        setStatusText("Loading, please wait ...");
        if(!curvesData->loadFromXMLFile(OpenDialog->FileName.c_str()))
        {
            ShowMessage("Could not load data from XML head!");
        }

        // Clear the previous status
        CurvesListBox->Items->Clear();

        // Copy the picture to main
        originalImage->Picture->Bitmap = NULL;
        originalImage->Width = curvesData->imageWidth;
        originalImage->Height = curvesData->imageHeight;
        int _counter = 0;
        for(int i=0; i<curvesData->imageWidth; i++)
        {
            for(int j=0; j<curvesData->imageHeight; j++)
            {
                originalImage->Canvas->Pixels[i][j] = (TColor)curvesData->serializedImage[_counter];
                _counter ++;
            }
        }
        imageIsLoaded = True;

        refreshCurvesList();

        // Recall the trim values
        TrimLeft->Value = curvesData->trimValues[0];
        TrimTop->Value = curvesData->trimValues[1];
        TrimRight->Value = curvesData->trimValues[2];
        TrimBottom->Value = curvesData->trimValues[3];

        // Adjust the trimming if needed
        setImageTrimValues(curvesData->trimValues[0], curvesData->trimValues[1],
                           curvesData->trimValues[2], curvesData->trimValues[3]);

        // Zoom to fill by default
        setZoomToMatchPanel();

        // Load axes data
        xAxisMinValue->Text = (AnsiString)curvesData->axes[0]->min;
        xAxisMaxValue->Text = (AnsiString)curvesData->axes[0]->max;
        yAxisMinValue->Text = (AnsiString)curvesData->axes[1]->min;
        yAxisMaxValue->Text = (AnsiString)curvesData->axes[1]->max;
        xAxisLogarithmicCheckBox->Checked = curvesData->axes[0]->isLogarithmic;
        yAxisLogarithmicCheckBox->Checked = curvesData->axes[1]->isLogarithmic;
        xAxisLogarithmicBase->Text = (AnsiString)curvesData->axes[0]->logarithmicBase;
        yAxisLogarithmicBase->Text = (AnsiString)curvesData->axes[1]->logarithmicBase;

        AnsiString * _xmlFilename = NULL;
        getOnlyFilename(&(OpenDialog->FileName), &_xmlFilename);
        loadedFilename->sprintf("Loaded from \"%s\"", _xmlFilename->c_str());
        setStatusTextToFilename();
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TrimImageButtonClick(TObject *Sender)
{
    if(!imageIsLoaded)
        return;

    isTrimming = !isTrimming;
    if(isTrimming)
    {
        Image->Picture = originalImage->Picture;
        ((TButton *)Sender)->Caption = "Done";
        TrimLeft->Enabled = True;
        TrimRight->Enabled = True;
        TrimTop->Enabled = True;
        TrimBottom->Enabled = True;
    }
    else
    {
        ((TButton *)Sender)->Caption = "Trim image";
        TrimLeft->Enabled = False;
        TrimRight->Enabled = False;
        TrimTop->Enabled = False;
        TrimBottom->Enabled = False;

        setImageTrimValues(curvesData->trimValues[0], curvesData->trimValues[1],
                           curvesData->trimValues[2], curvesData->trimValues[3]);
    }
    refreshPaintBox();
    setZoomToMatchPanel();
}
//---------------------------------------------------------------------------

void setImageTrimValues(int left, int top, int right, int bottom)
{
    // Check if trimming is actually needed
    if(left == 0 && top == 0 && right == 100 && bottom == 100)
    {
        // Leave image as is originally
        MainForm->Image->Width = originalImage->Picture->Width;
        MainForm->Image->Height = originalImage->Picture->Height;
        MainForm->Image->Picture = originalImage->Picture;
    }
    else
    {
    // Trim image
    setStatusText("Trimming, please wait ...");
    int _newWidth = (((float)right - (float)left)/100.0) * (float)(originalImage->Picture->Width);
    int _newHeight = (((float)bottom - (float)top)/100.0) * (float)(originalImage->Picture->Height);
    int _widthOffset = ((float)left / 100.0) * (float)(originalImage->Picture->Width);
    int _heightOffset = ((float)top / 100.0) * (float)(originalImage->Picture->Height);
    MainForm->Image->Picture->Bitmap = NULL;
    MainForm->Image->Width = _newWidth;
    MainForm->Image->Height = _newHeight;
    for(int i=0; i<_newWidth; i++)
    {
        for(int j=0; j<_newHeight; j++)
        {
            MainForm->Image->Canvas->Pixels[i][j] = originalImage->Canvas->Pixels[i+_widthOffset][j+_heightOffset];
        }
    }
    setStatusTextToFilename();
        }

}

void __fastcall TMainForm::TrimLeftChange(TObject *Sender)
{
    try
    {
        curvesData->trimValues[0] = ((TCSpinEdit *)Sender)->Value;
        refreshPaintBox();
    }
    catch (const Exception& e)
    { }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TrimTopChange(TObject *Sender)
{
    try
    {
        curvesData->trimValues[1] = ((TCSpinEdit *)Sender)->Value;
        refreshPaintBox();
    }
    catch (const Exception& e)
    { }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TrimRightChange(TObject *Sender)
{
    try
    {
        curvesData->trimValues[2] = ((TCSpinEdit *)Sender)->Value;
        refreshPaintBox();
    }
    catch (const Exception& e)
    { }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TrimBottomChange(TObject *Sender)
{
    try
    {
        curvesData->trimValues[3] = ((TCSpinEdit *)Sender)->Value;
        refreshPaintBox();
    }
    catch (const Exception& e)
    { }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TrimPaintImageMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    TCurvePoint _point;
    _point.x = ((float) X / (float)Image->Width);
    _point.y = ((float) Y / (float)Image->Height);

    float _x, _y;
    convertXYPointToValues(&_point,
        &curvesData->axes[0]->startPoint, &curvesData->axes[0]->endPoint,
        &curvesData->axes[1]->startPoint, &curvesData->axes[1]->endPoint,
        &(curvesData->axes[0]->min), &(curvesData->axes[0]->max),
        &(curvesData->axes[1]->min), &(curvesData->axes[1]->max),
        curvesData->axes[0]->isLogarithmic, curvesData->axes[1]->isLogarithmic,
        curvesData->axes[0]->logarithmicBase, curvesData->axes[1]->logarithmicBase,
        &_x, &_y);

    AnsiString * _string = new AnsiString();
    if(curvesData->axes[0]->startPoint.x != curvesData->axes[0]->endPoint.x ||
       curvesData->axes[0]->startPoint.y != curvesData->axes[0]->endPoint.y)
    {
        _string->printf("%.1f", _x);
    }
    else
    {
        _string->printf("0.0");
    }
    XPosLabel->Caption = _string->c_str();

    if(curvesData->axes[1]->startPoint.x != curvesData->axes[1]->endPoint.x ||
       curvesData->axes[1]->startPoint.y != curvesData->axes[1]->endPoint.y)
    {
        _string->printf("%.1f", _y);
    }
    else
    {
        _string->printf("0.0");
    }
    YPosLabel->Caption = _string->c_str();
}
//---------------------------------------------------------------------------



void convertXYPointToValues(TCurvePoint * point, TCurvePoint * xAxisStart,
                            TCurvePoint * xAxisEnd, TCurvePoint * yAxisStart,
                            TCurvePoint * yAxisEnd, float * xMinScale,
                            float * xMaxScale, float * yMinScale, float * yMaxScale,
                            bool xIsLogarithmic, bool yIsLogarithmic, float xBase,
                            float yBase, float * xOut, float * yOut)
{
    if (point == NULL || xAxisStart == NULL || xAxisEnd == NULL || yAxisStart
        == NULL || yAxisEnd == NULL || xMinScale == NULL || xMaxScale == NULL ||
        yMinScale == NULL || yMaxScale == NULL || xOut == NULL || yOut == NULL)
        return;

    /*
        Variables identification:
            point: x,y point to convert
            xAxisStart: x,y point where x-axis starts
            xAxisEnd: x,y point where x-axis ends
            yAxisStart: x,y point where y-axis starts
            yAxisEnd: x,y point where y-axis ends
            xMinScale: min value found in the selected scale (x)
            xmaxScale: max value found in the selected scale (x)
            yMinScale: min value found in the selected scale (y)
            yMaxScale: max value found in the selected scale (y)
            xOut: converted value, x
            yOut: converted value, y
    */

    try
    {
        // Vector operations with the axes and projection of point
        TVector _vectorPoint, _xAxisStartVector, _xAxisEndVector;
        _vectorPoint.x = point->x;
        _vectorPoint.y = point->y;
        _xAxisStartVector.x = xAxisStart->x;
        _xAxisStartVector.y = xAxisStart->y;
        _xAxisEndVector.x = xAxisEnd->x;
        _xAxisEndVector.y = xAxisEnd->y;
        double _distanceProjectionX = projectPointOnAxis(_vectorPoint, _xAxisStartVector, _xAxisEndVector, NULL);
        double _xAxisLength = mod(sub(_xAxisEndVector, _xAxisStartVector));
        TVector _yAxisStartVector, _yAxisEndVector;
        _yAxisStartVector.x = yAxisStart->x;
        _yAxisStartVector.y = yAxisStart->y;
        _yAxisEndVector.x = yAxisEnd->x;
        _yAxisEndVector.y = yAxisEnd->y;
        double _distanceProjectionY = projectPointOnAxis(_vectorPoint, _yAxisStartVector, _yAxisEndVector, NULL);
        double _yAxisLength = mod(sub(_yAxisEndVector, _yAxisStartVector));

        // Initial scaling, relative to the image displayed in percentage
        if(_xAxisLength != 0)
            (*xOut) = _distanceProjectionX / _xAxisLength;
        else
            (*xOut) = 0;

        if(_yAxisLength != 0)
            (*yOut) = - _distanceProjectionY / _yAxisLength;
        else
            (*yOut) = 0;

        // Second scaling, relative to the scale introduced by the user
        float _amountOfDecades, _relativeScalePosition, _log_compensated_scale;
        if(xIsLogarithmic)
        {
            if((*xMinScale != 0) && (*xMaxScale / *xMinScale > 0))
            {
                _amountOfDecades = log(*xMaxScale / *xMinScale) / log(xBase);
                _relativeScalePosition = *xOut;
                _log_compensated_scale = (pow(xBase, _relativeScalePosition * _amountOfDecades) -1)
                                            / pow(xBase, _amountOfDecades);
                (*xOut) = ((_log_compensated_scale) * (*xMaxScale - *xMinScale)) + *xMinScale;
            }
            else
            {
                (*xOut) = 0;
            }
        }
        else
        {
           (*xOut) = ((*xOut) * (*xMaxScale - *xMinScale)) + *xMinScale;
        }

        if(yIsLogarithmic)
        {
            if((*yMinScale != 0) && (*yMaxScale / *yMinScale > 0))
            {
                _amountOfDecades = log(*yMaxScale / *yMinScale) / log(yBase);
                _relativeScalePosition = *yOut;
                _log_compensated_scale = (pow(yBase, _relativeScalePosition * _amountOfDecades) -1)
                                            / pow(yBase, _amountOfDecades);
                (*yOut) = ((_log_compensated_scale) * (*yMaxScale - *yMinScale)) + *yMinScale;
            }
            else
            {
                (*xOut) = 0;
            }

        }
        else
        {
            (*yOut) = ((*yOut) * (*yMaxScale - *yMinScale)) + *yMinScale;
        }
    }
    catch(const Exception &ex)
    {
        (*xOut) = 0;
        (*yOut) = 0;
    }
}

void __fastcall TMainForm::Button1Click(TObject *Sender)
{
    // If there are any curves or an image loaded, save
    if(curvesData->curvesCount() > 0 || imageIsLoaded)
    {
        if(MainForm->SaveDialogTextFile->Execute())
        {
            setStatusText("Saving, please wait ...");

            AnsiString *_newLineStart = new AnsiString("");
            FILE * _file = NULL;
            AnsiString *_extension = new AnsiString("");
            switch(MainForm->SaveDialogTextFile->FilterIndex)
            {
                // TXT
                case 1:
                    _extension->sprintf("txt");
                    break;
                // MAT
                case 2:
                    _extension->sprintf("mat");
                    break;
                // PY
                case 3:
                    _extension->sprintf("py");
                    break;
                // CSV
                case 4:
                    _extension->sprintf("csv");
                    break;
                // Unknown extension, treated as plain text
                default:
                    break;
            }
            _file = (FILE *)fopen(MainForm->SaveDialogTextFile->FileName
                                  .cat_sprintf(".%s", _extension->c_str()).c_str(), "wt");

            // Header of the file
            AnsiString *_text = new AnsiString("");
            switch(MainForm->SaveDialogTextFile->FilterIndex)
            {
                // MAT
                case 2:
                    _newLineStart->sprintf("%  ");
                    break;
                // PY
                case 3:
                    _newLineStart->sprintf("#  ");
                    break;
                // TXT
                case 1:
                // CSV
                case 4:
                // Unknown extension, treated as plain text
                default:
                    break;
            }
            _text->sprintf("%s%s (Curves capture tool) - version: %s\n%s%s\n%s%s\n%sFile generated automatically on %s at %s.\n%s----\n\n",
                           _newLineStart->c_str(),
                           SOFTWARE_NAME,
                           SOFTWARE_VERSION,
                           _newLineStart->c_str(),
                           SOFTWARE_AUTHOR,
                           _newLineStart->c_str(),
                           SOFTWARE_WEBPAGE,
                           _newLineStart->c_str(),
                           Date().DateString().c_str(),
                           Time().TimeString().c_str(),
                           _newLineStart->c_str());
            fwrite(_text->c_str(), sizeof(char), _text->Length() , _file);

            for(int i=0; i<curvesData->curvesCount(); i++)
            {
                _text->sprintf("%sCurve %d of %d - Name: \"%s\"\n",
                               _newLineStart->c_str(),
                               i+1, curvesData->curvesCount(),
                               curvesData->getCurve(i)->getName());
                fwrite(_text->c_str(), sizeof(char), _text->Length() , _file);

                // Create the curve variable definition if needed
                switch(MainForm->SaveDialogTextFile->FilterIndex)
                {
                    // MAT
                    case 2:
                        _text->sprintf("curve_%d = [\n", i);
                        fwrite(_text->c_str(), sizeof(char), _text->Length() , _file);
                        break;
                    // PY
                    case 3:
                        _text->sprintf("curve_%d = [\n", i);
                        fwrite(_text->c_str(), sizeof(char), _text->Length() , _file);
                        break;
                    // TXT
                    case 1:
                    // CSV
                    case 4:
                    default:
                        break;
                }

                // Write the curve points
                for (int j=0; j<curvesData->getCurve(i)->pointsCount(); j++)
                {
                    float _x, _y;
                    convertXYPointToValues(((TCurvePoint **)(curvesData->getCurve(i)->getPoints()))[j],
                                           &curvesData->axes[0]->startPoint, &curvesData->axes[0]->endPoint,
                                           &curvesData->axes[1]->startPoint, &curvesData->axes[1]->endPoint,
                                           &(curvesData->axes[0]->min), &(curvesData->axes[0]->max),
                                           &(curvesData->axes[1]->min), &(curvesData->axes[1]->max),
                                           curvesData->axes[0]->isLogarithmic, curvesData->axes[1]->isLogarithmic,
                                           curvesData->axes[0]->logarithmicBase, curvesData->axes[1]->logarithmicBase, 
                                           &_x, &_y);

                    switch(MainForm->SaveDialogTextFile->FilterIndex)
                    {
                        // MAT
                        case 2:
                            _text->sprintf("  %.3f, %.3f\n", _x, _y);
                            break;
                        // PY
                        case 3:
                            _text->sprintf("  (%.3f, %.3f),\n", _x, _y);
                            break;
                        // CSV
                        case 4:
                            _text->sprintf("\t%.3f\t%.3f\n", _x, _y);
                            break;
                        // TXT
                        case 1:
                        // Unknown extension
                        default:
                            _text->sprintf("  %.3f  %.3f\n", _x, _y);
                            break;
                    }
                    fwrite(_text->c_str(), sizeof(char), _text->Length() , _file);
                }

                // Close the variable
                switch(MainForm->SaveDialogTextFile->FilterIndex)
                {
                    // MAT
                    case 2:
                    // PY
                    case 3:
                        _text->sprintf("  ];\n");
                        break;
                    // TXT
                    case 1:
                    // CSV
                    case 4:
                    // Unknown extension
                    default:
                        _text->sprintf("\n");
                        break;
                }
                fwrite(_text->c_str(), sizeof(char), _text->Length() , _file);
            }
            fclose(_file);

            setStatusTextToFilename();
        }
    }
}
//---------------------------------------------------------------------------




