package gielda;

public abstract class Zlecenie implements Comparable<Zlecenie> {
    private final boolean typ;  // true - kupno, false - sprzedaż
    private final String id;
    private Inwestor inwestor;
    private final Spolka spolka;
    private int liczba_sztuk;
    private final int limit;

    public Zlecenie( boolean typ, String id, Inwestor inwestor, Spolka spolka, int liczba_sztuk, int limit ) {
        this.typ = typ;
        this.id = id;
        this.inwestor = inwestor;
        this.spolka = spolka;
        this.liczba_sztuk = liczba_sztuk;
        this.limit = limit;
    }

    //*************************************** METODY PUBLIC *******************************************//

    public Inwestor inwestor() { return inwestor; }

    public Spolka spolka() { return spolka; }

    public int liczbaSztuk() { return liczba_sztuk; }

    public int limit() { return limit; }

    // Na podstawie unikalnego id, przydzielanego w określonym porządku, stwierdza, które zlecenie powstało wcześniej.
    public int czyStarsze( Zlecenie other ) {
        if ( this.id.length() < other.id.length() )
            return 1;
        if ( this.id.length() > other.id.length() )
            return -1;

        int i = 0;
        while ( i < this.id.length() ) {
            if ( this.id.charAt(i) < other.id.charAt(i) )
                return 1;
            if ( this.id.charAt(i) > other.id.charAt(i) )
                return -1;
            i++;
        }

        return 0;
    }

    // Ktoś kupił od nas ileś akcji spółki.
    public void uaktualnij( int liczba ) { liczba_sztuk -= liczba; }

    // Ktoś sprzedał nam ileś akcji spółki.
    public void uaktualnij( int liczba, int cena ) {
        liczba_sztuk -= liczba;
        spolka.uaktualnij(liczba, cena);
    }

    public int compareTo( Zlecenie other ) {
        if ( this.limit > other.limit ) {
            if ( typ )  // kupno
                return -1;
            else        // sprzedaż
                return 1;
        }
        if ( this.limit < other.limit ) {
            if ( typ )
                return 1;
            else
                return -1;
        }

        return -this.czyStarsze(other);
    }

    public boolean czyStac( Zlecenie oferta, int cena ) { return inwestor.czyStac(oferta, cena); }

    // Czy posiadamy jeszcze tyle akcji danej spółki, ile zadklarowaliśmy, że chcemy sprzedać?
    public boolean czyJeszczeMozliwa() { return inwestor.czyJeszczeMozliwa(spolka, liczba_sztuk); }

    public abstract boolean czyUsunac();
}
