package gielda;

public class Natychmiastowe extends Zlecenie {
    public Natychmiastowe( boolean typ, String id, Inwestor inwestor, Spolka spolka, int liczba, int limit ) {
        super( typ, id, inwestor, spolka, liczba, limit );
    }

    //*************************************** METODY PUBLIC *******************************************//

    public boolean czyUsunac() {
        return true;
    }
}
