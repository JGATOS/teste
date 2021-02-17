object Form1: TForm1
  Left = 0
  Top = 0
  Align = alCustom
  ClientHeight = 415
  ClientWidth = 686
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 78
    Top = 88
    Width = 66
    Height = 13
    Caption = 'ID_CONTATO'
    FocusControl = DBEdit1
  end
  object Label2: TLabel
    Left = 78
    Top = 130
    Width = 29
    Height = 13
    Caption = 'NOME'
    FocusControl = DB_nome
  end
  object Label3: TLabel
    Left = 78
    Top = 184
    Width = 50
    Height = 13
    Caption = 'TELEFONE'
    FocusControl = DBEdit3
  end
  object Label4: TLabel
    Left = 326
    Top = 181
    Width = 44
    Height = 13
    Caption = 'CELULAR'
    FocusControl = DBEdit4
  end
  object Label5: TLabel
    Left = 78
    Top = 232
    Width = 30
    Height = 13
    Caption = 'EMAIL'
    FocusControl = DBEdit5
  end
  object DBEdit1: TDBEdit
    Left = 78
    Top = 101
    Width = 134
    Height = 21
    DataField = 'ID_CONTATO'
    DataSource = Ds_cadastro
    TabOrder = 0
  end
  object DB_nome: TDBEdit
    Left = 78
    Top = 149
    Width = 500
    Height = 21
    DataField = 'NOME'
    DataSource = Ds_cadastro
    TabOrder = 1
  end
  object DBEdit3: TDBEdit
    Left = 78
    Top = 200
    Width = 199
    Height = 21
    DataField = 'TELEFONE'
    DataSource = Ds_cadastro
    TabOrder = 2
  end
  object DBEdit4: TDBEdit
    Left = 326
    Top = 200
    Width = 199
    Height = 21
    DataField = 'CELULAR'
    DataSource = Ds_cadastro
    TabOrder = 3
  end
  object DBEdit5: TDBEdit
    Left = 78
    Top = 248
    Width = 500
    Height = 21
    DataField = 'EMAIL'
    DataSource = Ds_cadastro
    TabOrder = 4
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 686
    Height = 73
    Align = alTop
    Color = clGradientActiveCaption
    ParentBackground = False
    TabOrder = 5
    object Btn_Novo: TBitBtn
      Left = 32
      Top = 18
      Width = 90
      Height = 40
      Caption = '&Novo'
      TabOrder = 0
      OnClick = Btn_NovoClick
    end
    object Btn_Editar: TBitBtn
      Left = 136
      Top = 18
      Width = 90
      Height = 40
      Caption = '&Editar'
      TabOrder = 1
      OnClick = Btn_EditarClick
    end
    object Btn_Deletar: TBitBtn
      Left = 240
      Top = 18
      Width = 90
      Height = 40
      Caption = '&Deletar'
      TabOrder = 2
      OnClick = Btn_DeletarClick
    end
    object Btn_Gravar: TBitBtn
      Left = 344
      Top = 18
      Width = 90
      Height = 40
      Caption = '&Gravar'
      TabOrder = 3
      OnClick = Btn_GravarClick
    end
    object Btn_Atualizar: TBitBtn
      Left = 455
      Top = 18
      Width = 90
      Height = 40
      Caption = '&Atualizar'
      TabOrder = 4
      OnClick = Btn_AtualizarClick
    end
    object Btn_Cancelar: TBitBtn
      Left = 559
      Top = 18
      Width = 90
      Height = 40
      Caption = '&Cancelar'
      TabOrder = 5
      OnClick = Btn_CancelarClick
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 342
    Width = 686
    Height = 73
    Align = alBottom
    Color = clGradientActiveCaption
    ParentBackground = False
    TabOrder = 6
    object DBNavigator1: TDBNavigator
      Left = 16
      Top = 16
      Width = 528
      Height = 41
      DataSource = Ds_cadastro
      VisibleButtons = [nbFirst, nbPrior, nbNext, nbLast]
      TabOrder = 0
    end
    object Btn_Pesquisa: TBitBtn
      Left = 568
      Top = 16
      Width = 90
      Height = 40
      Caption = '&Pesquisar'
      TabOrder = 1
      OnClick = Btn_PesquisaClick
    end
  end
  object Conexao: TFDConnection
    Params.Strings = (
      'Database=D:\Nova pasta\Sistema\Exe\Banco\CONTATO.FDB'
      'User_Name=SYSDBA'
      'Password=masterkey'
      'CharacterSet=WIN1252'
      'Protocol=TCPIP'
      'Server=localhost'
      'Port=3051'
      'DriverID=FB')
    Connected = True
    Left = 358
    Top = 296
  end
  object Q_cadastro: TFDQuery
    Connection = Conexao
    SQL.Strings = (
      'select '
      '* '
      'from CONTATO'
      'ORDER BY ID_CONTATO')
    Left = 38
    Top = 288
    object Q_cadastroID_CONTATO: TIntegerField
      FieldName = 'ID_CONTATO'
      Origin = 'ID_CONTATO'
      ProviderFlags = [pfInUpdate, pfInWhere, pfInKey]
      Required = True
    end
    object Q_cadastroNOME: TStringField
      FieldName = 'NOME'
      Origin = 'NOME'
      Required = True
      Size = 100
    end
    object Q_cadastroTELEFONE: TStringField
      FieldName = 'TELEFONE'
      Origin = 'TELEFONE'
      EditMask = '!\(99\)0000-0000;0;_'
      Size = 15
    end
    object Q_cadastroCELULAR: TStringField
      FieldName = 'CELULAR'
      Origin = 'CELULAR'
      EditMask = '!\(99\)00000-0000;0;_'
      Size = 15
    end
    object Q_cadastroEMAIL: TStringField
      FieldName = 'EMAIL'
      Origin = 'EMAIL'
      Required = True
      Size = 100
    end
  end
  object Ds_cadastro: TDataSource
    AutoEdit = False
    DataSet = cdscadastro
    Left = 254
    Top = 296
  end
  object dspcadastro: TDataSetProvider
    DataSet = Q_cadastro
    Left = 104
    Top = 288
  end
  object cdscadastro: TClientDataSet
    Active = True
    Aggregates = <>
    FieldDefs = <
      item
        Name = 'ID_CONTATO'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'NOME'
        Attributes = [faRequired]
        DataType = ftString
        Size = 100
      end
      item
        Name = 'TELEFONE'
        DataType = ftString
        Size = 15
      end
      item
        Name = 'CELULAR'
        DataType = ftString
        Size = 15
      end
      item
        Name = 'EMAIL'
        Attributes = [faRequired]
        DataType = ftString
        Size = 100
      end>
    IndexDefs = <
      item
        Name = 'cdscadastroIndexCodigo'
        Fields = 'ID_CONTATO'
        Options = [ixPrimary, ixUnique]
      end
      item
        Name = 'cdscadastroIndexNome'
        Fields = 'NOME'
      end>
    IndexName = 'cdscadastroIndexCodigo'
    Params = <>
    ProviderName = 'dspcadastro'
    StoreDefs = True
    AfterPost = cdscadastroAfterPost
    AfterDelete = cdscadastroAfterPost
    AfterScroll = cdscadastroAfterScroll
    OnNewRecord = cdscadastroNewRecord
    Left = 176
    Top = 288
    object cdscadastroID_CONTATO: TIntegerField
      FieldName = 'ID_CONTATO'
      Origin = 'ID_CONTATO'
      ProviderFlags = [pfInUpdate, pfInWhere, pfInKey]
      Required = True
    end
    object cdscadastroNOME: TStringField
      FieldName = 'NOME'
      Origin = 'NOME'
      Required = True
      Size = 100
    end
    object cdscadastroTELEFONE: TStringField
      FieldName = 'TELEFONE'
      Origin = 'TELEFONE'
      Size = 15
    end
    object cdscadastroCELULAR: TStringField
      FieldName = 'CELULAR'
      Origin = 'CELULAR'
      Size = 15
    end
    object cdscadastroEMAIL: TStringField
      FieldName = 'EMAIL'
      Origin = 'EMAIL'
      Required = True
      Size = 100
    end
  end
end