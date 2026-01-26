sudo modprobe can
sudo modprobe vcan
# sudo modprobe slcan
sudo ip link add dev slcan0 type vcan
sudo ip link set up slcan0
printf "Utiliser candump pour lire\n"
printf "Utiliser cansend pour écrire\n"
printf "Pour déconnecter: sudo ip link delete slcan0\n"
