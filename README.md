# Projekt Monitorowania Temperatury i Oświetlenia

## Opis
Ten projekt to prosty system monitorowania temperatury i poziomu światła przy użyciu Arduino. System wykorzystuje czujnik temperatury (termistor) oraz czujnik światła (fotorezystor) do zbierania danych o otoczeniu. Odczyty są prezentowane na wyświetlaczu LCD, a dane są przetwarzane przy użyciu filtru Kalmana, co poprawia dokładność pomiarów.

![Działanie projektu](https://ezgif.com/save/ezgif-6-f0fcd2d1dc.gif))

## Funkcjonalności
- **Pomiar temperatury**: System wykorzystuje termistor do pomiaru temperatury, a wyniki są filtrowane przy użyciu algorytmu Kalmana, co pozwala na uzyskanie stabilnych i dokładnych wartości.
- **Pomiar jasności**: Czujnik światła umożliwia monitorowanie poziomu oświetlenia w otoczeniu, co jest przydatne w różnych zastosowaniach.
- **Interaktywny wyświetlacz**: Dane o temperaturze i poziomie światła są prezentowane na wyświetlaczu LCD, co umożliwia łatwą obsługę i odczyt.
- **Histereza**: Implementacja histerezy dla czujnika światła zapobiega niepotrzebnemu włączaniu/wyłączaniu diody LED, co zwiększa stabilność systemu.

## Wymagane komponenty
- Arduino (np. Arduino Uno)
- Termistor (np. NTC 10kΩ)
- Fotorezystor (czujnik światła)
- Wyświetlacz LCD 16x2
- Diody LED
- Rezystory (do podłączenia czujników i diod)
- Przewody połączeniowe
- Płytka stykowa

## Schemat połączeń
(Zamieść tutaj schemat połączeń, jeśli masz, lub opisz, jak podłączyć komponenty).

### Pinout
- **Dioda LED**: Pin 2
- **Czujnik światła**: A4
- **Termistor**: A5
- **LCD**: Pin 8 (RS), Pin 9 (E), Pin 4 (D4), Pin 5 (D5), Pin 6 (D6), Pin 7 (D7)

## Kod źródłowy
Kod źródłowy projektu znajduje się w pliku `main.ino`. Skrypt zawiera implementację pomiarów temperatury i oświetlenia oraz wyświetlanie wyników na LCD.

## Instalacja
1. Skopiuj lub sklonuj ten repozytorium na swój komputer.
2. Otwórz projekt w Arduino IDE.
3. Upewnij się, że wszystkie komponenty są poprawnie podłączone.
4. Wgraj kod do Arduino.
5. Obserwuj dane wyświetlane na wyświetlaczu LCD.

## Użycie
Projekt jest użyteczny w aplikacjach wymagających monitorowania temperatury i oświetlenia, takich jak automatyzacja domowa, szkółki ogrodnicze, czy kontrola klimatu.

## Licencja
Ten projekt jest dostępny na licencji MIT. Możesz go używać, modyfikować i udostępniać według własnych potrzeb.

