package gielda;

public class DoTury extends Zlecenie {
    private int tura_graniczna;
    private SystemTransakcyjny system;

    public DoTury( boolean typ, String id, Inwestor inwestor, Spolka spolka, int liczba, int limit, int tura_graniczna, SystemTransakcyjny system ) {
        super( typ, id, inwestor, spolka, liczba, limit );
        this.tura_graniczna = tura_graniczna;
        this.system = system;
    }

    //*************************************** METODY PUBLIC *******************************************//

    public boolean czyUsunac() {
        return ( super.liczbaSztuk() <= 0 || tura_graniczna <= system.tura() );
    }
}
