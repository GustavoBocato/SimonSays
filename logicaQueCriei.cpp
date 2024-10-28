#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

// representa uma cor com seu nome e valores RGB
struct Cor
{
    std::string nome;
    int red, green, blue;
    Cor(std::string n, int r, int g, int b) : nome(n), red(r), green(g), blue(b) {}
};

void imprime_vetor(std::vector<Cor> &seq)
{
    std::cout << "vetor de cores: ";
    for (auto s : seq)
    {
        std::cout << s.nome << " ";
    }
    std::cout << "\n";
}

// mapeia um inteiro para um valor rgb especifico
Cor mapeia_cores(int numero)
{
    switch (numero)
    {
    case 0:
        return Cor("VERMELHO", 255, 0, 0);
    case 1:
        return Cor("VERDE", 0, 255, 0);
    case 2:
        return Cor("AZUL", 0, 0, 255);
    case 3:
        return Cor("AMARELO", 255, 255, 0);
    default:
        return Cor("PRETO", 0, 0, 0);
    }
}

Cor gera_cor_aleatoria()
{
    // gera numeros aleatorios entre 0 - 999 e mapeia pra um numero entre 0 - 3
    int numero = (rand() % 10000) % 4;
    return mapeia_cores(numero);
}

int main()
{
    unsigned seed = time(0);
    srand(seed);
    std::vector<Cor> sequencia = {};
    for (auto i = 0; i < 20; i++)
    {
        Cor c = gera_cor_aleatoria();
        sequencia.push_back(c);
        imprime_vetor(sequencia);
    }
    /*
        - jogo inicializa com um vetor de tamanho n de cores (logica acima)
        - a cada rodada, codigo percorre esse vetor, acendendo a cor em sequencia
        - codigo verifica se:
            * usuario ja apertou o numero de cores que deveria (tamanho do laço)
            * sequencia de cores bate (verifica os k primeiros elementos do vetor com o input do usuario)
        - se usuario errar, acaba o jogo e ele perde, caso contrario continua até sequencia de tamanho n

        obs: rgb da struct Cor pode ser usado na parte gráfica tb
    */
}