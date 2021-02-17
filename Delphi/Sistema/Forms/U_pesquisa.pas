unit U_pesquisa;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Data.DB, Vcl.Grids, Vcl.DBGrids,
  Vcl.DBCtrls, Vcl.ExtCtrls, Vcl.StdCtrls, Vcl.Buttons;

type
  TForm_pesquisa = class(TForm)
    DBGrid1: TDBGrid;
    Panel1: TPanel;
    Panel2: TPanel;
    DBNavigator1: TDBNavigator;
    Text_edit_pesquisa: TEdit;
    Btn_Pesquisa: TBitBtn;
    BitBtn1: TBitBtn;
    Rad_Grupo: TRadioGroup;
    procedure Btn_PesquisaClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form_pesquisa: TForm_pesquisa;

implementation

{$R *.dfm}

uses U_cadastro;

procedure TForm_pesquisa.Btn_PesquisaClick(Sender: TObject);
begin
  case Rad_Grupo.ItemIndex of
   0: begin
       Form1.cdscadastro.IndexName := 'cdscadastroIndexCodigo';
       if not Form1.cdscadastro.FindKey([Text_edit_pesquisa.Text]) then
        ShowMessage('Não encontrei!');
      end;
   1: begin
       Form1.cdscadastro.IndexName := 'cdscadastroIndexNome';

       if not Form1.cdscadastro.FindKey([Text_edit_pesquisa.Text]) then
        begin
         ShowMessage('Não encontrei! Vou fazer a busca aproximada.');
         Form1.cdscadastro.FindNearest([Text_edit_pesquisa.Text]);
        end;
      end;
  end;
end;

end.
