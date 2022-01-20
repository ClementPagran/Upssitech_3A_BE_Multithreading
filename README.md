# Upssitech_3A_BE_Multithreading
Se placer dans le dossier, faire `make` puis lancer DANS CET ORDRE :

1 : `./bin/service1.exe nominal`

2 : `./bin/capteur.exe` (et ne plus jamais y toucher)

3 : `./bin/service2.exe` (et ne plus jamais y toucher)

4 : `./bin/watchdog.exe` (et ne plus jamais y toucher)

On peut ensuite tuer le service 1 (`^C`), mais à la reprise, executer `./bin/service1 resume` (rien ne fonctionnera en cas d'oubli).

Nettoyage : `make clean`

`ipcrm -a` à chaque fois que l'on arrête les 4 programmes pour nettoyer les IPC toujours ouverts mais non utilisés par des processus.
