sudo modprobe can
sudo modprobe can_raw
sudo modprobe can_dev
# sudo modprobe slcan
sudo slcand -c -s1 /dev/ttyUSB0 slcan0
sudo ip link set dev slcan0 up
printf "Utiliser candump pour lire\n"
printf "Utiliser cansend pour écrire\n"
