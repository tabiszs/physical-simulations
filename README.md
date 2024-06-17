# spring

## Czarna dziura
Projekt na przedmiot Modelowanie Geometryczne 2

### Opis

Celem projektu jest stworzenie modelu czarnej dziury poprzez zastosowanie efektu soczewki grawitacyjnej. 
Efekt ten realizowano wyznaczając promień Schwarzschilda i ugięcie promieni wokół punktowej masy.

Z analizy zachowania promieni nadlatujących z nieskonczoności z dużym parametrem zderzenia jasno wynika, 
że wszystkie ciała powodują pewne ugięcie promieni świetlnych. Efekt ten ma duże znaczenie, 
ponieważ ugięcie promieni świetlnych przebiegających w pobliżu Słońca 
jest jednym z najważniejszych eksperymentalnych testów ogólnej teorii względności. 
Ugięcie promieni przez galaktyki jest podstawą działania soczewek grawitacyjnych, 
które omówimy w następnym rozdziale. Wielkoscią, która nas interesuje, 
jest kąt ugięcia δφu. Można go wyznaczyć obliczając całkę, gdzie 
- M to masa ciała, 
- b to odgległość promienia od ciała
- w - infinitezymalny kąt

![equation](https://github.com/tabiszs/whirligig/assets/92331225/b318b56d-2cb4-4780-9ac7-7f57e6a4f20c)

![angle](https://github.com/tabiszs/whirligig/assets/92331225/44d4f17b-7e64-4686-b271-5750ee5ae04a)

### Algorytm

1. Raycasting
2. Wyznaczenie odległości między rzuconym promieniem a czarną dziurą
3. Wyznaczenie przedziału całkowania kąta ugięcia
4. Obliczenie kąta ugięcia
5. Wyznaczenie nowego kierunku promienia po obrocie o wyznaczony kąt wokół osi prostopadłem do rzucango promienia


### Linki
- Inspiracja: GitHub - tomasz-herman/BlackHole
- Źródło: James B. Hartle - GRAWITACJA str. 235


