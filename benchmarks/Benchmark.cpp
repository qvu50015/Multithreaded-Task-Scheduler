#include <iostream>
#include <chrono>

bool isPrime(int n)
{
    if (n < 2) {
        return false;
    }

    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }

    return true;
}

int countPrimes(int start, int end)
{
    int count = 0;

    for (int i = start; i < end; i++) {
        if (isPrime(i)) {
            count++;
        }
    }

    return count;
}

int main()
{
    auto start = std::chrono::steady_clock::now();
    int result = countPrimes(2, 5000000);
    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Primes: " << result << '\n';
    std::cout << "Sequential: " << duration.count() << " ms\n";

    return 0;
}