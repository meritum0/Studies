package gielda;

public class BezTerminu extends Zlecenie {
    public BezTerminu( boolean typ, String id, Inwestor inwestor, Spolka spolka, int liczba, int limit ) {
        super( typ, id, inwestor, spolka, liczba, limit );
    }

    //*************************************** METODY PUBLIC *******************************************//

    public boolean czyUsunac() {
        return super.liczbaSztuk() <= 0;
    }
}
