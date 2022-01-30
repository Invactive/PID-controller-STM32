# Projekt regulatora PID - regulacja temperatury rezystora

Projekt realizowany na płytce rozwojowej NUCLEO-STM32F767ZI.

Funkcjonalności:
-	Zadawanie temperatury za pomocą komunikacji szeregowej UART lub enkodera (domyślna temperatura startowa to 20℃)
-	Podgląd aktualnej wartości mierzonej za pomocą komunikacji szeregowej UART
-	Wyświetlanie zadawanej oraz aktualnej temperatury na wyświetlaczu LCD
-	Skrypt Python do logowania danych na żywo i graficznego przedstawiania sygnałów pomiarowych
-	Układ automatycznej regulacji sterowany za pomocą programowo zaprojektowanego regulatora PID
-	System kontroli wersji GitHub


Elementy:
-	Płytka ewaluacyjna NUCLEO-STM32F767ZI
-	Rezystor ceramiczny 22Ω, 5W
-	Zasilacz sieciowy 5,7V/800mA DC
-	Moduł zasilania MB102
-	Tranzystor MOSFET IRF520N
-	Czujnik temperatury BMP280
-	Enkoder obrotowy
-	Wyświetlacz LCD 1602A



Źródła:
- https://os.mbed.com/platforms/ST-Nucleo-F767ZI/
- https://msalamon.pl/dziecinnie-prosta-sprzetowa-obsluga-enkodera-na-stm32/
- https://www.electronics-tutorials.ws/pl/tranzystor/mosfet-jako-przelacznik.html
- https://msalamon.pl/dostalismy-swietna-obsluge-przerwania-uart-idle-w-halu/
-	https://matplotlib.org/stable/index.html
-	https://www.researchgate.net/profile/Chris_Cox6/publication/316658102_First_order_plus_dead_time_FOPDT_model_parameter_estimation/links/5b2275ed0f7e9b0e37423cf6/First-order-plus-dead-time-FOPDT-model-parameter-estimation










