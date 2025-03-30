package gielda;

public class Akcje {
    private Spolka spolka;
    private int ile;

    public Akcje(Spolka spolka, int ile ) {
        this.spolka = spolka;
        this.ile = ile;
        spolka.dodajAkcje(ile);
    }

    //*************************************** METODY PUBLIC *******************************************//

    public Spolka spolka() { return spolka; }

    public int liczba() { return ile; }

    public void dodajLiczbe( int liczba ) { ile += liczba; }

    public void odejmijLiczbe( int liczba ) { ile -= liczba;; }

    public boolean equals( Akcje other ) {
        return spolka.equals( other.spolka );
    }
}
