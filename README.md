# IPK Calculator Protocol Client

Tento projekt implementuje klienta pre IPK Calculator Protocol (IPKCP) v jazyku C. Klient dokáže komunikovať s akýmkoľvek serverom používajúcim IPKCP.

## Inštalácia

Pre kompiláciu projektu vykonajte nasledujúci príkaz: make

Po kompilácii sa vytvorí spustiteľný súbor `ipkcpc` v koreňovom adresári projektu.

## Použitie

Spustite klienta pomocou príkazu: ipkcpc -h <host> -p <port> -m <mode>

Kde `<host>` je IPv4 adresa servera, `<port>` je port servera a `<mode>` je buď `tcp` alebo `udp`.

## Licencia

Tento projekt je chránený licenciou, pozrite si súbor [LICENSE](LICENSE) pre viac informácií.

## Zmeny a obmedzenia

Podrobnosti o zmene a obmedzeniach nájdete v súbore [CHANGELOG.md](CHANGELOG.md).

## Testovanie

Testovanie bolo vykonané na zostave:

Procesor: AMD Ryzen 5 5600H with Radeon Graphics, 3.30 GHz

Operačný systém: Windows 11 Pro 22H2

Jazyk prevedenia: C

PC bol pripojený cez Ethernet na kolejnet.

Boli vykonané základné testy podľa zadania: veľké hodnoty (kladné a záporné), 
desatinné čísla a chybné vstupy. Program sa dá kedykoľvek vypnúť signálom CTRL + C podľa zadania. 
Testy boli vykonané ako lokálne, tak aj pomocou SSH pripojenia na Merlinovi.

## Zdroje
Primárnym zdrojom mi boli slidy k prednáškam a dokumentácie k použitým knižniciam 
Zoznam sekundárnych zdrojov je uvedený nižšie:

Webstránky:

TCP komunikácia: https://www.geeksforgeeks.org/tcp-connection-termination/

UDP komunikácia: https://www.geeksforgeeks.org/user-datagram-protocol-udp/

Socket Programming in C/C++: https://www.geeksforgeeks.org/socket-programming-cc/
