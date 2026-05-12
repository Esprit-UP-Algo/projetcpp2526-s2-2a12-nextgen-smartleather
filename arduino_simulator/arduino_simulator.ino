#define BUZZER 8
float seuil = 30.0;  // Seuil d'alerte (identique au code réel)

// Variables pour le contrôle du buzzer (non-bloquant)
bool buzzerActive = false;
unsigned long buzzerActivatedTime = 0;  // Quand le buzzer est devenu actif
const unsigned long BUZZER_ON_DURATION = 200;   // Buzzer ON pendant 200ms
const unsigned long BUZZER_OFF_DURATION = 200;  // Buzzer OFF pendant 200ms
const unsigned long BUZZER_CYCLE = BUZZER_ON_DURATION + BUZZER_OFF_DURATION;  // Cycle total 400ms

// Variables pour l'envoi des données (non-bloquant)
unsigned long lastSendTime = 0;
const unsigned long SEND_INTERVAL = 1000;  // Envoyer toutes les 1 seconde

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  delay(2000);
  Serial.println("Systeme demarre...");
}

void loop() {
  unsigned long now = millis();
  
  // Envoyer les données seulement à l'intervalle spécifié (NON-BLOQUANT)
  if (now - lastSendTime >= SEND_INTERVAL) {
    lastSendTime = now;
    
    // Simuler une température variant entre 20 et 35°C
    float temperature = 25.0 + (5.0 * sin(now / 10000.0));
    
    // Simuler une humidité variant entre 40 et 70%
    float humidity = 55.0 + (15.0 * cos(now / 15000.0));
    
    // Envoyer au format standard
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
  
  // Contrôler le buzzer de manière non-bloquante à chaque itération
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
