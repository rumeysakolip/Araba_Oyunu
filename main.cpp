#include <iostream>
#include <string>
#include <vector>
#include <cstdlib> // For random number generation
#include <ctime>   // For seeding random numbers

using namespace std;

// Oyuncu sinifi
class Player {
public:
    string name;
    float money;

    Player(string playerName) {
        name = playerName;
        money = 0.0;
    }

    void startRace() {
        cout << "\nYaris basladi! Bol sans, " << name << "!\n";
    }
};

// Arac sinifi
class Vehicle {
public:
    string name;
    float fuelConsumption; // Km basina tuketim
    int upgradeLevel;

    Vehicle(string vehicleName, float consumption) {
        name = vehicleName;
        fuelConsumption = consumption;
        upgradeLevel = 0;
    }

    void upgrade() {
        upgradeLevel++;
        fuelConsumption -= 0.1; // Gelistirme ile tuketim azalir
        cout << name << " gelistirildi! Mevcut seviye: " << upgradeLevel << "\n";
    }

    float calculateFuel(float distance) {
        return distance * fuelConsumption;
    }
};

// Yaris Alani sinifi
class RaceTrack {
public:
    string name;
    float fuelPrice;
    float paymentPerKm;

    RaceTrack(string trackName, float price, float payment) {
        name = trackName;
        fuelPrice = price;
        paymentPerKm = payment;
    }

    void startRace() {
        cout << name << " yaris alaninda yaris basladi!\n";
    }
};

// Bildirim sinifi
class Notification {
public:
    string message;
    int fuelLevel;

    Notification(string msg, int fuel) {
        message = msg;
        fuelLevel = fuel;
    }

    void display() {
        cout << "[BILDIRIM]: " << message << " (Benzin: " << fuelLevel << "%)\n";
    }
};

// Yaris Yonetimi sinifi
class GameManager {
public:
    Player* player;
    Vehicle* vehicle;
    RaceTrack* raceTrack;

    GameManager(Player* p, Vehicle* v, RaceTrack* r) {
        player = p;
        vehicle = v;
        raceTrack = r;
    }

    void endDay(float earnings) {
        player->money += earnings;
        cout << "Gun sonu raporu: Kazanilan para: " << earnings << " TL\n";
        cout << "Toplam para: " << player->money << " TL\n";
    }
};

int main() {
    // Rastgele sayi uretimi icin baslangic
    srand(time(0));

    // Oyuncu olusturma
    string playerName;
    cout << "Adinizi giriniz: ";
    cin >> playerName;
    Player player(playerName);

    // Arac olusturma
    Vehicle vehicle("Taksi", 0.5);

    // Yaris alani olusturma
    RaceTrack track("Sehir", 10.0, 5.0);

    // Oyun yonetimi olusturma
    GameManager game(&player, &vehicle, &track);

    // Oyuncu yaris baslatiyor
    player.startRace();
    track.startRace();

    // Ornek bildirim
    Notification notification("Yaris basladi! Benzin azaldi!", 75);
    notification.display();

    // Gun sonu islemleri
    game.endDay(150.0); // Gunluk kazanc ornek

    return 0;
}
