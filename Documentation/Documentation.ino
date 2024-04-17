/** \file documentation.ino
 *
 * \mainpage Plante Connecté
 * 
 * \brief Notre projet se portera sur un espace de culture connecté permettant d’irriguer de manière autonome, 
 * les plantes, en fonction du taux d’humidité de la plante.
 * 
 * Ci-dessous un prototype visuel que nous allons détailler :
 * \image html Prorotypevisuelle.png
 * 
 * Nous avons deux plantes différentes équipées de nos capteurs d’humidité et d’une pompe d’irrigation.
 * L’utilisateur peut configurer un taux d’humidité critique à ne pas dépasser pour chaque plante ainsi qu’un taux minimal qui correspond au besoin nutritif de la plante.
 * Si le taux d’humidité d’une plante est trop bas, alors cela active le système d’irrigation (représenté par la pompe), 
 * une fois le taux d’humidité régulé, la pompe se désactive. 
 * Une sécurité désactive la pompe si un taux critique d’humidité est détecté dans l’une des plantes de l’espace de culture.
 * Une interface ‘’utilisateur’’ permettra la lecture des données collectées et il pourra activer manuellement la pompe si besoin. 
 * 
 * \page 1 Liste des fonctionnalités
 * \brief Liste des fonctionnalités du projet
 * - WiFi
 * - MQTT / Adafruit
 * - Serveur Web
 * - Network Transport Protocol
 * - Ticker et/ou Time
 * - Interaction type IFTTT 
 * 
 * \page 2 Architecture
 * \brief Architecture du projet
 * \image html Architectureprojet.png
 * Dans cette image qui représente l'architecture de notre projet, nous pouvons remarquer :
 * - Les cartes Plante1, Plante2, Pompe ont un accès en "acces point" afin de configurer respectivement une connexion à un routeur en "mode station" via leur web serveur
 * - Les cartes esp32 communiquent avec le broker via internet une fois la connexion à un routeur disposant d'un accès sur le net [ Protocole MQTT ]
 * - Le broker transmet et récupère les informations du client AdafruitIO via le net [ Protocole MQTT ]
 * - Un client IFTT permet l'intéraction avec le client AdafruitIO afin d'éffectuer des action manuellement via le net [ Protocole HTTP ]
 * - Un Smartphone permet l'accès au client IFTT à distance via le net [ Protocole HTTP ]
 * 
 * \page 3 Diagrame de séquence
 * \brief Diagrame de séquence du projet
 * \image html Diagrammedesequanceiot.png
 * 
 * Les acteurs :
 * - POMPE
 * - PLANTE 1
 * - PLANTE 2
 * - BROKER
 * - ADAFRUIT
 * 
 * \brief Réglages et messages du projet
 * 
 * Tout d'abord, Adafruit envoie des requêtes d’abonnement (subscribe) au broker MQTT pour s’abonner à trois topics différents : ‘POMPE’, ‘Humidité1’, ‘Humidité2’, 
 * cela signifie que Adafruit souhaite recevoir en temps réel des données provenant de ces trois sources.
 * Dans un premier temps la pompe publie (publish) son état off au broker, la Plante 1 et 2 s’abonnent (subscribe) au broker et écoutent activement les messages publiés par la pompe, 
 * cette opération permet aux deux plantes de recevoir des informations de la part de la pompe via le broker MQTT.
 * Si la pompe est éteinte (OFF), au bout de chaque 10 minutes les deux plantes envoient les données au broker en précisant l’état d’humidité de chacune et les données sont mises à jour au bout de cette fréquence.
 * Si la pompe est éteinte (OFF) et que l’humidité de la Plante 1 (H1) est inférieur à HumiditéMin qui correspond au besoin nutritif de la plante, 
 * alors la pompe s’allume en passant de (OFF) à (ON). 
 * La Plante 1 publie (publish) l’état de la pompe au broker et ce dernier publie (publish) à la Plante 2 l’état de la pompe. 
 * La fréquence (ticker) passe de 10 minutes à 15 secondes, tout en essayant de réguler les deux taux d’humidité des plantes. 
 * Une fois cette tâche réalisée, la Plante 2 fait changer l’état de la pompe en l’éteignant puis la Plante 1 et 2 publient (publish) au broker leurs taux d’humidité qui ont été régulés. 
 * Dans le cas où la pompe est en état (ON) et que l’humidité de la Plante 2 (H2) est supérieure ou égale à HumiditéMax défini par l’utilisateur, 
 * alors la Plante 2 publie (publish) en envoyant un message au broker d’éteindre la pompe 
 * et de passer de (ON) à (OFF) puis le broker informe la Plante 1 en publiant (publish) que la Plante 2 a éteint la pompe et la fréquence (ticker) passe de 15 secondes à 10 minutes.
 * 
 * Ci-dessous, un second diagramme de classe qui montre le fonctionnement du projet :
 * \image html Diagrammedesequence2iot.png
 * 
 * \page 5 Interface Utilisateur AdafruitIO
 * \brief Utilisation de l'interface Utilisateur
 * \image html Adafruit.png
 * - L'interface Utilisateur permet à l'utilisateur de visualiser les données des taux d'humidité des plantes, 
 * de configurer les seuils d'humidité et d'activer manuellement la pompe d'irrigation si nécessaire.
 * 
 * \page 6 Conclusion
 * \brief Conclusion
 * 
 * Le système d'irrigation connecté offre une solution automatisée et pratique pour la culture des plantes, en assurant un arrosage optimal en fonction des besoins spécifiques de chaque plante. 
 * En combinant la surveillance en temps réel, la communication avec Adafruit et la facilité d'utilisation de l'interface utilisateur, 
 * ce projet offre une approche moderne mais aussi efficace pour la gestion de l'irrigation dans les environnements domestiques et agricoles.
 * 
 * \page 7 Nos composants matériels
 * \brief Nos composants matériels
 *  
 * - Plante 1, Plante 2 et Pompe.
 * - Capteurs d'humidité connectés à chaque plante pour mesurer le taux d'humidité du sol des plantes.
 * - Un routeur Wi-Fi pour permettre la connectivité des trois cartes avec un broker MQTT et ce dernier permet d'échanger les informations avec Adafruit.
 * - Un PC ou un téléphone.
 * 
 * \page 8 Membre Membre du groupe
 * \brief Membre du groupe
 *
 * - SECRET Kévin
 */
