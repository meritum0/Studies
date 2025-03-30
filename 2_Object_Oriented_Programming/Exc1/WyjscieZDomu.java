package tramwaje;

public class WyjscieZDomu extends Event {
    private final Pasazer pasazer;
    private final Przystanek przystanek;
    public WyjscieZDomu( Pasazer pasazer, Przystanek przystanek, Czas czas ) {
        super(czas);
        this.przystanek = przystanek;
        this.pasazer = pasazer;
    }

    /*********************** Metody public ************************/

    public void wykonaj() {
        if ( przystanek.czyMozna() ) {
            przystanek.dodajOczekujacego(pasazer);
            System.out.println( czas().dzien() + ", " + czas().godzina().toString() + ": Pasażer " + pasazer.id() +
                    " wyszedł z domu i czeka na przystanku " + przystanek.toString() );
        }
        else
            pasazer.zrezygnowal();
    }
}
