object MainForm: TMainForm
  Left = 323
  Top = 183
  Width = 818
  Height = 500
  BorderWidth = 5
  Caption = 'Curves capture tool'
  Color = clBtnFace
  Constraints.MinHeight = 500
  Constraints.MinWidth = 655
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -10
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 193
    Top = 0
    Width = 6
    Height = 463
    Cursor = crHSplit
    ResizeStyle = rsLine
  end
  object PageControl: TPageControl
    Left = 0
    Top = 0
    Width = 193
    Height = 463
    ActivePage = TabSheet1
    Align = alLeft
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    MultiLine = True
    ParentFont = False
    TabIndex = 4
    TabOrder = 0
    TabPosition = tpLeft
    object TabSheetImage: TTabSheet
      Caption = ' Image '
      object LoadImageButton: TButton
        Left = 7
        Top = 7
        Width = 150
        Height = 20
        Caption = 'Load image from file'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        OnClick = LoadImageButtonClick
      end
      object CloseImageButton: TButton
        Left = 7
        Top = 59
        Width = 150
        Height = 20
        Caption = 'Close image'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
        OnClick = CloseImageButtonClick
      end
      object LoadFromClipboardButton: TButton
        Left = 7
        Top = 33
        Width = 150
        Height = 20
        Caption = 'Paste from clipboard'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        OnClick = LoadFromClipboardButtonClick
      end
      object GroupBox3: TGroupBox
        Left = 7
        Top = 85
        Width = 150
        Height = 143
        Caption = '  Image edition  '
        Ctl3D = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 3
        object Label9: TLabel
          Left = 5
          Top = 82
          Width = 18
          Height = 13
          Caption = 'Left'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label10: TLabel
          Left = 34
          Top = 50
          Width = 19
          Height = 13
          Caption = 'Top'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label11: TLabel
          Left = 70
          Top = 82
          Width = 25
          Height = 13
          Caption = 'Right'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label12: TLabel
          Left = 22
          Top = 115
          Width = 33
          Height = 13
          Caption = 'Bottom'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object TrimImageButton: TButton
          Left = 7
          Top = 20
          Width = 137
          Height = 20
          Caption = 'Trim image'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnClick = TrimImageButtonClick
        end
        object TrimLeft: TCSpinEdit
          Left = 26
          Top = 78
          Width = 40
          Height = 22
          Color = clWhite
          Ctl3D = True
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          MaxValue = 100
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 1
          OnChange = TrimLeftChange
        end
        object TrimTop: TCSpinEdit
          Left = 59
          Top = 46
          Width = 39
          Height = 22
          Color = clWhite
          Ctl3D = True
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          MaxValue = 100
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 2
          OnChange = TrimTopChange
        end
        object TrimRight: TCSpinEdit
          Left = 98
          Top = 78
          Width = 39
          Height = 22
          Color = clWhite
          Ctl3D = True
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          MaxValue = 100
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 3
          OnChange = TrimRightChange
        end
        object TrimBottom: TCSpinEdit
          Left = 59
          Top = 111
          Width = 39
          Height = 22
          Color = clWhite
          Ctl3D = True
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          MaxValue = 100
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 4
          OnChange = TrimBottomChange
        end
      end
    end
    object TabSheetAxes: TTabSheet
      Caption = ' Axes '
      ImageIndex = 1
      object GroupBox1: TGroupBox
        Left = 6
        Top = 7
        Width = 151
        Height = 163
        Caption = '  X - Axis  '
        Ctl3D = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 0
        object Label2: TLabel
          Left = 13
          Top = 112
          Width = 55
          Height = 13
          Caption = 'Max. value:'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label1: TLabel
          Left = 13
          Top = 86
          Width = 52
          Height = 13
          Caption = 'Min. value:'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object xAxisSetLabel: TLabel
          Left = 7
          Top = 49
          Width = 137
          Height = 30
          AutoSize = False
          Caption = 'Points not set'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Pitch = fpFixed
          Font.Style = []
          ParentFont = False
          WordWrap = True
        end
        object Label6: TLabel
          Left = 78
          Top = 138
          Width = 27
          Height = 13
          Caption = 'Base:'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object xAxisMaxValue: TEdit
          Left = 91
          Top = 111
          Width = 53
          Height = 19
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          Text = '0'
          OnChange = xAxisMaxValueChange
        end
        object xAxisMinValue: TEdit
          Left = 91
          Top = 85
          Width = 53
          Height = 19
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          Text = '0'
          OnChange = xAxisMinValueChange
        end
        object xAxisSetButton: TButton
          Left = 7
          Top = 16
          Width = 137
          Height = 24
          Caption = 'Set axis limit points'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnClick = xAxisSetButtonClick
        end
        object xAxisLogarithmicCheckBox: TCheckBox
          Left = 13
          Top = 138
          Width = 40
          Height = 14
          Caption = 'Log'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          OnClick = xAxisLogarithmicCheckBoxClick
        end
        object xAxisLogarithmicBase: TEdit
          Left = 111
          Top = 137
          Width = 33
          Height = 19
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          Text = '10'
          OnChange = xAxisLogarithmicBaseChange
        end
      end
      object GroupBox2: TGroupBox
        Left = 6
        Top = 176
        Width = 151
        Height = 163
        Caption = '  Y - Axis  '
        Ctl3D = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 1
        object Label3: TLabel
          Left = 13
          Top = 112
          Width = 55
          Height = 13
          Caption = 'Max. value:'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label4: TLabel
          Left = 13
          Top = 86
          Width = 52
          Height = 13
          Caption = 'Min. value:'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object yAxisSetLabel: TLabel
          Left = 7
          Top = 49
          Width = 137
          Height = 30
          AutoSize = False
          Caption = 'Points not set'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label5: TLabel
          Left = 78
          Top = 138
          Width = 27
          Height = 13
          Caption = 'Base:'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object yAxisMaxValue: TEdit
          Left = 91
          Top = 111
          Width = 53
          Height = 19
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          Text = '0'
          OnChange = yAxisMaxValueChange
        end
        object yAxisMinValue: TEdit
          Left = 91
          Top = 85
          Width = 53
          Height = 19
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          Text = '0'
          OnChange = yAxisMinValueChange
        end
        object yAxisSetButton: TButton
          Left = 7
          Top = 16
          Width = 137
          Height = 24
          Caption = 'Set axis limit points'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnClick = yAxisSetButtonClick
        end
        object yAxisLogarithmicCheckBox: TCheckBox
          Left = 13
          Top = 138
          Width = 40
          Height = 14
          Caption = 'Log'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          OnClick = yAxisLogarithmicCheckBoxClick
        end
        object yAxisLogarithmicBase: TEdit
          Left = 111
          Top = 137
          Width = 33
          Height = 19
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          Text = '10'
          OnChange = yAxisLogarithmicBaseChange
        end
      end
      object ViewAxes: TCheckBox
        Left = 7
        Top = 345
        Width = 124
        Height = 13
        Caption = 'Show axes geometry'
        Checked = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        State = cbChecked
        TabOrder = 2
        OnClick = ViewAxesClick
      end
    end
    object TabSheetCurves: TTabSheet
      BorderWidth = 5
      Caption = ' Curves '
      ImageIndex = 2
      object Panel1: TPanel
        Left = 0
        Top = 0
        Width = 156
        Height = 59
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        object RemoveCurveButton: TButton
          Left = 40
          Top = 7
          Width = 26
          Height = 20
          Caption = '-'
          TabOrder = 1
          OnClick = RemoveCurveButtonClick
        end
        object AddCurveButton: TButton
          Left = 7
          Top = 7
          Width = 27
          Height = 20
          Caption = '+'
          TabOrder = 0
          OnClick = AddCurveButtonClick
        end
        object CapturePointsButton: TButton
          Left = 7
          Top = 33
          Width = 143
          Height = 20
          Caption = 'Capture selected curve'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          OnClick = CapturePointsButtonClick
        end
      end
      object CapturedCurvesTitle: TGroupBox
        Left = 0
        Top = 59
        Width = 156
        Height = 312
        Align = alClient
        Caption = '  Captured curves (0) '
        Ctl3D = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 1
        object CurvesListBox: TListBox
          Left = 1
          Top = 14
          Width = 154
          Height = 297
          Align = alClient
          BevelOuter = bvNone
          BorderStyle = bsNone
          Color = clBtnFace
          Ctl3D = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 13
          MultiSelect = True
          ParentCtl3D = False
          ParentFont = False
          ParentShowHint = False
          PopupMenu = CurvesPopupMenu
          ShowHint = True
          TabOrder = 0
          OnClick = CurvesListBoxClick
          OnDblClick = CurvesListBoxDblClick
        end
      end
      object Panel4: TPanel
        Left = 0
        Top = 371
        Width = 156
        Height = 74
        Align = alBottom
        BevelOuter = bvNone
        TabOrder = 2
        object Label7: TLabel
          Left = 18
          Top = 6
          Width = 78
          Height = 13
          Caption = 'Curve face color'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label8: TLabel
          Left = 18
          Top = 24
          Width = 87
          Height = 13
          Caption = 'Curve border color'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object HideCurvesCheckBox: TCheckBox
          Left = 0
          Top = 58
          Width = 150
          Height = 14
          Caption = 'Hide others while capturing'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnClick = HideCurvesCheckBoxClick
        end
        object ShowCurveBorderCheckBox: TCheckBox
          Left = 0
          Top = 43
          Width = 118
          Height = 14
          Caption = 'Show curves border'
          Checked = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          State = cbChecked
          TabOrder = 0
          OnClick = ShowCurveBorderCheckBoxClick
        end
        object CurveFaceColorPanel: TPanel
          Left = 0
          Top = 7
          Width = 13
          Height = 13
          TabOrder = 2
          OnClick = CurveFaceColorPanelClick
        end
        object CurveBorderColorPanel: TPanel
          Left = 0
          Top = 25
          Width = 13
          Height = 13
          TabOrder = 3
          OnClick = CurveBorderColorPanelClick
        end
      end
    end
    object TabSheetData: TTabSheet
      Caption = ' Data '
      ImageIndex = 3
      object GroupBox4: TGroupBox
        Left = 7
        Top = 7
        Width = 150
        Height = 72
        Caption = '  Project recall  '
        Ctl3D = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 0
        object SaveXMLButton: TButton
          Left = 7
          Top = 46
          Width = 137
          Height = 20
          Caption = 'Save project to XML'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnClick = SaveXMLButtonClick
        end
        object LoadXMLButton: TButton
          Left = 7
          Top = 20
          Width = 137
          Height = 20
          Caption = 'Load project from XML'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnClick = LoadXMLButtonClick
        end
      end
      object GroupBox5: TGroupBox
        Left = 7
        Top = 85
        Width = 150
        Height = 46
        Caption = '  Data export  '
        Ctl3D = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 1
        object Button1: TButton
          Left = 7
          Top = 20
          Width = 137
          Height = 20
          Caption = 'Export curves to text file'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnClick = Button1Click
        end
      end
    end
    object TabSheet1: TTabSheet
      BorderWidth = 5
      Caption = ' About '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImageIndex = 4
      ParentFont = False
      object AboutTextLabel1: TLabel
        Left = 0
        Top = 0
        Width = 156
        Height = 13
        Align = alTop
        Caption = 'AboutTextLabel1'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        WordWrap = True
      end
      object AboutTextLabel2: TLabel
        Left = 0
        Top = 13
        Width = 156
        Height = 13
        Align = alTop
        Caption = 'AboutTextLabel2'
        WordWrap = True
      end
      object AboutTextLabel3: TLabel
        Left = 0
        Top = 26
        Width = 156
        Height = 13
        Align = alTop
        Caption = 'AboutTextLabel3'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        WordWrap = True
      end
    end
  end
  object ImagePanel: TPanel
    Left = 199
    Top = 0
    Width = 601
    Height = 463
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 1
    object Panel2: TPanel
      Left = 0
      Top = 443
      Width = 601
      Height = 20
      Align = alBottom
      TabOrder = 0
      object PictureFilenameLabel: TLabel
        Left = 105
        Top = 1
        Width = 340
        Height = 18
        Align = alClient
        Alignment = taRightJustify
        AutoSize = False
        BiDiMode = bdLeftToRight
        Caption = '-- No file loaded --'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentBiDiMode = False
        ParentFont = False
      end
      object ZoomTrackBar: TTrackBar
        Left = 454
        Top = 1
        Width = 93
        Height = 18
        Align = alRight
        Ctl3D = False
        Max = 500
        Min = 10
        Orientation = trHorizontal
        ParentCtl3D = False
        PageSize = 50
        PopupMenu = ZoomPopupMenu
        Frequency = 1
        Position = 10
        SelEnd = 0
        SelStart = 0
        TabOrder = 0
        TabStop = False
        ThumbLength = 15
        TickMarks = tmBoth
        TickStyle = tsNone
        OnChange = ZoomTrackBarChange
      end
      object Panel3: TPanel
        Left = 445
        Top = 1
        Width = 9
        Height = 18
        Align = alRight
        BevelOuter = bvNone
        TabOrder = 1
      end
      object ZoomTextPanel: TPanel
        Left = 547
        Top = 1
        Width = 53
        Height = 18
        Align = alRight
        BevelOuter = bvNone
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        PopupMenu = ZoomPopupMenu
        TabOrder = 2
        OnDblClick = ZoomTextPanelDblClick
      end
      object MousePositionPanel: TPanel
        Left = 1
        Top = 1
        Width = 104
        Height = 18
        Align = alLeft
        BevelOuter = bvLowered
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 3
        object XPosLabel: TLabel
          Left = 1
          Top = 1
          Width = 50
          Height = 16
          Align = alLeft
          Alignment = taRightJustify
          AutoSize = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clGreen
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          Transparent = True
        end
        object YPosLabel: TLabel
          Left = 53
          Top = 1
          Width = 50
          Height = 16
          Align = alRight
          AutoSize = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clMaroon
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          Transparent = True
        end
      end
    end
    object ImageBufferPanel: TPanel
      Left = 0
      Top = 0
      Width = 601
      Height = 443
      Align = alClient
      BevelOuter = bvNone
      Color = clGray
      TabOrder = 1
      object ScrollBox1: TScrollBox
        Left = 0
        Top = 0
        Width = 601
        Height = 443
        Align = alClient
        BevelInner = bvNone
        BevelOuter = bvNone
        BorderStyle = bsNone
        TabOrder = 0
        OnResize = ScrollBox1Resize
        object Image: TImage
          Left = 0
          Top = 0
          Width = 85
          Height = 85
          Stretch = True
        end
        object PaintedImage: TImage
          Left = 0
          Top = 0
          Width = 53
          Height = 53
          Stretch = True
          Transparent = True
          OnMouseUp = PaintedImageMouseUp
        end
        object TrimPaintImage: TImage
          Left = 0
          Top = 0
          Width = 33
          Height = 33
          Stretch = True
          Transparent = True
          OnMouseMove = TrimPaintImageMouseMove
          OnMouseUp = PaintedImageMouseUp
        end
      end
    end
  end
  object OpenPictureDialog: TOpenPictureDialog
    Filter = 
      'All (*.jpg;*.jpeg;*.bmp)|*.jpg;*.jpeg;*.bmp|JPEG Image File (*.j' +
      'pg)|*.jpg|JPEG Image File (*.jpeg)|*.jpeg|Bitmaps (*.bmp)|*.bmp'
    Left = 240
    Top = 400
  end
  object ZoomPopupMenu: TPopupMenu
    Left = 272
    Top = 400
    object Fitwindow1: TMenuItem
      Caption = 'Fit window'
      OnClick = Fitwindow1Click
    end
    object N1001: TMenuItem
      Caption = '100 %'
      OnClick = N1001Click
    end
    object N501: TMenuItem
      Caption = '50 %'
      OnClick = N501Click
    end
  end
  object CurvesPopupMenu: TPopupMenu
    Left = 304
    Top = 400
    object CaptureCurveMenuItem: TMenuItem
      Caption = 'Capture points of selected curve'
      OnClick = CaptureCurveMenuItemClick
    end
    object Rename1: TMenuItem
      Caption = 'Rename'
      OnClick = Rename1Click
    end
    object Delete1: TMenuItem
      Caption = 'Delete'
      OnClick = Delete1Click
    end
    object Addnew1: TMenuItem
      Caption = 'Add new'
      OnClick = Addnew1Click
    end
    object Toggleenabled1: TMenuItem
      Caption = 'Toggle enabled'
      OnClick = Toggleenabled1Click
    end
    object Selectall1: TMenuItem
      Caption = 'Select all'
      OnClick = Selectall1Click
    end
    object Selectnone1: TMenuItem
      Caption = 'Select none'
      OnClick = Selectnone1Click
    end
  end
  object ColorDialog: TColorDialog
    Ctl3D = True
    Left = 336
    Top = 400
  end
  object SaveDialog: TSaveDialog
    DefaultExt = '.xml'
    Filter = 'XML files|*.xml|All files|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Left = 368
    Top = 400
  end
  object OpenDialog: TOpenDialog
    Filter = 'XML files|*.xml|All files|*.*'
    Left = 400
    Top = 400
  end
  object SaveDialogTextFile: TSaveDialog
    Filter = 
      'Text file (*.txt)|*.txt|Matlab file (*.mat)|*.mat|Python file (*' +
      '.py)|*.py|CSV file (*.csv)|*.csv'
    Left = 432
    Top = 400
  end
end
