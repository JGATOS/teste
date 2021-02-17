program SistemaFinanca;

uses
  Vcl.Forms,
  U_principal in '..\Forms\U_principal.pas' {Progama_principal};

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TProgama_principal, Progama_principal);
  Application.Run;
end.
