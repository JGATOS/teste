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
    Left = 96
    Top = 24
    TabOrder = 0
    Width = 121
  end
  object Button_Soma: TButton
    Left = 96
    Top = 144
    Width = 75
    Height = 25
    Caption = '+'
    TabOrder = 1
    OnClick = Button_SomaClick
  end
  object Button_Div: TButton
    Left = 240
    Top = 144
    Width = 75
    Height = 25
    Caption = '/'
    TabOrder = 2
  end
  object Button_Mult: TButton
    Left = 96
    Top = 208
    Width = 75
    Height = 25
    Caption = '*'
    TabOrder = 3
  end
  object Button_Sub: TButton
    Left = 240
    Top = 208
    Width = 75
    Height = 25
    Caption = '-'
    TabOrder = 4
  end
  object Button_1: TButton
    Left = 368
    Top = 144
    Width = 75
    Height = 25
    Caption = '1'
    TabOrder = 5
    OnClick = Button_1Click
  end
  object Button_2: TButton
    Left = 384
    Top = 200
    Width = 75
    Height = 25
    Caption = '2'
    TabOrder = 6
    OnClick = Button_2Click
  end
  object Button2: TButton
    Left = 360
    Top = 80
    Width = 75
    Height = 25
    Caption = 'Button2'
    TabOrder = 7
  end
  object Button3: TButton
    Left = 496
    Top = 88
    Width = 75
    Height = 25
    Caption = 'Button3'
    TabOrder = 8
  end
  object Button4: TButton
    Left = 520
    Top = 168
    Width = 75
    Height = 25
    Caption = 'Button4'
    TabOrder = 9
  end
  object Button5: TButton
    Left = 488
    Top = 224
    Width = 75
    Height = 25
    Caption = 'Button5'
    TabOrder = 10
  end
  object Button6: TButton
    Left = 256
    Top = 56
    Width = 75
    Height = 25
    Caption = 'Button6'
    TabOrder = 11
  end
  object Button7: TButton
    Left = 144
    Top = 72
    Width = 75
    Height = 25
    Caption = 'Button7'
    TabOrder = 12
  end
  object Button8: TButton
    Left = 32
    Top = 80
    Width = 75
    Height = 25
    Caption = 'Button8'
    TabOrder = 13
  end
  object Button9: TButton
    Left = 16
    Top = 160
    Width = 75
    Height = 25
    Caption = 'Button9'
    TabOrder = 14
  end
  object Button_Igual: TButton
    Left = 128
    Top = 264
    Width = 75
    Height = 25
    Caption = '='
    TabOrder = 15
    OnClick = Button_IgualClick
  end
end