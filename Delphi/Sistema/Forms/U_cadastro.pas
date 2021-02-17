unit U_cadastro;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, FireDAC.Stan.Intf, FireDAC.Stan.Option,
  FireDAC.Stan.Error, FireDAC.UI.Intf, FireDAC.Phys.Intf, FireDAC.Stan.Def,
  FireDAC.Stan.Pool, FireDAC.Stan.Async, FireDAC.Phys, FireDAC.Phys.FB,
  FireDAC.Phys.FBDef, FireDAC.VCLUI.Wait, FireDAC.Stan.Param, FireDAC.DatS,
  FireDAC.DApt.Intf, FireDAC.DApt, Vcl.ExtCtrls, Vcl.StdCtrls, Vcl.Mask,
  Vcl.DBCtrls, Data.DB, FireDAC.Comp.DataSet, FireDAC.Comp.Client, Vcl.Buttons,
  Datasnap.DBClient, Datasnap.Provider;

type
    TForm1 = class(TForm)
    Conexao: TFDConnection;
    Q_cadastro: TFDQuery;
    Ds_cadastro: TDataSource;
    Q_cadastroID_CONTATO: TIntegerField;
    Q_cadastroNOME: TStringField;
    Q_cadastroTELEFONE: TStringField;
    Q_cadastroCELULAR: TStringField;
    Q_cadastroEMAIL: TStringField;
    Label1: TLabel;
    DBEdit1: TDBEdit;
    Label2: TLabel;
    DB_nome: TDBEdit;
    Label3: TLabel;
    DBEdit3: TDBEdit;
    Label4: TLabel;
    DBEdit4: TDBEdit;
    Label5: TLabel;
    DBEdit5: TDBEdit;
    Panel1: TPanel;
    Panel2: TPanel;
    Btn_Novo: TBitBtn;
    Btn_Editar: TBitBtn;
    Btn_Deletar: TBitBtn;
    Btn_Gravar: TBitBtn;
    Btn_Atualizar: TBitBtn;
    Btn_Cancelar: TBitBtn;
    DBNavigator1: TDBNavigator;
    Btn_Pesquisa: TBitBtn;
    dspcadastro: TDataSetProvider;
    cdscadastro: TClientDataSet;
    cdscadastroID_CONTATO: TIntegerField;
    cdscadastroNOME: TStringField;
    cdscadastroTELEFONE: TStringField;
    cdscadastroCELULAR: TStringField;
    cdscadastroEMAIL: TStringField;
    procedure Btn_NovoClick(Sender: TObject);
    procedure Btn_EditarClick(Sender: TObject);
    procedure Btn_DeletarClick(Sender: TObject);
    procedure Btn_GravarClick(Sender: TObject);
    procedure Btn_CancelarClick(Sender: TObject);
    procedure Btn_AtualizarClick(Sender: TObject);
    procedure Btn_PesquisaClick(Sender: TObject);
    procedure cdscadastroAfterPost(DataSet: TDataSet);
    procedure cdscadastroNewRecord(DataSet: TDataSet);
    procedure cdscadastroAfterScroll(DataSet: TDataSet);
    procedure FormShow(Sender: TObject);

  private
    function ProximoCodigo: Integer;
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.dfm}

uses U_pesquisa;

procedure TForm1.Btn_AtualizarClick(Sender: TObject);
begin
  cdscadastro.Refresh;
end;

procedure TForm1.Btn_CancelarClick(Sender: TObject);
begin
  cdscadastro.Cancel;
end;

procedure TForm1.Btn_DeletarClick(Sender: TObject);
begin
  if (cdscadastro.RecordCount > 0) and (messagedlg('Deseja deletar esse registro?', mtConfirmation, [mbOk, mbNo], 0) = mrOk) then
   cdscadastro.Delete;
end;

procedure TForm1.Btn_EditarClick(Sender: TObject);
begin
  if messagedlg('Deseja alterar esse registro?', mtConfirmation, [mbOk, mbNo], 0) = mrOk then
   cdscadastro.Edit;
end;
procedure TForm1.Btn_GravarClick(Sender: TObject);
begin
  cdscadastro.Post;
end;

procedure TForm1.Btn_NovoClick(Sender: TObject);
begin
  cdscadastro.Append;
  DB_nome.SetFocus;
end;

procedure TForm1.Btn_PesquisaClick(Sender: TObject);
begin
  Form_pesquisa := TForm_pesquisa.Create(self);

  try
   Form_pesquisa.ShowModal;
  finally
   if Assigned(Form_pesquisa) then
    FreeAndNil(Form_pesquisa);
  end;
end;

procedure TForm1.cdscadastroAfterPost(DataSet: TDataSet);
begin
  cdscadastro.ApplyUpdates(0);
end;

procedure TForm1.cdscadastroAfterScroll(DataSet: TDataSet);
begin
  Btn_Editar.Enabled := (cdscadastro.RecordCount > 0);
  Btn_Deletar.Enabled := Btn_Editar.Enabled;

  Btn_Gravar.Enabled := Ds_cadastro.DataSet.State in [dsInsert, dsEdit];
  Btn_Cancelar.Enabled := Btn_Gravar.Enabled;
end;

procedure TForm1.cdscadastroNewRecord(DataSet: TDataSet);
begin
  cdscadastroID_CONTATO.AsInteger := ProximoCodigo;
end;

procedure TForm1.FormShow(Sender: TObject);
begin
  cdscadastro.First;
end;

function TForm1.ProximoCodigo: Integer;
var
  qry: TFDQuery;
begin
  Result := 0;
  qry := TFDQuery.Create(Self);

  try
   qry.Connection := Conexao;
   qry.SQL.Add('SELECT MAX(ID_CONTATO) from CONTATO');
   qry.Open;

   Result := qry.Fields[0].AsInteger + 1;
  finally
   if Assigned(qry) then
    qry.Free;
  end;
end;

end.