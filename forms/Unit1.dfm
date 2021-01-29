object Form1: TForm1
  Left = 283
  Top = 127
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'MyCommander'
  ClientHeight = 680
  ClientWidth = 1370
  Color = clCream
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDefault
  Visible = True
  WindowState = wsMaximized
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 16
  object LabelCurrentPathRight: TLabel
    Left = 552
    Top = 41
    Width = 497
    Height = 17
    AutoSize = False
    Caption = 'LabelCurrentPathRight'
    Color = clGradientInactiveCaption
    ParentColor = False
  end
  object LabelCurrentPathLeft: TLabel
    Left = 8
    Top = 40
    Width = 489
    Height = 17
    AutoSize = False
    Caption = 'LabelCurrentPathLeft'
    Color = 15329769
    ParentColor = False
  end
  object LabelFormatting: TLabel
    Left = 1112
    Top = 64
    Width = 149
    Height = 20
    Caption = #1060#1086#1088#1084#1072#1090#1080#1088#1086#1074#1072#1085#1080#1077
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object FileListLeft: TStringGrid
    Left = 8
    Top = 64
    Width = 530
    Height = 601
    Color = 12895428
    ColCount = 6
    DefaultRowHeight = 17
    FixedCols = 0
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect, goThumbTracking]
    TabOrder = 2
    OnDblClick = FileListLeftDblClick
    OnEnter = FileListLeftEnter
    OnKeyDown = FileListLeftKeyDown
    ColWidths = (
      145
      77
      82
      126
      129
      131)
  end
  object ListDevicesLeft: TComboBox
    Left = 8
    Top = 4
    Width = 161
    Height = 24
    Cursor = crHandPoint
    Style = csDropDownList
    Color = clGradientInactiveCaption
    Sorted = True
    TabOrder = 0
    TabStop = False
    OnEnter = ListDevicesLeftEnter
    OnSelect = ListDevicesLeftSelect
  end
  object ListDevicesRight: TComboBox
    Left = 552
    Top = 4
    Width = 161
    Height = 24
    Cursor = crHandPoint
    Style = csDropDownList
    Color = clGradientInactiveCaption
    Sorted = True
    TabOrder = 1
    TabStop = False
    OnEnter = ListDevicesRightEnter
    OnSelect = ListDevicesRightSelect
  end
  object FileListRight: TStringGrid
    Left = 552
    Top = 64
    Width = 529
    Height = 601
    Color = 12895428
    ColCount = 6
    DefaultRowHeight = 17
    FixedCols = 0
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect]
    TabOrder = 3
    OnDblClick = FileListRightDblClick
    OnEnter = FileListRightEnter
    OnKeyDown = FileListRightKeyDown
    ColWidths = (
      148
      80
      82
      125
      125
      132)
  end
  object bRemove: TButton
    Left = 1112
    Top = 335
    Width = 177
    Height = 25
    Cursor = crHandPoint
    Caption = #1055#1077#1088#1077#1084#1077#1089#1090#1080#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 4
    TabStop = False
    OnClick = bRemoveClick
  end
  object bCopy: TButton
    Left = 1112
    Top = 296
    Width = 177
    Height = 25
    Cursor = crHandPoint
    Caption = #1050#1086#1087#1080#1088#1086#1074#1072#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 5
    TabStop = False
    OnClick = bCopyClick
  end
  object bCreateDir: TButton
    Left = 1112
    Top = 374
    Width = 177
    Height = 25
    Cursor = crHandPoint
    Caption = #1057#1086#1079#1076#1072#1090#1100' '#1082#1072#1090#1072#1083#1086#1075
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 6
    TabStop = False
    OnClick = bCreateDirClick
  end
  object bDelete: TButton
    Left = 1112
    Top = 415
    Width = 177
    Height = 25
    Cursor = crHandPoint
    Caption = #1059#1076#1072#1083#1080#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 7
    TabStop = False
    OnClick = bDeleteClick
  end
  object bCurrentRootDirLeft: TButton
    Left = 176
    Top = 4
    Width = 33
    Height = 25
    Cursor = crHandPoint
    Caption = '\'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 8
    TabStop = False
    OnClick = bCurrentRootDirLeftClick
  end
  object bCurrentRootDirRight: TButton
    Left = 719
    Top = 4
    Width = 33
    Height = 25
    Cursor = crHandPoint
    Caption = '\'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 9
    TabStop = False
    OnClick = bCurrentRootDirRightClick
  end
  object QuickFormatting: TCheckBox
    Left = 1112
    Top = 90
    Width = 193
    Height = 17
    Cursor = crHandPoint
    Caption = #1041#1099#1089#1090#1088#1086#1077' '#1092#1086#1088#1084#1072#1090#1080#1088#1086#1074#1072#1085#1080#1077
    Checked = True
    State = cbChecked
    TabOrder = 10
  end
  object bFormatting: TButton
    Left = 1112
    Top = 136
    Width = 177
    Height = 25
    Cursor = crHandPoint
    Caption = #1053#1072#1095#1072#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 11
    OnClick = bFormattingClick
  end
  object StandardFormatting: TCheckBox
    Left = 1112
    Top = 113
    Width = 225
    Height = 17
    Cursor = crHandPoint
    Caption = #1057#1090#1072#1085#1076#1072#1088#1090#1085#1086#1077' '#1092#1086#1088#1084#1072#1090#1080#1088#1086#1074#1072#1085#1080#1077
    TabOrder = 12
  end
  object LabeledEditSearch: TLabeledEdit
    Left = 1112
    Top = 208
    Width = 153
    Height = 24
    Cursor = crIBeam
    EditLabel.Width = 114
    EditLabel.Height = 20
    EditLabel.Caption = #1055#1086#1080#1089#1082' '#1092#1072#1081#1083#1072
    EditLabel.Font.Charset = DEFAULT_CHARSET
    EditLabel.Font.Color = clWindowText
    EditLabel.Font.Height = -16
    EditLabel.Font.Name = 'MS Sans Serif'
    EditLabel.Font.Style = [fsBold]
    EditLabel.ParentFont = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 13
  end
  object bSearch: TButton
    Left = 1112
    Top = 238
    Width = 177
    Height = 25
    Cursor = crHandPoint
    Caption = #1055#1086#1080#1089#1082
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 14
    OnClick = bSearchClick
  end
end
