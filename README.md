# 3D_scanner
Le but de ce projet est d'automatiser la prise de photo d'un object leger sur un socle. et d'envoyer les photos sur un ordinateur assez puissant pour faire tourner un logiciel de modelisation 3d basé sur les photographies.


differentes taches : 

Mouvement :
  electronique :
    - faire tourner un moteur pas a pas avec un raspberry pico,                             (obj : tourner l'object central)
    - faire tourner un moteur dc avec un raspberry pico avec un system de boutons stopper.  (obj : change la position de la camera)
    - faire tourner un servo moteur avec un raspbery pico.                                  (obj : change l'orientation de la camera)
    remarque : les moteurs n'ont pas a fonctionner en parrallele.
  mecanique :
    - modeliser bras qui tiens la camera et le servo moteur.                                (obj : change la position et orientation de la camera)
    - modeliser le support pour le bras et les boutons stoppeur.                            (obj : change la position)
    - modeliser le support de l'objet.                                                      (obj : tourner l'object central)
  code : 
    - écrire une api sur le pico utilisable par le pi (en spi)
    
Communication :
  electronique :
    - implementer une connection uart
   code : 
    - transmettres data a un ordinateur en wifi                                             (obj : delivrer photo)
    - retransmission de l'object 3d sur un server.                                          (obj : delivrer 3d)
    - nettoyage des datas. en cas de succees.                                               (obj : delivrer 3d)
    
Data : 
  electronique : 
    - prendre les photos depuis le raspberry pi                                            (obj : prise de donnée)
  code : 
    - utiliser du logiciel Meshroom pour reconstitution 3d.                                 (obj : produire 3d)

Productivité : 
  - trouver pour chaque objectif des valeurs refletant la qualité de l'objectif atteint. exemple : prise de donné : temps necessaire, qualité de l'image 
  - implementer une note sur les quatres facteurs de accelerate : lead time, nombre de deploiement  time to recover, TX d'echec (total deploiement / breaking one)
  - utiliser une architecture logiciel en quatre couche (architecture decouplée) 
  - imposer une couverture de test d'au moins 80 % pour les units tests. écrire integration test, et  end to end test. (test automatisée) 
  - ecrire une CI et CD. (intégration et déploiement continus)
  - monitoring

list materielle :
- moteur dc JGB37 24v 45 rpm.
- servo motor  MG995
- stepper motor nema 23 + dm556 20-50 v
- camera OV5647
- raspberry pico
- rasbperry pi 3b 
- alimentation 24 volt,
- limiteur LM2596 dc-dc

j'ai besoin d'un code pour le pico, un code pour le pi, un code pour l'ordinateur. tout vas etre ecrit en c++. 
l'utilisation de bibliotheque pour simplifier le code est aussi de mise. 
le pico controle les moteur, le pi controle les prises de photo et command le pico, l'ordinateur produit les images 3d et les places sur un server. 
