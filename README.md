

# Uzay Simülasyonu (Space Simulation)

Bu proje, gezegenler arası seyahat eden uzay araçlarını, yolcuların yaşam döngülerini ve gezegenlerin zaman akışını simüle eden kapsamlı bir sistemdir. Proje, aynı mantıksal yapının hem **C** hem de **Java** dilleri ile implementasyonunu içerir.

## 🚀 Proje Özellikleri

  * **Çoklu Dil Desteği:** Hem C (Memory management, Pointers, V-Table ile Polymorphism) hem de Java (OOP, Inheritance, Collections) uygulamaları.
  * **Zaman Yönetimi:** Her gezegenin kendi gün uzunluğuna (saat/gün) göre işleyen özel takvim sistemi (`SimTime`).
  * **Yaşlanma Mekanikleri:** Gezegen türlerine göre yolcuların farklı hızlarda yaşlanması (Time Dilation benzeri mantık).
  * **Dosya Okuma:** Yapılandırılmış `.txt` dosyalarından (Kişiler, Gezegenler, Araçlar) dinamik veri yükleme.
  * **Durum Yönetimi:** Uzay araçlarının durumları (Bekliyor, Yolda, Vardı, İmha) ve yolcuların hayatta kalma durumlarının anlık takibi.

## 📂 Proje Yapısı

Proje iki ana modülden oluşur:

```text
UzaySimulasyonu/
├── C/                  # C Dili Implementasyonu
│   ├── bin/            # Derlenmiş yürütülebilir dosya (.exe)
│   ├── files/          # Girdi dosyaları (.txt)
│   ├── include/        # Başlık dosyaları (.h)
│   ├── lib/            # Derlenmiş obje dosyaları (.o)
│   ├── src/            # Kaynak kodlar (.c)
│   └── Makefile        # Derleme talimatları
│
└── Java/               # Java Dili Implementasyonu
    ├── bin/            # Derlenmiş .class dosyaları
    ├── files/          # Girdi dosyaları (.txt)
    └── src/            # Kaynak kodlar (.java)
```

## 🪐 Simülasyon Mantığı ve Kurallar

Simülasyon her adımda saati 1 birim ilerletir ve aşağıdaki kuralları uygular:

### 1\. Gezegen Türleri ve Yaşlanma Faktörleri

Yolcular bulundukları gezegenin türüne göre farklı oranlarda yaşlanırlar (ömürlerinden düşer):

| Gezegen Türü | Açıklama | Yaşlanma Çarpanı |
| :--- | :--- | :--- |
| **Kayaç (Rocky)** | Dünya benzeri standart gezegenler. | **1.0x** (Normal) |
| **Gaz Devi (Gas Giant)** | Jüpiter vb. (Daha yavaş yaşlanma). | **0.1x** (10 kat yavaş) |
| **Buz Devi (Ice Giant)** | Uranüs vb. | **0.5x** (2 kat yavaş) |
| **Cüce (Dwarf)** | Plüton vb. (Çok yavaş yaşlanma). | **0.01x** (100 kat yavaş) |

### 2\. Uzay Aracı Durumları

  * **BEKLIYOR:** Araç kalkış gezegenindedir. Kalkış saati gelene kadar yolcular o gezegenin yaşlanma faktörüne tabidir.
  * **YOLDA:** Araç kalkış yapmış, hedefe gitmektedir. Yolcular yolda yaşlanmaya devam eder (sabit 1.0 veya kalkış gezegenine göre).
  * **VARDI:** Araç hedefe ulaşmıştır. Yolcular artık hedef gezegenin nüfusuna eklenir ve o gezegenin kurallarına göre yaşlanır.
  * **İMHA:** Eğer bir araçtaki (veya bekleme sırasındaki) **tüm** yolcuların ömrü tükenirse, araç "İMHA" statüsüne geçer.

-----

## 🛠️ Kurulum ve Çalıştırma

### C Versiyonu

C versiyonu `gcc` derleyicisi ve `make` aracı kullanılarak derlenir.

1.  Terminali `C/` dizininde açın.
2.  Projeyi derlemek için:
    ```bash
    make hepsi
    # veya sadece derlemek için:
    make derle
    ```
3.  Çalıştırmak için:
    ```bash
    make calistir
    # veya Windows'ta:
    ./bin/Simulasyon.exe
    ```

**Teknik Not (C):** C versiyonunda OOP mantığını sağlamak için `struct` içinde fonksiyon işaretçileri (function pointers) kullanılarak sanal tablo (V-Table) yapısı kurulmuştur. Bellek yönetimi (malloc/free) manuel olarak yapılmaktadır.

### Java Versiyonu

Java versiyonu standart JDK (Java 17+) ile çalışır.

1.  Terminali `Java/` dizininde açın.
2.  Projeyi derlemek için:
    ```bash
    javac -d bin src/simulation/*.java src/simulation/model/*.java src/simulation/io/*.java
    ```
3.  Çalıştırmak için:
    ```bash
    java -cp bin simulation.Simulation
    ```

-----

## 📄 Girdi Dosyası Formatları

Programın çalışması için `files/` klasörü altında aşağıdaki dosyaların bulunması gerekir.

**1. Gezegenler.txt**
Format: `GezegenAdı#TipID#GunlukSaat#BaslangicTarihi`

  * *TipID:* 0: Kayaç, 1: Gaz Devi, 2: Buz Devi, 3: Cüce Gezegen

<!-- end list -->

```text
Dunya#0#24#08.04.2024
Jupiter#1#9#01.07.2025
```

**2. Kisiler.txt**
Format: `Isim#Yas#KalanOmurSaat#AtandigiGemi`

```text
Ahmet#25#2000#Orion-X2
Ayse#30#150#AstraNova
```

**3. Araclar.txt**
Format: `GemiAdi#KalkisGezegeni#VarisGezegeni#KalkisTarihi#YolculukSuresi`

```text
Orion-X2#Dunya#Saturn#18.04.2025#100
```

-----

## 📊 Çıktı Örneği

Program çalışırken konsolda anlık durum raporu gösterir:

```text
Gezegenler:
              --- Dunya ---      --- Jupiter ---
  Tarih        08.04.2024         01.07.2025
  Nüfus            50                 0

Uzay Araçları:
  Gemi        Durum      Çıkış    Varış    Kalan Saat    Varış Tarihi
  Orion-X2    YOLDA      Dunya    Jupiter      45        20.04.2025
```

Simülasyon bittiğinde (tüm araçlar vardığında veya imha olduğunda), ölen yolcuların ve hatalı atamaların özeti ekrana yazdırılır.

-----

