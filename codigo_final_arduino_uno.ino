#include <Stepper.h>
#include <Servo.h>

// --- Hardware ---
#define STEPS_PER_REV 2048
Stepper stepper(STEPS_PER_REV, 2, 3, 4, 5);  // Ajusta pines según tu shield o driver
/////
Servo myServo;
const uint8_t SERVO_PIN = 9;
/////
const int BUZZER_PIN = 12;
#define REST        0

const int RELE_PIN = 10;


long last_steps = 0;

void setup() {
  Serial.begin(115200);
  // espera a que llegue algo desde Python
  // while (!Serial);
  myServo.attach(SERVO_PIN);
  stepper.setSpeed(10);  // velocidad por defecto

  pinMode(RELE_PIN, OUTPUT);
  digitalWrite(RELE_PIN, LOW);

  myServo.write(0);
  delay(1500);
  myServo.write(180);
  delay(1500);
  myServo.write(90);
  delay(500);
  stepper.step(STEPS_PER_REV);
  delay(500);
  stepper.step(-STEPS_PER_REV);
  delay(500);
  digitalWrite(RELE_PIN, HIGH);
  delay(1500);
  digitalWrite(RELE_PIN, LOW);
  delay(500);
  sonidoExito();






  Serial.println("Arduino listo, esperando datos...");
}

void loop() {


  // Espera a línea completa: "az alt\n"
  if (Serial.available()) {

    delay(1000);
    sonidoRecepcion();    
    delay(1000);

    // marchaImperial(true);


    if (last_steps != 0) {
      // marchaImperial(true);


      delay(500);
      digitalWrite(RELE_PIN, LOW);
      delay(1200);
      Serial.print("Retrocediendo ");
      Serial.print(last_steps);
      Serial.println(" pasos para volver a origen");
      delay(600);
      stepper.step(-last_steps);
      delay(600);
      myServo.write(0);
      delay(2000);
      


      last_steps = 0;  // limpiamos para no repetirlo
    }
    float az = Serial.parseFloat();
    float alt = Serial.parseFloat();

    if (Serial.read() == '\n') {
      // Mapea az → pasos
      long steps = lround((-az / 360) * STEPS_PER_REV);
      // Mapea alt → ángulo servo (0–180)
      int angle = constrain(int(alt + 90), 0, 180);

      Serial.print("Recibido az=");
      Serial.print(az);
      Serial.print(" alt=");
      Serial.print(alt);
      Serial.print(" → steps=");
      Serial.print(steps);
      Serial.print(" servo=");
      Serial.println(angle);

      // Mueve servo
      delay(500);
      myServo.write(angle);
      delay(500);

      // Mueve stepper
      stepper.step(steps);
      // marchaImperial(false);

      delay(1000);

      digitalWrite(RELE_PIN, HIGH);
      delay(1000);
      sonidoExito();
      Serial.println("Movimiento completo.");

      last_steps = steps;
    }
  }
}

void sonidoRecepcion() {
    const int startFreq = 800;
  const int endFreq   = 1600;
  const int steps     = 8;    // cuántos pasitos de frecuencia
  const int dur       = 30;   // duración de cada paso en ms

  int delta = (endFreq - startFreq) / steps;
  for (int i = 0; i <= steps; i++) {
    int f = startFreq + delta * i;
    tone(BUZZER_PIN, f, dur);
    delay(dur);
  }
  noTone(BUZZER_PIN);
}

void sonidoExito() {
  // Primer par de píos alternados
  tone(BUZZER_PIN, 1200, 100);
  delay(150);
  tone(BUZZER_PIN, 900, 100);
  delay(150);

  // Zumbido de confirmación (sostenido)
  tone(BUZZER_PIN, 400, 300);
  delay(350);

  noTone(BUZZER_PIN);
}

