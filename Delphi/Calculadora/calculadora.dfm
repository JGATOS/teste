object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 299
  ClientWidth = 635
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Edit_Num1: TcxTextEdit
    Left = 144
    Top = 40
    TabOrder = 0
    Width = 121
  end
  object Edit_Num2: TcxTextEdit
    Left = 376
    Top = 40
    TabOrder = 1
    Width = 121
  end
  object Button_Soma: TButton
    Left = 96
    Top = 144
    Width = 75
    Height = 25
    Caption = '+'
    TabOrder = 2
    OnClick = Button_SomaClick
  end
  object Button_Div: TButton
    Left = 240
    Top = 144
    Width = 75
    Height = 25
    Caption = '/'
    TabOrder = 3
  end
  object Button_Mult: TButton
    Left = 96
    Top = 208
    Width = 75
    Height = 25
    Caption = '*'
    TabOrder = 4
  end
  object Button_Sub: TButton
    Left = 240
    Top = 208
    Width = 75
    Height = 25
    Caption = '-'
    TabOrder = 5
  end
end
