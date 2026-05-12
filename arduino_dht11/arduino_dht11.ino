/*
  ARDUINO DHT11 SENSOR + BUZZER ALARM
  
  Capteur: DHT11 connecté à la broche 2
  Buzzer: connecté à la broche 8
  Format de sortie: TEMP:25.5,HUM:60.2
  
  Branchement DHT11:
  - VCC (rouge) -> 5V
  - GND (noir) -> GND
  - DATA (jaune) -> Digital Pin 2
  - Résistance 10kΩ entre VCC et DATA
  
  Branchement Buzzer:
  - Positif (+) -> Pin 8
  - Négatif (-) -> GND
*/

#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define BUZZER 8

DHT dht(DHTPIN, DHTTYPE);

// Seuil d'alerte en degrés Celsius
float seuil = 30.0;  // SYNCHRONISÉ AVEC QT (mainwindow.h: temperatureThreshold = 30.0f)

// Variables pour l'envoi des données
unsigned long lastSendTime = 0;
const unsigned long SEND_INTERVAL = 1000; // Envoyer données toutes les 1 seconde

// Variables pour le contrôle du buzzer (non-bloquant)
bool buzzerActive = false;
unsigned long buzzerActivatedTime = 0;  // Quand le buzzer est devenu actif
const unsigned long BUZZER_ON_DURATION = 200;   // Buzzer ON pendant 200ms
const unsigned long BUZZER_OFF_DURATION = 200;  // Buzzer OFF pendant 200ms
const unsigned long BUZZER_CYCLE = BUZZER_ON_DURATION + BUZZER_OFF_DURATION;  // Cycle total 400ms

void setup() {
  // Initialiser la communication série
  Serial.begin(9600);
  
  // Initialiser le capteur DHT
  dht.begin();
  
  // Initialiser le buzzer
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  
  // Attendre un peu avant de commencer
  delay(1000);
  Serial.println("Systeme demarre...");
}

void loop() {
  unsigned long now = millis();
  
  // Envoyer les données à l'intervalle spécifié
  if (now - lastSendTime >= SEND_INTERVAL) {
    lastSendTime = now;
    
    // Lire l'humidité et la température
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature(); // Celsius
    
    // Vérifier si la lecture a échoué
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("ERROR: Lecture DHT échouée");
      buzzerActive = false;
    } else {
      // Formater et envoyer: TEMP:25.5,HUM:60.2
      Serial.print("TEMP:");
      Serial.print(temperature);
      Serial.print(",HUM:");
      Serial.println(humidity);
      
      // Activer/désactiver le buzzer selon le seuil
      if (temperature >= seuil) {
        buzzerActive = true;
      } else {
        buzzerActive = false;
      }
    }
  }
  
  // Contrôler le buzzer de manière non-bloquante
  manageBuzzer(now);
  
  // Petit délai pour éviter une boucle trop rapide
  delay(100);
}

void manageBuzzer(unsigned long now) {
  if (!buzzerActive) {
    // Buzzer inactif, l'éteindre
    digitalWrite(BUZZER, LOW);
    buzzerActivatedTime = 0;
    return;
  }
  
  // Enregistrer le temps d'activation du buzzer
  if (buzzerActivatedTime == 0) {
    buzzerActivatedTime = now;
  }
  
  // Calculer la position dans le cycle (200ms ON + 200ms OFF = 400ms total)
  unsigned long elapsedTime = now - buzzerActivatedTime;
  unsigned long cyclePosition = elapsedTime % BUZZER_CYCLE;
  
  if (cyclePosition < BUZZER_ON_DURATION) {
    // Première moitié du cycle: le buzzer est ON
    digitalWrite(BUZZER, HIGH);
  } else {
    // Deuxième moitié du cycle: le buzzer est OFF
    digitalWrite(BUZZER, LOW);
  }
}
