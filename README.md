# 🚗⚡ Elektromobil Güvenlik Kontrol Sistemi

Bir elektrikli aracın motor başlatılmadan önce kritik güvenlik parametrelerini doğrulayan konsol tabanlı C programı.

---

## 📋 İçindekiler

- [Proje Hakkında](#proje-hakkında)
- [Özellikler](#özellikler)
- [Gereksinimler](#gereksinimler)
- [Derleme ve Çalıştırma](#derleme-ve-çalıştırma)
- [Kullanım](#kullanım)
- [Girdi Parametreleri](#girdi-parametreleri)
- [Güvenlik Mantığı ve Çıktılar](#güvenlik-mantığı-ve-çıktılar)
- [Akış Diyagramı](#akış-diyagramı)
- [Hata Yönetimi](#hata-yönetimi)
- [Örnek Çalıştırma Senaryoları](#örnek-çalıştırma-senaryoları)
- [Proje Yapısı](#proje-yapısı)
- [Geliştirme Notları](#geliştirme-notları)

---

## Proje Hakkında

Bu program, elektrikli araçlarda motor başlatma sürecini simüle eder. Gerçek hayatta bir elektromobilin ECU (Electronic Control Unit) yazılımı; batarya sıcaklığı, şarj durumu, kapı kilitleri ve fren pedal sensörü gibi parametreleri sürekli izler. Bu proje, söz konusu kontrol mantığının basitleştirilmiş bir C implementasyonudur.

---

## Özellikler

- ✅ 4 bağımsız sensör parametresi kontrolü
- ✅ Girdi doğrulama (aralık ve format kontrolleri)
- ✅ Öncelik sıralı güvenlik değerlendirmesi
- ✅ Açıklayıcı Türkçe durum mesajları
- ✅ Geçersiz sensör verisi tespiti
- ✅ Erken çıkış (`return 1`) ile hata yönetimi


## Derleme ve Çalıştırma

### Linux / macOS

```bash
# Derleme
gcc -o elektromobil elektromobil.c

# Çalıştırma
./elektromobil
```

### Windows (MinGW/GCC)

```cmd
gcc -o elektromobil.exe elektromobil.c
elektromobil.exe
```

### Standart belirtmek için (önerilen)

```bash
gcc -std=c99 -Wall -Wextra -o elektromobil elektromobil.c
```

> `-Wall -Wextra` bayrakları derleme zamanında olası uyarıları gösterir.

---

## Kullanım

Program çalıştırıldığında kullanıcıdan sırasıyla 4 parametre istenir:

```
=== ELEKTROMOBIL GUVENLIK KONTROLU ===

Batarya Sicakligi (°C): 45
Sarj Yuzdesi (%): 75
Kapi Durumu (A = Acik, K = Kapali): K
Fren Pedali (1 = Basili, 0 = Basili Degil): 1

SONUC: BASARILI  - Sistem hazir. Motor baslatiliyor...
```

---

## Girdi Parametreleri

### 1. Batarya Sıcaklığı (`float`)

| Alan | Değer |
|------|-------|
| Tip | `float` |
| Geçerli aralık | `-100.0` ile `200.0` °C arası |
| Kritik eşik | `> 60.0 °C` (motor engelleme) |
| Format | Ondalıklı sayı kabul edilir (örn. `45.5`) |

> **Not:** `-100` altı veya `200` üstü değerler sensör arızasına işaret eder ve program doğrulama hatasıyla sonlanır.

### 2. Şarj Yüzdesi (`int`)

| Alan | Değer |
|------|-------|
| Tip | `int` |
| Geçerli aralık | `0` ile `100` arası |
| Kritik eşik | `< 10` (sürüş başlatılamaz) |

### 3. Kapı Durumu (`char`)
| Alan | Değer |
|------|-------|
| Tip | `char` |
| Kabul edilen değerler | `A` (Açık) veya `K` (Kapalı) |
| Büyük/küçük harf | Yalnızca büyük harf kabul edilir |

### 4. Fren Pedalı (`int`)

| Alan | Değer |
|------|-------|
| Tip | `int` |
| Kabul edilen değerler | `1` (Basılı) veya `0` (Basılı Değil) |

## Hata Yönetimi

Program iki katmanlı hata yönetimi kullanır:

**1. Girdi Doğrulama (Erken Çıkış)**
Geçersiz sensör verisi tespit edildiğinde program anında `return 1` ile sonlanır; kalan girdiler sorulmaz. Bu, gerçek sistemlerdeki "fail-fast" yaklaşımını taklit eder.

**2. Güvenlik Değerlendirmesi**
Tüm girdiler geçerliyse `if-else if` zinciriyle öncelik sıralı değerlendirme yapılır. Bir koşul tetiklendiğinde sonraki koşullar kontrol edilmez.

**Dönüş Kodları:**

| Kod | Anlam |
|-----|-------|
| `0` | Program başarıyla tamamlandı |
| `1` | Geçersiz sensör verisi nedeniyle erken sonlandırma |


## Örnek Çalıştırma Senaryoları

### Senaryo 1 — Normal Başlatma ✅

Batarya Sicakligi: 35.0
Sarj Yuzdesi: 80
Kapi Durumu: K
Fren Pedali: 1

SONUC: BASARILI  - Sistem hazir. Motor baslatiliyor...

### Senaryo 2 — Aşırı Isınma 🔴

Batarya Sicakligi: 75.0
Sarj Yuzdesi: 80
Kapi Durumu: K
Fren Pedali: 1

SONUC: KRITIK HATA - Motor asiri isindi! Surus engellendi.

### Senaryo 3 — Düşük Batarya 🟠
Batarya Sicakligi: 25.0
Sarj Yuzdesi: 5
Kapi Durumu: K
Fren Pedali: 1

SONUC: UYARI     - Batarya kritik seviyede! Surus baslatılamaz.

### Senaryo 4 — Kapı Açık 🔴
Batarya Sicakligi: 25.0
Sarj Yuzdesi: 60
Kapi Durumu: A
Fren Pedali: 1

SONUC: HATA      - Kapilar acik! Lutfen kapatiniz.

### Senaryo 5 — Sensör Arızası (Geçersiz Sıcaklık) ⚠️

Batarya Sicakligi: 999.0

SONUC: HATA - Gecersiz sicaklik degeri (999.0). Lutfen sensor verilerini kontrol edin.


## Proje Yapısı
elektromobil-guvenlik/
│
├── elektromobil.c      # Ana kaynak kodu
└── README.md           # Bu dosya


