package tramwaje;

public class Odjazd extends Event {
    private final Tramwaj tramwaj;
    private final Przystanek przystanek;
    private final int idx;
    private final boolean kierunek;

    public Odjazd( Tramwaj tramwaj, Przystanek przystanek, int idx, boolean kierunek, Czas czas ) {
        super(czas);
        this.tramwaj = tramwaj;
        this.przystanek = przystanek;
        this.idx = idx;
        this.kierunek = kierunek;
    }

    /*********************** Metody public ************************/

    public void wykonaj() {
        tramwaj.wsiadanie( przystanek, idx, kierunek, czas() );
    }
}
