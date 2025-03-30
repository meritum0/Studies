package tramwaje;

public class KoniecDnia extends Event {

    public KoniecDnia( Czas czas ) {
        super(czas);
    }

    /*********************** Metody public ************************/

    public void wykonaj() {
        System.out.println( "---------------------- Koniec Dnia " + czas().dzien() + " ---------------------------" );
    }
}
