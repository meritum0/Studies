package gielda;

public class Portfel {
    private int gotowka;
    private ZbiorAkcji posiadane_akcje = new ZbiorAkcji();

    //*************************************** METODY PUBLIC *******************************************//

    public void dodajGotowke( int liczba ) { gotowka += liczba; }

    // Dodaje akcje i pobiera gotówkę, równą co do wartości dodanym akcjom.
    public void dodajAkcje( Akcje akcje, int cena ) {
        gotowka -= cena * akcje.liczba();
        posiadane_akcje.dodaj(akcje);
    }

    // Odejmuje akcje i dodaje gotówkę, równą co do wartości dodanym akcjom.
    public void odejmijAkcje( Akcje akcje, int cena ) {
        gotowka += cena * akcje.liczba();
        posiadane_akcje.odejmij(akcje);
    }
    public Spolka losowaSpolka() {
        return posiadane_akcje.losowaSpolka();
    }

    // Zwraca losową liczbę pomiędzy 1, a liczbą posiadanych akcji danej spółki.
    public int losowaLiczbaDoSprzedazy( Spolka spolka ) {
        return posiadane_akcje.losowaLiczbaDoSprzedazy(spolka);
    }

    public boolean czyPosiada( Spolka spolka ) { return posiadane_akcje.czyPosiada(spolka); }

    public boolean czyStac( Zlecenie oferta, int cena ) { return gotowka >= oferta.liczbaSztuk() * cena; }

    // Mówi, czy posiadamy jeszcze dla spółki taką liczbę akcji, jaką zadeklarowaliśmy na zleceniu sprzedaży.
    public boolean czyJeszczeMozliwa( Spolka spolka, int liczba) { return posiadane_akcje.czyJeszczeMozliwa(spolka, liczba); }

    public void wypiszRezultat() {
        System.out.print(gotowka);
        posiadane_akcje.wypiszRezultat();
    }

    // Metoda potrzebna do testów.
    public int gotowka() { return gotowka; }

    // Metoda potrzebna do testów.
    public int liczbaAkcji( Spolka spolka ) { return posiadane_akcje.liczbaAkcji(spolka); }
}
