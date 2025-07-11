# 🎯 Apuntado Láser Astronómico (Arduino Uno)

Este sketch para Arduino Uno recibe ángulo azimutal y altitud desde el host (por Serial), orienta un motor paso a paso en **azimut** y un servomotor en **elevación**, y dispara un diodo láser para señalar constelaciones. Incluye señales sonoras (buzzer) y un relé para el control del láser.

---

## 📋 Requisitos de hardware

| Componente                   | Descripción                          |
|------------------------------|--------------------------------------|
| Arduino Uno                  | MCU principal                        |
| Motor paso a paso            | 2048 pasos/rev (e.g. 28BYJ-48)       |
| Driver ULN2003               | Para el motor paso a paso           |
| Servomotor                   | 0–180 grados (p.ej. SG90)            |
| Diodo láser + relé 5 V       | Encendido/apagado digital            |
| Buzzer (activo o piezo)      | Señales acústicas                    |
| Cables, protoboard y fuente  | 5 V estable                          |

---

## 🔌 Conexión de pines

| Función            | Arduino Uno Pin |
|--------------------|-----------------|
| Motor paso a paso  | 2, 3, 4, 5      |
| Servo PWM          | 9               |
| Relé láser         | 10              |
| Buzzer             | 12              |
| Serial (USB)       | —               |

> **Nota:** el driver ULN2003 requiere alimentación externa de 5 V y GND común con el Arduino.

---

## 📥 Instalación de librerías

Abre el **Arduino IDE** y asegúrate de tener instaladas:

- **Stepper** (nativa)  
- **Servo** (nativa)  

No se requieren librerías externas adicionales.

---

## 🚀 Carga del sketch

1. Conecta tu Arduino Uno al PC por USB.  
2. Abre `ApuntadoLaser.ino` en el IDE.  
3. Selecciona placa **Arduino Uno** y puerto COM correspondiente.  
4. Pulsa **Subir**.

---

## ⚙️ Cómo funciona el código

1. **Setup**  
   - Inicializa Serial @115200 bps.  
   - Adjunta servomotor (pin 9).  
   - Escanea posiciones iniciales (servo 0→180→90, stepper +/− un giro).  
   - Parpadea el relé y emite un **tono de éxito**.  
   - Entra en espera de datos.

2. **Loop**  
   - Cuando llegan datos por Serial (`az alt\n`):  
     1. Emite **tono de recepción**.  
     2. Si quedó un movimiento previo, retrocede a origen.  
     3. Convierte:
        - **Azimut** → pasos `long steps = lround((-az/360)*STEPS_PER_REV)`
        - **Altitud** → ángulo servo `int angle = constrain(alt+90,0,180)`
     4. Mueve servo, luego stepper.  
     5. Activa relé (laser), emite **tono de éxito** y guarda `last_steps`.  
   - Vuelve a esperar nuevos datos.

---
    cmd = f"{az:.2f} {alt:.2f}\n"
    ser.write(cmd.encode())
    line = ser.readline().decode().strip()
    print(line)

# Ejemplo: apuntar a az=45°, alt=30°
apuntar(45.0, 30.0)
