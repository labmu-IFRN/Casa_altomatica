#include <Servo.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BMP3XX bmp;

// RS, E, D4, D5, D6, D7
LiquidCrystal lcd(22, 24, 2, 3, 4, 5);

// Leds
#define a1_1 36
#define a1_2 44
#define a1_3 46
#define a2_1 38
#define a2_2 40
#define a2_3 48
#define f1 47
#define f2 49
#define g 42
#define buzzer 33

const int sensor_dagua = A10;

Servo servo_porta;
Servo servo_varal;

byte comando = 200;

int pos_varal = 180;
int _pos_varal = 0;

bool _sensor_dagua_ = true;
bool _sensor_temperatura_ = true;
bool _luzes_frente_ = false;
bool _luzes_garagem_ = false;
bool _luzes_andar1_ = false;
bool _luzes_andar2_ = false;
bool _controle_porta = false;
bool _controle_varal = false;

void controle_sensor_dagua();
void controle_sensor_temperatura();
void controle_luzes();
void controle_ifs();
void controle_porta();
void controle_varal();
void receberDados(int numBytes);

void setup() {
  Wire.begin(8); 
  Wire.onReceive(receberDados);
  Serial.begin(115200);

  pinMode(a1_1, OUTPUT);
  pinMode(a1_2, OUTPUT);
  pinMode(a1_3, OUTPUT);
  pinMode(a2_1, OUTPUT);
  pinMode(a2_2, OUTPUT);
  pinMode(a2_3, OUTPUT);
  pinMode(f1, OUTPUT);
  pinMode(f2, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(buzzer, OUTPUT);

  lcd.begin(16, 2);
  lcd.setCursor(2, 0);

  /*if (!bmp.begin_I2C()) {
    lcd.print("Erro BMP!");
  }
  lcd.print("Erro BMP!");*/


  servo_porta.attach(25);
  servo_varal.attach(23);

  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_32X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_15);
  bmp.setOutputDataRate(BMP3_ODR_1_5_HZ);
  servo_varal.write(0);
}

void loop() {
  if (comando != 200) {
    controle_ifs();
    comando = 200;
  }

  controle_sensor_dagua();
  // controle_sensor_temperatura();
  controle_luzes();
  controle_porta();
  controle_varal();
}

void controle_porta() {
  servo_porta.write(_controle_porta ? 0: 90);
}

void controle_varal() {
  /*if (pos_varal != _pos_varal) {
    servo_varal.write(pos_varal);
    _pos_varal = pos_varal;
  }*/

  servo_varal.write(_controle_varal ? 180 : 0);

}

void controle_sensor_dagua() {
  if (_sensor_dagua_) {
    int leitura = analogRead(sensor_dagua);
    if (leitura >= 500) {
      servo_porta.write(90);
      delay(15);
      lcd.setCursor(0, 0);
      lcd.print("Esta Chovendo");
      lcd.setCursor(0, 1);
      lcd.print("Recolha Roupas");
    }else{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Ceu Aberto");
      lcd.setCursor(0, 1);
      lcd.print("Estenda Roupa");
    }
  }
}

/*void controle_sensor_temperatura() {
  if (_sensor_temperatura_) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(bmp.temperature);
    lcd.print("C");

    if (bmp.temperature >= 50) {
      lcd.setCursor(0, 1);
      lcd.print("INCENDIO!");
      for (int i = 0; i < 3; i++) {
        tone(buzzer, 2000);
        digitalWrite(f1, HIGH);
        digitalWrite(f2, HIGH);
        delay(200);
        noTone(buzzer);
        digitalWrite(f1, LOW);
        digitalWrite(f2, LOW);
        delay(200);
      }
    }
  }
}*/

void controle_luzes() {
  digitalWrite(g, _luzes_garagem_ ? HIGH : LOW);
  
  digitalWrite(f1, _luzes_frente_ ? HIGH : LOW);
  digitalWrite(f2, _luzes_frente_ ? HIGH : LOW);

  digitalWrite(a1_1, _luzes_andar1_ ? HIGH : LOW);
  digitalWrite(a1_2, _luzes_andar1_ ? HIGH : LOW);
  digitalWrite(a1_3, _luzes_andar1_ ? HIGH : LOW);

  digitalWrite(a2_1, _luzes_andar2_ ? HIGH : LOW);
  digitalWrite(a2_2, _luzes_andar2_ ? HIGH : LOW);
  digitalWrite(a2_3, _luzes_andar2_ ? HIGH : LOW);
}

void controle_ifs() {
  if (comando == 191) {_sensor_dagua_ = true; return;}
  if (comando == 192) {_sensor_dagua_ = false; return;}

  if (comando == 189) {_sensor_temperatura_ = true; return;}
  if (comando == 190) {_sensor_temperatura_ = false; return;}

  if (comando == 185) {_luzes_frente_ = true; return;}
  if (comando == 186) {_luzes_frente_ = false; return;}

  if (comando == 187) {_luzes_garagem_ = true; return;}
  if (comando == 188) {_luzes_garagem_ = false; return;}

  if (comando == 183) {_luzes_andar1_ = true; return;}
  if (comando == 184) {_luzes_andar1_ = false; return;}

  if (comando == 181) {_luzes_andar2_ = true; return;}
  if (comando == 182) {_luzes_andar2_ = false; return;}

  if (comando == 195) {_controle_varal = true; return;}
  if (comando == 196) {_controle_varal = false; return;}

  if (comando == 193) {_controle_porta = true; return;}
  if (comando == 194) {_controle_porta = false; return;}
  
  // if ((comando >= 0) && (comando <= 180)) { pos_varal = comando; }
}

void receberDados(int numBytes) {
  while (Wire.available()) {
    comando = Wire.read();
  }
}
