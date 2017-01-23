(FRENCH Version below)
Here a quick explanation about kmeans and self-organizing kohonen maps:

Context: During a study in CentraleSupélec (Engineering School France) we've implemented two algorithms of unsupervized classification. First of all the problem is to class hand-written figures (10 figures from 0 to 9). The problem is unsupervized, that means figures in the dataset are not labeled and the algorithm never know apriori which number is an image from the training set. First algorithm is K-means online, second is self-organizing maps of Kohonen.

K-means allows, given hyperparameter called K, to create K prototypes that could be labeled after a certain amount of learning and that can be used to classify new entries from a dataset. We choose arbitrary a distance from a prototype to a dataset entry. That distance evluates the difference between the prototype and the new entry. After evaluating the distances between the entry and each prototypes we can choose a prototype we'll call the elected prototype (the closest to the entry considering the given distance).

Then we closen the elected prototype to the entry by modifying the values of the prototype pixels according to a learning rate called alpha.
Finally we repeat the previous operations.

The two main conditions to have the K-means algorithm working are:
- A good initialization (see the differences between the beginning of kmeans.avi where we initialize the prototypes by random images and the end where we initialize the prototypes by entries from the training set);
- A reasonable number of prototypes (K), for example there will probably be multiple way of writing a 9 because of people sauf considering only one prototype per figure would be to simplistic (that happens as soon as the class are not convex according to the given distance).

The differences between K-means and Kohonen are firt the design of another distance bewteen prototypes (now we know why it's called "maps"). When a prototype is elected according to the first distance we described before we make that prototype learning but also the prototypes close to the elected prototype according to the second distance (inter prototypes). The learning rate decreases when the distance from one prototype to the elected prototypes increases.

The easiest to understand is to watch the kohonen.avi

If you want to re-execute the code, download the repository, open a terminal and type the following:
>>> g++ -o Main -Wall -ansi -O3 main.cpp
>>> ./Main
>>> avconv -i kmeansGrid-%06d.ppm -b:v 1M kmeanOwnVideo.avi

#----------------------------#

FRENCH VERSION:
Voici une petite explication à propos de kmeans et les cartes auto-organisatrices de kohonen :

Le contexte : nous avons effectué dans le cadre d’un bureau d’étude une implémentation de deux algorithmes de classification non-supervisé. Le problème de classification est le suivant : on veut classer des chiffres (10 chiffres de 0 à 9) écrits à la main. Le problème n’est pas supervisé car nous ne savons jamais si tel chiffre de notre base est effectivement tel ou tel autre chiffre. Il s’agissait de l’algorithme des K-means dans sa version online et de l’algorithme des cartes de Kohonen. Pour (r)appel, le premier permet sur la base d’un paramètre K donné a priori de définir K prototypes (ou pré-classes). On se donne une distance qui permet de déterminer la « différence » entre un prototype et un élément tiré de notre base de donnée. Pour chaque élément de notre base de donnée d’entraînement, appelé ici échantillon, on élit le prototype le plus proche de notre échantillon et on « rapproche » le prototype de l’échantillon au sens de la distance qu’on s’est défini d’un coefficient alpha qu’on se donne (appelé ici coefficient d’apprentissage).

Les deux principales conditions à respecter pour faire fonctionner correctement l’algorithme des K-means online sont :
- Une bonne initialisation (voir la différence de comportement de l’algorithme entre le début de la video kmeans.avi où on initialise les prototypes de manière aléatoire et la fin de la vidéo où on initialise les prototypes par des éléments de la base d’entraînement)
- Un nombre de prototypes K suffisant (dans le problème considéré on s’attache à classifier des chiffres écrits à la main, il serait beaucoup trop réducteur de considérer : K=10 parce qu’il y a 10 chiffres donc 10 classes. Dès lors que toutes les classes ne sont pas convexes au sens de la distance qu’on s’est défini).

La différence avec l’algorithme des cartes de Kohonen est d’abord la définition d’une seconde distance entre les prototypes (d’où le nom de « carte » de Kohonen). Lorsqu’un prototype est élu au sens de la proximité de la première distance qu’on s’est donnée (entre un échantillon et un prototype) on ne se contente non pas seulement de rapprocher le prototype élu de l’échantillon suivant un coefficient d’apprentissage mais également les prototypes « proches » du prototype élu au sens de la deuxième distance qu’on s’est défini avec un coefficient d’apprentissage qui dépend de la valeur de cette distance (plus un prototype est éloigné du prototype élu moins il « apprend »).

Le plus simple est encore de constater ce que ça donne en image avec la vidéo kohonen.avi ci-jointe.

Si vous voulez ré-executer le code, télécharger le repo Git, ouvrez un terminal et tappez les commandes suivantes :
>>> g++ -o Main -Wall -ansi -O3 main.cpp
>>> ./Main
>>> avconv -i kmeansGrid-%06d.ppm -b:v 1M kmeanOwnVideo.avi

#----------------------------#

Src:
http://archive.ics.uci.edu/ml/ for the dataset
