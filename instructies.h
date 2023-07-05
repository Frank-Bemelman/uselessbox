/*
 * Hier wat instructies voor de montage van een en ander
 * 
 * Als het binnenkastje is geplaatst (zonder de houders voor de servo motortjes) dan met dubbelzijdig plakband
 * de versterker en de microcontroller monteren. De versterker, met de gele connector met de 7 pinnetjes naar de 
 * achterkant van het kastje monteren. De microcontroller met de usb connecotr naar de achterkant.
 * 
 * Voor de luidspreker:
 * Neem een stukje kabel met pinnetjes, kleuren paars en blauw, ongeveer 8cm lang. Splits deze aan de afgeknipte
 * kant voor ongeveer 2cm. Strip de uiteinden aan stukje af. Vertin ze met de soldeerbout en soldeer ze daarna 
 * aan de luidspreker. Het maakt niet uit welke kleur op welke aansluiting van de luidspreker gaat. 
 * Lijm het luidsprekertje in de kast en sluit de draadjes aan op de groene schroefjes van de versterker.
 * 
 * Voor de versterker:
 * Pak een stukje kabel met aan weerskanten een connectortje zonder pin, in de kleuren zwart, wit, grijs.
 * Steek de connectortjes op de pinnen van de versterker, in de volgorde grijs, wit, zwart. Begin daarbij
 * aan de rechterkant, en werk naar links. Het andere uiteinde van deze kabel gaat naar de microcontroller.
 * Steek de witte op D8 (dat is de op 1 na achterste pin van de linker rij. De grijze draad gaat op de rechter
 * rij, op de derde pin van achteren. De laatste draad, de zwarte dus, gaat ook op de rechter rij, op de
 * tweede pin van voren. Controleer of dit klopt aan de hand van foto 1.
 * 
 * Voor de servo-motortjes:
 * Pak nu twee draadjes met aan één kant een connector zonder pin en aan de andere kant een connector met pin. Neem hiervoor
 * de kleuren orange en geel. Deze gebruiken we om de servo motortjes mee aan te sluiten. Het signaal op deze draadjes bepaalt
 * hoever het armpje van de servo verdraait. Zet de gele draad op D1 van de microcontroller, dat is de vierde pin vsn achteren, aan
 * de rechterkant. De oranje draad komt op D3, dat is de derde pin vsn voren, ook aan de rechterkant. 
 * 
 * Voor de 5V voeding:
 * Dan gaan we nu de kabel maken voor de min (ook wel massa of ground genoemd) en de plus 5 volt. Voor de min 
 * gebruiken we de kleur bruin en voor de 5V gebruiken we de kleur rood. De microprocessor werkt op de witte
 * stroomadapter. Op de twee achterste pinnen aan de rechterkant, is die +5V en min weer beschikbaar. Van 
 * daaruit gaan we de min en +5V brengen naar de twee servo motortjes, de versterker, en het ledstripje. Die 
 * hebben allemaal stroom nodig om te kunnen werken, en het is allemaal 5V. We moeten hier wel nauwgezet werken
 * en geen fouten maken. Electronische onderdelen gaan snel stuk als je die 'voedingsdraden' per ongeluk zou
 * verwisselen. Dus liever twee keer controleren dan één keer verkeerd doen.
 * 
 * Hier een lijstje wat we nodig hebben.
 * Rood:  2x met plugje zonder pin, 10cm lang
 *        2x met plugje mèt pin, 10cm lang
 *        1x zonder plugje, 20cm lengte
 * Bruin: 2x met plugje zonder pin, 10cm lang
 *        2x met plugje mèt pin, 10 cm lang 
 *        2x zonder plugje, 20cm lengte 
 * 
 * Strip alle rode draadjes af, ongeveer 1cm. Draai alle rode gestripte uiteinden stevig in elkaar, en vertin ze met de soldeerbout.
 * Neem dan een stukje van 3cm zwarte krimpkous en schuif dit voor ongeveer 2cm over de gesoldeerde uiteinde. Verwarm de krimpkous
 * met de fohn. Pas op, dat is geen gewone haarfohn en is erg heet. 
 * 
 * Hetzelfde doen we met de bruine draadjes. 
 * 
 * Er zijn drie draadjes zonder plugje, 1 rode en 2 bruine. Strip die allemaal een stukje af, ongeveer 3-4mm. Vertin de gestripte
 * draadjes met de soldeerbout. Deze voorbereiding zorgt ervoor dat we ze later makkelijker ergens aan vast kunnen solderen.
 * 
 * Voor de ledstrip:
 * Pak het ledtripje en vertin de 3 koperen aansluitingen, aan de kant waar het pijltje naar binnen wijst.
 * Neem een blauw draadje met connector zonder pin, en knip de connector aan de andere kant af. Strip het draadje op 3-4mm en
 * vertin het uiteinde. Soldeer dit witte draadje aan de middelste aansluting van de ledstrip. 
 * Soldeer daarna de rode draad van de rode draadboom aan de +5V aansluiting van de ledstrip. Die heb je al vertind, waardoor het solderen makkelijker gaat.
 * Soldeer tenslotte één andere bruine draad van het bruine draadbosje aan de 0V of GND van de ledstrip.
 * 
 * Voor de schakelaar:
 * Pak een groen draadje met een connector zonder pinnetje. Knip de connector aan het andere uiteinde af, strip het draadje op 3-4mm. 
 * Vertin nu het draadje met de soldeerbout. Pak nu een schakelaar en vertin daarvan 2 soldeerlipjes, de middelste en een van de buitenste. 
 * Soldeer één bruine draad van de bruine draadboom aan de middelste soldeerlip van de schakelaar.
 * Soldeer de groene draad aan een buitenste soldeerlip van de schakelaar.
 * Steek de groene draad op D2 van de microcontroller. Dat is de 4de pin van voren, aan de rechterkant. Die zit tussen de oranje en gele draad.
 * Neem de tijd, want het is echt een priegelwerkje!
 * 
 * 
 * De kabelbosjes verder aansluiten:
 * Plaats een rode en bruine connector op de microcontroller. De rode gaat op de achterste pin links, en een bruine daarnaast.
 * Ter controle, van achteren naar voren, de rechterrij, heb je nu:
 * rood - bruin - grijs - oranje - groen - geel - zwart - en een pin nog vrij
 * Aan de linkerkant heb je nu, van achteren naar voren:
 * een vrije pin, wit, groen, en de rest is ook vrij.
 * 
 * Sluit nu een rode en bruine aan op de pinnen van de versterker. De rode gaat geheel links, de bruine er naast.
 * Ter controle, van links naar rechts, heb je nu:
 * rood - bruin - 2x een vrije pin - zwart - wit - grijs
 * 
 * Sluit de servo voor het deksel aan met een rood, bruin en orange connector met pin. Sluit ze op kleur aan. Wikkel daarna een zwart
 * stukje tape om de verbinding heen, zodat deze niet meer zo makkelijk uit elkaar schiet. 
 * 
 * Tenslotte sluiten we de servo voor het armpje aan. Als het goed is heb je nog precies 3 connectortje over, een rode, een bruine
 * en een gele. Sluit deze aan op de servo, de gele gaat aan de oranje en de rode en bruine natuurlijk op rode en bruine. Wikkel
 * ook hier weer een stukje zwart tape om de verbinding, dan blijft het goed vast zitten.
 * 
 */
