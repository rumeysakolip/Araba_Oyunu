#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <random>

using namespace std;

class Arac {
public:
    string isim;
    double benzinTuketimi;
    int hizLimiti;
    int gelistirmeSeviyesi;
    double yakitDeposu;

    Arac(string n, double bt, int hl)
        : isim(n), benzinTuketimi(bt), hizLimiti(hl), gelistirmeSeviyesi(0), yakitDeposu(100) {}

    void gelistir() {
        gelistirmeSeviyesi++;
        benzinTuketimi *= 0.9; // Yakıt tüketimi %10 azalır
        hizLimiti += 5; // Hız limiti artar
        cout << isim << " gelistirildi! Yeni hiz limiti: " << hizLimiti << ", Yeni benzin tuketimi: " << fixed << setprecision(2) << benzinTuketimi << "\n";
    }

    double benzinHesapla(double mesafe) {
        return mesafe * benzinTuketimi;
    }
};

class YarisAlani {
public:
    string isim;
    double benzinFiyati;
    double kmBasinaOdeme;
    double zorlukSeviyesi;

    YarisAlani(string n, double bf, double kbo, double zs)
        : isim(n), benzinFiyati(bf), kmBasinaOdeme(kbo), zorlukSeviyesi(zs) {}
};

class Oyuncu {
public:
    string isim;
    double para;
    int puan;
    double toplamMesafe;
    Arac* arac;
    YarisAlani* yarisAlani;

    Oyuncu(string n, double baslangicPara)
        : isim(n), para(baslangicPara), puan(0), toplamMesafe(0), arac(nullptr), yarisAlani(nullptr) {}

    void aracSec(Arac* a) {
        arac = a;
    }

    void yarisAlaniSec(YarisAlani* ya) {
        yarisAlani = ya;
    }

    void mesafeEkle(double mesafe) {
        toplamMesafe += mesafe;
    }

    void benzinAl(double miktar) {
        double maliyet = miktar * yarisAlani->benzinFiyati;
        if (para >= maliyet) {
            para -= maliyet;
            arac->yakitDeposu = min(100.0, arac->yakitDeposu + miktar);
            cout << "Benzin alindi: " << miktar << " litre. Kalan para: $" << para << "\n";
        } else {
            cout << "Yeterli paraniz yok. Kalan para: $" << para << "\n";
        }
    }
};

class Bildirim {
public:
    string mesaj;
    double benzinDurumu;
    string gorevBilgisi;

    Bildirim() : benzinDurumu(100) {}

    void gorevOlustur(const string& gorevBilgisi) {
        this->gorevBilgisi = gorevBilgisi;
        mesaj = "Gorev: " + gorevBilgisi;
    }

    void durumGuncelle(double benzinDurumu) {
        this->benzinDurumu = max(0.0, benzinDurumu);
    }
};

class OyunYoneticisi {
public:
    Oyuncu* oyuncu;

    OyunYoneticisi() : oyuncu(nullptr) {}

    void oyunuBaslat() {
        string isim;
        double baslangicPara = 500.0; // Oyuncunun başlangıç bütçesi
        cout << "==============================\n";
        cout << "   YARIS OYUNUNA HOS GELDIN!\n";
        cout << "==============================\n";
        cout << "Takma adinizi girin: ";
        cin >> isim;
        oyuncu = new Oyuncu(isim, baslangicPara);
        cout << "\nHos geldiniz, " << isim << "! Baslangic butceniz: $" << baslangicPara << "\n\n";
    }

    void yarisKurulumu() {
        vector<Arac*> araclar = {
            new Arac("Taksi", 0.1, 70),
            new Arac("Otobus", 0.2, 80),
            new Arac("Motosiklet", 0.05, 65),
            new Arac("ATV", 0.15, 70),
            new Arac("Kamyon", 0.25, 85)
        };

        vector<YarisAlani*> alanlar = {
            new YarisAlani("Col", 2.0, 1.5, 1.2),
            new YarisAlani("Dag", 1.8, 1.8, 1.5),
            new YarisAlani("Otoban", 1.5, 2.0, 1.0),
            new YarisAlani("Sehir", 2.5, 1.2, 1.3)
        };

        cout << "Mevcut Araclar:\n";
        for (size_t i = 0; i < araclar.size(); ++i) {
            cout << i + 1 << ". " << left << setw(10) << araclar[i]->isim
                 << " | Benzin Tuketimi: " << fixed << setprecision(2) << araclar[i]->benzinTuketimi
                 << " | Hiz Limiti: " << araclar[i]->hizLimiti << "\n";
        }

        int aracSecim;
        while (true) {
            cout << "\nArac secin (1-5): ";
            cin >> aracSecim;
            if (aracSecim >= 1 && aracSecim <= 5) {
                break;
            } else {
                cout << "Gecersiz secim. Lutfen 1 ile 5 arasinda bir sayi girin.\n";
            }
        }
        oyuncu->aracSec(araclar[aracSecim - 1]);
        cout << "\nSectiginiz arac: " << araclar[aracSecim - 1]->isim << "\n";

        cout << "Mevcut Yaris Alanlari:\n";
        for (size_t i = 0; i < alanlar.size(); ++i) {
            cout << i + 1 << ". " << left << setw(10) << alanlar[i]->isim
                 << " | Benzin Fiyati: " << alanlar[i]->benzinFiyati
                 << " | KM Basina Odeme: " << alanlar[i]->kmBasinaOdeme
                 << " | Zorluk Seviyesi: " << alanlar[i]->zorlukSeviyesi << "\n";
        }

        int alanSecim;
        while (true) {
            cout << "\nYaris alani secin (1-4): ";
            cin >> alanSecim;
            if (alanSecim >= 1 && alanSecim <= 4) {
                break;
            } else {
                cout << "Gecersiz secim. Lutfen 1 ile 4 arasinda bir sayi girin.\n";
            }
        }
        oyuncu->yarisAlaniSec(alanlar[alanSecim - 1]);
        cout << "\nSectiginiz yaris alani: " << alanlar[alanSecim - 1]->isim << "\n";
    }

    void raporHazirla(int gorev, double kalanBenzin, int puan, double toplamMesafe, double kalanPara) {
        cout << "\n======= Gunluk Rapor =======\n";
        cout << "Gorev: " << gorev << "\n";
        cout << "Kalan Benzin: " << kalanBenzin << "%\n";
        cout << "Toplam Mesafe: " << toplamMesafe << " km\n";
        cout << "Guncel Puan: " << puan << "\n";
        cout << "Kalan Para: $" << kalanPara << "\n";
        cout << "===========================\n";
    }

    void yarisiBaslat() {
        Bildirim bildirim;
        cout << "\nYaris basliyor...\n";
        for (int i = 1; i <= 20; ++i) {
            cout << "\n======== Gorev " << i << " ========\n";
            cout << "Gorev secenekleri:\n";
            cout << "1- Hizlan\n";
            cout << "2- Yakit ikmali\n";
            cout << "3- Hedefe ulas\n";

            int karar;
            while (true) {
                cout << "Kararinizi secin: ";
                cin >> karar;
                if (karar >= 1 && karar <= 3) {
                    break;
                } else {
                    cout << "Gecersiz secim. Lutfen 1 ile 3 arasinda bir secim yapin.\n";
                }
            }

            if (karar == 1) {
                cout << "Hizlanmayi sectiniz. Gorev: Hiz yapiliyor...\n";
                double harcananBenzin = oyuncu->arac->benzinHesapla(10);
                if (harcananBenzin > oyuncu->arac->yakitDeposu) {
                    cout << "Yeterli benzin yok. Yakit ikmali yapmalisiniz!\n";
                } else {
                    oyuncu->arac->yakitDeposu -= harcananBenzin;
                    oyuncu->mesafeEkle(10);
                }
            } else if (karar == 2) {
                cout << "Yakit ikmali yapiliyor...\n";
                cout << "Ne kadar benzin almak istiyorsunuz? (litre): ";
                double miktar;
                cin >> miktar;
                oyuncu->benzinAl(miktar);
            } else if (karar == 3) {
                cout << "Hedefe ulasmaya calisiyorsunuz...\n";
                oyuncu->puan += 10;
                cout << "Puan kazandiniz! Guncel puan: " << oyuncu->puan << "\n";
            }

            raporHazirla(i, oyuncu->arac->yakitDeposu, oyuncu->puan, oyuncu->toplamMesafe, oyuncu->para);

            if (oyuncu->arac->yakitDeposu <= 20) {
                cout << "Uyari: Benzin az! Depoyu doldurmayi dusunun.\n";
            }

            if (oyuncu->arac->yakitDeposu <= 0) {
                cout << "Benzin bitti! Yaris gunu burada sona erdi.\n";
                break;
            }
        }
    }

    void gunSonu() {
        cout << "\n============================\n";
        cout << "Yaris gunu sona erdi.\n";
        double kazanc = oyuncu->toplamMesafe * oyuncu->yarisAlani->kmBasinaOdeme;
        oyuncu->para += kazanc;
        cout << "Kazandiginiz para: $" << kazanc << "\n";
        cout << "Toplam para: $" << oyuncu->para << "\n";
        cout << "============================\n";
    }
};

int main() {
    OyunYoneticisi oyunYoneticisi;
    oyunYoneticisi.oyunuBaslat();
    oyunYoneticisi.yarisKurulumu();
    oyunYoneticisi.yarisiBaslat();
    oyunYoneticisi.gunSonu();

    return 0;
}
