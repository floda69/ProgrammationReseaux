# TP 2/3/4 - Programmation d’un serveur de Jeu Awalé
Objectifs
L’objectif du TP est de réaliser un serveur de jeu Awalé. Le but est d’avoir une application client/serveur qui permettent aux clients de jouer des parties, de vérifier que les règles sont biens appliquées, de communiquer, de garder des historiques (de score, de parties jouées etc.).

Le TP est incrémental, vous pouvez ajouter des fonctionnalités à votre application à mesure que votre implantation progresse : suivre une partie en tant qu’observateur, organisation de tournois, discussions de groupe,
enregistrement des discussions, cryptage point à point, les seules limites sont celles de votre imagination et du temps que vous allez y consacrer.

Votre travail sera jugé sur une démonstration (prévoir 10 min) et sur le code que vous fournirez lors du dernier TP. Il faut privilégier une application robuste qui fonctionne pluôt que de multiplier les fonctionnalités qui ne fonctionnent pas. Le fait que le code soit évolutif fait aussi partie de ce qui est demandé: est il facile de rajouter de nouvelles fonctionnalités ?

Description du travail attendu
Vous réaliserez votre implantation de manière incrémentale. Il est primordial de bien réfléchir aux aspects architecturaux avant toute implantation: que se passe-t-il sur le serveur ? sur les clients ? quelle est votre politique de communication ? Quels sont les différents protocoles : pour les parties, pour les discussions, etc. Comment les clients peuvent s’appeler/se défier entre eux (juste par leur nom ? autrement ?) ? Quels sont vos choix pour que votre code soit évolutif ? Les tests de votre application sont importants: que se passe t il si un client se déconnecte ? Peut il récupérer la partie ? etc…

Vous pouvez partir de l’exemple donnez en cours 1 dans lequel le serveur rediffuse à tous les clients un message envoyé par un client: Client Serveur V2 dans Exemples - Cours 1

Le jeu d’Awalé
Les règles du jeu Awalé sont simples et peuvent se trouver ici: article wikipedia Awalé

Il faut que vous implantiez ce jeu c’est à dire qu’il faut être capable de représenter le plateau de jeu, de vérifier la légalité des coups et d’adjuger la victoire. D’autre part il faudra pouvoir sauvegarder et diffuser des parties, aux joueurs bien sûr, mais aussi par exemple à des observateurs. Juste une sortie ASCII est suffisante (on pourrait faire une belle interface graphique par dessus mais ce n’est pas le but du TP).

Guide schématique de l’implantation
Implanter le jeu d’Awalé : représentation interne, comment jouer un coup, compter les points, sauvegarder une partie, imprimer l’état du plateau etc.
Concevez une application client/serveur. Chaque client s’inscrit avec un pseudo.
Chaque client peut demander au serveur la liste des pseudos en ligne.
Un client A peut défier un autre client B. B peut accepter ou refuser.
Quand une partie est créée entre A et B, le serveur décide de qui commence par tirage au sort. Le serveur vérifie la légalité des coups joués (en utilisant le code créé pour 0).
Si vous avez une version qui fonctionne pour une partie, Vérifiez que cela fonctionne pour plusieurs parties en jeu. Vous pouvez ajouter comme fonctionalité un listing des parties en cours, et un mode “observateur” pour lequel le serveur envoie le plateau et le score à C qui observe la partie entre A et B.
Implanter une option de chat : les joueurs en plus d’envoyer des coups à peuvent échanger des messages pour discuter (dans et en dehors d’une partie).
Donnez la possiblité à un joueur d’écrire une bio : disons 10 lignes en ASCII, pour se présenter. On peut demander au serveur de nous afficher la bio d’un pseudo particulier.
Ajouter un mode privé: un joueur peut limiter la liste des observateurs à une liste d’amis. Implanter cette fonctionalité.
Ajouter la fonctionnalité de sauvegarde de partie jouée pour pouvoir la regarder par la suite.
Libre à vore imagination: classement des joueurs : article wikipedia sur le classement elo, organisation de tournois, adapter pour un autre jeu etc.
Environnement de programmation
Le programme sera fait en C. Pensez à répartir vos fonctions dans différents fichiers et à produire un Makefile pour la compilation. Pensez bien aux structures de données que vous allez utiliser. Vous pouvez partir des exemples vus dans les TP précédents ou bien en cours. Le but est de réaliser un serveur qui permette à deux clients de jouer une partie d’Awalé. Le serveur vérifie la validité des coups et compte les points.

Partez d’une base client serveur déjà programmée (au TP précédent ou bien en reprenant l’exemple du cours cité avant).

Vous enverrez tous les fichiers sources nécessaires pour compiler ainsi que comment les compiler. Vous incluerez aussi un manuel de l’utilisateur (quelles fonctionalités sont implantées par l’application et comment les utiliser pour le client et le serveur). Le tout sera envoyé par mail à frederic.prost@insa-lyon.fr avec pour sujet [4IF-PR-TP-CR].

Une présentation de 10min sera a prévoir pendant le dernier TP.
