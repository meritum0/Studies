package gielda;

public class Inwestor {
    private Strategia strategia;
    private Portfel portfel = new Portfel();

    public Inwestor( Strategia strategia ) {
        this.strategia = strategia;
    }

    //*************************************** METODY PUBLIC *******************************************//

    public void dodajGotowke( int liczba ) { portfel.dodajGotowke(liczba); }

    public void dodajAkcje( Akcje akcje, int cena ) {
        portfel.dodajAkcje( akcje, cena );
    }

    // Usuwa akcje z portfela i dodaje gotówkę, równą co do wartości usuniętym akcjom.
    public void odejmijAkcje( Akcje akcje, int cena ) { portfel.odejmijAkcje( akcje, cena ); }

    public boolean czyStac( Zlecenie oferta, int cena ) { return portfel.czyStac(oferta, cena); }

    // Mówi, czy posiadamy jeszcze w portfelu liczbę akcji, którą zadeklarowaliśmy, że chcemy sprzedać.
    public boolean czyJeszczeMozliwa( Spolka spolka, int liczba ) { return portfel.czyJeszczeMozliwa(spolka, liczba); }

    public void odpytaj( SystemTransakcyjny system ) {
            strategia.odpytaj( system, portfel, this );
    }

    public void wypiszRezultat() {
        System.out.print(strategia);
        portfel.wypiszRezultat();
    }

    // Metoda potrzebna do testów.
    public int gotowka() { return portfel.gotowka(); }

    // Metoda potrzebna do testów.
    public int liczbaAkcji( Spolka spolka ) { return portfel.liczbaAkcji(spolka); }
}
