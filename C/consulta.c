#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

bool login(char usuario, char senha){
	char arquivo[200];
	int strSize = strlen(arquivo);

	FILE *file ;
    file = fopen(usuario, "r");

    if(file == NULL){
     printf("Usuario nao encontrado\n"); 
    } else {
    while (fgets(arquivo, 200, file) != NULL){
        char delimitador[] = ",";

         // Usando strtok para dividir a string em partes
        char *token = strtok(arquivo, delimitador);
        char *parte1 = token;
        char *aux1 = usuario ;
		
		token = strtok(NULL, delimitador);
        char *parte2 = token;
        char *aux2 = senha;
		
		if ((strcmp(aux1, usuario) == 0) && (strcmp(aux2, senha) == 0)){
		 return true;
		}else {
		 return false;  
		}
	 	 
 }	
	}
    
}

int criaUsuario(char usuario, char senha){
	char arquivo[40];
	
	FILE *file ;
    
   if(fopen(usuario, "r") == NULL){
     printf("Usuario nao encontrado\n"); 
   } else{
	
    file = fopen(arquivo, "w");                                                     //cria o arquivo
    fprintf(file, usuario);                                                             //salva o valor de cpf
    fclose(file);	
    
    file = fopen(arquivo, "a");                                                     //abre o arquivo em modo escritra
    fprintf(file, ",");                                                             //escreve o separador entre os valores das variaveis
    fclose(file);
    
    file = fopen(arquivo, "w");                                                     //cria o arquivo
    fprintf(file, senha);                                                             //salva o valor de cpf
    fclose(file);
    }
    
    login(usuario,senha);
}

int incluir(){
    char arquivo[40];
    char cpf[40];
    char nome[40];
    char sobrenome[40];
    char cargo[40];

                                                              //copia o valor do segundo parametro para o primeiro

    printf("digite o CPF a ser incluido: ");
    scanf("%s", cpf);

    strcpy(arquivo, cpf);

    FILE *file ;
    file = fopen(arquivo, "w");                                                     //cria o arquivo
    fprintf(file, cpf);                                                             //salva o valor de cpf
    fclose(file);                                                                   //fecha o arquivo

    file = fopen(arquivo, "a");                                                     //abre o arquivo em modo escritra
    fprintf(file, ",");                                                             //escreve o separador entre os valores das variaveis
    fclose(file);                                                                   //fecha o arquivo

    printf("digite o nome a ser incluido: ");
    scanf("%s", nome);

    file = fopen(arquivo, "a");                                                     //abre o arquivo em modo escritra
    fprintf(file, nome);                                                            //salva o valor do nome
    fclose(file);                                                                   //fecha o arquivo

    file = fopen(arquivo, "a");                                                     //abre o arquivo em modo escritra
    fprintf(file, ",");                                                             //escreve o separador entre os valores das variaveis
    fclose(file);                                                                   //fecha o arquivo

    printf("digite o sobrenome a ser incluido: ");
    scanf("%s", sobrenome);

    file = fopen(arquivo, "a");                                                     //abre o arquivo em modo escritra
    fprintf(file, sobrenome);                                                       //salva o valor do nome
    fclose(file);                                                                   //fecha o arquivo

    file = fopen(arquivo, "a");                                                     //abre o arquivo em modo escritra
    fprintf(file, ",");                                                             //escreve o separador entre os valores das variaveis
    fclose(file);                                                                   //fecha o arquivo

    printf("digite o cargo a ser incluido: ");
    scanf("%s", cargo);

    file = fopen(arquivo, "a");                                                     //abre o arquivo em modo escritra
    fprintf(file, cargo);                                                           //salva o valor do nome
    fclose(file);                                                                   //fecha o arquivo

}

int consulta(){
    char arquivo[200];
    char cpf[40];
    int strSize = strlen(arquivo);

    printf("digite o CPF para consulta: ");
    scanf("%s", cpf);

    FILE *file ;
    file = fopen(cpf, "r");

    if(file == NULL){
        printf("arquivo nao encontrado\n");
    }


    // fgets(arquivo, 200, file);

    while (fgets(arquivo, 200, file) != NULL){
        char delimitador[] = ",";

         // Usando strtok para dividir a string em partes
        char *token = strtok(arquivo, delimitador);
        char *parte1 = token; // Primeiro pedaço

        token = strtok(NULL, delimitador);
        char *parte2 = token; // Segundo pedaço

        token = strtok(NULL, delimitador);
        char *parte3 = token; // Terceiro pedaço

        token = strtok(NULL, delimitador);
        char *parte4 = token; // Quarto pedaço

        // Imprimindo as partes
        printf("Cpf: %s\n"      , parte1);
        printf("Nome: %s\n"     , parte2);
        printf("Sobrenome: %s\n", parte3);
        printf("Cargo: %s\n"    , parte4);

    }

}

int deletar(){

    char cpf[40];

    printf("Digite o cpf a ser deletado: ");
    scanf("%s", cpf);

    FILE * file;
    file = fopen(cpf,"r");

    if(file == NULL){
        printf("arquivo nao encontrado\n");
    }
    else{
       fclose(file);
       printf("arquivo deletado\n");
        remove(cpf);
    }

}


int main(){
// declaracao de variaveis
int validacao = 1;
int opcao = 0;
char usuario[40];
char senha[40];

	printf("\t 1 Login    \n\n");	
	printf("\t 2 Cadastro \n\n");
	switch (opcao)
        {
            case 1:
            printf("Digite seu usuario\n");
            scanf("%s", usuario);
            printf("Digite sua senha\n");
            scanf("%s", senha);
            login(usuario,senha);
            system("pause");
            break;
            
            case 2:
            printf("Para cadastrar: \n");
            printf("Digite seu usuario\n");
            scanf("%s", usuario);
            printf("Digite sua senha\n");
            scanf("%s", senha);
            login(usuario,senha);
            system("pause");
            break;
		}

	if (login(usuario,senha)) {
    for(validacao = 1; validacao = 1;){         //laco de repeticao para continuar executando o programa

        setlocale(LC_ALL, "Portugese");
        system("cls");                          //comnado para limpar o terninal
        printf("\t     ------CARTORIO EBAC------\n\n");
        printf("\t 1 - [    Incluir nome       ]\n\n");
        printf("\t 2 - [    Consultar nome     ]\n\n");
        printf("\t 3 - [    Deletar nome       ]\n\n");
        printf("\t 4 - [    Sair do sistema    ]\n\n");

        scanf("%d", &opcao);                    //entrada de dados do usuario
        system("cls");                          //comnado para limpar o terninal

        switch (opcao)
        {
            case 1:
            // printf("opcao 1 (incluir nome)\n");
            incluir();
            system("pause");
            break;

            case 2:
            consulta();
            system("pause");
            break;

            case 3:
            //printf("opcao 3 (deletar nome)\n");
            deletar();
            system("pause");
            break;
            
            case 4:
            //printf("opcao 3 (deletar nome)\n");
            printf("Deus te elimine");
            return 0;
            break;

            default:
            printf("opcao nao disponivel\n");
            system("pause");
            break;
        }

    }
}
}
