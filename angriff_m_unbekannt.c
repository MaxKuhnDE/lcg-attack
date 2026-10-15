#include <stdio.h>
#include <stdlib.h>

long long ggt(long long a, long long b) {
    if (b == 0) return a;
    return ggt(b, a % b);
}

int main() {
    long long x[6];
    long long t[5];
    long long u[3];
    long long m;
    long long max_x = 0; // für Filter

    printf("Bitte 6 abgefangene Werte eingeben:\n");

    // 1. Werte einlesen
    for (int i = 0; i < 6; i++) {
        printf("Wert %d: ", i);
        scanf("%lld", &x[i]);
        if (x[i] > max_x) {
            max_x = x[i];
        }
    }
    // 2. Differenzen (T_n) berechnen: T_n = X_(n+1) - X_n
    for (int i = 0; i < 5; i++) {
        t[i] = x[i + 1] - x[i];
    }

    // 3. U-Werte berechnen: U_n = T_(n+2) * T_n - (T_(n+1))^2
    for (int i = 0; i < 3; i++) {
        u[i] = t[i + 2] * t[i] - t[i + 1] * t[i + 1];
    }

    // 4. Modulus m ist der ggT von den U-Werten
    m = ggt(llabs(u[0]), ggt(llabs(u[1]), llabs(u[2])));

    for (long long k = 2; k <= 100; k++) {
        while (m % k == 0 && (m / k) > max_x) {
            m /= k; // m wird durch k geteilt und überschrieben
        }
    }
    printf("Der unbekannte Modulus (m) ist: %lld\n", m);

    return 0;
}
