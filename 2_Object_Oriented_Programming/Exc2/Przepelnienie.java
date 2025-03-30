package gielda;

public class Przepelnienie extends RuntimeException {
    public Przepelnienie() {
        super( "Przepełnienie symulacji, skończyły się unikalne kody zleceń" );
    }
}
