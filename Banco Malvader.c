#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <locale.h>

#define MAXCONTAS 3
#define MAXSENHA 20
#define MAXCARACTER 50

struct Cliente {
    char tipo_conta[20];
    char agencia[MAXCARACTER];
    char numero_conta[MAXCARACTER];
    char limite_conta[MAXCARACTER]; 
    char data_vencimento[MAXCARACTER]; 
    char nome[MAXCARACTER];
    char cpf[MAXCARACTER];
    char data_nascimento[MAXCARACTER];
    char telefone[MAXCARACTER];
    char endereco[MAXCARACTER];
    char cep[MAXCARACTER];
    char local[MAXCARACTER];
    char numero_casa[MAXCARACTER];
    char bairro[MAXCARACTER];
    char cidade[MAXCARACTER];
    char estado[MAXCARACTER];
    char senha[MAXCARACTER];
};

struct Funcionario {
    char codigo_funcionario[MAXCARACTER];
    char cargo[MAXCARACTER];
    char nome_do_funcionario[MAXCARACTER]; 
    char CPF_do_funcionario[MAXCARACTER];
    char data_de_nascimento[MAXCARACTER];
    char telefone_de_contato[MAXCARACTER];
    char endereco_do_funcionario[MAXCARACTER];
    char CEP[MAXCARACTER];
    char local[MAXCARACTER];
    char numero_da_casa[MAXCARACTER];
    char bairro[MAXCARACTER];
    char cidade[MAXCARACTER];
    char estado[MAXCARACTER];
    char senha_do_funcionario[MAXCARACTER];
};

struct Conta {
    struct Cliente cliente;
    double saldo;
};

struct Conta contas[MAXCONTAS];
int numContas = 0;

struct Funcionario funcionarios[MAXCONTAS];
int numContasFunc = 0;

void salvarContas() {
    FILE *file = fopen("Contas.dat", "wb");
    if (file != NULL) {
        fwrite(&numContas, sizeof(int), 1, file);
        fwrite(&numContasFunc, sizeof(int), 1, file);
        fwrite(contas, sizeof(struct Conta), numContas, file);
        fwrite(funcionarios, sizeof(struct Funcionario), numContasFunc, file);
        fclose(file);
    } else {
        printf("Erro ao abrir arquivo para salvar.\n");
    }
}

void carregarContas() {
    FILE *file = fopen("Contas.dat", "rb");
    if (file != NULL) {
        fread(&numContas, sizeof(int), 1, file);
        fread(&numContasFunc, sizeof(int), 1, file);
        fread(contas, sizeof(struct Conta), numContas, file);
        fread(funcionarios, sizeof(struct Funcionario), numContasFunc, file);
        fclose(file);
    } else {
        printf("Nenhum arquivo de contas encontrado. Iniciando com dados vazios.\n\n");
    }
}

int verificarSenhaCliente(char *senhaDigitada, char *senhaCorreta) {
    return strcmp(senhaDigitada, senhaCorreta) == 0;
}

int verificarSenhaAdministrador() {
    char senhaCorreta[] = "batata";
    char senhaDigitada[20];
    int i = 0;
    char ch;

    printf("DIGITE A SENHA DE ADMINISTRADOR: ");

    while (1) {
        ch = getch();

        if (ch == 13) {
            senhaDigitada[i] = '\0';
            break;
        } else if (ch == 8) {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            senhaDigitada[i++] = ch;
            printf("*");
        }
    }

    if (strcmp(senhaDigitada, senhaCorreta) == 0) {
        return 1;
    } else {
        system("cls");
        printf("Senha incorreta.\n\n");
        return 0;
    }
}

void menuCliente(struct Conta *conta) {
    int opcao;
    double valor;

    do {
        printf("Bem Vindo ao Menu Cliente!\n\n(1)Saldo\n(2)Depositar\n(3)Saque\n(4)Extrato\n(5)Consultar Limite\n(6)Sair\n\nOpção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
            	system("cls");
                printf("Saldo atual: %.2f\n\n", conta->saldo);
                break;
            case 2:
            	system("cls");
                printf("Valor para depositar: ");
                scanf("%lf", &valor);
                system("cls");
                conta->saldo += valor;
                printf("Deposito realizado.\nSaldo atual: %.2f\n\n", conta->saldo);
                break;
            case 3:
            	system("cls");
                printf("Valor para sacar: ");
                scanf("%lf", &valor);
                system("cls");
                if (valor <= conta->saldo) {
                    conta->saldo -= valor;
                    printf("Saque realizado.\nSaldo atual: %.2f\n\n", conta->saldo);
                } else {
                    printf("Saldo insuficiente.\n\n");
                }
                break;
            case 4: 
            	system("cls");
            		printf("Extrato não implementado ainda.\n\n");
            	break;
            case 5:
                system("cls");
                	if (strcmp(conta->cliente.tipo_conta, "Corrente") == 0) {
        				printf("Limite da conta: %s\n\n", conta->cliente.limite_conta);
    				} else {
        				printf("Conta poupançaa não possui limite.\n\n");
    				}
                break;			
			case 6:
            	system("cls");
                printf("Saindo...\n\n");
                break;
            default:
                printf("\nOpção inválida.\n\n");
        }
    } while (opcao != 6);
}

void loginCliente() {
    char senhaDigitada[MAXSENHA];
    int i = 0;
    char ch;
    int contaEncontrada = 0;

    printf("Digite o número da conta: ");
    char numeroConta[MAXCARACTER];
    scanf("%s", numeroConta);

    printf("Digite a senha: ");
    while (1) {
        ch = getch();
        if (ch == 13) { // Enter
            senhaDigitada[i] = '\0';
            break;
        } else if (ch == 8) { // Backspace
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            senhaDigitada[i++] = ch;
            printf("*");
        }
    }

    for (int j = 0; j < numContas; j++) {
        if (strcmp(contas[j].cliente.numero_conta, numeroConta) == 0 && verificarSenhaCliente(senhaDigitada, contas[j].cliente.senha)) {
            contaEncontrada = 1;
            system("cls");
            menuCliente(&contas[j]);
            break;
        }
    }

    if (!contaEncontrada) {
    	system("cls");
        printf("Conta não encontrada.\n\n");
    }
}

void adicionarConta(int tipo) {	
	if(tipo == 1) {
		if (numContas >= MAXCONTAS) {
    	system("cls");
        printf("Limite de contas atingido.\n\n");
        return;
    	}
		
		struct Cliente novoCliente;
    	struct Conta novaConta;
    	
		int opcao;
		
    	printf("Digite o tipo de conta:\n\n(1)Conta Poupança\n(2)Conta Corrente\n\nOpção: ");
    	scanf("%d", &opcao);
    	system("cls");
    	  
		FILE *arquivo_clientes = fopen("ContasClientes.csv", "a");
        	if (arquivo_clientes == NULL) {
        		printf("Erro ao abrir o arquivo.\n");
        		exit(1);
			}
		   	
		fprintf(arquivo_clientes, "Conta Cliente: \n");
		
    	if(opcao == 1) {
            strcpy(novoCliente.tipo_conta, "Conta Poupança");
            fprintf(arquivo_clientes, "Tipo de Conta: %s\n", novoCliente.tipo_conta);
        } else if(opcao == 2) {
            strcpy(novoCliente.tipo_conta, "Conta Corrente");
            fprintf(arquivo_clientes, "Tipo de Conta: %s\n", novoCliente.tipo_conta);
        } else {
            printf("\nOpção inválida.\n");
            return;
        }
        
		printf("Digite a agencia: ");
		getchar();
    	fgets(novoCliente.agencia, sizeof(novoCliente.agencia), stdin);
        novoCliente.agencia[strcspn(novoCliente.agencia, "\n")] = '\0';
        fprintf(arquivo_clientes, "Agencia: %s\n", novoCliente.agencia);
    	printf("Digite o numero da conta: ");
    	fgets(novoCliente.numero_conta, sizeof(novoCliente.numero_conta), stdin);
        novoCliente.numero_conta[strcspn(novoCliente.numero_conta, "\n")] = '\0';
        fprintf(arquivo_clientes, "Numero da Conta: %s\n", novoCliente.numero_conta);
			if(opcao == 2) {
				printf("Digite o limite da conta: ");
    			fgets(novoCliente.limite_conta, sizeof(novoCliente.limite_conta), stdin);
        		novoCliente.limite_conta[strcspn(novoCliente.limite_conta, "\n")] = '\0';
        		fprintf(arquivo_clientes, "Limite da Conta: %s\n", novoCliente.limite_conta);
    			printf("Digite a data de vencimento: ");
				fgets(novoCliente.data_vencimento, sizeof(novoCliente.data_vencimento), stdin);
        		novoCliente.data_vencimento[strcspn(novoCliente.data_vencimento, "\n")] = '\0';
        		fprintf(arquivo_clientes, "Data de Vencimento: %s\n", novoCliente.data_vencimento);
			}
    	printf("Digite o nome: ");
        fgets(novoCliente.nome, sizeof(novoCliente.nome), stdin);
        novoCliente.nome[strcspn(novoCliente.nome, "\n")] = '\0';
        fprintf(arquivo_clientes, "Nome: %s\n", novoCliente.nome);
    	printf("Digite o CPF: ");
    	fgets(novoCliente.cpf, sizeof(novoCliente.cpf), stdin);
        novoCliente.cpf[strcspn(novoCliente.cpf, "\n")] = '\0';
        fprintf(arquivo_clientes, "CPF: %s\n", novoCliente.cpf);
    	printf("Digite a data de nascimento: ");
    	fgets(novoCliente.data_nascimento, sizeof(novoCliente.data_nascimento), stdin);
        novoCliente.data_nascimento[strcspn(novoCliente.data_nascimento, "\n")] = '\0';
        fprintf(arquivo_clientes, "Data de Nascimento: %s\n", novoCliente.data_nascimento);
    	printf("Digite o telefone: ");
    	fgets(novoCliente.telefone, sizeof(novoCliente.telefone), stdin);
        novoCliente.telefone[strcspn(novoCliente.telefone, "\n")] = '\0';
        fprintf(arquivo_clientes, "Telefone de Contato: %s\n", novoCliente.telefone);
    	printf("Digite o endereco: ");
    	fgets(novoCliente.endereco, sizeof(novoCliente.endereco), stdin);
        novoCliente.endereco[strcspn(novoCliente.endereco, "\n")] = '\0';
        fprintf(arquivo_clientes, "Endereco: %s\n", novoCliente.endereco);
    	printf("Digite o CEP: ");
    	fgets(novoCliente.cep, sizeof(novoCliente.cep), stdin);
        novoCliente.cep[strcspn(novoCliente.cep, "\n")] = '\0';
        fprintf(arquivo_clientes, "CEP: %s\n", novoCliente.cep);
    	printf("Digite o local: ");
    	fgets(novoCliente.local, sizeof(novoCliente.local), stdin);
        novoCliente.local[strcspn(novoCliente.local, "\n")] = '\0';
        fprintf(arquivo_clientes, "Local: %s\n", novoCliente.local);
    	printf("Digite o numero da casa: ");
    	fgets(novoCliente.numero_casa, sizeof(novoCliente.numero_casa), stdin);
        novoCliente.numero_casa[strcspn(novoCliente.numero_casa, "\n")] = '\0';
        fprintf(arquivo_clientes, "Numero da Casa: %s\n", novoCliente.numero_casa);
    	printf("Digite o bairro: ");
    	fgets(novoCliente.bairro, sizeof(novoCliente.bairro), stdin);
        novoCliente.bairro[strcspn(novoCliente.bairro, "\n")] = '\0';
        fprintf(arquivo_clientes, "Bairro: %s\n", novoCliente.bairro);
    	printf("Digite a cidade: ");
    	fgets(novoCliente.cidade, sizeof(novoCliente.cidade), stdin);
        novoCliente.cidade[strcspn(novoCliente.cidade, "\n")] = '\0';
        fprintf(arquivo_clientes, "Cidade: %s\n", novoCliente.cidade);
    	printf("Digite o estado: ");
    	fgets(novoCliente.estado, sizeof(novoCliente.estado), stdin);
        novoCliente.estado[strcspn(novoCliente.estado, "\n")] = '\0';
        fprintf(arquivo_clientes, "Estado: %s\n", novoCliente.estado);
    	printf("Digite a senha: ");
    	fgets(novoCliente.senha, sizeof(novoCliente.senha), stdin);
        novoCliente.senha[strcspn(novoCliente.senha, "\n")] = '\0';
        fprintf(arquivo_clientes, "Senha: %s\n", novoCliente.senha);

		fprintf(arquivo_clientes, "\n");

    	novaConta.cliente = novoCliente;
    	novaConta.saldo = 0.0;

    	contas[numContas++] = novaConta;
    	
    	fclose(arquivo_clientes);
    	
		system("cls");
    	printf("Conta cliente adicionada com sucesso.\n\n");
	}

	else if (tipo == 2) {
		
		if (numContasFunc >= MAXCONTAS) {
    		system("cls");
        	printf("Limite de contas atingido.\n\n");
        	return;
    	}
		
		struct Funcionario novoFuncionario;
    	struct Funcionario novaContaFunc;

		if (verificarSenhaAdministrador()) {			
		
        	system("cls");
			printf("Senha correta\n\n");
			
			FILE *arquivo_funcionarios = fopen("ContasFuncionarios.csv", "a");
        		if (arquivo_funcionarios == NULL) {
        			printf("Erro ao abrir o arquivo de cotas.\n");
        			exit(1);
				}
			
        	printf("Digite o codigo do funcionario: ");
        	getchar();
        	fgets(novoFuncionario.codigo_funcionario, sizeof(novoFuncionario.codigo_funcionario), stdin);
        	novoFuncionario.codigo_funcionario[strcspn(novoFuncionario.codigo_funcionario, "\n")] = '\0';
        	fprintf(arquivo_funcionarios, "Codigo do Funcionario: %s\n", novoFuncionario.codigo_funcionario);
	   		printf("Digite o cargo do funcionario: ");
	   		fgets(novoFuncionario.cargo, sizeof(novoFuncionario.cargo), stdin);
        	novoFuncionario.cargo[strcspn(novoFuncionario.cargo, "\n")] = '\0';
        	fprintf(arquivo_funcionarios, "Cargo: %s\n", novoFuncionario.cargo);
	   		printf("Digite o nome do funcionario: ");
	   		fgets(novoFuncionario.nome_do_funcionario, sizeof(novoFuncionario.nome_do_funcionario), stdin);
        	novoFuncionario.nome_do_funcionario[strcspn(novoFuncionario.nome_do_funcionario, "\n")] = '\0';
        	fprintf(arquivo_funcionarios, "Nome: %s\n", novoFuncionario.nome_do_funcionario);
	   		printf("Digite o CPF: ");
		    fgets(novoFuncionario.CPF_do_funcionario, sizeof(novoFuncionario.CPF_do_funcionario), stdin);
        	novoFuncionario.CPF_do_funcionario[strcspn(novoFuncionario.CPF_do_funcionario, "\n")] = '\0';
        	fprintf(arquivo_funcionarios, "CPF: %s\n", novoFuncionario.CPF_do_funcionario);
	   		printf("Digite a data de nascimento: ");
		    fgets(novoFuncionario.data_de_nascimento, sizeof(novoFuncionario.data_de_nascimento), stdin);
        	novoFuncionario.data_de_nascimento[strcspn(novoFuncionario.data_de_nascimento, "\n")] = '\0';
        	fprintf(arquivo_funcionarios, "Data de Nascimento: %s\n", novoFuncionario.data_de_nascimento);
	   		printf("Digite o telefone de contato: ");
		    fgets(novoFuncionario.telefone_de_contato, sizeof(novoFuncionario.telefone_de_contato), stdin);
        	novoFuncionario.telefone_de_contato[strcspn(novoFuncionario.telefone_de_contato, "\n")] = '\0';
        	fprintf(arquivo_funcionarios, "Telefone de Contato: %s\n", novoFuncionario.telefone_de_contato);
	   		printf("Digite o endereco: ");
		    fgets(novoFuncionario.endereco_do_funcionario, sizeof(novoFuncionario.endereco_do_funcionario), stdin);
        	novoFuncionario.endereco_do_funcionario[strcspn(novoFuncionario.endereco_do_funcionario, "\n")] = '\0';
        	fprintf(arquivo_funcionarios, "Endereco: %s\n", novoFuncionario.endereco_do_funcionario);
	   		printf("Digite o CEP: ");
		    fgets(novoFuncionario.CEP, sizeof(novoFuncionario.CEP), stdin);
        	novoFuncionario.CEP[strcspn(novoFuncionario.CEP, "\n")] = '\0';
        	fprintf(arquivo_funcionarios, "CEP: %s\n", novoFuncionario.CEP);
	   		printf("Digite o local: ");
		    fgets(novoFuncionario.local, sizeof(novoFuncionario.local), stdin);
        	novoFuncionario.local[strcspn(novoFuncionario.local, "\n")] = '\0';
        	fprintf(arquivo_funcionarios, "Local: %s\n", novoFuncionario.local);
	   		printf("Digite o numero da casa: ");
		    fgets(novoFuncionario.numero_da_casa, sizeof(novoFuncionario.numero_da_casa), stdin);
        	novoFuncionario.numero_da_casa[strcspn(novoFuncionario.numero_da_casa, "\n")] = '\0';
        	fprintf(arquivo_funcionarios, "Numero da Casa: %s\n", novoFuncionario.numero_da_casa);
		    printf("Digite o bairro: ");
	   		fgets(novoFuncionario.bairro, sizeof(novoFuncionario.bairro), stdin);
        	novoFuncionario.bairro[strcspn(novoFuncionario.bairro, "\n")] = '\0';
        	fprintf(arquivo_funcionarios, "Bairro: %s\n", novoFuncionario.bairro);
	   		printf("Digite o cidade: ");
	    	fgets(novoFuncionario.cidade, sizeof(novoFuncionario.cidade), stdin);
        	novoFuncionario.cidade[strcspn(novoFuncionario.cidade, "\n")] = '\0';
        	fprintf(arquivo_funcionarios, "Cidade: %s\n", novoFuncionario.cidade);
	   		printf("Digite o estado: ");
	    	fgets(novoFuncionario.estado, sizeof(novoFuncionario.estado), stdin);
        	novoFuncionario.estado[strcspn(novoFuncionario.estado, "\n")] = '\0';
        	fprintf(arquivo_funcionarios, "Estado: %s\n", novoFuncionario.estado);
	    	printf("Digite a senha: ");
    		fgets(novoFuncionario.senha_do_funcionario, sizeof(novoFuncionario.senha_do_funcionario), stdin);
        	novoFuncionario.senha_do_funcionario[strcspn(novoFuncionario.senha_do_funcionario, "\n")] = '\0';
			fprintf(arquivo_funcionarios, "Senha: %s\n", novoFuncionario.senha_do_funcionario);
				
			fprintf(arquivo_funcionarios, "\n");
		
			funcionarios[numContasFunc++] = novoFuncionario;
			
			fclose(arquivo_funcionarios);
			
			system("cls");
    		printf("Conta funcionario adicionada com sucesso.\n\n");
       	}
        else {
       		printf("\nSenha Incorreta\n\n");
   		}		
	}   
}

void consultarConta(struct Conta *conta) {
    printf("Tipo de conta: %s\n", conta->cliente.tipo_conta);
    printf("Nome: %s\n", conta->cliente.nome);
    printf("CPF: %s\n", conta->cliente.cpf);
    printf("Saldo: %.2f\n\n", conta->saldo);
    if (strcmp(conta->cliente.tipo_conta, "Conta Corrente") == 0) {
        printf("Limite da conta: %s\n", conta->cliente.limite_conta);
        printf("Data de vencimento: %s\n\n", conta->cliente.data_vencimento);
    }
}

void consultarFuncionario(struct Funcionario *funcionario) {
	printf("Código Funcionario: %s\n", funcionario->codigo_funcionario);
	printf("Cargo: %s\n", funcionario->cargo);
	printf("Nome: %s\n", funcionario->nome_do_funcionario);
	printf("CPF: %s\n",funcionario->CPF_do_funcionario);
	printf("Data de Nascimento: %s\n", funcionario->data_de_nascimento);
	printf("Telefone: %s\n", funcionario->telefone_de_contato);
	printf("Endereco: %s\n", funcionario->endereco_do_funcionario);
	printf("N da Casa: %s\n", funcionario->numero_da_casa);
	printf("CEP: %s\n", funcionario->CEP);
	printf("Bairro: %s\n", funcionario->bairro);
	printf("Cidade: %s\n", funcionario->cidade);
	printf("Estado: %s\n\n", funcionario->estado);
}
	
void consultarCliente(struct Conta *conta) {	 
	printf("Nome: %s\n", conta->cliente.nome);
    printf("CPF: %s\n", conta->cliente.cpf);
    printf("Data de Nascimento: %s\n", conta->cliente.data_nascimento);
    printf("Telefone: %s\n", conta->cliente.telefone);
    printf("Endereco: %s\n", conta->cliente.endereco);
    printf("N da Casa: %s\n", conta->cliente.numero_casa);
    printf("CEP: %s\n", conta->cliente.cep);
    printf("Bairro: %s\n", conta->cliente.bairro);
    printf("Cidade: %s\n", conta->cliente.cidade);
    printf("Estado: %s\n\n", conta->cliente.estado); 			   
}

void consultarDados() {	
	int opcao;
	
	do {
		int contaEncontrada = 0;
		
		printf("Digite a opcao desejada!\n\n");
		printf("(1)Consultar Conta\n(2)Consultar Funcionário\n(3)Consultar Cliente\n(4)Voltar");
		printf("\n\nOpção: ");
		scanf("%d", &opcao);
		
		switch(opcao) {
			case 1:
				system("cls");
                printf("Digite o número da conta: ");
                char numeroConta[MAXCARACTER];
                scanf("%s", numeroConta);
                
                for (int j = 0; j < numContas; j++) {
                    if (strcmp(contas[j].cliente.numero_conta, numeroConta) == 0) {
                        contaEncontrada = 1;
                        system("cls");
                        consultarConta(&contas[j]);
                        break;
                    }
                }
                
                if (!contaEncontrada) {
                    system("cls");
                    printf("Conta não encontrada.\n\n");
                }
                break;
			case 2:
				system("cls");
                printf("Digite o código do funcionário: ");
                char codigoFuncionario[MAXCARACTER];
                scanf("%s", codigoFuncionario);
                
                for (int j = 0; j < numContasFunc; j++) {
                    if (strcmp(funcionarios[j].codigo_funcionario, codigoFuncionario) == 0) {
                        contaEncontrada = 1;
                        system("cls");
                        consultarFuncionario(&funcionarios[j]);
                        break;
                    }
                }
                
                if (!contaEncontrada) {
                    system("cls");
                    printf("Funcionário não encontrado.\n\n");
                }
                break;
			case 3:
				system("cls");
                printf("Digite o número da conta: ");
                scanf("%s", numeroConta);
                
                for (int j = 0; j < numContas; j++) {
                    if (strcmp(contas[j].cliente.numero_conta, numeroConta) == 0) {
                        contaEncontrada = 1;
                        system("cls");
                        consultarCliente(&contas[j]);
                        break;
                    }
                }
                
                if (!contaEncontrada) {
                    system("cls");
                    printf("Conta não encontrada.\n\n");
                }
                break;
			case 4:
				system("cls");
				printf("Voltando...\n\n");
				break;
			default:
				printf("\nOpção inválida.\n\n");
		}
	}
	while(opcao != 4);
}

void alterarConta(struct Conta *conta) {
	int tipo;
	int alteracaoValida = 0;
	
    printf("Digite o novo tipo de conta (1 - Conta Poupanca, 2 - Conta Corrente): ");
    scanf("%d", &tipo);
		if (tipo == 1) {
            strcpy(contas->cliente.tipo_conta, "Conta Poupanca");
        } 
		else if (tipo == 2) {
            strcpy(contas->cliente.tipo_conta, "Conta Corrente");
            printf("Digite o novo limite disponivel da conta: ");
			getchar();		
			fgets(contas->cliente.limite_conta, sizeof(contas->cliente.limite_conta), stdin);
        	contas->cliente.limite_conta[strcspn(contas->cliente.limite_conta, "\n")] = '\0';
    		printf("Digite a nova data de vencimento da conta: ");
    		fgets(contas->cliente.data_vencimento, sizeof(contas->cliente.data_vencimento), stdin);
        	contas->cliente.data_vencimento[strcspn(contas->cliente.data_vencimento, "\n")] = '\0';
        }
		else {
            printf("\nOpcao invalida.\n\n");
        }
        
		alteracaoValida = 1;
        
        if (alteracaoValida) {
            printf("Dados alterados com sucesso.\n\n");
		}
}

void alterarFuncionario(struct Funcionario *funcionario) {
	int opcao;
    
    do {
    	int alteracaoValida = 0;
    	
        printf("Digite a opcao desejada para alteracao:\n\n(1)Codigo do Funcionario\n(2)Cargo\n(3)Telefone\n(4)Endereco\n(5)Numero da Casa\n(6)Bairro\n(7)Cidade\n(8)Estado\n(9)Voltar\n\nOpcao: ");
        scanf("%d", &opcao);
    
        switch(opcao) {
            case 1:
                system("cls");
                printf("Digite o novo codigo funcionario: ");
    			getchar();
    			fgets(funcionarios->codigo_funcionario, sizeof(funcionarios->codigo_funcionario), stdin);
    			funcionarios->codigo_funcionario[strcspn(funcionarios->codigo_funcionario, "\n")] = '\0';
    			alteracaoValida = 1;
                break;
            case 2:
                system("cls");
                printf("Digite o novo cargo da conta: ");
                getchar();
    			fgets(funcionarios->cargo, sizeof(funcionarios->cargo), stdin);
    			funcionarios->cargo[strcspn(funcionarios->cargo, "\n")] = '\0';
    			alteracaoValida = 1;
                break;
            case 3:
                system("cls");
                printf("Digite o novo telefone de contato: ");
                getchar();
    			fgets(funcionarios->telefone_de_contato, sizeof(funcionarios->telefone_de_contato), stdin);
    			funcionarios->telefone_de_contato[strcspn(funcionarios->telefone_de_contato, "\n")] = '\0';
    			alteracaoValida = 1;
                break;
            case 4:
                system("cls");
                printf("Digite o novo endereco : ");
                getchar();
    			fgets(funcionarios->endereco_do_funcionario, sizeof(funcionarios->endereco_do_funcionario), stdin);
    			funcionarios->endereco_do_funcionario[strcspn(funcionarios->endereco_do_funcionario, "\n")] = '\0';
				alteracaoValida = 1; 
                break;
            case 5:
                system("cls");
                printf("Digite o novo numero da casa: ");
                getchar();
    			fgets(funcionarios->numero_da_casa, sizeof(funcionarios->numero_da_casa), stdin);
    			funcionarios->numero_da_casa[strcspn(funcionarios->numero_da_casa, "\n")] = '\0';
    			alteracaoValida = 1;
                break;
            case 6:
                system("cls");
                printf("Digite o novo bairro: ");
                getchar();
    			fgets(funcionarios->bairro, sizeof(funcionarios->bairro), stdin);
    			funcionarios->bairro[strcspn(funcionarios->bairro, "\n")] = '\0';
    			alteracaoValida = 1;
                break;
            case 7:
                system("cls");
                printf("Digite a nova cidade: ");
                getchar();
    			fgets(funcionarios->cidade, sizeof(funcionarios->cidade), stdin);
    			funcionarios->cidade[strcspn(funcionarios->cidade, "\n")] = '\0';
    			alteracaoValida = 1;
                break;
            case 8:
            	system("cls");
				printf("Digite o novo estado: ");
				getchar();
    			fgets(funcionarios->estado, sizeof(funcionarios->estado), stdin);
    			funcionarios->estado[strcspn(funcionarios->estado, "\n")] = '\0';
    			alteracaoValida = 1;	
			case 9:
                system("cls");
                printf("Voltando...\n\n");
                break;
            default:
                printf("\nOpção inválida.\n\n");
        }         
        if (alteracaoValida) {
            printf("Dados alterados com sucesso.\n\n");
        }
    } 
	while(opcao != 9);
}

void alterarCliente(struct Conta *conta) {
    int opcao;
    
    do {
    	int alteracaoValida = 0;
    	
        printf("Digite a opcao desejada para alteracao:\n\n(1)Telefone\n(2)Endereco\n(3)Numero da Casa\n(4)CEP\n(5)Bairro\n(6)Cidade\n(7)Estado\n(8)Voltar\n\nOpcao: ");
        scanf("%d", &opcao);
    
        switch(opcao) {
            case 1:
                system("cls");
                printf("Digite o novo telefone de contato: ");
				getchar();            
				fgets(conta->cliente.telefone, sizeof(contas->cliente.telefone), stdin);
        		contas->cliente.telefone[strcspn(contas->cliente.telefone, "\n")] = '\0';
        		alteracaoValida = 1;
                break;
            case 2:
                system("cls");
                printf("Digite o novo endereco: ");
                getchar();
                fgets(conta->cliente.endereco, sizeof(contas->cliente.endereco), stdin);
        		contas->cliente.endereco[strcspn(contas->cliente.endereco, "\n")] = '\0';
        		alteracaoValida = 1;
                break;
            case 3:
                system("cls");
                printf("Digite o novo numero da casa: ");
                getchar();
                fgets(conta->cliente.numero_casa, sizeof(contas->cliente.numero_casa), stdin);
        		contas->cliente.numero_casa[strcspn(contas->cliente.numero_casa, "\n")] = '\0';
        		alteracaoValida = 1;
                break;
            case 4:
                system("cls");
                printf("Digite o novo CEP : ");
                getchar();
                fgets(conta->cliente.cep, sizeof(contas->cliente.cep), stdin);
        		contas->cliente.cep[strcspn(contas->cliente.cep, "\n")] = '\0';
				alteracaoValida = 1;  
                break;
            case 5:
                system("cls");
                printf("Digite o novo bairro: ");
                getchar();
                fgets(conta->cliente.bairro, sizeof(contas->cliente.bairro), stdin);
        		contas->cliente.bairro[strcspn(contas->cliente.bairro, "\n")] = '\0';
        		alteracaoValida = 1;
                break;
            case 6:
                system("cls");
                printf("Digite o novo cidade: ");
                getchar();
                fgets(conta->cliente.cidade, sizeof(contas->cliente.cidade), stdin);
        		contas->cliente.cidade[strcspn(contas->cliente.cidade, "\n")] = '\0';
        		alteracaoValida = 1;
                break;
            case 7:
                system("cls");
                printf("Digite o novo estado: ");
                getchar();
                fgets(conta->cliente.estado, sizeof(contas->cliente.estado), stdin);
        		contas->cliente.estado[strcspn(contas->cliente.estado, "\n")] = '\0';
        		alteracaoValida = 1;
                break;
            case 8:
                system("cls");
                printf("Voltando...\n\n");
                break;
            default:
                printf("\nOpção inválida.\n\n");
        }        
        if (alteracaoValida) {
            printf("Dados alterados com sucesso.\n\n");
        }
    } 
	while(opcao != 8);
}

void alterarDados() {
	int opcao;
	
	do {
		int contaEncontrada = 0;
		
		printf("Digite a opcao desejada!\n\n(1)Alterar Conta\n(2)Alterar Funcionario\n(3)Alterar Cliente\n(4)Voltar\n\nOpcao: ");
		scanf("%d", &opcao);
		
		switch(opcao) {
			case 1:
				system("cls");
                	if (verificarSenhaAdministrador()) {
                    	system("cls");
                		printf("Digite o número da conta: ");
                		char numeroConta[MAXCARACTER];
                		scanf("%s", numeroConta);
                
                			for (int j = 0; j < numContas; j++) {
                    			if (strcmp(contas[j].cliente.numero_conta, numeroConta) == 0) {
                        			contaEncontrada = 1;
                        			system("cls");
                        			alterarConta(&contas[j]);
                    			}
                			}
                		if (!contaEncontrada) {
                    		system("cls");
                    		printf("Conta não encontrada.\n\n");
                		}
                	}
				break;
			case 2:
				system("cls");
					if (verificarSenhaAdministrador()) {
                		system("cls");
						printf("Digite o código do funcionário: ");
                		char codigoFuncionario[MAXCARACTER];
                		scanf("%s", codigoFuncionario);
                
                			for (int j = 0; j < numContasFunc; j++) {
                    			if (strcmp(funcionarios[j].codigo_funcionario, codigoFuncionario) == 0) {
                        			contaEncontrada = 1;
                        			system("cls");
                        			alterarFuncionario(&funcionarios[j]);
                    			}
                			}
                		if (!contaEncontrada) {
                    		system("cls");
                    		printf("Funcionário não encontrado.\n\n");
                		}
            		}
                break;
			case 3:
				system("cls");
                	if (verificarSenhaAdministrador()) {
                    	system("cls");
                		printf("Digite o número da conta: ");
                		char numeroConta[MAXCARACTER];
                		scanf("%s", numeroConta);
                
                			for (int j = 0; j < numContas; j++) {
                    			if (strcmp(contas[j].cliente.numero_conta, numeroConta) == 0) {
                        			contaEncontrada = 1;
                        			system("cls");
                        			alterarCliente(&contas[j]);
                    			}
                			}
                		if (!contaEncontrada) {
                    		system("cls");
                    		printf("Conta não encontrada.\n\n");
                		}
                	}
				break;
			case 4:
				system("cls");
				printf("Voltando...\n\n");
				break;
			default:
				printf("\nOpção inválida.\n\n");
		}
	}
	while(opcao != 4);
}

void encerramentoDeConta() {
	//Não conseguimos fazer ;-;
}

void gerarRelatorio() {
	system("ContasFuncionarios.csv");
	system("ContasClientes.csv");	
}

void menuFuncionario() {
    int opcao;
	
	do {
		printf("Bem Vindo ao Menu Funcionário!\nDigite a opcao desejada!\n\n(1)Abertura de Conta\n(2)Encerramento de Conta\n(3)Consultar Dados\n(4)Alterar Dados\n(5)Cadastro de Funcionario\n(6)Gerar Relatorios\n(7)Sair\n\nOpcao: ");
		scanf("%d", &opcao);
		
		switch(opcao) {
			case 1:
				system("cls");
				adicionarConta(1);
				break;
			case 2:
				system("cls");
				if (verificarSenhaAdministrador()) {
                    system("cls");
                    printf("Senha correta\n\n"); 
					int numero_conta_excluir;                 
					printf("Digite o número da conta: ");
					scanf("%d", &numero_conta_excluir);	
					encerramentoDeConta();
                }			
				break;
			case 3:
				system("cls");
				consultarDados();
				break;
			case 4:
				system("cls");	
				alterarDados();
				break;
			case 5:
				system("cls");
        		adicionarConta(2);
				break;
			case 6:
				system("cls");			
				gerarRelatorio();
				break;	
			case 7:
				system("cls");
				printf("Saindo...\n\n");
				break;
      		default:
      	    	printf("\nOpção inválida.\n\n");
        	}
	}

	while(opcao != 7);
	
}

void printBancoASCII() {
    char bancoASCII[6][60] = {
        "#####    ####   #    #  #####   #### ",
        "#    #  #    #  ##   #  #      #    #",
        "#####   ######  # #  #  #      #    #",
        "#    #  #    #  #  # #  #      #    #",
        "#####   #    #  #   ##  #####   #### ",
        ""
    };

    for (int i = 0; i < 6; i++) {
        printf("%s\n", bancoASCII[i]);
    }
}

int main() {
    int opcao;
    
    setlocale(LC_ALL, "Portuguese_Brazil");
	carregarContas();

    do {
        printBancoASCII();
        printf("Bem Vindo ao Banco!\nDigite a opção desejada!\n\n(1)Funcionário\n(2)Cliente\n(3)Sair\n\nOpção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                system("cls");
                if (verificarSenhaAdministrador()) {
                    system("cls");
                    printf("Senha correta\n\n");
                    menuFuncionario();
                }
                break;
            case 2:
                system("cls");
                loginCliente();
                break;
            case 3:
            	salvarContas();
                printf("\nEncerrando o programa...\n");
                break;
            default:
                printf("\nOpção inválida.\n\n");
        }
    } while (opcao != 3);

    return 0;
}
