# Guide Utilisateur - Jeu Awalé Client-Serveur

Bienvenue dans l’application de jeu Awalé ! Ce guide explique comment utiliser les fonctionnalités disponibles pour jouer, observer des parties, discuter avec d'autres joueurs, etc.

## 1 - Introduction

Awalé est un jeu de stratégie, cette application permet de jouer à ce jeu en ligne via un système client-serveur. Le serveur permet de :
- Jouer contre d’autres joueurs.
- Afficher les joueurs et les parties en cours.
- Gérer une liste d'amis.
- Observer des parties en cours.
- Limiter les observateurs à votre liste d'amis
- Participer à des discussions globales ou privées.
- Modifier et afficher votre bio.
- Enregistrer une partie dans un fichier
- (non fonctionnel) Afficher une partie enregistrée.

---

## 2 - Démarrage
### Compilation
Placez-vous dans le répertoire Awale.
Utilisez le `Makefile` pour compiler l’application :
```bash
 make
```   
### Lancer le serveur
Démarrez le serveur en exécutant :
```bash
 ./serveur/serveur_exec
```
### Lancer le client
Démarrez un client en exécutant :
```bash
./client/client_exec <IP> <name>
```
---
## 3 - Commandes Disponibles

### Commandes Générales
- **`/disconnect`** : Déconnecte le client du serveur.
- **`/players`** : Affiche la liste des joueurs connectés.
- **`/games`** : Affiche la liste des parties en cours.
- **`/bio <pseudo>`** : Affiche la bio d'un joueur.
- **`/setbio <texte>`** : Définit ou met à jour votre bio (200 caractères maximum).
- **`/mkfriend`** : Ajoute un ami à sa liste persistante d'amis.

### Commandes de Chat
- **`/switch chat mode`** : Bascule entre le mode de chat global et le mode de chat privé. Par défaut un client est en mode global et reçoit donc tous les messages, en mode privé il recevra uniquement les messages privés.
- **`/global <message>`** : Envoie un message dans le chat global.
- **`/private <pseudo> <message>`** : Envoie un message privé à un joueur spécifique.

### Commandes de Jeu
- **`/defy <pseudo>`** : Défie un joueur pour une partie.
- **`/accept`** : Accepte une invitation à jouer.
- **`/decline`** : Refuse une invitation à jouer.
- **`/play <case>`** : Joue un coup lorsque vous êtes dans une partie, le coup est un entier entre 0 et 5.
- **`/spectate <joueur>`** : Observe la partie en cours d'un joueur.
- **`/resign`** : Abandonne la partie en cours.
- **`/switch privacy`** : Fait passer du mode privé à publique et réciproquement. Seuls les amis peuvent observer une partie privée.
- **`/save`** : Définit un fichier de sauvegarde.
- **`/load save`** : Affiche le contenu du fichier de sauvegarde défini.
---
## 4 - Affichage du jeu
Pendant une partie, le jeu s'actualise en temps réel à chaque coup joué.
Le plateau s'affiche dans l'invite de commande comme suit :

```bash
Plateau :
joueur1
	 4  4  4  4  4  4 
  0				 0
	 4  4  4  4  4  4 
				joueur2
```
Le jeu demande au joueur dont c'est le tour de choisir une case de 0 à 5, les cases sont ordonnées de gauche à droite pour les 2 joueurs. Les scores sont affichés aux extrémités du plateau, du côté correspondant au joueur.
```c
	     0  1  2  3  4  5 
  score j1			score j2
	     0  1  2  3  4  5 
```
Enfin, le jeu tourne dans le sens **trigonométrique / antihoraire**. 
Par exemple, si c'est au joueur 1 de faire un coup, et qu'il entre la commande `/play 1` alors nous auront le plateau suivant : 
```bash
Plateau :
joueur1
	 5  0  4  4  4  4 
  0				 0
	 5  5  5  4  4  4 
				joueur2
```
