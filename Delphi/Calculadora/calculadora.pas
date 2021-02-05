unit calculadora;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, cxGraphics, cxControls, cxLookAndFeels,
  cxLookAndFeelPainters, cxContainer, cxEdit, Vcl.StdCtrls, cxTextEdit;

type
  TForm1 = class(TForm)
    Edit_Num1: TcxTextEdit;
    Edit_Num2: TcxTextEdit;
    Button_Soma: TButton;
    Button_Div: TButton;
    Button_Mult: TButton;
    Button_Sub: TButton;
    procedure Button_SomaClick(Sender: TObject);
  private
    { Private declarations }
    function soma(valor1, valor2 :integer ) : integer;
  public
    { Public declarations }

  end;

var
  Form1: TForm1;

implementation

{$R *.dfm}


procedure TForm1.Button_SomaClick(Sender: TObject);
      var
        saida:integer;
    begin
      saida := soma(StrToInt(Edit_Num1.Text), StrToInt(Edit_Num2.Text));
      ShowMessage('Resultado:' + IntToStr(saida));
    end;


function TForm1.soma(valor1: Integer; valor2: Integer) :integer;

   begin

      Result := valor1 + valor2;

   end;

end.
