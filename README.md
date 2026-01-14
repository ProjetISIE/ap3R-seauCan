# Projet Réseau CAN – SocketCAN & C++ 
## Description
Ce projet s’inscrit dans le cadre d’un travail pratique portant sur la mise en œuvre d’une communication **CAN** sous **GNU/Linux** à l’aide de la bibliothèque **SocketCAN** et du langage **C++**.

Il a pour objectif de manipuler des trames CAN, de valider leur format et de faciliter leur exploitation via une approche orientée objet.

Le projet inclut notamment le développement d’une classe `CanMessage`, permettant de représenter, valider et afficher des messages CAN, ainsi que des tests unitaires assurant sa fiabilité.

## Objectifs
- [ ] Comprendre le fonctionnement d’un réseau CAN sous GNU/Linux
- [ ] Configurer et utiliser un dongle CANUSB
- [ ] Utiliser les outils SocketCAN (`candump`, `cansend`)
- [ ] Concevoir une classe C++ représentant un message CAN
- [ ] Valider le format des trames CAN
- [ ] Tester le code à l’aide de tests unitaires

## Prérequis
- Système GNU/Linux
- Compilateur g++ compatible C++23
- Bibliothèque SocketCAN
- Outils SocketCAN :
  - `can-utils`
- Framework de tests unitaires doctest
- Un dongle CANUSB (pour les tests matériels)

## Structure du projet
```StrucProjet
├── Test
└── README.md

```

