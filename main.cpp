#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <random>
#include <chrono>
#include <thread>

using namespace std;

// Programda kullanılan zamanlama işlevi, milisaniye cinsinden bekleme sağlar.
void bekle(int milisaniye) {
    this_thread::sleep_for(chrono::milliseconds(milisaniye));
}

// Yakıt seviyesini görselleştiren bir çizgi çubuğu oluşturur.
void cizgiCubugu(double seviye, double maxDepo) {
    int bar = (int)((seviye / maxDepo) * 10); // Maksimum yakıt deposuna göre ölçeklendirme
    cout << "[";
    for (int i = 0; i < 10; ++i) {
        if (i < bar) cout << "=";
        else cout << " ";
    }
    cout << "] " << seviye << " litre\n";
}

// Yarışın başlangıç animasyonunu konsolda gösterir.
void yarisAnimasyonu() {
    for (int i = 0; i <= 10; ++i) {
        cout << "\rYaris Basliyor: [";
        for (int j = 0; j < i; ++j) cout << "#";
        for (int j = i; j < 10; ++j) cout << " ";
        cout << "] " << i * 10 << "%";
        bekle(200);
    }
    cout << "\n\n";
}

// Araç sınıfı, oyundaki araçları tanımlayan sınıftır.
class Arac {
public:
    string isim;    // Araç ismi
    double benzinTuketimi;  // 100 km başına yakıt tüketimi
    int gelistirmeSeviyesi; // Araç geliştirme seviyesi
    double yakitDeposu;     // Mevcut yakıt miktarı
    double maxYakitDeposu;  // Maksimum yakıt kapasitesi

    // Araç için yapıcı fonksiyon
    Arac(string n, double bt, double maxDepo)
        : isim(n), benzinTuketimi(bt), gelistirmeSeviyesi(0), yakitDeposu(maxDepo), maxYakitDeposu(maxDepo) {}

    // Araç geliştirme işlevi, yakıt tüketimini azaltır.
    void gelistir() {
        if (gelistirmeSeviyesi < 10) {
            gelistirmeSeviyesi++;
            benzinTuketimi *= 0.95; // Yakıt tüketimi %5 azalır
            cout << "\033[1;32m" << isim << " gelistirildi! Yeni benzin tuketimi: " << fixed << setprecision(2) << benzinTuketimi << "\033[0m\n";
        } else {
            cout << "\033[1;31mBu arac maksimum gelistirme seviyesine ulasmis!\033[0m\n";
        }
    }

    // Belirli bir mesafe için gereken yakıt miktarını hesaplar.
    double benzinHesapla(double mesafe) {
        return (mesafe/20) * benzinTuketimi;
    }
};

// Yarış alanlarını temsil eden sınıf.
class YarisAlani {
public:
    string isim;    // Yarış alanı adı
    double benzinFiyati;    // Benzin fiyatı (litre başına)
    double kmBasinaOdeme;   // Kilometre başına ödeme miktarı

    // Yarış alanı için yapıcı fonksiyon
    YarisAlani(string n, double bf, double kbo)
        : isim(n), benzinFiyati(bf), kmBasinaOdeme(kbo) {}
};

// Oyuncu sınıfı, oyuncunun durumunu ve eylemlerini tanımlar.
class Oyuncu {
public:
    string isim;    // Oyuncunun adı
    double para;    // Oyuncunun mevcut parası
    int puan;       // Oyuncunun puanı
    double toplamMesafe;    // Oyuncunun toplam kat ettiği mesafe
    Arac* arac;     // Oyuncunun sahip olduğu araç
    YarisAlani* yarisAlani; // Oyuncunun seçtiği yarış alanı

    // Oyuncu için yapıcı fonksiyon
    Oyuncu(string n, double baslangicPara)
        : isim(n), para(baslangicPara), puan(0), toplamMesafe(0), arac(nullptr), yarisAlani(nullptr) {}

    // Oyuncunun araç seçmesini sağlar.
    void aracSec(Arac* a) {
        arac = a;
    }

    // Oyuncunun yarış alanı seçmesini sağlar.
    void yarisAlaniSec(YarisAlani* ya) {
        yarisAlani = ya;
    }

    // Oyuncunun toplam mesafesine yeni mesafe ekler.
    void mesafeEkle(double mesafe) {
        toplamMesafe += mesafe;
    }

    // Oyuncunun yakıt almasını sağlar.
    void benzinAl(double miktar) {
        double maliyet = miktar * yarisAlani->benzinFiyati;
        if (para >= maliyet) {
            para -= maliyet;
            arac->yakitDeposu = min(arac->maxYakitDeposu, arac->yakitDeposu + miktar);
            cout << "\033[1;34mBenzin alindi: " << miktar << " litre. Kalan para: $" << para << "\033[0m\n";
        } else {
            cout << "\033[1;31mYeterli paraniz yok. Kalan para: $" << para << "\033[0m\n";
        }
    }
};

// Oyun yöneticisi, oyunun genel kontrolünü sağlar.
class OyunYoneticisi {
public:
    Oyuncu* oyuncu; // Oyunun oyuncusu
    vector<Arac*> araclar; // Araç listesi burada tanımlandı
    vector<YarisAlani*> alanlar; // Kullanılabilir yarış alanları listesi

    // Oyun yöneticisi için yapıcı fonksiyon
    OyunYoneticisi() : oyuncu(nullptr) {}

    // Oyunun durumunu gösteren bir durum çubuğu.
    void durumCubugu() {
        cout << "\033[1;36m==================== DURUM ====================\033[0m\n";
        cout << "Benzin Durumu: ";
        cizgiCubugu(oyuncu->arac->yakitDeposu, oyuncu->arac->maxYakitDeposu);
        cout << "Toplam Mesafe: " << oyuncu->toplamMesafe << " km\n";
        cout << "Kalan Para: $" << oyuncu->para << "\n";
        cout << "\033[1;36m==============================================\033[0m\n";
    }

    // Oyunun başlangıç aşamasını başlatır.
    void oyunuBaslat() {
        string isim;
        double baslangicPara = 500.0; // Oyuncunun başlangıç bütçesi
        cout << "==============================\n";
        cout << "   \033[1;36mYARIS OYUNUNA HOS GELDIN!\033[0m\n";
        cout << "==============================\n";
        cout << "Takma adinizi girin: ";
        cin >> isim;
        oyuncu = new Oyuncu(isim, baslangicPara);
        cout << "\nHos geldiniz, \033[1;32m" << isim << "\033[0m! Baslangic butceniz: $" << baslangicPara << "\n\n";
    }

    // Yarış kurulumunu yapar.
    void yarisKurulumu() {
        araclar = {
            new Arac("Taksi", 2.0, 30.0),
            new Arac("Otobus", 3.0, 40.0),
            new Arac("Motosiklet", 2.5, 25.0),
            new Arac("ATV", 4.0, 35.0),
            new Arac("Kamyon", 8.0, 50.0)
        };

        alanlar = {
            new YarisAlani("Col", 1.0, 1.5),
            new YarisAlani("Dag", 2.0, 1.8),
            new YarisAlani("Otoban", 3.0, 2.0),
            new YarisAlani("Sehir", 4.0, 1.2)
        };

        // Oyuncunun araç seçimini sağlar.
        cout << "\033[1;33mMevcut Araclar:\033[0m\n";
        for (size_t i = 0; i < araclar.size(); ++i) {
            cout << i + 1 << ". " << left << setw(10) << araclar[i]->isim
                 << " | Benzin Tuketimi: " << fixed << setprecision(2) << araclar[i]->benzinTuketimi
                 << " | Maksimum Depo: " << araclar[i]->maxYakitDeposu << " litre\n";
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
        cout << "\nSectiginiz arac: \033[1;32m" << araclar[aracSecim - 1]->isim << "\033[0m\n";

        // Oyuncunun yarış alanı seçimini sağlar.
        cout << "\033[1;33mMevcut Yaris Alanlari:\033[0m\n";
        for (size_t i = 0; i < alanlar.size(); ++i) {
            cout << i + 1 << ". " << left << setw(10) << alanlar[i]->isim
                 << " | Benzin Fiyati: " << alanlar[i]->benzinFiyati
                 << " | KM Basina Odeme: " << alanlar[i]->kmBasinaOdeme << "\n";
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
        cout << "\nSectiginiz yaris alani: \033[1;32m" << alanlar[alanSecim - 1]->isim << "\033[0m\n";
    }

    void gelistirmeEkrani(vector<Arac*>& araclar) {
        cout << "\033[1;36m==================== GELISTIRME ====================\033[0m\n";
        for (size_t i = 0; i < araclar.size(); ++i) {
            cout << i + 1 << ". " << left << setw(10) << araclar[i]->isim
                << " | Gelistirme Seviyesi: " << araclar[i]->gelistirmeSeviyesi
                << " | Benzin Tuketimi: " << fixed << setprecision(2) << araclar[i]->benzinTuketimi << "\n";
        }
        cout << "0. Cikis\n";
        
        int secim;
        while (true) {
            cout << "Hangi araci gelistirmek istiyorsunuz? (Cikis icin 0): ";
            cin >> secim;
            if (secim == 0) break;
            if (secim >= 1 && secim <= (int)araclar.size()) {
                double maliyet = 100.0; // Her geliştirme için sabit bir maliyet
                if (oyuncu->para >= maliyet) {
                    oyuncu->para -= maliyet;
                    araclar[secim - 1]->gelistir();
                    cout << "Kalan Para: $" << oyuncu->para << "\n";
                } else {
                    cout << "\033[1;31mYeterli paraniz yok!\033[0m\n";
                }
            } else {
                cout << "Gecersiz secim. Lutfen tekrar deneyin.\n";
            }
        }
    }

    void odulHesapla(vector<Arac*>& araclar) {
        double kazanc = oyuncu->toplamMesafe * oyuncu->yarisAlani->kmBasinaOdeme;
        oyuncu->para += kazanc;
        cout << "\033[1;32mGun sonu odul: $" << kazanc << "\033[0m\n";
        cout << "Toplam kazanciniz: $" << oyuncu->para << "\n";

        gelistirmeEkrani(araclar); // Geliştirme ekranını göster
    }

    void aracVeAlanDegistir() {
        char secim;

        // Araç değiştirme
        cout << "Aracinizi degistirmek istiyor musunuz? (E/H): ";
        cin >> secim;
        if (secim == 'E' || secim == 'e') {
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
            cout << "\nSectiginiz arac: \033[1;32m" << araclar[aracSecim - 1]->isim << "\033[0m\n";

            cout << "\033[1;33mMevcut Yaris Alanlari:\033[0m\n";
            for (size_t i = 0; i < alanlar.size(); ++i) {
                cout << i + 1 << ". " << left << setw(10) << alanlar[i]->isim
                    << " | Benzin Fiyati: " << alanlar[i]->benzinFiyati
                    << " | KM Basina Odeme: " << alanlar[i]->kmBasinaOdeme << "\n";
            }
        }

        // Yarış alanı değiştirme
        cout << "Yaris alanini degistirmek istiyor musunuz? (E/H): ";
        cin >> secim;
        if (secim == 'E' || secim == 'e') {
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
            cout << "\nSectiginiz yaris alani: \033[1;32m" << alanlar[alanSecim - 1]->isim << "\033[0m\n";
        }
    }

    // Oyunun ana döngüsü olan yarış işlevini başlatır.
    void yarisiBaslat() {
        char devamMi = 'E';
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> mesafeDagitim(5, 15);

        while (true) {
            int gorevSayisi = 0;
            oyuncu->toplamMesafe = 0;

            while (devamMi == 'E' || devamMi == 'e') {
            if (gorevSayisi >= 20 || oyuncu->arac->yakitDeposu <= 0) {
                cout << "\033[1;31mGun sona erdi!\033[0m\n";
                break;
            }

            gorevSayisi++;
            durumCubugu();
            yarisAnimasyonu();
            cout << "\033[1;36mGunluk Gorev " << gorevSayisi << ":\033[0m\n";
            int mesafe1 = mesafeDagitim(gen);
            int mesafe2 = mesafeDagitim(gen);
            int mesafe3 = mesafeDagitim(gen);

            cout << "1. Bilinmeyen Yol\n";
            cout << "2. Bilinmeyen Yol\n";
            cout << "3. Bilinmeyen Yol\n";
            cout << "4. Benzin Al\n";
            int secim;
            cout << "Seciminizi girin: ";
            cin >> secim;

            if (secim >= 1 && secim <= 3) {
                int secilenMesafe = (secim == 1) ? mesafe1 : (secim == 2) ? mesafe2 : mesafe3;
                double harcananBenzin = oyuncu->arac->benzinHesapla(secilenMesafe);

                if (oyuncu->arac->yakitDeposu >= harcananBenzin) {
                    oyuncu->arac->yakitDeposu -= harcananBenzin;
                    oyuncu->mesafeEkle(secilenMesafe);
                    cout << "\033[1;32m" << secilenMesafe << " km ilerlediniz. Harcanan benzin: " << harcananBenzin << " litre\033[0m\n";
                } else if (oyuncu->arac->yakitDeposu > 0) {
                    cout << "\033[1;33mBenzin seviyeniz yetersiz! Kalan benzin: " << fixed << setprecision(2) << oyuncu->arac->yakitDeposu << " litre. Yolculuk tamamlanamadi.\033[0m\n";
                }

                // Benzin düşükse uyarı ver
                if (oyuncu->arac->yakitDeposu > 0 && oyuncu->arac->yakitDeposu <= oyuncu->arac->maxYakitDeposu * 0.2) {
                    cout << "\033[1;33mUyari: Benzin seviyesi dusuk (" << fixed << setprecision(1) << oyuncu->arac->yakitDeposu << " litre).\033[0m\n";
                }

                // Benzin tamamen biterse gün sona ersin
                if (oyuncu->arac->yakitDeposu <= 0.5) {
                    cout << "\033[1;31mAraciniz yolda kaldi! Gun sona erdi.\033[0m\n";
                    break;
                }
            } else if (secim == 4) {
                double miktar;
                cout << "Ne kadar benzin almak istiyorsunuz? (litre): ";
                cin >> miktar;
                oyuncu->benzinAl(miktar);
            } else {
                cout << "\033[1;31mGecersiz secim!\033[0m\n";
            }
        }

            odulHesapla(araclar);

            // Araç ve yarış alanı değiştirme seçeneği
            aracVeAlanDegistir();

            cout << "Yeni bir gune baslamak istiyor musunuz? (E/H): ";
            cin >> devamMi;
            if (devamMi != 'E' && devamMi != 'e') {
                cout << "\033[1;31mOyun sona erdi.\033[0m\n";
                break;
            }
        }
    }
};

// Programın başlangıç noktası.
int main() {
    OyunYoneticisi oyunYoneticisi;
    oyunYoneticisi.oyunuBaslat(); // Oyunu başlatır.
    oyunYoneticisi.yarisKurulumu(); // Yarış için kurulumları yapar.
    oyunYoneticisi.yarisiBaslat(); // Yarış başlatılır.
    return 0;
}
