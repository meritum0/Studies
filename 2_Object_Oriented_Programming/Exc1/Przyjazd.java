package tramwaje;

public class Przyjazd extends Event {
    private final Tramwaj tramwaj;
    private final Przystanek przystanek;

    public Przyjazd( Tramwaj tramwaj, Przystanek przystanek, Czas czas ) {
        super(czas);
        this.tramwaj = tramwaj;
        this.przystanek = przystanek;
    }

    /*********************** Metody public ************************/

    public void wykonaj() {
        System.out.println( czas().dzien() + ", " + czas().godzina().toString() + ": Tramwaj linii " + tramwaj.linia().nr() +
                            " (nr boczny " + tramwaj.nr_boczny() + ") przyjechał na przystanek " + przystanek.toString() );
        przystanek.poWymianie();
        tramwaj.wysiadanie( przystanek, czas() );
    }
}
