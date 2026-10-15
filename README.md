# LCG-Angriff

Ein linearer Kongruenzgenerator (LKG, engl. LCG) in C und zwei Angriffe, die aus beobachteten Zufallswerten die geheimen Parameter des Generators zurückrechnen. Entstanden im Praktikum 1 im Fach Kryptografie (Sommersemester 2026, Hochschule Albstadt-Sigmaringen).

Der Generator berechnet

```
X(n+1) = (a · X(n) + b) mod m
Byte   = X(n+1) mod 256
```

## Dateien

| Datei | Aufgabe | Inhalt |
|---|---|---|
| `lkg.c` | 1.1, 1.2 | Generator; a, b, m, Startwert und Anzahl der Werte werden eingegeben. Gibt den internen Zustand und das Byte (dezimal und hex) aus. |
| `angriff_m_bekannt.c` | 1.3 | Rekonstruiert a und b, wenn m bekannt ist, aus vier aufeinanderfolgenden Zuständen. |
| `angriff_m_unbekannt.c` | 1.4 | Rekonstruiert m aus sechs aufeinanderfolgenden Zuständen. |

Beide Angriffe arbeiten mit den internen Zuständen X(n), nicht mit den auf ein Byte gekürzten Ausgaben.

## Kompilieren und starten

```bash
gcc -o lkg lkg.c
gcc -o angriff_m_bekannt angriff_m_bekannt.c
gcc -o angriff_m_unbekannt angriff_m_unbekannt.c
./lkg
```

Alle Programme fragen ihre Eingaben interaktiv ab. Hinweis: `lkg.c` und `angriff_m_bekannt.c` rechnen mit `long`. Unter Linux/WSL ist das 64 Bit breit, unter Windows (MinGW, MSVC) nur 32 Bit; dort können bei großem m die Produkte überlaufen.

Beispiel (`lkg` mit a = 75, b = 74, m = 65537, Startwert 0):

```
Byte 1:
Intern 74 | Byte: 74 (4A)
Byte 2:
Intern 5624 | Byte: 248 (F8)
Byte 3:
Intern 28652 | Byte: 236 (EC)
...
```

## Aufgabe 1.2: Parametersätze

**Nur 0 und 1:** a = 1, b = 1, m = 2, Startwert 0 ergibt `1, 0, 1, 0, …`. Modulo 2 kann nur 0 oder 1 herauskommen, und mit b = 1 wechselt der Wert bei jedem Schritt.

**Maximale Periode:** a = 5, b = 3, m = 8, Startwert 0 ergibt `3, 2, 5, 4, 7, 6, 1, 0` und danach wieder `3, 2, …`. Alle 8 möglichen Werte kommen vor, die Periode ist also gleich m. Damit das klappt, müssen b und m teilerfremd sein. Vollständig beschreibt das der Satz von Hull und Dobell: Die Periode ist genau dann maximal, wenn

1. b und m teilerfremd sind,
2. a − 1 durch jeden Primfaktor von m teilbar ist,
3. a − 1 durch 4 teilbar ist, falls m durch 4 teilbar ist.

## Aufgabe 1.3: Angriff bei bekanntem m

Aus drei aufeinanderfolgenden Zuständen fällt b durch Differenzbildung heraus:

```
x1 = a·x0 + b   (mod m)
x2 = a·x1 + b   (mod m)
x2 − x1 = a·(x1 − x0)   (mod m)

a = (x2 − x1) · (x1 − x0)⁻¹   mod m
b = x1 − a·x0               mod m
```

Das modulare Inverse berechnet `modInverse()` mit dem erweiterten euklidischen Algorithmus.

Rechenbeispiel mit m = 65537 (Primzahl) und den Werten x0 = 0, x1 = 74, x2 = 5624:

```
x1 − x0 = 74          x2 − x1 = 5550
74⁻¹ mod 65537 = 9742
a = 5550 · 9742 mod 65537 = 75
b = 74 − 75 · 0 = 74
```

**Beliebiges m:** Das Inverse von (x1 − x0) existiert nur, wenn d = ggT(x1 − x0, m) = 1 ist. Bei einer Primzahl als Modulus ist das immer der Fall (solange x1 ≠ x0). Ist d > 1, teilt das Programm die Gleichung durch d und löst sie modulo m/d. Das liefert d Kandidaten

```
a = a_base + k · (m/d),   k = 0 … d−1
```

Den richtigen Kandidaten findet es, indem es mit dem vierten Wert prüft, ob `a·x2 + b ≡ x3 (mod m)` gilt.

Getestete Beispiele (Eingabe m, x0 bis x3):

| m | x0, x1, x2, x3 | gefundenes a | gefundenes b |
|---|---|---|---|
| 65537 | 0, 74, 5624, 28652 | 75 | 74 |
| 24262 | 0, 22, 242, 2442 | 10 | 22 |
| 45478 | 19041, 43736, 20801, 14450 | 4557 | 445 |
| 1000 | 217, 564, 851, 878 | 21 | 7 |
| 2503 | 59, 1045, 286, 2401 | 17 | 42 |
| 3253 | 1516, 2775, 1228, 1005 | 345 | 235 |

Bei m = 24262 ist ggT(22, 24262) = 2, hier greift also der Fall für beliebiges m.

## Aufgabe 1.4: Angriff bei unbekanntem m

1. **b eliminieren:** Differenzen `t(n) = x(n+1) − x(n)`. Es gilt `t(n+1) ≡ a · t(n) (mod m)`.
2. **a eliminieren:** `u(n) = t(n+2) · t(n) − t(n+1)²`. Einsetzen ergibt `u(n) ≡ a²·t(n)² − a²·t(n)² ≡ 0 (mod m)`, jedes u(n) ist also ein Vielfaches von m.
3. **m isolieren:** Der ggT von u(0), u(1) und u(2) ist ein Vielfaches von m, meistens m selbst.
4. **Prüfen:** Da m größer sein muss als jeder beobachtete Wert, teilt das Programm kleine Faktoren k = 2 … 100 heraus, solange das Ergebnis noch größer als der größte beobachtete Wert ist. So wird verhindert, dass nur ein Vielfaches des eigentlichen Modulus ausgegeben wird.

Mit dem gefundenen m lassen sich anschließend a und b mit `angriff_m_bekannt` bestimmen.

Getestete Beispiele (je 6 aufeinanderfolgende Zustände):

| erzeugt mit a, b, m | beobachtete Werte | gefundenes m |
|---|---|---|
| 75, 74, 65537 | 74, 5624, 28652, 51790, 17641, 12409 | 65537 |
| 4557, 445, 45478 | 19041, 43736, 20801, 14450, 42429, 22420 | 45478 |
| 21, 7, 1000 | 217, 564, 851, 878, 445, 352 | 1000 |
| 17, 42, 2503 | 59, 1045, 286, 2401, 811, 1314 | 2503 |
| 345, 235, 3253 | 1516, 2775, 1228, 1005, 2142, 794 | 3253 |

Beispiel für den kompletten Angriff: Der Generator mit a = 345, b = 235, m = 3253 und Startwert 32 liefert die Zustände 1516, 2775, 1228, 1005, 2142, 794. `angriff_m_unbekannt` findet daraus m = 3253, und `angriff_m_bekannt` mit m = 3253 und den ersten vier Werten liefert a = 345 und b = 235.

## Hinweis zur Entstehung

Die beiden Angriffsprogramme lagen nur noch als Screenshots in der Abgabe vor und wurden daraus übernommen. In `angriff_m_bekannt.c` sind die Funktion `ggt()`, die Fallunterscheidung `d == 1` und das Einlesen von x3 ergänzt, weil diese Teile im Screenshot nicht zu sehen waren; in `angriff_m_unbekannt.c` fehlte `#include <stdio.h>`. Alle Beispiele in den Tabellen oben wurden mit den Programmen in diesem Repository nachgerechnet.
