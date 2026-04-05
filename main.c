#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "telemetry.h"


int main() {

    srand(time(NULL));

    float batarya_sicakligi;
    int sarj_yuzdesi;
    char kapi_durumu;
    int fren_pedali;

    printf("=== ELEKTROMOBIL GUVENLIK KONTROLU ===\n\n");

    /* --- Batarya sicakligi --- */
    printf("Batarya Sicakligi (C): ");
    scanf(" %f", &batarya_sicakligi);

    if (batarya_sicakligi < -100 || batarya_sicakligi > 200) {
        printf("SONUC: HATA - Gecersiz sicaklik degeri (%.1f). "
               "Lutfen sensor verilerini kontrol edin.\n", batarya_sicakligi);
        return 1;
    }

    /* --- Sarj yuzdesi --- */
    printf("Sarj Yuzdesi (%%): ");
    scanf(" %d", &sarj_yuzdesi);

    if (sarj_yuzdesi < 0 || sarj_yuzdesi > 100) {
        printf("SONUC: HATA - Imkansiz sarj seviyesi (%d). "
               "Lutfen sensor verilerini kontrol edin.\n", sarj_yuzdesi);
        return 1;
    }

    /* --- Kapi durumu --- */
    printf("Kapi Durumu (A = Acik, K = Kapali): ");
    scanf(" %c", &kapi_durumu);

    if (kapi_durumu != 'A' && kapi_durumu != 'K') {
        printf("SONUC: HATA - Gecersiz giris. Lutfen yalnizca 'A' veya 'K' girin.\n");
        return 1;
    }

    /* --- Fren pedali --- */
    printf("Fren Pedali (1 = Basili, 0 = Basili Degil): ");
    scanf(" %d", &fren_pedali);

    if (fren_pedali != 0 && fren_pedali != 1) {
        printf("SONUC: HATA - Gecersiz giris. Lutfen yalnizca '0' veya '1' girin.\n");
        return 1;
    }

    /* --- Guvenlik degerlendirmesi --- */
    printf("\n");

    if (batarya_sicakligi > 60) {
        printf("SONUC: KRITIK HATA - Motor asiri isindi! Surus engellendi.\n");
        return 1;  // ekle
    } else if (sarj_yuzdesi < 10) {
        printf("SONUC: UYARI - Batarya kritik seviyede! Surus baslatılamaz.\n");
        return 1;  // ekle
    } else if (kapi_durumu == 'A') {
        printf("SONUC: HATA - Kapilar acik! Lutfen kapatiniz.\n");
        return 1;  // ekle
    } else if (fren_pedali == 0) {
        printf("SONUC: BILGI - Guvenlik icin frene basarak tekrar deneyin.\n");
        return 1;  // ekle
    } else {
        printf("SONUC: BASARILI - Sistem hazir. Motor baslatiliyor...\n");
    }   
    sistemi_kur(batarya_sicakligi, sarj_yuzdesi);


    // Menü döngüsü
    int secim;
    while(1) {
        printf("\n--- ELEKTROMOBIL SURUS SIMULASYONU ---\n");
        printf("1. Gaza Bas\n");
        printf("2. Frene Bas\n");
        printf("3. Rejeneratif Frenleme Yap\n");
        printf("4. Anlik Telemetri ve Istatistikleri Oku\n");
        printf("5. Sistemi Kapat\n");
        printf("Seciminiz: ");
    
        if(scanf(" %d", &secim) != 1) {
            // Harf girilirse buffer temizle
            while(getchar() != '\n');
            printf("HATA: Gecersiz giris! Lutfen 1-5 arasi sayi girin.\n");
            continue;
        }
        while(getchar() != '\n');
        
        switch(secim) {
            case 1: gaza_bas(); break;
            case 2: frene_bas(); break;
            case 3: rejen_fren(); break;
            case 4: telemetri_ve_istatistik_yazdir(); break;
            case 5: sistemi_kapat(); return 0;
            default: printf("Gecersiz secim! 1-5 arasi girin.\n");
        }
    }
    return 0;
}