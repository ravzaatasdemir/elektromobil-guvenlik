#include <stdio.h>

int main() {
    float batarya_sicakligi;
    int sarj_yuzdesi;
    char kapi_durumu;
    int fren_pedali;

    printf("=== ELEKTROMOBIL GUVENLIK KONTROLU ===\n\n");

    /* --- Batarya sicakligi --- */
    printf("Batarya Sicakligi (°C): ");
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
    } else if (sarj_yuzdesi < 10) {
        printf("SONUC: UYARI     - Batarya kritik seviyede! Surus baslatılamaz.\n");
    } else if (kapi_durumu == 'A') {
        printf("SONUC: HATA      - Kapilar acik! Lutfen kapatiniz.\n");
    } else if (fren_pedali == 0) {
        printf("SONUC: BILGI     - Guvenlik icin frene basarak tekrar deneyin.\n");
    } else {
        printf("SONUC: BASARILI  - Sistem hazir. Motor baslatiliyor...\n");
    }
    return 0;
}