#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keyboard.h> 
#include <Mouse.h> 
 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
 
#define trigPin 11
#define echoPin 12
#define buzzer 5 
 
#define CLK 8
#define DT 9
#define SW 10
 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
 
int lastStateCLK;
int currentStateCLK;

// MOD VE BUTON SÜRE DEĞİŞKENLERİ
int mevcutMod = 0; // 0 = Zoom Modu, 1 = Fırça Modu
unsigned long butonBasildiZamani = 0;
bool butonBasiliyorMu = false;
bool uzunBasmaGerceklesti = false;

// YENİ: BUTON ÇILGINLIĞINI ENGELLEYEN FİLTRE DEĞİŞKENLERİ
unsigned long lastDebounceTimeSW = 0;
int lastSwState = HIGH;
 
// MULTITASKING İÇİN KRONOMETRE DEĞİŞKENLERİ
unsigned long sonZaman = 0; 
bool sirenDurumu = false; 
 
void setup() {
  Serial.begin(9600);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  
  pinMode(CLK, INPUT_PULLUP); 
  pinMode(DT, INPUT_PULLUP);   
  pinMode(SW, INPUT_PULLUP); 
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while(true);
  }
  display.clearDisplay();
  
  Keyboard.begin(); 
  Mouse.begin(); 
  lastStateCLK = digitalRead(CLK); 
}
 
void loop() {
  
  // --- 1. ROTARY ENCODER TARAMA (SADECE SEÇİLİ MODDA TEMİZ KOMUT GÖNDERİR) ---
  currentStateCLK = digitalRead(CLK);
  
  if (currentStateCLK != lastStateCLK && currentStateCLK == 1) {
    if (digitalRead(DT) != currentStateCLK) {
      // --- SAĞA DÖNÜŞ ---
      if (mevcutMod == 0) {
        Keyboard.press(KEY_LEFT_CTRL);
        Mouse.move(0, 0, 1); 
        Keyboard.releaseAll();
      } else {
        Keyboard.write(']'); 
      }
    } else {
      // --- SOLA DÖNÜŞ ---
      if (mevcutMod == 0) {
        Keyboard.press(KEY_LEFT_CTRL);
        Mouse.move(0, 0, -1); 
        Keyboard.releaseAll();
      } else {
        Keyboard.write('['); 
      }
    }
    delay(10); // Mekanik tıkırtıların arasına mini bariyer
  }
  lastStateCLK = currentStateCLK; 
  
  // --- 2. ROTARY BUTONU (YENİ: GÜRÜLTÜYÜ %100 YUTAN ASENKRON DEBOUNCE) ---
  int mevcutSwState = digitalRead(SW);

  if (mevcutSwState != lastSwState) {
    lastDebounceTimeSW = millis(); // Butonda hareket varsa kronometreyi kur
  }

  // Eğer buton sinyali 45 milisaniye boyunca sabit ve kararlı kaldıysa parazit değildir!
  if ((millis() - lastDebounceTimeSW) > 45) { 
    if (mevcutSwState == LOW) {
      if (!butonBasiliyorMu) {
        butonBasildiZamani = millis();
        butonBasiliyorMu = true;
        uzunBasmaGerceklesti = false;
      }
      
      // Uzun basma kontrolü (Ctrl+Z)
      if (!uzunBasmaGerceklesti && (millis() - butonBasildiZamani > 450)) {
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press('z');
        delay(20);
        Keyboard.releaseAll();
        uzunBasmaGerceklesti = true;
      }
    } else {
      if (butonBasiliyorMu) {
        if (!uzunBasmaGerceklesti) {
          mevcutMod = !mevcutMod; // Yalnızca kasıtlı kısa basışta modu değiştir
        }
        butonBasiliyorMu = false;
      }
    }
  }
  lastSwState = mevcutSwState;
 
  // --- 3. SENSÖR, EKRAN VE SİREN DÖNGÜSÜ (100 MİLİSANİYEDE BİR ÇALIŞIR) ---
  if (millis() - sonZaman > 100) { 
    sonZaman = millis(); 
    
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    long sure = pulseIn(echoPin, HIGH, 20000); 
    int mesafe;
    
    if (sure == 0) {
      mesafe = 400; 
    } else {
      mesafe = sure * 0.034 / 2;
    }
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    display.setCursor(20,2);
    display.println("FlowDesk");
    
    display.setCursor(10,15);
    display.print("Mesafe: ");
    display.print(mesafe);
    display.println(" cm");
    
    display.setCursor(10,28);
    display.print("Mod: ");
    if (mevcutMod == 0) {
      display.println("ZOOM MODU");
    } else {
      display.println("FIRCA MODU");
    }
    
    if (mesafe < 30 && mesafe > 0) {
      display.setTextSize(2);
      display.setCursor(10,44);
      display.println("UYARI!");
      
      sirenDurumu = !sirenDurumu; 
      if(sirenDurumu) tone(buzzer, 2000);
      else tone(buzzer, 1000);
      
    } else {
      noTone(buzzer);
    }
    
    display.display(); 
  }
}
