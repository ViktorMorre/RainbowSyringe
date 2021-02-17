# RainbowSyringe
Dit is de uitleg horende bij de vaccintester uit de escape room.

## Inhoud

- [Algemene uitleg](#algemene-uitleg)
- [Flowchart](#flowchart)
- [Implementatie](#implementatie)
- [Budget](#budget)
- [Risico's](#risicos)
- [Gantt chart](#gantt-chart)

## Algemene uitleg

Door het oplossen van de 5G-puzzel, wordt er een RFID kaart verkregen. Die kaart kan gebruikt worden om de uiteindelijke kleurensequentie voor de vaccintester te vinden. In de deur van een kast zit op een bepaalde plaats een NFC reader. De spelers moeten dan met de kaart op zoek gaan naar de kast en uitzoeken waar in de deur de NFC reader zit. Door de kaart dan te scannen worden er leds in de kast geactiveerd. Dit zijn witte leds in de kast. Boven elke witte led bevindt zich een gekleurde vloeistof in een proefbuis. De houders voor de proefbuis kunnen gebruikt worden om de witte leds weg te steken. De leds gaan in een bepaalde volgorde oplichten, zodat er een sequentie gevonden kan worden van 7 kleuren die na elkaar oplichten. Om die kleurensequentie in te geven wordt er gebruik gemaakt van drie knoppen die verspreid zijn in de ruimte. Elke knop staat voor een bepaalde primaire kleur. Om de kleuren te vormen worden de knoppen dan ingedrukt, maar om alle spelers te laten meedoen moeten de knoppen om de kleuren te vormen binnen een bepaald interval ingedrukt worden. Zo wordt groen gevormd door geel en blauw en moeten geel en blauw dus ongeveer op hetzelfde moment ingedrukt worden. Die knoppen zorgen er dan voor dat de bijhorende ledstrip die zich in een spuit bevindt oplicht en via een kleursensor wordt er getest als de juiste kleur verkregen is. Wanneer de juiste kleur verkregen is wordt er gesprongen naar de volgende kleur in de sequentie en herhaalt het vorige zich. Wanneer de laatste kleur juist is, wordt een random getal gegenereerd dat een digit van de te zoeken code is. Deze wordt doorgestuurd naar de centrale eenheid en ook getoond aan de deelnemers door het aantal proefbuizen die oplichten binnen de kast.

Om dit alles te kunnen uitvoeren wordt er gebruik gemaakt van twee ESP32's, die met elkaar kunnen communiceren via een ESPnow-connectie. Wanneer de juiste RFID gescand wordt, wordt er een signaal verzonden van de ESP32 die verbonden is met de NFC-reader en de leds in de kast naar de ESP32 die verbonden is met de kleursensoren, de drukknoppen en de leds binnen de spuit. Dat signaal zal ervoor zorgen dat de kleursensoren, de leds binnen de spuit en de drukknoppen geactiveerd worden.

Daarnaast zal de laatst besproken ESP via het wifi-netwerk geconnecteerd zijn met de broker. Op die manier kan er uiteindelijk een digit van de code die ingevoerd moet worden bij alohomara doorgestuurd worden naar de centrale eenheid of broker.

## Flowchart

![Flowchart_vaccintester](https://github.com/ViktorMorre/RainbowSyringe/blob/main/Vaccintester_flowchart%20.png)

## Implementatie

De positie van deze puzzel binnen het lokaal maakt in principe niet zo veel uit. Er moet wel voor gezorgd worden dat de knoppen minstens 1,5 meter van elkaar gescheiden zijn zodat telkens aan de voorwaarde van de "T'is beter op anderhalve meter"-puzzel voldaan is. Daarnaast gaat de spuit aan een muur moeten hangen zodat alles op een correcte manier aangesloten kan worden.

## Budget

## Risico's

Wanneer de 5G-puzzle niet werkt, kan de nodige RFID kaart niet verkregen worden. Dit kan opgelost worden door een tweede kaart te gebruiken die ook gebruikt kan worden om onze puzzel te starten. Deze kaart kan ergens verstopt worden binnen de kamer. De positie van die kaart kan dan via een raadsel achterhaald worden.

Een ander risico is dat de RFID reader en de receiver niet geconnecteerd zijn. Dit kan eventueel opgelost worden door Alohomora een signaal te laten versturen naar de tweede ESP32. 

## Gantt chart

Via volgende link gaat u naar onze Gantt chart horende bij dit project: 
[Gantt vaccintester (ClickUp)](https://share.clickup.com/g/h/4dne7-50/c3532202026c060)

