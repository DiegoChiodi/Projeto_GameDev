#include <iostream>
#include <vector>
#include <string>
#include <limits> // Para limpar a entrada inválida

struct Livro 
{
    std::string titulo;
    std::string autor;
    int anoPublicacao;
};

void cadastrarLivro(std::vector<Livro>& biblioteca) 
{
    Livro novoLivro;

    std::cout << "Digite o título do livro: ";
    std::getline(std::cin >> std::ws, novoLivro.titulo);

    std::cout << "Digite o autor do livro: ";
    std::getline(std::cin >> std::ws, novoLivro.autor);

    std::cout << "Digite o ano de publicação: ";
    std::cin >> novoLivro.anoPublicacao;

    // Verifica se o ano é válido
    if (std::cin.fail()) 
    {
        std::cin.clear(); // Limpa estado de erro
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa buffer
        std::cout << "Entrada inválida para o ano. Livro não cadastrado.\n";
        return;
    }

    biblioteca.push_back(novoLivro);
    std::cout << "Livro cadastrado com sucesso!\n";
}

void listarLivros(const std::vector<Livro>& biblioteca) 
{
    if (biblioteca.empty()) 
    {
        std::cout << "Nenhum livro cadastrado.\n";
        return;
    }

    std::cout << "\nLista de Livros Cadastrados:\n";
    for (size_t i = 0; i < biblioteca.size(); ++i) 
    {
        std::cout << "Livro " << i + 1 << ":\n";
        std::cout << "  Título: " << biblioteca[i].titulo << "\n";
        std::cout << "  Autor: " << biblioteca[i].autor << "\n";
        std::cout << "  Ano de Publicação: " << biblioteca[i].anoPublicacao << "\n";
    }
}

int main() 
{
    std::vector<Livro> biblioteca;
    char opcao;

    do 
    {
        std::cout << "\nSistema de Cadastro de Livros\n";
        std::cout << "[1] Cadastrar Livro\n";
        std::cout << "[2] Listar Livros\n";
        std::cout << "[0] Sair\n";
        std::cout << "Escolha uma opção: ";
        std::cin >> opcao;

        switch (opcao) 
        {
            case '1':
                cadastrarLivro(biblioteca);
                break;
            case '2':
                listarLivros(biblioteca);
                break;
            case '0':
                std::cout << "Saindo...\n";
                break;
            default:
                std::cout << "Opção inválida. Tente novamente.\n";
        }
    } while (opcao != '0');

    return 0;
}
