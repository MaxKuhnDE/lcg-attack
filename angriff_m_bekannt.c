#include <stdio.h>

long ggt(long a, long b) {
    //groesster gemeinsamer Teiler (Euklid)
    if (b == 0) return a;
    return ggt(b, a % b);
}

long modInverse(long a, long m) {
    //Modulo Inverse berechnen
    long m0 = m, y = 0, x = 1;
    if (m == 1) return 0;
    while (a > 1) {
        long q = a / m;
        long t = m;
        m = a % m, a = t;
        t = y;
        y = x - q * y;
        x = t;
    }
    if (x < 0) x += m0;
    return x;
}

int main() {
    long m, x0, x1, x2, x3;
    long a, b; // hier werden die Zwischenwerte gespeichert

    printf("Modulus (m) eingeben: ");
    scanf("%ld", &m);
    printf("Erster Wert (x0): ");
    scanf("%ld", &x0);
    printf("Zweiter Wert (x1): ");
    scanf("%ld", &x1);
    printf("Dritter Wert (x2): ");
    scanf("%ld", &x2);
    printf("Vierter Wert (x3): ");
    scanf("%ld", &x3);

    // Differenzen bilden
    long diff0 = (x1 - x0 + m) % m;
    long diff1 = (x2 - x1 + m) % m;
    long d = ggt(diff0, m);

    if (d == 1) {
        // a berechnen mit modInverse
        long inv = modInverse(diff0, m);
        a = (diff1 * inv) % m;
        // Formel umgestellt um b zu finden
        b = (x1 - (a * x0) % m + m) % m;
    } else {
        // bel. modulo
        long m_neu = m / d;
        long inv = modInverse(diff0 / d, m_neu);
        long a_base = ((diff1 / d) * inv) % m_neu;

        for (int k = 0; k < d; k++) {
            long a_test = a_base + (k * m_neu);
            long b_test = (x1 - (a_test * x0) % m + m) % m;

            if ((a_test * x2 + b_test) % m == x3) {
                a = a_test;
                b = b_test;
                break;
            }
        }
    }

    printf("\nAngriff erfolgreich!\n");
    printf("Gefundenes a: %ld\n", a);
    printf("Gefundenes b: %ld\n", b);
    return 0;
}
