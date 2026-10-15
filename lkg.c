#include <stdio.h>
int main() {
    long a, b, m, x; //die Variablen, jeweils unten nochmal erklärt
    int anzahl;

    printf("Multiplikator (a): ");
    scanf("%ld", &a);
    printf("Inkrement (b): ");
    scanf("%ld", &b);
    printf("Modulus (m): ");
    scanf("%ld", &m);
    printf("Startwert (Seed): ");
    scanf("%ld", &x);
    printf("Anzahl Sequenzen: ");
    scanf("%d", &anzahl);

    //Schleife für Sequenz
    for (int i = 0; i < anzahl; i++) {
        //Formel: X = (a * X + b) mod m
        x = (a * x + b) % m;
        // Wegen Hex aus Byte
        int byte = x % 256;

        printf("Byte %d: \n", i + 1); //Ausgabe in Format
        printf("Intern %ld | Byte: %d (%02X)\n", x , byte, byte); //Ausgabe Intern
    }
    return 0;
}