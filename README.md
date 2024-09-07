# Skrypty do płytki Kreweta CM4

Instrukcja pierwszego setupu:

    sudo apt-get update
    sudo apt-get install ansible git -y
    git clone https://github.com/dzerzzz/KREWETA_Firmware setup
    ansible-playbook setup/setup.yml
    
Po zakończeniu playbooka zrestartuj raspberry pi:

    sudo reboot now

Po ponownym połączeniu, zsynchronizuj pierwszy raz zegar rtc:

    ansible-playbook setup/sync_rtc.yml

Możesz także sprawdzić działanie kamery:
    
    libcamera-jpeg -o image.jpeg

Jeśli nie pojawiły się żadne błędy, płytka jest gotowa do pracy.

## Najważniejsze komendy:
| Polecenie | Opis |
|--|--|
| ansible-playbook setup/enable.yml | Uzbraja i uruchamia rejestrację danych, nawet po resecie zasilania |
| ansible-playbook setup/disable.yml | Wyłącza rejestrację danych, nawet po resecie zasilania |
| ansible-playbook setup/sync_rtc.yml | Synchronizuje zegar RTC z obecnego czasu systemowego |
| ansible-playbook setup/purge.yml | Usuwa dotychczasowe dane |
| ansible-playbook setup/set_temp.yml --extra-vars temperature=***target-temp***| Ustawia docelową temperaturę w pudełku |
| journalctl -u camera0.service -b | Logi Kamery 0 od ostatniego boota  |
| journalctl -u camera1.service -b | Logi Kamery 1 od ostatniego boota  |

## Checklista przed lotem

 - [ ] Usuń stare dane używając: ***ansible-playbook setup/purge.yml***
 - [ ] Jeśli posiadasz połączenie z internetem wykonaj ***ansible-playbook setup/sync_rtc.yml***
 - [ ] Włącz zbieranie danych używając: ***ansible-playbook setup/enable.yml***
 - [ ] Poczekaj 20 sekund aby powstały pierwsze kompletne nagrania;
 - [ ] Pobierz najstarsze nagranie z folderu camera0 i zweryfikuj jego zawartość;
 - [ ] Sprawdź czy liczba plików w folderze camera0 zwiększa się co 10 sekund;
 - [ ] Pobierz najstarsze nagranie z folderu camera1 i zweryfikuj jego zawartość;
 - [ ] Sprawdź czy liczba plików w folderze camera1 zwiększa się co 10 sekund;
 - [ ] Sprawdź czy liczba plików w folderze gyroscope zwiększa się co 60 sekund;
 - [ ] Sprawdź czy liczba plików w folderze temperature zwiększa się co 60 sekund;
 - [ ] Sprawdź czy liczba plików w folderze scd30 zwiększa się co 60 sekund;
 - [ ] Zamknij sesję terminala;
 - [ ] Wyłącz router wifi z którym połączone jest raspberry pi;
 
Jesteś gotowy do lotu! Zanotuj dokładny czas startu z precyzją do paru minut. Ułatwi Ci to szukanie lotu w nagraniach.

## Checklista po locie
 - [ ] Wyłączasz zbieranie danych używając: ***ansible-playbook setup/disable.yml***
 - [ ] Wyłączasz raspberry pi używając ***sudo shutdown now***

Potem na spokojnie poszukaj nagrań lotu w folderze camera0 i camera1 oraz danych z czujników w folderach: gyroscope, temperature, scd30. Jeśli wszystko poszło dobrze wystarczy kierować się czasem stworzenia plików.

## Komnata chwały
Pierwotna wersja firmware należy do https://github.com/sq3tle/cm4_camera_setup