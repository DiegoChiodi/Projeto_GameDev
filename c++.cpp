#include <iostream>
#include <string>
#include <list>

struct Livro {
	std::string nome;
	std::string autor;
	int pub;
	bool emprestado;
};

struct Aluno { //Tipo do aluno/Class
	std::string nome;
	std::string ra;
	std::string cpf;
	bool emprestando;
};

struct Emprestar {
	Aluno *aluno;
	Livro *livro;
};

void CadastrarAluno (std::list<Aluno>& alunos) {
	Aluno aluno; //Declarei um aluno ex: "int aluno;"

	std::cout << "Digite o primeiro nome do aluno: ";
	std::cin >> aluno.nome;

	std::cout << "Digite o RA: ";
	std::cin >> aluno.ra; // Guarda o que o usuario digitar no aluno

	std::cout << "Digite o cpf: ";
	std::cin >> aluno.cpf;

	aluno.emprestando = false;
	alunos.push_back(aluno); // Envia as informações para o fim da lista
	std::cout << "Aluno cadastrado com sucesso!" << std::endl;
}

void CadastrarLivro (std::list<Livro>& livros) { //para saber em que lista eu devolvo

	Livro livro; //Declarei um livro ex: "int Livro;
	
	std::cout << "Digite o nome do Livro: ";
	std::cin >> livro.nome;

	std::cout << "Digite o nome do autor: ";
	std::cin >> livro.autor;

	std::cout << "Digite o ano de publicação: ";
	std::cin >> livro.pub;

	livro.emprestado = false;	
	livros.push_back(livro); // Envia as informações para o fim da lista
	std::cout << "Livro cadastrado com sucesso!" << std::endl;
}

void VisualizarLivros (std::list<Livro>& livros)
{
	if (livros.empty())
	{
		std::cout << "Não a livros cadastrados!" << std::endl;
	} else {
		for (const Livro& livro : livros) {
			std::cout << "Livro: " << livro.nome << ", Autor: " << livro.autor  << ", Ano de publicação: " << livro.pub << ", Emprestado:" << livro.emprestado << std::endl;
		}
	}
}

template <typename T, typename U> //permite receber 2 tipos de variaveis
void Deletar(std::list<T>& lista, U T::* comparar) 
{ //T pode ser Aluno ou Livro ou Emprestar, U pode ser nome ou ra
	std::string comparador;
	std::cin >> comparador;

    if (lista.empty()) {  // Verifica se a lista está vazia
        std::cout << "A lista está vazia" << std::endl;
    } else {
        for (auto it = lista.begin(); it != lista.end();) {
            // Usa o operador .* após desreferenciar o iterador
            if ((*it).*comparar == comparador) {  // Verifica se o campo é igual ao valor fornecido
                std::cout << (*it).nome << " removido com sucesso!" << std::endl; // Mostra o nome antes de remover
                it = lista.erase(it);  // Remove o elemento e retorna o próximo iterador
                break;  // Sai do loop, pois o item é único
            } else {
                ++it;  // Avança para o próximo elemento
            }
        }
    }
}

template <typename T, typename U, typename V, typename W>
void Alterar(std::string item, std::string campo1, std::string campo2, std::string campo3,
             std::list<T>& lista, U T::*campo1_ptr, V T::*campo2_ptr, W T::*campo3_ptr) {
    std::cout << "Digite o " << campo1 << " do " << item << " que deseja alterar: ";
    std::string identificador;
    std::cin >> identificador;

    if (lista.empty()) {
        std::cout << "Não há " << item << " para alterar!" << std::endl;
        return;
    }

    for (auto& elemento : lista) {
        if (elemento.*campo1_ptr == identificador) {
            std::cout << item << " encontrado!" << std::endl;

            int escolha;
            std::cout << "Deseja alterar o atual " << campo1 << ": " << elemento.*campo1_ptr << "? (1 para Sim, 0 para Não): ";
            std::cin >> escolha;
            if (escolha == 1) {
                std::cout << "Digite o novo " << campo1 << ": ";
                std::cin >> elemento.*campo1_ptr;
            }

            std::cout << "Deseja alterar o atual " << campo2 << ": " << elemento.*campo2_ptr << "? (1 para Sim, 0 para Não): ";
            std::cin >> escolha;
            if (escolha == 1) {
                std::cout << "Digite o novo " << campo2 << ": ";
                std::cin >> elemento.*campo2_ptr;
            }

            std::cout << "Deseja alterar o atual " << campo3 << ": " << elemento.*campo3_ptr << "? (1 para Sim, 0 para Não): ";
            std::cin >> escolha;
            if (escolha == 1) {
                std::cout << "Digite o novo " << campo3 << ": ";
                std::cin >> elemento.*campo3_ptr;
            }
            std::cout << item << " alterado com sucesso!" << std::endl;
            return;
        }
    }
    std::cout << item << " não encontrado!" << std::endl;
}

void VisualizarAlunos(std::list<Aluno>& alunos)
{
	if (alunos.empty())
	{
		std::cout << "Não a alunos cadastrados!" << std::endl;
	} else {
		for (const Aluno& aluno : alunos) 
		{
			std::cout << "Aluno: " << aluno.nome << ", RA: " << aluno.ra << ", CPF: " << aluno.cpf <<", Emprestando: " << aluno.emprestando << " livro."<< std::endl;
		}
	}
}

void Devolver (std::list<Emprestar>& emprestimo) {
	
	std::list<Emprestar> emprestimos;
	std::cout << "Digite o nome do livro que deseja devolver: ";
	std::string devolver;
	std::cin >> devolver;

    if (emprestimos.empty()) {  // Verifica se a lista ficou vazia após a remoção	
	    std::cout << "A lista está vazia";
	} else {
	    for (auto emprestimo = emprestimos.begin(); emprestimo != emprestimos.end();) {  
	    	if (emprestimo->livro->nome == devolver) { 
				emprestimo = emprestimos.erase(emprestimo);  // Remove o aluno e retorna o próximo iterador
			    std::cout << "Livro devolvido com sucesso!" << std::endl;
			   	break;  // Sai do loop, pois o RA é único
		   	} else {
		  	 	++emprestimo;  // Avança para o próximo aluno se não encontraxr o RA
		    }
	    }
    }
}

void emprestarLivro(std::list<Livro>& livros, std::list<Aluno>& alunos, std::list<Emprestar>& emprestimos) {

    std::string nomeLivro;
    std::cout << "Digite o nome do livro que deseja emprestar: ";
    std::cin >> nomeLivro; // Usar getline para capturar nomes compostos

    for (auto& livro : livros) {
        if (livro.nome == nomeLivro) {
            if (livro.emprestado) {
                std::cout << "Um aluno já pegou esse livro!" << std::endl;
                return;
            } else {
                std::cout << "Livro encontrado com sucesso!" << std::endl;
                std::string raAluno;
                std::cout << "Digite o RA do aluno que deseja pegar o livro: ";
                std::cin >> raAluno;

                for (auto& aluno : alunos) {
                    if (aluno.ra == raAluno) {
                        if (aluno.emprestando) {
                            std::cout << "O aluno está no limite de livros emprestados!" << std::endl;
                            return;
                        } else {
                            std::cout << "Aluno encontrado com sucesso!" << std::endl;

                            Emprestar emprestimo;
                            emprestimo.aluno = &aluno;
                            emprestimo.livro = &livro;
                            emprestimos.push_back(emprestimo);

                            aluno.emprestando = true;
                            livro.emprestado = true;

                            std::cout << "Empréstimo registrado com sucesso!" << std::endl;
                            return;
                        }
                    }
                }
                std::cout << "Esse aluno não está no sistema!" << std::endl;
                return;
            }
        }
    }
    std::cout << "Esse livro não está no sistema!" << std::endl;
}

int main() {

	std::list<Aluno> alunos; //A lista "alunos" guarda o valor "Aluno"
	std::list<Livro> livros;
	std::list<Emprestar> emprestimos;
	
	int opcao;

	std::cout << "Seja bem-vindo à biblioteca!" << std::endl;

	std::cout << "[1] Cadastrar aluno" << std::endl;
	std::cout << "[2] Deletar aluno " << std::endl;
	std::cout << "[3] Alterar aluno " << std::endl;
	std::cout << "[4] Visualizar alunos " << std::endl << std::endl;
	
	std::cout << "[5] Cadastrar livro" << std::endl;
	std::cout << "[6] Deletar livro " << std::endl;
	std::cout << "[7] Alterar livro" << std::endl;
	std::cout << "[8] Visualizar livros" << std::endl << std::endl;
	
	std::cout << "[9] Emprestar livro para aluno" << std::endl;
	std::cout << "[10] Fazer devolução de livro" << std::endl;
	std::cout << "[11] Visualizar empréstimos" << std::endl;
	std::cout << "[12] Parar execução do programa" << std::endl << std::endl;

	while (true) {
		std::cout << "Escolha uma opção:" << std::endl;
		std::cin >> opcao;

		switch (opcao)
			case 1:
				CadastrarAluno(alunos);
				break;
			case 2:
     	       std::cout << "Digite o RA do aluno que deseja deletar do sistema: ";
				Deletar(alunos, &Aluno::ra);
				break;
			case 3:
				Alterar ("Aluno", "ra","nome", "cpf", alunos, &Aluno::ra, &Aluno::nome, &Aluno::cpf);
				break;
			case 4: 
				VisualizarAlunos(alunos);
				break;
			case 5:
				CadastrarLivro(livros);
				break;
			case 6:
				std::cout << "Digite o nome do livro que deseja deletar do sistema: ";
    	        Deletar(livros, &Livro::nome);
				break;
			case 7:
				Alterar ("Livro", "nome","autor", "data de publicação", livros, &Livro::nome, &Livro::autor, &Livro::pub);
				break;
			case 8:
				VisualizarLivros(livros);
				break;
			case 9:
				emprestarLivro(livros, alunos, emprestimos);
				break;
			case 10:
				Devolver(emprestimos);
				break;
			case 11:
				if (emprestimos.empty())
				{
					std::cout << "Não a livros emprestados!" << std::endl;
				} else {
					for (const Emprestar& emprestimo : emprestimos) {
						std::cout << "O Aluno: " << emprestimo.aluno->nome << " pegou o livro: " << emprestimo.livro->nome << " emprestado" << std::endl;
					}
				}
				break;
			default:
				int restart = main();
				break;
		}
	return 0;
}