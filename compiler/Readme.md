Michał Krosny 256791
Kompilator prostego języka imperatywnego

W celu zbudowania należy wywołać polecenie:
make

W celu kompilacji programu należy wywołać polecenie:
./kompilator <nazwa pliku wejściowego> <nazwa pliku wyjściowego>




kompilator wykożystuje lexer generowany przez flex z pliku lexer.l
oraz parser bison generowany z pliku parser.y

Pliki w folderze ast odpowiadają za reprezentację programu w formie AST (Abstract Syntax Tree)
analogicznie w folderze rtl reprezentację programu w formie RTL (Register Transfer Language)
oraz prosty plik reprezentujący go w ALL (Assembly Level Language).

Kompilator posiada pliki allokatora rejestrów jednak ze względu na problemy implementacyjne został
on usunięty a pliki te zawierają rozwiązanie 'tymczasowe' które docelowo zostało by zastąpione realnym
allokatorem który przeprowadzał by kolorowanie rejestrów w obszarze bloków programu.

Jak widać projekt miął być napisany nieco inaczej i posiada wiele nieskończonych elementów (z czego wynika ilośc plików)
jednak ze względu na czas zostały zaimplementowane tymczasowe rozwiązania jedynie by kompilator działał poprawnie jednak
niekoniecznie efektywnie.