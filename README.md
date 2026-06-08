# FlowDesk: Çok İşlevli Masaüstü Ergonomi ve Tasarım Asistanı 🚀

FlowDesk, uzun süreli bilgisayar destekli tasarım ve etkileşimli multimedya içerik üretimi süreçlerinde (Figma, Adobe Animate vb.) kullanıcıların karşılaştığı fiziksel postür bozukluklarını engellemek ve iş akışı kesintilerini en aza indirmek amacıyla geliştirilmiş donanım tabanlı bir masaüstü asistanıdır.

## 🛠️ Donanım Özellikleri & Bileşenler
Sistemin merkezinde yerleşik HID (Human Interface Device) desteği sunan **Arduino Leonardo (ATmega32U4)** mikrodenetleyicisi yer almaktadır. Herhangi bir ek sürücüye ihtiyaç duymadan "Tak-Çalıştır" mimarisiyle çalışır.

* **HC-SR04 Ultrasonik Mesafe Sensörü:** Kullanıcının ekrana olan mesafesini anlık olarak milimetrik doğrulukta ölçer.
* **0.96" I2C OLED Ekran:** Anlık mesafe verisini, aktif çalışma modunu (Zoom/Fırça) ve ergonomik uyarı grafiklerini kullanıcıya canlı yansıtır.
* **Rotary Encoder (Dönel Kodlayıcı):** Çift katmanlı durum makinesi (state machine) ile donanımsal kısayol yönetimini üstlenir.
* **Pasif Buzzer:** Güvenli postür sınırı (30 cm) ihlal edildiğinde çift frekanslı (1000 Hz / 2000 Hz) işitsel koruyucu siren çalar.

## 🧠 Yazılım Mimarisi & Öne Çıkan Özellikler
Yazılım, sıralı çalışan kodların neden olduğu sistem kilitlenmelerini (blocking) tamamen engellemek adına **Zamanlayıcı Tabanlı Çoklu İşlem (Multitasking)** algoritmasıyla kurgulanmıştır:

1. **Kesintisiz Sinyal Tarama (`millis()` kontrolü):** `delay()` komutları sistemden tamamen çıkarılmıştır. Sensör okuması ve ekran güncellemesi 100 ms periyotlarla asenkron çalışırken, işlemci kapasitesi sürekli olarak döner kodlayıcı pinlerini tarar.
2. **Asenkron Yazılımsal Debounce Filtresi:** Döner düğmedeki mekanik sinyal sıçramalarının (contact bounce) istemsiz mod geçişlerine yol açması, milisaniye tabanlı akıllı bir filtreleme katmanıyla %100 engellenmiştir.
3. **Çift Katmanlı Çalışma Modları:**
   * **Zoom Modu (Varsayılan):** Düğme çevrildiğinde arayüzde pürüzsüz yakınlaştırma/uzaklaştırma komutu iletir.
   * **Fırça Modu (Kısa Basış):** Tek dokunuşla geçilen bu modda, vektörel çizim programlarının fırça/kontur kalınlıklarını doğrudan klavye haritalaması üzerinden dinamik olarak kontrol eder.
   * **Geri Alma Fonksiyonu (Uzun Basış):** Düğmeye 450 ms ve üzeri basılı tutulduğunda doğrudan asenkron `Ctrl + Z` komutunu tetikler.

## 💻 Kullanılan Kütüphaneler
* `<Wire.h>` & `<Adafruit_SSD1306.h>` (I2C OLED Ekran Sürücüleri)
* `<Keyboard.h>` & `<Mouse.h>` (Yerleşik HID Klavye-Fare Emülasyonu)

## 🔧 Kurulum ve Çalıştırma
1. Bu depodaki `.ino` uzantılı Arduino kodunu indirin.
2. Arduino IDE üzerinden kart tipini **Arduino Leonardo** olarak seçin.
3. Gerekli ekran kütüphanelerini yükledikten sonra kodu kartınıza yükleyin.
4. Adobe Animate, Figma veya herhangi bir çizim programını açarak asistanınızı kullanmaya başlayın!

---
Developed by **Selin GENÇER** - Ankara Üniversitesi BÖTE Bölümü
