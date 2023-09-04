# Symulacja windy

Program wizualizujący działanie windy
![screen](https://github.com/jwszol-classes/tp2023-s193369/assets/134955266/9a6befad-bc97-41ea-8683-7966dbc4f05e)

## Podział pracy
##### Kacper:
- kolejkowanie pięter 
- zatrzymywanie windy
- implementacja pasażerów
- wyświetlanie wagi
##### Karol:
- wsiadanie 
- wysiadanie
- implementacja programu na wiele pięter
- podwójne buforowanie

## Opis
- #### Kolejkowanie pięter
Jeśli na planszy są wykryci pasażerowie, a winda zatrzyma się i wektor, w którym są zapisywane piętra jest pusty to w zależności od tego, na którym piętrze znajduje się oczekujący, nr tego piętra zostaje dodany do wektora i winda się tam udaje. Podobnie jest jeśli winda zatrzyma się i wektor z kolejnymi piętrami jest pusty, a w windzie są osoby - wtedy do wektora kolejnych pięter dodaje się po kolei piętra, zaczynając od pasażera, który wsiadł najwcześniej.
Te dwie zależności powstały w wyniku "łatania" poprzednich prób kolejkowania i ostatecznie zastąpiły pierwotny sposób kolejkowania, w którym wektor pięter zapełniał się przy kliknięciu przycisku na piętrze.
- #### Zatrzymywanie windy
Kiedy winda przejeżdża obok danego piętra, sprawdzane jest, czy na tym piętrze ktoś czeka (oraz czy w windzie jest miejsce) albo czy pasażer (obecny w windzie) ma zamiar wysiąść - jego zmienna dokad pomnożona przez 100 i odjęta od 400 równa się wartości, na jakiej jest winda. Jeśli któryś z tych warunków jest spełniony to winda zatrzymuje się i wykonują się odpowiednie polecenia (wsiadanie/wysiadanie).

- #### Implementacja pasażerów
Po kliknięciu przycisku na piętrze, zostaje narysowany i dodany do wektora pasażer (z utworzonej klasy - "Osoba"). Są mu przypisywane współrzędne (za pomocą funkcji setWsp), piętro, z którego i na które jedzie (setSkad, setDokad) oraz zwiększa się zmienna odpowiadająca za liczbę pasażerów na danym piętrze. Po dodaniu pasażera do wektora zostaje uruchomiona funkcja Spawn, odpowiadająca za narysowanie pasażera na danym piętrze. Klasa "Osoba" posiada również funkcję umożliwiającą przemieszczanie się pasażera, która po podaniu odpowiednich argumentów obrazuje wsiadanie/wysiadanie. W klasie tej są dodatkowo funkcje zwracające współrzędne pasażera oraz ustawiające je.

- #### Wyświetlanie wagi
Za wyświetlanie wagi odpowiada funkcja repaintWindow, która służy również do rysowania ruchomych części windy. Jeden z argumentów pobierany przez tę funkcję (w_windzie), umożliwia wyświetlenie odpowiedniej wagi po przemnożeniu liczby osób znajdujących się w windzie przez 70. Jeśli w windzie jest pusto, wyświetla się 000, w przeciwnym przypadku miejsce na wagę zostaje wyczyszczone (poprzez wyświetlenie białych znaków) i po tym wyświetla się odpowiednia waga.
- #### Podwójne buforowanie
Podwójne buforowanie umożliwa nam to że ekran przy rysowaniu nie migota. Ogólnie podczas działania bez implemetancji tej metody, system po narysowaniu czegokolwiek, zmazuje to i ewentualnie maluje na nowo. To okno czasowe pomiędzy zmazywaniem, a ponownym rysowaniem ukazuje się jako migotanie. Podwójne buforowanie zapobiega temu. W skrócie mówiąc polega ono na tym że za pomocą odpowiednich komend mówimy systemowi, by przestał zmazywać całe okno. W tym czasie tworzymy kopie okna jako bitmape i jest ona wyświetlana cały czas. To rozwiązanie rozwiązuje problem migotania lecz przy ruchomych częściach trzeba "zmazywać" ich wcześniejsze pozycje rysując po prostu na kolor tła (u nas biały). Bez zmazywania winda przykładowo jadąc do góry zostawiałaby po sobie swój niebieski ślad.
- #### Zmazywanie 
Podczas wchodzenia do windy lub wychodzenia z niej, by zmazywać "ślady" pasażerów, które zostawiają spowodowane wcześniej omówioną implementacją podwójnego buforowania, bierzemy po prostu współrzędne pasażera i odpowiednio zmazujemy jego wcześniejszą pozycje. Za windę i pasażerów w niej odpowiada już zmienna "value" na jej podstawie zależnie od jazdy w górę czy w dół zmazujemy "ślady" windy jak i pasażerów.
- #### Wsiadanie
Wsiadanie polega na tym, że w pierwszej kolejności sprawdzamy ile osób może wejść do windy. Potem przeszukujemy wektor pasażerów w caleu znalazienia tych co są na danym piętrze i jeszcze nie weszli do windy, po wyszukaniu takowych zmieniamy jego współrzędne, aż do wejścia do windy. Ile osób może wejść do windy i czy dany pasażer wszedł do windy umożliwia nam tablica 8 elementowa czy_zajete, która opisuje miejsca w windzie na poszczególnych współrzędnych. Z funkcją odpowiedzialną za wsiadanie powiązana jest też podchodzenie pasażerów na piętrze, który nie zmieścili się w windzie. Ci którzy nie zmieścili się w windzie podchodzą aż do określonego miejsca i wtedy się zatrzymują.
- #### Wysiadanie 
Pierwsze przy wysiadaniu aktualizujemy współrzędne pasażerów w windzie, gdyż samo ich się poruszanie w windzie nie jest związane ze zmianą ich współrzędnych. Później przeszukujemy wektor pasażerów w celu wyszukania tych co chcieli przyjechać na dane piętro i są w windzie. Później zależnie od ich współrzędnych tagujemy ich jako wysiadający i zwalniamy ich dotychczasowe miejsce w windzie. Potem zmieniamy współrzędne wysiadających tak żeby wyszli z windy. W momencie przekroczenia określonej współrzędnej pasażer jest usuwany z wektora, by zwolnić miejsce.

### Autorzy:
- Kacper Płocharczyk 193369
- Karol Połomski 194403