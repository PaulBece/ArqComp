#include <iostream>
#include <bitset>

bool fullAdder(bool b1, bool b2, bool& carry)
{
    bool sum = (b1 ^ b2) ^ carry;
    carry = (b1 && b2) || (b1 && carry) || (b2 && carry);
    return sum;
}

template <typename T>
T bitsetAdd(T& x, T& y,int n)
{
    bool carry = false;

    T ans;
    for (int i = 0; i < n; i++) {
        ans[i] = fullAdder(x[i], y[i], carry);
    }
    return ans;
}

bool fullSubtractor(bool b1, bool b2, bool& borrow)
{
    bool diff;
    if (borrow) {
        diff = !(b1 ^ b2);
        borrow = !b1 || (b1 && b2);
    }
    else {
        diff = b1 ^ b2;
        borrow = !b1 && b2;
    }
    return diff;
}

template <class T>
T bitsetSubtract(T& x, T& y,int n)
{
    bool borrow = false;

    T ans;
    for (int i = 0; i < n; i++) {
        ans[i] = fullSubtractor(x[i], y[i], borrow);
    }
    return ans;
}

template <class T>
void arithmeticShiftRight(T& A, T& Q, bool& q, int n) {
    q = Q[0];
    bool tmp = A[0];
    Q >>= 1;
    A >>= 1;
    Q[n - 1] = tmp;
    A[n - 1] = A[n - 2];
}

template <typename T>
void multi_enteros_signo(T& A, bool& q, T& M, T& Q, int n) {
    int nn = n;
    do {
        if (Q[0] == 1) {
            if (q == 0) {
                A = bitsetSubtract<T>(A, M, nn);
                arithmeticShiftRight<T>(A, Q, q, nn);
            }
            else {
                arithmeticShiftRight<T>(A, Q, q, nn);
            }
        }
        else {
            if (q == 1) {
                A = bitsetAdd<T>(A, M, nn);
                arithmeticShiftRight<T>(A, Q, q, nn);
            }
            else {
                arithmeticShiftRight<T>(A, Q, q, nn);
            }
        }
        } while (--n > 0);
}

int main() {
    const int n = 32; //n bits

    int Multiplicand = -4;
    int Multiplier = -2;


    std::bitset<n> A = 0;
    std::bitset<n> M = Multiplicand;
    std::bitset<n> Q = Multiplier;
    bool q = 0;

    std::cout <<"Cantidad de bits = "<<n<<"\n" << "M = " << Multiplicand << " = " << M << "\nQ = " << Multiplier << " = " << Q << "\n-----------------------------------------\n";

    multi_enteros_signo<std::bitset<n>>(A, q, M, Q, n);
    
    std::cout <<"A: " << A << "\n" << "Q: " << Q<<"\n";

    return 0;
}