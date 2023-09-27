#include <iostream>
#include <bitset>
using namespace std;

bool fullAdder(bool b1, bool b2, bool& carry)
{
    bool sum = (b1 ^ b2) ^ carry;
    carry = (b1 && b2) || (b1 && carry) || (b2 && carry);
    return sum;
}

template <typename T>
T bitsetAdd(T& x, T& y, int n)
{
    bool carry = false;

    T ans;
    for (int i = 0; i < n; i++) {
        ans[i] = fullAdder(x[i], y[i], carry);
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
void multi_enteros_sin_signo(T& A, bool& C, T& M, T& Q, int n) {
    int nn = n;
    do {
        if (Q[0] == 1) {
            A = bitsetAdd<T>(A, M, nn);
            arithmeticShiftRight<T>(A, Q, C, nn);
            
        }
        else {
            
            arithmeticShiftRight<T>(A, Q, C, nn);            
        }
    } while (--n > 0);
}

int main() {

    //variables a multiplicar
	float f = 3.22;
	float f2 = -753.159;

	bitset<sizeof f * 8> a= bitset<sizeof f * 8>(*(long unsigned int*)(&f));
	bitset<sizeof f2 * 8> b = bitset<sizeof f2 * 8>(*(long unsigned int*)(&f2));

    cout << "Numero flotante " << f << " en binario:\n" << a << endl;
    cout << "Numero flotante " << f2 << " en binario:\n" << b << endl;

    //primera variable dividida en 3 partes
	bitset < 1 > signo;
	signo[0] = a[31];
	bitset<32> exponentes;
	for (int i = 0; i < 8; i++)
		exponentes[i] = a[23 + i];
	bitset<32> significando;
	for (int i = 0; i < 23; i++)
		significando[i] = a[i];
    significando[23] = 1;

    //segunda variable dividida en 3 partes
	bitset < 1 > signo2;
	signo2[0] = b[31];
	bitset<32> exponentes2;
	for (int i = 0; i < 8; i++)
		exponentes2[i] = b[23 + i];
	bitset<32> significando2;
	for (int i = 0; i < 23; i++) 
		significando2[i] = b[i];
    significando2[23] = 1;

    //variables para dar la respuesta
	bitset<32> res;
	bitset < 1 > signo3;	
	bitset<32> exponentes3;	
	bitset<32> significando3;

    //variables auxiliares
    bitset < 32 > bias = -127;
    bitset < 32 > one = 1;
    bitset<32> A = 0;
    bool C = 0;
	
	if (a == 0 || b == 0)   //verificamos si una variables es 0
		res = 0;
	else {
        signo3[0] = signo[0] ^ signo2[0];   //asignamos el signo correcto a la respuesta
		exponentes3 = bitsetAdd<bitset<32> >(exponentes, exponentes2, 32);  //sumamos exponentes
		exponentes3 = bitsetAdd<bitset<32> >(exponentes3, bias, 32);    //restamos el bias

        if (exponentes3[31])
            cout << "Underflow de exponentes"<<endl;    //comprobamos si hay underflow
        else if (exponentes3[8])
            cout << "Overflow de exponentes"<<endl;     //comprobamos si hay overflow
        else {

            multi_enteros_sin_signo<bitset<32> >(A, C, significando, significando2, 32);    //multiplicamos los significandos
            bitset<48> aux;
            for (int i = 0; i < 32; i++) {      //guardamos los valores de la multiplicacón en la variable aux
                aux[i] = significando2[i];
            }
            for (int i = 0; i < 16; i++) {
                aux[i + 32] = A[i];
            }


            int count = 0;
            while (count < 48) {
                if (aux[47 - count]) break;
                count++;
            }
            if (!count) exponentes3 = bitsetAdd<bitset<32> >(exponentes3, one, 32);     //normalizamos los exponentes y significandos
            for (int i = 0; i < 23; i++)
                significando3[22 - i] = aux[46 - i - count];

            res[31] = signo3[0];    //asignamos los valores de signo, exponentes y significando a la respuesta
            for (int i = 0; i < 8; i++)
                res[30 - i] = exponentes3[7 - i];
            for (int i = 0; i < 23; i++)
                res[i] = significando3[i];
        }
	}
    cout << "Multiplicacion en binario:\n" << res << endl;  //se imprime la respuesta

	return 0;
}