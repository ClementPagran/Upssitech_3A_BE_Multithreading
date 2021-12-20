#!/bin/sh
xterm -hold -e "./bin/service1.exe" &
sleep 0.5
xterm -hold -e "./bin/capteur.exe" &
sleep 0.5
xterm -hold -e "./bin/service2.exe" &
sleep 0.5
xterm -hold -e "./bin/safety.exe" 
