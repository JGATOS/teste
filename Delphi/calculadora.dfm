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
    Left = 120
    Top = 48
    TabOrder = 0
    Width = 121
  end
  object Edit_Resultado: TcxTextEdit
    Left = 240
    Top = 96
    TabOrder = 1
    Width = 121
  end
  object Edit_Num2: TcxTextEdit
    Left = 352
    Top = 48
    TabOrder = 2
    Width = 121
  end
  object Button_Soma: TButton
    Left = 128
    Top = 168
    Width = 75
    Height = 25
    Caption = '+'
    TabOrder = 3
    OnClick = Button_SomaClick
  end
  object Button_Sub: TButton
    Left = 128
    Top = 224
    Width = 75
    Height = 25
    Caption = '-'
    TabOrder = 4
  end
  object Button_Mult: TButton
    Left = 240
    Top = 168
    Width = 75
    Height = 25
    Caption = '*'
    TabOrder = 5
  end
  object Button_Div: TButton
    Left = 240
    Top = 224
    Width = 75
    Height = 25
    Caption = '/'
    TabOrder = 6
  end
end
