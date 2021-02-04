unit calculadora;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, cxGraphics, cxControls, cxLookAndFeels,
  cxLookAndFeelPainters, cxContainer, cxEdit, cxTextEdit, Vcl.StdCtrls;

type
  TForm1 = class(TForm)
    Edit_Num1: TcxTextEdit;
    Edit_Resultado: TcxTextEdit;
    Edit_Num2: TcxTextEdit;
    Button_Soma: TButton;
    Button_Sub: TButton;
    Button_Mult: TButton;
    Button_Div: TButton;
    procedure Button_SomaClick(Sender: TObject);



  private
    { Private declarations }
    function soma (valor1, valor2 :currency) :currency;
  public
    { Public declarations }
  end;

var
  Form1: TForm1;
  resultado: currency;

implementation

{$R *.dfm}

procedure TForm1.Button_SomaClick(Sender: TObject);
var
    Resultado: currency;

begin

    Resultado := soma(StrToCurr(Edit_Num1.Text), StrToCurr(Edit_Num2.Text));
    Edit_Resultado.Text := CurrToStr(Resultado);

end;

function TForm1.soma(valor1: Currency; valor2: Currency) : currency;
begin

  resultado := valor1 + valor2;

end;

end.
