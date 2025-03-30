package gielda;

public class BrakSystemuTransakcyjnego extends RuntimeException {
    public BrakSystemuTransakcyjnego() {
        super( "Nie udało się utworzyć symulacji." );
    }
}
