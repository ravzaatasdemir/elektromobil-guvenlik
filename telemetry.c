#include "telemetry.h"
#include <stdio.h>
#include <stdlib.h>

// Aracin anlık verileri
static float hiz = 0.0;
static int batarya = 0;
static float motor_sicakligi = 0.0;
static float batarya_sicakligi = 0.0;

// Kayit dizileri
static float hizlanma_kayitlari[MAX_KAYIT];
static float yavaslama_kayitlari[MAX_KAYIT];
static float rejen_kayitlari[MAX_KAYIT];

// Sayaçlar
static int hizlanma_sayisi = 0;
static int yavaslama_sayisi = 0;
static int rejen_sayisi = 0;



// Fonksiyon imzaları
void sistemi_kur(float baslangic_sicaklik, int baslangic_sarj) {
    hiz = 0.0;
    batarya = baslangic_sarj;
    motor_sicakligi = baslangic_sicaklik;
    batarya_sicakligi = baslangic_sicaklik;
}

void gaza_bas() {
    float artis = (rand() % 81 + 20) / 10.0f;
    hiz += artis;
    if(hiz > 70){
        hiz = 70;
    }

    if(batarya > 0){
        batarya -= 2;
    }else{
        printf("Batarya Tukendi");
        return;
    }

    motor_sicakligi += 5.0;

    batarya_sicakligi += 2.0;
    if(motor_sicakligi > 90.0) {
    printf("KRITIK HATA: Asiri Isinma! Sistem kapatiliyor.\n");
    exit(1);
    }

    if(batarya_sicakligi > 70.0) {
    printf("KRITIK HATA: Asiri Isinma! Sistem kapatiliyor.\n");
    exit(1);
    }
    if(hizlanma_sayisi < MAX_KAYIT) {
    hizlanma_kayitlari[hizlanma_sayisi] = artis;
    hizlanma_sayisi++;  
    }
    printf("[BILGI] Gaza basildi. Arac %.1f km/s hizlandi. Motor: %.1f C, Batarya: %.1f C\n", 
       artis, motor_sicakligi, batarya_sicakligi);
}


void frene_bas(){
     if(hiz == 0.0) {
        printf("UYARI: Arac zaten duruyor!\n");
        return;
    }

    float azalis = (rand() % 101 + 50) / 10.0f;
    float gercek_azalis = azalis;
    hiz -= azalis;

    if(hiz < 0){
        gercek_azalis = gercek_azalis + hiz;
        hiz = 0.0;
    }

    motor_sicakligi -= 3.0;
    batarya_sicakligi -= 1.0;
    if(motor_sicakligi < 20){
        motor_sicakligi = 20.0;
    }
    if(batarya_sicakligi < 20){
        batarya_sicakligi = 20.0;
    }
    
    yavaslama_kayitlari[yavaslama_sayisi] = gercek_azalis;
    yavaslama_sayisi++;

    printf("[BILGI] Frene basildi. Arac %.1f km/s yavasladi. Motor: %.1f C, Batarya: %.1f C\n", 
       azalis, motor_sicakligi, batarya_sicakligi);

}


void rejen_fren(){
    if(hiz == 0.0) {
         printf("UYARI: Arac zaten duruyor!\n");
        return;
    }
    float azalis = (rand() % 81 + 20) / 10.0f;

    hiz -= azalis;

    if(hiz < 0){
        hiz = 0.0;
    }

    batarya += 1;
    if(batarya > 100){
        batarya = 100;
    }

    motor_sicakligi -= 2.0;

    if(motor_sicakligi < 20.0){
        motor_sicakligi = 20.0;
    }

    batarya_sicakligi += 1.0;

    if(batarya_sicakligi > 70.0){
        printf("KRITIK HATA: Asiri Isinma! Sistem kapatiliyor.\n");
        exit(1);
    }

    rejen_kayitlari[rejen_sayisi] = azalis;
    rejen_sayisi++;

    printf("[BILGI] Rejeneratif fren devrede. Arac %.1f km/s yavasladi. Batarya sarj oluyor. Motor: %.1f C, Batarya: %.1f C\n", 
       azalis, motor_sicakligi, batarya_sicakligi);
}



void telemetri_ve_istatistik_yazdir(){
    float toplam_hiz = 0;
    for(int i = 0; i < hizlanma_sayisi; i++) {
        toplam_hiz += hizlanma_kayitlari[i];
    }
   float ortalama_hizlanma = (hizlanma_sayisi > 0) ? toplam_hiz / hizlanma_sayisi : 0.0;

    float toplam_yavas = 0;
    for(int i = 0; i < yavaslama_sayisi; i++) {
        toplam_yavas += yavaslama_kayitlari[i];
    }
    float ortalama_yavaslama = (yavaslama_sayisi > 0) ? toplam_yavas / yavaslama_sayisi : 0.0;

    float toplam_rejen = 0;
    for(int i = 0; i < rejen_sayisi; i++) {
        toplam_rejen += rejen_kayitlari[i];
    }
    float ortalama_rejen = (rejen_sayisi > 0) ? toplam_rejen / rejen_sayisi : 0.0;


    printf("--- ANLIK TELMETRI ---\n");
    printf("Guncel Hiz: %.1f km/s\n", hiz);
    printf("Batarya : %d%%\n", batarya); 
    printf("Motor Sicakligi: %.1f C\n", motor_sicakligi);
    printf("Batarya Sicakligi: %.1f C\n", batarya_sicakligi);
    printf("-----------------------\n");
    printf("--- SURUS ISTATISTIKLERI ---\n");
    printf("Gaza Basma Sayisi: %d (Ortalama Artis: %.1f km/s)\n", hizlanma_sayisi, ortalama_hizlanma);
    printf("Frene Basma Sayisi: %d (Ortalama Dusus: %.1f km/s)\n", yavaslama_sayisi, ortalama_yavaslama);
    printf("Rejeneratif Frenleme Sayisi: %d (Ortalama Dusus: %.1f km/s)\n", rejen_sayisi,ortalama_rejen);
    printf("-----------------------\n");
}



void sistemi_kapat(){
    printf("[BILGI] Sistem kapatiliyor...\n");
    telemetri_ve_istatistik_yazdir();
    printf("[BILGI] Motor guvenli sekilde kapatildi. Iyi gunler!\n");
}
