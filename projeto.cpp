#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  int id;
  int idade; 
  char nometutor[30];
  char nomeanimal[20];
  char raca[30];
  char telefone[20];
  char status;
  char endereco[30];
} reg;

int tamanho(FILE *);
void cadastrar(FILE *);
void consultar(FILE *);
void consultar_nomeanimal(FILE *);
void consultar_codigo(FILE *);
void gerararqtxt(FILE *);
void excluir(FILE *);
void excluir_codigo(FILE *);
void excluir_nomeanimal(FILE *);

//================================================================================//
int main() {
  setlocale(LC_ALL, "Portuguese");
  int op;
  FILE *arquivo;

  if ((arquivo = fopen("arquivo.dat", "rb+")) == NULL) {
    if ((arquivo = fopen("arquivo.dat", "wb+")) == NULL) {
      printf("Erro ao abrir o arquivo");
      exit(1);
    }
  }

  do {
    printf("\n========= REGISTRO =========== \n");
    printf("1.Cadastrar animal\n");
    printf("2.Consultar animal\n");
    printf("3.Gerar arquivo\n");
    printf("4.Excluir cadastro\n");
    printf("5.Sair\n");
    printf("======= Animais Cadastrados: %d =\n", tamanho(arquivo));
    printf("Opcao:");
    scanf("%d", &op);
    switch (op) {
      case 1:
        cadastrar(arquivo);
        break;
      case 2:
        consultar(arquivo);
        break;
      case 3:
        gerararqtxt(arquivo);
        break;
      case 4:
        excluir(arquivo);
        break;
    }
  } while (op != 5);

  printf("\n==Programa finalizado com sucesso.==\n");
  sleep(3);
  system("clear");

  fclose(arquivo);

  return 0;
}

//================================================================================//

int tamanho(FILE *arquivo) {
  fseek(arquivo, 0, SEEK_END);
  return ftell(arquivo) / sizeof(reg);
}

void cadastrar(FILE *arquivo) {
  reg contato;
  char confirma;
  contato.status = 'a';

  printf("\n");
  printf("Novo cadastro:\n");
  contato.id = tamanho(arquivo) + 1;  // Atribui o próximo número de cadastro
  printf("\nNumero de cadastro:%d\n ", contato.id);
  printf("\n");
  printf("Nome do tutor...........:");
  scanf(" %[^\n]", contato.nometutor);
  printf("Nome do animal...........:");
  scanf(" %[^\n]", contato.nomeanimal);
  printf("Raça:.......:");
  scanf(" %[^\n]", contato.raca);
  printf("Telefone de contato:.......:");
  scanf(" %[^\n]", contato.telefone);
  printf("Idade do animal:...........:");
  scanf("%d", &contato.idade);
  printf("Endereço:.......:");
  scanf(" %[^\n]", contato.endereco);
  contato.status = 'a';
  printf("\n");
  printf("\nConfirmar cadastro?(s/n)");
  scanf(" %c", &confirma);
  if (confirma == 's' || confirma == 'S') {
    printf("\nCadastro confirmado!\n");
    fseek(arquivo, 0, SEEK_END);
    fwrite(&contato, sizeof(reg), 1, arquivo);
  } else {
    printf("\nCadastro cancelado.\n");
  }
}

void consultar(FILE *arquivo) {
  reg contato;
  char opcao;

  printf("\n==Consulta de animais cadastrados.==\n");
  do {
    getchar();
    printf("\n\nDejesa consultar por:");
    printf("\n1.Código");
    printf("\n2.Nome do Animal");
    printf("\n3.Voltar");
    printf("\nOpção:");
    scanf("%c", &opcao);
    getchar();
    switch (opcao) {
      case '1':
        consultar_codigo(arquivo);
        break;
      case '2':
        consultar_nomeanimal(arquivo);
        break;
    }
  } while (opcao != '3');
}

void consultar_codigo(FILE *arquivo) {
  reg contato;
  int codigo;
  int id_consultar;
  printf("\nDigite o código do cadastro: ");
  scanf("%d", &id_consultar);
  if ((id_consultar <= tamanho(arquivo)) && (id_consultar > 0)) {
    fseek(arquivo, (id_consultar - 1) * sizeof(reg), SEEK_SET);
    fread(&contato, sizeof(reg), 1, arquivo);
    if (contato.status == 'a') {
      printf("\nCódigo:...............: %d\n", contato.id);
      printf("\nNome do Tutor:........: %s", contato.nometutor);
      printf("\nNome do animal:.......: %s", contato.nomeanimal);
      printf("\nTelefone :............: (35) %s", contato.telefone);
      printf("\nIdade:................: %.2d", contato.idade);
      printf("\nRaça do Cachorrro:....: %s", contato.raca);
      printf("\nEndereço:.............: %s\n", contato.endereco);
    } else
      printf("\nCadastro inativo.\n");
  } else
    printf("\nCadastro não encontrado;");
}

void consultar_nomeanimal(FILE *arquivo) {
  reg contato;
  char nome[20];
  printf("\nDigite o nome do animal:");
  scanf("%[^\n]", nome);
  nome[strcspn(nome, "\n")] = '\0';
  rewind(arquivo);
  while (fread(&contato, sizeof(reg), 1, arquivo) == 1) {
    if (strcmp(nome, contato.nomeanimal) == 0) {
      printf("\nCódigo:...............: %d\n", contato.id);
      printf("\nNome do Tutor:........: %s", contato.nometutor);
      printf("\nNome do animal:.......: %s", contato.nomeanimal);
      printf("\nTelefone :............: (35) %s", contato.telefone);
      printf("\nIdade:................: %.2d", contato.idade);
      printf("\nRaça do Cachorrro:....: %s", contato.raca);
      printf("\nEndereço:.............: %s\n", contato.endereco);
    }
  }
}

void gerararqtxt(FILE *arquivo) {
  FILE *arquivo_txt;
  reg contato;

  if ((arquivo_txt = fopen("arquivo.txt", "w")) == NULL) {
    printf("Erro ao abrir arquivo texto");
    exit(1);
  }
  rewind(arquivo);

  fprintf(arquivo_txt, "==========================\n");
  fprintf(arquivo_txt, "Animais Cadastrados\n");
  fprintf(arquivo_txt, "==========================\n");

  while (fread(&contato, sizeof(reg), 1, arquivo) == 1) {
    if (contato.status == 'a') {
      fprintf(arquivo_txt, "\nCódigo:...............: %d\n", contato.id);
      fprintf(arquivo_txt, "\nNome do Tutor:........: %s", contato.nometutor);
      fprintf(arquivo_txt, "\nNome do animal:.......: %s", contato.nomeanimal);
      fprintf(arquivo_txt, "\nTelefone :............: (35) %s", contato.telefone);
      fprintf(arquivo_txt, "\nIdade:................: %.2d", contato.idade);
      fprintf(arquivo_txt, "\nRaça do Cachorrro:....: %s", contato.raca);
      fprintf(arquivo_txt, "\nEndereço:.............: %s", contato.endereco);
      fprintf(arquivo_txt, "\nStatus:...............: Ativo\n");
      fprintf(arquivo_txt, "==========================\n");
    }
  }

  printf("\nArquivo gerado com sucesso!\n");
  fclose(arquivo_txt);
}


void excluir(FILE *arquivo) {
  reg contato;
  char opcao;

  printf("\n==Exclusao de animais cadastrados.==\n");
  do {
    getchar();
    printf("\nDejesa excluir por:");
    printf("\n1.Código");
    printf("\n2.Nome do Animal");
    printf("\n3.Voltar");
    printf("\nOpção:");
    scanf("%c", &opcao);
    getchar();
    switch (opcao) {
      case '1':
        excluir_codigo(arquivo);
        break;
      case '2':
        excluir_nomeanimal(arquivo);
        break;
    }
  } while (opcao != '3');
}

void excluir_codigo(FILE *arquivo) {
  reg contato;
  int codigo;
  int id_excluir;

  printf("\nDigite o código do cadastro: ");
  scanf("%d", &id_excluir);

  if ((id_excluir <= tamanho(arquivo)) && (id_excluir > 0)) {
    fseek(arquivo, (id_excluir - 1) * sizeof(reg), SEEK_SET);
    fread(&contato, sizeof(reg), 1, arquivo);
    if (contato.status == 'a') {
      printf("\nCódigo:...............: %d\n", contato.id);
      printf("\nNome do Tutor:........: %s", contato.nometutor);
      printf("\nNome do animal:.......: %s", contato.nomeanimal);
      printf("\nTelefone :............: (35) %s", contato.telefone);
      printf("\nIdade:................: %.2d", contato.idade);
      printf("\nRaça do Cachorrro:....: %s", contato.raca);
      printf("\nEndereço:.............: %s", contato.endereco);
      printf("\n\nConfirmar exclusão?(s/n)");
      scanf(" %c", &contato.status);
      if ((contato.status == 's') || (contato.status == 'S')) {
        fseek(arquivo, (id_excluir - 1) * sizeof(reg), SEEK_SET);
        contato.status = 'i';
        fwrite(&contato, sizeof(reg), 1, arquivo);
        printf("\nCadastro excluido.\n");
      } else
        printf("\nExclusão cancelada.\n");
    } else
      printf("\nCadastro inativo.\n");
  } else
    printf("\nCadastro não encontrado.\n");
}

void excluir_nomeanimal(FILE *arquivo) {
  reg contato;
  char nome[20];

  printf("\nDigite o nome do animal: ");
  scanf("%[^\n]", nome);
  nome[strcspn(nome, "\n")] = '\0';

  rewind(arquivo);
  while (fread(&contato, sizeof(reg), 1, arquivo) == 1) {
    if (strcmp(nome, contato.nomeanimal) == 0) {
      printf("\nCódigo:...............: %d\n", contato.id);
      printf("\nNome do Tutor:........: %s", contato.nometutor);
      printf("\nNome do animal:.......: %s", contato.nomeanimal);
      printf("\nTelefone :............: (35) %s", contato.telefone);
      printf("\nIdade:................: %.2d", contato.idade);
      printf("\nRaça do Cachorrro:....: %s", contato.raca);
      printf("\nEndereço:.............: %s", contato.endereco);
      printf("\n\nConfirmar exclusão?(s/n)");
      scanf(" %c", &contato.status);
      if ((contato.status == 's') || (contato.status == 'S')) {
        fseek(arquivo, -sizeof(reg), SEEK_CUR);
        contato.status = 'i';
        fwrite(&contato, sizeof(reg), 1, arquivo);
        printf("\nCadastro excluido.\n");
      } else
        printf("\nExclusão cancelada.\n");
    }
  }
}
