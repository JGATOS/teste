unit calculadora;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, cxGraphics, cxControls, cxLookAndFeels,
  cxLookAndFeelPainters, cxContainer, cxEdit, Vcl.StdCtrls, cxTextEdit, IdStrings;

type
  TForm1 = class(TForm)
    Edit_Num1: TcxTextEdit;
    Button_Soma: TButton;
    Button_Div: TButton;
    Button_Mult: TButton;
    Button_Sub: TButton;
    Button_1: TButton;
    Button_2: TButton;
    Button2: TButton;
    Button3: TButton;
    Button4: TButton;
    Button5: TButton;
    Button6: TButton;
    Button7: TButton;
    Button8: TButton;
    Button9: TButton;
    Button_Igual: TButton;
    procedure Button_SomaClick(Sender: TObject);
    procedure Button_1Click(Sender: TObject);
    procedure Button_2Click(Sender: TObject);
    procedure Button_IgualClick(Sender: TObject);
  private
    { Private declarations }
    function soma(valor1, valor2 :integer ) : integer;
  public
    { Public declarations }

  end;

var
  Form1: TForm1;
  entrada : string;

implementation

{$R *.dfm}


procedure TForm1.Button_1Click(Sender: TObject);
begin
    Edit_Num1.Text := '1';
    entrada := + '1';

end;

procedure TForm1.Button_2Click(Sender: TObject);
begin
    Edit_Num1.Text :=  '2';
    entrada := + '2';
end;

procedure TForm1.Button_IgualClick(Sender: TObject);
var

        x : integer;
        y : integer;
        cavalo : TStringList;

    begin


      entrada := Edit_Num1.Text;

      cavalo :=    TStringlist.Create;
      cavalo.Delimiter:= '+';
      cavalo.DelimitedText := entrada;
      x := StrToInt(cavalo[0]);
      y := StrToInt(cavalo[1]);
      x := x + y;


      ShowMessage('Resultado' + IntToStr(x));



    end;

procedure TForm1.Button_SomaClick(Sender: TObject);

begin;
    Edit_Num1.Text :=  '+';
    entrada := + '+';
end;

 

function TForm1.soma(valor1: Integer; valor2: Integer) :integer;

   begin

      Result := valor1 + valor2;

   end;

end.
