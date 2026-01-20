# Projet Réseau CAN – SocketCAN & C++ 
## Description
Ce projet s’inscrit dans le cadre d’un travail pratique portant sur la mise en œuvre d’une communication **CAN** sous **GNU/Linux** à l’aide de la bibliothèque **SocketCAN** et du langage **C++**.

Il a pour objectif de manipuler des trames CAN, de valider leur format et de faciliter leur exploitation via une approche orientée objet.

Le projet inclut notamment le développement d’une classe `CanMessage`, permettant de représenter, valider et afficher des messages CAN, ainsi que des tests unitaires assurant sa fiabilité.

## Objectifs
- [x] Comprendre le fonctionnement d’un réseau CAN sous GNU/Linux
- [x] Configurer et utiliser un dongle CANUSB
- [x] Utiliser les outils SocketCAN (`candump`, `cansend`)
- [x] Concevoir une classe C++ représentant un message CAN
- [x] Valider le format des trames CAN
- [x] Tester le code à l’aide de tests unitaires

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
├── include
│   └── CanManager.hpp
│   └── CanMessage.hpp
│   └── slcanInterface.hpp
├── src
│   └── CanMessage.cpp
├── test
│   └── CMakeLists.txt
│   └── CanManager.cpp
│   └── CanMessage.cpp
│   CMakeLists.txt
└── README.md
```

## Classe CanMessage
La classe CanMessage permet de représenter un message CAN et de fournir des services associés. 

Voici le diagramme associé :

<img width="676" height="370" alt="image" src="https://github.com/user-attachments/assets/956d2cbd-add7-4bbd-8cd1-158459160475" />

## Matériel utilisé
- Dongle CANUSB
- Oscilloscope avec décodeur CAN (ex. PicoScope)
- Nœuds CAN de test

