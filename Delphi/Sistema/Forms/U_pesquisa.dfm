object Form_pesquisa: TForm_pesquisa
  Left = 0
  Top = 0
  Align = alClient
  Caption = 'Pesquisa'
  ClientHeight = 456
  ClientWidth = 1000
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  WindowState = wsMaximized
  PixelsPerInch = 96
  TextHeight = 13
  object DBGrid1: TDBGrid
    Left = 0
    Top = 97
    Width = 1000
    Height = 295
    Align = alClient
    DataSource = Form1.Ds_cadastro
    Options = [dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgConfirmDelete, dgCancelOnExit, dgTitleClick, dgTitleHotTrack]
    TabOrder = 0
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'Tahoma'
    TitleFont.Style = []
    Columns = <
      item
        Expanded = False
        FieldName = 'ID_CONTATO'
        ReadOnly = True
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'NOME'
        ReadOnly = True
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'TELEFONE'
        ReadOnly = True
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'CELULAR'
        ReadOnly = True
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'EMAIL'
        ReadOnly = True
        Visible = True
      end>
  end
  object Panel1: TPanel
    Left = 0
    Top = 392
    Width = 1000
    Height = 64
    Align = alBottom
    Color = clActiveBorder
    ParentBackground = False
    TabOrder = 1
    object DBNavigator1: TDBNavigator
      Left = 248
      Top = 6
      Width = 468
      Height = 41
      DataSource = Form1.Ds_cadastro
      VisibleButtons = [nbFirst, nbPrior, nbNext, nbLast]
      TabOrder = 0
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 1000
    Height = 97
    Align = alTop
    Color = clActiveBorder
    ParentBackground = False
    TabOrder = 2
    object Text_edit_pesquisa: TEdit
      Left = 184
      Top = 19
      Width = 250
      Height = 21
      TabOrder = 0
    end
    object Btn_Pesquisa: TBitBtn
      Left = 184
      Top = 51
      Width = 90
      Height = 40
      Caption = 'Pesquisar'
      TabOrder = 1
      OnClick = Btn_PesquisaClick
    end
    object BitBtn1: TBitBtn
      Left = 616
      Top = 19
      Width = 90
      Height = 40
      TabOrder = 2
    end
    object Rad_Grupo: TRadioGroup
      Left = 11
      Top = 9
      Width = 144
      Height = 72
      Caption = 'Op'#231#245'es'
      Color = clSilver
      ItemIndex = 0
      Items.Strings = (
        'C'#243'digo'
        'Nome')
      ParentBackground = False
      ParentColor = False
      TabOrder = 3
    end
  end
end