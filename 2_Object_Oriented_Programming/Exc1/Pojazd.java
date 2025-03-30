package tramwaje;

public abstract class Pojazd {
    private final int nr_boczny;
    private final Linia linia;

    protected Pojazd( int nr_boczny, Linia linia ) {
        this.nr_boczny = nr_boczny;
        this.linia = linia;
    }

    /*********************** Metody protected ************************/

    protected int getNr_boczny() { return nr_boczny; }
    protected Linia getLinia() { return linia; }
}
