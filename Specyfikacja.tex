\documentclass{article}
\usepackage[utf8]{inputenc}
\usepackage[polish]{babel}
\usepackage{listings}
\usepackage{geometry}
\geometry{a4paper, margin=1in}

\title{Spiecyfikacja}
\author{Arseni Kiyko}
\date{April 2024}

\begin{document}

\maketitle

\section{Introduction}

\title{Specyfikacja Funkcjonalna Programu do Analizy Labiryntów}
\author{Arseni Kiyko}
\date{\today}

\begin{document}

\maketitle

\section{Cel Projektu}
Celem projektu jest stworzenie programu do analizy i nawigacji po labiryntach reprezentowanych w plikach tekstowych lub binarnych. Program umożliwia wyszukiwanie najkrótszej ścieżki od punktu startowego do wyjścia, a także eksportuje tę ścieżkę w formie tekstowej lub binarnej w zależności od wyboru użytkownika.

\section{Opis Funkcji}
Program oferuje następujące funkcjonalności:
\begin{itemize}
  \item Wczytywanie labiryntu z pliku tekstowego lub binarnego.
  \item Wyszukiwanie najkrótszej ścieżki przy użyciu algorytmu BFS (Breadth-First Search).
  \item Eksport znalezionej ścieżki do pliku tekstowego lub binarnego.
  \item Eksport struktury labiryntu do pliku binarnego.
\end{itemize}

\section{Ograniczenia}

Program został zaprojektowany do efektywnego przetwarzania i analizy labiryntów o maksymalnych wymiarach 200x200 jednostek. To ograniczenie wynika z kilku kluczowych czynników związanych z zarządzaniem pamięcią i złożonością obliczeniową algorytmów używanych do analizy labiryntów:

\begin{itemize}
    \item \textbf{Zarządzanie pamięcią:} Program wykorzystuje dynamiczną alokację pamięci do przechowywania struktury labiryntu oraz do tablic ścieżek i odległości używanych przez algorytm BFS. W przypadku labiryntów o wymiarach przekraczających 200x200, ilość wymaganej pamięci RAM gwałtownie rośnie, co może prowadzić do przeciążeń i problemów z wydajnością na systemach z ograniczoną ilością pamięci operacyjnej.
    
    \item \textbf{Złożoność obliczeniowa:} Algorytm BFS, który jest stosowany do znajdowania najkrótszej ścieżki w labiryncie, ma złożoność obliczeniową proporcjonalną do liczby wierzchołków i krawędzi w grafie. Dla labiryntu o wymiarach 200x200 liczba potencjalnych wierzchołków może osiągnąć 20 000, co przy gęstej sieci połączeń skutkuje znaczącymi wymaganiami obliczeniowymi. Większe labirynty zwiększają ryzyko znacznego spowolnienia czasu odpowiedzi programu.
    
    \item \textbf{Funkcje walidacji:} Funkcje sprawdzające poprawność danych wejściowych (np. czy labirynt nie zawiera niedozwolonych znaków, czy szerokość wszystkich wierszy jest jednolita) również stają się bardziej zasobożerne w miarę wzrostu wymiarów labiryntu. Skomplikowanie i zróżnicowanie danych wejściowych w dużych labiryntach zwiększa ryzyko błędów podczas analizy struktury labiryntu.
\end{itemize}

Z powyższych powodów, użytkownicy mogą doświadczyć problemów technicznych, takich jak długie czasy odpowiedzi lub nawet awarie programu, przy próbie załadowania i analizy labiryntów przekraczających zalecane wymiary. Zaleca się ograniczenie rozmiarów labiryntów do maksymalnie 200x200, aby zapewnić stabilność i efektywność działania programu.

\section{Dostępne Flagi Sterujące}
Program akceptuje następujące flagi i parametry:
\begin{description}
  \item[-f nazwa\_pliku.txt] Wczytanie labiryntu z pliku tekstowego.
  \item[-b nazwa\_pliku.bin] Wczytanie labiryntu z pliku binarnego.
  \item[-o nazwa\_pliku\_wyjściowego] Określenie nazwy pliku wyjściowego.
  \item[-h] Wyświetlenie informacji pomocniczych o użyciu programu.
\end{description}

\section{Formaty Danych}

\subsection{Plik Tekstowy}
Labirynt w pliku tekstowym jest reprezentowany jako siatka znaków, gdzie każdy znak odpowiada jednemu kwadratowi w labiryncie. Struktura pliku jest następująca:
\begin{itemize}
  \item \textbf{P} - punkt startowy. Oznacza miejsce, z którego użytkownik rozpoczyna nawigację.
  \item \textbf{K} - wyjście. Jest to cel, do którego użytkownik musi dotrzeć.
  \item \textbf{X} - ściany. Znaki te blokują przejście, reprezentując przeszkody w labiryncie.
  \item Spacja - ścieżka. Otwarte miejsca, przez które można się przemieszczać.
\end{itemize}
Plik powinien być zapisany w formacie UTF-8 bez znacznika kolejności bajtów (BOM). Każdy wiersz w pliku tekstowym reprezentuje jeden rząd labiryntu, a liczba znaków w wierszu odpowiada szerokości labiryntu. Wszystkie wiersze muszą mieć tę samą liczbę znaków, co zapewnia jednorodność struktury.

\subsection{Plik Binarny}
Plik binarny zawiera bardziej skompresowaną formę labiryntu, co pozwala na efektywniejsze przechowywanie i szybsze przetwarzanie dużych labiryntów:
\begin{itemize}
  \item \textbf{Dwa pierwsze bajty} określają wysokość labiryntu jako liczba bez znaku w formacie little-endian.
  \item \textbf{Kolejne dwa bajty} określają szerokość labiryntu, również w formacie little-endian.
  \item \textbf{Następne bajty} reprezentują strukturę labiryntu, gdzie każdy bajt odpowiada jednemu kwadratowi labiryntu:
    \begin{itemize}
      \item \texttt{0x50} ('P') - punkt startowy.
      \item \texttt{0x4B} ('K') - wyjście.
      \item \texttt{0x58} ('X') - ściana.
      \item \texttt{0x20} (spacja) - ścieżka.
    \end{itemize}
\end{itemize}
Plik nie zawiera żadnych separatorów pomiędzy wierszami, co oznacza, że cała struktura labiryntu jest zapisana jako ciągły strumień bajtów. Długość tego strumienia powinna być równa iloczynowi wysokości i szerokości labiryntu. Zastosowanie formatu binarnego minimalizuje ilość używanej przestrzeni dyskowej oraz zmniejsza czas potrzebny na wczytanie i przetworzenie danych.

Użytkownik powinien zapewnić, że plik binarny jest poprawnie przygotowany zgodnie z powyższymi specyfikacjami, aby program mógł prawidłowo wczytać i przetworzyć labirynt.

\section{Sytuacje Wyjątkowe}
Program przewiduje następujące sytuacje wyjątkowe:
\begin{itemize}
  \item Brak pliku wejściowego — program zwraca komunikat o błędzie i kończy działanie z kodem błędu 2.
  \item Nieprawidłowy format danych — zwraca stosowny komunikat o błędzie.
  \item Błąd alokacji pamięci — zwraca komunikat o błędzie i kończy działanie.
\end{itemize}

\section{Przykłady Użycia}
Przykład wczytywania labiryntu z pliku tekstowego i eksportu ścieżki do pliku tekstowego:
\begin{lstlisting}[language=bash]
./program -f labirynt.txt -o sciezka
Enter output type (1 for text file, 2 for binary file, 3 for maze in bin file): 1
\end{lstlisting}
Przykład wczytywania labiryntu z pliku binarnego i eksportu struktury labiryntu do nowego pliku binarnego:
\begin{lstlisting}[language=bash]
./program -b labirynt.bin -o nowy_labirynt
Enter output type (1 for text file, 2 for binary file, 3 for maze in bin file): 3
\end{lstlisting}

\end{document}
\end{document}
