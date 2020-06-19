/**
 * Este programa cria um simples "hash" do nome
 * de uma pessoa. Um hash é um inteiro que matematicamente
 * é derivado de outro objeto. Neste caso, uma string
 * (o primeiro e último nome) é acessado
 * caractere por caractere e o hash é construído
 * a partir do número gerado por cada caracetere.
 * Essa função não produz hash únicos (nomes diferentes
 * podem ter o mesmo valor de hash), há uma 
 * pequena possibilidade desas "colisões" serem praticamente
 * zero utilizando os alunos registrados na disciplina.
 *
 * Vamos aprender mais sobre hash no futuro.
 */

#include <iostream>
#include <string>

using namespace std;

/**
 * Protótipo da função hashingNome. Assim podemos utilizar
 * uma função no main que foi definida posteriormente.
 */
int hashingNome(string first, string last);

int main() {
    string primeiro_nome = "Vitor";
    string ultimo_nome = "Yeso";

    int valorHash = hashingNome(primeiro_nome, ultimo_nome);

    cout << "O hash do seu nome é: " << valorHash << endl;
    return 0;
}

/**
 * Implementação: Função hashingNome
 * ---------------------------------
 * Está é a função que calcula o código hash. 
 * Nós iremos conversar mais sobre isso futuramente. 
 * Agora é importante saber que essa função percorre 
 * os caracteres de entrada e produz um número.
 *
 * Para os curiosos, essa função trata cada caractere do nome
 * informado como um número entre 0 e 128. Então são 
 * utilizados como coeficientes num polinômio finito, F_p,
 * onde p é um número primo bem grande, e estima esse
 * polinômio usando um número primo pequeno q.
 */
int hashingNome(string primeiro, string ultimo){
    /**
     * Essa função hash precisa de dois números primo, um
     * número grande e outro pequeno. Estes números primos
     * foram escolhidos porque seu produto é menor que 
     * 2^31 - número primo grande -1.
     */
    static const int kPrimoGrande = 16908799;
    static const int kPrimoPequeno = 127;

    int valorHash = 0;

    /* Itera sobre todos os caracteres no primero nome, e depois no 
    * último nome. Essa função só funciona se o compilador tiver
    * a opção -std=c++11, porque apenas nessa versão entrou esse
    * operador do for baseado no tamanho(range-based). Um exercício
    * simples seria modificar o for usando a versão padrão (iterador i).
    *
    * Assim, a função itera sobre os caracteres e atualiza o
    * hash a cada etapa.
    */
    
    for (char ch: primeiro + ultimo) {
        /* Converte os caracteres informados para minúsculo. 
        * Assim os valores dos caracteres sempre serão menor que 127.
         */
        ch = tolower(ch);
        valorHash = (kPrimoPequeno * valorHash + ch) % kPrimoGrande;
    }
    return valorHash;
}
