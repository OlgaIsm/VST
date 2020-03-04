object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Check your history'
  ClientHeight = 464
  ClientWidth = 840
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Information: TLabel
    Left = 16
    Top = 400
    Width = 66
    Height = 16
    Caption = 'Information'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Status: TLabel
    Left = 120
    Top = 400
    Width = 36
    Height = 16
    Caption = 'Status'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object VirtualStringTree: TVirtualStringTree
    Left = 8
    Top = 8
    Width = 824
    Height = 369
    Header.AutoSizeIndex = 0
    Header.Options = [hoColumnResize, hoDblClickResize, hoDrag, hoShowSortGlyphs, hoVisible, hoAutoSpring]
    TabOrder = 0
    TreeOptions.PaintOptions = [toShowButtons, toShowDropmark, toShowRoot, toShowTreeLines, toThemeAware, toUseBlendedImages, toFullVertGridLines]
    TreeOptions.SelectionOptions = [toFullRowSelect, toMultiSelect]
    OnAddToSelection = VirtualStringTreeAddToSelection
    OnGetText = VirtualStringTreeGetText
    Columns = <
      item
        Position = 0
        Text = 'ID'
        Width = 60
      end
      item
        Position = 1
        Text = 'URL'
        Width = 348
      end
      item
        Position = 2
        Text = 'Information'
        Width = 400
      end>
  end
  object Button1: TButton
    Left = 216
    Top = 431
    Width = 145
    Height = 25
    Caption = 'Extract'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 375
    Top = 431
    Width = 145
    Height = 25
    Caption = 'Remove all'
    TabOrder = 2
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 534
    Top = 431
    Width = 145
    Height = 25
    Caption = 'Remove Node'
    TabOrder = 3
    OnClick = Button3Click
  end
end
