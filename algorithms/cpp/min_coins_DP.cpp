#include <iostream>
#include <vector>

using namespace std;

/* simple linear search in a list */
bool search_unsigned(unsigned* list, unsigned list_len, unsigned value);

/* O algoritmo recebe uma lista de valores de moedas disponiveis
 * e o valor do troco. eh retornado a quantidade minima de moedas 
 * para dar o troco.
*/
int min_coins_recursive(unsigned* coins, unsigned list_len, unsigned change);

/* algoritmo min_coins recursivo usando tabela para memoization */
int min_coins_rec_memoization(unsigned* coins, unsigned list_len, unsigned change, unsigned* table);

/* algoritmo min_coins usando programacao dinamica */
int min_coins_DP(unsigned* coins, unsigned list_len, unsigned change, unsigned* min_coins, unsigned* coins_used);

/* print unsigned array */
void print_unsigned_arr(unsigned* list, unsigned list_len);

void printCoins(unsigned* coinsUsed,unsigned change);

int main() {
    unsigned LEN = 5;
    unsigned* coins = new unsigned[LEN];

    coins[0] = 1;
    coins[1] = 5;
    coins[2] = 10;
    coins[3] = 21;
    coins[4] = 25;

    unsigned change = 63;
    unsigned* table = new unsigned[change];
    for (unsigned i=0; i<change; i++)
        table[i] = 0;

    unsigned* min_coins = new unsigned[change];
    for (unsigned i=0; i<change; i++)
        min_coins[i] = 0;

    unsigned* coins_used = new unsigned[change];
    for (unsigned i=0; i<change; i++)
        coins_used[i] = 0;

    print_unsigned_arr(coins, LEN);
    cout << ", troco de " << change << "." << endl;
    //cout << "total de: " << min_coins_recursive(coins, LEN, change) << " moedas" << endl;
    cout << "total de: " << min_coins_rec_memoization(coins, LEN, change, table) << " moedas" << endl;
    cout << "total de: " << min_coins_DP(coins, LEN, change, min_coins, coins_used) << " moedas" << endl;
    printCoins(coins_used, change);
    
}

int min_coins_recursive(unsigned* coins, unsigned list_len, unsigned change) {
    unsigned min_coins = change;
    unsigned num_coins;

    if (search_unsigned(coins, list_len, change)) {
        return 1;
    }
    else {
        for (unsigned i=0; i<list_len; i++) {
            if (coins[i] < change) {
                num_coins = 1 + min_coins_recursive(coins, list_len, change - coins[i]);

                if (num_coins < min_coins)
                    min_coins = num_coins;
            }
        }
    }
    return min_coins;
}

int min_coins_rec_memoization(unsigned* coins, unsigned list_len, unsigned change, unsigned* table) {
    unsigned min_coins = change;
    unsigned num_coins;

    if (search_unsigned(coins, list_len, change)) {
        table[change] = 1;
        return 1;
    }
    else if (table[change] > 0) {
        return table[change];
    }
    else {
        for (unsigned i=0; i<list_len; i++) {
            if (coins[i] < change) {
                num_coins = 1 + min_coins_rec_memoization(coins, list_len, change - coins[i], table);

                if (num_coins < min_coins) {
                    min_coins = num_coins;
                    table[change] = min_coins;
                }
            }
        }
    }
    return min_coins;
}

int min_coins_DP(unsigned* coins, unsigned list_len, unsigned change, unsigned* min_coins, unsigned* coins_used) {
    unsigned coin_count;
    unsigned new_coin;
    for (unsigned cents=0; cents<=change; cents++) {
        coin_count = cents;
        new_coin = 1;
        for (unsigned j=0; j<list_len; j++) {
            if (coins[j] <= cents) {
                if (min_coins[cents - coins[j]] + 1 < coin_count) {
                    coin_count = min_coins[cents - coins[j]] + 1;
                    new_coin = coins[j];
                }
            }
        }
        min_coins[cents] = coin_count;
        coins_used[cents] = new_coin;
    }
    return min_coins[change];
}

bool search_unsigned(unsigned* list, unsigned list_len, unsigned value) {
    for (unsigned i=0; i<list_len; i++) {
        if (value == list[i]) return true;
    }
    return false;
}

void print_unsigned_arr(unsigned* list, unsigned list_len) {
    cout << "[";
    for (unsigned i=0; i<list_len-1; i++) {
        cout << list[i] << " ";
    }
    cout << list[list_len - 1] << "] ";
}

void printCoins(unsigned* coinsUsed, unsigned change) {
   unsigned coin = change;
   unsigned thisCoin;
   while (coin > 0) {
      thisCoin = coinsUsed[coin];
      cout << thisCoin << endl;
      coin = coin - thisCoin;
   }
}