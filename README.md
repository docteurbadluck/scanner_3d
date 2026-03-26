# scanner_3d
Le but de ce projet est d'automatiser la prise de photo d'un object leger sur un socle. et d'envoyer les photos sur un ordinateur assez puissant pour faire tourner un logiciel de modelisation 3d basé sur les photographies.


differentes taches : 

Mouvement :
  electronique :
    - faire tourner un servo pas a pas avec un raspberry pico,                              (obj : tourner l'object central)
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
    - implementer une connection SPI ou le pi est maitre et pico executant.                 (obj : controle de la position)
    - connection SPI ou le pico qui gere les lumieres est aussi controller par le pi.       (obj : controle lumiere (optionnell)
   code : 
    - transmettres data a un ordinateur en wifi                                             (obj : delivrer photo)
    - retransmission de l'object 3d sur un server.                                          (obj : delivrer 3d)
    - nettoyage des datas. en cas de succees.                                               (obj : delivrer 3d)
Data : 
  electronique : 
    - prendre les photos depuis le raspberry pi                                            (obj : prise de donnée)
  code : 
    - utiliser un logiciel de reconstitution 3d.                                           (obj : produire 3d)
    
