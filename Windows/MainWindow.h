//---------------------------------------------------------------------------

#ifndef MainWindowH
#define MainWindowH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>

#include <msxmldom.hpp>
#include <XMLDoc.hpp>
#include <xmldom.hpp>
#include <XMLIntf.hpp>
#include <XMLBrokr.hpp>

#include <xmldom.hpp>
#include <Xmlxform.hpp>
#include <msxmldom.hpp>
#include <XMLDoc.hpp>
#include <XMLIntf.hpp>
#include <oxmldom.hpp>
#include "CSPIN.h"
#include "vectors.h"


//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
    TOpenPictureDialog *OpenPictureDialog;
    TSplitter *Splitter1;
    TPageControl *PageControl;
    TTabSheet *TabSheetImage;
    TTabSheet *TabSheetAxes;
    TTabSheet *TabSheetCurves;
    TTabSheet *TabSheetData;
    TPanel *ImagePanel;
    TPanel *Panel2;
    TLabel *PictureFilenameLabel;
    TTrackBar *ZoomTrackBar;
    TPanel *Panel3;
    TPanel *ZoomTextPanel;
    TPanel *MousePositionPanel;
    TButton *LoadImageButton;
    TButton *CloseImageButton;
    TButton *LoadFromClipboardButton;
    TPopupMenu *ZoomPopupMenu;
    TMenuItem *N1001;
    TMenuItem *Fitwindow1;
    TMenuItem *N501;
    TGroupBox *GroupBox1;
    TLabel *Label2;
    TEdit *xAxisMaxValue;
    TEdit *xAxisMinValue;
    TLabel *Label1;
    TLabel *xAxisSetLabel;
    TButton *xAxisSetButton;
    TCheckBox *xAxisLogarithmicCheckBox;
    TEdit *xAxisLogarithmicBase;
    TGroupBox *GroupBox2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *yAxisSetLabel;
    TEdit *yAxisMaxValue;
    TEdit *yAxisMinValue;
    TButton *yAxisSetButton;
    TCheckBox *yAxisLogarithmicCheckBox;
    TEdit *yAxisLogarithmicBase;
    TCheckBox *ViewAxes;
    TLabel *Label5;
    TLabel *Label6;
    TPopupMenu *CurvesPopupMenu;
    TMenuItem *Rename1;
    TMenuItem *CaptureCurveMenuItem;
    TMenuItem *Delete1;
    TPanel *Panel1;
    TButton *RemoveCurveButton;
    TButton *AddCurveButton;
    TButton *CapturePointsButton;
    TGroupBox *CapturedCurvesTitle;
    TListBox *CurvesListBox;
    TMenuItem *Addnew1;
    TPanel *Panel4;
    TCheckBox *HideCurvesCheckBox;
    TCheckBox *ShowCurveBorderCheckBox;
    TColorDialog *ColorDialog;
    TPanel *CurveFaceColorPanel;
    TPanel *CurveBorderColorPanel;
    TLabel *Label7;
    TLabel *Label8;
    TMenuItem *Selectall1;
    TMenuItem *Selectnone1;
    TMenuItem *Toggleenabled1;
    TSaveDialog *SaveDialog;
    TOpenDialog *OpenDialog;
    TGroupBox *GroupBox3;
    TButton *TrimImageButton;
    TCSpinEdit *TrimLeft;
    TCSpinEdit *TrimTop;
    TCSpinEdit *TrimRight;
    TCSpinEdit *TrimBottom;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TGroupBox *GroupBox4;
    TButton *SaveXMLButton;
    TButton *LoadXMLButton;
    TGroupBox *GroupBox5;
    TButton *Button1;
    TTabSheet *TabSheet1;
    TLabel *AboutTextLabel1;
    TLabel *AboutTextLabel2;
    TLabel *AboutTextLabel3;
    TSaveDialog *SaveDialogTextFile;
    TPanel *ImageBufferPanel;
    TScrollBox *ScrollBox1;
    TImage *Image;
    TImage *PaintedImage;
    TImage *TrimPaintImage;
    TLabel *XPosLabel;
    TLabel *YPosLabel;


    void __fastcall Exit1Click(TObject *Sender);
    void __fastcall ZoomTrackBarChange(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall ZoomTextPanelDblClick(TObject *Sender);
    void __fastcall LogarithmicXaxis1Click(TObject *Sender);
    void __fastcall LogarithmicYaxis1Click(TObject *Sender);
    void __fastcall LoadImageButtonClick(TObject *Sender);
    void __fastcall CloseImageButtonClick(TObject *Sender);
    void __fastcall LoadFromClipboardButtonClick(TObject *Sender);
    void __fastcall N1001Click(TObject *Sender);
    void __fastcall Fitwindow1Click(TObject *Sender);
    void __fastcall N501Click(TObject *Sender);
    void __fastcall xAxisSetButtonClick(TObject *Sender);
    void __fastcall yAxisSetButtonClick(TObject *Sender);
    void __fastcall PaintedImageMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall xAxisLogarithmicCheckBoxClick(TObject *Sender);
    void __fastcall yAxisLogarithmicCheckBoxClick(TObject *Sender);
    void __fastcall xAxisLogarithmicBaseChange(TObject *Sender);
    void __fastcall yAxisLogarithmicBaseChange(TObject *Sender);
    void __fastcall yAxisMinValueChange(TObject *Sender);
    void __fastcall yAxisMaxValueChange(TObject *Sender);
    void __fastcall xAxisMaxValueChange(TObject *Sender);
    void __fastcall xAxisMinValueChange(TObject *Sender);
    void __fastcall ViewAxesClick(TObject *Sender);
    void __fastcall ScrollBox1Resize(TObject *Sender);
    void __fastcall AddCurveButtonClick(TObject *Sender);
    void __fastcall CurvesListBoxDblClick(TObject *Sender);
    void __fastcall RemoveCurveButtonClick(TObject *Sender);
    void __fastcall Delete1Click(TObject *Sender);
    void __fastcall Rename1Click(TObject *Sender);
    void __fastcall CapturePointsButtonClick(TObject *Sender);
    void __fastcall CaptureCurveMenuItemClick(TObject *Sender);
    void __fastcall Addnew1Click(TObject *Sender);
    void __fastcall HideCurvesCheckBoxClick(TObject *Sender);
    void __fastcall CurvesListBoxClick(TObject *Sender);
    void __fastcall CurveFaceColorPanelClick(TObject *Sender);
    void __fastcall CurveBorderColorPanelClick(TObject *Sender);
    void __fastcall ShowCurveBorderCheckBoxClick(TObject *Sender);
    void __fastcall Selectall1Click(TObject *Sender);
    void __fastcall Selectnone1Click(TObject *Sender);
    void __fastcall Toggleenabled1Click(TObject *Sender);
    void __fastcall SaveXMLButtonClick(TObject *Sender);
    void __fastcall LoadXMLButtonClick(TObject *Sender);
    void __fastcall TrimImageButtonClick(TObject *Sender);
    void __fastcall TrimLeftChange(TObject *Sender);
    void __fastcall TrimTopChange(TObject *Sender);
    void __fastcall TrimRightChange(TObject *Sender);
    void __fastcall TrimBottomChange(TObject *Sender);
    void __fastcall TrimPaintImageMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
    void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
