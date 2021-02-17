program contatos;

uses
  Vcl.Forms,
  U_cadastro in '..\Forms\U_cadastro.pas' {Form1},
  U_pesquisa in '..\Forms\U_pesquisa.pas' {Form_pesquisa};

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TForm1, Form1);
  Application.CreateForm(TForm_pesquisa, Form_pesquisa);
  Application.Run;
end.
