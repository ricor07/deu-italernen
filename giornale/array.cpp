#include <QVector>
#include <QPair>
#include <QString>
#include "array.h"

QVector<QPair<QString, QPair<QVector<QPair<QString, QString>>, QPair<float, float>>>> giornali;

void initGiornali() {
    if(giornali.empty()) {
        // Berlin
        QVector<QPair<QString, QString>> berlinPapers = {
            {"Berliner Zeitung", "https://www.berliner-zeitung.de/"},
            {"Berliner Kurier", "https://www.berliner-kurier.de/"},
            {"Berliner Morgenpost", "https://www.morgenpost.de/"},
            {"Der Tagesspiegel", "https://www.tagesspiegel.de/"},
            {"Die Welt", "https://www.welt.de/"}
        };
        giornali.append(qMakePair(QString("Berlin"), 
                                qMakePair(berlinPapers, 
                                        qMakePair(52.5f, 13.4f))));

        // Frankfurt
        QVector<QPair<QString, QString>> frankfurtPapers = {
            {"Frankfurter Allgemeine Zeitung (FAZ)", "https://www.faz.net/"},
            {"Frankfurter Rundschau", "https://www.fr.de/"},
            {"Hessenschau.de", "https://www.hessenschau.de/"},
            {"Frankfurter Neue Presse (FNP)", "https://www.fnp.de/"}
        };
        giornali.append(qMakePair(QString("Frankfurt"),
                                qMakePair(frankfurtPapers,
                                        qMakePair(50.1f, 8.7f))));
        
        // Munchen
        QVector<QPair<QString, QString>> munchenPapers = {
            {"Münchner Merkur", "https://www.merkur.de/"},
            {"Abendzeitung München", "https://www.abendzeitung-muenchen.de/"},
            {"tz München", "https://www.tz.de/"}
        };
        giornali.append(qMakePair(QString("Munchen"), 
                                qMakePair(munchenPapers, 
                                        qMakePair(48.1f, 11.6f))));

        // Hamburg
        QVector<QPair<QString, QString>> hamburgPapers = {
            {"Hamburger Abendblatt", "https://www.abendblatt.de/"},
            {"Die Welt Hamburg", "https://www.welt.de/hamburg/"},
            {"Hamburger Morgenpost", "https://www.mopo.de/"},
            {"NDR Hamburg", "https://www.ndr.de/hamburg/"}
        };
        giornali.append(qMakePair(QString("Hamburg"), 
                                qMakePair(hamburgPapers, 
                                        qMakePair(53.5f, 10.0f))));

        // Cologne
        QVector<QPair<QString, QString>> colognePapers = {
            {"Kölner Stadt-Anzeiger", "https://www.ksta.de/"},
            {"Kölnische Rundschau", "https://www.rundschau-online.de/"},
            {"Kölner Express", "https://www.express.de/koeln/"}
        };
        giornali.append(qMakePair(QString("Cologne"), 
                                qMakePair(colognePapers, 
                                        qMakePair(50.9f, 6.9f))));

        // Düsseldorf
        QVector<QPair<QString, QString>> düsseldorfPapers = {
            {"Düsseldorfer Express", "https://www.express.de/duesseldorf/"},
            {"WZ Düsseldorf", "https://www.wz.de/duesseldorf/"},
            {"Düsseldorfer Anzeiger", "https://www.duesseldorfer-anzeiger.de/"}
        };
        giornali.append(qMakePair(QString("Düsseldorf"), 
                                qMakePair(düsseldorfPapers, 
                                        qMakePair(51.2f, 6.8f))));

        // Stuttgart
        QVector<QPair<QString, QString>> stuttgartPapers = {
            {"Stuttgarter Zeitung", "https://www.stuttgarter-zeitung.de/"},
            {"Stuttgarter Nachrichten", "https://www.stuttgarter-nachrichten.de/"},
            {"Stuttgarter Wochenblatt", "https://www.wochenblatt-stuttgart.de/"}
        };
        giornali.append(qMakePair(QString("Stuttgart"), 
                                qMakePair(stuttgartPapers, 
                                        qMakePair(48.8f, 9.2f))));

        // Leipzig
        QVector<QPair<QString, QString>> leipzigPapers = {
            {"Leipziger Volkszeitung (LVZ)", "https://www.lvz.de/"},
            {"Leipziger Internet Zeitung", "https://www.l-iz.de/"}
        };
        giornali.append(qMakePair(QString("Leipzig"), 
                                qMakePair(leipzigPapers, 
                                        qMakePair(51.3f, 12.4f))));

        // Dortmund
        QVector<QPair<QString, QString>> dortmundPapers = {
            {"Ruhr Nachrichten Dortmund", "https://www.ruhrnachrichten.de/dortmund/"},
            {"Dortmund Express", "https://www.express.de/dortmund/"}
        };
        giornali.append(qMakePair(QString("Dortmund"), 
                                qMakePair(dortmundPapers, 
                                        qMakePair(51.5f, 7.5f))));

        // Bremen
        QVector<QPair<QString, QString>> bremenPapers = {
            {"Weser-Kurier", "https://www.weser-kurier.de/"},
            {"buten un binnen", "https://www.butenunbinnen.de/"}
        };
        giornali.append(qMakePair(QString("Bremen"), 
                                qMakePair(bremenPapers, 
                                        qMakePair(53.1f, 8.8f))));

        // Essen
        QVector<QPair<QString, QString>> essenPapers = {
            {"Westdeutsche Allgemeine Zeitung (WAZ)", "https://www.waz.de/essen/"},
            {"Ruhr Nachrichten Essen", "https://www.ruhrnachrichten.de/"}
        };
        giornali.append(qMakePair(QString("Essen"), 
                                qMakePair(essenPapers, 
                                        qMakePair(51.5f, 7.0f))));

        // Dresden
        QVector<QPair<QString, QString>> dresdenPapers = {
            {"Sächsische Zeitung", "https://www.sz-online.de/"},
            {"Dresdner Neueste Nachrichten", "https://www.dnn.de/"},
            {"Tag24 Dresden", "https://www.tag24.de/dresden/"},
            {"Dresden Online", "https://www.dresden-online.de/"}
        };
        giornali.append(qMakePair(QString("Dresden"), 
                                qMakePair(dresdenPapers, 
                                        qMakePair(51.1f, 13.7f))));

        // Nurnberg
        QVector<QPair<QString, QString>> nurnbergPapers = {
            {"Nürnberger Nachrichten", "https://www.nordbayern.de/"},
            {"Nordbayern.de", "https://www.nordbayern.de/"}
        };
        giornali.append(qMakePair(QString("Nurnberg"), 
                                qMakePair(nurnbergPapers, 
                                        qMakePair(49.5f, 11.1f))));

        // Hannover
        QVector<QPair<QString, QString>> hannoverPapers = {
            {"Hannoversche Allgemeine Zeitung (HAZ)", "https://www.haz.de/"},
            {"Neue Presse Hannover", "https://www.neuepresse.de/"},
            {"Hannover.de", "https://www.hannover.de/"}
        };
        giornali.append(qMakePair(QString("Hannover"), 
                                qMakePair(hannoverPapers, 
                                        qMakePair(52.4f, 9.7f))));

        // Duisburg
        QVector<QPair<QString, QString>> duisburgPapers = {
            {"Duisburger Nachrichten", "https://www.waz.de/duisburg/"},
            {"NRZ Duisburg", "https://www.nrz.de/duisburg/"}
        };
        giornali.append(qMakePair(QString("Duisburg"), 
                                qMakePair(duisburgPapers, 
                                        qMakePair(51.4f, 6.8f))));

        // Wuppertal
        QVector<QPair<QString, QString>> wuppertalPapers = {
            {"Westdeutsche Zeitung Wuppertal", "https://www.wz.de/wuppertal/"},
            {"Wuppertaler Rundschau", "https://www.wuppertaler-rundschau.de/"},
            {"Wuppertal.de", "https://www.wuppertal.de/"}
        };
        giornali.append(qMakePair(QString("Wuppertal"), 
                                qMakePair(wuppertalPapers, 
                                        qMakePair(51.3f, 7.2f))));

        // Bochum
        QVector<QPair<QString, QString>> bochumPapers = {
            {"WAZ Bochum", "https://www.waz.de/bochum/"},
            {"Ruhr Nachrichten Bochum", "https://www.ruhrnachrichten.de/bochum/"}
        };
        giornali.append(qMakePair(QString("Bochum"), 
                                qMakePair(bochumPapers, 
                                        qMakePair(51.5f, 7.5f))));

        // Bielefeld
        QVector<QPair<QString, QString>> bielefeldPapers = {
            {"Neue Westfälische", "https://www.nw.de/"}
        };
        giornali.append(qMakePair(QString("Bielefeld"), 
                                qMakePair(bielefeldPapers, 
                                        qMakePair(52.0f, 8.5f))));

        // Bonn
        QVector<QPair<QString, QString>> bonnPapers = {
            {"General-Anzeiger Bonn", "https://www.general-anzeiger-bonn.de/"}
        };
        giornali.append(qMakePair(QString("Bonn"), 
                                qMakePair(bonnPapers, 
                                        qMakePair(50.7f, 7.1f))));

        // Mannheim
        QVector<QPair<QString, QString>> mannheimPapers = {
            {"Mannheimer Morgen", "https://www.morgenweb.de/mannheimer-morgen/"},
            {"Rhein-Neckar-Zeitung", "https://www.rnz.de/"},
            {"Mannheim24", "https://www.mannheim24.de/"}
        };
        giornali.append(qMakePair(QString("Mannheim"), 
                                qMakePair(mannheimPapers, 
                                        qMakePair(49.5f, 8.4f))));

        // Karlsruhe
        QVector<QPair<QString, QString>> karlsruhePapers = {
            {"Badische Neueste Nachrichten (BNN)", "https://www.bnn.de/karlsruhe/"},
            {"Ka-News.de", "https://www.ka-news.de/"}
        };
        giornali.append(qMakePair(QString("Karlsruhe"), 
                                qMakePair(karlsruhePapers, 
                                        qMakePair(49.0f, 8.4f))));

        // Freiburg
        QVector<QPair<QString, QString>> freiburgPapers = {
            {"Badische Zeitung", "https://www.badische-zeitung.de/"},
            {"Freiburger Nachrichten", "https://www.freiburger-nachrichten.de/"}
        };
        giornali.append(qMakePair(QString("Freiburg"), 
                                qMakePair(freiburgPapers, 
                                        qMakePair(47.9f, 7.8f))));

        // Leverkusen
        QVector<QPair<QString, QString>> leverkusenPapers = {
            {"Leverkusener Anzeiger", "https://www.leverkusener-anzeiger.de/"},
            {"Rheinische Post Leverkusen", "https://rp-online.de/nrw/staedte/leverkusen/"}
        };
        giornali.append(qMakePair(QString("Leverkusen"), 
                                qMakePair(leverkusenPapers, 
                                        qMakePair(51.0f, 6.9f))));

        // Hagen
        QVector<QPair<QString, QString>> hagenPapers = {
            {"Westfalenpost Hagen", "https://www.wp.de/staedte/hagen/"},
            {"Hagener Zeitung", "https://www.hagener-zeitung.de/"}
        };
        giornali.append(qMakePair(QString("Hagen"), 
                                qMakePair(hagenPapers, 
                                        qMakePair(51.4f, 7.5f))));

        // Krefeld
        QVector<QPair<QString, QString>> krefeldPapers = {
            {"Westdeutsche Zeitung Krefeld", "https://www.wz.de/krefeld/"},
            {"Krefelder Nachrichten", "https://www.krefelder-nachrichten.de/"}
        };
        giornali.append(qMakePair(QString("Krefeld"), 
                                qMakePair(krefeldPapers, 
                                        qMakePair(51.3f, 6.6f))));

        // Munster
        QVector<QPair<QString, QString>> munsterPapers = {
            {"Münstersche Zeitung", "https://www.mz-web.de/"},
            {"Münsterland Zeitung", "https://www.muensterlandzeitung.de/"}
        };
        giornali.append(qMakePair(QString("Munster"), 
                                qMakePair(munsterPapers, 
                                        qMakePair(51.9f, 7.6f))));

        // Augsburg
        QVector<QPair<QString, QString>> augsburgPapers = {
            {"Augsburger Allgemeine", "https://www.augsburger-allgemeine.de/"}
        };
        giornali.append(qMakePair(QString("Augsburg"), 
                                qMakePair(augsburgPapers, 
                                        qMakePair(48.4f, 10.9f))));

        // Chemnitz
        QVector<QPair<QString, QString>> chemnitzPapers = {
            {"Freie Presse Chemnitz", "https://www.freiepresse.de/"},
            {"Tag24 Chemnitz", "https://www.tag24.de/chemnitz/"}
        };
        giornali.append(qMakePair(QString("Chemnitz"), 
                                qMakePair(chemnitzPapers, 
                                        qMakePair(50.8f, 12.9f))));

        // Halle
        QVector<QPair<QString, QString>> hallePapers = {
            {"Mitteldeutsche Zeitung", "https://www.mz-web.de/halle-saale/"},
            {"Hallesche Nachrichten", "https://www.halleschenachrichten.de/"}
        };
        giornali.append(qMakePair(QString("Halle"), 
                                qMakePair(hallePapers, 
                                        qMakePair(51.5f, 11.9f))));

        // Magdeburg
        QVector<QPair<QString, QString>> magdeburgPapers = {
            {"Volksstimme Magdeburg", "https://www.volksstimme.de/"},
            {"Magdeburger Nachrichten", "https://www.mz-web.de/magdeburg/"}
        };
        giornali.append(qMakePair(QString("Magdeburg"), 
                                qMakePair(magdeburgPapers, 
                                        qMakePair(52.1f, 11.6f))));

        // Aachen
        QVector<QPair<QString, QString>> aachenPapers = {
            {"Aachener Nachrichten", "https://www.aachener-nachrichten.de/"},
            {"Aachener Zeitung", "https://www.aachener-zeitung.de/"}
        };
        giornali.append(qMakePair(QString("Aachen"), 
                                qMakePair(aachenPapers, 
                                        qMakePair(50.8f, 6.1f))));

        // Wiesbaden
        QVector<QPair<QString, QString>> wiesbadenPapers = {
            {"Wiesbadener Kurier", "https://www.wiesbadener-kurier.de/"},
            {"Sensor Wiesbaden", "https://www.sensor-wiesbaden.de/"}
        };
        giornali.append(qMakePair(QString("Wiesbaden"), 
                                qMakePair(wiesbadenPapers, 
                                        qMakePair(50.1f, 8.2f))));

        // Regensburg
        QVector<QPair<QString, QString>> regensburgPapers = {
            {"Regensburg-Digital", "https://www.regensburg-digital.de/"},
            {"Regensburger Stadtzeitung", "https://www.regensburger-stadtzeitung.de/"},
            {"Wochenblatt.de", "https://www.wochenblatt.de/lokales/regensburg-oberpfalz"}
        };
        giornali.append(qMakePair(QString("Regensburg"), 
                                qMakePair(regensburgPapers, 
                                        qMakePair(49.0135f, 12.1016f))));
        
        // Ingolstadt
        QVector<QPair<QString, QString>> ingolstadtPapers = {
            {"In-direkt", "https://in-direkt.de/"},
            {"Ingolstadt Today", "https://www.ingolstadt-today.de/"},
            {"Ingolstadter Stimme", "https://www.xn--ingolstdter-stimme-rtb.de/"}
        };
        giornali.append(qMakePair(QString("Ingolstadt"), 
                                qMakePair(ingolstadtPapers, 
                                        qMakePair(48.7667f, 11.4250f))));
        
        // Ulm
        QVector<QPair<QString, QString>> ulmPapers = {
            {"Südwest Presse", "https://www.swp.de/lokales/ulm/"},
            {"Ulm News", "https://www.ulm-news.de/"}
        };
        giornali.append(qMakePair(QString("Ulm"), 
                                qMakePair(ulmPapers, 
                                        qMakePair(48.4011f, 9.9876f))));
        
        // Reutlingen
        QVector<QPair<QString, QString>> reutlingenPapers = {
            {"Reutlinger General-Anzeiger", "https://www.reutlinger-general-anzeiger.de/"},
            {"Reutlingen Nachrichten", "https://www.reutlingen-nachrichten.de/"}
        };
        giornali.append(qMakePair(QString("Reutlingen"), 
                                qMakePair(reutlingenPapers, 
                                        qMakePair(48.4661f, 9.2057f))));

        // Pforzheim
        QVector<QPair<QString, QString>> pforzheimPapers = {
            {"Pforzheimer Zeitung", "https://www.pforzheimerzeitung.de/"},
            {"Pforzheim24", "https://www.pforzheim24.de/"}
        };
        giornali.append(qMakePair(QString("Pforzheim"), 
                                qMakePair(pforzheimPapers, 
                                        qMakePair(48.8951f, 8.7010f))));
        
        // Saarbrücken
        QVector<QPair<QString, QString>> saarbrueckenPapers = {
            {"Saarbrücker Zeitung", "https://www.saarbruecker-zeitung.de/"},
            {"Breaking News Saarland", "https://breaking-news-saarland.de/"}
        };
        giornali.append(qMakePair(QString("Saarbrücken"), 
                                qMakePair(saarbrueckenPapers, 
                                        qMakePair(49.2f, 6.9f))));

        // Trier
        QVector<QPair<QString, QString>> trierPapers = {
            {"Sol", "https://www.sol.de/trier-mosel"}
        };
        giornali.append(qMakePair(QString("Trier"), 
                                qMakePair(trierPapers, 
                                        qMakePair(49.8f, 6.6f))));

        // Kaiserslautern
        QVector<QPair<QString, QString>> kaiserslauternPapers = {
            {"Nachrichten Kaiserslautern", "https://www.nachrichten-kl.de/"},
            {"SWR Kaiserslautern", "https://www.swr.de/swraktuell/rheinland-pfalz/kaiserslautern/"},
            {"Wochenblatt Kaiserslautern", "https://www.wochenblatt-reporter.de/wochenblatt-kaiserslautern"}

        };
        giornali.append(qMakePair(QString("Kaiserslautern"), 
                                qMakePair(kaiserslauternPapers, 
                                        qMakePair(49.4f, 7.8f))));
        
        // Darmstadt
        QVector<QPair<QString, QString>> darmstadtPapers = {
            {"Hessenschau", "https://www.hessenschau.de/orte/darmstadt-102.html"},
            {"Echo Online", "https://www.echo-online.de/lokales/darmstadt/"}
        };
        giornali.append(qMakePair(QString("Darmstadt"), 
                                qMakePair(darmstadtPapers, 
                                        qMakePair(49.9f, 8.6f))));
        
        // Rostock
        QVector<QPair<QString, QString>> rostockPapers = {
            {"Ostsee Zeitung", "https://www.ostsee-zeitung.de/lokales/rostock/"},
            {"Nordkurier", "https://www.nordkurier.de/regional/rostock/"}
        };
        giornali.append(qMakePair(QString("Rostock"), 
                                qMakePair(rostockPapers, 
                                        qMakePair(54.1f, 12.1f))));
        
        // Wurzburg
        QVector<QPair<QString, QString>> wuerzburgPapers = {
            {"Mainpost", "https://www.mainpost.de/wuerzburg"},
            {"Wurzburgs Wochenzeitung", "https://wob24.net/"}
        };
        giornali.append(qMakePair(QString("Wuerzburg"), 
                                qMakePair(wuerzburgPapers, 
                                        qMakePair(49.8f, 9.9f))));

        // Koblenz
        QVector<QPair<QString, QString>> koblenzPapers = {
            {"Blick Aktuell", "https://www.blick-aktuell.de/Koblenz"},
            {"Rhein Zeitung", "https://www.rhein-zeitung.de/lokales/koblenz-region/"}
        };
        giornali.append(qMakePair(QString("Koblenz"), 
                                qMakePair(koblenzPapers, 
                                        qMakePair(50.4f, 7.6f))));
        
        // Kassel
        QVector<QPair<QString, QString>> kasselPapers = {
            {"HNA", "https://www.hna.de/"},
            {"FRIZZ", "https://frizz-kassel.de/"}
        };
        giornali.append(qMakePair(QString("Kassel"), 
                                qMakePair(kasselPapers, 
                                        qMakePair(51.3f, 9.5f))));

        // Gottingen
        QVector<QPair<QString, QString>> gottingenPapers = {
            {"Gottinger Tageblatt", "https://www.goettinger-tageblatt.de/"}
        };
        giornali.append(qMakePair(QString("Gottingen"), 
                                qMakePair(gottingenPapers, 
                                        qMakePair(51.5f, 9.9f))));
        
        // Braunschweig
        QVector<QPair<QString, QString>> braunschweigPapers = {
            {"Braunschweig Zeitung", "https://www.braunschweiger-zeitung.de/"}
        };
        giornali.append(qMakePair(QString("Braunschweig"), 
                                qMakePair(braunschweigPapers, 
                                        qMakePair(52.3f, 10.5f))));
        
        // Osnabruck
        QVector<QPair<QString, QString>> osnabruckPapers = {
            {"NOZ", "https://www.noz.de/"},
            {"Osnabrucker Rundschau", "https://os-rundschau.de/"}
        };
        giornali.append(qMakePair(QString("Osnabruck"), 
                                qMakePair(osnabruckPapers, 
                                        qMakePair(52.3f, 8.0f))));
        
        // Oldenburg
        QVector<QPair<QString, QString>> oldenburgPapers = {
            {"Oldenburg Onlinezeitung", "https://www.oldenburger-onlinezeitung.de/"},
            {"Kreiszeitung Oldenburg", "https://www.kreiszeitung.de/lokales/oldenburg/"}
        };
        giornali.append(qMakePair(QString("Oldenburg"), 
                                qMakePair(oldenburgPapers, 
                                        qMakePair(53.1f, 8.2f))));

        // Bremerhaven
        QVector<QPair<QString, QString>> bremerhavenPapers = {
            {"Moin Bremerhaven", "https://moin-bremerhaven.de/"},
            {"Nordsee Zeitung-Bremerhaven", "https://www.nordsee-zeitung.de/bremerhaven"}
        };
        giornali.append(qMakePair(QString("Bremerhaven"), 
                                qMakePair(bremerhavenPapers, 
                                        qMakePair(53.5f, 8.6f))));
        
        // Kiel
        QVector<QPair<QString, QString>> kielPapers = {
            {"Kieler Nachrichten", "https://www.kn-online.de/"}
        };
        giornali.append(qMakePair(QString("Kiel"), 
                                qMakePair(kielPapers, 
                                        qMakePair(54.3f, 10.1f))));

        // Wien
        QVector<QPair<QString, QString>> wienPapers = {
            {"Die Presse", "https://www.diepresse.com/"},
            {"Kronen Zeitung", "https://www.krone.at/"},
            {"Der Standard", "https://www.derstandard.at/"},
            {"Kurier", "https://www.kurier.at/"},
            {"Heute.at", "https://www.heute.at/"}
        };
        giornali.append(qMakePair(QString("Wien"), 
                                qMakePair(wienPapers, 
                                        qMakePair(48.2f, 16.4f))));

        // Innsbruck
        QVector<QPair<QString, QString>> innsbruckPapers = {
            {"Tiroler Tageszeitung", "https://www.tt.com/"},
            {"Kronen Zeitung Tirol", "https://www.krone.at/tirol/innsbruck/"}
        };
        giornali.append(qMakePair(QString("Innsbruck"), 
                                qMakePair(innsbruckPapers, 
                                        qMakePair(47.2f, 11.2f))));

        // Graz
        QVector<QPair<QString, QString>> grazPapers = {
            {"Kleine Zeitung", "https://www.kleinezeitung.at/steiermark/graz/"}
        };
        giornali.append(qMakePair(QString("Graz"), 
                                qMakePair(grazPapers, 
                                        qMakePair(47.0f, 15.3f))));

        // Linz
        QVector<QPair<QString, QString>> linzPapers = {
            {"Oberösterreichische Nachrichten (OÖN)", "https://www.nachrichten.at/"}
        };
        giornali.append(qMakePair(QString("Linz"), 
                                qMakePair(linzPapers, 
                                        qMakePair(48.2f, 14.2f))));

        // Salzburg
        QVector<QPair<QString, QString>> salzburgPapers = {
            {"Salzburger Nachrichten", "https://www.sn.at/"},
            {"Salzburger Krone", "https://www.krone.at/salzburg/"},
            {"Salzburg24", "https://www.salzburg24.at/"}
        };
        giornali.append(qMakePair(QString("Salzburg"), 
                                qMakePair(salzburgPapers, 
                                        qMakePair(47.4f, 13.2f))));

        // Klagenfurt
        QVector<QPair<QString, QString>> klagenfurtPapers = {
            {"Kleine Zeitung Kärnten", "https://www.kleinezeitung.at/kaernten/klagenfurt/"},
            {"Kronen Zeitung Kärnten", "https://www.krone.at/kaernten/klagenfurt/"}
        };
        giornali.append(qMakePair(QString("Klagenfurt"), 
                                qMakePair(klagenfurtPapers, 
                                        qMakePair(46.4f, 14.1f))));

        // Bern
        QVector<QPair<QString, QString>> bernPapers = {
            {"Berner Zeitung (BZ)", "https://www.bzbasel.ch/"},
            {"Der Bund", "https://www.derbund.ch/"},
            {"20 Minuten Bern", "https://www.20min.ch/bern/"}
        };
        giornali.append(qMakePair(QString("Bern"), 
                                qMakePair(bernPapers, 
                                        qMakePair(46.6f, 7.3f))));

        // Zurich
        QVector<QPair<QString, QString>> zurichPapers = {
            {"Tages-Anzeiger", "https://www.tagesanzeiger.ch/"},
            {"NZZ (Neue Zürcher Zeitung)", "https://www.nzz.ch/"},
            {"Zürichsee-Zeitung", "https://www.zsz.ch/"}
        };
        giornali.append(qMakePair(QString("Zurich"), 
                                qMakePair(zurichPapers, 
                                        qMakePair(47.2f, 8.3f))));

        // Zug
        QVector<QPair<QString, QString>> zugPapers = {
            {"Zuger Zeitung", "https://www.zugerzeitung.ch/"}
        };
        giornali.append(qMakePair(QString("Zug"), 
                                qMakePair(zugPapers, 
                                        qMakePair(47.1f, 8.3f))));

        // St. Gallen
        QVector<QPair<QString, QString>> stGallenPapers = {
            {"St. Galler Tagblatt", "https://www.tagblatt.ch/"},
            {"20 Minuten Ostschweiz", "https://www.20min.ch/ostschweiz/"}
        };
        giornali.append(qMakePair(QString("St. Gallen"), 
                                qMakePair(stGallenPapers, 
                                        qMakePair(47.3f, 9.2f))));

        // Winterthur
        QVector<QPair<QString, QString>> winterthurPapers = {
            {"Landbote Winterthur", "https://www.landbote.ch/"},
            {"Winterthurer Zeitung", "https://www.winterthurerzeitung.ch/"}
        };
        giornali.append(qMakePair(QString("Winterthur"), 
                                qMakePair(winterthurPapers, 
                                        qMakePair(47.3f, 8.4f))));

        // Luzern
        QVector<QPair<QString, QString>> luzernPapers = {
            {"Luzerner Zeitung", "https://www.luzernerzeitung.ch/"},
            {"Zentralplus", "https://www.zentralplus.ch/"}
        };
        giornali.append(qMakePair(QString("Luzern"), 
                                qMakePair(luzernPapers, 
                                        qMakePair(47.0f, 8.2f))));
    }
}