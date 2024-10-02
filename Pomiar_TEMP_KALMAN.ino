#include <LiquidCrystal.h>

// Definicje pinów
#define LED_PIN 2              // Pin do diody LED
#define PHOTO_SENSOR_PIN A4    // Pin czujnika światła
#define THERMISTOR_PIN A5      // Pin termistora
#define LED_PIN_DIGITAL 2      // Pin cyfrowy do diody LED z pull-up

// Histereza na pomiar światła
int lightThreshold = 5;      // Mniejszy próg czułości
int lightHysteresis = 5;     // Histereza na poziomie 5

// Zmienne do pomiarów
int previousLightLevel = 0;  // Ostatni zmierzony poziom światła
int maxLightValue = 0;       // Maksymalna wartość jasności

// Tworzenie obiektu wyświetlacza LCD
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // Definicje pinów dla LCD

/*=========================================================================================
    ================================  FUNKCJE ZWIĄZANE Z TEMPERATURĄ =====================
=========================================================================================*/

// Parametry filtru Kalmana
float q = 0.01; // Procesowa niepewność
float r = 0.1;  // Pomiarowa niepewność
float x_hat = 0; // Oszacowanie stanu (temperatura)
float p = 1;    // Błąd szacowania

// Funkcja do odczytu temperatury z filtrem Kalmana
float readTemperatureWithKalman() {
    const int numReadings = 20; // Liczba odczytów do uśrednienia
    int readings[numReadings];   // Tablica na odczyty temperatury

    // Zbieranie odczytów z termistora
    for (int i = 0; i < numReadings; i++) {
        readings[i] = analogRead(THERMISTOR_PIN); // Odczyt wartości z termistora
        delay(10); // Krótka przerwa między odczytami
    }

    // Obliczanie średniej rezystancji
    float averageAnalogValue = 0;
    for (int i = 0; i < numReadings; i++) {
        averageAnalogValue += readings[i];
    }
    averageAnalogValue /= numReadings;

    // Obliczanie oporu termistora i temperatury
    float resistance = (1023.0 / averageAnalogValue - 1.0) * 10000; // Oblicz opór termistora
    float temperature = 1.0 / (log(resistance / 10000.0) / 3950 + 1.0 / 298.15) - 273.15; // Wzór do obliczenia temperatury w °C

    // Filtr Kalmana
    // Predykcja
    x_hat = x_hat;  // Oczekiwane oszacowanie stanu
    p = p + q;      // Aktualizacja błędu szacowania

    // Pomiar
    float k = p / (p + r); // Oblicz wzmocnienie Kalmana
    x_hat = x_hat + k * (temperature - x_hat); // Aktualizacja oszacowania stanu
    p = (1 - k) * p;       // Aktualizacja błędu szacowania

    return x_hat; // Zwróć oszacowaną temperaturę
}

// Funkcja do wyświetlania temperatury
void displayTemperature() {
    static unsigned long lastTempReadTime = 0; // Czas ostatniego odczytu temperatury
    unsigned long currentMillis = millis();     // Aktualny czas w milisekundach

    // Odczyt temperatury co 2 sekundy
    if (currentMillis - lastTempReadTime >= 2000) { // Czas od ostatniego odczytu (2000 ms)
        lastTempReadTime = currentMillis; // Zaktualizuj czas ostatniego odczytu
        lcd.setCursor(0, 0); // Ustawienie kursora w pierwszym wierszu
        lcd.print("Temp: ");
        lcd.print(readTemperatureWithKalman(), 1); // Wyświetlanie pomiaru temperatury z jedną cyfrą po przecinku
        lcd.write(1); // Wyświetlanie zdefiniowanego znaku stopnia
        lcd.print("C"); // Wyświetlanie litery 'C' dla Celsjusza
    }
}

/*=========================================================================================
    ================================  FUNKCJE ZWIĄZANE Z CZUJNIKIEM ŚWIATŁA ==============
=========================================================================================*/

// Funkcja do uśredniania pomiarów z czujnika światła
int readLightLevel() {
    int total = 0;
    for (int i = 0; i < 10; i++) {  // Uśrednianie 10 odczytów
        total += analogRead(PHOTO_SENSOR_PIN);
        delay(10); // Krótka przerwa między odczytami
    }
    return total / 10; // Zwrócenie średniej wartości
}

// Funkcja obsługi zmiany światła z histerezą
void handleLightChange(int lightLevel) {
    // Jeżeli poziom światła spadnie poniżej poprzedniej wartości minus próg, zapal diodę
    if (lightLevel < previousLightLevel - lightThreshold) {
        digitalWrite(LED_PIN_DIGITAL, LOW); // Zapal diodę (stan niski)
        previousLightLevel = lightLevel;     // Zaktualizuj ostatni poziom światła
    }
    // Jeżeli poziom światła wzrośnie ponad poprzednią wartość plus próg + histereza, zgaś diodę
    else if (lightLevel > previousLightLevel + lightThreshold + lightHysteresis) {
        digitalWrite(LED_PIN_DIGITAL, HIGH); // Zgaś diodę (stan wysoki)
        previousLightLevel = lightLevel;      // Zaktualizuj ostatni poziom światła
    }
}

// Funkcja do wyświetlania segmentów
void displayLightSegments(int lightLevel) {
    lcd.setCursor(0, 1); // Ustawienie kursora w drugim wierszu

    // Mapowanie odczytu na 1-16 segmentów
    int segmentsToShow = map(lightLevel, 0, maxLightValue, 1, 16);

    // Wyświetlanie segmentów
    for (int i = 0; i < 16; i++) {
        if (i < segmentsToShow) {
            lcd.write((uint8_t)0); // Rysuj zdefiniowany segment
        } else {
            lcd.write(' '); // Puste miejsce dla nieaktywnych segmentów
        }
    }
}

/*=========================================================================================
    ==========================  FUNKCJE ZWIĄZANE Z ZNAKAMI GRAFICZNYMI ====================
=========================================================================================*/

// Funkcja do utworzenia znaku stopnia Celsjusza
void createDegreeSymbol() {
    byte degreeSymbol[8] = {
        B00010,
        B00101,
        B00010,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000
    };
    lcd.createChar(1, degreeSymbol); // Ustalamy znak graficzny dla stopnia
}

// Ustalamy znak graficzny dla segmentu
void createCustomCharacter() {
    byte segment[8] = {
        B11111, // Pełny blok
        B11111,
        B11111,
        B11111,
        B11111,
        B11111,
        B11111,
        B11111
    };
    lcd.createChar(0, segment); // Zdefiniowanie znaku dla segmentu
}

/*=========================================================================================
    =================================  FUNKCJA KONFIGURACJI =================================
=========================================================================================*/

// Funkcja konfiguracji
void setup() {
    // Konfiguracja pinów
    pinMode(LED_PIN_DIGITAL, INPUT_PULLUP); // Dioda z pull-up
    pinMode(LED_PIN, OUTPUT);                // Ustaw pin diody jako wyjście
    lcd.begin(16, 2);                        // Inicjalizacja wyświetlacza LCD
    lcd.print("Temp: ");                     // Tekst na pierwszym wierszu
    createCustomCharacter();                 // Ustalamy znak graficzny dla segmentu
    createDegreeSymbol();                    // Stworzenie znaku dla stopnia
}

/*=========================================================================================
    ======================================  GŁÓWNA PĘTLA ===================================
=========================================================================================*/

// Główna pętla
void loop() {
    // Uśrednianie pomiaru jasności
    int lightLevel = readLightLevel();

    // Zaktualizuj maksymalną wartość światła
    if (lightLevel > maxLightValue) {
        maxLightValue = lightLevel; // Aktualizacja maksymalnej wartości
    }

    // Obsługa zmiany światła z histerezą
    handleLightChange(lightLevel);

    /*=========================================================================================
    ===============================  WYŚWIETLANIE TEMPERATURY ============================
    ==========================================================================================*/
    displayTemperature();

    /*=========================================================================================
    ========================  WYŚWIETLANIE SEGMENTÓW W DRUGIM WIERSZU =========================
    ==========================================================================================*/
    displayLightSegments(lightLevel);

    delay(500); // Przerwa dla lepszego odświeżania danych
}
